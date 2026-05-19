#include "data_controller.h"
#include "mcm_data.h"
#include "mcm_struct.h"
#include <QByteArray>
#include <QDebug>

DataController::DataController(QObject *parent) :
    QThread(parent)
{

}

void DataController::run()
{
    //    char buffer[1500];
    //    qDebug()<<"address of DataController"<<this;
    mo_mcmData = McmData::getMcmDataInstance();
    mo_mdmData = new MdmDataContrl;
    mcsock_mdm = new McastSender;

    ms_logout = new STRUCT_MESSAGE_HEADER;
    ms_logout->source_id = MCM_ID;
    ms_logout->dest_id = MDM_ID;
    ms_logout->msg_id = ADMIN_LOGOUT_FROM_MCM_MSG_ID;
    ms_logout->msg_len = sizeof(STRUCT_MESSAGE_HEADER);

    QByteArray bytArrBuf;
    //    qDebug()<<"now in removing  run";
    while(1)
    {
        while (!mo_mcmData->msgList.isEmpty())
        {
            //            qDebug()<<"now in removing";
            bytArrBuf =  mo_mcmData->msgList.takeFirst();
            filterRecvMsg(bytArrBuf.data(), bytArrBuf.size());
            //            qDebug()<<"byte size "<<bytArrBuf.size();
            //            memset(buffer, NULL, 1500);
            //            memcpy(buffer, bytArrBuf.data(), bytArrBuf.size());
            //            STRUCT_MPIO_10S_CARD_DETAILS *s_card = (STRUCT_MPIO_10S_CARD_DETAILS *) buffer;
            //            qDebug()<<s_card->card_config[9].ip_addr;
            //            qDebug()<<"source"<<s_card->msg_hdr.source_id;

        }
        msleep(10);
    }

}

void DataController::filterRecvMsg(char *buffer, int msgLen)
{
//        printf(" bytes received in MCM %d \n", msgLen);
//        for(int i =0; i < msgLen ; i++)
//        {
//            printf(" %d--> %d  ",i+1, buffer[i]);
//        }
//        fflush(stdout);
    STRUCT_MESSAGE_HEADER *msghdr = (STRUCT_MESSAGE_HEADER *) buffer;
    if(msghdr->msg_id == LOGOUT_MSG_ID)
    {
        pthread_mutex_trylock (&mo_mcmData->mutex_admin);
        if(mo_mcmData->m_adminState == 1)
        {
            mo_mcmData->m_adminState = 0;
            qDebug()<<"logout request received for ADMIN";
        }

        pthread_mutex_unlock(&mo_mcmData->mutex_admin);
    }
    else if(msghdr->msg_id == ADMIN_MSG_ID)
    {
        pthread_mutex_trylock (&mo_mcmData->mutex_admin);
        //        qDebug()<<"admin hlth came";
        mo_mcmData->m_adminCnt = 0;
        mo_mcmData->m_adminState = 1;
        pthread_mutex_unlock(&mo_mcmData->mutex_admin);
    }

    else if(msghdr->source_id == MDM_ID && msghdr->dest_id == MCM_ID)
    {
        pthread_mutex_trylock (&mo_mcmData->mutex_hlth);
        if(mo_mcmData->m_currentState == 1)
        {

            //            printf("check msg source %d \n",msghdr->source_id);
            //            printf("check msg dest %d \n",msghdr->dest_id);
            processMsgFromMdm(buffer, msgLen);
        }
        pthread_mutex_unlock(&mo_mcmData->mutex_hlth);


    }
    else if(msghdr->source_id == DADM_ID && ((msghdr->dest_id == MCM_ID) || (msghdr->dest_id == MDM_ID)))
    {
        //        printf(" msg id is %d \n", msghdr->msg_id);
        if(msghdr->msg_id == NODE_STATUS_MSG_ID)
            objPrint.EqptStatusEqpt(buffer, msgLen);
        pthread_mutex_trylock (&mo_mcmData->mutex_hlth);
        if(mo_mcmData->m_currentState == 1)
        {
            processMsgFromDadm(buffer, msgLen);
        }
        pthread_mutex_unlock(&mo_mcmData->mutex_hlth);


    }
    else if(msghdr->source_id == MCM_ID && msghdr->dest_id == MCM_ID)
    {
        processMsgFromMcm(buffer, msgLen);
    }


}

void DataController::processMsgFromMcm(char *buffer, int msgLen)
{
    m_sendFilestate = false;

    STRUCT_MCM_HEALTH_MSG *ms_hlt = (STRUCT_MCM_HEALTH_MSG *)buffer;
    if(mo_mcmData->m_selfState != ms_hlt->mcm_state)
    {
        pthread_mutex_trylock (&mo_mcmData->mutex_hlth);
        mo_mcmData->m_mcmCnt = 0;

        if(ms_hlt->current_state == 1)
        {
            if(mo_mcmData->m_currentState == 1)
            {
                if(mo_mcmData->m_epochTime < ms_hlt->epoch_sec)//replace as per new  mo_mcmData->m_epochTime < ms_hlt->epoch_sec
                {
                    mo_mcmData->m_currentState = 0;
                    mo_mcmData->mo_mcmCntrol->ms_hlt->mcm_state = 0;
                    qDebug()<<"I am going to passive mode";
                }
                else if(mo_mcmData->m_epochTime > ms_hlt->epoch_sec)
                {
                    if(mo_mcmData->m_selfState == 1)
                    {
                        if(ms_hlt->epoch_sec != mo_mcmData->m_epochTime)
                        {
                            m_sendFilestate = true;
                            qDebug()<<" both network came sending file to master 2";
                            mo_mcmData->mo_dadmSender->updateMcm2();
                            mcsock_mdm->sendDatagram((unsigned char *)ms_logout, sizeof(STRUCT_MESSAGE_HEADER));
                        }

                    }
                    else if(mo_mcmData->m_selfState == 2)
                    {
                        if(ms_hlt->epoch_sec != mo_mcmData->m_epochTime)
                        {
                            m_sendFilestate = true;
                            qDebug()<<" both network came  sending file to master 1";
                            mo_mcmData->mo_dadmSender->updateMcm1();
                            mcsock_mdm->sendDatagram((unsigned char *)ms_logout, sizeof(STRUCT_MESSAGE_HEADER));
                        }

                    }
                }
                else
                {
                    if(mo_mcmData->m_selfState == 2)
                    {
                        mo_mcmData->m_currentState = 0;
                        mo_mcmData->mo_mcmCntrol->ms_hlt->mcm_state = 0;
                        qDebug()<<"I am going to passive mode";
                    }
                }
            }
            else
            {
                mo_mcmData->m_anotherAdmin = 1;
            }
        }
        if(ms_hlt->epoch_sec != mo_mcmData->m_epochTime)
        {
            qDebug()<<"received time"<<ms_hlt->epoch_sec<< "self time"<<mo_mcmData->m_epochTime;
            if(mo_mcmData->m_currentState == 1)
            {
                if(ms_hlt->current_state != 1)
                {
                    //send file to MCM
                    if(m_sendFilestate == false)
                    {
                        if(mo_mcmData->m_selfState == 1)
                        {
                            qDebug()<<" 111sending file to master 2";
                            mo_mcmData->mo_dadmSender->updateMcm2();
                        }
                        else if(mo_mcmData->m_selfState == 2)
                        {
                            qDebug()<<" 222sending file to master 1";
                            mo_mcmData->mo_dadmSender->updateMcm1();
                        }
                    }

                }
//                if(mo_mcmData->m_selfState == 1)
//                {
//                    if(ms_hlt->current_state == 1)
//                    {
//                        //send file to MCM
//                        qDebug()<<" 333sending file to master ------  2";
//                        mo_mcmData->mo_dadmSender->updateMcm2();
//                    }

//                }

            }

        }
        pthread_mutex_unlock(&mo_mcmData->mutex_hlth);

    }

}

void DataController::processMsgFromDadm(char *buffer, int msgLen)
{
    STRUCT_MESSAGE_HEADER *msghdr = (STRUCT_MESSAGE_HEADER *) buffer;
    switch(msghdr->msg_id)
    {
    case NODE_STATUS_MSG_ID:

        objPrint.EqptStatusEqpt(buffer, msgLen);


        setCurrentEquipmentState(buffer, msgLen);
        emit mpioStatus(buffer);


        break;
    case CARD_STATUS_MSG_ID:
    {
        mcsock_mdm->sendDatagram((unsigned char*) buffer, msgLen);
        STRUCT_MPIO_CARD_STATUS_MSG * msg = (STRUCT_MPIO_CARD_STATUS_MSG *)buffer;
        if(msg->mpioId == 1)
        {
            pthread_mutex_trylock (&mo_mcmData->mutex_mpioA1);
            mo_mcmData->m_cntMpioA1 = 0;
//            qDebug()<<" mpio A1 card status received ";
            pthread_mutex_unlock(&mo_mcmData->mutex_mpioA1);

        }
        else if(msg->mpioId == 2)
        {
            pthread_mutex_trylock (&mo_mcmData->mutex_mpioA2);
            mo_mcmData->m_cntMpioA2 = 0;
            pthread_mutex_unlock(&mo_mcmData->mutex_mpioA2);
        }
        else if(msg->mpioId == 3)
        {
            pthread_mutex_trylock (&mo_mcmData->mutex_mpioB);
            mo_mcmData->m_cntMpioB = 0;
            pthread_mutex_unlock(&mo_mcmData->mutex_mpioB);
        }
        else if(msg->mpioId == 4)
        {
            pthread_mutex_trylock (&mo_mcmData->mutex_mpioC);
            mo_mcmData->m_cntMpioC = 0;
            pthread_mutex_unlock(&mo_mcmData->mutex_mpioC);
        }
        else if(msg->mpioId == 5)
        {
            pthread_mutex_trylock (&mo_mcmData->mutex_mpioJ1);
            mo_mcmData->m_cntMpioJ1 = 0;
            pthread_mutex_unlock(&mo_mcmData->mutex_mpioJ1);
        }
        else if(msg->mpioId == 6)
        {
            pthread_mutex_trylock (&mo_mcmData->mutex_mpioJ2);
            mo_mcmData->m_cntMpioJ2 = 0;
            pthread_mutex_unlock(&mo_mcmData->mutex_mpioJ2);
        }

        //        objPrint.CardStatus(buffer, msgLen);



        //        printf("card status send mesg \n");
        //        objPrint.CardStatus(buffer, msgLen);
    }

        break;
    default:
        qDebug()<<"wrong msg id received from MCM";
        break;
    }
}

void DataController::setCurrentEquipmentState(char *buffer, int msgLen)
{
    STRUCT_MPIO_STATUS_MSG *s_mpio = (STRUCT_MPIO_STATUS_MSG *)buffer;
//        printf(" epoch time receievd is %d and stored time is %d \n",s_mpio->epoch_sec, mo_mcmData->m_epochTime);
    if(s_mpio->epoch_sec == mo_mcmData->m_epochTime)
    {
        pthread_mutex_trylock(&mo_mcmData->mutex_mpio[s_mpio->mpio_id]);
        if(mo_mcmData->ms_eqptInfo[s_mpio->mpio_id].no_of_eqpt > 0)
        {
            gettimeofday(&curSecond, NULL);
            mo_mcmData->m_timeMpio[s_mpio->mpio_id] = curSecond.tv_sec;
                        printf(" current time is set as %d for mpio id %d \n",mo_mcmData->m_timeMpio[s_mpio->mpio_id], s_mpio->mpio_id);
            for(int i = 0; i < MAX_EQPT; i++)
            {

                if(s_mpio->eqpt[i].eqpt_status >  0)
                {
                    mo_mcmData->ms_local->eqpt[i].eqpt_status = s_mpio->eqpt[i].eqpt_status;
                    mo_mcmData->ms_local->eqpt[i].eqpt_id = s_mpio->eqpt[i].eqpt_id;
                                       printf(" eqpt index %d set as status %d  \n",i , mo_mcmData->ms_local->eqpt[i].eqpt_id);
                                       fflush(stdout);
                }
                else
                {
                    //                    printf(" eqpt index %d set as 0 \n",i );
                    //                    fflush(stdout);
                    //                    mo_mcmData->ms_local->eqpt[i].eqpt_status = 0;
                    //                    mo_mcmData->ms_local->eqpt[i].eqpt_id = 0;
                }
            }
        }

        pthread_mutex_unlock(&mo_mcmData->mutex_mpio[s_mpio->mpio_id]);
    }
    else
    {
        //send updated file
        printf(" config file mismatch for mpio %d \n", s_mpio->mpio_id);
        fflush(stdout);
        mo_mcmData->mo_dadmSender->updateMpio(s_mpio->mpio_id);

    }
    fflush(stdout);
}

void DataController::processMsgFromMdm(char *buffer, int msgLen)
{
    STRUCT_MESSAGE_HEADER *msghdr = (STRUCT_MESSAGE_HEADER *) buffer;
    //    printf("now ***msg source %d \n",msghdr->source_id);
    switch(msghdr->msg_id)
    {
    case CONFIG_MPIO_MSG_ID:
        mo_mdmData->processMpio10SConfigMsg(buffer, msgLen);
        break;
    case INIT_MPIO_MSG_ID:
        mo_mdmData->processInitMpioMsg(buffer, msgLen);
        break;
    case ADD_EQPT_MSG_ID:
        objPrint.printAddEqpt(buffer, msgLen);
        mo_mdmData->processAddEqpt(buffer, msgLen);
        break;
    case DELETE_EQPT_MSG_ID:
//        objPrint.printAddEqpt(buffer, msgLen);
        mo_mdmData->processDeleteEqpt(buffer, msgLen);
//        objPrint.printAddEqpt(buffer, msgLen);
        break;
    case UPDATE_ALL_MSG_ID:
        mo_mcmData->mo_dadmSender->updateAllMpios();
        break;
    case LIVE_DATA_MSG_ID:
//        printf("dlive data mesg recvd \n");
//        objPrint.liveDataEqpt(buffer, msgLen);
        mo_mcmData->mo_dadmSender->sendLiveDataReqToDadm(buffer, msgLen);
//        printf("dlive data mesg send \n");
//        objPrint.liveDataEqpt(buffer, msgLen);
        break;
    default:
        qDebug()<<"wrong msg id received "<<msghdr->msg_id;
        break;
    }
}

void DataController::runThread()
{
    start();
}
