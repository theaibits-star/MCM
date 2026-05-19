#ifndef PRINT_MSG_H
#define PRINT_MSG_H
#include "mcm_struct.h"

class PrintMsg
{
public:
    PrintMsg();
    void printAddEqpt(char *buffer, int msgLen);
    void liveDataEqpt(char *buffer, int msgLen);
    void EqptStatusEqpt(char *buffer, int msgLen);
    void CardStatus(char *buffer, int msgLen);
    void EqptStatusSend(char *buffer, int msgLen);
};

#endif // PRINT_MSG_H
