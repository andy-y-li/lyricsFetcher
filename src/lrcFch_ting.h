/**
* lrcFch.h
* lyricsFetcher

*Created by liaogang on 7/7/14.
*Copyright (c) 2014 gang.liao. All rights reserved.
*/



#include <vector>
#include <string>





/**
 * is in unix or win-nt
 */
#ifndef _WIN_NT
#include <sys/types.h>
#include <netinet/in.h>//sockaddr_in
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
#endif


