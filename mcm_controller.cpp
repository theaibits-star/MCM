#include "mcm_controller.h"
#include "mcm_data.h"
#include <QDebug>

McmController::McmController(QObject *parent) :
    QObject(parent)
{
    mb_firstTime = false;
    mb_timer = false;
    m_localCnt = 0;
    m_hlthOnStartup = 0;
    m_master2Cnt =0;
    mtm_mcmState = new QTimer(this);
    mo_mcmData = McmData::getMcmDataInstance();
    ms_hlt = new STRUCT_MCM_HEALTH_MSG;
    ms_hlt->msg_hdr.source_id = MCM_ID;
    ms_hlt->msg_hdr.dest_id =  MCM_ID;
    ms_hlt->msg_hdr.msg_id = MCM_HEALTH_MSG_ID;
    ms_hlt->msg_hdr.msg_len = sizeof(STRUCT_MCM_HEALTH_MSG);
    ms_hlt->mcm_state = mo_mcmData->m_selfState;
    ms_hlt->current_state = -1;
    connect(mtm_mcmState, SIGNAL(timeout()), this, SLOT(startMcmStateTimer()));

}

void McmController::startMcmStateTimer()
{
    pthread_mutex_trylock (&mo_mcmData->mutex_admin);

    if(mo_mcmData->m_adminState == 1)
    {
        mo_mcmData->m_adminCnt++;
        if(mo_mcmData->m_adminCnt > 4)
        {
            mo_mcmData->m_adminState = 0;
            qDebug()<<" MDM ADMIN is down so resetting the admin";

        }
    }


    if(mo_mcmData->m_adminCnt > 100000000)//to avoid infinite increments
    {
        mo_mcmData->m_adminCnt = 50;
    }

    pthread_mutex_unlock(&mo_mcmData->mutex_admin);


     pthread_mutex_trylock (&mo_mcmData->mutex_hlth);

//     qDebug()<<"mcmcnt"<<mo_mcmData->m_mcmCnt;
    if(m_hlthOnStartup < 3)//for startup master 1 will send first 3 packets
    {
        if(mo_mcmData->m_selfState == 1)
        {
            msock_mcm.sendudpMessage((unsigned char *)ms_hlt, sizeof(STRUCT_MCM_HEALTH_MSG),MCM_S_IP,8005);
            mo_mcmData->m_mcmCnt++;
        }
        m_hlthOnStartup++;

    }
    else
    {
        if(mo_mcmData->m_selfState == 1)
        {
            msock_mcm.sendudpMessage((unsigned char *)ms_hlt, sizeof(STRUCT_MCM_HEALTH_MSG),MCM_S_IP,8005);
        }
        if((mo_mcmData->m_selfState == 2))
        {
            msock_mcm.sendudpMessage((unsigned char *)ms_hlt, sizeof(STRUCT_MCM_HEALTH_MSG),MCM_P_IP,8005);
        }
        mo_mcmData->m_mcmCnt++;
    }

    if(mo_mcmData->m_mcmCnt > 2) // previously 3
    {
        if(mb_firstTime == false)//only first time check
        {
//            m_localCnt++;
            if(mo_mcmData->m_selfState == 1)
            {
//                if(m_localCnt > 2)
                {
                    mo_mcmData->m_currentState = 1;
                    ms_hlt->current_state = 1;
                    mb_firstTime = true;
                    qDebug()<< " now master 1 is activated";
                }
            }
            else if(mo_mcmData->m_selfState == 2)
            {
                mo_mcmData->m_currentState = 1;
                ms_hlt->current_state = 1;
                mb_firstTime = true;
                qDebug()<< " now master 2 is activated";

            }
        }

        if(mb_firstTime == true)
        {
            if(mo_mcmData->m_currentState != 1)
            {
                if(mo_mcmData->m_selfState == 2)
                {
                    mo_mcmData->m_currentState = 1;
                    ms_hlt->current_state = 1;
                    qDebug()<< " master 1 is down so now master 2 is activated";
                }
                if(mo_mcmData->m_selfState == 1)
                {
                    mo_mcmData->m_currentState = 1;
                    ms_hlt->current_state = 1;
                    qDebug()<< " master 2 is down so now master 1 is activated";
                }
            }
        }


    }

    if(mb_firstTime == false)
    {
        m_localCnt++;
        if(m_localCnt < 5)
        {
            if(mo_mcmData->m_currentState != 1)
            {
                if(mo_mcmData->m_selfState == 2)
                {
                    if(m_localCnt > 3)
                    {
                        if(mo_mcmData->m_anotherAdmin == 0)
                        {
                            mo_mcmData->m_currentState = 1;
                            ms_hlt->current_state = 1;
                            qDebug()<< " %%  master 2 is activated";
                             mb_firstTime = true;
                        }

                    }

                }
                if(mo_mcmData->m_selfState == 1)
                {
                    if(m_localCnt > 3)
                    {
                        if(mo_mcmData->m_anotherAdmin == 0)
                        {
                            mo_mcmData->m_currentState = 1;
                            ms_hlt->current_state = 1;
                            qDebug()<< " %% master 1 is activated";
                             mb_firstTime = true;
                        }

                    }

                }
            }



        }
        else
        {
            mb_firstTime = true;
            qDebug()<<" fisrt time falg is true now";
        }
    }


    if(mo_mcmData->m_mcmCnt > 100000000)//to avoid infinite increments
    {
        mo_mcmData->m_mcmCnt = 50;
    }
    pthread_mutex_unlock(&mo_mcmData->mutex_hlth);
}

void McmController::startTimerMcm()
{
    if(mb_timer == false)
    {
      mtm_mcmState->start(12000);// changed on 19 june
      mb_timer = true;
    }
}

void McmController::stopTimerMcm()
{
    if(mb_timer == true)
    {
      mtm_mcmState->stop();
      mb_timer = false;
    }
}
