/**********************************************************************************
1.		File Name			bss_si_threadudpsock.h


2.		Company Name		BEL-CENTRAL RESEARCH LABORATORY,DELHI CENTRE

3.		Project Name		BattleField Surveillence System(BSS)

4.    Project ID			-

5.    System Name			-

6.    CSCI Name

7.    CSCI PUI

8.    Class Name			- ThreadUdpsock

9.    Class PUI

10. 	Version No	: 0.9

11.	Synopsis				The files contains the declarations of all the
                                        literals and structures which are local to
                                        ThreadUdpsock.

12.	Author(s)				Pritesh Prakash

13.	Department 			CRL

14.	Designation			M(RS)

15.	Date of File		: 12-12-2012
        Creation

16.    Macros

17.  Modification
       History

   (a) Modification
        Date

   (b) Modified By


   (c) Modification
        Details
*************************************************************************/


#ifndef BSS_SI_THREADUDPSOCK_H
#define BSS_SI_THREADUDPSOCK_H



#include <QThread>
#include <QObject>
#include "QUdpSocket"


class ThreadUdpsock : public QThread
{
    Q_OBJECT
public:
    explicit ThreadUdpsock(QObject *parent = 0);

    ThreadUdpsock(char *ipAddress,int port);

    void runThread();
    void run();
    //char buffer[1500];
    void sendudpMessage(unsigned char * sendbuffer,int sendinglen, QString ipaddr,qint16 sendport);
signals:
    void MessageTrigger(QByteArray);
    void mySlot(int);

protected:


private:
    QUdpSocket *recvsocket;
    QUdpSocket sendsocket;
    int datalen;
    QByteArray buffer;



public slots:
    void ReceiveMessage();
    
};

#endif // BSS_SI_THREADUDPSOCK_H
