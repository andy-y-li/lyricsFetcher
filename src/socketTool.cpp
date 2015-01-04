//
//  lrcFch.cpp
//  lyricsFetcher
//
//  Created by liaogang on 7/7/14.
//  Copyright (c) 2014 gang.liao. All rights reserved.
//

#include "socketTool.h"

#include <string.h> //memset strcpy

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>//fopen


/**
 * is in unix or win-nt
 */
#ifndef _WIN_NT_
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

#define  HTTP_PORT 80
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>//gethostbyname
#include <unistd.h>//close

int GetLastError()
{
    return errno;
}
#endif






//%s for %E5%9B%9A%E9%B8%9F$$%E9%82%93%E7%B4%AB%E6%A3%8B

const char searchHeader[]=
"GET /v1/restserver/ting?method=baidu.ting.search.lrcpic&format=json&from=bmpc&version=1.0.0&version_d=9.0.4.7&query=%s&ts=41451944&type=2&e=UZISa2PLtV8fo8ah2vairbLl33U2XVUlRzuZKXFjVCRcBuGgVlPwdmDiwgqcv52XkNA8Zlvg4tz6PqgwQP9eHQjGor7Ma59Y87C7yZcmMJA%3D HTTP/1.1\
Connection: Keep-Alive\
Referer: http://pc.music.baidu.com\
User-Agent: bmpc_1.0.0\
Host: tingapi.ting.baidu.com";

const char strAddrTing[] = "tingapi.ting.baidu.com" ;





SOCKET socketClient;
BOOL _initSocket()
{
    BOOL bRet ;
    bRet = FALSE;
    
    //in socket
    struct SOCKADDR_IN sockaddrClient;
    memset((void*)&sockaddrClient,0,sizeof(sockaddrClient));
    sockaddrClient.sin_family=AF_INET;
    sockaddrClient.sin_port=htons(HTTP_PORT);
    
    
    //get ip by name
    struct HOSTENT *host= gethostbyname(strAddrTing);
    if (!host)
    {
        bRet = FALSE;
    }
    else
    {
        if(host->h_addrtype == AF_INET)
             sockaddrClient.sin_addr.s_addr=*(u_long *) host->h_addr_list[0];
    }
    
    
    //create client socket
    socketClient=socket(AF_INET,SOCK_STREAM,0);
    if (socketClient==INVALID_SOCKET )
    {
        bRet = FALSE;
        //DWORD error=GetLastError();
    }
    else
          bRet = TRUE;
    
    
    return  bRet;
}



BOOL search(const char * artist , const char * title)
{
    BOOL bRet = FALSE;
    
    SOCKET socketClient ;
    if( CreateTcpSocketClient(strAddrTing, &socketClient) )
    {
        
        
        
        
        
        
        
    }
    
    
    
    
    return bRet;
}






BOOL CreateTcpSocketClient(const char *strHost , SOCKET *socketClient)
{
    BOOL bRet = FALSE ;
    
    //fill socket addr struct
    struct SOCKADDR_IN sockaddrClient;
    memset((void*)&sockaddrClient,0,sizeof(sockaddrClient));
    sockaddrClient.sin_family=AF_INET;
    sockaddrClient.sin_port=htons(HTTP_PORT);
    
    
    //get host's ip address
    struct HOSTENT *host=gethostbyname(strHost);
    if (host)
    {
        if(host->h_addrtype == AF_INET)
        {
            sockaddrClient.sin_addr.s_addr=*(u_long *) host->h_addr_list[0];
            
            //create client socket
            *socketClient=socket(AF_INET,SOCK_STREAM,0);
            if (*socketClient!=INVALID_SOCKET )
            {

                //connect
                if(SOCKET_ERROR==connect(*socketClient,(const struct sockaddr*)&sockaddrClient,sizeof(sockaddrClient)))
                {
                    printf("thread %u:connect error : %d \n", GetLastError() );
                }
                else
                {
                    bRet = TRUE;
                }
            }
            else
            {
                printf("create socket failed.\n");
                //DWORD error=GetLastError();
            }
        }
    }
    else
    {
        printf("can not find the host's address.\n");
    }
    
    return  bRet;
}



/// return bytes sended.
unsigned long sendDataToSocket(SOCKET socket , unsigned char *buffer , unsigned long bufLen)
{
    //send data
    unsigned long send=0,totalsend=0;
    for (;(send=::send(socket,buffer+totalsend,bufLen-totalsend,0))>0;totalsend+=send);
    return totalsend;
}



///return bytes writed to file.
int writeHttpContent2(SOCKET socketDownload, FILE *pFile )
{
    int  bytesWrited = 0 ;
    //recv data
    char *buf = NULL;
    const int RECV_BUF_LEN =2600;
    buf=(char*)malloc(RECV_BUF_LEN);
    if (buf)
    {
        ssize_t byteRecv=0;
        byteRecv=recv(socketDownload,(void*)buf,RECV_BUF_LEN,0);
        if(recv>0)
        {
            const char find1[]="HTTP/1.";//1.1 or 1.0
            const char find2[]=" 200 OK";
            const char constContentLength[] = "\r\nContent-Length: ";
            const char constBreakLine[] = "\r\n\r\n";
            
            int iContentLength=0;
            const int  find1Len = sizeof(find1)/sizeof(find1[0])-1;
            const int find2Len =sizeof(find2)/sizeof(find2[0])-1;
            if (strncmp(buf, find1, find1Len ) == 0 &&
                strncmp(buf + find1Len + 1, find2, find2Len ) == 0
                )
            {
                ////find Content-Length and \r\n\r\n
                char *contentLength = strstr(buf, constContentLength);
                char *breakLine = strstr(buf, constBreakLine);
                
                if (contentLength)
                {
                    contentLength += sizeof(constContentLength)/sizeof(constContentLength[0]) -1;
                    
                    
                    char *p = strchr(contentLength, '\r');
                    
                    char tmp [20] = {0};
                    strncpy(tmp, contentLength , (int) (p - contentLength)) ;
                    iContentLength = atoi(tmp);
                    
                    if (breakLine)
                    {
                        breakLine+=sizeof(constBreakLine)/sizeof(constBreakLine[0])-1;
                        
                        int contentLengthRecv = (int)(buf + byteRecv - breakLine);
                        fwrite( breakLine , sizeof(buf[0]) ,  contentLengthRecv ,pFile);
                        
                        while (contentLengthRecv < iContentLength )
                        {
                            byteRecv=recv(socketDownload , buf  , iContentLength - contentLengthRecv , 0 );
                            
                            contentLengthRecv += byteRecv ;
			
			    fwrite( buf  , sizeof(buf[0]) ,  byteRecv , pFile );
                        }
                        
                        
                        bytesWrited = iContentLength ;
                    }
                    else
                    {
                        printf("http error ,haven't find \"\r\n\r\n\" \n");
                    }
                }
                else
                {
                    printf("http error, haven't find \"Content-Length\"\n");
                }
            }
            else
            {
                printf("http error.\n");
            }
        }
        else
        {
            printf ("recv nothing from server.\n " );
        }
        
        free(buf);
    }
    else
    {
        printf("malloc error \n ");
    }
    
    return bytesWrited;
}


SocketBuffer* recvSocketData(SOCKET socketDownload )
{
    SocketBuffer *resultBuffer = new SocketBuffer;
    resultBuffer->length=0;
    
    int  bytesWrited = 0 ;
    //recv data
    char *buf = NULL;
    const int RECV_BUF_LEN =2600;
    buf=(char*)malloc(RECV_BUF_LEN);
    if (buf)
    {
        ssize_t byteRecv=0;
        byteRecv=recv(socketDownload,(void*)buf,RECV_BUF_LEN,0);
        if(byteRecv>0)
        {
            const char find1[]="HTTP/1.";//1.1 or 1.0
            const char find2[]=" 200 OK";
            const char constContentLength[] = "\r\nContent-Length: ";
            const char constBreakLine[] = "\r\n\r\n";
            
            int iContentLength=0;
            const int  find1Len = sizeof(find1)/sizeof(find1[0])-1;
            const int find2Len =sizeof(find2)/sizeof(find2[0])-1;
            if (strncmp(buf, find1, find1Len ) == 0 &&
                strncmp(buf + find1Len + 1, find2, find2Len ) == 0
                )
            {
                ////find Content-Length and \r\n\r\n
                char *contentLength = strstr(buf, constContentLength);
                char *breakLine = strstr(buf, constBreakLine);
                
                if (contentLength)
                {
                    contentLength += sizeof(constContentLength)/sizeof(constContentLength[0]) -1;
                    
                    
                    char *p = strchr(contentLength, '\r');
                    
                    char tmp [20] = {0};
                    strncpy(tmp, contentLength , (int) (p - contentLength)) ;
                    iContentLength = atoi(tmp);
                    
                    if (breakLine)
                    {
                        breakLine+=sizeof(constBreakLine)/sizeof(constBreakLine[0])-1;
                       
                        free(resultBuffer);
                        resultBuffer=(SocketBuffer*)malloc( sizeof(SocketBuffer) + iContentLength);
                        resultBuffer->length = iContentLength;
                        
                        int contentLengthRecv = (int)(buf + byteRecv - breakLine);
                        memcpy(resultBuffer->buffer, breakLine , contentLengthRecv * sizeof(buf[0]) );
                        
                        
                        while (contentLengthRecv < iContentLength )
                        {
                            byteRecv=recv(socketDownload , resultBuffer->buffer + contentLengthRecv , iContentLength - contentLengthRecv , 0 );
                            
                            contentLengthRecv += byteRecv ;
                        }
                        
                        
                        bytesWrited = iContentLength ;
                        
                        assert(bytesWrited == iContentLength);
                    }
                    else
                    {
                        printf("http error ,haven't find \"\r\n\r\n\" \n");
                    }
                }
                else
                {
                    printf("http error, haven't find \"Content-Length\"\n");
                }
            }
            else
            {
                printf("http error.\n");
            }
        }
        else
        {
            printf ("recv nothing from server.\n " );
        }
        
        free(buf);
    }
    else
    {
        printf("malloc error \n ");
    }
    
    
    return resultBuffer;
}

int writeHttpContent(SOCKET httpResponse , const char *savepath )
{
    int bytesWrited= 0;
    
    FILE * pFile;
    pFile = fopen( savepath , ("w") );
    if (pFile)
    {
        bytesWrited=  writeHttpContent2(httpResponse, pFile) ;
        fclose(pFile);
    }
    else
    {
        printf("can not open file to write. %s",savepath);
    }
    
    
    return bytesWrited;
}



const char curlUrlFileHeaderFormat[] =
"GET %s HTTP/1.1\r\n\
Connection: Keep-Alive\r\n\
Host: %s\r\n\
\r\n\r\n";
const size_t curlUrlFileHeaderFormatLen = sizeof(curlUrlFileHeaderFormat)/sizeof(curlUrlFileHeaderFormat[0]) -1;
const size_t curlUrlFileHeaderFormatSLen = 4; //lenght of %s in above.

int curlUrlFile(const char *url , const char *savepath)
{
    int bytesWrited= 0;
    const char protocol[] = "http://";
    const size_t protocolLen = (sizeof(protocol)/sizeof(protocol[0])-1) ;
    char host[40] = {0};
    const char *path = 0 ;
    
    if (strstr(url, protocol))
    {
        //sub string host
        const char *hostBegin,*hostEnd;
        hostBegin=url + protocolLen  ;
        
        if (hostBegin) {
            hostEnd = strchr(hostBegin, '/');
            if (hostEnd)
            {
                strncpy(host, hostBegin, hostEnd-hostBegin);
                path = hostEnd;
                if (path)
                {
                    //char header[200]={0};
                    //sprintf(header, curlUrlFileHeaderFormat, path , host );
                    
                    
                    SOCKET socketClient;
                    if(CreateTcpSocketClient(host, &socketClient) )
                    {
                        size_t strLen=curlUrlFileHeaderFormatLen - curlUrlFileHeaderFormatSLen + strlen(url) - protocolLen ;
                        void *sendStr=malloc(strLen);
                        sprintf((char*)sendStr,curlUrlFileHeaderFormat,path,host);
                        
                        //send data
                        ssize_t bytesSend=0,totalsend=0;
                        for (;(bytesSend=send(socketClient,(char*)sendStr+totalsend,strLen-totalsend,0))>0;totalsend+=bytesSend);
                        
                        if (totalsend)
                        {
                            bytesWrited = writeHttpContent (socketClient , savepath );
                        }
                        
                        free(sendStr);
                    }
                }
            }
        }
    }
    else
    {
        printf("protocol error.");
    }
    
    
    
    
    return bytesWrited;
}




