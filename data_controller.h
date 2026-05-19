#ifndef DATA_CONTROLLER_H
#define DATA_CONTROLLER_H

#include <QThread>
#include "mdm_data_contrl.h"
#include "mcast_sender.h"
#include <sys/time.h>
#include "print_msg.h"
#include "mcast_sender.h"

class McmData;
class DataController : public QThread
{
    Q_OBJECT
public:
    explicit DataController(QObject *parent = 0);
    void run();
    void runThread();

private:
    void filterRecvMsg(char* buffer, int msgLen);
    void processMsgFromMdm(char * buffer, int msgLen);
    void processMsgFromDadm(char * buffer, int msgLen);
    void processMsgFromMcm(char * buffer, int msgLen);
    void setCurrentEquipmentState(char * buffer, int msgLen);

private:
    McmData *mo_mcmData;
    MdmDataContrl *mo_mdmData;
    McastSender *mcsock_mdm;
    int m_index;
    struct timeval curSecond;
    PrintMsg objPrint;
    bool m_sendFilestate;
    STRUCT_MESSAGE_HEADER *ms_logout;
    
signals:
    void mpioStatus(char *);
    
public slots:
    
};

#endif // DATA_CONTROLLER_H
