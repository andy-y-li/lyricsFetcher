
#import "audioTag2.h"

#include <string.h>
#include <taglib/taglib.h> 
#include <taglib/id3v2tag.h> 
#include <taglib/id3v2frame.h>
#include <taglib/id3v2header.h>
#include <taglib/mpegfile.h>
#include <taglib/apetag.h>
#include <taglib/id3v1tag.h>
#include <taglib/wavfile.h>
#include <taglib/infotag.h>

#include "stringConv.h"


using namespace TagLib;

/*
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
*/


/*
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
 */

void gdkStringTaglib2NS(NSMutableString *ns, TagLib::String from)
{
    if (!from.isEmpty())
    {
        BOOL isLatin = from.isLatin1();
        
        char temp[256];
        strcpy(temp,from.toCString(false));
        
        NSStringEncoding gbkEncoding = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000);
        if(gbkEncoding != kCFStringEncodingInvalidId)
        {
            NSString *a = [[NSString alloc] initWithBytes:temp length:strlen(temp) encoding:gbkEncoding];
            if(a)
                [ns setString:a];
        }
    }
}


/// Using utf8 if it's not gbk encoding.
void stringTaglib2NS(NSMutableString *ns, TagLib::String from)
{
    std::cout<<from<<std::endl;
    
    if(!from.isEmpty())
    {
        BOOL isLatin = from.isLatin1();
        
        char temp[256];
        if ( false && !from.isAscii() && !from.isLatin1())
        {
            strcpy(temp,from.toCString(false));
            int tempLen = strlen(temp);
            
            if (tempLen == from.length())
            {
                NSStringEncoding gbkEncoding = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000);
                if(gbkEncoding != kCFStringEncodingInvalidId)
                {
                    NSString *a = [[NSString alloc] initWithBytes:temp length:tempLen encoding:gbkEncoding];
                    if(a && a.length < tempLen)
                    {
                        [ns setString:a];
                        return;
                    }
                }
            }
        }
        
        // default utf8
        strcpy(temp,from.toCString(true));
        [ns setString:[NSString stringWithUTF8String:temp]];
    }
    
}

class MyLatin1StringHandler : public TagLib::ID3v2::Latin1StringHandler
{
public:
    virtual ~MyLatin1StringHandler()
    {
        
    }
    
    /*!
     * Decode a string from \a data.  The default implementation assumes that
     * \a data is an ISO-8859-1 (Latin1) character array.
     */
    virtual String parse(const ByteVector &data) const
    {
        NSStringEncoding gbkEncoding = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000);
        NSString *temp = [[NSString alloc] initWithBytes:data.data() length:data.size() encoding:gbkEncoding];
        
        if(temp)
            return String (temp.UTF8String,String::UTF8);
        
        return String::null;
    }
    
};


/**Since A valide audio file may have no tag info,so this will always @return true.
 */
bool getID3Info(ID3v2::Tag *id3v2tag, TagLib::Tag *other_tag ,const char * filename , NSMutableString *artist , NSMutableString * title  ,NSMutableString *album, NSMutableString *genre, NSMutableString *year)
{
    bool validId3v2 = false;
    bool validId3v1 = false;
    
    
    if(id3v2tag && !id3v2tag->isEmpty() )
    {
        stringTaglib2NS(artist, id3v2tag->artist());
        stringTaglib2NS(title, id3v2tag->title());
        stringTaglib2NS(album, id3v2tag->album());
        stringTaglib2NS(genre, id3v2tag->genre());
        
        char _year[256];
        _year[0]='\0';
        uint uYear=id3v2tag->year();
        if(uYear!=0)
        {
            sprintf(_year, "%u" ,uYear);
            [year setString:[NSString stringWithUTF8String:_year]];
        }
        
        if ( title.length > 0  || artist.length > 0 )
            validId3v2 = true;
    }
    
    
    
    
    if( ! validId3v2 )
    {
        if(other_tag && !other_tag->isEmpty() )
        {
            /// ID3v1 is assumed to be Latin1 and Ogg Vorbis comments use UTF8.
            gdkStringTaglib2NS(artist, other_tag->artist());
            gdkStringTaglib2NS(title, other_tag->title());
            gdkStringTaglib2NS(album, other_tag->album());
            gdkStringTaglib2NS(genre, other_tag->genre());
            
            
            
            
            char _year[256];
            _year[0]='\0';
            uint uYear=other_tag->year();
            if(uYear!=0)
            {
                sprintf(_year, "%d" ,uYear);
                [year setString:[NSString stringWithUTF8String:_year]];
            }
            
            
            if ( title.length > 0  || artist.length > 0 )
                validId3v1 = true;
        }
    }
    
    
    
    //if the title is empty ,we will use the filename
    //without suffix
    if (title.length==0)
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
            [title setString:[NSString stringWithUTF8String:title2.c_str()]];
        }
        
        
    }
    
    
    return true;
}


/// Reg gbk ,mpge only
bool getID3Info(const char * filename , NSMutableString *artist , NSMutableString * title  ,NSMutableString *album, NSMutableString *genre, NSMutableString *year)
{
    if (strstr(filename, ".DS_Store") )
        return false;
    
    ID3v2::Tag *id3v2tag;
    ID3v1::Tag *id3v1tag;
    
    
    NSString *lowser = [NSString stringWithUTF8String:filename].lowercaseString;
    if( [lowser.pathExtension isEqualToString:@"wav"] )
    {
        RIFF::WAV::File file(filename);
        
        if (file.length() < 10)
        {
            return false;
        }
        
        id3v2tag = file.ID3v2Tag();
        RIFF::Info::Tag *info_tag = file.InfoTag();


        
        return getID3Info(id3v2tag, info_tag , filename, artist, title, album, genre, year);
    }
    else if( [lowser.pathExtension isEqualToString:@"mp3"] )
    {
        static MyLatin1StringHandler *handler = new MyLatin1StringHandler;
        if (handler) {
            ID3v2::Tag::setLatin1StringHandler(handler);
            handler = nullptr;
        }
        
        
        MPEG::File file(filename);
        if (file.length() < 10)
        {
            return false;
        }
        
        id3v2tag = file.ID3v2Tag();
        id3v1tag = file.ID3v1Tag();
        

        
        return getID3Info(id3v2tag, id3v1tag, filename, artist, title, album, genre, year);
    }
    else
    {
        return false;
    }
    
}
