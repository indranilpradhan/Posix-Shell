#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<bits/stdc++.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <pwd.h>
#include <fcntl.h>

using namespace std;
char directory[10000];
char *sudo;
int root = 0;
int len = 0;
void getcommand(char command[],char *parameter[],char cmdpara[],char global[])
{
    char * input =new char[1000];
    int count = 0;
    int i = 0;
    int j = 0;
    char *a[1000], *b;

    while(1)
    {
      int p = fgetc(stdin);
      //count=0;
      input[count++] = (char) p;
      if( p == '\n')
        break;
    }
    if(count == 1)
    return;

    b = strtok(input," \n");
    i=0;
    //delete [] input;
    while(b != NULL)
    {
      a[i++] = strdup(b);
      //global[k++] = a[i];
      b = strtok(NULL," \n");
    }

    for(int k=0;k<i;k++)
    {
      strcat(global,a[k]);
      strcat(global," ");
    }
    len = i;
    if(strcmp(a[0],"sudo")==0 || strcmp(a[0],"su")==0)
    {
      sudo = a[0];
      root = 1;
      strcpy(command,a[1]);
//      cout<<"i "<<i<<endl;
      for(int j = 1;j<i; j++)
      {
  //      cout<<"a "<<a[j]<<endl;
        parameter[j-1] = a[j];
      }
  //    cout<<"para "<<sizeof(parameter)<<endl;
      //parameter[j]=NULL;
      for(int j = 2;j<i; j++)
      {
    //   cout<<a[j];
        strcat(cmdpara,a[j]);
        strcat(cmdpara," ");
      }
    }
    else
    {
      strcpy(command,a[0]);

  //  cout<<i;
  //  parameter[0]=a[0];
      for(int j = 0;j<i; j++)
      {
//        cout<<"a "<<a[j]<<endl;
        parameter[j] = a[j];
      }
    //parameter[j]=NULL;
      for(int j = 1;j<i; j++)
      {
  //   cout<<a[j];
        strcat(cmdpara,a[j]);
        strcat(cmdpara," ");
      }
    }
    parameter[i] = NULL;
    cmdpara[strlen(cmdpara)-1] = '\0';
  //  cout<<cmdpara<<endl;
}

int main() {
  //  char command[100];
  //  char cmd[100];
  //  char *parameter[200];
    char reading[10000];
  //  char cmdpara[10000];
    char *home;
    char *key;
    char *value;
    char *b;
    map<string, string> alias;
    getcwd(directory,sizeof(directory));
    char *environment[] = {(char*) "PATH=/bin", 0};
  //  char* prompt = getenv("HOME");
    FILE *fp = fopen(".resourcerc","r");
    struct passwd *pw = getpwuid(getuid());
    while(fgets(reading, sizeof(reading),fp))
    {
      key = strtok(reading,"=");
      value = strtok(NULL,"=");
      setenv(key,value,1);
    }
    string ps1 = "";
    while(1)
    {
      char command[100];
      char cmd[100];
      char *parameter[200];
      char cmdpara[10000];
      char *ech;
      char *arg;
      char *rcmd;
      char *pcmd;
      char *parg;
      int status;
      char global[1000];
      if(root==0)
      {
        ps1 = (string)getenv("PS1");
        ps1 = ps1 + directory;
        cout<<ps1<<">>";
      }
      else
      {
    //    cout<<"here"<<endl;
        ps1 = "@root";
        cout<<ps1<<">>";
      }
      getcommand(command,parameter,cmdpara,global);
      if(strstr(cmdpara,"|"))
      {
  //      cout<<"here"<<endl;
        char *argument[1000];
        char *end;
        char *endfile;
    //     cout<<"global "<<global<<endl;
        if(strstr(cmdpara,">"))
        {
          int filed[2];
          char *temparg[1000];
          int nextfd =0;
          pcmd = strtok(global,"|");
          argument[0]=pcmd;
          int i =1;
          while(pcmd != NULL)
          {
            pcmd = strtok(NULL,"|");
            if(pcmd == NULL)
              break;
            argument[i++] = pcmd;
          }
          end=argument[i-1];
          endfile = strtok(end,">");
          argument[i-1] = endfile;
          endfile = strtok(NULL,">");
          argument[i]=NULL;
          int j = 0;

    //      while(argument[j] != NULL)
      //    {
        //    cout<<"arg "<<argument[j]<<endl;
          //  j++;
          //}
          int fd = open(endfile,O_CREAT|O_WRONLY|O_TRUNC,777);
          int k =0;
          while(k < i)
          {
    //        cout<<"arg "<<argument[k]<<endl;
            char* temp = strtok(argument[k]," ");
            temparg[0]=temp;
            int j =1;
            while(temp != NULL)
            {
              temp = strtok(NULL," ");
              if(temp == NULL)
                break;
              temparg[j++] = temp;
            }
            temparg[j]=NULL;
      //      execvp(*temparg,temparg);
            pipe(filed);
            pid_t pid = fork();
            if(pid == 0)
            {
        //      cout<<"child"<<endl;
    //          cout<<"tempo "<<*temparg<<endl;
              dup2(nextfd,0);
              if (k+1 != i) {
                dup2(filed[1],1);
              }
              close(filed[0]);
              close(filed[1]);
              if(k+1 == i)
              {
              dup2(fd,1);
          //    fd = open(endfile,O_CREAT|O_WRONLY|O_TRUNC,777);
              //close(fd);
              //cout<<*temparg<<endl;
              close(fd);
             }
              execvp(*temparg,temparg);
            }
            else
            {
        //      cout<<"here"<<endl;
              wait(NULL);
              close(filed[1]);
              //close(fd);
              nextfd = filed[0];
              k++;
            }
            //k++;
          }
        }
        else
        {
            int filed[2];
            pcmd = strtok(global,"|");
            argument[0]=pcmd;
            char *temparg[1000];
            int nextfd =0;
            int i =1;
            while(pcmd != NULL)
            {
              pcmd = strtok(NULL,"|");
              if(pcmd == NULL)
                break;
              argument[i++] = pcmd;
            }
            argument[i]=NULL;
          //  int j = 0;
          //  while(argument[j] != NULL)
          //  {
          //  cout<<"arg "<<argument[j]<<endl;
        //      j++;
        //    }
        //    cout<<"out "<<argument[0]<<endl;
          //  cout<<"add "<<*argument<<endl;
          //  cout<<"argu "<<argument<<endl;
            //execvp(argument[0],argument);
            int k =0;
            while(k < i)
            {
      //        cout<<"arg "<<argument[k]<<endl;
              char* temp = strtok(argument[k]," ");
              temparg[0]=temp;
              int j =1;
              while(temp != NULL)
              {
                temp = strtok(NULL," ");
                if(temp == NULL)
                  break;
                temparg[j++] = temp;
              }
              temparg[j]=NULL;
        //      execvp(*temparg,temparg);
              pipe(filed);
              pid_t pid = fork();
              if(pid == 0)
              {
          //      cout<<"child"<<endl;
      //          cout<<"tempo "<<*temparg<<endl;
                dup2(nextfd,0);
                if (k+1 != i) {
                  dup2(filed[1],1);
                }
                close(filed[0]);
                close(filed[1]);

                execvp(*temparg,temparg);
              }
              else
              {
          //      cout<<"here"<<endl;
                wait(NULL);
                close(filed[1]);
                nextfd = filed[0];
              }
              k++;
            }
        }
        memset(argument, 0, sizeof(argument));
      }
      else if(fork() != 0)
        wait(NULL);
      else
      {

        if((strstr(cmdpara,">")||strstr(cmdpara,">>")) && !strstr(cmdpara,"|"))
        {
          char *argument[1000];
    //      cout<<"global"<<global<<endl;
          if(strstr(cmdpara,">"))
          {
            rcmd = strtok(global,">");
      //      cout<<"rcmd "<<rcmd<<endl;
            arg = strtok(NULL,">");
            char* temp = strtok(rcmd," ");
            argument[0]=temp;
            int i =1;
            while(temp != NULL)
            {
              temp = strtok(NULL," ");
              if(temp == NULL)
                break;
              argument[i++] = temp;
            }
            argument[i]=NULL;
            int fd = open(arg,O_CREAT|O_WRONLY,777);
            int p = dup2(fd,1);
        //    cout<<*argument<<endl;
        //    cout<<argument<<endl;
            execvp(*argument,argument);
            close(fd);
          }
          if(strstr(cmdpara,">>"))
          {
            rcmd = strtok(global,">>");
      //      cout<<"rcmd "<<rcmd<<endl;
            arg = strtok(NULL,">>");
            char* temp = strtok(rcmd," ");
            argument[0]=temp;
            int i =1;
            while(temp != NULL)
            {
              temp = strtok(NULL," ");
              if(temp == NULL)
                break;
              argument[i++] = temp;
            }
            argument[i]=NULL;
            int fd = open(arg,O_CREAT|O_APPEND,777);
            int p = dup2(fd,1);
    //        cout<<*argument<<endl;
      //      cout<<argument<<endl;
            execvp(*argument,argument);
            close(fd);
          }
        }
        else
        {
        if(strcmp(command,"echo") ==0)
        {
          if(strstr(cmdpara,"$"))
          {
            if(strcmp(cmdpara,"$$") == 0)
            {
              cout<<(long)getpid()<<endl;
            }
            else if(strcmp(cmdpara,"$?") == 0)
            {
              cout<<status<<endl;
            }
            else
            {
              ech=strtok(cmdpara,"$");
              cout<<getenv(ech)<<endl;
            }
          }
          else
          {
            strcpy(cmd, "/bin/");
            strcat(cmd, command);
            execve(cmd,parameter,environment);
          }
        }
        else if(strcmp(command,"alias") == 0)
        {
          char *key;
          char *value;
          cout<<"command "<<command<<endl;
          cout<<"cmdpara "<<cmdpara<<endl;
          char *temp = strtok(cmdpara,"=");
          key = temp;
          temp = (char*)(NULL,"\"");
          cout<<temp<<endl;
          cout<<key<<endl;
        }
        else if(strcmp(command,"cd") == 0)
        {
          if(strlen(cmdpara) == 0)
          {
              chdir("/");
              getcwd(directory,sizeof(directory));
          }
          else if(strcmp(cmdpara,"~")==0)
          {
            home = pw->pw_dir;
            chdir(home);
            getcwd(directory,sizeof(directory));
          }
          else
          {
            chdir(cmdpara);
            getcwd(directory,sizeof(directory));
          }
        }
        else
        {
        //    cout<<"comdpara "<<cmdpara<<endl;
      //      cout<<"command "<<command<<endl;
            strcpy(cmd, "/bin/");
            strcat(cmd, command);
            execve(cmd,parameter,environment);
        }
        }
        int exitstatus;
        waitpid((long)getpid(),&exitstatus,0);
        status = WEXITSTATUS(exitstatus);
      }
      if(strcmp(command,"exit")==0)
      {
        if(root==0)
        {
          break;
        }
        else
        {
          root =0;
        }
      }
      memset(cmdpara, 0, sizeof(cmdpara));
      memset(command, 0, sizeof(command));
      memset(parameter, 0, sizeof(parameter));
      memset(cmd, 0, sizeof(cmd));
      memset(global,0, sizeof(global));
      fflush(stdin);
      fflush(stdout);
    }
    return 0;
  }
