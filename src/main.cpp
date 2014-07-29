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


int runningJobs = 0;
int totalDownload = 0;
int downloadFailed = 0;
int tagFileFinded=0;
int skiped=0;

/**
 * brief "artist" , "title" , "/a/b"  to "/a/b/artist-title.lrc"
 * param lyricsFilename : path to save. without a '/'.
 */
char *GenerateLyricsName(const char *artist , const char *title ,const char *path , char *fullname)
{
    char *tmp ;

    strcpy ( fullname ,path );
    
    if(path[strlen(path)]!='/')
	tmp = strcat(fullname , "/" );
    
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
    bool bRet = false ;

    lrcFchS *s = (lrcFchS*)lrcFchArg ;
    const char *artist = s->artist;
    const char *title = s->title;
    const char *savepath= s->savepath;
    
    printf("thread %u: beginning to search: %s , %s.\n",pthread_self() ,artist , title );
    SearchLyric sl;
    if(  sl.Search(artist,title) )
    {
        char fullname[128] ={ 0 };
        
        bRet = sl.Download(GenerateLyricsName(artist ,title , savepath ,fullname ) );
    }
    else
    {
        printf("thread %u: search failed.\n",pthread_self() );
    }
    
    runningJobs--;
    totalDownload++;

    if(bRet == false )
        downloadFailed++;

    free(s);

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


char *_savepath = 0;
void* addJobIsFileAudio(const char * file ,void *arg)
{
    lrcFchS * lrcFchArg = (lrcFchS*)malloc(sizeof(lrcFchS)) ;
    
    lrcFchArg->savepath = _savepath;
    
    
    if (getId3Info(file , lrcFchArg->artist,  lrcFchArg->title )    )
    {
        //printf("%u , audio's tag info : %s , %s." , pthread_self() ,lrcFchArg->artist , lrcFchArg ->title );
        
        tagFileFinded++;
        
        char fullname[128] = {0 };
        
        GenerateLyricsName( lrcFchArg->artist , lrcFchArg->title , lrcFchArg->savepath , fullname );
        
        //if fullname is exist in file system. ignore to search and download the lyrics.
        if (  FileExists(fullname ) )
        {
            skiped++;
            //printf(" ~~~~~~~Lyrics file exists. Skip to fetch.~~~~~~\n");
        }
        else
        {
            //printf("\n");
            pool_add_job(lrcFchThread, (void*)lrcFchArg );
            runningJobs++;
        }
    }
    
    return nullptr;
}





void usage(const char *exec)
{
    int len = strlen (exec );
    
    char *ext =(char*)exec + len ;
    
    for (; ext[-1] != '/' ; ext -= 1 )
        ;
    
    
    printf("This is a program to download music track's lyrics from internet.\n");
    printf("usage: %s [music folder] [download folder] \n", ext );
}


int main(int argc, const char * argv[])
{
    if (argc != 3 )
    {
        usage( argv[0] );
        return 0;
    }
    
    const char * sourceFolder = argv[1];
    //const char * targetFolder = argv[2];
    _savepath =(char*)argv[2];
    
    pool_init(8);
    
    IterFiles(string (sourceFolder ), string (sourceFolder ), addJobIsFileAudio, nullptr );
    
    
    
    while (runningJobs > 0)
    {
        sleep(1);
    }
    
    
    pool_destroy();
 
    printf("\n ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nSummary:\n");
    printf(": audio source directory: \033[;32m%s\033[0m\n",argv[1]);
    printf(": target lyrics directory: \033[;32m%s\033[0m\n\n",argv[2]);
    printf("\033[;32m%d\033[0m Audio File with tag Finded.\n",tagFileFinded);
    if(skiped>0)
	printf("\033[;32m%d\033[0m Audio File is skipped (there is a lyrics file in the target directory).\n",skiped);

    if(downloadFailed>0)
	printf("\033[;32m%d\033[0m failed to download.\n",downloadFailed);
    printf("\033[;32m%d\033[0m files downloaded.\n",totalDownload-downloadFailed);
    return 0;
}



