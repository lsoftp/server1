#include "tcpserver.h"
#include "dbinterface.h"

#include <QDebug>
#include <QHostAddress>
extern DBInterface db;
const int kPort = 50000;

const QString connectflag = "server return message: connect server is ok!";

const QString hasconnect  = "I'm still connect";

const int kBase = 1000;

const char *socketErrors[100] = {"ConnectionRefusedError",
                                 "RemoteHostClosedError",
                                 "HostNotFoundError",
                                 "SocketAccessError",
                                 "SocketResourceError",
                                 "SocketTimeoutError",
                                 "DatagramTooLargeError",
                                 "NetworkError",
                                 "AddressInUseError",
                                 "SocketAddressNotAvailableError",
                                 "UnsupportedSocketOperationError",
                                 "ProxyAuthenticationRequiredError",
                                 "SslHandshakeFailedError",
                                 "UnfinishedSocketOperationError",
                                 "ProxyConnectionRefusedError",
                                 "ProxyConnectionClosedError",
                                 "ProxyConnectionTimeoutError",
                                 "ProxyNotFoundError",
                                 "ProxyProtocolError",
                                 "UnknownSocketError"
                                };

TcpServer::TcpServer(QObject *parent) :
    QObject(parent),
    m_tcpserver(NULL),
    m_hearttime(1),
    m_startHeartTime(false)

{
    m_userlist.clear();

    initserver();
}

void TcpServer::initserver()
{
    //启动一个tcpserver
    m_tcpserver = new QTcpServer(this);

    //监听
    if( !m_tcpserver->listen(QHostAddress::Any, kPort) )
    {
        qDebug() << "listen error: " << m_tcpserver->errorString();
        return;
    }
    else
    {
        qDebug() << "server start is ok!";
    }
    //来一个客户端连接，就会产生一个slot
    connect(m_tcpserver, SIGNAL(newConnection()), this, SLOT(slot_newConnection()));

    m_timer = new QTimer();

    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_updateUserList()));
 //  this->moveToThread(&rr);
   // connect(&rr,SIGNAL(started()),this,SLOT(run()));
    //rr.start();

}

void TcpServer::slot_newConnection()
{
            printf("snew connection----------------------------\n");
            fflush(stdout);
    //得到每一个连接成功的客户段socket套接字
    QTcpSocket *newtcpsocket = m_tcpserver->nextPendingConnection();

    if( NULL == newtcpsocket )
    {
        qDebug() << "socket is error";
        return ;
    }

    m_tcpsocket = newtcpsocket;
    //m_tcpsocket->moveToThread(this);
    m_client= new TcpClient();
    //m_client->moveToThread(this);
    m_client->setsock( m_tcpsocket);
    connect(m_client,SIGNAL(addPacket(string,string,string)),m_pw,SLOT(addrow(string,string,string)));

    QHostAddress address = newtcpsocket->peerAddress();
    quint16 p=newtcpsocket->peerPort();

    QString Ip = address.toString();
    QString port=QString::number(p,10);
    QString clientIp=Ip+":"+port;
    qDebug() << "clientIp: " << clientIp;

    qDebug() << "currnet user count: "<< m_userlist.size();
    if( m_userlist.size() > 0 )
    {
        qDebug() << "they is :";
    }
        //mutexclient.lock();
    QMap<QString, TcpClient*>::Iterator iter;
    for(iter = m_userlist.begin(); iter != m_userlist.end(); ++ iter)
    {
        qDebug() << iter.key();
    }
    if( NULL == m_userlist[clientIp] )
    {
        qDebug() << "new user join it's ip: " << clientIp;

        m_userlist.insert(clientIp, m_client);

        //connect(newtcpsocket, SIGNAL(readyRead()), m_client, SLOT(slot_readdata()));
        //connect(newtcpsocket, SIGNAL(readyRead()), m_client, SLOT(slot_readdata()));
        connect(newtcpsocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slot_errors(QAbstractSocket::SocketError)));

        if( "" != clientIp )
        {
            //senddata(newtcpsocket, connectflag);
        }
        if( false == m_startHeartTime )
        {
            //qDebug() << "start heart time";
            startHeart();
            m_startHeartTime = true;
        }
    }
          //  mutexclient.unlock();
}

void TcpServer::startHeart()
{
    int socketcount = m_userlist.size();

    if( socketcount > 0 )
    {
        m_timer->start(m_hearttime * kBase);
    }
    else
    {
        m_startHeartTime = false;
        m_timer->stop();
    }
}

void TcpServer::run()
{
    while(1)
    {
        mutexclient.lock();
        QMap<QString, TcpClient*>::Iterator iter;
        for(iter = m_userlist.begin(); iter != m_userlist.end(); )
        {
            //senddata()
            TcpClient *pc=*iter;
            //printf("%d",pc);

            pc->getmsg_send();
            QAbstractSocket::SocketState curstate = (*iter)->ac_socket->state();

            if( QAbstractSocket::UnconnectedState == curstate ||
            QAbstractSocket::ClosingState == curstate||!pc->isRunning)
             {
                qDebug() << "erase close socket";
                delete pc;
                iter = m_userlist.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
        mutexclient.unlock();
    }
}
void TcpServer::slot_updateUserList()
{
    QMap<QString, TcpClient*>::Iterator iter;
    for(iter = m_userlist.begin(); iter != m_userlist.end(); )
    {
        //qDebug("++++++++++++++++++++++++++++++++++++\n");
         TcpClient *pc=*iter;
        QAbstractSocket::SocketState curstate = (*iter)->ac_socket->state();
        QDateTime now=QDateTime::currentDateTime();
        int t =pc->m_lasthearttime.secsTo(now);
        if(t>140) pc->stop();
        if(t>3) db.flush();

        pc->getmsg_send();
        pc->reconnect();
        if( QAbstractSocket::UnconnectedState == curstate ||
           QAbstractSocket::ClosingState == curstate||!pc->isRunning )
        {
            qDebug() << "erase close socket";
            if (!pc){delete pc; pc=NULL;}
            iter = m_userlist.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void TcpServer::senddata(QTcpSocket *newtcpsocket, const QString &str)
{
    int len = newtcpsocket->write(str.toLocal8Bit());
    if( len > 0 )
    {
        QString userip = newtcpsocket->peerAddress().toString();
        qDebug() << "user "<< userip << "has connected ok";
    }
}

void TcpServer::slot_readdata()
{
    QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());

    QString recvbuf = "";

    recvbuf = newtcpsocket->readAll();

    QString clientip = newtcpsocket->peerAddress().toString();

//    if( recvbuf == "I'm will disconnect" )
//    {
//        m_userlist.remove(clientip);
//        return ;
//    }

    qDebug() << "user " << clientip << "sendText: " << recvbuf;

    newtcpsocket->write("server return message:send text is sucess!");
}

void TcpServer::slot_errors(QAbstractSocket::SocketError errors)
{
    QString displays = "";

    QTcpSocket *newtcpsocket = static_cast<QTcpSocket*>(this->sender());

    //发生错误时，服务端踢掉出错socket
    m_userlist.remove(newtcpsocket->peerAddress().toString());

    qDebug() << "user " << newtcpsocket->peerAddress().toString() << " is left..";

    if( errors == 1 )
    {
        return ;
    }
    else if( -1 == errors )
    {
        displays = socketErrors[19];
    }
    else
    {
        displays = socketErrors[errors];
    }

}
