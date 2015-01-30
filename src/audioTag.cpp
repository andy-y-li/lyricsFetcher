#include <string.h>
#include <taglib/taglib.h> 
#include <taglib/id3v2tag.h> 
#include <taglib/id3v2frame.h>
#include <taglib/id3v2header.h>
#include <taglib/mpegfile.h>
#include <taglib/apetag.h>

#include <taglib/id3v1tag.h>


using namespace TagLib;

bool getId3Info(const char * filename , char *artist , char * title  ,char *album, char *genre, char *year)
{
	static bool bInit = false;
	if(bInit == false)
	{
		//to do 
		bInit = true;
	}
    
   	MPEG::File f(filename);
    
    //APE::Tag *apeTag=f.APETag();
    
    ID3v2::Tag *id3v2tag = f.ID3v2Tag();
    
    bool validId3v2 = false;
    bool validId3v1 = false;
    
    if(id3v2tag)
    {
        strcpy(artist,(const char*) id3v2tag->artist().toCString(true));
        strcpy(title,id3v2tag->title().toCString(true));
        
        strcpy(album,id3v2tag->album().toCString(true));
        strcpy(genre,id3v2tag->genre().toCString(true));
        
        year[0]='?';
        year[1]='\0';
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
            strcpy(artist,id3v1tag->artist().toCString(true));
            strcpy(title,id3v1tag->title().toCString(true));

            
            strcpy(album,id3v1tag->album().toCString(true));
            strcpy(genre,id3v1tag->genre().toCString(true));

            year[0]='?';
            year[1]='\0';
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

























