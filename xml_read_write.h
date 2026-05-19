#ifndef XML_READ_WRITE_H
#define XML_READ_WRITE_H

#include <qxmlstream.h>
#include "mcm_struct.h"
#include <QFile>
#include <QDomElement>
#include <sys/time.h>
#include<QObject>
class McmData;
class XmlReadWrite : public QObject
{
    Q_OBJECT
public:
    XmlReadWrite();
    bool writeMpio10scardDetails(STRUCT_MPIO_INIT_DETAIL_MSG *s_card);
    int readMpioConfiguration();
    int readUserDetails();
    void readMpio(QDomElement ePort, int mpioId, int slotNo, int mpioType, int cardStatus, QString ip);
    void adduserTest();
    bool addEqptDetail(STRUCT_MPIO_EDIT_DETAIL_MSG *s_eqpt);

private:
    bool createMpioDetail(STRUCT_MPIO_INIT_DETAIL_MSG *s_card);
    bool addMpioDetail(STRUCT_MPIO_INIT_DETAIL_MSG *s_card);

signals:
    void stopHltTimer();
    void startHltTimer();
    void stopMcmContTimer();
    void startMcmContTimer();
private:
    McmData *mo_mcmData;
    int m_eqptIndex;
    int m_eqptId;
    STRUCT_MPIO_EQPT_INFO ms_eqptInfo;
    int m_eqptNo;
    struct timeval curSecond;

private:
    int getCardType(int slotNo);
};

#endif // XML_READ_WRITE_H
