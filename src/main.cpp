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
        string sp (savepath);
        sp+="/";
        sp+=artist;
        sp+="-";
        sp+=title;
        sp+=".lrc";
        
        sl.Download( 0 ,sp.c_str() );
    }
    else
    {
        printf("thread %u: search failed.\n",pthread_self() );
    }
        
    return 0;
}


void* addJobIsFileAudio(const char * file ,void *arg)
{
    char *savepath= "/Users/shijunhe/lyrics";
    
    lrcFchS * lrcFchArg = (lrcFchS*)malloc(sizeof(lrcFchS)) ;
    
    lrcFchArg->savepath = savepath;
    

    if (getId3Info(file , lrcFchArg->artist,  lrcFchArg->title )    )
    {
        //printf("job .. \n");
        pool_add_job(lrcFchThread, (void*)lrcFchArg );
        sleep(12);
    }
    
    return nullptr;
}

int main(int argc, const char * argv[]) {

    pool_init(4);
    
    IterFiles(string ("/Users/shijunhe/Music/Music/abc"), string ("/Users/shijunhe/Music/Music/abc"), addJobIsFileAudio, nullptr );
    
    pool_destroy();
    
    
    while (1) {
        printf("~~~.");
        sleep(2);
    }
    

    
    return 0;
}

