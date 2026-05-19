#ifndef MDM_STRUCT
#define MDM_STRUCT

#include "mcm_constant.h"
#include "mcm_message_id.h"

#pragma pack(1)

typedef char                                INT_8;
typedef unsigned char                       UINT_8;
typedef unsigned short                      UINT_16;
typedef short                               INT_16;
typedef int                                 INT_32;
typedef unsigned int                        UINT_32;
typedef unsigned long                       UINT_64;

typedef struct
{
    UINT_8                              source_id;
    UINT_8                              dest_id;
    UINT_16                             msg_id;
    UINT_16                             msg_len;

}STRUCT_MESSAGE_HEADER;

typedef struct
{
    UINT_8                              card_type;
    INT_8                               ip_addr[MAX_IP_BYTE];

}STRUCT_CARD_IP_DETAILS;


typedef struct
{
    UINT_8                              eqpt_id;

}STRUCT_NON_SERIAL_CARD_DETAILS;


typedef struct
{
    UINT_8                              eqpt_id;
    UINT_8                              baudrate;
    UINT_8                              parity;
    UINT_8                              stopbit;

}STRUCT_CARD_CONFIG_DETAILS;

typedef struct
{
    INT_8                               cc_ip[2][MAX_IP_BYTE];
    STRUCT_CARD_CONFIG_DETAILS          cc_card[2][MAX_SERIAL_PORT];

}STRUCT_MPIO_2S_CONFIG_DETAILS;

typedef struct
{
    INT_8                               switch1_ip[MAX_IP_BYTE];
    INT_8                               switch2_ip[MAX_IP_BYTE];
    INT_8                               sr_dr_ip[MAX_IP_BYTE];
    STRUCT_NON_SERIAL_CARD_DETAILS      sr_dr_card[MAX_SRDC_DSRC_PORT];
    INT_8                               disc_ip[MAX_IP_BYTE];
    STRUCT_NON_SERIAL_CARD_DETAILS      discrt_card[MAX_DISCRETE_PORT];
    INT_8                               cc_ip[5][MAX_IP_BYTE];
    STRUCT_CARD_CONFIG_DETAILS          cc_card[5][MAX_SERIAL_PORT];
    INT_8                               sys_ip[MAX_IP_BYTE];
    INT_8                               card_status[10];

}STRUCT_MPIO_10S_CONFIG_DETAILS;

typedef union
{
    STRUCT_MPIO_2S_CONFIG_DETAILS       mpio_2s;
    STRUCT_MPIO_10S_CONFIG_DETAILS      mpio_10s;

}STRUCT_MPIO_10S_2S_CONFIG_DETAILS;


typedef struct
{
    UINT_8                              config_status;
    UINT_8                              mpio_type;
    STRUCT_MPIO_10S_2S_CONFIG_DETAILS   mpio_is;

}STRUCT_MPIO_CONFIG_DETAILS;

typedef struct
{
    UINT_8                              is_usr[MAX_USR_ACT];
    INT_8                               usrname[MAX_USR_ACT][MAX_USR_ALPHABET];
    INT_8                               pass[MAX_USR_ACT][MAX_USR_ALPHABET];

}STRUCT_USER_DETAILS;




//  MSGS

typedef struct
{
    STRUCT_MESSAGE_HEADER           msg_hdr;
    UINT_8                          mcm_state;
    UINT_8                          current_state;
    UINT_32                         epoch_sec;

}STRUCT_MCM_HEALTH_MSG;

typedef struct
{
    STRUCT_MESSAGE_HEADER           msg_hdr;
    UINT_8                          user_mode;
    UINT_8                          mdm_id;
//    UINT_8                          user_name[20];
//    UINT_8                          password[20];

}STRUCT_MPIO_INIT_MSG;

typedef struct
{
    STRUCT_MESSAGE_HEADER               msg_hdr;
    UINT_8                              mpio_id;
    UINT_8                              mpio_type;
    STRUCT_CARD_IP_DETAILS              card_config[SLOT_10S];

}STRUCT_MPIO_10S_CARD_DETAILS;

typedef struct
{
    STRUCT_MESSAGE_HEADER               msg_hdr;
    UINT_8                              usr_mode;
    UINT_8                              mdm_id;
    INT_8                               no_of_mpio;

}STRUCT_MPIO_INIT_REPLY_MSG;

typedef struct
{
    STRUCT_MESSAGE_HEADER               msg_hdr;
    UINT_8                              mpio_id;
    UINT_8                              mdm_id;
    STRUCT_MPIO_CONFIG_DETAILS          mpio;

}STRUCT_MPIO_INIT_DETAIL_MSG;

typedef struct
{
    STRUCT_MESSAGE_HEADER               msg_hdr;
    UINT_8                              mpio_id;
    UINT_8                              mpio_type;
    UINT_8                              slot_no;
    UINT_8                              port_no;
    UINT_8                              interface;
    STRUCT_CARD_CONFIG_DETAILS          eqpt;

}STRUCT_MPIO_EDIT_DETAIL_MSG;

typedef struct
{
    STRUCT_MESSAGE_HEADER               msg_hdr;
    UINT_8                              mpio_id;
}STRUCT_MPIO_CONFIG_ERROR_MSG;

typedef struct
{
    STRUCT_MESSAGE_HEADER               msg_hdr;
    UINT_8                              usr_status;

}STRUCT_MPIO_INIT_REPLY_ERROR_MSG;

typedef struct
{
    UINT_8                                  channel_no;
    UINT_8                                  eqpt_id;
    UINT_8                                  eqpt_status;

}STRUCT_POLL_EQPT_MSG;

typedef struct
{
    STRUCT_MESSAGE_HEADER               msg_hdr;
    UINT_32                             epoch_sec;
    UINT_8                              mpio_id;
    STRUCT_POLL_EQPT_MSG                eqpt[MAX_EQPT];

}STRUCT_MPIO_STATUS_MSG;

typedef struct
{
    STRUCT_MESSAGE_HEADER               msg_hdr;
    STRUCT_POLL_EQPT_MSG                eqpt[MAX_EQPT];

}STRUCT_ALL_MPIO_STATUS_MSG;

typedef struct
{
    STRUCT_POLL_EQPT_MSG                eqpt[MAX_EQPT];

}STRUCT_LOCAL_MPIO_STATUS_MSG;

typedef struct
{
    int                                 no_of_eqpt;
    int                                 eqpt[MAX_EQPT];

}STRUCT_MPIO_EQPT_INFO;

typedef struct
{
    STRUCT_MESSAGE_HEADER           msg_hdr;
    UINT_8                          eqpt_id;
    UINT_8                          mpio_id;
    UINT_8                          slot_no;
    UINT_8                          port_no;
    UINT_8                          time;

}STRUCT_LIVE_DATA_MSG;

typedef struct
{
    STRUCT_MESSAGE_HEADER           msg_hdr;
    UINT_8                          eqpt_id;
    UINT_8                          mpio_id;
    UINT_8                          slot_no;
    UINT_8                          port_no;
    UINT_8                          resp_id;

}STRUCT_LIVE_DATA_RESP_MSG;

typedef struct
{
    UINT_16                             day;
    UINT_8                              hour;
    UINT_8                              minute;
    UINT_8                              second;

}STRUCT_TIME_MSG;

typedef struct
{
    STRUCT_MESSAGE_HEADER               msg_hdr;
    UINT_8                              mpioId;//7
    char                                status[11];//8 + slot No  ( 9 = swithc1,14 =slot6, 18 system)
    STRUCT_TIME_MSG                     time[11];
    char                                crc;

}STRUCT_MPIO_CARD_STATUS_MSG;

#endif // MDM_STRUCT

