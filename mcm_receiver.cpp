#include "mcm_receiver.h"
#include "mcm_struct.h"
#include "mcm_data.h"
#include <QDebug>

McmReceiver::McmReceiver(QObject *parent) :
    QObject(parent)
{
    mo_mcmData = McmData::getMcmDataInstance();
    mo_mcmData->setEquipmentDetails();
    mo_mcmData->initData();
    mo_mcmData->callReadFile();
    mo_mcmData->mo_dadmSender = new DadmSender;
    mo_xml= new XmlReadWrite;
}

void McmReceiver::initializeSocket()
{
    goForSavedConfiguration();
    qDebug()<<"going to start thread";
    msock_mcmRecv = new ThreadUdpsock("127.0.0.1",8005);
    msock_mcmRecv->runThread();
    connect(msock_mcmRecv,SIGNAL(MessageTrigger(QByteArray)),this,SLOT(processRecvMsg(QByteArray)));
}

void McmReceiver::processRecvMsg(QByteArray byteArrBuf)
{
//    qDebug()<<"size "<<byteArrBuf.size();
    if(byteArrBuf.size() > 0 && byteArrBuf.size() < 1500)
    {
//            qDebug()<<"some data came here";
        mo_mcmData->msgList.append(byteArrBuf);
    }
}

void McmReceiver::goForSavedConfiguration()
{
    int readVal = mo_xml->readMpioConfiguration();

    for(int i = 1; i < MAX_MPIO + 1; i++)
    {
        for(int j = 0; j < mo_mcmData->ms_eqptInfo[i].no_of_eqpt; j++)
        {
            qDebug()<<" eqpt index "<< mo_mcmData->ms_eqptInfo[i].eqpt[j] << " at mpio id "<< i;
        }
    }

    mo_mcmData->mo_hlthSender->startTimer();
//    mo_xml->adduserTest();
    if(readVal == 0)
    {

    }

//    int usrVal = mo_xml->readUserDetails();
}
