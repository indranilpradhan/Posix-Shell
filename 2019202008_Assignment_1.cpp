#include<stdio.h>
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
#include <algorithm>

using namespace std;
char directory[10000];
char *sudo;
int root = 0;
int len = 0;
int flag = 0;
map<string, string> alias;
map<string, string> echovalue;

void getcommand(char command[],char *parameter[],char cmdpara[],char global[])
{
    char * input =new char[1000];
    int count = 0;
    int i = 0;
    int j = 0;
    char *a[1000], *b, *newa[1000];
    while(1)
    {
      int p = fgetc(stdin);
      input[count++] = (char) p;
      if( p == '\n')
        break;
    }
    input[count] = '\0';
    if(count == 1)
    return;

    b = strtok(input," \n");
    i=0;
    while(b != NULL)
    {
      a[i++] = strdup(b);
      b = strtok(NULL," \n");
    }
    a[i]=NULL;
    len = i;
    if(strcmp(a[0],"sudo")==0 || strcmp(a[0],"su")==0)
    {
      sudo = a[0];
      root = 1;
      strcpy(command,a[1]);
      for(int j = 1;j<i; j++)
      {
        parameter[j-1] = a[j];
      }
      for(int j = 2;j<i; j++)
      {
        strcat(cmdpara,a[j]);
        strcat(cmdpara," ");
      }
      for(int k=0;k<i;k++)
      {
        strcat(global,a[k]);
        strcat(global," ");
      }
      cmdpara[strlen(cmdpara)-1] = '\0';
    }
    else
    {
      map<string , string> :: iterator it;
      string str(a[0]);
      it = alias.find(str);
      if(it != alias.end())
      {
        char temp[1000];
        strcpy(temp,(it->second).c_str());
        char* t = strtok(temp," ");
        newa[0]=t;
        int l =1;
        while(t != NULL)

        {
          t = strtok(NULL," ");
          newa[l++] = t;
        }
        --l;
        for(int n =1;n<i;n++)
        {
          newa[l-1+n] = a[n];
        }
        len = l-1+i;
        for(int k=0;k<len;k++)
        {
          strcat(global,newa[k]);
          strcat(global," ");
        }
  //      cout<<"global "<<global<<endl;
        strcpy(command,newa[0]);
  //      cout<<"command 122 "<<command<<endl;
        for(int j = 0;j<len; j++)
        {
    //      cout<<"newa "<<a[j]<<endl;
          parameter[j] = newa[j];
        }
      //parameter[j]=NULL;
        for(int j = 1;j<len; j++)
        {
    //   cout<<a[j];
          strcat(cmdpara,newa[j]);
          strcat(cmdpara," ");
        }
        parameter[len] = NULL;
        cmdpara[strlen(cmdpara)-1] = '\0';
    //    cout<<"cmdpara "<<cmdpara<<endl;
      }
      else
      {
    //    cout<<"here"<<endl;
        strcpy(command,a[0]);
  //      cout<<"command 104 "<<a[0]<<endl;
      //  cout<<"command 105 "<<a[1]<<endl;
  //  cout<<i;
  //  parameter[0]=a[0];
      //cout<<"here"<<endl;
        for(int j = 0;j<i; j++)
        {
  //        cout<<"a->"<<a[j]<<endl;
          parameter[j] = a[j];
        }
    //parameter[j]=NULL;
        for(int j = 1;j<i; j++)
        {
   //   cout<<a[j];
          strcat(cmdpara,a[j]);
          strcat(cmdpara," ");
        }

        for(int k=0;k<i;k++)
        {
          strcat(global,a[k]);
          strcat(global," ");
        }
  //      cout<<"global "<<global<<endl;
        parameter[i] = NULL;
        cmdpara[strlen(cmdpara)-1] = '\0';
//        cout<<"cmdpara "<<cmdpara<<endl;
      }
    }
  /*  cout<<"command 174 "<<command<<endl;
    cout<<"cmdpara 175 "<<cmdpara<<endl;
    cout<<"length cmdpara "<<strlen(cmdpara)<<endl;
    cout<<"global "<<global<<endl; */
}

int main() {
  //  char command[100];
  //  char cmd[100];
  //  char *parameter[200];
    char reading[10000];
  //  char cmdpara[10000];
    char * home;
    char * key;
    char * value;
    char * b;
    getcwd(directory,sizeof(directory));
    char *environment[] = {(char*) "PATH=/bin", 0};
  //  char* prompt = getenv("HOME");
    FILE *fp3 = fopen(".resourcerc","r");
    struct passwd *pw = getpwuid(getuid());
    while(fgets(reading, sizeof(reading),fp3))
    {
      key = strtok(reading,"=");
      value = strtok(NULL,"=");
  //    cout<<"key "<<key<<endl;
    //  cout<<"value "<<value<<endl;
      setenv(key,value,1);
    }
  //  fflush(fp3);
    fclose(fp3);
    string ps1 = "";

    while(1)
    {
      fflush(stdin);
      fflush(stdout);
      char command[100];
      char cmd[100];
      char *parameter[200];
      char cmdpara[10000];
      char *ech;
      char *arg;
      char *cpyarg;
      char *rcmd;
      char *pcmd;
      char *parg;
      char *cpycmdpara;
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
    /*  FILE *fp4 = fopen(".resourcerc","r");
      while(fgets(reading, sizeof(reading),fp4))
      {
        key = strtok(reading,"=");
        value = strtok(NULL,"=");
        if(strcmp(key,"PS1") != 0 || strcmp(key,"HOME") != 0 || strcmp(key,"USER") != 0 || strcmp(key,"HOSTNAME") != 0 || strcmp(key,"PATH") != 0)
          setenv(key,value,1);
      }
    //  fflush(fp4);
      fclose(fp4); */
    //  freopen("log.txt", "w", stdout);
  //  cout<<"command 241 "<<command<<endl;
  //  cout<<"cmdpara 242 "<<cmdpara<<endl;
  //  cout<<"length cmdpara "<<strlen(cmdpara)<<endl;
    if(strstr(command,"record"))
    {
      if(strstr(cmdpara,"start"))
        flag = 1;
    }
    if(flag == 1)
    {
      char buf[1000];
      FILE *fp1 = fopen("log.txt","a+");
      fprintf (fp1,"%s\n",global);
  //    fflush(fp1);
      fclose(fp1);
  //    ofstream outFile("myfile.out", ios::out);
    //  outfile<<stdout;
  //  cout<<stdout<<endl;
      if(strstr(command,"record"))
      {
        if(strstr(cmdpara,"stop"))
          flag = 0 ;
      }
    }
      if(strstr(cmdpara,"|"))
      {
  //      cout<<"here"<<endl;
        char *argument[1000];
        char *end;
        char *endfile;
    //     cout<<"global "<<global<<endl;
        if(strstr(cmdpara,">>"))
        {
          //  cout<<"here"<<endl;
            int filed[2];
            char *temparg[1000];
            int nextfd =0;
            pcmd = strtok(global,"|");
            argument[0]=pcmd;
            int i =1;
            while(pcmd != NULL)
            {
              pcmd = strtok(NULL,"|");
              argument[i++] = pcmd;
            }
            --i;
            end=argument[i-1];
            endfile = strtok(end,">>");
            argument[i-1] = endfile;
            endfile = strtok(NULL,">>");
            argument[i]=NULL;
            int j = 0;
            int s = 0;
      //      cout<<"here1"<<endl;
            int r =0;
            while(endfile[r])
            {
              if(endfile[r] != ' ')
                endfile[s++] = endfile[r];
              r++;
            }
            endfile[s]='\0';
      //      while(argument[j] != NULL)
        //    {
          //    cout<<"arg "<<argument[j]<<endl;
            //  j++;
            //}
            int fd = open(endfile,O_APPEND|O_CREAT|O_WRONLY,777);
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
                  //cout<<k<<endl;
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
                close(fd);
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
            close(fd);
          //  close(1);
        }
        else if(strstr(cmdpara,">"))
        {
        //  cout<<"here"<<endl;
          int filed[2];
          char *temparg[1000];
          int nextfd =0;
          pcmd = strtok(global,"|");
          argument[0]=pcmd;
          int i =1;
          while(pcmd != NULL)
          {
            pcmd = strtok(NULL,"|");
            argument[i++] = pcmd;
          }
          --i;
          end=argument[i-1];
          endfile = strtok(end,">");
          argument[i-1] = endfile;
          endfile = strtok(NULL,">");
          argument[i]=NULL;
          int j = 0;
          int s = 0;
    //      cout<<"here1"<<endl;
          int r =0;
          while(endfile[r])
          {
            if(endfile[r] != ' ')
              endfile[s++] = endfile[r];
            r++;
          }
          endfile[s]='\0';
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
              close(fd);
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
          close(fd);
        //  close(1);
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
              argument[i++] = pcmd;
            }
            argument[--i]=NULL;
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
                //cout<<"k "<<k<<endl;
                close(filed[0]);
                close(filed[1]);
                //cout<<"temparg"<<*temparg<<endl;
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
      //  cout<<"command 511 "<<command<<endl;
      //  cout<<"cmdpara 512 "<<cmdpara<<endl;
        if(strstr(command,"record"))
        {
          int exitstatus;
          waitpid((long)getpid(),&exitstatus,0);
          status = WEXITSTATUS(exitstatus);
          continue;
        }
        if((strstr(cmdpara,">")||strstr(cmdpara,">>")) && !strstr(cmdpara,"|"))
        {
          char *argument[1000];
    //      cout<<"global"<<global<<endl;
          if(strstr(cmdpara,">"))
          {
            rcmd = strtok(global,">");
      //      cout<<"rcmd "<<rcmd<<endl;
            arg = strtok(NULL,">");
            int s = 0;
      //      cout<<"here1"<<endl;
            int r =0;
            while(arg[r])
            {
              if(arg[r] != ' ')
                arg[s++] = arg[r];
              r++;
            }
          //  cout<<"here2"<<endl;
            arg[s]='\0';
        //    cout<<"cpyarg "<<arg<<endl;
            char* temp = strtok(rcmd," ");
            argument[0]=temp;
            int i =1;
            while(temp != NULL)
            {
              temp = strtok(NULL," ");
              argument[i++] = temp;
            }
            argument[i]=NULL;
            int fd = open(arg,O_CREAT|O_WRONLY|O_APPEND,777);
          //  FILE *fp = fopen(arg,"w");
          //  int fd = fileno(fp);
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
            int s = 0;
      //      cout<<"here1"<<endl;
            int r =0;
            while(arg[r])
            {
              if(arg[r] != ' ')
                arg[s++] = arg[r];
              r++;
            }
          //  cout<<"here2"<<endl;
            arg[s]='\0';
            char* temp = strtok(rcmd," ");
            argument[0]=temp;
            int i =1;
            while(temp != NULL)
            {
              temp = strtok(NULL," ");
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
        //  cout<<"here"<<endl;
        if(strcmp(command,"echo") ==0)
        {
          if(strcmp(cmdpara,"$$") == 0)
          {
            //cout<<"here"<<endl;
            cout<<(long)getpid()<<endl;
          }
          else
          {
          map<string, string>::iterator it;
          ech=strtok(cmdpara,"$");
          ech=strtok(ech," ");
          //cout<<"ech "<<strlen(ech)<<endl;
          it = echovalue.find(ech);
          if(it != echovalue.end())
          {
            cout<<it->second<<endl;
          }
          else
          {
            if(strstr(cmdpara,"$"))
            {
        //      cout<<"cmdpara "<<cmdpara<<endl;
               if(strcmp(cmdpara,"$?") == 0)
              {
                cout<<status<<endl;
              }
              else
              {
          //      ech=strtok(cmdpara,"$");
            //    ech=strtok(ech," ");
                cout<<getenv(ech)<<endl;
              }
            }
            else
            {
            //cout<<cmdpara<<endl;
              strcpy(cmd, "/bin/");
              strcat(cmd, command);
              execve(cmd,parameter,environment);
            }
          }
          }
        }
        else if(strcmp(command,"alias") == 0)
        {
    //      cout<<"here"<<endl;
      //    cout<<"alias "<<command<<endl;
          if(strlen(cmdpara) == 0 || strcmp(cmdpara,"-p") == 0)
          {
            for(auto it = alias.begin();it != alias.end(); it++)
            {
              cout<<"alias "<<it->first<<"="<<"'"<<it->second<<"'"<<endl;
            }
          }
          else
          {
          char *key;
          char value[1000];
      //    cout<<"command "<<command<<endl;
        //  cout<<"cmdpara "<<cmdpara<<endl;
          char *temp = strtok(cmdpara,"=");
          int m = 0;
          for (int n = 0; temp[n]; n++)
          {
            if (temp[n] != ' ')
              temp[m++] = temp[n];
          }
          key = temp;
          temp = strtok(NULL,"=");

        //  cout<<temp[0]<<" and "<<temp[sizeof(temp)-1]<<endl;
          int k =0;
          int l =0;
          while(temp[k]!='\0')
          {
            if(k == 0 && temp[k] == ' ')
            {
              k++;
              continue;
            }
            if(temp[k]!='"')
              value[l++] = temp[k];
            k++;
          }
          map<string, string>::iterator it1;
          it1 = alias.find(key);
          if(it1 == alias.end())
            alias.insert(pair<string, string>(key,value));
          else
            alias[key] = value;
          }
        }
        else if(strcmp(command,"unalias") == 0)
        {
      //    cout<<"command "<<command<<endl;
      //    cout<<"cmdpara "<<cmdpara<<endl;
          if(strcmp(cmdpara,"-a") ==0)
          {
            alias.clear();
          }
          else
          {
            map<string, string>::iterator it;
            it = alias.find(cmdpara);
            if(it != alias.end())
            {
              alias.erase(it);
            }
          }
        }
        else if(strcmp(command,"export") == 0 && strstr(cmdpara,"="))
        {
          char exp[1000];
          char *key;
          char *value;
      //    cout<<"cmdpara "<<cmdpara<<endl;
          strcpy(exp,cmdpara);
      //    cout<<"cmdpara "<<cmdpara<<endl;
        //  cout<<"global "<<global<<endl;
          key = strtok(cmdpara,"=");
        //  value = strtok(cmdpara,"=");
          value = strtok(NULL,"=");
        ///  cout<<"value "<<value<<endl;
        //  cout<<"key "<<key<<endl;
        //cout<<"cmdpara "<<exp<<endl;
          setenv(key,value,1);
          FILE *fp2 = fopen(".resourcerc","a+");
          fprintf (fp2,"%s\n",exp);
    //      fflush(fp2);
          fclose(fp2);
        }
        else if(strcmp(command,"export") == 0 && !strstr(cmdpara,"="))
        {
          char key[1000];
          char temp[1000];
          char n1[1000];
          char n2[1000];
      //    cout<<"command "<<command<<endl;
      //    cout<<"cmdpara "<<cmdpara<<endl;
          map<string, string>::iterator it;
          it = echovalue.find(cmdpara);
          if(it != echovalue.end())
          {
            strcpy(temp,(it->second).c_str());
        //    cout<<"here"<<endl;
            strcpy(key,cmdpara);
            strcat(key,"=");
            strcat(key,temp);
            echovalue.erase(it);
            strcpy(n1,cmdpara);
            strcpy(n2,temp);
        //    cout<<"n1 "<<n1<<endl;
        //    cout<<"n2 "<<n2<<endl;
            setenv(n1,n2,1);
      //      cout<<"key "<<key<<endl;
            FILE *fp5 = fopen(".resourcerc","a+");
            fprintf (fp5,"%s\n",key);
            fclose(fp5);
          }
        }
        else if(strstr(cmdpara,"=") && !strstr(command,"alias") && !strstr(command,"export"))
        {
          char* key;
          char* value;
          key = strtok(command, " ");
          value  = strtok(cmdpara,"=");
          value = strtok(value," ");
    //      cout<<"key "<<key<<endl;
    //      cout<<"value "<<value<<endl;
          echovalue[key] = value;
          map<string,string>::iterator it;
         for(it = echovalue.begin();it != echovalue.end(); it++)
         {
            cout<<it->first<<" "<<it->second<<endl;
          }
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
        else if(strcmp(command,"exit")==0)
        {
          if(root==0)
          {
            exit(1);
          }
          else
          {
            ps1 = (string)getenv("PS1");
            ps1 = ps1 + directory;
          //  cout<<ps1<<">>";
            root =0;
          }
        }
        else
        {
        //   cout<<"comdpara else "<<strlen(cmdpara)<<endl;
        //    cout<<"command else "<<strlen(command)<<endl;
            strcpy(cmd, "/bin/");
            strcat(cmd, command);
            execve(cmd,parameter,environment);
        }
        }
        int exitstatus;
        waitpid((long)getpid(),&exitstatus,0);
        status = WEXITSTATUS(exitstatus);
        fflush(stdin);
        fflush(stdout);
        memset(cmdpara, 0, sizeof(cmdpara));
        memset(command, 0, sizeof(command));
        memset(parameter, 0, sizeof(parameter));
        memset(cmd, 0, sizeof(cmd));
        memset(global,0, sizeof(global));
      }

      memset(cmdpara, 0, sizeof(cmdpara));
      memset(command, 0, sizeof(command));
      memset(parameter, 0, sizeof(parameter));
      memset(cmd, 0, sizeof(cmd));
      memset(global,0, sizeof(global));
  //    close(fd);
      fflush(stdin);
      fflush(stdout);
    }
    return 0;
  }
