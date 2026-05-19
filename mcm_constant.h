#ifndef MDM_CONSTANT
#define MDM_CONSTANT

// MDM CONSTANT


const char AWOS_A                                  = 1;
const char AWOS_B                                  = 2;
const char AIS_A                                   = 3;
const char AIS_B                                   = 4;
const char ECHO_A                                  = 5;
const char ECHO_B                                  = 6;
const char GPS_A                                   = 7;
const char GPS_B                                   = 8;
const char LOG_1A                                  = 9;
const char LOG_1B                                  = 10;
const char LOG_16A                                 = 11;
const char LOG_16B                                 = 12;
const char RLG_1A                                  = 13;
const char RLG_1B                                  = 14;
const char RLG_10A                                 = 15;
const char RLG_10B                                 = 16;
const char RLG_100A                                = 17;
const char RLG_100B                                = 18;
const char MAGNETIC_COMPASS_A                      = 19;
const char MAGNETIC_COMPASS_B                      = 20;
//const char RLG_NMEA_A                              = 21;
//const char RLG_NMEA_B                              = 22;

const char SRCG_A                                   = 101;
const char SRCG_B                                   = 102;
const char STABLIZER_A                              = 103;
//const char STABLIZER_B                            = 104;

const char MPIO_TYPE_2S                            = 1;
const char MPIO_TYPE_10S                           = 2;
const char MAX_MPIO                                = 7;

const char SLOT_10S                                = 10;
const char SLOT_2S                                 = 2;

const char MAX_SERIAL_PORT                         = 8;
const char MAX_DISCRETE_PORT                       = 24;
const char MAX_SRDC_DSRC_PORT                      = 9;

const char CARRIER_SERIAL_CARD                     = 1;
const char CARRIER_MIL_CARD                        = 2;
const char SYSTEM_CARD                             = 3;
const char DISCRETE_CARD                           = 4;
const char SRDC_DSCR_CARD                          = 5;
const char SWITCH_CARD                             = 6;

const char SEM_ID                                  = 1;
const char SIEM_ID                                 = 2;
const char SSEM_ID                                 = 3;
const char DADM_ID                                 = 4;
const char MCM_ID                                  = 5;
const char MDM_ID                                  = 6;

const char MAX_USR_ACT                             = 5;
const char MAX_USR_ALPHABET                        = 20;

const char ADMIN_MODE                              = 1;
const char USER_MODE                               = 2;

const char MAX_EQPT                                = 23 + 1;
const char MAX_SERIAL_EQPT                         = 23; // check ??
const char MAX_EQPT_DETAIL                         = 9;
const char INDEX_STATUS                            = 0;
const char INDEX_EQPT                              = 1;
const char INDEX_INTER                             = 2;
const char INDEX_BAUD                              = 3;
const char INDEX_PAR                               = 4;
const char INDEX_STOP                              = 5;
const char INDEX_MPIO                              = 6;
const char INDEX_SLOT                              = 7;
const char INDEX_PORT                              = 8;

const char SERIAL_INTERFACE                        = 1;
const char SRDC_INTERFACE                          = 2;
const char DSRC_INTERFACE                          = 3;
const char DISCRETE0_INTERFACE                     = 4;
const char DISCRETE1_INTERFACE                     = 5;
const char DISCRETE2_INTERFACE                     = 6;
const char MIL_INTERFACE                           = 7;

//GEERAL FOR ALL APP
//baudrate
const char CBR_1200                                = 1;
const char CBR_2400                                = 2;
const char CBR_4800                                = 3;
const char CBR_9600                                = 4;
const char CBR_14400                               = 5;
const char CBR_19200                               = 6;
const char CBR_38400                               = 7;
const char CBR_56000                               = 8;
const char CBR_57600                               = 9;
const char CBR_115200                              = 10;
const char CBR_128000                              = 11;
const char CBR_256000                              = 12;


//GEERAL

const char MAX_IP_BYTE                             = 20;
const char ADD_EQPT                                = 2;
const char DELETE_EQPT                             = 1;

const char dadm1_primary[]                         ="10.2.1.9";
//const char dadm1_primary[]                         ="10.2.1.24";
const char dadm1_second[]                          ="10.2.1.87";

const char dadm2_primary[]                         ="10.2.1.54";
const char dadm2_second[]                          ="10.2.1.87";

const char dadm3_primary[]                         ="10.2.1.84";
const char dadm3_second[]                          ="10.2.1.87";

const char dadm4_primary[]                         ="10.2.1.114";
const char dadm4_second[]                          ="10.2.1.87";

//const char dadm5_primary[]                         ="10.2.1.144";
//const char dadm5_second[]                          ="10.2.1.87";

//const char dadm6_primary[]                         ="10.2.1.174";
//const char dadm6_second[]                          ="10.2.1.87";



//const char dadm1_primary[]                         ="155.4.167.130";
//const char dadm1_second[]                          ="155.4.167.131";

//const char dadm2_primary[]                         ="10.2.1.54";
//const char dadm2_second[]                          ="10.2.1.87";

//const char dadm3_primary[]                         ="155.4.167.130";
//const char dadm3_second[]                          ="10.2.1.87";

//const char dadm4_primary[]                         ="155.4.167.129";
//const char dadm4_second[]                          ="10.2.1.87";

//const char dadm5_primary[]                         ="10.2.1.144";
//const char dadm5_second[]                          ="10.2.1.87";

//const char dadm6_primary[]                         ="10.2.1.174";
//const char dadm6_second[]                          ="10.2.1.87";




const char PRIMARY                                 =1;
const char SECONDARY                               =2;




//const char *MCM_NODE_IP                                 ="10.2.1.14";

const char NO_ERR                                   = 1;
const char DOWN_ERR                                 = 2;
const char CHECKS_ERR                               = 3;
const char FRAME_ERR                                = 4;
const char HEADER_ERR                               = 5;
const char PORT_ERR                                 = 6;
const char UNKNOWN_ERR                              = 7;
const char DECONFIG_STATE                           = 8;



//MDM ENDS
//const char MCM_P_IP[]                              ="10.2.1.42";

//const char MCM_P_IP[]                              ="10.7.30.253";
//const char MCM_S_IP[]                              ="10.7.30.254";


//// FOR Local testing purpose
const char MCM_P_IP[]                              ="10.2.1.9";
const char MCM_S_IP[]                              ="10.2.1.10";


//GEERAL

const char MCM_FILE[]                              = "/home/MPIO/mcm.txt";

#endif // MDM_CONSTANT

