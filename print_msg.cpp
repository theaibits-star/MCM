#include "print_msg.h"
#include <stdio.h>


PrintMsg::PrintMsg()
{
}

void PrintMsg::printAddEqpt(char *buffer, int msgLen)//Add and delete eqpt
{
//    STRUCT_MPIO_EDIT_DETAIL_MSG *msg = (STRUCT_MPIO_EDIT_DETAIL_MSG*)buffer;
//    printf(" source %d \n", msg->msg_hdr.source_id);
//    printf(" dest %d \n", msg->msg_hdr.dest_id);
//    printf(" msgid %d \n", msg->msg_hdr.msg_id);
//    printf(" len %d \n", msg->msg_hdr.msg_len);

//    printf(" mpio_id %d \n", msg->mpio_id);
//    printf(" mpio_type %d \n", msg->mpio_type);
//    printf(" slot_no %d \n", msg->slot_no);
//    printf(" port_no %d \n", msg->port_no);
//    printf(" interface %d \n", msg->interface);

//    printf(" eqpt_id %d \n", msg->eqpt.eqpt_id);
//    printf(" baudrate %d \n", msg->eqpt.baudrate);
//    printf(" parity %d \n", msg->eqpt.parity);
//    printf(" stopbit %d \n", msg->eqpt.stopbit);


//    printf("\n \n \n");
//    fflush(stdout);

}

void PrintMsg::liveDataEqpt(char *buffer, int msgLen)//Add and delete eqpt
{
//    STRUCT_LIVE_DATA_MSG *msg = (STRUCT_LIVE_DATA_MSG*)buffer;
//    printf(" source %d \n", msg->msg_hdr.source_id);
//    printf(" dest %d \n", msg->msg_hdr.dest_id);
//    printf(" msgid %d \n", msg->msg_hdr.msg_id);
//    printf(" len %d \n", msg->msg_hdr.msg_len);

//    printf(" eqpt_id %d \n", msg->eqpt_id);
//    printf(" mpio_id %d \n", msg->mpio_id);
//    printf(" slot_no %d \n", msg->slot_no);
//    printf(" port_no %d \n", msg->port_no);
//    printf(" time %d \n", msg->time);


//    printf("\n \n \n");
//    fflush(stdout);

}

void PrintMsg::EqptStatusEqpt(char *buffer, int msgLen)
{
//    STRUCT_MPIO_STATUS_MSG *msg =(STRUCT_MPIO_STATUS_MSG*)buffer;

//        printf("eqpt status recv mesg \n");
//        printf(" source %d \n", msg->msg_hdr.source_id);
//        printf(" dest %d \n", msg->msg_hdr.dest_id);
//        printf(" msgid %d \n", msg->msg_hdr.msg_id);
//        printf(" len %d \n", msg->msg_hdr.msg_len);
//        printf(" epoch sec %d \n", msg->epoch_sec);
//        printf(" mpio_id %d \n", msg->mpio_id);

//        for(int i =0 ; i< MAX_EQPT; i++)
//        {
//            printf(" index %d channel no %d \n",i,msg->eqpt[i].channel_no);
//            printf(" index %d eqpt id %d \n",i,msg->eqpt[i].eqpt_id);
//            printf(" index %d eqpt status %d \n",i,msg->eqpt[i].eqpt_status);
//        }




//        printf("\n \n \n");
//        fflush(stdout);



}
void PrintMsg::EqptStatusSend(char *buffer, int msgLen)
{
//    STRUCT_ALL_MPIO_STATUS_MSG *msg = (STRUCT_ALL_MPIO_STATUS_MSG*)buffer;
//    printf(" source %d \n", msg->msg_hdr.source_id);
//    printf(" dest %d \n", msg->msg_hdr.dest_id);
//    printf(" msgid %d \n", msg->msg_hdr.msg_id);
//    printf(" len %d \n", msg->msg_hdr.msg_len);
//    for(int i =0 ; i< MAX_EQPT; i++)
//    {
//        printf(" index %d channel no %d \n",i,msg->eqpt[i].channel_no);
//        printf(" index %d eqpt id %d \n",i,msg->eqpt[i].eqpt_id);
//        printf(" index %d eqpt status %d \n",i,msg->eqpt[i].eqpt_status);
//    }




//    printf("\n \n \n");
//    fflush(stdout);

}

void PrintMsg::CardStatus(char *buffer, int msgLen)//Card Status
{
//    STRUCT_MESSAGE_HEADER *msg = (STRUCT_MESSAGE_HEADER*)buffer;
//    printf(" source %d \n", msg->source_id);
//    printf(" dest %d \n", msg->dest_id);
//    printf(" msgid %d \n", msg->msg_id);
//    printf(" len %d \n", msg->msg_len);

//    printf(" mpio id %d \n",msg->mpioId);

//    for(int i=0; i< 11; i++)
//    {
//        printf(" index %d status %d \n",i,msg->status[i]);

//    }
//    for(int i=0 ; i< 11; i++)
//    {
//        printf(" index %d day %d \n",i,msg->time[i].day);
//        printf(" index %d hour %d \n",i,msg->time[i].hour);
//        printf(" index %d minute %d \n",i,msg->time[i].minute);
//        printf(" index %d second %d \n",i,msg->time[i].second);
//      }
//        printf("  crc %d \n",msg->crc);





//    printf("\n \n \n");
//    fflush(stdout);

}
