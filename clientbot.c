#include<stdio.h>
#include<string.h>
#define _GNU_SOURCE
char* strcasestr(const char* haystack, const char* needle);
#include<stdlib.h>

#define who "WHO:"
#define lenwho 4
#define msg "MSG:"
#define lenmsg 4
#define taken "NAME_TAKEN:"
#define lentaken 11
#define yt "YT:"
#define lenyt 3
#define kick "KICK:"
#define lenkick 5
#define kick1 "KICK:\n"
#define lenkick1 6
#define done "DONE:"
#define lendone 5


typedef struct {
    char* search;
    char* name;
    char output[200][200];
    int count;
    int namelen;
} transfer;


void input_msg(char* recieved, transfer* t, FILE* chatfile, char* str) {
    char* s = ":";
    char* token = strtok(recieved,s);
    int lentoken = strlen(token);
    if (lentoken + 1 == lenmsg) {
        char* end = strtok(NULL,"\n");
        if(strstr(end,s)!=NULL) {
            token = strtok(end,s);
        } else {
            fprintf(stderr,"Communications error\n");
            exit(2);
        }
        char* e = "\n"; 
        fprintf(stderr,"(%s)",token);
        fflush(stdout);
        char* token1 = strtok(NULL,e);
        if(strstr(token1,s)!=NULL) {
            token1 = strtok(token1,s);
            fprintf(stderr," %s\n",token1);
        } else {
            fprintf(stderr," %s\n",token1);
        }
        t->search = token1;
        char* x = recieved+lentoken+1;
        x = strtok(x,s);
    }
    while (fgets(str,5000,chatfile) != NULL) {
        if(str[0] == '#') {
            continue;
        }
        char* token1 = strtok(str,":");
        if (strcasestr(t->search,token1) != 0) {
            char* token1 = strtok(NULL,"\0");
            int tokenlen = strlen(token);
            if(strstr(t->name,token)!= NULL && t->namelen == tokenlen) { 
                continue;
            }
            else {
                strcpy(t->output[t->count],token1);
            }
            t->count++;
            t->search = "abcl";
            fseek(chatfile, 0, SEEK_SET);
        }                
    }
    fseek(chatfile, 0, SEEK_SET);
}

void input_yt(transfer* t) {
    char* e = "\n";
    //printf("%d\n",t->count);
    if(t->count != 0) {
    for( int i = 0; i<t->count; i++) {
        printf("CHAT:%s",t->output[i]);
        fflush(stdout);
        if(strstr(t->output[i],e) == NULL){ 
            printf("\n");
        }
    }
    for( int i = 0; i<t->count; i++) {
        t->output[100][i]='\0';
    }
    t->count=0;
    }
    printf("DONE:\n");
    //break;
}

void input_kick() {
    fprintf(stderr,"Kicked\n");
    exit(3);
}

void com_err() {
    fprintf(stderr,"Communications error\n");
    exit(2);
}

void clientbot_res() {
    fprintf(stderr, "Usage: clientbot responsefile\n");
    exit(1);
}


void clientbot (char** argv) {
    transfer t;
    FILE* chatfile;
    char* str = (char*)malloc(sizeof(char)* 100);
    char* name = (char*)malloc(sizeof(char)* 100);
    chatfile = fopen(argv[1],"r");
    if (chatfile == NULL) {
        clientbot_res();    
    }
    name = "NAME:clientbot\n";
    t.name = "clientbot";
    t.namelen = strlen(t.name);
    t.search="lol";
    t.count = 0;
    char* input = (char*)malloc(sizeof(char)*100);
    int cno =0;
    char* ptr = NULL;
    int c;
    int index = 0;
        input[0] = '\0';
        while ( (c = fgetc(stdin))) {
            if (c == '\n' || c == EOF || c == '\r') {
                break;
            }
            input[index++] = (char)c;
        }
        input[index++]='\n';
    while (*input != '\n') {
         //   fgets(input,10000,stdin);
        char* recieved = input;
        //printf("%s\n",input);
        int leninput = strlen(recieved); 
        if (strstr(recieved,who)!= NULL && leninput == lenwho + 1) {
                printf("%s",name);
                fflush(stdout);
        } else if (strstr(recieved,taken)!=NULL && leninput == lentaken + 1) {
                char temp[1000];
                char* temp2;
                snprintf(temp,100,"NAME:clientbot%d\n",cno);
                name= temp;
                temp2=name+6;
                t.name=temp2-2;
                t.namelen = strlen(temp2);
                cno++;
        } else if (strstr(recieved,msg)!= NULL) {
            input_msg(recieved,&t,chatfile,str);
        }else if (strstr(recieved,kick)!= NULL && leninput <= lenkick + 1) {
            input_kick();
        } else if (strstr(recieved,kick1)!= NULL && leninput <= lenkick1 + 1) {
           input_kick();
        }  else if (strstr(recieved,yt)!= NULL && leninput == lenyt + 1) {
            input_yt(&t);
            //continue;
            //break;
        } 
        else {
            com_err();
        }
        index = 0;
        memset(input,0,sizeof(input));
        while ( (c = fgetc(stdin))) {
            if (c == '\n' || c == EOF || c == '\r') {
                break;
            }
            input[index++] = (char)c;
        }
        input[index++] = '\n';
    }

}

int main(int argc, char** argv) {
    if (argc == 2) {
        clientbot(argv);
    }
    else {
        clientbot_res();    
    }
}
