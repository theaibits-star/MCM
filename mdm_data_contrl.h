#ifndef MDM_DATA_CONTRL_H
#define MDM_DATA_CONTRL_H

#include <QObject>
#include "xml_read_write.h"
#include "mcast_sender.h"

class McmData;
class MdmDataContrl : public QObject
{
    Q_OBJECT
public:
    explicit MdmDataContrl(QObject *parent = 0);
    void processMpio10SConfigMsg(char * buffer, int msgLen);
    void processInitMpioMsg(char * buffer, int msgLen);
    void processAddEqpt(char * buffer, int msgLen);
    void processDeleteEqpt(char * buffer, int msgLen);
    void processCmMsg(char * buffer, int msgLen);

private:
    McmData *mo_mcmData;
    XmlReadWrite *mo_xml;
    McastSender *mcscok_mdm;
    bool mf_wrtMpio10Crd;

private:
    void storeMpio10SDetails(STRUCT_MPIO_INIT_DETAIL_MSG *s_card);
    void replyInitMpioSuccess(int mode, int mdmId);
    void replyRejectAdminMode(int mdmId);
    
signals:
    
private slots:
    void sendUpdateResponse();
    
};

#endif // MDM_DATA_CONTRL_H
