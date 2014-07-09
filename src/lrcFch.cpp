//
//  lrcFch.cpp
//  lyricsFetcher
//
//  Created by liaogang on 7/7/14.
//  Copyright (c) 2014 gang.liao. All rights reserved.
//

#include "lrcFch.h"


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>//fopen

#include "stringConv.h"

/**
 * is in unix or win-nt
 */
#ifndef _WIN_NT
typedef unsigned int UINT;
#define FALSE (0)
#define TRUE  (1)
#define BOOL INT
#define INT int
#define SOCKET int
#define HOSTENT hostent
#define INVALID_SOCKET  (-1)
#define SOCKET_ERROR    (-1)
#define SOCKADDR_IN sockaddr_in
#define closesocket(s) close(s)
#define _tcslen wcslen
#define _T(x) L##x

#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>//gethostbyname
#include <unistd.h>//close
#include <iosfwd>//base_string


FILE * _tfopen(const char *filename, const char *mode)
{
    return fopen(filename, mode);
}

int GetLastError()
{
    return errno;
}


#endif



long Conv(int i)
{
    long r = i % 4294967296;
    if (i >= 0 && r > 2147483648L)
        r -= 4294967296L;
    
    if (i < 0 && r < 2147483648L)
        r += 4294967296L;
    return r;
}








void CreateQianQianCode(char *id,char *ar,char*ti,std::string &code)
{
    int iId=atoi(id);
    std::string arti(ar);
    arti+=ti;
    
    int length=arti.length();
    int *song=new int[length];
    for (int i=0;i<length;++i)
    {
        char *ak=(char*)(arti.c_str()+i);
        char  akk=*ak;
        song[i]=((int)akk) & 0x000000FF;
    }
    
    int t1=0,t2=0,t3=0;
    
    t1 = (iId & 0x0000FF00) >> 8;
    
    if ((iId & 0x00FF0000) == 0)
        t3 = 0x000000FF & ~t1;
    else
        t3 = 0x000000FF & ((iId & 0x00FF0000) >> 16);
    
    t3 |= (0x000000FF & iId) << 8;
    t3 <<= 8;
    t3 |= 0x000000FF & t1;
    t3 <<= 8;
    
    if ((iId & 0xFF000000) == 0)
        t3 |= 0x000000FF & (~iId);
    else
        t3 |= 0x000000FF & (iId >> 24);
    
    int j = length - 1;
    while (j >= 0)
    {
        int c = song[j];
        if (c >= 0x80) c = c - 0x100;
        
        t1 = (int)((c + t2) & 0x00000000FFFFFFFF);
        t2 = (int)((t2 << (j % 2 + 4)) & 0x00000000FFFFFFFF);
        t2 = (int)((t1 + t2) & 0x00000000FFFFFFFF);
        j -= 1;
    }
    
    
    j = 0;
    t1 = 0;
    while (j <= length - 1)
    {
        int c = song[j];
        if (c >= 128) c = c - 256;
        int t4 = (int)((c + t1) & 0x00000000FFFFFFFF);
        t1 = (int)((t1 << (j % 2 + 3)) & 0x00000000FFFFFFFF);
        t1 = (int)((t1 + t4) & 0x00000000FFFFFFFF);
        j += 1;
    }
    
    int t5 = (int)Conv(t2 ^ t3);
    t5 = (int)Conv(t5 + (t1 | iId));
    t5 = (int)Conv(t5 * (t1 | t3));
    t5 = (int)Conv(t5 * (t2 ^ iId));
    
    
    long t6 = (long)t5;
    if (t6 > 2147483648)
        t5 = (int)(t6 - 4294967296);
    
    char tmp[20]={0};
    _itoa(t5,tmp,10);
    code=tmp;
}




#define  HTTP_PORT 80
/*千千静听歌词搜索
	网通服务器
	125.39.78.11
	ttlrccnc.qianqian.com
	
 电信服务器
 220.181.5.39
 lrcct2.ttplayer.com
 ttlrcct.qianqian.com
 ttlrcct2.qianqian.com
 */



const char StrServiceDianXin[]="ttlrcct2.qianqian.com";
const char  StrServiceWangTong[]="ttlrccnc.qianqian.com";
const char  StrServiceDownload[]="ttlrcct2.qianqian.com";
//ttlrcct.qianqian.com





SearchLyric::SearchLyric():socketInit(SOCKET_NOT_INIT),downloadSocketInit(FALSE)
{
    
}
INT SearchLyric::InitDownloadSocket()
{
    if(downloadSocketInit)return 1;
    
    MakeSureSocketStartUp();
    
    
    
    //in socket
    memset((void*)&addrDownload,0,sizeof(addrDownload));
    addrDownload.sin_family=AF_INET;
    addrDownload.sin_port=htons(HTTP_PORT);
    
    
    HOSTENT *host=::gethostbyname(StrServiceDownload);
    if (!host)
    {
        return downloadSocketInit=0;
    }
    else
    {
        if(host->h_addrtype == AF_INET)
            addrDownload.sin_addr.s_addr=*(u_long *) host->h_addr_list[0];
    }
    
    
    socketDownload=socket(AF_INET,SOCK_STREAM,0);
    if (socketDownload==INVALID_SOCKET )
    {
        downloadSocketInit=0;
        //DWORD error=GetLastError();
    }
    else
        downloadSocketInit = TRUE;
    
    return downloadSocketInit;
}


//Init search client socket
INT SearchLyric::Init()
{
    MakeSureSocketStartUp();
    socketInit=SOCKET_INIT_FAIL;
    
    
    //in socket
    memset((void*)&sAddr,0,sizeof(sAddr));
    sAddr.sin_family=AF_INET;
    sAddr.sin_port=htons(HTTP_PORT);
    
    
    hostent *host=::gethostbyname(StrServiceDianXin);
    if(host && host->h_addrtype == AF_INET)
    {
        sAddr.sin_addr.s_addr=*(u_long *) host->h_addr_list[0];
    }
    else
    {
        goto r;
    }
    
    
    outS=socket(AF_INET,SOCK_STREAM,0);
    if (outS ==INVALID_SOCKET )
    {
        goto r;
    }
    
    return TRUE;
    
r:
    return socketInit;
}

BOOL SearchLyric::MakeSureSocketStartUp()
{
#ifdef _WIN_32_
    static BOOL bInit=FALSE;
    
    if (!bInit)
    {
        WSADATA data;
        int error;
        error=WSAStartup(MAKEWORD(2,2),&data);
        if (error)
            bInit=FALSE;
        else
            bInit=TRUE;
    }
    return bInit;
#else
    
    return TRUE;
#endif
}

//发送的是artist和title字符小写形式
//的UNICODE编码
BOOL SearchLyric::_Search(const char *artist,const char *title)
{
    if(!Init())
        return FALSE;
    
    if (socketInit!=SOCKET_INIT_SUCCESS)
    {
        outS=socket(AF_INET,SOCK_STREAM,0);
        if (!outS)
        {
            socketInit=FALSE;
            return FALSE;
        }
        socketInit=TRUE;
    }
    
    BOOL connected=FALSE;
    int error=::connect(outS,(sockaddr*)&sAddr,sizeof(sAddr));
    if(error==SOCKET_ERROR)
    {
        
        //DWORD errorNumber=GetLastError();
        int errorNumber = errno;
        if (errorNumber==10056)
        {
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        connected=TRUE;
    }
    
static	const char *header=
"GET /dll/lyricsvr.dll?sh?Artist=%s&Title=%s&Flags=2&ci=525b525b56454d7178646432492b345316101c0265631a5d5157535e55424c671c464808425c5353011d01 HTTP/1.1\r\n\
Host: %s\r\n\
\r\n\0\0";
    
    string codeAr=str2UnicodeCode(artist,strlen(artist));
    string codeTi=str2UnicodeCode(title,strlen(title));
    int strLen=strlen(header)-6 + codeAr.length()+codeTi.length() + strlen(StrServiceDianXin);
    char *sendStr=new char[strLen];
    sprintf(sendStr,header,codeAr.c_str(),codeTi.c_str(),StrServiceDianXin);
    
    //send data
    int send=0,totalsend=0;
    for (;(send=::send(outS,sendStr+totalsend,strLen-totalsend,0))>0;totalsend+=send);
    
    //recv data
    char *buf;
    const int RECV_BUF_LEN =200;
    buf=(char*)malloc(RECV_BUF_LEN);
    memset(buf,0,RECV_BUF_LEN);
    strRecv.clear();
    
    int recv=0,totalRecv=0;
    while((recv=::recv(outS,buf,RECV_BUF_LEN,0)) >0)
    {
        string tmp(buf,RECV_BUF_LEN);
        
        totalRecv+=recv;
        
        strRecv+=tmp;
        memset(buf,0,RECV_BUF_LEN);
    }
    
    free(buf);
    closesocket(outS);
    
    return 1;
}

BOOL SearchLyric::_ParseResult()
{
    int lineIndex = 0;
    auto index=strRecv.find("<result>");
    if(index>= strRecv.npos)
        return FALSE;
    
    strRecv.erase(0,index);
    index=0;
    
    vecLrcLines.clear();
    
    while(1)
    {
        index=strRecv.find("<lrc",index);
        auto last=strRecv.find("</lrc>",index);
        if(index == 0 || last == string::npos)
        {
            break;
        }
        else
        {
            int findEnd;
            static const char * strToFind[]={"id=\"","artist=\"","title=\""};
            string r[3];
            for (int i=0;i<3;++i)
            {
                index=strRecv.find(strToFind[i],index);
                index+=strlen(strToFind[i]);
                findEnd=strRecv.find("\"",index);
                r[i]=strRecv.substr(index,findEnd-index);
                index=findEnd;
                
                
                vecLrcLines.push_back(r[i]);
            }
            
            lineIndex++;
            printf("%d :%s , %s \n",lineIndex,r[1].c_str(), r[2].c_str());
        }
        
        
    }//while(1)
    
    return vecLrcLines.size();
}

BOOL SearchLyric::_SaveLyricToFile(const char *filepath)
{
    BOOL ret=FALSE;
    const char find[]={"Server: apache"};
    const char ign[]={"\r\n\t "};
    
    int idx=strRecv.find(find,0);
    idx+=sizeof(find);
    strRecv.erase(0,idx);
    
    idx=strRecv.find_first_not_of(ign,0);
    strRecv.erase(0,idx);
    
    idx=strRecv.find_last_not_of(ign);
    if(idx!=strRecv.npos)
        strRecv.erase(idx+1);
    
    FILE * pFile;
    pFile = fopen( filepath, ("w") );
    if (pFile)
    {
        fwrite(strRecv.c_str(),sizeof(char),strRecv.length(),pFile);
        fclose(pFile);
        printf("Lyric is saved to file : %s \n",filepath);
        ret=TRUE;
    }
    else
    {
        printf("Unable to open file %s \n",filepath);
    }
    
    return ret;
}


BOOL SearchLyric::_Download(int idx,const char *savepath)
{
    assert(idx+1 <= vecLrcLines.size() && idx >=0 );
    
    return _DownloadLyric((char*)vecLrcLines[idx*3].c_str(),
                         (char*)vecLrcLines[idx*3+1].c_str(),
                         (char*)vecLrcLines[idx*3+2].c_str())
    ?_SaveLyricToFile(savepath):FALSE;
}


BOOL SearchLyric::_DownloadLyric(char *id,char *ar,char*ti)
{
    InitDownloadSocket();
    
    socketDownload=socket(AF_INET,SOCK_STREAM,0);
    if (socketDownload==INVALID_SOCKET)
        return FALSE;
    
    if(SOCKET_ERROR==::connect(socketDownload,(sockaddr*)&addrDownload,sizeof(addrDownload)))
    {
        if (GetLastError()!=10056)
            return FALSE;
    }
    
    
    string code;
    CreateQianQianCode(id,ar,ti,code);
    
static const  char *header=
"GET /dll/lyricsvr.dll?dl?Id=%s&Code=%s HTTP/1.1\r\n\
Host: %s\r\n\
Connection: keep-alive\r\n\
\r\n\0\0";
    
    int strLen=strlen(header)-6 + strlen(id) + code.length() + sizeof(StrServiceDownload);
    char *sendStr=new char[strLen];
    sprintf(sendStr,header,id,code.c_str(),StrServiceDownload);
    
    //send data
    int send=0,totalsend=0;
    for (;(send=::send(socketDownload,sendStr+totalsend,strLen-totalsend,0))>0;totalsend+=send);
    
    //recv data
    char *buf;
    const int RECV_BUF_LEN =300;
    buf=(char*)malloc(RECV_BUF_LEN);
    strRecv.clear();
    
    
    int recv=0;
    while((recv=::recv(socketDownload,buf,RECV_BUF_LEN,0)) >0)
    {
        string tmp(buf,buf+recv);
        strRecv+=tmp;
    }
    
    free(buf);
    
    return TRUE;
}