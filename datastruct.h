#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <string>
#include <string.h>
//#include <arpa/inet.h>
#include <list>
#include <unistd.h>
#include <iostream>
#include <QtEndian>
#include <sstream>
//#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
//#include <sys/times.h>
#include <sys/time.h>
//#include <sys/select.h>
#include <exception>
#include <stdexcept>
#include "pthread.h"
#include <QTextCodec>

#define BUFFER_SIZE 1560

using namespace std;

typedef string MYSTRING;
typedef unsigned char MYBYTE;
typedef unsigned short MYWORD;
typedef unsigned int MYDWORD;
typedef unsigned char MYBCD;

inline QString UTF82GBK(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    //QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QString utf2gbk = gbk->toUnicode(inStr.toLocal8Bit());
    return utf2gbk;
}
struct Msg
{
    unsigned char stream[BUFFER_SIZE];
    int len;
    int resendTimes;
    MYWORD msgSerialNumber;
    int sendChars;
    struct timeval sendTime;
    bool complete;
    bool isAck;
    Msg()
    {
        isAck = false;
        resendTimes = 0;
        complete = false;
        sendChars = 0;
    }
};

struct RecvStream
{
    unsigned char stream[BUFFER_SIZE];
    int size;
};

class RecvBuf
{
public:
    unsigned char stream[BUFFER_SIZE];
    int size;
    RecvBuf()
    {
        size = 0;
    }
    int getDataFromBuf(unsigned char *buf, int *len)
    {
        int j,begin = -1,end=-1;
        if(size == 0)
        {
            *len = 0;
            return 0;
        }

        for (j=0;j<size;j++)
        {
            if (stream[j] == 0x7e)
            {
                if(begin == -1)
                {
                    begin=j;
                }
                else
                {
                    end=j;
                    break;
                }
            }
        }
        if(end == begin +1)
        {
            memmove(stream, stream+end, size - end);
            size = size -end;
            return -1;
        }
        if((begin != -1)&&(end == -1)) //not found end of the packet
        {
            if (size==BUFFER_SIZE)
            {
                size=0;
                return 0;
            }
            memmove(stream, stream+begin, size - begin);
            size = size -begin;
            return 0;
        }
        if((begin == -1)&&(end == -1)) // no begin or end added 07 07
        {
            *len = 0;
            size = 0;
            return 0;
        }

        int check=verifyMsg(begin,end);
        printf("check is %d\n",check);
        if (0==check){
            memcpy(buf, stream+begin, end-begin +1);
            memmove(stream, stream+end+1,size - end-1);
            size = size -end -1;
            *len = end -begin +1;
            return *len;

        }
        else{
            memmove(stream, stream+end,size - end);
            size = size -end ;
            *len = 0;
            return -1;

        }
        if((size -end -1) >0)
        {
            memcpy(buf, stream+begin, end-begin +1);
            memmove(stream, stream+end+1,size - end-1);
            size = size -end -1;
            *len = end -begin +1;
            return *len;
        }
        else if ((size - end -1) == 0)
        {
            memcpy(buf, stream+begin, size-begin);

            *len = size -begin;
            size=0;
            return *len;
        }
        else
        {
            *len = 0;
            return 0;
        }
    }
    int verifyMsg(int s, int e)
    {
        //int j = 0;
        int start = 0;
        //int tmplen = 0;
        int comlen =e-s+1;
        unsigned char c=0x00;
        if(comlen<15)
        {
            return -2;
        }


        for(int i=0;i<comlen-2;i++)
        {
            if ((stream[i+s] == 0x7e))
            {
                start = i +s+ 1;
                continue;
            }

            if (stream[i+s] != 0x7d)
            {
                c = c^stream[i+s] ;
            }
            else
            {
                if((i+s)!=(e-2))
                {
                    if(stream[i+s+1] == 0x02)
                    {
                        c = c^0x7e;
                        //i++;
                    }
                    else if(stream[i+s+1] == 0x01)
                    {
                        c = c^0x7d;
                        //i++;
                    }
                    else
                    {
                        return -1;
                    }

                    i++;

                }
                else
                {
                    if(stream[i+s+1] == 0x02)
                    {
                        return c== 0x7e?0:-1;
                        //i++;
                    }
                    else if(stream[i+s+1] == 0x01)
                    {
                        return c== 0x7d?0:-1;
                        //i++;
                    }
                    else
                    {
                        return -1;
                    }
                }

            }
        }
        if (c==stream[e-1])
            return 0;
        else

        return -3;
    }

};

struct MsgHeader
{
    MYWORD msgId;
    MYWORD property;
    MYBCD  phoneNumber[6];
    MYWORD msgSerialNumber;
    MYWORD packetCount;
    MYWORD packetNo;
    int toStream(unsigned char * original)
    {
        int j = 0;
        MYWORD tmp;
        tmp = qToBigEndian(msgId);
        memcpy(original, &tmp, sizeof(MYWORD));
        j += sizeof(MYWORD);
        tmp = qToBigEndian(property);
        memcpy(original+j, &tmp,2);
        j += 2;
        memcpy(original+j, phoneNumber,6);
        j += 6;
        tmp =qToBigEndian(msgSerialNumber);
        memcpy(original+j, &tmp, 2);
        j += 2;
        if(property& 0x2000 )
        {
            tmp = qToBigEndian(packetCount);
            memcpy(original +j, &tmp,2);
            j += 2;
            tmp = qToBigEndian(packetNo);
            memcpy(original + j, &tmp,2);
            j += 2;
        }
        return j;
    }
    int fromStream(unsigned char * original)
    {
        int j =0;
        msgId=qFromBigEndian(*(MYWORD*)(original+j));
        j += 2;
        property = qFromBigEndian(*(MYWORD *)(original+j));
        j += 2;
        memcpy(&phoneNumber,original+j,6);
        j += 6;
        msgSerialNumber = qFromBigEndian(*(MYWORD*)(original+j));
        j += 2;
        if(property & 0x2000)
        {
            packetCount = qFromBigEndian(*(MYWORD *)(original+j));
            j += 2;
            packetNo = qFromBigEndian(*(MYWORD*)(original+j));
            j += 2;
        }
        return j;
    }
};

class TerminalAck
{
public:
    MsgHeader header; //0x0001
    MYWORD serialNumber;
    MYWORD msgId;
    MYBYTE result; //0 success ,1 failure, 2 invalid msg 3 not support

    TerminalAck()
    {
        header.msgId = 0x0001;
    }

    int toStream(unsigned char * original)
    {
        int j;
        j = header.toStream(original);
        MYWORD tmp;
        tmp = qToBigEndian(serialNumber);
        memcpy(original + j, &tmp, 2);
        j += 2;
        tmp = qToBigEndian(msgId);
        memcpy(original + j, &tmp, 2);
        j += 2;
        memcpy(original + j, &result, 1);
        j += 1;
        return j;
    }
    int fromStream(unsigned char * original)
    {
        int j = 0;
        j = header.fromStream(original);
        serialNumber=qFromBigEndian(*(MYWORD*)(original+j));
        j += 2;
        msgId = qFromBigEndian(*(MYWORD *)(original+j));
        j += 2;
        result = *(MYBYTE *)(original+j);
        j += 1;
        return j;
    }

};
class TrushBoxAck
{
public:
    MsgHeader header; //0x8001
    MYWORD serialNumber;
    MYBYTE result; //0 success ,1 failure, 2 invalid msg 3 not support
    MYBCD time[6]={0x17,0x07,0x07,0x16,0x36,0x07};
    MYBYTE span; //default 24 hours
    MYBYTE mode=0; //0~5 5 test mode 0 super power saving
    MYWORD limit;//default 1000
    MYBCD worktime[4]={0x08,0x30,0x15,0x00};
    int toStream(unsigned char *ori)
    {
        int j;
        j = header.toStream(ori);
        MYWORD tmp;
        tmp = qToBigEndian(serialNumber);
        memcpy(ori+j,&tmp,2);
        j += 2;
//        tmp = qToBigEndian(msgId);
//        memcpy(ori+j,&tmp,2);
//        j += 2;
        memcpy(ori+j,&result,1);
        j += 1;
        memcpy(ori+j,&time,6);
        j += 6;
        if(5==result)
        {
            memcpy(ori+j,&span,1);
            j += 1;
            memcpy(ori+j,&mode,1);
            j += 1;
            tmp =qToBigEndian(limit);
            memcpy(ori+j,&tmp,2);
            j += 2;
        }
        if(mode>15 &&mode < 32)
        {
            memcpy(ori+j,&worktime,4);
            j+=4;
        }
        return j;

    }

};
/*
class TrushBoxAck
{
public:
    MsgHeader header; //0x8001
    MYWORD serialNumber;
    MYBYTE result; //0 success ,1 failure, 2 invalid msg 3 not support
    MYBCD time[6]={0x17,0x07,0x07,0x07,0x07,0x07};
    MYBYTE span; //default 24 hours
    MYBYTE mode; //0~5 5 test mode 0 super power saving
    MYWORD limit;//default 1000

    int toStream(unsigned char *ori)
    {
        int j;
        j = header.toStream(ori);
        MYWORD tmp;
        tmp = qToBigEndian(serialNumber);
        memcpy(ori+j,&tmp,2);
        j += 2;
//        tmp = qToBigEndian(msgId);
//        memcpy(ori+j,&tmp,2);
//        j += 2;
        memcpy(ori+j,&result,1);
        j += 1;
        memcpy(ori+j,&time,6);
        j += 6;
        if(5==result)
        {
            memcpy(ori+j,&span,1);
            j += 1;
            memcpy(ori+j,&mode,1);
            j += 1;
            tmp =qToBigEndian(limit);
            memcpy(ori+j,&tmp,2);
            j += 2;
        }
        return j;

    }

};

*/


class PlatformAck
{
public:
    MsgHeader header; //0x8001
    MYWORD serialNumber;
    MYWORD msgId;
    MYBYTE result; //0 success ,1 failure, 2 invalid msg 3 not support


    int fromStream(unsigned char * original)
    {
        int j = 0;
        j = header.fromStream(original);
        serialNumber=qFromBigEndian(*(MYWORD*)(original+j));
        j += 2;
        msgId = qFromBigEndian(*(MYWORD *)(original+j));
        j += 2;
        result = *(MYBYTE *)(original+j);
        j += 1;
        return j;
    }
    int toStream(unsigned char *ori)
    {
        int j;
        j = header.toStream(ori);
        MYWORD tmp;
        tmp = qToBigEndian(serialNumber);
        memcpy(ori+j,&tmp,2);
        j += 2;
        tmp = qToBigEndian(msgId);
        memcpy(ori+j,&tmp,2);
        j += 2;
        memcpy(ori+j,&result,1);
        j += 1;
        return j;

    }

};

class Register
{
public:
    MsgHeader header; //0x0100
    MYWORD provinceId;
    MYWORD cityId;
    MYBYTE manufacturerId[5];
    MYBYTE type[8];
    MYBYTE terminalId[7];
    MYBYTE plateColor;
    MYSTRING plateNumber;

    Register()
    {

    }
    int toStream(unsigned char * original)
    {
        int j;
        j = header.toStream(original);
        MYWORD tmp;
        tmp = qToBigEndian(provinceId);
        memcpy(original +j , &tmp,2);
        j += 2;
        tmp = qToBigEndian(cityId);
        memcpy(original + j, &tmp,2);
        j += 2;
        memcpy(original + j, manufacturerId,5);
        j += 5;
        memcpy(original + j, type, 8);
        j += 8;
        memcpy(original + j, terminalId, 7);
        j += 7;
        memcpy(original + j, &plateColor, 1);
        j += 1;
        memcpy(original + j, plateNumber.c_str(), plateNumber.length()+1);
        j =  j + plateNumber.length()+1;
        return j;

    }


};

class Authentication
{
public:
    MsgHeader header; //0x0102
    MYSTRING code;
    Authentication()
    {
        header.msgId = 0x0102;
    }
    int toStream(unsigned char * original)
    {
        int j;
        j = header.toStream(original);
        memcpy(original+j, code.c_str(),code.length()+1);
        j = j + code.length() + 1;
        return j;
    }
};

class RegisterAck
{
public:
    MsgHeader header; //0x8100
    MYWORD sn;
    MYBYTE result;
    MYSTRING authenticationCode;

    int fromStream(unsigned char* original, int len)
    {
        int j = 0;
        j = header.fromStream(original);
        sn = qFromBigEndian(*(MYWORD*)(original+j));
        j += 2;
        result = *(original+j);
        j += 1;
        if(0 == result)
        {
            authenticationCode = (char*)(original+j);
            j = j + authenticationCode.length() + 1;
        }
        len =j;
        return j;

    }
    int toStream(unsigned char * original)
    {
        int j;
        j = header.toStream(original);
        MYWORD tmp;
        tmp = qToBigEndian(sn);
        memcpy(original+j, &tmp,2);
        j += 2;
        memcpy(original+j, &result,1);
        j += 1;
        memcpy(original+j, authenticationCode.c_str(),authenticationCode.length()+1);//problem with cpy len,should no +1
        j = j+ authenticationCode.length();
        return j;
    }

};

class BoxReport
{
public:
    MsgHeader header; //0x8100
    MYWORD percent;

    MYBYTE power;
    MYDWORD weidu;
    MYDWORD jindu;
    short  temp;
    MYBYTE gps;
    MYBYTE gprs;
    MYWORD distance;
    double w;
    double j1;
    float temp1;
    int fromStream(unsigned char* original, int len)
    {
        int j = 0;
        j = header.fromStream(original);
        percent = qFromBigEndian(*(MYWORD*)(original+j));
        j += 2;
        power = *(original+j);
        j += 1;
        weidu = qFromBigEndian(*(MYDWORD*)(original +j));
        w=weidu*0.000001;
        j += 4;
        jindu = qFromBigEndian(*(MYDWORD*)(original +j));
        j1=jindu*0.000001;
        j += 4;
        temp= qFromBigEndian(*(MYWORD*)(original+j));
        temp1=temp*0.1;
        j +=2;
        gps=*(original+j);
        j+=1;
        gprs=*(original+j);
        j+=1;
        distance = qFromBigEndian(*(MYWORD*)(original+j));
        j+=2;

        return j;

    }
 /*   int toStream(unsigned char * original)
    {
        int j;
        j = header.toStream(original);
        MYWORD tmp;
        tmp = qToBigEndian(sn);
        memcpy(original+j, &tmp,2);
        j += 2;
        memcpy(original+j, &result,1);
        j += 1;
        memcpy(original+j, authenticationCode.c_str(),authenticationCode.length()+1);
        j = j+ authenticationCode.length();
        return j;
    }
*/
};
class BatPosition
{
public:
    MsgHeader header; //0x8100
    MYWORD count;
    MYBYTE type; //0 bat upload  1 blindarea
    MYSTRING authenticationCode;

    int fromStream(unsigned char* original, int len)
    {
        int j = 0;
        j = header.fromStream(original);
        count = qFromBigEndian(*(MYWORD*)(original+j));
        j += 2;
        type = *(original+j);
        j += 1;

        len =j;
        return j;

    }


};

class PositionReport
{
public:
    MsgHeader header;//0x0200
    MYDWORD warningMark;
    MYDWORD status;
    MYDWORD latitude;
    MYDWORD longitude;
    MYWORD altitude;
    MYWORD speed;
    MYWORD direction;
    MYBCD time[6];

    //status
    const static MYDWORD acc = 0x00000001;
    const static MYDWORD location = 0x00000002;
    const static MYDWORD south  = 0x00000004;
    const static MYDWORD west = 0x00000008;
    const static MYDWORD outage = 0x00000010;
    const static MYDWORD encryption = 0x00000020;
    const static MYDWORD oidDisconnect = 0x00000400;
    const static MYDWORD circuitDisconnect = 0x00000800;
    const static MYDWORD lock =0x00001000;

    //warning mark
    const static MYDWORD alarmSwitch = 0x00000001;
    const static MYDWORD overspeed = 0x00000002;
    const static MYDWORD fatigueDriving = 0x00000004;
    const static MYDWORD forewarning = 0x00000008;
    const static MYDWORD GNSSModuleFailure = 0x00000010;
    const static MYDWORD GNSSAntennaOffline = 0x00000020;
    const static MYDWORD GNSSAntennaShortout = 0x00000040;
    const static MYDWORD undervoltage = 0x00000080;
    const static MYDWORD powerFailure = 0x00000100;
    const static MYDWORD displayFault = 0x00000200;
    const static MYDWORD TTSFailure = 0x00000400;
    const static MYDWORD cameraFailure = 0x00000800;
    const static MYDWORD drivingTimeout = 0x00040000;
    const static MYDWORD parkingTimeout = 0x00080000;
    const static MYDWORD inOutArea = 0x00100000;
    const static MYDWORD inOutLine = 0x00200000;
    const static MYDWORD drivingTimeShortOrTooLong = 0x00400000;
    const static MYDWORD lineDeparture = 0x00800000;
    const static MYDWORD VSSFailure = 0x01000000;
    const static MYDWORD abnormalOil = 0x02000000;
    const static MYDWORD stolen = 0x04000000;
    const static MYDWORD illegalIgnition = 0x08000000;
    const static MYDWORD illegalDisplacement = 0x10000000;

    //additional info id
    const static MYBYTE mileage = 0x01;
    const static MYBYTE oilMass = 0x02;
    const static MYBYTE recordSpeed = 0x03;
    const static MYBYTE overspeedAlarm = 0x11;
    const static MYBYTE inOutAlarm = 0x12;
    const static MYBYTE timeShortOrTooLong = 0x13;

    struct OverspeedInfo
    {
        MYBYTE positionType;
        MYDWORD id;
    };

    struct InOutInfo
    {
        MYBYTE positionType;
        MYDWORD id;
        MYBYTE direction;
    };

    struct ShortOrTooLongInfo
    {
        MYDWORD id;
        MYWORD time;
        MYBYTE result;
    };

    int toStream(unsigned char * original)
    {
        int j;
        j = header.toStream(original);
        MYDWORD dw = qToBigEndian(warningMark);
        memcpy(original+j, &dw, sizeof(dw));
        j += 4;
        dw = qToBigEndian(status);
        memcpy(original+j, &dw, sizeof(dw));
        j += 4;
        dw = qToBigEndian(latitude);
        memcpy(original+j, &dw, sizeof(dw));
        j += 4;
        dw = qToBigEndian(longitude);
        memcpy(original+j, &dw, sizeof(dw));
        j += 4;
        MYWORD tmp = qToBigEndian(altitude);
        memcpy(original+j,&tmp, sizeof(tmp));
        j += 2;
        tmp = qToBigEndian(speed);
        memcpy(original+j,&tmp, sizeof(tmp));
        j += 2;
        tmp = qToBigEndian(direction);
        memcpy(original+j,&tmp, sizeof(tmp));
        j += 2;
        memcpy(original+j,time,6);
        j += 6;


        return j;
    }

    int fromStream(unsigned char* original, int len)
    {
        int j = 0;
        j = header.fromStream(original);
        warningMark = qFromBigEndian(*(MYDWORD*)(original+j));
        j += 4;

        status = qFromBigEndian(*(MYDWORD*)(original +j));
        //w=weidu*0.000001;
        j += 4;
        latitude = qFromBigEndian(*(MYDWORD*)(original +j));
        //j1=jindu*0.000001;
        j += 4;
        longitude= qFromBigEndian(*(MYDWORD*)(original+j));
        //temp1=temp*0.1;
        j +=4;
        altitude =qFromBigEndian(*(MYWORD*)(original+j));
        j+=2;
        speed = qFromBigEndian(*(MYWORD*)(original+j));

        j+=2;
        direction=qFromBigEndian(*(MYWORD*)(original+j));
        j+=2;
        //time = qFromBigEndian(*(MYWORD*)(original+j));
        j+=6;

        return j;

    }
};


#endif // DATASTRUCT_H
