#include "tcpserverlisten.h"

TcpServerListen::TcpServerListen(QObject *parent) :
    QObject(parent)
{
}

void TcpServerListen::init()
{
       /* port = 9999;
        if(( socket_fd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0 ){
                throw "socket() failed";
        }

        memset(&myserver,0,sizeof(myserver));
        myserver.sin_family = AF_INET;
        myserver.sin_addr.s_addr = htonl(INADDR_ANY);
        myserver.sin_port = qToBigEndian(port);

        if( bind(socket_fd,(sockaddr*) &myserver,sizeof(myserver)) < 0 ) {
            printf("bind socket error: %s(errno:%d)\n)",strerror(errno),errno);
             fflush(stdout);
                           return ;                                                      ;
        }

        if( listen(socket_fd,10) < 0 ) {
                throw "listen() failed";
        }
        this->moveToThread(&acceptthread);
        connect(&acceptthread,SIGNAL(started()),this,SLOT(acceptsocket()));
        acceptthread.start();
        timer.setInterval(5000);
        connect(&timer,SIGNAL(timeout()), this, SLOT(ontime()));
*/
}

void TcpServerListen::ontime()
{
    deleteServer();

}

void TcpServerListen::start()
{
    running = true;
    init();

}
void TcpServerListen::stop()
{
    running =false;
    close(socket_fd);
    MYWORD sn;
    TcpClient *pserver = NULL;
    list<TcpClient *>::iterator it1;
    for(it1 = serverList.begin(); it1 != serverList.end(); )
    {
        pserver = (*it1);
        pserver->stop();
    }
    deleteServer();




}
void TcpServerListen::deleteServer()
{
    TcpClient *pserver = NULL;
    list<TcpClient *>::iterator it1;
    for(it1 = serverList.begin(); it1 != serverList.end(); )
    {
        pserver = (*it1);
        if (!pserver->isRunning)
        {
            delete pserver;
            serverList.erase(it1++);
        }



    }

}

void TcpServerListen::acceptsocket()
{

     /*   while( running ) {

                socklen_t sin_size = sizeof(struct sockaddr_in);
                if(( accept_fd = accept(socket_fd,(struct sockaddr*) &remote_addr,&sin_size)) == -1 )
                {
                        throw "Accept error!";
                        continue;
                }
//        if(!srthread.isRunning()){
//                this->moveToThread(&srthread);
//                connect(&srthread,SIGNAL(started()),this,SLOT(sendrecv()));
//                srthread.start();
//            }
//        }
             TcpClient* pts=new TcpClient;
             pts->setAccept_fd(accept_fd);
             pts->start();
             serverList.push_front(pts);
        }


        return ;*/
}
