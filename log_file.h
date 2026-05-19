#ifndef LOG_FILE_H
#define LOG_FILE_H

#include <QThread>


class McmData;
class LogFile : public QThread
{
    Q_OBJECT
public:
    explicit LogFile(QObject *parent = 0);
    void run();
    void runThread();
    void setDataContollerObject();

private:
    void filterRecvMsg(char* buffer, int msgLen);

private:
    McmData *mo_mcmData;

signals:

public slots:
    void makeDifference(char *);

};

#endif // LOG_FILE_H
