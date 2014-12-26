#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
using namespace std;

bool IsDirectory(char *pszName)
{
    struct stat S_stat;
    
    //取得文件状态
    if (lstat(pszName, &S_stat) < 0)
    {
        return false;
    }
    
    if (S_ISDIR(S_stat.st_mode))
    {
        return true;
    }
    else
    {
        return false;
    }
}

///return -1 if error . 0 for success.
int IterFiles(string srcPath, string destPath , void * (*doSomeThing)(const char *file , void *arg) , void *arg )
{
    int iRet = 0;
    DIR *pDir;
    string destNewPath;
    struct dirent *pDirent;
    pDir = opendir(srcPath.c_str());
    if (pDir == NULL)
    {
        return -1;
    }
    while ((pDirent = readdir(pDir)) != NULL)
    {
        // if ((pDirent->d_name == '.') || pDirent->d_name[0] == '..')
        if (strcmp(pDirent->d_name, ".") == 0 || strcmp(pDirent->d_name, "..") == 0)
        {
            continue;
        }
        
        
        char szTmpPath[1024] = {0};
        
        if (srcPath.back() != '/') {
            srcPath.push_back('/');
        }
        
        sprintf(szTmpPath, "%s%s", srcPath.c_str(), pDirent->d_name);
        destNewPath = destPath  + pDirent->d_name;

        
        if (IsDirectory(szTmpPath))
        {
#ifdef DEBUG
            printf("Dir  : ");
            printf("%s\n",destNewPath.c_str());
#endif
            
            //如果是文件夹则进行递归
            iRet = IterFiles(szTmpPath, destNewPath , doSomeThing ,arg );
            if (iRet < 0)
            {
                break;
            }
        }
        else
        {
            /* do something */
            
            printf("File finded : ");
            printf("%s\n",destNewPath.c_str());

            if(doSomeThing)
                doSomeThing( destNewPath.c_str() , arg);
        }
        

    }
    
    closedir(pDir);
    
    return iRet;
}







char *getHome()
{
    return  getenv("HOME");
}
