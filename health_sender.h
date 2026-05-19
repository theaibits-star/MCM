#ifndef HEALTH_SENDER_H
#define HEALTH_SENDER_H

#include <QObject>
#include <QTimer>
#include "mcm_struct.h"
#include "mcast_sender.h"
#include <sys/time.h>

class McmData;
class HealthSender : public QObject
{
    Q_OBJECT
public:
    explicit HealthSender(QObject *parent = 0);
    static HealthSender* getHealthSenderDataInstance();
    static HealthSender *mo_HealthSender;

private:
    QTimer *m_hltTimer;
    McmData *mo_mcmData;
    struct timeval curSecond;
    int m_failCnt[MAX_MPIO + 1];
    int m_index;
    McastSender *msock_mcm;
    STRUCT_MPIO_CARD_STATUS_MSG ms_card;


private:
    void sendMpioDownCardStatus(int mpioid);

signals:

public slots:
    void updateStatus();
    void startTimer();
    void stopTimer();

};

#endif // HEALTH_SENDER_H
