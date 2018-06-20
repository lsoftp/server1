#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <datastruct.h>
#include <QTime>
#include <QTimer>

struct BoxRecord{
    QString phone;
    QString time;
    BoxReport br;

};

class DBInterface : public QObject
{
    Q_OBJECT
public:
    QTimer qtimer;
    QString a1[100],a2[100],a3[100],tt[100];
    int num=0,size =0;
    explicit DBInterface(QObject *parent = 0);
    QSqlDatabase m_db;
    void open();
    void open1();
    void starttimer();
    void addrow(QString phone, QString msgid,QTime t, RecvStream s3);
    QString getBoxStatus(QString phone);
    BoxRecord getLastRow(QString phone);
    int updateProductInfo(QString phone,int percent,int power,QString address, double w,double j,float temp );
    int updateSoundboxInfo(QString phone, QString time,double j,double w, double cj, double cw,QString status, QString acc);
    // update '离线'
    int updateSoundboxStatus(QString phone,QString status);
    void setOffline(QString phone);
    void setOfflineSoundbox(QString phone);
    int updateProductBoxstatus(QString phone ,QString status);
    int updateProductLastRecycle(QString phone ,QString last);
    void  addboxlogEvent(string phone,QString time,int percent,int power,double weidu,double jindu,float temp,int gps,int gprs,int distance,QString event);
signals:

public slots:
    void addlog(string phone, string msgid,QTime t, RecvStream s3);
    void addlogs(string phone, string msgid,QTime t, string s3);
    void addlogs11(string phone, string msgid,QString t, string s3);
    void addlogs1(string phone, string msgid,QString t, string s3);
    void addboxlog(string phone,QString time,int percent,int power,double weidu,double jindu,float temp,int gps,int gprs,int distance);
    void process(string phone,QString time,int percent,int power,double weidu,double jindu,float temp,int gps,int gprs,int distance);
    void positionprocess(string phone,QString time,PositionReport pr);
    void flush();
    void checkonline();

    void rsendcan(string phone,QString time,string stime,string canid,string data);
    void rsendoil(string phone,QString time,double v,double v1,double t);
    void rsendwater(string phone,QString time,double v,double v1,double t);
};

extern DBInterface db;
extern DBInterface dbhandle;
#endif // DBINTERFACE_H
