#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define who "WHO:"
#define lenwho 4
#define msg "MSG:"
#define lenmsg 4
#define finish "QUIT:"
#define lenquit 5
#define taken "NAME_TAKEN:"
#define lentaken 11
#define yt "YT:"
#define lenyt 3
#define kick "KICK:"
#define lenkick 5
#define kick1 "KICK:\n"
#define lenkick1 6
#define chat "CHAT:"
#define done "DONE:"
#define lendone 5

void input_msg(char* input) {
    char* s = ":";
    char* token = strtok(input, s);
    int lentoken = strlen(token);
    if (lentoken + 1 == lenmsg) {
        char* end = strtok(NULL, "\n");
        if(strstr(end, s) != NULL) {
            token = strtok(end, s);
        } else {
            fprintf(stderr, "Communications error\n");
            exit(2);
        }
        char* e = "\n";
        fprintf(stderr, "(%s)", token);
        fflush(stdout);
        token = strtok(NULL, e);
        if (strstr(token, s) != NULL) {
            token = strtok(token, s);
            fprintf(stderr, " %s\n", token);
        } else {
            fprintf(stderr, " %s\n", token);
        }
    }   
}

void input_yt(char* str, FILE* chatfile, int leninput) {
    char* e = "\n";
    while (fgets(str, 100, chatfile) != NULL) {
        if(strstr(str, chat) != NULL || strstr(str, kick) != NULL) {
            printf("%s", str);
            fflush(stdout);
            if(strstr(str, e) != NULL) {
            } else {
                printf("\n");
            } 
        } 
        if (strstr(str, done) != NULL) {
        int lenstr = strlen(str);
        if (lenstr == lendone + 1) {
            printf("%s" ,str);
            fflush(stdout);
            if (strstr(str, e) != NULL) {
            } else {
                printf("\n");
            }
            break;
        } else {
            continue;
        }
    }      
    if(strstr(str, finish) != NULL) {
        if (leninput + 1 == lenquit) {
            printf("%s", str);
            fflush(stdout);
            if(strstr(str, e) != NULL) {
            } else {
                printf("\n");
            }
            exit(0);
            }
        }
    }

}

void input_kick() {
    fprintf(stderr, "Kicked\n");
    exit(3);
}

void com_err() {
    fprintf(stderr, "Communications error\n");
    exit(2);
}

void usage_err() {
    fprintf(stderr, "Usage: client chatscript\n");
    exit(1);
}

void client (char** argv) {
    FILE* chatfile;
    char* str = (char*)malloc(sizeof(char)* 100);
    char* name = (char*)malloc(sizeof(char)* 100);
    chatfile = fopen(argv[1], "r");
    if (chatfile == NULL) {
        usage_err();    
    }
    name = "NAME:client\n";
    char* input = (char*)malloc(sizeof(char)* 100) ;
    int quit = 1, cno = 0;
    while (quit == 1) {
        fgets(input, 5000, stdin);
        int leninput = strlen(input); 
        if (strstr(input, who) != NULL && leninput == lenwho + 1) {
            char* e = "\n";
            printf("%s", name);
            fflush(stdout);
            if(strstr(name, e) != NULL) {
            } else {
                printf("\n");
            }
            continue;
        } else if (strstr(input, taken) != NULL && leninput == lentaken + 1) {
            char temp[1000];
            snprintf(temp, 100, "NAME:client%d\n", cno);
            name = temp;
            cno++;
            continue;
        } else if (strstr(input, msg) != NULL) {    
            input_msg(input);
        } else if (strstr(input, yt) != NULL && leninput == lenyt + 1) {
            input_yt(str, chatfile, leninput);
        } else if (strstr(input, kick) != NULL && leninput <= lenkick + 1) {
            input_kick();
        }  else {
            com_err();
        }
    }
}

int main(int argc, char** argv) {
    if (argc == 2) {
        client(argv);
    }
    else {
        usage_err();    
    }
}

