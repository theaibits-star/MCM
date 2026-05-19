#ifndef MCM_DATA_H
#define MCM_DATA_H
#include <QList>
#include <QByteArray>
#include "mcm_struct.h"
#include "dadm_sender.h";
#include "health_sender.h"
#include "data_controller.h"
#include "mcm_controller.h"
#include <QMutex>

class McmData
{
private:
    McmData();
    static McmData *mo_mcmData;

public:
    bool ms_config_file;
    DataController *mo_dataCntrl;
    int m_mcmCnt;
    int m_mdmCnt;
    int  m_selfState;
    int m_currentState;
    int m_adminState;
    int m_adminCnt;
    int m_anotherAdmin;

    int m_cntMpioA1;
    int m_cntMpioA2;
    int m_cntMpioB;
    int m_cntMpioC;
    int m_cntMpioJ1;
    int m_cntMpioJ2;

    pthread_mutex_t mutex_hlth;
    pthread_mutex_t mutex_admin;

    pthread_mutex_t mutex_mpioA1;
    pthread_mutex_t mutex_mpioA2;
    pthread_mutex_t mutex_mpioB;
    pthread_mutex_t mutex_mpioC;
    pthread_mutex_t mutex_mpioJ1;
    pthread_mutex_t mutex_mpioJ2;

public:
    void setEquipmentDetails();
    static McmData* getMcmDataInstance();
    void initData();
    void callReadFile();
    bool readFile();

public:
    QList<QByteArray> msgList;
    pthread_mutex_t mutex_mpio[MAX_MPIO + 1];
    STRUCT_MPIO_CONFIG_DETAILS  *mst_mpio;
    STRUCT_USER_DETAILS *mst_usr;
    DadmSender *mo_dadmSender;
    HealthSender *mo_hlthSender;

    McmController *mo_mcmCntrol;
    int m_eqpt[MAX_EQPT][MAX_EQPT_DETAIL];
    int m_eqptIndex[150];
    int m_epochTime;
//    STRUCT_POLL_EQPT_MSG ms_actual[MAX_EQPT];
    STRUCT_ALL_MPIO_STATUS_MSG *ms_mpios;
    STRUCT_LOCAL_MPIO_STATUS_MSG *ms_local;
    STRUCT_MPIO_EQPT_INFO ms_eqptInfo[MAX_MPIO + 1];
    unsigned int m_timeMpio[MAX_MPIO + 1];

};

#endif // MCM_DATA_H
