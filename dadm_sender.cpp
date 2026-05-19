#include "dadm_sender.h"

DadmSender::DadmSender(QObject *parent) :
    QObject(parent)
{
    mo_File = new FileSender;
    strcpy((char*)m_ip[1][PRIMARY],( char*)dadm1_primary);
    strcpy((char*)m_ip[1][SECONDARY],( char*)dadm1_second);

    strcpy((char*)m_ip[2][PRIMARY],( char*)dadm2_primary);
    strcpy((char*)m_ip[2][SECONDARY],( char*)dadm2_second);

    strcpy((char*)m_ip[3][PRIMARY],( char*)dadm3_primary);
    strcpy((char*)m_ip[3][SECONDARY],( char*)dadm3_second);

    strcpy((char*)m_ip[4][PRIMARY],( char*)dadm4_primary);
    strcpy((char*)m_ip[4][SECONDARY],( char*)dadm4_second);

//    strcpy((char*)m_ip[5][PRIMARY],( char*)dadm5_primary);
//    strcpy((char*)m_ip[5][SECONDARY],( char*)dadm5_second);

//    strcpy((char*)m_ip[6][PRIMARY],( char*)dadm6_primary);
//    strcpy((char*)m_ip[6][SECONDARY],( char*)dadm6_second);
}

void DadmSender::showErrorLog(int errNo, char *ip)
{
    qDebug()<<"error in file for "<<ip<< "with error no "<<errNo;
    if(errNo == 1)
    {
        qDebug()<<"connection to host failled";
    }
    else if(errNo == 2)
    {
        qDebug()<<"invalid user name or password";
    }
    else if(errNo == 7)
    {
        qDebug()<<"file is not present in local machine";
    }
}


void DadmSender::updateAllMpios()
{
//    STRUCT_MESSAGE_HEADER *conf = new STRUCT_MESSAGE_HEADER;
//    conf->source_id = MCM_ID;
//    conf->dest_id = DADM_ID;
//    conf->msg_id = CONFIG_FILE_RECV_MSG_ID;
//    conf->msg_len = sizeof(STRUCT_MESSAGE_HEADER);

//    int status = 0;
//    status = mo_File->sendFile(m_ip[1][PRIMARY],"/home/MPIO/CONFIG_FILES/SDN_CONFIG.xml","/home/MPIO/SDN_CONFIG.xml");
//    sock_dadm[1][PRIMARY].sendudpMessage((unsigned char*)conf,conf->msg_len, m_ip[1][PRIMARY], 8004);
//    if(status != 0)
//    {
//        showErrorLog(status, m_ip[1][PRIMARY]);
//    }
//    status = mo_File->sendFile(m_ip[1][PRIMARY],"/home/MPIO/CONFIG_FILES/MPIO_ID","/home/MPIO/DADM/MPIO1/MPIO_ID");
//    status = mo_File->sendFile(m_ip[1][SECONDARY],"/home/MPIO/SDN_CONFIG.xml","/home/MPIO/SDN_CONFIG.xml");
//    if(status != 0)
//    {
//        showErrorLog(status, m_ip[1][SECONDARY]);
//    }



//    status = mo_File->sendFile(m_ip[2][PRIMARY],"/home/MPIO/CONFIG_FILES/SDN_CONFIG.xml","/home/MPIO/SDN_CONFIG.xml");
//    if(status != 0)
//    {
//        showErrorLog(status, m_ip[2][PRIMARY]);
//    }
//    status = mo_File->sendFile(m_ip[2][SECONDARY],"/home/MPIO/CONFIG_FILES/SDN_CONFIG.xml","/home/MPIO/SDN_CONFIG.xml");
//    if(status != 0)
//    {
//        showErrorLog(status, m_ip[2][SECONDARY]);
//    }



//    status = mo_File->sendFile(m_ip[3][PRIMARY],"/home/MPIO/CONFIG_FILES/SDN_CONFIG.xml","/home/MPIO/SDN_CONFIG.xml");
//    if(status != 0)
//    {
//        showErrorLog(status, m_ip[3][PRIMARY]);
//    }
//    status = mo_File->sendFile(m_ip[3][SECONDARY],"/home/MPIO/CONFIG_FILES/SDN_CONFIG.xml","/home/MPIO/SDN_CONFIG.xml");
//    if(status != 0)
//    {
//        showErrorLog(status, m_ip[3][SECONDARY]);
//    }


//    status = mo_File->sendFile(m_ip[4][PRIMARY],"/home/MPIO/CONFIG_FILES/SDN_CONFIG.xml","/home/MPIO/SDN_CONFIG.xml");
//    if(status != 0)
//    {
//        showErrorLog(status, m_ip[4][PRIMARY]);
//    }
//    status = mo_File->sendFile(m_ip[4][SECONDARY],"/home/MPIO/CONFIG_FILES/SDN_CONFIG.xml","/home/MPIO/SDN_CONFIG.xml");
//    if(status != 0)
//    {
//        showErrorLog(status, m_ip[4][SECONDARY]);
//    }



//    status = mo_File->sendFile(m_ip[5][PRIMARY],"/home/MPIO/CONFIG_FILES/SDN_CONFIG.xml","/home/MPIO/SDN_CONFIG.xml");
//    if(status != 0)
//    {
//        showErrorLog(status, m_ip[5][PRIMARY]);
//    }
//    status = mo_File->sendFile(m_ip[5][SECONDARY],"/home/MPIO/CONFIG_FILES/SDN_CONFIG.xml","/home/MPIO/SDN_CONFIG.xml");
//    if(status != 0)
//    {
//        showErrorLog(status, m_ip[5][SECONDARY]);
//    }



//    status = mo_File->sendFile(m_ip[6][PRIMARY],"/home/MPIO/CONFIG_FILES/SDN_CONFIG.xml","/home/MPIO/SDN_CONFIG.xml");
//    if(status != 0)
//    {
//        showErrorLog(status, m_ip[6][PRIMARY]);
//    }
//    status = mo_File->sendFile(m_ip[6][SECONDARY],"/home/MPIO/CONFIG_FILES/SDN_CONFIG.xml","/home/MPIO/SDN_CONFIG.xml");
//    if(status != 0)
//    {
//        showErrorLog(status, m_ip[6][SECONDARY]);
//    }

//    delete conf;
//    conf = NULL;
//    emit updateMpioComplete();
}

void DadmSender::sendLiveDataReqToDadm(char * buffer, int len)
{
    STRUCT_LIVE_DATA_MSG *ms_liveData = (STRUCT_LIVE_DATA_MSG *) buffer;
    int mId = ms_liveData->mpio_id;
    sock_dadm[mId][PRIMARY].sendudpMessage((unsigned char*)buffer,len, m_ip[mId][PRIMARY], 8004);
    printf("live data mesg send \n");
    objPrint.liveDataEqpt(buffer, len);

}

void DadmSender::updateMpio(int mpioID)
{
    STRUCT_MESSAGE_HEADER *conf = new STRUCT_MESSAGE_HEADER;
    conf->source_id = MCM_ID;
    conf->dest_id = DADM_ID;
    conf->msg_id = CONFIG_FILE_RECV_MSG_ID;
    conf->msg_len = sizeof(STRUCT_MESSAGE_HEADER);
    printf(" config file send  for mpio %d \n", mpioID);
    printf(" source %d \n", conf->source_id);
    printf(" dest %d \n", conf->dest_id);
    printf(" msgid %d \n",conf->msg_id);
    printf(" len %d \n", conf->msg_len);

    int status = 0;
    status = mo_File->sendFile(m_ip[mpioID][PRIMARY],"/home/MPIO/CONFIG_FILES/SDN_CONFIG.xml","/home/MPIO/SDN_CONFIG.xml", 0);
//    sleep(1);
    usleep(100);
    sock_dadm[mpioID][PRIMARY].sendudpMessage((unsigned char*)conf,conf->msg_len, m_ip[mpioID][PRIMARY], 8004);
    if(status != 0)
    {
        showErrorLog(status, m_ip[1][PRIMARY]);
    }
    else
    {
        printf(" config file successfully sent to DADM ip %s \n",m_ip[mpioID][PRIMARY]);
        fflush(stdout);
    }
    delete conf;
    conf = NULL;
}

void DadmSender::updateMcm1()
{
    printf(" config file send  to MCM 1 \n");

    int status = 0;
    status = mo_File->sendFile(MCM_P_IP,"/home/MPIO/SDN_CONFIG.xml","/home/MPIO/SDN_CONFIG.xml", 1);


    if(status != 0)
    {
        showErrorLog(status, (char*)MCM_P_IP);
    }

    int killState = mo_File->killMcm1();
    if(killState == 0)
    {
        qDebug()<<" kill succesfully";
    }
    else
    {
       qDebug()<<" kill failled";
    }
}

void DadmSender::updateMcm2()
{
    printf(" config file send  to MCM 2 \n");

    int status = 0;
    status = mo_File->sendFile(MCM_S_IP,"/home/MPIO/SDN_CONFIG.xml","/home/MPIO/SDN_CONFIG.xml", 1);

    if(status != 0)
    {
        showErrorLog(status, (char*)MCM_S_IP);
    }
    int killState = mo_File->killMcm2();
    if(killState == 0)
    {
        qDebug()<<" kill succesfully";
    }
    else
    {
       qDebug()<<" kill failled";
    }
}



