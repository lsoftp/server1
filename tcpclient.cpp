#include "tcpclient.h"
#include <QDebug>
#include <dbinterface.h>
#include <QHostAddress>

bool TcpClient::isconfig =false;
bool TcpClient::isdisplay =true;
const string TcpClient::HEX_DIGITS[16]={"0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"};
TcpClient::TcpClient(QObject *parent) :
    QObject(parent)
{
    //add proxy
    proxy_socket = new QTcpSocket(this);
    proxy_socket->connectToHost(QHostAddress("101.201.174.75"),4133);
    m_isOkConect = proxy_socket->waitForConnected(3000);
    connect(proxy_socket, SIGNAL(disconnected()), this, SLOT(onDisConnect()));
}
void TcpClient::onDisConnect()
{
    m_isOkConect=false;
}
void TcpClient::reconnect()
{
    if(!m_isOkConect){
    proxy_socket->connectToHost(QHostAddress("101.201.174.75"),4133);
    m_isOkConect = proxy_socket->waitForConnected(3000);
    }
}

void TcpClient::setsock(QTcpSocket* p)
{
    m_lasthearttime=QDateTime::currentDateTime();
    ac_socket =p;

    //ac_socket->moveToThread(pts);
    connect(p,SIGNAL(readyRead()), this, SLOT(slot_readdata()));
    connect(this,SIGNAL(ss()), this, SLOT(stop()));
    connect(this,SIGNAL(se()), this, SLOT(getmsg_send()));
    connect(this,SIGNAL(addrow1(string,string,QString,string)),&db,SLOT(addlogs11(string,string,QString,string)));
    connect(this,SIGNAL(addboxlog(string,QString,int,int,double,double,float,int,int,int)),&db,SLOT(addboxlog(string,QString,int,int,double,double,float,int,int,int)));
    connect(this,SIGNAL(sendbr(string,QString,int,int,double,double,float,int,int,int)),&dbhandle,SLOT(process(string,QString,int,int,double,double,float,int,int,int)));
    connect(this,SIGNAL(sendpositioin(string,QString,PositionReport)),&dbhandle,SLOT(positionprocess(string,QString,PositionReport)));
    connect(this,SIGNAL(sendcan(string,QString,string,string,string)),&dbhandle,SLOT(rsendcan(string,QString,string,string,string)));
    connect(this,SIGNAL(sendwater(string,QString,double,double,double)),&dbhandle,SLOT(rsendwater(string,QString,double,double,double)));
    connect(this,SIGNAL(sendoil(string,QString,double,double,double)),&dbhandle,SLOT(rsendoil(string,QString,double,double,double)));
}
TcpClient::~TcpClient()
{
    //add proxy
    proxy_socket->close();
    proxy_socket->waitForDisconnected();
    delete proxy_socket;
}
void TcpClient::setConfig(bool b)
{
    printf("isconfig is %d\n",isconfig);
    isconfig=b;
    printf("isconfig is %d\n",isconfig);
                fflush(stdout);
}

void TcpClient::init()
{
        /*port = 8899;
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
*/
}

void TcpClient::start()
{
    running=true;
    if(!srthread.isRunning()){
            this->moveToThread(&srthread);
            connect(&srthread,SIGNAL(started()),this,SLOT(sendrecv()));
            srthread.start();
      }
    //m_lasthearttime.fromMSecsSinceEpoch(0);
    m_lasthearttime=QDateTime::currentDateTime();
            connect(this,SIGNAL(addPacket(string,string,QTime,RecvStream)),&db,SLOT(addlog(string,string,QTime,RecvStream)));
        connect(this,SIGNAL(addrow(string,string,QTime,string)),&db,SLOT(addlogs(string,string,QTime,string)));
            connect(this,SIGNAL(addrow1(string,string,QString,string)),&db,SLOT(addlogs1(string,string,QString,string)));
}
void TcpClient::stop()
{
    running =false;

    ac_socket->close();
    isRunning =false;
}

int TcpClient::getMsgFromBuf()
{
    RecvStream recvstream,rs;
    static int i=-1,j=-1;
    static int t=0;
    int r=0;

//        QDateTime now=QDateTime::currentDateTime();
//        if(m_lasthearttime.toMSecsSinceEpoch() != 0){
//            //long span=now.currentMSecsSinceEpoch() - m_lasthearttime.currentMSecsSinceEpoch();
//            int span=m_lasthearttime.secsTo(now);
//            if (span >10) {
//                isRunning=false;
//                //emit ss();
//                stop();
//                return 0;
//            }
//        }
        //mutexRecvStream.lock();
        if(recvbuf.size>0)
        {
            r=recvbuf.getDataFromBuf(recvstream.stream, &(recvstream.size));
            if ((i!= recvbuf.size)|| (j !=recvstream.size))
            {
                i= recvbuf.size;
                j=recvstream.size;
                printf("recvbuf size %d,r %d,recvstream.size %d\n", recvbuf.size,r,recvstream.size);
            }
        }
        //mutexRecvStream.unlock();
        if(r>0)
        {
            for(int i=0; i<recvstream.size;i++)
                printf("%02x ", recvstream.stream[i]);
            printf("\n");
            fflush(stdout);
            //888888888888888 add proxy

            QByteArray qb;
            qb.resize(recvstream.size);
               memcpy(qb.data(), recvstream.stream, recvstream.size);
               proxy_socket->write(qb);
            //99999999999999999999999999999
            //r =0;
            int y=toOriginalMsg(recvstream.stream,recvstream.size, rs.stream,&rs.size);
            if(y!=0) return -1;
            int j = checkCode(rs.stream,rs.size);
            if( j < 0)
            {
                //continue;
            }
            if(j ==1)
            {
                MsgHeader header;
                int i = header.fromStream(rs.stream);
                QTime ttt1;
                QDate d1;
                        string s,s1,s2;
                s = toHexStr((unsigned char*)header.phoneNumber,6);
                //mw->ui->tableWidget->setItem(t,0,new QTableWidgetItem(s.c_str()));
                MYWORD tmp=qFromBigEndian(header.msgId);

                s1 = toHexStr((unsigned char*)&(tmp),2);
                //mw->ui->tableWidget->setItem(t,1,new QTableWidgetItem(s.c_str()));
                s2 = toHexStr(rs.stream,rs.size);
                //mw->ui->tableWidget->setItem(t,3,new QTableWidgetItem(s1.c_str()));
                ttt1=QTime::currentTime();
                d1=QDate::currentDate();

                QString sss=d1.toString("yyyy-MM-dd")+" "+ttt1.toString("hh:mm:ss.zzz");
                //emit addPacket(s,s1,ttt1,rs);
                       // cout <<"**************"<< s1 << s2<< s;
                emit addrow1(s,s1,sss,s2);
                if(isdisplay)
                    emit addPacket(s,s1,s2);
                t++;
                m_lasthearttime=QDateTime::currentDateTime();
                //cout << "msgid "<<hex<<header.msgId<<endl;
                switch(header.msgId)
                {
                    case 0x0100: handleRegister(&rs);break;
                    case 0x0102: handleAuthentication(&rs);break;
                    case 0x0002:handleheartbeat(&rs);
                                m_lasthearttime=QDateTime::currentDateTime();
                    break;
                    case 0x0704:handleblind(&rs);break;
                    case 0x0200:handleposition(&rs);break;
                    case 0x0705:handcan(&rs);break;
                    case 0x0fff: handlebox(&rs);break;
                    case 0x0001: handleboxconfig(&rs);break;
                    default:
                            handledefault(&rs);  break;
                }
            }

        }
    return r;
    //this->stop();
}

void TcpClient::handleboxconfig(RecvStream* prs)
{
    TerminalAck ta;
    ta.fromStream(prs->stream);
    MYWORD sn=ta.serialNumber;
    removesnfrommsglist(sn);
}

void TcpClient::removesnfrommsglist(MYWORD sn)
{

    Msg *pmsg = NULL;
        list<Msg>::iterator it1;
        mutex.lock();
        for(it1 = msgList.begin(); it1 != msgList.end(); )
        {
            pmsg = &(*it1);
            printf("p->sn %d  sn %d  isack  iscomplete\n",pmsg->msgSerialNumber,sn,pmsg->complete );
            if((pmsg->msgSerialNumber == sn)&&(!pmsg->isAck)&&(pmsg->complete))

            //if((pmsg->complete)&&((pmsg->resendTimes >= m_resendtimes)||(pmsg->isAck)))
            {
                msgList.erase(it1++);


            }
            else
            {
                ++it1;
            }
        }
        mutex.unlock();



}
void gettime(MYBCD tttt[6])
{
  int h=  QTime::currentTime().hour();
  int m=QTime::currentTime().minute();
  int s=QTime::currentTime().second();
  int y=QDate::currentDate().year();
  int M=QDate::currentDate().month();
  int d=QDate::currentDate().day();

 printf("y %d %d %d  %d %d %d\n", y,M,d,h,m,s);
 fflush(stdout);
  tttt[0]=(((y%100/10)&0x0f)<<4)|((y%10)&0x0f);
  tttt[1]=(((M/10)&0x0f)<<4)|((M%10)&0x0f);
  tttt[2]=(((d/10)&0x0f)<<4)|((d%10)&0x0f);

  tttt[3]=(((h/10)&0x0f)<<4)|((h%10)&0x0f);
  tttt[4]=(((m/10)&0x0f)<<4)|((m%10)&0x0f);
  tttt[5]=(((s/10)&0x0f)<<4)|((s%10)&0x0f);

  for(int i=0; i<6;i++)
      printf("%02x ", tttt[i]);
  printf("\n");
  fflush(stdout);
}

void setTime(TrushBoxAck *ta)
{
    int h,m;
    QTime qti=QTime::currentTime();
    QTime st =qti.addSecs(1500);
    QTime ed =qti.addSecs(7200);
    ed=qti.addSecs(123);
    st= QTime(5,0);
    ed= QTime(16,0);
    h=st.hour(); m=st.minute();
    ta->worktime[0]=(((h/10)&0x0f)<<4)|((h%10)&0x0f);
    ta->worktime[1]=(((m/10)&0x0f)<<4)|((m%10)&0x0f);
    h=ed.hour(); m=ed.minute();
    ta->worktime[2]=(((h/10)&0x0f)<<4)|((h%10)&0x0f);
    ta->worktime[3]=(((m/10)&0x0f)<<4)|((m%10)&0x0f);
    for(int i=0; i<4;i++)
        printf("%02x ", ta->worktime[i]);
    printf("\n");
    fflush(stdout);
}

void TcpClient::handlebox(RecvStream* prs)
{
    Msg msg;
    TrushBoxAck ra;
    MsgHeader head;

    head.fromStream(prs->stream);
    BoxReport br;
   br.fromStream(prs->stream,prs->size);
   string s = toHexStr((unsigned char*)head.phoneNumber,6);
   QTime ttt1;
   QDate d1;

   ttt1=QTime::currentTime();
   d1=QDate::currentDate();

   QString sss=d1.toString("yyyy-MM-dd")+" "+ttt1.toString("hh:mm:ss.zzz");
  //printf("report: id:% %d %d %d\n", br.distance,br.power,br.weidu,br.jindu);
   //emit addboxlog(s,sss,br.percent,br.power,br.w,br.j1,br.temp1,br.gps,br.gprs,br.distance);
   emit sendbr(s,sss,br.percent,br.power,br.w,br.j1,br.temp1,br.gps,br.gprs,br.distance);
   qDebug()<<"send thread id is "<<QThread::currentThreadId()<<endl;
   fflush(stdout);
    ra.header = head;
    ra.header.msgId = 0x8fff;
    ra.header.property = 9;
    ra.serialNumber=head.msgSerialNumber;
    gettime(ra.time);
    setTime(&ra);

    if (isconfig)
        ra.result =5;
    else
        ra.result =0;
    if(ra.result ==5)
    {
        ra.span=6;
        ra.mode=0x04;
        ra.limit=2000;
        ra.header.property = 13;
    }
    msg.msgSerialNumber=ra.serialNumber;

    unsigned char ori[1024];
    int len;
    len = ra.toStream(ori);
    len = addCheckCode(ori,len);

    toComposedMsg(ori,len, msg.stream, &(msg.len));
    if(ra.result !=5) msg.isAck =true;
    printf("send\n");
    for(int i=0; i<msg.len;i++)
        printf("%02x ", msg.stream[i]);
    printf("\n");
    fflush(stdout);
    //mutex.lock();
    msgList.push_front(msg);
    //mutex.unlock();
}
/*
void TcpServer::handlebox(RecvStream* prs)
{
    Msg msg;
    TrushBoxAck ra;
    MsgHeader head;

    head.fromStream(prs->stream);
    BoxReport br;
   br.fromStream(prs->stream,prs->size);
   printf("report: %d %d %d %d\n", br.distance,br.power,br.weidu,br.jindu);
   fflush(stdout);
    ra.header = head;
    ra.header.msgId = 0x8fff;
    ra.header.property = 9;
    ra.serialNumber=head.msgSerialNumber;
    if (isconfig)
        ra.result =5;
    else
        ra.result =0;
    if(ra.result ==5)
    {
        ra.span=1;
        ra.mode=5;
        ra.limit=2000;
        ra.header.property = 13;
    }
    msg.msgSerialNumber=ra.serialNumber;

    unsigned char ori[1024];
    int len;
    len = ra.toStream(ori);
    len = addCheckCode(ori,len);

    toComposedMsg(ori,len, msg.stream, &(msg.len));
    if(ra.result !=5) msg.isAck =true;
    printf("send\n");
    for(int i=0; i<msg.len;i++)
        printf("%02x ", msg.stream[i]);
    printf("\n");
    mutex.lock();
    msgList.push_front(msg);
    mutex.unlock();
}*/
//int main(int argc, char **argv)
//{
//char *in_utf8 = "姝ｅ?ㄥ??瑁?";
//char *in_gb2312 = "正在安装";
//char out[OUTLEN];

//// utf-8-->gb2312
//CodeConverter cc = CodeConverter("utf-8","gb2312");
//cc.convert(in_utf8,strlen(in_utf8),out,OUTLEN);
//cout << "utf-8-->gb2312 in=" << in_utf8 << ",out=" << out << endl;

//// gb2312-->utf-8
//CodeConverter cc2 = CodeConverter("gb2312","utf-8");
//cc2.convert(in_gb2312,strlen(in_gb2312),out,OUTLEN);
//cout << "gb2312-->utf-8 in=" << in_gb2312 << ",out=" << out << endl;
//}

void TcpClient::handleRegister(RecvStream* prs)
{
    Msg msg;
    RegisterAck ra;
    MsgHeader head;
    char *in_utf8 = "sound";
    QString out=UTF82GBK(  in_utf8);
    // utf-8-->gb2312
    //CodeConverter cc = CodeConverter("utf-8","gb2312");
    //cc.convert(in_utf8,strlen(in_utf8),out,OUTLEN);
    //cout << "utf-8-->gb2312 in=" << in_utf8 << ",out=" << out << endl;

    head.fromStream(prs->stream);
    ra.header = head;
    ra.header.msgId = 0x8100;
    ra.sn=head.msgSerialNumber;
    ra.authenticationCode =out.toStdString();

    ra.result = 0;
    ra.header.property = 8;
    unsigned char ori[1024];
    int len;
    len = ra.toStream(ori);
    len = addCheckCode(ori,len);

    toComposedMsg(ori,len, msg.stream, &(msg.len));
    msg.isAck =true;
    printf("send\n");
    for(int i=0; i<msg.len;i++)
        printf("%02x ", msg.stream[i]);
    printf("\n");
    mutex.lock();
    msgList.push_front(msg);
    mutex.unlock();

}

void TcpClient::handleheartbeat(RecvStream* prs)
{
    Msg msg;
    PlatformAck pa;
    pa.header.fromStream(prs->stream);
    pa.serialNumber=pa.header.msgSerialNumber;
    pa.header.msgId = 0x8001;
    pa.header.property =5;
    pa.result = 0;
    pa.msgId = 0x0002;
    unsigned char ori[1024];
    int len;
    len = pa.toStream(ori);
    len = addCheckCode(ori,len);
    toComposedMsg(ori,len, msg.stream, &(msg.len));
    msg.isAck =true;
    mutex.lock();
    msgList.push_front(msg);
    mutex.unlock();


}
void TcpClient::handledefault(RecvStream* prs)
{
    Msg msg;
    PlatformAck pa;
    pa.header.fromStream(prs->stream);
    pa.serialNumber=pa.header.msgSerialNumber;
    pa.msgId = pa.header.msgId;
    pa.header.msgId = 0x8001;
    pa.header.property =5;
    pa.result = 0;

    unsigned char ori[1024];
    int len;
    len = pa.toStream(ori);
    len = addCheckCode(ori,len);
    toComposedMsg(ori,len, msg.stream, &(msg.len));
    msg.isAck =true;
    mutex.lock();
    msgList.push_front(msg);
    mutex.unlock();

}

void TcpClient::handcan(RecvStream* prs){
    Msg msg;
    PlatformAck pa;
    pa.header.fromStream(prs->stream);
    pa.serialNumber=pa.header.msgSerialNumber;
    pa.header.msgId = 0x8001;
    pa.header.property =5;
    pa.result = 0;
    pa.msgId = 0x0705;
    unsigned char ori[1024];
    int len;
    len = pa.toStream(ori);
    len = addCheckCode(ori,len);
    toComposedMsg(ori,len, msg.stream, &(msg.len));
    msg.isAck =true;
    mutex.lock();
    msgList.push_front(msg);
    mutex.unlock();
    processcandata(prs);
}
// parse can data ,insert into can log ,waterlog, oillog
void TcpClient::processcandata(RecvStream *prs)
{
    MsgHeader mh;
    int j= mh.fromStream(prs->stream);
    string s = toHexStr((unsigned char*)mh.phoneNumber,6);
    QTime ttt1;
    QDate d1;

    ttt1=QTime::currentTime();
    d1=QDate::currentDate();

    QString sss=d1.toString("yyyy-MM-dd")+" "+ttt1.toString("hh:mm:ss.zzz");
    int  num=qFromBigEndian(*(MYWORD*)(prs->stream+j));
    j+=2;
    string stime = toHexStr((unsigned char*)(prs->stream+j),5);
    j+=5;
    for (int i=0;i<num;i++)
        {
            string canid=toHexStr((unsigned char*)(prs->stream+j),4);
            j+=4;

            string data=toHexStr((unsigned char*)(prs->stream+j),8);
            if (canid=="1c8ad0d3")
            {
                //oil
                MYWORD  v=qFromLittleEndian(*(MYWORD*)(prs->stream+j));
                MYWORD  v1=qFromLittleEndian(*(MYWORD*)(prs->stream+j+2));
                short    t=qFromLittleEndian(*(MYWORD*)(prs->stream+j+4));
                double dv=v*0.01;//cm
                double dv1=v1*0.01;//cm
                //emit insert delayed into oil
                emit sendoil(s,sss,dv,dv1,t);

            }
            if (canid=="1c8ad0a1")
            {
                //water
                MYWORD  v=qFromLittleEndian(*(MYWORD*)(prs->stream+j));
                MYWORD  v1=qFromLittleEndian(*(MYWORD*)(prs->stream+j+2));
                short    t=qFromLittleEndian(*(MYWORD*)(prs->stream+j+4));
                double dv=v*0.1;//cm
                double dv1=v1*0.1;//cm

                //emit insert int water
                emit sendwater(s,sss,dv,dv1,t);
            }
            j+=8;
            //insert into canlog
            emit sendcan(s,sss,stime,canid,data);

        }
}




void TcpClient::handleposition(RecvStream* prs){

    Msg msg;
    PlatformAck pa;
    pa.header.fromStream(prs->stream);

    PositionReport pr;
   pr.fromStream(prs->stream,prs->size);
   string s = toHexStr((unsigned char*)pa.header.phoneNumber,6);
   QTime ttt1;
   QDate d1;

   ttt1=QTime::currentTime();
   d1=QDate::currentDate();

   QString sss=d1.toString("yyyy-MM-dd")+" "+ttt1.toString("hh:mm:ss.zzz");
  //printf("report: id:% %d %d %d\n", br.distance,br.power,br.weidu,br.jindu);
   //emit addboxlog(s,sss,br.percent,br.power,br.w,br.j1,br.temp1,br.gps,br.gprs,br.distance);
   emit sendpositioin(s,sss,pr);

    pa.serialNumber=pa.header.msgSerialNumber;
    pa.header.msgId = 0x8001;
    pa.header.property =5;
    pa.result = 0;
    pa.msgId = 0x0200;
    unsigned char ori[1024];
    int len;
    len = pa.toStream(ori);
    len = addCheckCode(ori,len);
    toComposedMsg(ori,len, msg.stream, &(msg.len));
    msg.isAck =true;
    mutex.lock();
    msgList.push_front(msg);
    mutex.unlock();
}

void TcpClient::handleblind(RecvStream* prs){
    Msg msg;
    PlatformAck pa;
    pa.header.fromStream(prs->stream);
    pa.serialNumber=pa.header.msgSerialNumber;
    pa.header.msgId = 0x8001;
    pa.header.property =5;
    pa.result = 0;
    pa.msgId = 0x0704;
    unsigned char ori[1024];
    int len;
    len = pa.toStream(ori);
    len = addCheckCode(ori,len);
    toComposedMsg(ori,len, msg.stream, &(msg.len));
    msg.isAck =true;
    mutex.lock();
    msgList.push_front(msg);
    mutex.unlock();
}

void TcpClient::handleAuthentication(RecvStream* prs)
{
    Msg msg;
    //
    PlatformAck pa;
    pa.header.fromStream(prs->stream);
    pa.serialNumber=pa.header.msgSerialNumber;
    pa.header.msgId = 0x8001;
    pa.header.property =5;
    pa.result = 0;
    pa.msgId = 0x0102;
    unsigned char ori[1024];
    int len;
    len = pa.toStream(ori);
    len = addCheckCode(ori,len);
    toComposedMsg(ori,len, msg.stream, &(msg.len));
    msg.isAck =true;
    mutex.lock();
    msgList.push_front(msg);
    mutex.unlock();


}

void TcpClient::acceptsocket()
{

}
Msg * TcpClient::getMsgToSend()
{
    Msg *pmsg=NULL, *pmsg1 = NULL;
    struct timeval endtime;
    gettimeofday(&endtime, NULL);
    list<Msg>::iterator it;
//	printf("sdfdfdsfdsf\n");
    //fflush(stdout);
    //mutex.lock();
    for(it=msgList.begin(); it != msgList.end(); ++it)
    {

        pmsg = &(*it);
        long int time = (endtime.tv_sec - pmsg->sendTime.tv_sec)*1000000 + (endtime.tv_usec - pmsg->sendTime.tv_usec);
        if(!(pmsg->complete))
        {
            pmsg1 = pmsg;
            break;
        }
        else if((time >= m_timeout) &&!(pmsg->isAck))
        {
            if(pmsg->resendTimes <m_resendtimes)
            {
                pmsg->sendChars = 0;
                pmsg->complete = false;
                pmsg1 = pmsg;
                break;
            }

        }
    }
    //mutex.unlock();
    return pmsg1;
}
int  TcpClient::handleMsgList()
{
    MYWORD sn;
    Msg *pmsg = NULL;
        list<Msg>::iterator it1;
        //mutex.lock();
        for(it1 = msgList.begin(); it1 != msgList.end(); )
        {
            pmsg = &(*it1);


            if((pmsg->complete)&&((pmsg->resendTimes >= m_resendtimes)||(pmsg->isAck)))
            {
                msgList.erase(it1++);
                if(pmsg->resendTimes >= m_resendtimes)
                {
                    //save msg or other things
                }

            }
            else
            {
                ++it1;
            }
        }
       // mutex.unlock();



}

void TcpClient::slot_readdata()
{
    printf("**************************************\n");
    fflush(stdout);
    static int ttt=-9,ttt1=-9;
    QByteArray qb;


        qb=this->ac_socket->readAll();
    //recvChars = recv(sfd, recvbuf.stream+recvbuf.size, 1560-recvbuf.size,0);
    int recvChars =qb.size();
    //mutexRecvStream.lock();
    if (recvChars+recvbuf.size>1560){
        int copylen= 1560-recvbuf.size;
        memcpy(recvbuf.stream+recvbuf.size,&qb.data()[recvChars-copylen],copylen);
        recvChars =copylen;
    }
    else
        memcpy(recvbuf.stream+recvbuf.size,qb.data(),recvChars);
    if((ttt!= recvbuf.size)&&(ttt1!=recvChars)){
        ttt= recvbuf.size;
        ttt1=recvChars;
        printf("recvbuffing len:%d,%d\n", recvbuf.size,recvChars);
        fflush(stdout);

    }
    if( recvChars < 0 )
    {
        printf("recv message error\n");

    }
    else if(recvChars == 0)
    {
        //网络断开，尝试重连
    }
    else{
        recvbuf.size += recvChars;

    }
    //if recvbuf
    getmsg_send();
}

void TcpClient::getmsg_send()
{
//     Msg *pmsg=NULL;
     int     haha;
     //slot_readdata();
     while(getMsgFromBuf()!=0)
     {
         haha=msgList.size();
     };

    //handleMsgList();
     while((pmsg = getMsgToSend())!=NULL)
     {
        //printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%d\n",pmsg);

        if((pmsg != NULL) && (pmsg->sendChars < pmsg->len)&&(!pmsg->complete))
        {
             QByteArray qb;
             qb.resize(pmsg->len - pmsg->sendChars);
                memcpy(qb.data(), pmsg->stream+pmsg->sendChars, pmsg->len - pmsg->sendChars);
            int sendChars=this->ac_socket->write(qb);
         //this->ac_socket->flush();
         //sendChars = send( sfd,pmsg->stream+pmsg->sendChars, pmsg->len - pmsg->sendChars,0 );
            if( sendChars < 0 )
            {
                     printf("send message error\n");
                     //return ;
            }
            else
            {
                 printf("send chars%d\n",sendChars);
                pmsg->sendChars += sendChars;
            }
        }
        if((pmsg != NULL) && (pmsg->sendChars == pmsg->len)&&(!pmsg->complete))
        {
             (pmsg->resendTimes)++;
             gettimeofday(&(pmsg->sendTime),NULL);
             pmsg->complete = true;
             pmsg = NULL;
        }

      }

    handleMsgList();
    haha=msgList.size();
    //printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@size%d\n",haha);
    fflush(stdout);

}

void TcpClient::sendrecv()
{

}
int TcpClient::toOriginalMsg(unsigned char * composed,int comlen, unsigned char * original, int *origlen)
{
    int j = 0;
    int start = 0;
    int tmplen = 0;

    for(int i=0;i<comlen;i++)
    {
        if ((composed[i] == 0x7e)&&(i != comlen-1))
        {
            start = i + 1;
            continue;
        }
        if ((composed[i] == 0x7e)&&(i == comlen-1))
        {
            if(tmplen != 0)
            {
                        memcpy(&original[j], &composed[start],tmplen);
            }
            j = j + tmplen;
            continue;
        }
        if (composed[i] != 0x7d)
        {
            tmplen++;
        }
        else
        {
            printf("j=%d  tmplen= %d, start=%d\n", j, tmplen, start);
            if(tmplen != 0)
            {
                        memcpy(&original[j], &composed[start],tmplen);
            }
            if(composed[i+1] == 0x02)
            {
                original[j+tmplen] = 0x7e;
            }
            else if(composed[i+1] == 0x01)
            {
                original[j+tmplen] = 0x7d;
            }
            else
            {
                return -1;
            }
            j = j + tmplen + 1;
            start = start + tmplen + 2;
            i++;
            tmplen = 0;
        }
    }
    *origlen = j;


    return 0;
}

int TcpClient::toComposedMsg(unsigned char * original,int origlen, unsigned char * composed, int* comlen)
{
    int j = 0;
    int tmplen  = 0;
    int start = 0;

    composed[j] = 0x7e;
    j++;
    for(int i =0; i< origlen; i++)
    {
        if((original[i] != 0x7e)&&(original[i]!=0x7d))
        {
            tmplen++;
        }
        else
        {
        //printf("j=%d  tmplen= %d, start=%d\n", j, tmplen, start);
            if(tmplen != 0)
            {
                memcpy(composed+j, original+start, tmplen);
            }
            start =start+tmplen+1;
            if(original[i] == 0x7e)
            {
                composed[j+tmplen] = 0x7d;
                composed[j+tmplen+1] = 0x02;
            }
            if(original[i] == 0x7d)
            {
                composed[j+tmplen] = 0x7d;
                composed[j+tmplen+1] = 0x01;
            }

            j=j+tmplen+2;
            tmplen =0;
        }
    }

    if(tmplen != 0)
    {
        memcpy(composed+j, original+start, tmplen);

    }
    j = j + tmplen;
    composed[j]=0x7e;
    j++;

    *comlen = j;
    return 0;
}

int TcpClient::addCheckCode(unsigned char * original, int len)
{
    unsigned char c= 0x00;
    for(int i = 0;i<len; i++)
    {
        c= c ^original[i];
    }
    original[len] = c;
    return len+1;
}

int TcpClient::checkCode(unsigned char * original, int len)
{
    unsigned char c=0x00;
    if(len<2)
    {
        return -2;
    }
    for(int i=0; i<len-1;i++)
    {
        c = c^original[i];
    }
    if(original[len-1] != c)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

/**
 * convert bytes array to hex string
 */
string TcpClient::toHexStr(unsigned char* bytes, int len)
{
    int h,l;
    std::ostringstream out1;

    for(int i=0;i<len;i++) {

        h = (bytes[i]>>4)&0xf;
        l = bytes[i]&0xf;

        out1<<HEX_DIGITS[h]<<HEX_DIGITS[l];
    }

    return out1.str();
}

void TcpClient::setAccept_fd(int acfd)
{
    this->accept_fd= acfd;
}
