#ifndef DBHANDLE_H
#define DBHANDLE_H
#include "datastruct.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QEventLoop>
struct Coor{
    double w;
    double j;

};

class DBHandle : public QObject
{
    Q_OBJECT
public:

    //QNetworkReply reply;
    explicit DBHandle(QObject *parent = nullptr);

    QString getaddressReq( double w,double j)
    {
         QNetworkAccessManager qnam;
        QString ss;
        ss.sprintf("http://api.map.baidu.com/geocoder/v2/?callback=renderReverse&location=%f,%f&output=json&ak=dg05Yew93CtRGyk9y96zwaRDGrCi0Y7f",w,j);
        QNetworkRequest request;
        request.setUrl(QUrl(ss));
        QNetworkReply* reply = qnam.get(request);
                            QEventLoop eventLoop;
                            connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
                            eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
                            //QJsonObject data = QJsonDocument::fromJson(reply->readAll()).object();
                            QByteArray qb=reply->readAll();
                            QString sss(qb);

                            int  begin = sss.indexOf("formatted_address");
                            int end = sss.indexOf('"',begin+20);
                            QString ssss=sss.mid(begin+20,end-begin-20);
                            qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!"<<ssss;
                           /* if(data.contains("formatted_address"))
                                        {
                                            QJsonValue name_value = data.take("formatted_address");
                                            //qDebug()<<name_value;
                                            if(name_value.isString())
                                            {
                                                ss = name_value.toString();
                                            }
                                        }
                            qDebug()<<"##########################"<<ss;
                            */     return ssss;
        //request.setRawHeader("apikey", "b446bb51d329b1098b008568231a772b");
    }
    //get coordination and address
   Coor getConv(double w,double j)
   {
        QNetworkAccessManager qnam;
       QNetworkRequest request;
       //request.setUrl(QUrl(QString().sprintf("http://api.map.baidu.com/geoconv/v1/?coords=%f,%f&from=1&to=5&ak=dg05Yew93CtRGyk9y96zwaRDGrCi0Y7f",j,w)));
       request.setUrl(QUrl(QString("http://api.map.baidu.com/geoconv/v1/?coords=%1,%2&from=1&to=5&ak=dg05Yew93CtRGyk9y96zwaRDGrCi0Y7f").arg(j).arg(w)));
       QNetworkReply* reply = qnam.get(request);
        QEventLoop eventLoop;
        connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
        QJsonObject data = QJsonDocument::fromJson(reply->readAll()).object();
             qDebug()<<data;
             Coor c={0,0};
             if (data.contains("result") && data["result"].isArray()) {
                     QJsonArray npcArray = data["result"].toArray();
                     QJsonObject json=npcArray[0].toObject();
                     if (json.contains("x")&&json["x"].isDouble())
                     {
                         c.j=json["x"].toDouble();
                     }
                     if (json.contains("y")&&json["y"].isDouble())
                     {
                         c.w=json["y"].toDouble();
                     }
             }
            /* if(data.contains("result"))
             {
                 QJsonValue name_value = data.take("result");
                 //qDebug()<<name_value;
                 if(name_value.isArray())
                 {
                     c.j = name_value.toDouble();
                 }
             }*/
/*             if(data.contains("result"))
             {
                 QJsonValue name_value = data.take("result");
                 //qDebug()<<name_value;
                 if(name_value.isArray())
                 {
                     c.j = name_value.toDouble();
                 }
             }
*/
             //c.j=1;
             //c.w=2;
             qDebug()<<"***********************"<<c.w<<"  "<<c.j;
            // QString sql;


             //sql=QString("UPDATE  productinfo set status='在线', volume=%1,power=%2,address='',weidu=,jindu=, temp= where id='' ").arg(w1).arg(j1);
             return c;
   }

signals:

public slots:
    void process(BoxReport br);
};
extern DBHandle dbhandler;
#endif // DBHANDLE_H
