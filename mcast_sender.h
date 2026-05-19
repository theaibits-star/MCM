#ifndef MCAST_SENDER_H
#define MCAST_SENDER_H

#include <QObject>
#include <QHostAddress>
#include "print_msg.h"

class QUdpSocket;

class McastSender : public QObject
{
    Q_OBJECT
public:
    explicit McastSender(QObject *parent = 0);
    int sendDatagram(unsigned char * buffer, int msgLen);
    
signals:
    
public slots:

private:
    QUdpSocket *udpSocket;
    QHostAddress groupAddress;
    PrintMsg mo_print;
    
};

#endif // MCAST_SENDER_H
