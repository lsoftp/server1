#include "mainwindow.h"
#include <QApplication>
#include "tcpserverlisten.h"
#include "dbinterface.h"
#include "tcpserver.h"
#include "datastruct.h"
#include "dbhandle.h"
#define DEBUG
#ifdef DEBUG
#define DPRINTK(format,...) printf("hahah""  is ""*********************abcd is %d""\n", ##__VA_ARGS__)
#else
#define DPRINTK( x,... )
#endif

DBInterface db;
DBInterface dbhandle;

int main(int argc, char *argv[])
{
    //QApplication::addLibraryPath("/usr/lib/x86_64-linux-gnu/qt4/plugins/");

/*    char    tt[2]={'\xff', '\xc1'};
    short t=qFromBigEndian(*(short*)(tt));
    float ft=t*0.1;
    unsigned short ut=qFromBigEndian(*(short*)(tt));
    printf("***************%d  %d %f\n",t,ut,ft);

    QString bs((const char*)0 );

    qDebug()<<")))))))))))))))))"<<bs.isNull();
     */
   //QString ss("12312312312321312"),sss;
//sss=QString("sdfsdfsd%1").arg(ss);
   //qDebug()<<")))))))))))))))))"<<sss;
DPRINTK("*********************abcd is %d",10);
    QApplication a(argc, argv);
    qRegisterMetaType<string>("string");
    qRegisterMetaType<PositionReport>("PositionReport");
    MainWindow w;
    w.show();
    DPRINTK("*********************abcd is %d",10);
        db.open();
        dbhandle.open1();
        db.getBoxStatus("000755001202");
    db.updateProductBoxstatus("000755001202","回收");
   // db.updateProductInfo("000755001202",88,99,"天门",29,119,23);
        QThread thread;
    dbhandle.moveToThread(&thread);
    //dbhandler.moveToThread(&thread);
    //dbhandle.qtimer.moveToThread(&thread);
    thread.start();
    dbhandle.starttimer();
    dbhandler.getConv(29.889646,114.317629);
    dbhandler.getaddressReq(29,114);
    //TcpServerListen tsl;
    //tsl.start();

      TcpServer ts;
    ts.setMw(&w);
    QString q="sound";
    QString o=UTF82GBK(q);
    cout<<o.toStdString()<<endl;
 //   printf(o.toStdString());
    fflush(stdout);
  //  Run r;
  //  r.start();


//    TcpServer tser;

//    tser.mw = &w;
//    QObject::connect(&tser,SIGNAL(addPacket(string,string,string)),&w,SLOT(addrow(string,string,string)));
//    tser.init();

    //tser.acceptsocket();

    int ret= a.exec();
    thread.terminate();
    thread.wait();
    return ret;
}
