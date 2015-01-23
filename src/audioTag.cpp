#include <string.h>
#include <taglib/taglib.h> 
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/id3v2header.h>
#include <taglib/mpegfile.h>
#include <taglib/apetag.h>

#include <taglib/id3v1tag.h>


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

























