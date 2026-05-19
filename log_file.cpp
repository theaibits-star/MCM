#include "log_file.h"
#include "mcm_data.h"

LogFile::LogFile(QObject *parent) :
    QThread(parent)
{
    mo_mcmData =McmData::getMcmDataInstance();
}

void LogFile::run()
{
    connect(mo_mcmData->mo_dataCntrl, SIGNAL(mpioStatus(char*)), this , SLOT(makeDifference(char*)));
    exec();
}

void LogFile::makeDifference(char *buffer)
{

}
