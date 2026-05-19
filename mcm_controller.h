#ifndef MCM_CONTROLLER_H
#define MCM_CONTROLLER_H

#include <QObject>
#include <QTimer>
#include "threadudpsock.h"
#include "mcm_struct.h"

class McmData;

class McmController : public QObject
{
    Q_OBJECT
public:
    explicit McmController(QObject *parent = 0);
     STRUCT_MCM_HEALTH_MSG *ms_hlt;
     static McmController* getMcmControllerInstance();
      static McmController *mo_McmController;

private:
    McmData *mo_mcmData;
    QTimer *mtm_mcmState;
    bool mb_firstTime;
    int m_localCnt;
    int m_master2Cnt;
    int m_hlthOnStartup;
    ThreadUdpsock msock_mcm;

    bool mb_timer;
//    int m_currentState;

public:


signals:

public slots:

    void startMcmStateTimer();
    void startTimerMcm();
    void stopTimerMcm();

};

#endif // MCM_CONTROLLER_H
