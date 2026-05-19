#include "mdm_data_contrl.h"
#include "mcm_struct.h"
#include "mcm_data.h"
#include <QDebug>


MdmDataContrl::MdmDataContrl(QObject *parent) :
    QObject(parent)
{
    mo_mcmData = McmData::getMcmDataInstance();
    mo_xml = new XmlReadWrite;
    mcscok_mdm = new McastSender;
    connect(mo_mcmData->mo_dadmSender,SIGNAL(updateMpioComplete()), this, SLOT(sendUpdateResponse()));
}

void MdmDataContrl::sendUpdateResponse()
{
    STRUCT_MESSAGE_HEADER *s_update = new STRUCT_MESSAGE_HEADER;
    s_update->source_id = MCM_ID;
    s_update->dest_id =   MDM_ID;
    s_update->msg_id =    UPDATE_ALL_REPLY_MSG_ID;
    s_update->msg_len =   sizeof(STRUCT_MESSAGE_HEADER);

    mcscok_mdm->sendDatagram((unsigned char*)s_update, sizeof(STRUCT_MESSAGE_HEADER));

    delete s_update;
    s_update = NULL;
}


void MdmDataContrl::processMpio10SConfigMsg(char *buffer, int msgLen)
{
    STRUCT_MPIO_INIT_DETAIL_MSG *s_mpio = (STRUCT_MPIO_INIT_DETAIL_MSG *) buffer;
    int mpio_id = s_mpio->mpio_id;
    if(s_mpio->mpio.mpio_type == MPIO_TYPE_10S)
    {
        if(mo_mcmData->mst_mpio[mpio_id].config_status == 0)
        {
            {
                QDebug debug(QtDebugMsg);
                for(int i =0; i < msgLen; i++)
                {
                    {

                        debug<< i+1<<"-->"<<(int) buffer[i]<< "  ";

                    }
                }

            }

            qDebug()<<" ip slot 6"<<s_mpio->mpio.mpio_is.mpio_10s.cc_ip[5];

            mf_wrtMpio10Crd = mo_xml->writeMpio10scardDetails(s_mpio);
            if(mf_wrtMpio10Crd == true)
            {
                storeMpio10SDetails(s_mpio);
                s_mpio->msg_hdr.source_id = MCM_ID;
                s_mpio->msg_hdr.source_id = MDM_ID;
                s_mpio->msg_hdr.msg_id = CONFIG_MPIO_SUCCESS_MSG_ID;
                mcscok_mdm->sendDatagram((unsigned char*)s_mpio, sizeof(STRUCT_MPIO_INIT_DETAIL_MSG));

                for(int i = 1; i < MAX_MPIO + 1; i++)
                {
                    for(int j = 0; j < mo_mcmData->ms_eqptInfo[i].no_of_eqpt; j++)
                    {
                        qDebug()<<" eqpt index "<< mo_mcmData->ms_eqptInfo[i].eqpt[j] << " at mpio id "<< i;
                    }
                }

            }
        }
        else
        {
            STRUCT_MPIO_CONFIG_ERROR_MSG * msgErr = new STRUCT_MPIO_CONFIG_ERROR_MSG;
            msgErr->msg_hdr.source_id = MCM_ID;
            msgErr->msg_hdr.dest_id = MDM_ID;
            msgErr->msg_hdr.msg_id = CONFIG_MPIO_ERROR_MSG_ID;
            msgErr->msg_hdr.msg_len = sizeof(STRUCT_MPIO_CONFIG_ERROR_MSG);
            msgErr->mpio_id = mpio_id;
            mcscok_mdm->sendDatagram((unsigned char*)msgErr, sizeof(STRUCT_MPIO_CONFIG_ERROR_MSG));
            delete msgErr;
            msgErr = NULL;
            qDebug()<<"mpio id "<< mpio_id << "  already configured";
        }
    }

}

void MdmDataContrl::processAddEqpt(char *buffer, int msgLen)
{

    STRUCT_MPIO_EDIT_DETAIL_MSG * s_edit = (STRUCT_MPIO_EDIT_DETAIL_MSG *) buffer;

    char mpioId = s_edit->mpio_id;
    char mpioType = s_edit->mpio_type;
    char slotNo = s_edit->slot_no;
    char portNo = s_edit->port_no;
    int eqptId = s_edit->eqpt.eqpt_id;
    char baud = s_edit->eqpt.baudrate;
    char par = s_edit->eqpt.parity;
    char stop = s_edit->eqpt.stopbit;
    if(mo_mcmData->mst_mpio[mpioId].config_status == 1)
    {
        if(mpioType == MPIO_TYPE_10S)
        {
            if(s_edit->interface == SERIAL_INTERFACE)
            {
                if(mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_card[slotNo - 5][portNo -1].eqpt_id ==
                    0)
                {
                    bool addStatus = mo_xml->addEqptDetail(s_edit);
                    if(addStatus == true)
                    {
                        s_edit->msg_hdr.source_id = MCM_ID;
                        s_edit->msg_hdr.dest_id = MDM_ID;
                        s_edit->msg_hdr.msg_id = ADD_EQPT_SUCCESS_MSG_ID;
                        if(mo_mcmData->mst_mpio[mpioId].mpio_type == MPIO_TYPE_10S)
                        {
                            if(slotNo > 4) //changed 5 to 4
                            {
                                mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_card[slotNo - 5][portNo -1].eqpt_id = eqptId;
                                mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_card[slotNo - 5][portNo -1].baudrate = baud;
                                mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_card[slotNo - 5][portNo -1].parity = par;
                                mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_card[slotNo - 5][portNo -1].stopbit = stop;
                            }

                        }
                        mcscok_mdm->sendDatagram((unsigned char*)s_edit, sizeof(STRUCT_MPIO_EDIT_DETAIL_MSG));
                        for(int i = 1; i < MAX_MPIO + 1; i++)
                        {
                            for(int j = 0; j < mo_mcmData->ms_eqptInfo[i].no_of_eqpt; j++)
                            {
                                qDebug()<<" eqpt index "<< mo_mcmData->ms_eqptInfo[i].eqpt[j] << " at mpio id "<< i;
                            }
                        }
                    }
                }
            }
            else if(s_edit->interface == DSRC_INTERFACE ||
                     s_edit->interface == SRDC_INTERFACE ||
                     s_edit->interface == DISCRETE0_INTERFACE ||
                     s_edit->interface == DISCRETE1_INTERFACE ||
                     s_edit->interface == DISCRETE2_INTERFACE)
            {
                if(mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.sr_dr_card[portNo -1].eqpt_id == 0)
                {
                    bool addStatus = mo_xml->addEqptDetail(s_edit);
                    if(addStatus == true)
                    {
                        s_edit->msg_hdr.source_id = MCM_ID;
                        s_edit->msg_hdr.dest_id = MDM_ID;
                        s_edit->msg_hdr.msg_id = ADD_EQPT_SUCCESS_MSG_ID;
                        if(mo_mcmData->mst_mpio[mpioId].mpio_type == MPIO_TYPE_10S)
                        {
                            if(slotNo == 3)
                            {
                                mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.sr_dr_card[portNo -1].eqpt_id = eqptId;
                            }

                        }
                        mcscok_mdm->sendDatagram((unsigned char*)s_edit, sizeof(STRUCT_MPIO_EDIT_DETAIL_MSG));
                        for(int i = 1; i < MAX_MPIO + 1; i++)
                        {
                            for(int j = 0; j < mo_mcmData->ms_eqptInfo[i].no_of_eqpt; j++)
                            {
                                qDebug()<<" eqpt index "<< mo_mcmData->ms_eqptInfo[i].eqpt[j] << " at mpio id "<< i;
                            }
                        }

                    }

                }
            }
            ///newly added please check

        }
    }

}

void MdmDataContrl::processDeleteEqpt(char *buffer, int msgLen)
{
    STRUCT_MPIO_EDIT_DETAIL_MSG * s_edit = (STRUCT_MPIO_EDIT_DETAIL_MSG *) buffer;
    char mpioId = s_edit->mpio_id;
    char mpioType = s_edit->mpio_type;
    char slotNo = s_edit->slot_no;
    char portNo = s_edit->port_no;
    int eqptId = s_edit->eqpt.eqpt_id;

    if(mo_mcmData->mst_mpio[mpioId].config_status == 1)
    {
        if(mpioType == MPIO_TYPE_10S)
        {
            if(s_edit->interface == SERIAL_INTERFACE)
            {
                if(mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_card[slotNo - 5][portNo -1].eqpt_id ==
                    eqptId)
                {
                    bool addStatus = mo_xml->addEqptDetail(s_edit);
                    if(addStatus == true)
                    {
                        s_edit->msg_hdr.source_id = MCM_ID;
                        s_edit->msg_hdr.dest_id = MDM_ID;
                        s_edit->msg_hdr.msg_id = DELETE_EQPT_SUCCESS_MSG_ID;

                        if(mo_mcmData->mst_mpio[mpioId].mpio_type == MPIO_TYPE_10S)
                        {
                            if(slotNo > 4) //changed 5 to 4
                            {
                                mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_card[slotNo - 5][portNo -1].eqpt_id = 0;
                                mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_card[slotNo - 5][portNo -1].baudrate = 0;
                                mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_card[slotNo - 5][portNo -1].parity = 0;
                                mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_card[slotNo - 5][portNo -1].stopbit = 0;
                            }

                        }
                        qDebug()<<"send delete msg reply success";
                        mcscok_mdm->sendDatagram((unsigned char*)s_edit, sizeof(STRUCT_MPIO_EDIT_DETAIL_MSG));
                        for(int i = 1; i < MAX_MPIO + 1; i++)
                        {
                            for(int j = 0; j < mo_mcmData->ms_eqptInfo[i].no_of_eqpt; j++)
                            {
                                qDebug()<<" eqpt index "<< mo_mcmData->ms_eqptInfo[i].eqpt[j] << " at mpio id "<< i;
                            }
                        }
                    }
                }
            }
            else if(s_edit->interface == DSRC_INTERFACE ||
                     s_edit->interface == SRDC_INTERFACE ||
                     s_edit->interface == DISCRETE0_INTERFACE ||
                     s_edit->interface == DISCRETE1_INTERFACE ||
                     s_edit->interface == DISCRETE2_INTERFACE )
            {
                if(mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.sr_dr_card[portNo - 1].eqpt_id ==
                    eqptId)
                {
                    bool addStatus = mo_xml->addEqptDetail(s_edit);
                    if(addStatus == true)
                    {
                        s_edit->msg_hdr.source_id = MCM_ID;
                        s_edit->msg_hdr.dest_id = MDM_ID;
                        s_edit->msg_hdr.msg_id = DELETE_EQPT_SUCCESS_MSG_ID;

                        if(mo_mcmData->mst_mpio[mpioId].mpio_type == MPIO_TYPE_10S)
                        {
                            if(slotNo == 3)
                            {
                                mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.sr_dr_card[portNo - 1].eqpt_id = 0;
                            }

                        }
                        qDebug()<<"send delete msg reply success";
                        mcscok_mdm->sendDatagram((unsigned char*)s_edit, sizeof(STRUCT_MPIO_EDIT_DETAIL_MSG));
                        for(int i = 1; i < MAX_MPIO + 1; i++)
                        {
                            for(int j = 0; j < mo_mcmData->ms_eqptInfo[i].no_of_eqpt; j++)
                            {
                                qDebug()<<" eqpt index "<< mo_mcmData->ms_eqptInfo[i].eqpt[j] << " at mpio id "<< i;
                            }
                        }
                    }
                }
            }

        }
    }


}

void MdmDataContrl::storeMpio10SDetails(STRUCT_MPIO_INIT_DETAIL_MSG *s_mpio)
{
    int mpio_id = s_mpio->mpio_id;
    memcpy(&mo_mcmData->mst_mpio[mpio_id],&s_mpio->mpio, sizeof(STRUCT_MPIO_CONFIG_DETAILS));

    mo_mcmData->mst_mpio[mpio_id].config_status = 1;
    mo_mcmData->mst_mpio[mpio_id].mpio_type == MPIO_TYPE_10S;
}

void MdmDataContrl::processInitMpioMsg(char *buffer, int msgLen)
{
    bool findUser = false;
    bool passErr = false;
    int usrNo;
    STRUCT_MPIO_INIT_MSG *s_init = (STRUCT_MPIO_INIT_MSG*)buffer;
    //    qDebug()<<"usr name received "<<(char*)s_init->user_name;
    //    for(usrNo =0; usrNo < MAX_USR_ACT; usrNo++)
    //    {
    ////        qDebug()<<"usr name stored "<<(char*)mo_mcmData->mst_usr->usrname[usrNo];
    //        if(mo_mcmData->mst_usr->is_usr[usrNo] == 1)
    //        {
    //            if(!(strcmp((char*)mo_mcmData->mst_usr->usrname[usrNo],(char*)s_init->user_name)))
    //            {
    //                if(!(strcmp((char*)mo_mcmData->mst_usr->pass[usrNo],(char*)s_init->password)))
    //                {
    //                    qDebug()<<"correct usr name received with correct password "<<(char*)s_init->user_name;
    //                }
    //                else
    //                {
    //                   passErr = true;
    //                }
    //                findUser = true;
    //                break;
    //            }
    //        }
    //    }

    if(s_init->user_mode == ADMIN_MODE)
    {
        qDebug()<<"admin request came";
        if(mo_mcmData->m_adminState == 0)
        {
            qDebug()<<" admin request accepeted";
            replyInitMpioSuccess(s_init->user_mode, s_init->mdm_id);
            mo_mcmData->m_adminState = 1;
        }
        else
        {
            qDebug()<<" admin request rejected";
            replyRejectAdminMode(s_init->mdm_id);
        }

    }
    else if(s_init->user_mode == USER_MODE)
    {
        qDebug()<<"USER request came";
        replyInitMpioSuccess(s_init->user_mode,s_init->mdm_id);
    }

}

void MdmDataContrl::replyRejectAdminMode(int mdmId)
{
    STRUCT_MPIO_INIT_REPLY_MSG *s_initReply = new STRUCT_MPIO_INIT_REPLY_MSG;
    s_initReply->msg_hdr.source_id = MCM_ID;
    s_initReply->msg_hdr.dest_id = MDM_ID;
    s_initReply->msg_hdr.msg_id = INIT_MPIO_SUCCESS_ID;
    s_initReply->msg_hdr.msg_len = sizeof(STRUCT_MPIO_INIT_REPLY_MSG);

    s_initReply->usr_mode = 3;
    s_initReply->mdm_id = mdmId;

    s_initReply->no_of_mpio = 0;
    mcscok_mdm->sendDatagram((unsigned char*)s_initReply, sizeof(STRUCT_MPIO_INIT_REPLY_MSG));
    delete s_initReply;
    s_initReply = NULL;
}


void MdmDataContrl::replyInitMpioSuccess(int mode, int mdmId)
{


    STRUCT_MPIO_INIT_REPLY_MSG *s_initReply = new STRUCT_MPIO_INIT_REPLY_MSG;
    STRUCT_MPIO_INIT_DETAIL_MSG *s_mpio = new STRUCT_MPIO_INIT_DETAIL_MSG;
    s_initReply->msg_hdr.source_id = MCM_ID;
    s_initReply->msg_hdr.dest_id = MDM_ID;
    s_initReply->msg_hdr.msg_id = INIT_MPIO_SUCCESS_ID;
    s_initReply->msg_hdr.msg_len = sizeof(STRUCT_MPIO_INIT_REPLY_MSG);

    s_initReply->usr_mode = mode;
    s_initReply->mdm_id = mdmId;
    qDebug()<<"admin id is "<<s_initReply->mdm_id;

    int noOfMpio =0;
    for(int i =0; i < 20; i++)
    {
        if(mo_mcmData->mst_mpio[i].config_status == true)
        {
            noOfMpio++;
        }
    }
    s_initReply->no_of_mpio = noOfMpio;
    mcscok_mdm->sendDatagram((unsigned char*)s_initReply, sizeof(STRUCT_MPIO_INIT_REPLY_MSG));
    delete s_initReply;
    s_initReply = NULL;
    //    msleep(100);

    for(int i =0; i < 20; i++)
    {
        if(mo_mcmData->mst_mpio[i].config_status == 1)
        {
            printf(" current Mpio %d and complete no of Mpio %d \n",i,noOfMpio);
            fflush(stdout);
            memset(s_mpio, NULL,sizeof(STRUCT_MPIO_INIT_DETAIL_MSG));
            s_mpio->msg_hdr.source_id = MCM_ID;
            s_mpio->msg_hdr.dest_id = MDM_ID;
            s_mpio->msg_hdr.msg_id = INIT_MPIO_DETAIL_ID;
            s_mpio->msg_hdr.msg_len = sizeof(STRUCT_MPIO_INIT_DETAIL_MSG);
            s_mpio->mpio_id = i;
            s_mpio->mdm_id = mdmId;
            memcpy(&s_mpio->mpio, &mo_mcmData->mst_mpio[i], sizeof(STRUCT_MPIO_CONFIG_DETAILS));
            mcscok_mdm->sendDatagram((unsigned char*)s_mpio, sizeof(STRUCT_MPIO_INIT_DETAIL_MSG));
            usleep(10);
        }
    }

    delete s_mpio;
    s_mpio = NULL;

}

void MdmDataContrl::processCmMsg(char *buffer, int msgLen)
{
    //    STRUCT_LIVE_DATA_MSG *ms_liveData = (STRUCT_LIVE_DATA_MSG *) buffer;

}
