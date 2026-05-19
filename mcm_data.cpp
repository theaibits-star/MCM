#include "mcm_data.h"

McmData* McmData::mo_mcmData = NULL;
McmData::McmData()
{
    mst_mpio = new STRUCT_MPIO_CONFIG_DETAILS[20];
    memset(mst_mpio, NULL, sizeof(STRUCT_MPIO_CONFIG_DETAILS) * 20);
    mst_usr = new STRUCT_USER_DETAILS;
    memset(mst_usr, NULL, sizeof(STRUCT_USER_DETAILS));
    ms_config_file = false;
    //**************************************************************************/
    //added by anurag during IFATS on 16 june 2018 to avoid crash of
    //pthread_mutex_unlock(&mo_mcmData->mutex_mpio[mpioId]);
    //accoring to POSIX if pthread_mutex is not initialized then behavior of lock()
    //  and unlock() is undefined
    //**************************************************************************/
    for(int i=0; i< MAX_MPIO +1;i++)
    {
        pthread_mutex_init(mutex_mpio + i, NULL);
    }
    pthread_mutex_init(&mutex_hlth, NULL);
    pthread_mutex_init(&mutex_admin, NULL);
    pthread_mutex_init(&mutex_mpioA1, NULL);
    pthread_mutex_init(&mutex_mpioA2, NULL);
    pthread_mutex_init(&mutex_mpioB, NULL);
    pthread_mutex_init(&mutex_mpioC, NULL);
    pthread_mutex_init(&mutex_mpioJ1, NULL);
    pthread_mutex_init(&mutex_mpioJ2, NULL);


}

McmData *McmData::getMcmDataInstance()
{
    if(mo_mcmData == NULL)
    {
        mo_mcmData = new McmData;
        return  mo_mcmData;
    }
    return mo_mcmData;
}

void McmData::initData()
{
    ms_mpios = new STRUCT_ALL_MPIO_STATUS_MSG;
    ms_local = new STRUCT_LOCAL_MPIO_STATUS_MSG;
    mo_hlthSender = new HealthSender;
    for(int i =0; i < MAX_EQPT; i++)
    {
        ms_mpios->eqpt[i].eqpt_status = DECONFIG_STATE;
    }
    m_epochTime = 0;
    m_mcmCnt = 0;
    m_mdmCnt = 0;
    m_currentState = -1;
    m_selfState = 0;
    m_adminState = 0;
    m_adminCnt = 0;
    m_anotherAdmin = 0;

    m_cntMpioA1 = 0;
    m_cntMpioA2 = 0;
    m_cntMpioB = 0;
    m_cntMpioC = 0;
    m_cntMpioJ1 = 0;
    m_cntMpioJ1 = 0;

    memset(&ms_eqptInfo, 0, sizeof(STRUCT_MPIO_EQPT_INFO) * (MAX_MPIO + 1));
}

void McmData::callReadFile()
{
    while(1)
    {
        if(access(MCM_FILE, F_OK) != -1)
        {
            if(readFile())
            {
                break;
            }

        }
        else
        {
            printf("file not found \n");
            fflush(stdout);
            sleep(5);
            continue;
        }
    }
    printf(" self state MCM is %%%%%%%%%%  %d \n", m_selfState);
    fflush(stdout);
}

bool McmData::readFile()
{
    FILE *fp = NULL;
    char * token = NULL;
    int lineSize = 200;
    char * line = new char[lineSize];
    int  state;

    fp = fopen(MCM_FILE,"rb");
    if(fp == NULL)
    {
        printf("error in opening snma file \n");
        fflush(stdout);
        return false;
    }
    memset(line , NULL, lineSize);
    int n =1;
    while(!feof(fp))
    {
        fgets(line, lineSize,fp);
        if(strlen(line))
        {
            token = strtok(line, ":");
            if((token != NULL)  && (strlen(token) > 0))
            {
                state = atoi(token);
                m_selfState = state;
                break;
                n++;
            }
            token = strtok(NULL, "\n");
            token = NULL;
            memset(line , NULL, lineSize);

        }
    }
    fclose(fp);
    return true;
}


void McmData::setEquipmentDetails()
{
    //IMP NOTE: INCREASE MAX_EQPT value in constant file while adding new equipment


    int i = 1;

    m_eqpt[i][INDEX_EQPT]  = AWOS_A;//1
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_4800;
    m_eqpt[i][INDEX_PAR]   =  0;
    m_eqptIndex[AWOS_A] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = AWOS_B;//2
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_4800;
    m_eqpt[i][INDEX_PAR]   =  0;
    m_eqptIndex[AWOS_B] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = AIS_A;//3
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_38400;
    m_eqpt[i][INDEX_PAR]   =  0;
    m_eqptIndex[AIS_A] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = AIS_B;//4
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_38400;
    m_eqpt[i][INDEX_PAR]   =  0;
    m_eqptIndex[AIS_B] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = ECHO_A;//5
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_4800;
    m_eqpt[i][INDEX_PAR]   =  0;
    m_eqptIndex[ECHO_A] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = ECHO_B;//6
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_4800;
    m_eqpt[i][INDEX_PAR]   =  0;
    m_eqptIndex[ECHO_B] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = GPS_A;//7
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_4800;
    m_eqpt[i][INDEX_PAR]   =  0;
    m_eqptIndex[GPS_A] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = GPS_B;//8
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_4800;
    m_eqpt[i][INDEX_PAR]   =  0;
    m_eqptIndex[GPS_B] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = LOG_1A;//9
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_4800;
    m_eqpt[i][INDEX_PAR]   =  0;
    m_eqptIndex[LOG_1A] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = LOG_1B;//10
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_4800;
    m_eqpt[i][INDEX_PAR]   =  0;
    m_eqptIndex[LOG_1B] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = LOG_16A;//11
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_19200;
    m_eqpt[i][INDEX_PAR]   =  2;
    m_eqptIndex[LOG_16A] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = LOG_16B;//12
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_19200;
    m_eqpt[i][INDEX_PAR]   =  2;
    m_eqptIndex[LOG_16B] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = RLG_1A;//13
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_4800;
    m_eqpt[i][INDEX_PAR]   =  0;
    m_eqptIndex[RLG_1A] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = RLG_1B;//14
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_4800;
    m_eqpt[i][INDEX_PAR]   =  0;
    m_eqptIndex[RLG_1B] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = RLG_10A;//15
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_19200;
    m_eqpt[i][INDEX_PAR]   =  2;
    m_eqptIndex[RLG_10A] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = RLG_10B;//16
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_19200;
    m_eqpt[i][INDEX_PAR]   =  2;
    m_eqptIndex[RLG_10B] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = RLG_100A;//17
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_38400;
    m_eqpt[i][INDEX_PAR]   =  2;
    m_eqptIndex[RLG_100A] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = RLG_100B;//18
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqpt[i][INDEX_BAUD]  =  CBR_38400;
    m_eqpt[i][INDEX_PAR]   =  2;
    m_eqptIndex[RLG_100B] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = MAGNETIC_COMPASS_A;//19   //  UPDATE ONCE PROTOCOL FINALISED
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_BAUD]  =  CBR_38400;
    m_eqpt[i][INDEX_PAR]   =  2;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqptIndex[MAGNETIC_COMPASS_A] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = MAGNETIC_COMPASS_B;//20   //  UPDATE ONCE PROTOCOL FINALISED
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_BAUD]  =  CBR_38400;
    m_eqpt[i][INDEX_PAR]   =  2;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqptIndex[MAGNETIC_COMPASS_B] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = SRCG_A;//28
    m_eqpt[i][INDEX_INTER]  = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_BAUD]   = CBR_4800; //6
    m_eqpt[i][INDEX_PAR]    = 0;
    m_eqpt[i][INDEX_STOP]   = 1;
    m_eqptIndex[SRCG_A] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = SRCG_B;//29
    m_eqpt[i][INDEX_INTER]  = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_BAUD]   = CBR_4800; //6
    m_eqpt[i][INDEX_PAR]    = 0;
    m_eqpt[i][INDEX_STOP]   = 1;
    m_eqptIndex[SRCG_B] = i;

    i++;

    m_eqpt[i][INDEX_EQPT]  = STABLIZER_A;//21
    m_eqpt[i][INDEX_INTER] = SERIAL_INTERFACE;
    m_eqpt[i][INDEX_BAUD]  =  CBR_4800;
    m_eqpt[i][INDEX_PAR]   =  0;
    m_eqpt[i][INDEX_STOP]  =  1;
    m_eqptIndex[STABLIZER_A] = i;

    i++;


    //  printf("total eqpt added are %d\n",i);
}
