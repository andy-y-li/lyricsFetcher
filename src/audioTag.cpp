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
            strcpy(artist,id3v2tag->artist().toCString());
            strcpy(title,id3v2tag->title().toCString());
            
            
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
            strcpy(artist,id3v1tag->artist().toCString());
            strcpy(title,id3v1tag->title().toCString());
            
            //the id3v1tag string may be wrongly
            //like Œ“∫‹∫√0000fsadfsdf...
           // title=id3v1tag->title().toWString();
           // artist=id3v1tag->artist().toWString();
           // album=id3v1tag->album().toWString();
           // genre=id3v1tag->genre().toWString();
            
           // TCHAR cYear[MAX_PATH]={_T("?")};
//            uYear=id3v1tag->year();
//            if(uYear!=0)
//                _itow(id3v1tag->year(),cYear,10);
//            year=cYear;
//
            
            //to fix
//            TrimRightByNull(title);
//            TrimRightByNull(artist);
//            TrimRightByNull(album);
//            TrimRightByNull(genre);
        }
    }
    
    
    if(id3v2tag || id3v1tag)
    {
        //if the title is empty ,we will use the filename
        //without suffix
        if (strlen(title))
        {
//            int a=url.find_last_of('\\');
//            int b=url.find_last_of('.');
//            if (a<b && b!=url.npos)
//                title=url.substr(a+1,b-(a+1));
        }
    }
    
    
    return id3v2tag || id3v1tag;
}

























