#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "datastruct.h"
#include "dbhandle.h"
//#include "tcpserver.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>
#include <iostream>
//#include <sys/socket.h>
//#include <arpa/inet.h>
#include <QTcpSocket>
#include <QTime>
#include <QObject>
#include <QThread>
#include <QMutex>
#include <unistd.h>
#include <iostream>
//#include <sys/socket.h>
//#include <arpa/inet.h>
#include <QDateTime>
//#include "tcpserver.h"


#define MAXSIZE 1024


class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);
    QTcpSocket *proxy_socket;
    QMutex mutexbuf;
    virtual ~TcpClient();
    QDateTime m_lasthearttime;
    //TcpServer *pts;
    void setAccept_fd(int acfd);
         void hahah(){ emit se();}

    void init();
    void start();

    bool running;
    bool isRunning= true;
    MainWindow *mw;
    QTcpSocket *ac_socket;
    void setsock(QTcpSocket* p);

    static bool isconfig;
         static bool isdisplay;
private:
    QThread acceptthread;
    QThread srthread;
    QThread getMsgThread;
    int socket_fd;
    int accept_fd;
    int sendchar=-1;
    int port;
    bool m_isOkConect;
    //sockaddr_in myserver;
    //sockaddr_in remote_addr;
    RecvBuf recvbuf;

     list<Msg> msgList;
     QMutex mutex;
     list<RecvStream> recvStreamList;
     QMutex mutexRecvStream;
     const static int m_timeout=2500000;
     const static int m_resendtimes=3;

     const static string HEX_DIGITS[16];


     string toHexStr(unsigned char* bytes, int len);
     int toOriginalMsg(unsigned char * composed,int comlen, unsigned char * original, int *origlen);
     int toComposedMsg(unsigned char * original,int origlen, unsigned char * composed, int* comlen);
     int addCheckCode(unsigned char * original , int len);
     int checkCode(unsigned char * original, int len);
     Msg * getMsgToSend();
     int  handleMsgList();
     void handleRegister(RecvStream* prs);
     void handleAuthentication(RecvStream* prs);
     void handleheartbeat(RecvStream* prs);
     void handleposition(RecvStream* prs);
     void handleblind(RecvStream* prs);
     void handcan(RecvStream* prs);
     void handledefault(RecvStream* prs);
     void handlebox(RecvStream* prs);
     // parse can data ,insert into can log ,waterlog, oillog
     void processcandata(RecvStream *prs);
     void handleboxconfig(RecvStream* prs);
     void removesnfrommsglist(MYWORD sn);
    int getMsgFromBuf();
signals:
    //void addPacket(string,string,QTime,RecvStream);
        void addPacket(string,string,string);
        void addrow(string,string ,QTime,string);
        void addrow1(string,string ,QString,string);
        void addboxlog(string,QString,int,int,double,double,float,int,int,int);
        void sendbr(string,QString,int,int,double,double,float,int,int,int);
        void sendpositioin(string,QString,PositionReport);
        void sendcan(string,QString,string,string,string);
        void sendoil(string,QString,double,double,double);
        void sendwater(string,QString,double,double,double);
        void ss();
        void se();
public slots:
    void sendrecv();
    void acceptsocket();

    void setConfig(bool b);
    void slot_readdata();
    void stop();
    void getmsg_send();
    void reconnect();
    void onDisConnect();
};


//#include <iconv.h>
#include <iostream>

#define OUTLEN 255

using namespace std;
/*
// 代码转换操作类
class CodeConverter {
private:
iconv_t cd;
public:
// 构造
int out;
CodeConverter(const char *from_charset,const char *to_charset) {
cd = iconv_open(to_charset,from_charset);
}

// 析构
~CodeConverter() {
iconv_close(cd);
}

// 转换输出
int convert(char *inbuf,int inlen,char *outbuf,int outlen) {
    char **pin = &inbuf;
    char **pout = &outbuf;

    memset(outbuf,0,outlen);
    int rr;
    rr= iconv(cd,pin,(size_t *)&inlen,pout,(size_t *)&outlen);
    out=outlen;
    return rr;
}
};

*/
#endif // TCPSERVER_H
