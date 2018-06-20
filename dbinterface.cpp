#include "dbinterface.h"
#include "dbhandle.h"
#include <qsqlquery.h>
#include <qsqlquerymodel.h>
#include <qsqlrecord.h>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QByteArray>
#include <QThread>
DBInterface::DBInterface(QObject *parent) :
    QObject(parent)
{
}
void DBInterface::open1()
{
    m_db = QSqlDatabase::addDatabase("QMYSQL","handler");
    QString qs =m_db.lastError().text();
    qDebug(qs.toLatin1().data());
    m_db.setHostName("localhost");
    m_db.setDatabaseName("my808");
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.setConnectOptions("MYSQL_OPT_RECONNECT=1");
    bool b =m_db.open();
    qDebug("&&&&&&&&&&&&&&&&&&&%d\n",b);
     qs =m_db.lastError().text();
    if(!b) qDebug(qs.toLatin1().data());
   // qtimer = new QTimer(this);
}
void DBInterface::open()
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    QString qs =m_db.lastError().text();
    qDebug(qs.toLatin1().data());
    m_db.setHostName("localhost");
    m_db.setDatabaseName("my808");
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.setConnectOptions("MYSQL_OPT_RECONNECT=1");
    bool b =m_db.open();
    qDebug("&&&&&&&&&&&&&&&&&&&%d\n",b);
     qs =m_db.lastError().text();
    if(!b) qDebug(qs.toLatin1().data());
   // qtimer = new QTimer(this);
}

void DBInterface::starttimer()
{

    connect(&qtimer,SIGNAL(timeout()),this,SLOT(checkonline()));
    qtimer.start(1000*60*60*1);
    //qtimer.start(1000*6);
}

void DBInterface::addrow(QString phone, QString msgid,QTime t, RecvStream s3)
{
    QSqlQuery query(m_db);
    // 插入数据user
    query.prepare("insert into rawlog values(?,?,?,?)");

   query.bindValue(0,phone);
    query.bindValue(1,msgid);
    query.bindValue(2,t);
    QByteArray ba;
    ba.resize(s3.size); //设置容量
    memcpy(ba.data(),s3.stream,s3.size);

    query.bindValue(3,ba);

    bool success = query.exec();
    if(!success){
        QSqlError lastError = query.lastError();
        qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        return;
    }
}

void DBInterface::addlog(string phone, string msgid,QTime t, RecvStream s3)
{
    QString a1,a2;
    a1.fromStdString(phone);
    a2.fromStdString(msgid);

    addrow(a1,a2,t,s3);
}

void DBInterface::addlogs(string phone, string msgid,QTime t, string s3)
{
     //cout <<"++++++++++++++++++++"<< phone << msgid<< s3;
    QSqlQuery query(m_db);
    QString a1,a2,a3;
    a1=QString::fromStdString(phone);
    a2=QString::fromStdString(msgid);
    a3=QString::fromStdString(s3);
    query.prepare("insert delayed into rawlog_s values(?,?,?,?)");

   query.bindValue(0,a1);
    query.bindValue(1,a2);
    query.bindValue(2,t.toString("hh:mm:ss.zzz"));


    query.bindValue(3,a3);

    bool success = query.exec();
           qDebug() <<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<t;
  //  qDebug<<phone<<"  "<<msgid<<" "<<t<<" "<<s3;
    if(!success){
        QSqlError lastError = query.lastError();
        qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        //qDebug() << a1 << a2<< a3;
        //qDebug << a1 << "  " << a2 << " " << t << " " << a3;

        return;
    }

}
void DBInterface::rsendcan(string phone,QString time,string stime,string canid,string data)
{
    QSqlQuery query(m_db);
    QString p;

    query.prepare("insert delayed into soundboxcan_log values(?,?,?,?,?)");
    p=QString::fromStdString(phone);
    QString qstime=QString::fromStdString(stime);
    QString qcanid=QString::fromStdString(canid);
    QString qdata=QString::fromStdString(data);
    query.bindValue(0,p);
    query.bindValue(1,time);
    query.bindValue(2,qstime);
    query.bindValue(3,qcanid);
    query.bindValue(4,qdata);
    bool success = query.exec();
           //qDebug() <<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<t;
  //  qDebug<<phone<<"  "<<msgid<<" "<<t<<" "<<s3;
    if(!success){
        QSqlError lastError = query.lastError();
        qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        //qDebug() << a1 << a2<< a3;
        //qDebug << a1 << "  " << a2 << " " << t << " " << a3;

        return;
    }
}

void DBInterface::rsendoil(string phone,QString time,double v,double v1,double t)
{
    QSqlQuery query(m_db);
    QString p;

    query.prepare("insert delayed into oillog values(?,?,?,?,?)");
    p=QString::fromStdString(phone);

    query.bindValue(0,p);
    query.bindValue(1,time);
    query.bindValue(2,v);
    query.bindValue(3,v1);
    query.bindValue(4,t);
    bool success = query.exec();
           //qDebug() <<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<t;
  //  qDebug<<phone<<"  "<<msgid<<" "<<t<<" "<<s3;
    if(!success){
        QSqlError lastError = query.lastError();
        qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        //qDebug() << a1 << a2<< a3;
        //qDebug << a1 << "  " << a2 << " " << t << " " << a3;

        return;
    }

}

void DBInterface::rsendwater(string phone,QString time,double v,double v1,double t)
{
    QSqlQuery query(m_db);
    QString p;

    query.prepare("insert delayed into waterlog values(?,?,?,?,?)");
    p=QString::fromStdString(phone);

    query.bindValue(0,p);
    query.bindValue(1,time);
    query.bindValue(2,v);
    query.bindValue(3,v1);
    query.bindValue(4,t);
    bool success = query.exec();
           //qDebug() <<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<t;
  //  qDebug<<phone<<"  "<<msgid<<" "<<t<<" "<<s3;
    if(!success){
        QSqlError lastError = query.lastError();
        qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        //qDebug() << a1 << a2<< a3;
        //qDebug << a1 << "  " << a2 << " " << t << " " << a3;

        return;
    }
}

void DBInterface::addboxlog(string phone,QString time,int percent,int power,double weidu,double jindu,float temp,int gps,int gprs,int distance)
{
    QSqlQuery query(m_db);
    QString p;

    query.prepare("insert delayed into box_log values(?,?,?,?,?,?,?,?,?,?,default)");
    p=QString::fromStdString(phone);

    query.bindValue(0,p);
    query.bindValue(1,time);
    query.bindValue(2,percent);
    query.bindValue(3,power);
    query.bindValue(4,weidu);
    query.bindValue(5,jindu);
    query.bindValue(6,temp);
    query.bindValue(7,gps);
    query.bindValue(8,gprs);
    query.bindValue(9,distance);
    bool success = query.exec();
           //qDebug() <<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<t;
  //  qDebug<<phone<<"  "<<msgid<<" "<<t<<" "<<s3;
    if(!success){
        QSqlError lastError = query.lastError();
        qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        //qDebug() << a1 << a2<< a3;
        //qDebug << a1 << "  " << a2 << " " << t << " " << a3;

        return;
    }
}
void DBInterface::addlogs1(string phone, string msgid,QString t, string s3)
{
     //cout <<"++++++++++++++++++++"<< phone << msgid<< s3;
    QSqlQuery query(m_db);
    QString a1,a2,a3;
    a1=QString::fromStdString(phone);
    a2=QString::fromStdString(msgid);
    a3=QString::fromStdString(s3);
    query.prepare("insert delayed into rawlog_s1 values(?,?,?,?)");

   query.bindValue(0,a1);
    query.bindValue(1,a2);
    query.bindValue(2,t);


    query.bindValue(3,a3);

    bool success = query.exec();
           qDebug() <<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<t;
  //  qDebug<<phone<<"  "<<msgid<<" "<<t<<" "<<s3;
    if(!success){
        QSqlError lastError = query.lastError();
        qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        //qDebug() << a1 << a2<< a3;
        //qDebug << a1 << "  " << a2 << " " << t << " " << a3;

        return;
    }

}
void DBInterface::flush()
{
    if (num == 0) return;
    //cout <<"++++++++++++++++++++"<< phone << msgid<< s3;

    QSqlQuery query(m_db);
    QString sql=  "insert delayed into rawlog_s1 values(?,?,?,?)";
    for(int i=0;i<num-1;i++)
        sql+=",(?,?,?,?)";
    query.prepare(sql);
    for(int i=0;i<num;i++){
           query.bindValue(4*i,a1[i]);
           query.bindValue(4*i+1,a2[i]);
           query.bindValue(4*i+2,tt[i]);
           query.bindValue(4*i+3,a3[i]);
       }
     QString sssss=query.executedQuery();
     bool success = query.exec();
     sssss=query.executedQuery();
     qDebug() <<sssss;
 //  qDebug<<phone<<"  "<<msgid<<" "<<t<<" "<<s3;
     if(!success){
         QSqlError lastError = query.lastError();
         qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
       //qDebug() << a1 << a2<< a3;
       //qDebug << a1 << "  " << a2 << " " << t << " " << a3;

     }
     num=0;
     size = 0;

     return;


}

void DBInterface::addlogs11(string phone, string msgid,QString t, string s3)
{
     //cout <<"++++++++++++++++++++"<< phone << msgid<< s3;

    QSqlQuery query(m_db);


    a1[num]=QString::fromStdString(phone);
    a2[num]=QString::fromStdString(msgid);
    a3[num]=QString::fromStdString(s3);
    tt[num]=t;
    size++;

 if(num==96){
     query.prepare("insert delayed into rawlog_s1 values(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),"
                   "(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),"
                    "(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),"
                    "(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),"
                    "(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),"
                    "(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),"
                    "(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),"
                    "(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),"
                    "(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),"
                    "(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?),(?,?,?,?)");
     for(int i=0;i<=num;i++){
            query.bindValue(4*i,a1[i]);
            query.bindValue(4*i+1,a2[i]);
            query.bindValue(4*i+2,tt[i]);
            query.bindValue(4*i+3,a3[i]);
        }



        QString sssss=query.executedQuery();
        qDebug() <<sssss;
        bool success = query.exec();
         sssss=query.executedQuery();
        qDebug() <<sssss;
  //  qDebug<<phone<<"  "<<msgid<<" "<<t<<" "<<s3;
        if(!success){
          QSqlError lastError = query.lastError();
         qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        //qDebug() << a1 << a2<< a3;
        //qDebug << a1 << "  " << a2 << " " << t << " " << a3;

         }
        num=0;
        size = 0;
         return;
    }
    num++;
}
void DBInterface::positionprocess(string phone,QString time,PositionReport pr)
{
//decide acc if acc "在线行驶" else 在线停车
    QString p=QString::fromStdString(phone);
    QString qacc = (pr.acc&pr.status)?QString("开启"):QString("关闭");
    QString qstatus = (pr.acc&pr.status)?QString("在线行驶"):QString("在线停车");
    double weidu=pr.latitude*0.000001;
    double jindu=pr.longitude*0.000001;
    Coor c=dbhandler.getConv(weidu,jindu);
    this->updateSoundboxInfo(p,time,jindu,weidu,c.j,c.w,qstatus,qacc);

}

void DBInterface:: process(string phone,QString time,int percent,int power,double weidu,double jindu,float temp,int gps,int gprs,int distance)
{
    QString event("");
    QString p=QString::fromStdString(phone);
    qDebug()<<"eeeeeeeeeeeeeeeeee&&&&&&&& processid "<<QThread::currentThreadId()<<endl;
    Coor c=dbhandler.getConv(weidu,jindu);
    QString ad = dbhandler.getaddressReq(c.w,c.j);
    this->updateProductInfo(p,percent,power,ad,c.w,c.j,temp);
    BoxRecord brr=this->getLastRow(p);
    QString boxs=this->getBoxStatus(p);
    if(distance<=2000 and distance>0)
    {
        if((boxs.compare("回收提示")!=0)){
            this->updateProductBoxstatus(p,"回收提示");
            event=QString("回收提示");
        }
    }

    if(distance>8000){
        if(distance-brr.br.distance >600) //this line can be removed
        {
            if((boxs.compare("回收提示")==0)||(boxs.compare("满溢")==0)){
                this->updateProductBoxstatus(p,"回收");
                event=QString("回收");
                this->updateProductLastRecycle(p,time);
            }
            else
            {
                this->updateProductBoxstatus(p,"未满回收");
                event=QString("未满回收");
                this->updateProductLastRecycle(p,time);
            }

        }
    }
    if(distance==0)
    {
        if(boxs.compare("满溢")){
            this->updateProductBoxstatus(p,"满溢");
            event=QString("满溢");
        }


    }
    this->addboxlogEvent(phone,time,percent,power,weidu,jindu,temp,gps,gprs,distance,event);
    //get converted coordination

    //get formarted_address

// update product info

    //计算事件  满  清空

    //增加事件记录



}

void DBInterface::checkonline()
{
    QSqlQueryModel querymodel;
    querymodel.setQuery("SELECT id,status FROM productinfo ORDER BY id ASC",m_db);
    for (num=0;num<querymodel.rowCount();num++)
    {
        QString phone=querymodel.record(num).value("id").toString();
        QString status=querymodel.record(num).value("status").toString();
        if(status.compare("未激活"))
        {
            BoxRecord bre =getLastRow(phone);
            QDateTime last;
            if(bre.time.length()>20){
                last=QDateTime::fromString(bre.time,"yyyy-MM-dd hh:mm:ss.zzz");
                    //"yyyy-MM-dd")+" "+ttt1.toString("hh:mm:ss.zzz");
                QDateTime now=QDateTime::currentDateTime();
                if(last.secsTo(now)>(60*60*28))
                    this->setOffline(phone);
            }
        }
    }

    querymodel.setQuery("SELECT id,time FROM soundboxinfo ORDER BY id ASC",m_db);
    for (num=0;num<querymodel.rowCount();num++)
    {
        QString phone=querymodel.record(num).value("id").toString();
        QString time=querymodel.record(num).value("time").toString();

            QDateTime last;
            if(time.length()>20)
            {
                last=QDateTime::fromString(time,"yyyy-MM-dd hh:mm:ss.zzz");
                    //"yyyy-MM-dd")+" "+ttt1.toString("hh:mm:ss.zzz");
                QDateTime now=QDateTime::currentDateTime();
                if(last.secsTo(now)>(60*60*1))
                    this->setOfflineSoundbox(phone);
            }
        }

    //check product last report time ,if tonow >1 day  update productinfo row to "离线"

 qDebug()<<"&&&&&&&& threadid "<<QThread::currentThreadId()<<  endl;
}

QString DBInterface::getBoxStatus(QString phone)
{
    QSqlQuery query(m_db);
    //int numRows;
    QString sql;
    QString bs((const char *)0);
    //qDebug()<<")))))))))))))))))"<<bs.isNull();
    sql=QString("SELECT boxstatus FROM productinfo where id='%1'").arg(phone);
    bool success=query.exec(sql);
    if(!success){
      QSqlError lastError = query.lastError();
     qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        return bs;

     }
    while (query.next())
    {
         bs = query.value(0).toString();//query.value()的参数只能是index
       qDebug()<<")))))))))))))))))"<<bs.isNull();

    }
    return bs;
}

BoxRecord DBInterface::getLastRow(QString phone)
{
    QSqlQuery query(m_db);
    BoxRecord br;
    br.phone=QString((const char *)0);
    QString sql;
    sql=QString("SELECT * FROM box_log where phone='%1' order by time desc limit 1").arg(phone);
    bool success=query.exec(sql);
    if(!success){
      QSqlError lastError = query.lastError();
     qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        return br;

     }
    while (query.next())
    {
        br.phone= query.value(0).toString();
        br.time=query.value(1).toString();
        br.br.percent=query.value(2).toInt();
        br.br.power=query.value(3).toInt();
        br.br.w=query.value(4).toDouble();
        br.br.j1=query.value(5).toDouble();
        br.br.temp1 =query.value(6).toFloat();
        br.br.gps=query.value(7).toInt();
        br.br.gprs=query.value(8).toInt();
        br.br.distance=query.value(9).toInt();

        //bs = query.value(0).toString();//query.value()的参数只能是index
       //qDebug()<<")))))))))))))))))"<<bs.isNull();

    }
    //return bs;

    return br;
}
int DBInterface::updateSoundboxInfo(QString phone, QString time,double j,double w, double cj, double cw,QString status, QString acc)
{
    //int i=0;
    QSqlQuery query(m_db);
    QString sql;
    QString w1=QString::number(w,'f',9);
    QString j1=QString::number(j,'f',9);
    QString cw1=QString::number(cw,'f',9);
    QString cj1=QString::number(cj,'f',9);
    sql=QString("UPDATE  soundboxinfo set  time='%1',j=%2,w=%3,cj=%4,cw=%5, status='%6', acc='%7' where id='%8' ").arg(time).arg(j1).arg(w1).arg(cj1).arg(cw1).arg(status).arg(acc).arg(phone);
    qDebug()<<sql<<endl;
    bool success=query.exec(sql);
    if(!success){
      QSqlError lastError = query.lastError();
     qDebug() << "updatesoundboxinfo fail " << lastError.driverText() << lastError.databaseText();
        return 0;

     }
    return 0;
}
// update '离线'
int DBInterface::updateSoundboxStatus(QString phone,QString status)
{
    return 0;
}

int DBInterface::updateProductInfo(QString phone,int percent,int power,QString address, double w,double j,float temp )
{                                 //(QString phone,QString percent,QString power,QString address, double w,double j,float temp );
    int i=0;
    QSqlQuery query(m_db);
    QString sql;
    QString w1=QString::number(w,'f',9);
    QString j1=QString::number(j,'f',9);
    sql=QString("UPDATE  productinfo set status='在线', volume=%1,power=%2,address='%3',weidu=%4,jindu=%5, temp=%6 where id='%7' ").arg(percent).arg(power).arg(address).arg(w1).arg(j1).arg(temp).arg(phone);
    qDebug()<<sql<<endl;
    bool success=query.exec(sql);
    if(!success){
      QSqlError lastError = query.lastError();
     qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        return 0;

     }

    return 0;
}
void DBInterface::setOffline(QString phone)
{
    QSqlQuery query(m_db);
    QString sql;
    sql=QString("UPDATE  productinfo set status='离线' where id='%1' ").arg(phone);
    qDebug()<<sql<<endl;
    bool success=query.exec(sql);
    if(!success){
      QSqlError lastError = query.lastError();
     qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        return ;

     }

    //return 0;
}

void DBInterface::setOfflineSoundbox(QString phone)
{
    QSqlQuery query(m_db);
    QString sql;
    sql=QString("UPDATE  soundboxinfo set status='离线' where id='%1' ").arg(phone);
    qDebug()<<sql<<endl;
    bool success=query.exec(sql);
    if(!success){
      QSqlError lastError = query.lastError();
     qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        return ;

     }

    //return 0;
}

int DBInterface::updateProductBoxstatus(QString phone ,QString status)
{
    //status   未满回收 ，回收   回收提示， 满溢
    QSqlQuery query(m_db);
    QString sql;
    sql=QString("UPDATE  productinfo set boxstatus='%2' where id='%1' ").arg(phone).arg(status);
    qDebug()<<sql<<endl;
    bool success=query.exec(sql);
    if(!success){
      QSqlError lastError = query.lastError();
     qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        return 0;

     }

    //return 0;

    return 0;
}

int DBInterface::updateProductLastRecycle(QString phone ,QString last)
{
    QSqlQuery query(m_db);
    QString sql;
    sql=QString("UPDATE  productinfo set last='%2' where id='%1' ").arg(phone).arg(last);
    qDebug()<<sql<<endl;
    bool success=query.exec(sql);
    if(!success){
      QSqlError lastError = query.lastError();
     qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
        return 0;

     }

    //return 0;

    return 0;
    //return 0;
}

void  DBInterface::addboxlogEvent(string phone,QString time,int percent,int power,double weidu,double jindu,float temp,int gps,int gprs,int distance,QString event)
{
    if(event.isNull())
        this->addboxlog(phone,time,percent,power,weidu,jindu,temp,gps,gprs,distance);
    else
    {
        QSqlQuery query(m_db);
        QString p;

        query.prepare("insert delayed into box_log values(?,?,?,?,?,?,?,?,?,?,?)");
        p=QString::fromStdString(phone);

        query.bindValue(0,p);
        query.bindValue(1,time);
        query.bindValue(2,percent);
        query.bindValue(3,power);
        query.bindValue(4,weidu);
        query.bindValue(5,jindu);
        query.bindValue(6,temp);
        query.bindValue(7,gps);
        query.bindValue(8,gprs);
        query.bindValue(9,distance);
        query.bindValue(10,event);
        bool success = query.exec();
               //qDebug() <<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<t;
      //  qDebug<<phone<<"  "<<msgid<<" "<<t<<" "<<s3;
        if(!success){
            QSqlError lastError = query.lastError();
            qDebug() << "插入失败：" << lastError.driverText() << lastError.databaseText();
            //qDebug() << a1 << a2<< a3;
            //qDebug << a1 << "  " << a2 << " " << t << " " << a3;

            return;
        }
    }
}
