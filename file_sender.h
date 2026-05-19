#ifndef FILESENDER_H
#define FILESENDER_H
#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h>
#include <libssh/sftp.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mcm_constant.h"
#define MAX_XFER_BUF_SIZE 16384



class FileSender
{
public:
    int sendFile(const char* ip, const char* loc1, const char* loc2, int senderVal);
//    int sendFile(const char* ip, const char* loc1, const char* loc2, int mcmId);
    void free_channel(ssh_channel channel);
    void free_session(ssh_session session);
    void error(ssh_session session);
    int sftp_read_sync(ssh_session session, sftp_session sftp, const char* loc1, const char* loc2);

    int killMcm1();
    int killMcm2();
};

#endif // FILESENDER_H
