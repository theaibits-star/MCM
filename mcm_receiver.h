#ifndef MCM_RECEIVER_H
#define MCM_RECEIVER_H

#include <QObject>
#include "threadudpsock.h"
#include "xml_read_write.h"


class McmData;
class McmReceiver : public QObject
{
    Q_OBJECT
public:
    explicit McmReceiver(QObject *parent = 0);
    void initializeSocket();

private:
    ThreadUdpsock *msock_mcmRecv;
    McmData *mo_mcmData;
    XmlReadWrite *mo_xml;


private:

    void processMsgFromMdm();
    void goForSavedConfiguration();


    
signals:
    
private slots:
    void processRecvMsg(QByteArray byteArrBuf);
    
};

#endif // MCM_RECEIVER_H
