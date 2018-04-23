
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



 int count=0;
 int  cur_level=-1;
char data[100]={0};
 int testbit=0;


void clglobal(void)
{
	count = 0;
	cur_level=-1;
	int i=0;
	testbit=0;
  while(i<100)
 data[i++]=0;


 
	
}



void clear_pathname(char *name)
{

 int i=0;
  while(i<256)
 name[i++]=0;


}
int getestbit(void)
{
	return testbit;
	
}

int Is_not(const char *name,char *dont_del,int num,int flag)
{
  int i=0;
  if(flag==1)
  return 1;
  
   for(i=0;i<num;i++)
  {
   
   printf("dont_del=%s  name=%s\n",dont_del,name);
   if(strcmp(name,dont_del)==0)
    return 0;
  
  }
  return 1;

}
void setdata(int t)
{
  int i=0;
  while(i<=t)
  data[i++]=1;

}


int rm_dir(const char* dir_full_path,char *dont_del,int num,int flag)
{    char sub_path[256]={0};
     char f=0;
     cur_level++;
     DIR* dirp = opendir(dir_full_path);    
    if(!dirp)
    {
        return -1;
    }
    struct dirent *dir;
    struct stat st;
    while((dir = readdir(dirp)) != NULL)
    {
        if(strcmp(dir->d_name,".") == 0
                || strcmp(dir->d_name,"..") == 0)//忽略这两种路径
        {  
          clear_pathname(sub_path);
            continue;
        }   
      
     
       strncpy(sub_path,dir_full_path,strlen(dir_full_path));//*
       strncpy(sub_path+strlen(dir_full_path),"/",1);//*
       strncpy(sub_path+strlen(dir_full_path)+1,dir->d_name,strlen(dir->d_name));//*

        if(lstat(sub_path,&st) == -1)
        {   
           
           clear_pathname(sub_path);
            continue;
        }   
       if(Is_not(sub_path,dont_del,num,flag)==0) 
       {
           setdata(cur_level);
            clear_pathname(sub_path);
            continue;
       }
            
        if(S_ISDIR(st.st_mode))
        {
            if(rm_dir(sub_path,dont_del,num,flag) == -1) // 如果是目录文件，递归删除sub_path下的文件
            {  
                 cur_level--;
               // printf("level=%d",count);
                closedir(dirp);
                return -1;
            }
            cur_level--;
          // if(data[cur_level]==0&&rmdir(sub_path))
          /* if(data[cur_level]==0&&rmdir(sub_path) == -1)//delete dir itself.
        {
        
            return -1;
       }
           if(data[cur_level]==1)
        {
           data[cur_level]=0;
        }*/

   
           // rmdir(sub_path);
      }
        else if(S_ISREG(st.st_mode))
        {   
           if( unlink(sub_path)!=0)
             {  
               return -1;
             }

        }
        else
        {
            clear_pathname(sub_path);
            continue;
        }
       clear_pathname(sub_path);
    }//end while
    
    if(data[cur_level]==0&&rmdir(dir_full_path) == -1)
    {
        closedir(dirp);
        return -1;
    }
    if(data[cur_level]==1)
    {
       data[cur_level]=0;
    }
    closedir(dirp);
	clear_pathname(sub_path);
    return 0;
}



int rm(const char *file_name,char *dont_del,int num,int flag)
{
    char const  * file_path = file_name;
    struct stat st;    
    if(lstat(file_path,&st) == -1)
    {
        return -1;
    }
    if(S_ISREG(st.st_mode))
    {
        if(unlink(file_path) == -1) //判断传入的是否是文件夹
        {
            return -1;
        }    
    }
    else if(S_ISDIR(st.st_mode))
    {
        if(strcmp(file_name,".") ==0  || strcmp(file_name,"..") ==0)//绝对路径
        {
            return -1;
        }    
        if(rm_dir(file_path,dont_del,num,flag) == -1)//delete all the files in dir.
        {
            return -1;
        }
    }
	testbit=1;
    return 0;
}



int main(int argc,char**argv )
{

 if(argc!=3)
 {
  printf("arg erro");
  exit(1);
 }
 if(rm(argv[1],argv[2],1,0)==-1) //flag=1 整个目录 flag=0 指定过滤的目录
 {
   printf("del %s fail",argv[1]);
   exit(1);
 }
else
printf("count num= %d\n",count);
return 0;

}








