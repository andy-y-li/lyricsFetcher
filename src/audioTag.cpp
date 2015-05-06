#include <string.h>
#include <taglib/taglib.h> 
#include <taglib/id3v2tag.h> 
#include <taglib/id3v2frame.h>
#include <taglib/id3v2header.h>
#include <taglib/mpegfile.h>
#include <taglib/apetag.h>
#include <taglib/id3v1tag.h>

#include "stringConv.h"

using namespace TagLib;


bool getId3Info(const char * filename , char *artist , char * title  )
{
    static bool bInit = false;
    if(bInit == false)
    {
        //to do
        bInit = true;
    }
    
    
    
   	MPEG::File f(filename);
    
    APE::Tag *apeTag=f.APETag();
    
    
    ID3v2::Tag *id3v2tag = f.ID3v2Tag();
    bool bInvalidID3V2= false;
    if(id3v2tag)
    {
        strcpy(artist,(const char*) id3v2tag->artist().toCString(true));
        strcpy(title,id3v2tag->title().toCString(true));
        
        
        if ( strlen(title) == 0  || strlen(artist)==0 )
        {
            bInvalidID3V2=true;
        }
        else
        {
        }
    }
    else
    {
        bInvalidID3V2= true;
    }
    
    
    bool scanId3v1  = false ;
    
    if ( bInvalidID3V2 )
        scanId3v1 = true;
    
    
    ID3v1::Tag *id3v1tag=NULL;
    if( scanId3v1 )
    {
        id3v1tag = f.ID3v1Tag();
        if(id3v1tag)
        {
            strcpy(artist,id3v1tag->artist().toCString(true));
            strcpy(title,id3v1tag->title().toCString(true));
        }
    }
    
    
    if(id3v2tag || id3v1tag)
    {
        //if the title is empty ,we will use the filename
        //without suffix
        if (strlen(title))
        {
        }
    }
    
    
    return (id3v2tag || id3v1tag) && (strlen(artist) > 1 || strlen(title) >1 ) ;
}


bool getId3Info(const char * filename , char *artist , char * title  ,char *album, char *genre, char *year)
{
   	MPEG::File f(filename);
    
    ID3v2::Tag *id3v2tag = f.ID3v2Tag();
    
    bool validId3v2 = false;
    bool validId3v1 = false;
    
    
    
    artist[0]=' ';
    album[0]=' ';
    title[0]=' ';
    genre[0]=' ';
    year[0]=' ';
    
    if(id3v2tag)
    {
        strcpy(artist,id3v2tag->artist().toCString(false));
        strcpy(title,id3v2tag->title().toCString(false));
        
        strcpy(album,id3v2tag->album().toCString(false));
        strcpy(genre,id3v2tag->genre().toCString(false));
        
        // year[0]='\0';
        uint uYear=id3v2tag->year();
        if(uYear!=0)
            sprintf(year, "%u" ,uYear);
        
        if ( strlen(title) > 0  || strlen(artist) > 0 )
            validId3v2 = true;
            
    }



    ID3v1::Tag *id3v1tag=NULL;
    if( ! validId3v2 )
    {
        id3v1tag = f.ID3v1Tag();
        if(id3v1tag)
        {
            strcpy(artist,id3v1tag->artist().toCString(false));
            strcpy(title,id3v1tag->title().toCString(false));

            
            strcpy(album,id3v1tag->album().toCString(false));
            strcpy(genre,id3v1tag->genre().toCString(false));

//            year[0]='\0';
            uint uYear=id3v1tag->year();
            if(uYear!=0)
                sprintf(year, "%d" ,uYear);
            
            if ( strlen(title) > 0  || strlen(artist) > 0 )
                validId3v1 = true;
        }
    }
    
    
    if(validId3v2  || validId3v1)
    {
        //if the title is empty ,we will use the filename
        //without suffix
        if (!strlen(title))
        {
            std::string url(filename);
#ifdef _WINDOW
            int a=url.find_last_of('\\');
#else
            int a=(int)url.find_last_of('/');
#endif
            int b=(int)url.find_last_of('.');
            if (a<b && b!=url.npos)
            {
                std::string title2=url.substr(a+1,b-(a+1));
                strcpy(title, title2.c_str());
            }
            
            
        }
    }
    
    
    return (validId3v2  || validId3v1) && (strlen(artist) > 1 || strlen(title) >1 ) ;
}
