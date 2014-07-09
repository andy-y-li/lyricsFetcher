/**
* lrcFch.h
* lyricsFetcher

*Created by liaogang on 7/7/14.
*Copyright (c) 2014 gang.liao. All rights reserved.

* Usage:
 *1. Search lyrics with the param artist and title , we will get a lyrics list
 *2. choose a index in the lyrics list
 *3. specify a path to save
 *
 History:
 * fixed at 2014.7.7 convert to unix.  add changed the title and artist in lyrics file , from unicode str to hex mem. by lg.
 *
 *Author:
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

using namespace std;
class SearchLyric
{
public:
    //id artist title ,...
    vector<string> vecLrcLines;
    
    
    SearchLyric();

    /**search and parse result
    *return the result lyrics numbers
     */
    inline int Search(const char *artist,const char *title)
    {
        if(_Search(artist, title))
            return _ParseResult();
        
        return 0;
    }
private:
    BOOL _Search(const char *artist,const char *title);
    BOOL _ParseResult();
    
public:
    /**download and save lyrics file to the path.
     */
    inline BOOL Download(int idx,const char *savepath)
    {
        if(_Download(idx, savepath) )
            return _SaveLyricToFile(savepath);
        
        return FALSE;
    }
    
private:
    BOOL _Download(int idx,const char *savepath);
    BOOL _DownloadLyric(char *id,char *ar,char*ti);
    BOOL _SaveLyricToFile(const char *filepath);
    
    
private:
    BOOL MakeSureSocketStartUp();
    INT Init();
    INT InitDownloadSocket();
    
    
    
    //search lyric
    int outS;
    SOCKADDR_IN sAddr;
    INT socketInit;
    
    string strRecv;
    enum
    {
        SOCKET_NOT_INIT=-1,
        SOCKET_INIT_FAIL,
        SOCKET_INIT_SUCCESS
    };
    
    //download
    SOCKET socketDownload;
    SOCKADDR_IN addrDownload;
    INT downloadSocketInit;

};