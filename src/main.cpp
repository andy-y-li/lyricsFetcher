/**
 * 
 */


#include "lrcFch.h"
#include <pthread.h>
#include "threadpool.h"
#include "fileCtrl.h"

struct lrcFchS
{
    char artist[128];
    char title[128] ;
    const char *savepath;
};

void lrcFchSInit(lrcFchS *s, const char *a , const char *t, const char *p)
{
    strcpy(s->artist, a);
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
        
        sl.Download( 0 ,sp.c_str() );
    }
    
    return 0;
}


void addJobIsFileAudio(const char * file ,void *arg)
{
    if (file /*is audio*/) {
        pool_add_job(lrcFchThread, &lrcFchArg );
    }
}

//argv is utf-8 format code string.
int main(int argc, const char * argv[]) {
    if (argc < 3 ) {
        printf("Input a artist and title to search\n");
        return 0;
    }
   
    char *savepath= "/Users/shijunhe/Download2";
    
    
    
    const char *artist = argv[1];
    const char *title = argv[2];
   
    
    
//    pthread_t t;
//    pthread_attr_t attr;
//    pthread_attr_init(&attr);
    struct lrcFchS lrcFchArg;
    lrcFchSInit( &lrcFchArg , artist , title , savepath );
//    pthread_create(&t, (&attr), lrcFchThread , (void*)&lrcFchArg);
    
    
    
    pool_init(4);
    
    IterFiles(string ("/Users/shijunhe/Music/Music"), string ("/Users/shijunhe/Music/Music"), addJobIsFileAudio, lrcFchArg );
   
    
    pool_destroy();
    
    
    
    
    return 0;
}

