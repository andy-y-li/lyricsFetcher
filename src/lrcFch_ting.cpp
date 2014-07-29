//
//  lrcFch.cpp
//  lyricsFetcher
//
//  Created by liaogang on 7/7/14.
//  Copyright (c) 2014 gang.liao. All rights reserved.
//

#include "lrcFch_ting.h"


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








const char searchHeader[]=
"GET /v1/restserver/ting?method=baidu.ting.search.lrcpic&format=json&from=bmpc&version=1.0.0&version_d=9.0.4.7&query=%E5%9B%9A%E9%B8%9F$$%E9%82%93%E7%B4%AB%E6%A3%8B&ts=41451944&type=2&e=UZISa2PLtV8fo8ah2vairbLl33U2XVUlRzuZKXFjVCRcBuGgVlPwdmDiwgqcv52XkNA8Zlvg4tz6PqgwQP9eHQjGor7Ma59Y87C7yZcmMJA%3D HTTP/1.1\
Connection: Keep-Alive\
Referer: http://pc.music.baidu.com\
User-Agent: bmpc_1.0.0\
Host: tingapi.ting.baidu.com";







