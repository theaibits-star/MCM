#include "file_sender.h"
#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h>
#include <libssh/sftp.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <QDebug>

#define MAX_XFER_BUF_SIZE 16384


int FileSender::sendFile(const char *ip, const char *loc1, const char *loc2, int senderVal)
{
    ssh_session session;
    ssh_channel channel;
    sftp_session sftp;
    int rc, port = 22;


    printf("Session...\n");
    session = ssh_new();
    if (session == NULL) exit(-1);

    ssh_options_set(session, SSH_OPTIONS_HOST, ip);
    ssh_options_set(session, SSH_OPTIONS_PORT, &port);
    ssh_options_set(session, SSH_OPTIONS_USER, "root");

    printf("Connecting...\n");
    rc = ssh_connect(session);
    if (rc != SSH_OK)
    {

//        error(session);
        return 1;
    }

    printf("Password Autentication...\n");
    if(senderVal == 0)
    {
       rc = ssh_userauth_password(session,"root","root123");
    }
    else
    {
        rc = ssh_userauth_password(session,"root","root123");//for MCM
//        rc = ssh_userauth_password(session,"root","belnhms123");//for MCM
    }

    if (rc != SSH_AUTH_SUCCESS)
    {
//        error(session);
        return 2;
    }

    printf("Channel...\n");
    channel = ssh_channel_new(session);
    if (channel == NULL)
    {
        return 3;
    }


    printf("Opening...\n");
    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK)
    {
//        error(session);
        return 4;
    }

    printf("SFTP session...\n");
    sftp = sftp_new(session);
    if (sftp == NULL)
    {
        fprintf(stderr, "Error allocating SFTP session: %s\n",
                ssh_get_error(session));
        return 5;
    }
    rc = sftp_init(sftp);
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Error initializing SFTP session: %d.\n",
                sftp_get_error(sftp));
        sftp_free(sftp);
        return 6;
    }

    printf("Reading File...\n");
    rc = sftp_read_sync(session, sftp, loc1, loc2);
    if (rc != SSH_OK)
    {
        sftp_free(sftp);
        free_channel(channel);
        free_session(session);
        return 7;
    }

    else
    {
        sftp_free(sftp);
        free_channel(channel);
        free_session(session);
        return 0;
    }
}

//int FileSender::sendFile(const char* ip, const char*loc1, const char*loc2, int mcmId)
//{
//    ssh_session session;
//    ssh_channel channel;
//    sftp_session sftp;
//    int rc, port = 22;


//    printf("Session...\n");
//    session = ssh_new();
//    if (session == NULL) exit(-1);

//    ssh_options_set(session, SSH_OPTIONS_HOST, ip);
//    ssh_options_set(session, SSH_OPTIONS_PORT, &port);
//    ssh_options_set(session, SSH_OPTIONS_USER, "root");

//    printf("Connecting...\n");
//    rc = ssh_connect(session);
//    if (rc != SSH_OK)
//    {

////        error(session);
//        return 1;
//    }

//    printf("Password Autentication...\n");
//    rc = ssh_userauth_password(session,"root","root123");
//    if (rc != SSH_AUTH_SUCCESS)
//    {
////        error(session);
//        return 2;
//    }

//    printf("Channel...\n");
//    channel = ssh_channel_new(session);
//    if (channel == NULL)
//    {
//        return 3;
//    }


//    printf("Opening...\n");
//    rc = ssh_channel_open_session(channel);
//    if (rc != SSH_OK)
//    {
////        error(session);
//        return 4;
//    }

//    printf("SFTP session...\n");
//    sftp = sftp_new(session);
//    if (sftp == NULL)
//    {
//        fprintf(stderr, "Error allocating SFTP session: %s\n",
//                ssh_get_error(session));
//        return 5;
//    }
//    rc = sftp_init(sftp);
//    if (rc != SSH_OK)
//    {
//        fprintf(stderr, "Error initializing SFTP session: %d.\n",
//                sftp_get_error(sftp));
//        sftp_free(sftp);
//        return 6;
//    }

//    printf("Reading File...\n");
//    rc = sftp_read_sync(session, sftp, loc1, loc2);
//    if (rc != SSH_OK)
//    {
//        sftp_free(sftp);
//        free_channel(channel);
//        free_session(session);
//        return 7;
//    }

//    else
//    {
//        sftp_free(sftp);
//        free_channel(channel);
//        free_session(session);
//        return 0;
//    }
//}

void FileSender::free_channel(ssh_channel channel)
{
    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
}

void FileSender::free_session(ssh_session session)
{
    ssh_disconnect(session);
    ssh_free(session);
}

void FileSender::error(ssh_session session)
{
    fprintf(stderr, "Error: %s\n", ssh_get_error(session));
    free_session(session);
    exit(-1);
}

int FileSender::sftp_read_sync(ssh_session session, sftp_session sftp, const char* loc1, const char* loc2)
{
    int access_type;
    sftp_file file;
    char* buffer[MAX_XFER_BUF_SIZE];
    int nbytes, nwritten, rc;
    int fd;
    access_type = O_WRONLY | O_CREAT | O_TRUNC;
    file = sftp_open(sftp, loc1,
                     access_type, S_IRWXU);
    if (file == NULL) {
        fprintf(stderr, "Can't open file for writing: %s\n",
                ssh_get_error(session));
        return SSH_ERROR;
    }
    fd = open(loc2, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Can't open file for reading: %s\n",
                strerror(errno));
        return SSH_ERROR;
    }
    for (;;) {

        nwritten = read(fd, buffer, sizeof(buffer));
        if (nwritten < 0)
        {
            fprintf(stderr, "Error Reading: %s\n",
                    strerror(errno));
            sftp_close(file);
            return SSH_ERROR;
        }
        nbytes = sftp_write(file, buffer, nwritten);
        if (nbytes == 0)
        {
            break; // EOF
        }
        else if (nbytes < 0)
        {
            fprintf(stderr, "Error Writing: %s\n",
                    ssh_get_error(session));
            sftp_close(file);
            return SSH_ERROR;
        }

    }

    rc = sftp_close(file);
    if (rc != SSH_OK) {
        fprintf(stderr, "Can't close the read file: %s\n",
                ssh_get_error(session));
        return rc;
    }
    return SSH_OK;
}

int FileSender::killMcm1()
{
    ssh_session session;
    ssh_channel channel;
    sftp_session sftp;
    int rc, port = 22;

    qDebug()<<"Session...\n";

    session = ssh_new();

    qDebug()<<"SSH session:"<<session;

    if (session == NULL)
        exit(-1);

    ssh_options_set(session, SSH_OPTIONS_HOST,MCM_P_IP);
    ssh_options_set(session, SSH_OPTIONS_PORT, &port);
    ssh_options_set(session, SSH_OPTIONS_USER, "root");

    qDebug()<<"Connecting...\n";
    rc = ssh_connect(session);

    qDebug()<<"Rc is:"<<rc;
    if (rc != SSH_OK)
    {

        //        error(session);
        return 1;
    }

    qDebug()<<"Password Authentication...\n";
//    rc = ssh_userauth_password(session,"root","belnhms123");
    rc = ssh_userauth_password(session,"root","root123");

    qDebug()<<"Rc is:"<<rc;
    if (rc != SSH_AUTH_SUCCESS)
    {
        //        error(session);
        return 2;
    }

    qDebug()<<"Channel...\n";
    channel = ssh_channel_new(session);

    qDebug()<<channel;
    if (channel == NULL)
    {
        return 3;
    }


    qDebug()<<"Opening...\n";
    rc = ssh_channel_open_session(channel);
    qDebug()<<"Rc is:"<<rc;
    if (rc != SSH_OK)
    {
        //        error(session);
        return 4;
    }

    qDebug()<<"SFTP session...\n";
    sftp = sftp_new(session);

    qDebug()<<"SFTP session:"<<sftp;

    if (sftp == NULL)
    {
        fprintf(stderr, "Error allocating SFTP session: %s\n",
                ssh_get_error(session));
        return 5;
    }
    rc = sftp_init(sftp);
    qDebug()<<"Rc is:"<<rc;
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Error initializing SFTP session: %d.\n",
                sftp_get_error(sftp));
        sftp_free(sftp);
        return 6;
    }


    qDebug()<<"Debugging Executed";

    QString cmd = "DISPLAY=:0 killall MCM \n DISPLAY=:0 screen -d -m /home/MPIO/SOFTWARE/MCM";

    rc = ssh_channel_request_exec(channel,cmd.toStdString().c_str());

    qDebug()<<"Rc is:"<<rc;

    if(rc != SSH_OK)
    {


        return 7;

    }


        sftp_free(sftp);
        free_channel(channel);
        free_session(session);


    return 0;
}

int FileSender::killMcm2()
{
    ssh_session session;
    ssh_channel channel;
    sftp_session sftp;
    int rc, port = 22;

    qDebug()<<"Session...\n";

    session = ssh_new();

    qDebug()<<"SSH session:"<<session;

    if (session == NULL)
        exit(-1);

    ssh_options_set(session, SSH_OPTIONS_HOST, MCM_S_IP);
    ssh_options_set(session, SSH_OPTIONS_PORT, &port);
    ssh_options_set(session, SSH_OPTIONS_USER, "root");

    qDebug()<<"Connecting...\n";
    rc = ssh_connect(session);

    qDebug()<<"Rc is:"<<rc;
    if (rc != SSH_OK)
    {

        //        error(session);
        return 1;
    }

    qDebug()<<"Password Authentication...\n";
//    rc = ssh_userauth_password(session,"root","belnhms123");
    rc = ssh_userauth_password(session,"root","root123");

    qDebug()<<"Rc is:"<<rc;
    if (rc != SSH_AUTH_SUCCESS)
    {
        //        error(session);
        return 2;
    }

    qDebug()<<"Channel...\n";
    channel = ssh_channel_new(session);

    qDebug()<<channel;
    if (channel == NULL)
    {
        return 3;
    }


    qDebug()<<"Opening...\n";
    rc = ssh_channel_open_session(channel);
    qDebug()<<"Rc is:"<<rc;
    if (rc != SSH_OK)
    {
        //        error(session);
        return 4;
    }

    qDebug()<<"SFTP session...\n";
    sftp = sftp_new(session);

    qDebug()<<"SFTP session:"<<sftp;

    if (sftp == NULL)
    {
        fprintf(stderr, "Error allocating SFTP session: %s\n",
                ssh_get_error(session));
        return 5;
    }
    rc = sftp_init(sftp);
    qDebug()<<"Rc is:"<<rc;
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Error initializing SFTP session: %d.\n",
                sftp_get_error(sftp));
        sftp_free(sftp);
        return 6;
    }


    qDebug()<<"Debugging Executed";

    QString cmd = "DISPLAY=:0 killall MCM \n DISPLAY=:0 screen -d -m /home/MPIO/SOFTWARE/MCM";

    rc = ssh_channel_request_exec(channel,cmd.toStdString().c_str());

    qDebug()<<"Rc is:"<<rc;

    if(rc != SSH_OK)
    {


        return 7;

    }


        sftp_free(sftp);
        free_channel(channel);
        free_session(session);


    return 0;
}
