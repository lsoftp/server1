#ifndef TCPSERVERLISTEN_H
#define TCPSERVERLISTEN_H
#include "tcpserver.h"
#include <QTimer>


class TcpServerListen : public QObject
{
    Q_OBJECT
public:
    explicit TcpServerListen(QObject *parent = 0);
    void init();
    int port;
    int socket_fd;
    int accept_fd;
    void start();
    void stop();
    void deleteServer();
    bool running;
private:
    //sockaddr_in myserver;
    //sockaddr_in remote_addr;
    QThread acceptthread;
    list<TcpClient*> serverList;
    QTimer timer;
signals:

public slots:
    void acceptsocket();
    void ontime();
};

#endif // TCPSERVERLISTEN_H
