/******************************************************************************
1.File Name	    : bss_si_threadudpsock.cpp

2.Company Name	: BEL-CENTRAL RESEARCH LABORATORY,DELHI CENTRE

3.Project Name	: Ship Data Network(SDN)

4.Project ID	:

5.System Name	:

6.CSCI Name		:

7.CSCI PUI		:

8.Class Name	: ThreadUdpsock

9.Class PUI 	:

10.Version No	: 0.9

11.Synopsis		:		This file is the source file for threadbased ThreadUdpsock
                    class of the BSS project.This file contains the coding (definitions)  of
                    functions of the  class.Each method contents the description
                    of the purpose of itself and the code that implements it's
                    purpose in the object_oriented language (C++).

12.Author(s)	:Pritesh Prakash

13.Department 	: CRL

14.Designation	: M(RS)
15.Date of File	: 12-12-2012
   Creation

16.Include File	:
                        QHostAddress

17.Libraries	:
   Used

18.List of              run
   Routines 	:		runThread				Ver 0.9
                        sendudpMessage
                        MessageTrigger
                        ReceiveMessage

19.Macros 		:

20.Modification

   (a) Modification :
            Date

   (b) Modified By	:

   (c) Modification :
        Details

******************************************************************************/

#include "threadudpsock.h"
ThreadUdpsock::ThreadUdpsock(QObject *parent) :
    QThread(parent)
{

}

ThreadUdpsock::ThreadUdpsock(char *ipAddress,int port)
{
//    qDebug()<<"address of ThreadUdpsock"<<this;
    recvsocket =new QUdpSocket();
    QString ipaddr=ipAddress;
    QHostAddress addr;
    addr=ipaddr;
    qint16 recvport=port;
    bool boundstate;

    boundstate =recvsocket->bind(QHostAddress::Any,recvport);
    if(boundstate == false)
        qDebug()<<"bound state is not ok";
    else
        qDebug()<<"bound state is ok";
}

void ThreadUdpsock::run()
{

    bool conn_status = connect(recvsocket, SIGNAL(readyRead()), this, SLOT(ReceiveMessage()));
    if(conn_status==false)
        qDebug()<<"connect udp status failled";
    else
        qDebug()<<"connect udp status succeed";
//    recvsocket->flush();

   exec();

}

void ThreadUdpsock::runThread()
{
    start();
}

void ThreadUdpsock::ReceiveMessage()
{

//    qDebug()<<"in udp receiver";
    buffer.resize(recvsocket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    datalen=recvsocket->readDatagram(buffer.data(), buffer.length(), &sender, &senderPort);
    if(datalen<0)
    {
        qDebug()<<"Not Received";
    }
    else
    {
//        qDebug()<<buffer.size();
        emit MessageTrigger(buffer);

    }
}

void ThreadUdpsock::sendudpMessage(unsigned char * sendbuffer,int sendinglen, QString ipaddr,qint16 sendport)

{
    QHostAddress sendaddr;
    sendaddr=ipaddr;

//    for(int i = 0 ; i < sendinglen; i++)
//    {
//        qDebug()<<sendbuffer[i];
//    }

    qint64 send_byte = sendsocket.writeDatagram((char*)sendbuffer,sendinglen,sendaddr,sendport);
//    qDebug()<<"sent bytes are "<<send_byte;



}



