#ifndef DADM_SENDER_H
#define DADM_SENDER_H

#include <QObject>
#include "threadudpsock.h"
#include "mcm_struct.h"
#include "file_sender.h"
#include "print_msg.h"

class DadmSender : public QObject
{
    Q_OBJECT
public:
    explicit DadmSender(QObject *parent = 0);
//    void initializeDadmSocket();
//    void filterMessageDadm(int msgId);

signals:
    void updateMpioComplete();

public slots:

private:
    ThreadUdpsock sock_dadm[MAX_MPIO + 1][2];
    QString mstr_ip[MAX_MPIO + 1][2];
    FileSender *mo_File;
    char m_ip[MAX_MPIO + 1][2][20];
    PrintMsg objPrint;


private:
    void showErrorLog(int errNo, char * ip);

public:
    void updateAllMpios();
    void updateOneMpio(unsigned char *msg);
    void sendLiveDataReqToDadm(char * buffer, int len);
    void updateMpio(int mpioID);
    void updateMcm1();
    void updateMcm2();


};

#endif // DADM_SENDER_H
