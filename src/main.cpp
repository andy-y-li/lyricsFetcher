/**
 * 
 */

#include <stdlib.h>
#include "lrcFch.h"
#include <pthread.h>
#include "threadpool.h"
#include "fileCtrl.h"
#include "audioTag.h"
#include <unistd.h>

#include <sys/stat.h>//stat


#ifdef DEBUG
#include <assert.h>
#endif


/**
 * brief "artist" , "title" , "/a/b"  to "/a/b/artist-title.lrc"
 * param lyricsFilename : path to save. without a '/'.
 */
char *GenerateLyricsName(const char *artist , const char *title ,const char *path , char *fullname)
{
    strcpy ( fullname ,path );
    
    char *tmp  = strcat(fullname , "/" );
    
    tmp =  strcat( tmp ,artist  );
    
    tmp =strcat(tmp, "-");
    
    tmp = strcat(tmp , title );
    
    strcat(tmp , ".lrc");
    
    return fullname;
}



struct lrcFchS
{
    char artist[128];
    char title[128] ;
    const char *savepath;
};

void lrcFchSInit(lrcFchS *s, const char *a , const char *t, const char *p)
{
    if(a)
        strcpy(s->artist, a);
    if(t)
        strcpy(s->title, t);
    s->savepath=p;
}

void * lrcFchThread(void* lrcFchArg)
{
    lrcFchS *s = (lrcFchS*)lrcFchArg ;
    const char *artist = s->artist;
    const char *title = s->title;
    const char *savepath= s->savepath;
    
    SearchLyric sl;
    if(  sl.Search(artist,title) )
    {
	char fullname[128] ={ 0 };

        sl.Download( 0 , GenerateLyricsName(artist ,title , savepath ,fullname ) );
    }
    else
    {
        printf("thread %u: search failed.\n",pthread_self() );
    }
        
    return 0;
}

bool FileExists(const char* filename)
{
    struct stat info;
    int ret = -1;
    
    //get the file attributes
    ret = stat(filename, &info);
    if(ret == 0)
    {
        //stat() is able to get the file attributes,
        //so the file obviously exists
        return true;
    }
    else
    {
        //stat() is not able to get the file attributes,
        //so the file obviously does not exist or
        //more capabilities is required
        return false;
    }
}



void* addJobIsFileAudio(const char * file ,void *arg)
{
    char *savepath= "/Users/shijunhe/lyrics";
    
    lrcFchS * lrcFchArg = (lrcFchS*)malloc(sizeof(lrcFchS)) ;
    
    lrcFchArg->savepath = savepath;
    
    
    if (getId3Info(file , lrcFchArg->artist,  lrcFchArg->title )    )
    {
        char fullname[128] = {0 };
        
        GenerateLyricsName( lrcFchArg->artist , lrcFchArg->title , lrcFchArg->savepath , fullname );
        
        //if fullname is exist in file system. ignore to search and download the lyrics.
        if ( ! FileExists(fullname ) )
        {
            printf("%u , audio's tag info : %s , %s \n" , pthread_self() ,lrcFchArg->artist , lrcFchArg ->title );
            //printf("job .. \n");
            pool_add_job(lrcFchThread, (void*)lrcFchArg );
            //sleep(12);
        }
    }
    
    return nullptr;
}

int main(int argc, const char * argv[]) {

    pool_init(4);
    
    IterFiles(string ("/Users/shijunhe/Music/Music/abc"), string ("/Users/shijunhe/Music/Music/abc"), addJobIsFileAudio, nullptr );
    
    pool_destroy();
    
/*    
    while (1) {
        printf("~~~.");
        sleep(2);
    }
 */   

    
    return 0;
}

