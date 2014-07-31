//
//  lrcFch.cpp
//  lyricsFetcher
//
//  Created by liaogang on 7/7/14.
//  Copyright (c) 2014 gang.liao. All rights reserved.
//

#include "lrcFch_ting.h"
#include "socketTool.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>//fopen

#include "stringConv.h"

#ifdef DEBUG
#include <pthread.h>
#else
int pthread_self()
{
	return 0;
}
#endif

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

#define  HTTP_PORT 80
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>//gethostbyname
#include <unistd.h>//close
#include <iosfwd>//base_string


#endif






//%s for %E5%9B%9A%E9%B8%9F$$%E9%82%93%E7%B4%AB%E6%A3%8B

const char searchHeader[]=
"GET /v1/restserver/ting?method=baidu.ting.search.lrcpic&format=json&from=bmpc&version=1.0.0&version_d=9.0.4.7&query=%s&ts=41451944&type=2&e=%s HTTP/1.1\r\n\
Connection: Keep-Alive\r\n\
Referer: http://pc.music.baidu.com\r\n\
User-Agent: bmpc_1.0.0\r\n\
Host: tingapi.ting.baidu.com\r\n\
\r\n\r\n";
const char searchParamE[] = "UZISa2PLtV8fo8ah2vairbLl33U2XVUlRzuZKXFjVCRcBuGgVlPwdmDiwgqcv52XkNA8Zlvg4tz6PqgwQP9eHQjGor7Ma59Y87C7yZcmMJA%3D";
const size_t searchHeaderLen= sizeof(searchHeader)/sizeof(searchHeader[0]) -1 ;
const size_t searchHeaderSLen = 4; // bytes of %s in above.
const char strAddrTing[] = "tingapi.ting.baidu.com" ;




bool tingSearch(const char * artist , const char * title)
{
    bool bRet = false;
    
    char lrcAddress[240]={0};
    
    //search
    SOCKET socketClient ;
    if( CreateTcpSocketClient(strAddrTing, &socketClient) )
    {
        
        std::string ar = str2UnicodeCode( artist, strlen(artist) ,TRUE );
        //$$
        std::string ti = str2UnicodeCode( title, strlen(title) ,TRUE );
        
        ar.append("$$");
        ar.append(ti);
        
        size_t sendLen = searchHeaderLen - searchHeaderSLen + strlen( ar.c_str() ) + strlen(searchParamE) ;
        char *sendStr = (char*)malloc(sendLen);
        if (sendStr) {
            
            sprintf(sendStr, searchHeader , ar.c_str() ,searchParamE );
            
            //send
            ssize_t bytesSended=0;
            if ( (bytesSended = send(socketClient, sendStr,  (int)sendLen , 0)))
            {
                //recv
                const size_t bufLen = 2500 ;
                char buf[bufLen] ={0};
                
                //get "lrclink":"http://musicdata.baidu.com/data2/lrc/84888208/%E.lrc"
                const char  find1[] =  "\"lrclink\"";
                const char find2[]="http:\\/\\/";
                const char find3[]=".lrc";
                
                const size_t find1Len = sizeof(find1)/sizeof(find1[0]) -1;
                const size_t find2Len = sizeof(find2)/sizeof(find2[0]) -1;
                const size_t find3Len = sizeof(find3)/sizeof(find3[0]) -1;
                
                char *findPtr = 0;
                char *find2Ptr=0;
                while (1)
                {
                    ssize_t bytesRecv = recv(socketClient,  buf ,  bufLen ,  0);
                    
                    findPtr = strstr(buf, find1);
                    if (findPtr)
                    {
                        findPtr+=find1Len;
                        
                        findPtr = strstr(findPtr, find2);
                        if (findPtr)
                        {
                            find2Ptr = strstr(findPtr, find3);
                            if (find2Ptr)
                            {
                                find2Ptr+=find3Len;
                                //now the lrc address is findPtr end with find2Ptr.
                                //let's remove the '\\' char in it.
                                char *lrcaddrPtr=lrcAddress;
                                while (1)
                                {
                                    char *tmp=strchr(findPtr, '\\');
                                    if (tmp && tmp< find2Ptr)
                                    {
                                        strncpy(lrcaddrPtr, findPtr, tmp - findPtr);
                                        lrcaddrPtr+=tmp-findPtr;
                                        findPtr=tmp+1;
                                    }
                                    else
                                    {
                                        strncpy(lrcaddrPtr, findPtr, find2Ptr - findPtr);
                                        break;
                                    }
                                }
                                
                                
                                
                                curlUrlFile(lrcAddress, "/Users/liaogang/a.txt");
                            }
                        }
                        
                        break;
                    }
                }
                
            }
            
            
            free(sendStr);
        }
        
        
    }
    
    
    
    
    return bRet;
}







