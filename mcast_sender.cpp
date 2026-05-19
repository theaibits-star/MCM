#include "mcast_sender.h"
#include <QtNetwork>
#include <QDebug>
#include"mcm_struct.h"

McastSender::McastSender(QObject *parent) :
    QObject(parent)
{
//    qDebug()<<"address of McastSender"<<this;
    groupAddress = QHostAddress("239.10.43.21");
    udpSocket = new QUdpSocket(this);

    udpSocket->bind(QHostAddress::AnyIPv4, 45454,QUdpSocket::ReuseAddressHint|QUdpSocket::ShareAddress);
    udpSocket->joinMulticastGroup(groupAddress);

    udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, 4);// this 1 is to be
                                                //      verified for remote and local MDM

}

int McastSender::sendDatagram(unsigned char *buffer, int msgLen)
{
//    qDebug()<<"address of McastSender sendDatagram"<<this;
    int write = udpSocket->writeDatagram((char*) buffer, msgLen,
                             groupAddress, 45454);
    STRUCT_MESSAGE_HEADER *msghdr = (STRUCT_MESSAGE_HEADER *) buffer;
    if(msghdr->msg_id == ADD_EQPT_SUCCESS_MSG_ID)
    {
        printf(" send config success \n");
//        mo_print.printAddEqpt((char*)buffer, msgLen);
    }
    else if(msghdr->msg_id == DELETE_EQPT_SUCCESS_MSG_ID)
    {
        printf(" send Deconfig success \n");
//        mo_print.printAddEqpt((char*)buffer, msgLen);
    }
    else if(msghdr->msg_id == ALL_MPIO_STATUS_MSG_ID)
    {
//        printf(" send eqpt status \n");
//        mo_print.EqptStatusSend((char*)buffer, msgLen);
    }
//    printf("write bytes are %d \n",write);
//    fflush(stdout);
    return write;
}
