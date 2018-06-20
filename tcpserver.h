#ifndef TCPSERVER1_H
#define TCPSERVER1_H

#include <QObject>

//#include "tcpserver.h"
#include <QTcpSocket>
#include <QTcpServer>
#include <QMap>
#include <QTimer>
#include "mainwindow.h"
#include "tcpclient.h"
class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    void setMw(MainWindow *pw){m_pw=pw;}
    MainWindow *m_pw;

signals:

public slots:
    void slot_newConnection();
    void run();
    void slot_readdata();

    void slot_errors(QAbstractSocket::SocketError errors);

    void slot_updateUserList();

private:
    QTcpServer *m_tcpserver;
    QMutex mutexclient;
    QTcpSocket *m_tcpsocket;
    TcpClient * m_client;
    QMap<QString, TcpClient* > m_userlist;

    //心跳
    QTimer *m_timer;

    //心跳时间
    int m_hearttime;

    bool m_startHeartTime;

private:
    void initserver();

    void senddata(QTcpSocket *newtcpsocket, const QString &);

    void startHeart();
};
class Run:public QThread
{
public:
    //
    //Run(TcpServer*p){pt=p;}

private:
    void run()
    {
      TcpServer *pt=new TcpServer;

            pt->run();
    }
};

#endif // TCPSERVER1_H
