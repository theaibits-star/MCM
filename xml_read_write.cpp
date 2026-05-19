#include "xml_read_write.h"
#include "mcm_data.h"
#include <QDebug>
#include <QDomDocument>

#include <QDomNodeList>
#include <QString>
#include <health_sender.h>
#include <mcm_constant.h>

XmlReadWrite::XmlReadWrite()
{
    mo_mcmData = McmData::getMcmDataInstance();
   QObject::connect(this, SIGNAL(stopHltTimer()),mo_mcmData->mo_hlthSender, SLOT(stopTimer()));
   QObject::connect(this, SIGNAL(startHltTimer()),mo_mcmData->mo_hlthSender, SLOT(startTimer()));
   QObject::connect(this, SIGNAL(stopMcmContTimer()),mo_mcmData->mo_mcmCntrol, SLOT(stopTimerMcm()));
   QObject::connect(this, SIGNAL(startMcmContTimer()),mo_mcmData->mo_mcmCntrol, SLOT(startTimerMcm()));


//   QObject::connect(this, SIGNAL(stopHltTimer()),HealthSender::getHealthSenderDataInstance(), SLOT(stopTimer()));
//   QObject::connect(this, SIGNAL(startHltTimer()),HealthSender::getHealthSenderDataInstance(), SLOT(startTimer()));
//   QObject::connect(this, SIGNAL(stopMcmContTimer()),McmController::getMcmControllerInstance(), SLOT(stopTimerMcm()));
//   QObject::connect(this, SIGNAL(startMcmContTimer()),McmController::getMcmControllerInstance(), SLOT(startTimerMcm()));


}

bool XmlReadWrite::writeMpio10scardDetails(STRUCT_MPIO_INIT_DETAIL_MSG *s_mpio)
{
    bool checkFileStatus = true;
    bool writeStatus = false;
    QFile file("/home/MPIO/SDN_CONFIG.xml");
    if(!file.open(QIODevice::ReadOnly))
    {
        checkFileStatus = false;
        ////qDebug()<<"SDN_CONFIG file is going to be create";

    }

    if(checkFileStatus == false)
    {
        stopHltTimer();
//        mo_mcmData->mo_hlthSender->stopTimer();
        writeStatus = createMpioDetail(s_mpio);
//        mo_mcmData->mo_hlthSender->startTimer();
        startHltTimer();

    }
    else
    {
        file.close();
        stopHltTimer();
//        mo_mcmData->mo_hlthSender->stopTimer();
        writeStatus = addMpioDetail(s_mpio);// add delete both
//        mo_mcmData->mo_hlthSender->startTimer();
        startHltTimer();

    }


    return writeStatus;
}

void XmlReadWrite::adduserTest()
{
    QDomDocument doc;
    QDomElement root = doc.createElement("UserManagement");
    doc.appendChild(root);

    QDomElement usr = doc.createElement("Admin");
    usr.setAttribute("username","crl123");
    usr.setAttribute("password","crl@123");
    root.appendChild(usr);

    QDomElement usr1 = doc.createElement("User1");
    usr1.setAttribute("username","crlusr");
    usr1.setAttribute("password","crl@123");
    root.appendChild(usr1);

    QFile file("/home/MPIO/USER_MGT.xml");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        //qDebug()<<"open USER_MGT.xml for writng fail";
    }
    else
    {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
        //qDebug()<<"USER_MGT.xml writing is done";
    }
}

bool XmlReadWrite::createMpioDetail(STRUCT_MPIO_INIT_DETAIL_MSG *s_mpio)
{
    m_eqptNo = 0;
    QDomDocument doc;
    QDomElement root = doc.createElement("time");
    gettimeofday(&curSecond, NULL);

    mo_mcmData->m_epochTime = curSecond.tv_sec;
    root.setAttribute("value",mo_mcmData->m_epochTime);
    doc.appendChild(root);

    QDomElement mpio = doc.createElement("MPIO");
    mpio.setAttribute("MPIO_ID",s_mpio->mpio_id);
    mpio.setAttribute("MPIO_TYPE",s_mpio->mpio.mpio_type);
    root.appendChild(mpio);

    for(int slotNo = 0; slotNo < 10; slotNo++)
    {
        QDomElement slot = doc.createElement("SLOT");
        slot.setAttribute("SLOT_NO",slotNo + 1);
        slot.setAttribute("CARD_TYPE",getCardType(slotNo));
        slot.setAttribute("CARD_STATUS",s_mpio->mpio.mpio_is.mpio_10s.card_status[slotNo]);
        if(slotNo == 0)
        {
            slot.setAttribute("IP",s_mpio->mpio.mpio_is.mpio_10s.switch1_ip);
        }
        else if(slotNo == 1)
        {
            slot.setAttribute("IP",s_mpio->mpio.mpio_is.mpio_10s.switch2_ip);
        }
        else if(slotNo == 2)
        {
            slot.setAttribute("IP",s_mpio->mpio.mpio_is.mpio_10s.sr_dr_ip);
        }
        else if(slotNo == 3)
        {
            slot.setAttribute("IP",s_mpio->mpio.mpio_is.mpio_10s.sr_dr_ip);
        }
        else if(slotNo > 3 && slotNo < 9)
        {
            //qDebug()<<"ip of slot "<< slotNo<< " is "<<s_mpio->mpio.mpio_is.mpio_10s.cc_ip[slotNo - 4];
            slot.setAttribute("IP",s_mpio->mpio.mpio_is.mpio_10s.cc_ip[slotNo - 4]);
        }
        else
        {
            slot.setAttribute("IP",s_mpio->mpio.mpio_is.mpio_10s.sys_ip);
        }
        mpio.appendChild(slot);

        if(s_mpio->mpio.mpio_is.mpio_10s.card_status[slotNo] == 1)
        {
            if(slotNo == 2)
            {
                for(int i = 0; i < MAX_SRDC_DSRC_PORT; i++)
                {
                    m_eqptId = s_mpio->mpio.mpio_is.mpio_10s.sr_dr_card[i].eqpt_id;

                    QDomElement port = doc.createElement("PORT");
                    port.setAttribute("PORT_NO",i + 1);
                    port.setAttribute("EQPT",m_eqptId);
                    slot.appendChild(port);

                    m_eqptIndex = mo_mcmData->m_eqptIndex[m_eqptId];
                    if(m_eqptId > 0)
                    {
                        mo_mcmData->ms_eqptInfo[s_mpio->mpio_id].eqpt[m_eqptNo++] = m_eqptId;
                    }
                    mo_mcmData->m_eqpt[m_eqptIndex][INDEX_PORT] = i + 1;
                    mo_mcmData->m_eqpt[m_eqptIndex][INDEX_SLOT] = slotNo + 1;
                    mo_mcmData->m_eqpt[m_eqptIndex][INDEX_STATUS] = 1;

                    mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_status = DOWN_ERR;
                    mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_id = m_eqptId;
                    mo_mcmData->ms_mpios->eqpt[m_eqptIndex].channel_no = i + 1;
                }
            }
            else if(slotNo == 3)
            {
                for(int i = 0; i < MAX_DISCRETE_PORT; i++)
                {
                    QDomElement port = doc.createElement("PORT");
                    port.setAttribute("PORT_NO",i + 1);
                    port.setAttribute("EQPT",s_mpio->mpio.mpio_is.mpio_10s.discrt_card[i].eqpt_id);
                    slot.appendChild(port);
                }
            }
            else if(slotNo == 4)
            {
                for(int i = 0; i < MAX_SERIAL_PORT; i++)
                {
                    QDomElement port = doc.createElement("PORT");
                    port.setAttribute("PORT_NO",i + 1);
                    port.setAttribute("EQPT",s_mpio->mpio.mpio_is.mpio_10s.cc_card[slotNo - 4][i].eqpt_id);
                    slot.appendChild(port);

                }
            }
            else if(slotNo > 4 && slotNo < 9)
            {
                for(int i = 0; i < MAX_SERIAL_PORT; i++)
                {
                    m_eqptId = s_mpio->mpio.mpio_is.mpio_10s.cc_card[slotNo-4][i].eqpt_id;
                    QDomElement port = doc.createElement("PORT");
                    port.setAttribute("PORT_NO",i + 1);
                    port.setAttribute("EQPT",m_eqptId);
                    port.setAttribute("BAUDRATE",s_mpio->mpio.mpio_is.mpio_10s.cc_card[slotNo-4][i].baudrate);
                    port.setAttribute("PARITY",s_mpio->mpio.mpio_is.mpio_10s.cc_card[slotNo-4][i].parity);
                    port.setAttribute("STOPBIT",s_mpio->mpio.mpio_is.mpio_10s.cc_card[slotNo-4][i].stopbit);
                    slot.appendChild(port);

                    m_eqptIndex = mo_mcmData->m_eqptIndex[m_eqptId];
                    if(m_eqptId > 0)
                    {
                        mo_mcmData->ms_eqptInfo[s_mpio->mpio_id].eqpt[m_eqptNo++] = m_eqptId;
                    }
                    mo_mcmData->m_eqpt[m_eqptIndex][INDEX_PORT] = i + 1;
                    mo_mcmData->m_eqpt[m_eqptIndex][INDEX_SLOT] = slotNo + 1;
                    mo_mcmData->m_eqpt[m_eqptIndex][INDEX_STATUS] = 1;

                    mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_status = DOWN_ERR;
                    mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_id = m_eqptId;
                    mo_mcmData->ms_mpios->eqpt[m_eqptIndex].channel_no = i + 1;
                }
            }
        }

    }

    mo_mcmData->ms_eqptInfo[s_mpio->mpio_id].no_of_eqpt = m_eqptNo;

    QFile file("/home/MPIO/SDN_CONFIG.xml");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        //qDebug()<<"open SDN_CONFIG.xml for writng fail";
        return false;
    }
    else
    {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
        //qDebug()<<"SDN_CONFIG.xml writing is done";
        mo_mcmData->ms_config_file = true;
        return true;

    }
}


bool XmlReadWrite::addMpioDetail(STRUCT_MPIO_INIT_DETAIL_MSG *s_mpio)//sdd delete both
{
    m_eqptNo = 0;
    //qDebug()<<" came for updating xml file";
    QFile file("/home/MPIO/SDN_CONFIG.xml");
    QDomDocument doc("SDN_CONFIG");

    if(!doc.setContent(&file))
    {
        //qDebug()<<"can not parse the file for update";
        return false;
    }
    file.close();

    gettimeofday(&curSecond, NULL);
    mo_mcmData->m_epochTime = curSecond.tv_sec;

    QDomElement root = doc.documentElement();
    root.setAttribute("value",mo_mcmData->m_epochTime);
    QDomElement mpio = doc.createElement("MPIO");
    mpio.setAttribute("MPIO_ID",s_mpio->mpio_id);
    mpio.setAttribute("MPIO_TYPE",s_mpio->mpio.mpio_type);
    root.appendChild(mpio);

    for(int slotNo = 0; slotNo < 10; slotNo++)
    {
        QDomElement slot = doc.createElement("SLOT");
        slot.setAttribute("SLOT_NO",slotNo + 1);
        slot.setAttribute("CARD_TYPE",getCardType(slotNo));
        slot.setAttribute("CARD_STATUS",s_mpio->mpio.mpio_is.mpio_10s.card_status[slotNo]);
        if(slotNo == 0)
        {
            slot.setAttribute("IP",s_mpio->mpio.mpio_is.mpio_10s.switch1_ip);
        }
        else if(slotNo == 1)
        {
            slot.setAttribute("IP",s_mpio->mpio.mpio_is.mpio_10s.switch2_ip);
        }
        else if(slotNo == 2)
        {
            slot.setAttribute("IP",s_mpio->mpio.mpio_is.mpio_10s.sr_dr_ip);
        }
        else if(slotNo == 3)
        {
            slot.setAttribute("IP",s_mpio->mpio.mpio_is.mpio_10s.sr_dr_ip);
        }
        else if(slotNo > 3 && slotNo < 9)
        {
            slot.setAttribute("IP",s_mpio->mpio.mpio_is.mpio_10s.cc_ip[slotNo - 4]);
        }
        else
        {
            slot.setAttribute("IP",s_mpio->mpio.mpio_is.mpio_10s.sys_ip);
        }
        mpio.appendChild(slot);

        if(s_mpio->mpio.mpio_is.mpio_10s.card_status[slotNo] == 1)
        {
            if(slotNo == 2)
            {
                for(int i = 0; i < MAX_SRDC_DSRC_PORT; i++)
                {
                    m_eqptId = s_mpio->mpio.mpio_is.mpio_10s.sr_dr_card[i].eqpt_id;
                    QDomElement port = doc.createElement("PORT");
                    port.setAttribute("PORT_NO",i + 1);
                    port.setAttribute("EQPT",s_mpio->mpio.mpio_is.mpio_10s.sr_dr_card[i].eqpt_id);
                    slot.appendChild(port);

                    m_eqptIndex = mo_mcmData->m_eqptIndex[m_eqptId];
                    if(m_eqptId > 0)
                    {
                        mo_mcmData->ms_eqptInfo[s_mpio->mpio_id].eqpt[m_eqptNo++] = m_eqptId;
                    }
                    mo_mcmData->m_eqpt[m_eqptIndex][INDEX_PORT] = i + 1;
                    mo_mcmData->m_eqpt[m_eqptIndex][INDEX_SLOT] = slotNo + 1;
                    mo_mcmData->m_eqpt[m_eqptIndex][INDEX_STATUS] = 1;

                    mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_status = DOWN_ERR;
                    mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_id = m_eqptId;
                    mo_mcmData->ms_mpios->eqpt[m_eqptIndex].channel_no = i + 1;
                }
            }
            //            if(slotNo == 2)
            //            {
            //                for(int i = 0; i < MAX_SRDC_DSRC_PORT; i++)
            //                {
            //                    QDomElement port = doc.createElement("PORT");
            //                    port.setAttribute("PORT_NO",i + 1);
            //                    port.setAttribute("EQPT",s_mpio->mpio.mpio_is.mpio_10s.sr_dr_card[i].eqpt_id);
            //                    slot.appendChild(port);
            //                }
            //            }
            //            if(slotNo == 2)
            //            {
            //                for(int i = 0; i < MAX_SRDC_DSRC_PORT; i++)
            //                {
            //                    QDomElement port = doc.createElement("PORT");
            //                    port.setAttribute("PORT_NO",i + 1);
            //                    port.setAttribute("EQPT",s_mpio->mpio.mpio_is.mpio_10s.sr_dr_card[i].eqpt_id);
            //                    slot.appendChild(port);
            //                }
            //            }
            else if(slotNo == 3)
            {
                for(int i = 0; i < MAX_DISCRETE_PORT; i++)
                {
                    QDomElement port = doc.createElement("PORT");
                    port.setAttribute("PORT_NO",i + 1);
                    port.setAttribute("EQPT",s_mpio->mpio.mpio_is.mpio_10s.discrt_card[i].eqpt_id);
                    slot.appendChild(port);
                }
            }
            else if(slotNo == 4)
            {
                for(int i = 0; i < MAX_SERIAL_PORT; i++)
                {
                    QDomElement port = doc.createElement("PORT");
                    port.setAttribute("PORT_NO",i + 1);
                    port.setAttribute("EQPT",s_mpio->mpio.mpio_is.mpio_10s.cc_card[slotNo-4][i].eqpt_id);
                    slot.appendChild(port);
                }
            }
            else if(slotNo > 4 && slotNo < 9)
            {
                for(int i = 0; i < MAX_SERIAL_PORT; i++)
                {
                    m_eqptId = s_mpio->mpio.mpio_is.mpio_10s.cc_card[slotNo-4][i].eqpt_id;
                    QDomElement port = doc.createElement("PORT");
                    port.setAttribute("PORT_NO",i + 1);
                    port.setAttribute("EQPT",m_eqptId);
                    port.setAttribute("BAUDRATE",s_mpio->mpio.mpio_is.mpio_10s.cc_card[slotNo-4][i].baudrate);
                    port.setAttribute("PARITY",s_mpio->mpio.mpio_is.mpio_10s.cc_card[slotNo-4][i].parity);
                    port.setAttribute("STOPBIT",s_mpio->mpio.mpio_is.mpio_10s.cc_card[slotNo-4][i].stopbit);
                    slot.appendChild(port);


                    if(m_eqptId > 0)
                    {
                        m_eqptIndex = mo_mcmData->m_eqptIndex[m_eqptId];
                        mo_mcmData->ms_eqptInfo[s_mpio->mpio_id].eqpt[m_eqptNo++] = m_eqptId;
                        mo_mcmData->m_eqpt[m_eqptIndex][INDEX_PORT] = i + 1;
                        mo_mcmData->m_eqpt[m_eqptIndex][INDEX_SLOT] = slotNo + 1;
                        mo_mcmData->m_eqpt[m_eqptIndex][INDEX_STATUS] = 1;

                        mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_status = DOWN_ERR;
                        mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_id = m_eqptId;
                        mo_mcmData->ms_mpios->eqpt[m_eqptIndex].channel_no = i + 1;
                    }

                }
            }
        }
    }

    mo_mcmData->ms_eqptInfo[s_mpio->mpio_id].no_of_eqpt = m_eqptNo;

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        //qDebug()<<"open SDN_CONFIG.xml for updating fail";
        return false;
    }
    else
    {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
        //qDebug()<<"SDN_CONFIG.xml updation is done";
        mo_mcmData->ms_config_file = true;
        return true;

    }
}

int XmlReadWrite::readUserDetails()
{
    QString usrname;
    QString password;
    //qDebug()<<" came for reading xml file";
    QFile file("/home/MPIO/USER_MGT.xml");
    QDomDocument doc("USER_MGT");
    if(!file.open(QIODevice::ReadOnly))
    {

        //qDebug()<<"USER_MGT file can not be open for read";
        return 1;

    }
    if(!doc.setContent(&file))
    {
        //qDebug()<<"can not parse USER_MGT  file for reading";
        return 2;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomNode n = root.firstChild();
    while(!n.isNull()) {
        QDomElement usr = n.toElement(); // try to convert the node to an element.
        if(!usr.isNull()) {
            //qDebug() << qPrintable(usr.tagName()) << endl; // the node really is an element.
            if(usr.tagName() == "Admin")
            {
                usrname = usr.attribute("username");
                password = usr.attribute("password");


                mo_mcmData->mst_usr->is_usr[0] = 1;
                strcpy(mo_mcmData->mst_usr->usrname[0], usrname.toStdString().c_str());
                strcpy(mo_mcmData->mst_usr->pass[0], password.toStdString().c_str());
                //qDebug()<<"usr name"<<(char*)mo_mcmData->mst_usr->usrname[0]
                //                       <<" password"<<(char*) mo_mcmData->mst_usr->pass[0];
            }
            if(usr.tagName() == "User1")
            {
                usrname = usr.attribute("username");
                password = usr.attribute("password");

                mo_mcmData->mst_usr->is_usr[1] = 1;
                strcpy(mo_mcmData->mst_usr->usrname[1], usrname.toStdString().c_str());
                strcpy(mo_mcmData->mst_usr->pass[1], password.toStdString().c_str());
                //qDebug()<<"usr name"<<usrname <<" password"<< password;
            }

        }
        n = n.nextSibling();
    }
    return 0;
}
bool XmlReadWrite::addEqptDetail(STRUCT_MPIO_EDIT_DETAIL_MSG *s_eqpt)
{
    bool breakStatus = false;
    int mpioType = 0;
    int mpioId = 0;
    int slotNo = 0;
    int cardType = 0;
    int portNo = 0;
    //qDebug()<<" came for add eqpt details in xml file";
    QFile file("/home/MPIO/SDN_CONFIG.xml");
    QDomDocument doc("SDN_CONFIG");
    if(!doc.setContent(&file))
    {
        //qDebug()<<"can not parse the file for update";
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();
    gettimeofday(&curSecond, NULL);
    mo_mcmData->m_epochTime = curSecond.tv_sec;
    root.setAttribute("value",mo_mcmData->m_epochTime);
    QDomNode n = root.firstChild();
    while(!n.isNull())
    {
        QDomElement mpio = n.toElement();
        if(!mpio.isNull())
        {
            if(mpio.tagName() == "MPIO")
            {
                mpioId = mpio.attribute("MPIO_ID").toInt();
                if(mpioId == s_eqpt->mpio_id)
                {
                    QDomNode slot = mpio.firstChild();
                    while(!slot.isNull())
                    {
                        QDomElement eSlot = slot.toElement();
                        if(!eSlot.isNull())
                        {
                            if(eSlot.tagName() == "SLOT")
                            {
                                slotNo = eSlot.attribute("SLOT_NO").toInt();
                                if(slotNo == s_eqpt->slot_no)
                                {
                                    QDomNode port = eSlot.firstChild();
                                    while(!port.isNull())
                                    {
                                        QDomElement ePort = port.toElement();
                                        if(!ePort.isNull())
                                        {
                                            if(ePort.tagName() == "PORT")
                                            {
                                                portNo = ePort.attribute("PORT_NO").toInt();
                                                if(portNo == s_eqpt->port_no)
                                                {
                                                    QDomElement port = doc.createElement("PORT");
                                                    if(s_eqpt->interface == SERIAL_INTERFACE)
                                                    {
                                                        if(s_eqpt->msg_hdr.msg_id == ADD_EQPT_MSG_ID)
                                                        {
                                                            stopHltTimer();
                                                            stopMcmContTimer();

//                                                            mo_mcmData->mo_hlthSender->stopTimer();
//                                                            mo_mcmData->mo_mcmCntrol->stopTimerMcm();
                                                            mo_mcmData->mo_mcmCntrol->ms_hlt->epoch_sec = mo_mcmData->m_epochTime;

                                                            port.setAttribute("PORT_NO",s_eqpt->port_no);
                                                            port.setAttribute("EQPT",s_eqpt->eqpt.eqpt_id);
                                                            port.setAttribute("BAUDRATE",s_eqpt->eqpt.baudrate);
                                                            port.setAttribute("PARITY",s_eqpt->eqpt.parity);
                                                            port.setAttribute("STOPBIT",s_eqpt->eqpt.stopbit);
                                                            //qDebug()<<"eqpt "<< s_eqpt->eqpt.eqpt_id<< "added";

                                                            m_eqptIndex = mo_mcmData->m_eqptIndex[s_eqpt->eqpt.eqpt_id];
                                                            if(s_eqpt->eqpt.eqpt_id > 0)
                                                            {
                                                                //
                                                                mo_mcmData->ms_eqptInfo[s_eqpt->mpio_id].eqpt[mo_mcmData->ms_eqptInfo[s_eqpt->mpio_id].no_of_eqpt++]
                                                                        = s_eqpt->eqpt.eqpt_id;
                                                            }
                                                            mo_mcmData->m_eqpt[m_eqptIndex][INDEX_PORT] = s_eqpt->port_no;
                                                            mo_mcmData->m_eqpt[m_eqptIndex][INDEX_SLOT] = slotNo;
                                                            mo_mcmData->m_eqpt[m_eqptIndex][INDEX_STATUS] = 1;

                                                            mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_status = DOWN_ERR;
                                                            mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_id = m_eqptId;
                                                            mo_mcmData->ms_mpios->eqpt[m_eqptIndex].channel_no = s_eqpt->port_no;
//                                                            mo_mcmData->mo_hlthSender->startTimer();
//                                                            mo_mcmData->mo_mcmCntrol->startTimerMcm();
                                                            startHltTimer();
                                                            startMcmContTimer();
                                                        }
                                                        else if(s_eqpt->msg_hdr.msg_id == DELETE_EQPT_MSG_ID)
                                                        {
                                                            stopHltTimer();
                                                            stopMcmContTimer();
//                                                            mo_mcmData->mo_hlthSender->stopTimer();
//                                                            mo_mcmData->mo_mcmCntrol->stopTimerMcm();
                                                            mo_mcmData->mo_mcmCntrol->ms_hlt->epoch_sec = mo_mcmData->m_epochTime;
                                                            port.setAttribute("PORT_NO",s_eqpt->port_no);
                                                            port.setAttribute("EQPT","0");
                                                            port.setAttribute("BAUDRATE","0");
                                                            port.setAttribute("PARITY","0");
                                                            port.setAttribute("STOPBIT","0");
                                                            //qDebug()<<"eqpt "<< s_eqpt->eqpt.eqpt_id<< "deleted";

                                                            m_eqptIndex = mo_mcmData->m_eqptIndex[s_eqpt->eqpt.eqpt_id];
                                                            if(s_eqpt->eqpt.eqpt_id > 0)
                                                            {
                                                                m_eqptNo = 0;
                                                                memset(&ms_eqptInfo, 0, sizeof(STRUCT_MPIO_EQPT_INFO));
                                                                memcpy(&ms_eqptInfo, &mo_mcmData->ms_eqptInfo[s_eqpt->mpio_id],
                                                                        sizeof(STRUCT_MPIO_EQPT_INFO));
                                                                memset(&mo_mcmData->ms_eqptInfo[s_eqpt->mpio_id], 0,
                                                                        sizeof(STRUCT_MPIO_EQPT_INFO));
                                                                for(int i = 0; i < ms_eqptInfo.no_of_eqpt; i++)
                                                                {
                                                                    if(ms_eqptInfo.eqpt[i] == s_eqpt->eqpt.eqpt_id)
                                                                    {
                                                                        //do nothing
                                                                    }
                                                                    else
                                                                    {
                                                                        mo_mcmData->ms_eqptInfo[s_eqpt->mpio_id].eqpt[m_eqptNo++] = ms_eqptInfo.eqpt[i];
                                                                    }
                                                                }
                                                                mo_mcmData->ms_eqptInfo[s_eqpt->mpio_id].no_of_eqpt = m_eqptNo;
                                                            }
                                                            mo_mcmData->m_eqpt[m_eqptIndex][INDEX_PORT] = 0;
                                                            mo_mcmData->m_eqpt[m_eqptIndex][INDEX_SLOT] = 0;
                                                            mo_mcmData->m_eqpt[m_eqptIndex][INDEX_STATUS] = 0;

                                                            mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_status = DECONFIG_STATE;
                                                            mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_id = m_eqptId;
                                                            mo_mcmData->ms_mpios->eqpt[m_eqptIndex].channel_no = 0;
//                                                            mo_mcmData->mo_hlthSender->startTimer();
//                                                            mo_mcmData->mo_mcmCntrol->startTimerMcm();
                                                            startHltTimer();
                                                            startMcmContTimer();
                                                        }
                                                    }
                                                    else if(s_eqpt->interface == DSRC_INTERFACE ||
                                                            s_eqpt->interface == SRDC_INTERFACE ||
                                                            s_eqpt->interface == DISCRETE0_INTERFACE ||
                                                            s_eqpt->interface == DISCRETE1_INTERFACE ||
                                                            s_eqpt->interface == DISCRETE2_INTERFACE )
                                                    {
                                                        if(s_eqpt->msg_hdr.msg_id == ADD_EQPT_MSG_ID)
                                                        {
//                                                            mo_mcmData->mo_hlthSender->stopTimer();
//                                                            mo_mcmData->mo_mcmCntrol->stopTimerMcm();
                                                            stopHltTimer();
                                                            stopMcmContTimer();
                                                            mo_mcmData->mo_mcmCntrol->ms_hlt->epoch_sec = mo_mcmData->m_epochTime;
                                                            port.setAttribute("PORT_NO",s_eqpt->port_no);
                                                            port.setAttribute("EQPT",s_eqpt->eqpt.eqpt_id);
                                                            //qDebug()<<"eqpt "<< s_eqpt->eqpt.eqpt_id<< "added";

                                                            m_eqptIndex = mo_mcmData->m_eqptIndex[s_eqpt->eqpt.eqpt_id];
                                                            if(s_eqpt->eqpt.eqpt_id > 0)
                                                            {
                                                                //
                                                                mo_mcmData->ms_eqptInfo[s_eqpt->mpio_id].eqpt[mo_mcmData->ms_eqptInfo[s_eqpt->mpio_id].no_of_eqpt++]
                                                                        = s_eqpt->eqpt.eqpt_id;
                                                            }
                                                            mo_mcmData->m_eqpt[m_eqptIndex][INDEX_PORT] = s_eqpt->port_no;
                                                            mo_mcmData->m_eqpt[m_eqptIndex][INDEX_SLOT] = slotNo;
                                                            mo_mcmData->m_eqpt[m_eqptIndex][INDEX_STATUS] = 1;

                                                            mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_status = DOWN_ERR;
                                                            mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_id = m_eqptId;
                                                            mo_mcmData->ms_mpios->eqpt[m_eqptIndex].channel_no = s_eqpt->port_no;

//                                                            mo_mcmData->mo_hlthSender->startTimer();
//                                                            mo_mcmData->mo_mcmCntrol->startTimerMcm();
                                                            startHltTimer();
                                                            startMcmContTimer();

                                                        }
                                                        else if(s_eqpt->msg_hdr.msg_id == DELETE_EQPT_MSG_ID)
                                                        {
                                                            stopHltTimer();
                                                            stopMcmContTimer();
//                                                            mo_mcmData->mo_hlthSender->stopTimer();
//                                                            mo_mcmData->mo_mcmCntrol->stopTimerMcm();
                                                            mo_mcmData->mo_mcmCntrol->ms_hlt->epoch_sec = mo_mcmData->m_epochTime;
                                                            port.setAttribute("PORT_NO",s_eqpt->port_no);
                                                            port.setAttribute("EQPT","0");

                                                            //qDebug()<<"eqpt "<< s_eqpt->eqpt.eqpt_id<< "deleted";

                                                            m_eqptIndex = mo_mcmData->m_eqptIndex[s_eqpt->eqpt.eqpt_id];
                                                            if(s_eqpt->eqpt.eqpt_id > 0)
                                                            {
                                                                m_eqptNo = 0;
                                                                memset(&ms_eqptInfo, 0, sizeof(STRUCT_MPIO_EQPT_INFO));
                                                                memcpy(&ms_eqptInfo, &mo_mcmData->ms_eqptInfo[s_eqpt->mpio_id],
                                                                        sizeof(STRUCT_MPIO_EQPT_INFO));
                                                                memset(&mo_mcmData->ms_eqptInfo[s_eqpt->mpio_id], 0,
                                                                        sizeof(STRUCT_MPIO_EQPT_INFO));
                                                                for(int i = 0; i < ms_eqptInfo.no_of_eqpt; i++)
                                                                {
                                                                    if(ms_eqptInfo.eqpt[i] == s_eqpt->eqpt.eqpt_id)
                                                                    {
                                                                        //do nothing
                                                                    }
                                                                    else
                                                                    {
                                                                        mo_mcmData->ms_eqptInfo[s_eqpt->mpio_id].eqpt[m_eqptNo++] = ms_eqptInfo.eqpt[i];
                                                                    }
                                                                }
                                                                mo_mcmData->ms_eqptInfo[s_eqpt->mpio_id].no_of_eqpt = m_eqptNo;
                                                            }
                                                            mo_mcmData->m_eqpt[m_eqptIndex][INDEX_PORT] = 0;
                                                            mo_mcmData->m_eqpt[m_eqptIndex][INDEX_SLOT] = 0;
                                                            mo_mcmData->m_eqpt[m_eqptIndex][INDEX_STATUS] = 0;

                                                            mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_status = DECONFIG_STATE;
                                                            mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_id = m_eqptId;
                                                            mo_mcmData->ms_mpios->eqpt[m_eqptIndex].channel_no = 0;

//                                                            mo_mcmData->mo_hlthSender->startTimer();
//                                                            mo_mcmData->mo_mcmCntrol->startTimerMcm();
                                                            startHltTimer();
                                                            startMcmContTimer();
                                                        }
                                                    }

                                                    eSlot.replaceChild(port,ePort);
                                                    breakStatus = true;
                                                    //                                                    break;
                                                    //ePort.setNodeValue(QString::number((int)s_eqpt->eqpt.eqpt_id));
                                                }
                                            }
                                        }
                                        port = port.nextSibling();
                                    }
                                }
                            }
                        }
                        //                        if(breakStatus == true)
                        //                        {
                        //                            break;
                        //                        }
                        slot = slot.nextSibling();
                    }
                }
            }
        }
        //        if(breakStatus == true)
        //        {
        //            break;
        //        }
        n = n.nextSibling();
    }

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        //qDebug()<<"open SDN_CONFIG.xml for adding eqpt fail";
        return false;
    }
    else
    {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
        //qDebug()<<"SDN_CONFIG.xml adding eqpt is done";
        mo_mcmData->ms_config_file = true;
        return breakStatus;

    }

}

int XmlReadWrite::readMpioConfiguration()
{
    int mpioType = 0;
    int mpioId = 0;
    int slotNo = 0;
    int cardType = 0;
    int cardStatus = 0;

    QString ip;
    //qDebug()<<" came for reading xml file";
    QFile file("/home/MPIO/SDN_CONFIG.xml");
    QDomDocument doc("SDN_CONFIG");
    if(!file.open(QIODevice::ReadOnly))
    {

        //qDebug()<<"SDN_CONFIG file can not be open for read";
        return 1;

    }
    if(!doc.setContent(&file))
    {
        //qDebug()<<"can not parse the file for reading";
        return 2;
    }
    file.close();

    QDomElement root = doc.documentElement();
    unsigned int time = root.attribute("value").toInt();
    mo_mcmData->m_epochTime = time;

    mo_mcmData->mo_mcmCntrol->ms_hlt->epoch_sec = mo_mcmData->m_epochTime;
//    mo_mcmData->mo_mcmCntrol->startTimerMcm();
    startMcmContTimer();
    //    //qDebug()<<"time "<<time;
    QDomNode n = root.firstChild();
    while(!n.isNull()) {
        QDomElement mpio = n.toElement(); // try to convert the node to an element.
        if(!mpio.isNull()) {
            //qDebug() << qPrintable(mpio.tagName()) << endl; // the node really is an element.
            if(mpio.tagName() == "MPIO")
            {
                mpioType = mpio.attribute("MPIO_TYPE").toInt();
                mpioId = mpio.attribute("MPIO_ID").toInt();
                qDebug()<<" MPIO ID is in config file"<< mpioId;
                if(mpioType = MPIO_TYPE_10S)
                {
                    mo_mcmData->mst_mpio[mpioId].config_status = 1;
                    mo_mcmData->mst_mpio[mpioId].mpio_type = MPIO_TYPE_10S;
                    //                      qDebug()<<" MPIO ID is in config file"<< mpioId;
                }
                else if(mpioType = MPIO_TYPE_2S)
                {
                    mo_mcmData->mst_mpio[mpioId].config_status = 1;
                    mo_mcmData->mst_mpio[mpioId].mpio_type = MPIO_TYPE_2S;
                }
                m_eqptNo = 0;
                //qDebug()<<"mpio_type"<<mpioType <<" mpio id"<< mpioId;
                QDomNode slot = mpio.firstChild();
                while(!slot.isNull())
                {
                    QDomElement eSlot = slot.toElement(); // try to convert the node to an element.
                    if(!eSlot.isNull())
                    {
                        if(eSlot.tagName() == "SLOT")
                        {
                            //qDebug() << qPrintable(eSlot.tagName()) << endl;
                            ip = eSlot.attribute("IP");
                            slotNo = eSlot.attribute("SLOT_NO").toInt();
                            cardType = eSlot.attribute("CARD_TYPE").toInt();
                            cardStatus = eSlot.attribute("CARD_STATUS").toInt();
                            //qDebug()<<" ip "<<ip <<"slot no" <<slotNo << "card type" << cardType;
                            QDomNode port = eSlot.firstChild();
                            while(!port.isNull())
                            {
                                QDomElement ePort = port.toElement();
                                if(!ePort.isNull())
                                {
                                    if(ePort.tagName() == "PORT")
                                    {
                                        readMpio(ePort, mpioId, slotNo , mpioType, cardStatus, ip);
                                    }


                                }
                                port = port.nextSibling();
                            }

                        }

                    }
                    slot = slot.nextSibling();
                }
            }

        }
        n = n.nextSibling();
    }

    return 0;

}

void XmlReadWrite::readMpio(QDomElement ePort, int mpioId, int slotNo, int mpioType, int cardStatus, QString ip)
{
    int portNo = 0;
    int eqptId = 0;
    int baudrate = 0;
    int parity = 0;
    int stopbit = 0;
    if((slotNo == 1 || slotNo ==2) && mpioType == MPIO_TYPE_2S)
    {
        portNo = ePort.attribute("PORT_NO").toInt();
        eqptId = ePort.attribute("EQPT").toInt();

        baudrate = ePort.attribute("BAUDRATE").toInt();
        parity = ePort.attribute("PARITY").toInt();
        stopbit = ePort.attribute("STOPBIT").toInt();

        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_2s.cc_card[slotNo - 1][portNo -1].baudrate = baudrate;
        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_2s.cc_card[slotNo - 1][portNo -1].parity = parity;
        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_2s.cc_card[slotNo - 1][portNo -1].stopbit = stopbit;

        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_2s.cc_card[slotNo - 1][portNo -1].eqpt_id = eqptId;
        strcpy(mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_2s.cc_ip[slotNo - 1] ,
                ip.toStdString().c_str());

        //qDebug()<<"port "<<portNo << "eqpt "<<eqptId << "baudrate "<<baudrate
        //               <<" parity "<<parity << "stopbit" << stopbit;


        //qDebug()<<"ip address "<<mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_2s.cc_ip[slotNo - 1];
    }
    if(slotNo == 3 && mpioType == MPIO_TYPE_10S)
    {
        portNo = ePort.attribute("PORT_NO").toInt();
        eqptId = ePort.attribute("EQPT").toInt();
        //qDebug()<<"port "<<portNo << "eqpt "<<eqptId;

        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.sr_dr_card[portNo -1].eqpt_id = eqptId;
        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.card_status[slotNo - 1] = cardStatus;
        strcpy(mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.sr_dr_ip ,
               ip.toStdString().c_str());

        m_eqptIndex = mo_mcmData->m_eqptIndex[eqptId];
        if(eqptId > 0)
        {
            mo_mcmData->ms_eqptInfo[mpioId].eqpt[m_eqptNo++] = eqptId;
            mo_mcmData->ms_eqptInfo[mpioId].no_of_eqpt = m_eqptNo;
        }
        mo_mcmData->m_eqpt[m_eqptIndex][INDEX_PORT] = portNo;
        mo_mcmData->m_eqpt[m_eqptIndex][INDEX_SLOT] = slotNo;
        mo_mcmData->m_eqpt[m_eqptIndex][INDEX_STATUS] = 1;

        mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_status = DOWN_ERR;
        mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_id = eqptId;
        mo_mcmData->ms_mpios->eqpt[m_eqptIndex].channel_no = portNo;

    }
    else if(slotNo == 4)
    {
        portNo = ePort.attribute("PORT_NO").toInt();
        eqptId = ePort.attribute("EQPT").toInt();
        //qDebug()<<"port "<<portNo << "eqpt "<<eqptId;

        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.discrt_card[portNo -1].eqpt_id = eqptId;
        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.card_status[slotNo - 1] = cardStatus;
        strcpy(mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.disc_ip ,
               ip.toStdString().c_str());
    }
    // commented and below slotno chnage 5 to 4
//    else if(slotNo == 5)
//    {
//        portNo = ePort.attribute("PORT_NO").toInt();
//        eqptId = ePort.attribute("EQPT").toInt();
//        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.card_status[slotNo - 1] = cardStatus;

//        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_card[slotNo -5][portNo -1].eqpt_id = eqptId;
//        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.card_status[slotNo - 1] = cardStatus;
//        strcpy(mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_ip[slotNo -5] ,
//                ip.toStdString().c_str());
//    }
    else if(slotNo > 4 && slotNo < 10)
    {
        portNo = ePort.attribute("PORT_NO").toInt();
        eqptId = ePort.attribute("EQPT").toInt();

        baudrate = ePort.attribute("BAUDRATE").toInt();
        parity = ePort.attribute("PARITY").toInt();
        stopbit = ePort.attribute("STOPBIT").toInt();

        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_card[slotNo - 5][portNo -1].baudrate = baudrate;
        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_card[slotNo -5][portNo -1].parity = parity;
        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_card[slotNo -5][portNo -1].stopbit = stopbit;

        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_card[slotNo -5][portNo -1].eqpt_id = eqptId;
        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.card_status[slotNo - 1] = cardStatus;
        strcpy(mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_ip[slotNo -5] ,
                ip.toStdString().c_str());

        //qDebug()<<"port "<<portNo << "eqpt "<<eqptId << "baudrate "<<baudrate
        //               <<" parity "<<parity << "stopbit" << stopbit<< "mpio id"<< mpioId<<" slot"<< slotNo -5;


        //qDebug()<<"ip address "<<mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.cc_ip[slotNo -5];

        m_eqptIndex = mo_mcmData->m_eqptIndex[eqptId];
        if(eqptId > 0)
        {
            mo_mcmData->ms_eqptInfo[mpioId].eqpt[m_eqptNo++] = eqptId;
            mo_mcmData->ms_eqptInfo[mpioId].no_of_eqpt = m_eqptNo;
        }
        mo_mcmData->m_eqpt[m_eqptIndex][INDEX_PORT] = portNo;
        mo_mcmData->m_eqpt[m_eqptIndex][INDEX_SLOT] = slotNo;
        mo_mcmData->m_eqpt[m_eqptIndex][INDEX_STATUS] = 1;

        mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_status = DOWN_ERR;
        mo_mcmData->ms_mpios->eqpt[m_eqptIndex].eqpt_id = eqptId;
        mo_mcmData->ms_mpios->eqpt[m_eqptIndex].channel_no = portNo;
    }

    else if(slotNo == 10)
    {
        mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.card_status[slotNo - 1] = cardStatus;
        strcpy(mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.sys_ip ,
               ip.toStdString().c_str());
    }
    mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.card_status[0] = 1;
    mo_mcmData->mst_mpio[mpioId].mpio_is.mpio_10s.card_status[1] = 1;

}


int XmlReadWrite::getCardType(int slotNo)
{
    switch(slotNo + 1)
    {
    case 1:
    case 2:
        return SWITCH_CARD;
    case 3:
        return SRDC_DSCR_CARD;
    case 4:
        return DISCRETE_CARD;
    case 5:
        return CARRIER_MIL_CARD;
    case 6:
    case 7:
    case 8:
    case 9:
        return CARRIER_SERIAL_CARD;
    case 10:
        return SYSTEM_CARD;
    default:
        //qDebug()<<"wrong slot no came for checking";
        break;

    }
}


