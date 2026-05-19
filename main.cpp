#include <QCoreApplication>
#include "mcm_receiver.h"
#include "data_controller.h"
#include <mcm_data.h>
#include<sstream>
#include<unistd.h>
#include<QStringList>


int main(int argc, char *argv[])
{
//    qDebug()<<" size is "<<sizeof(STRUCT_MPIO_STATUS_MSG) <<" card "<< sizeof(STRUCT_MPIO_CARD_STATUS_MSG);
    sleep(10);
    QCoreApplication a(argc, argv);

            pid_t pid = getpid();
            std::stringstream str;
            str<<pid;
            std::stringstream command;
            command <<"pidof MCM > process.txt";
            system(command.str().c_str());
            QFile p_file("process.txt");
            if(p_file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&p_file);
                QString line = in.readLine();
                QStringList str_lst;
                QString id=str.str().c_str();
                while (!line.isNull())
                {
                    str_lst=line.split(" ");
                    for(int i=0;i<str_lst.size();i++)
                    {

                        if(str_lst.at(i).compare(id)!=0)
                        {

                            QString kill_cmd;
                            kill_cmd=QString("kill -9 %1").arg(str_lst.at(i));
                            int success=system(kill_cmd.toStdString().c_str());
                            if(success==0)
                            {
                                qDebug()<<"Already running MCM app is killed with PID"<<str_lst.at(i);

                            }
                        }

                    }

                    line = in.readLine();
                }

            }
            p_file.close();


    qDebug()<<"MCM is running here";
    McmData *mo_mcmData = McmData::getMcmDataInstance();
     mo_mcmData->mo_mcmCntrol = new McmController;
    McmReceiver *o_mcmRecv = new McmReceiver;


    o_mcmRecv->initializeSocket();

    mo_mcmData->mo_dataCntrl = new DataController;
    mo_mcmData->mo_dataCntrl->runThread();



    qDebug()<<" MCM came here";

    return a.exec();
}
