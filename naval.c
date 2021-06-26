#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct {
int row;
int column;
int ship;
char grid[50][50];
char cpu[50][50];
char show[50][50];
int length[50];
char cor[50];
char cor1[50];
char col[50];
char col1[50];
int dig[50];
char dig1[50];
char cpucol[50];
int cpudig[50];
int k;
char pt[50];
int pt1[50];
char st;
int st1;
int cd;
int de;
int ef;
int fg;
} myStruct;

void file(char* rules, myStruct *s){ 
int rows, columns, ships;
FILE *fp;
char str[200];
fp = fopen(rules,"r");
    if (fp==NULL){
    printf("couldn't open file \n");
    exit(20);
    }
do{
fgets(str,200,fp);
}while(*str=='#');
    sscanf(str,"%d %d", &rows, &columns);
    if (rows>26 || columns>26){
        printf("maximum size of rows and columns is 26");
        exit(50);
    }
        int g,j; 
    for(g=0;g<rows;g++){
         for(j=0;j<columns;j++){
          s->cpu[g][j]='.';  
          s->grid[g][j]='.';
          s->show[g][j]='.';
                    }
           }
do{
    fgets(str,200,fp);
}while(*str=='#');
sscanf(str,"%d", &ships);
if(rows<=0 || columns<=0){
        printf("invalid input parameters \n");
        exit(20); 
}
if(ships>15){
    printf(">15 ships \n");
    exit(0);
}
do{
fgets(str,200,fp);
}while(*str=='#');

int i=0;
int lengths[ships];
do{
sscanf(str,"%d",&lengths[i]);
i++;
}while(fgets(str,200,fp)!=NULL);
for(i=0;i<ships;i++){
   s->length[i]=lengths[i];
    }
s->ship=ships;
s->row=rows;
s->column=columns;
int x=65+s->column;
    printf(" ");
    for(int a=65;a<x;a++){
        printf(" %c",a);
    }
    printf("\n");
for(int g=0;g<s->row;g++){
        if ((g+1)<10){
            printf("%d ",g+1);
        }
        else{
            printf("%d",g+1);
        }
        for(int j=0;j<s->column;j++){
            printf("%c ",s->show[g][j]);
        }
        printf("\n");
    }
printf("===\n");

fclose(fp);
}

void map(char* map,myStruct *mayank){
FILE *fp;
char str[2];
int arr[100];
char* cor1=(char*)malloc(sizeof(char)*100);
char* cor2=(char*)malloc(sizeof(char)*100);

fp = fopen(map,"r");
    if (fp==NULL){
    printf("couldn't open file \n");
    exit(30);
}
int k=0, l=0;
while(fgets(str,200,fp)!=NULL){
if(*str=='#')
{
continue;
}
int c=mayank->column+65;
sscanf(str,"%c%d %c",&cor1[k],&arr[k],&cor2[k]);
 mayank->cor[k]=cor1[k];
 mayank->dig[k]=arr[k];
mayank->col[k]=cor2[k];

 if(cor1[k]>c || arr[k]>mayank->row){
        printf("\nblock not found on the board\n");
        exit(120);
 }
 if(cor1[k]<65 || arr[k]<1){
        printf("\nblock not found on the board\n");
        exit(120);
 }
k++;
if(k==mayank->ship){
    break;
}
}

int c=mayank->column+65;
for(int ab=0;ab<mayank->ship;ab++){ 
      if(mayank->col[ab]==83){
        int d=mayank->dig[ab]-1;
        int e=mayank->cor[ab]-65;
        for(l=0;l<mayank->length[ab];l++){           
            if(mayank->grid[d][e]!=46){
                    printf("overlapping ships \n");
                    exit(60);
            }
             else if(d>mayank->row || d<0)
            {
                printf("ships out of bound in PLAYER map s\n");
                exit(80);
            }

             mayank->grid[d][e]=ab+1+'0';
            d++;
        }
    }
    
    if(mayank->col[ab]==69){
        int d=mayank->dig[ab]-1;
        int e=mayank->cor[ab]-65;
        for(l=0;l<mayank->length[ab];l++){ 
            if(mayank->grid[d][e]!=46){
                    printf("overlapping ships \n");
                    exit(60);
            }else if(e>c || e<0 )
            {
                printf("ships out of bound in PLAYER map e\n");
                exit(80);
            }

             mayank->grid[d][e]=ab+1+'0';
            e++;
        }
    }

    if(mayank->col[ab]==78){
        int d=mayank->dig[ab]-1;
        int e=mayank->cor[ab]-65;
        for(l=0;l<mayank->length[ab];l++){ 
            if(mayank->grid[d][e]!=46){
                    printf("overlapping ships \n");
                     exit(60);
            }else if(d>mayank->row || d<0)
            {
                printf("ships out of bound in PLAYER map n\n");
                exit(80);
            }

                    
            mayank->grid[d][e]=ab+1+'0';
            d--;
        }
    }

    if(mayank->col[ab]==87){
        int d=mayank->dig[ab]-1;
        int e=mayank->cor[ab]-65;
        for(l=0;l<mayank->length[ab];l++){
            if(mayank->grid[d][e]!=46){
                    printf("overlapping ships \n");
                             exit(60);
            }else if(e>c || e<0 )
            {
                printf("ships out of bound in PLAYER map w\n");
                exit(80);
            }

            mayank->grid[d][e]=ab+1+'0';
            e--;
        }
    }

 mayank->grid[mayank->dig[ab]-1][mayank->cor[ab]-65]=ab+1+'0';
}
int x=65+mayank->column;
    printf(" ");
    for(int a=65;a<x;a++){
        printf(" %c",a);
    }
    printf("\n");
for(int g=0;g<mayank->row;g++){
        if ((g+1)<10){
            printf("%d ",g+1);
        }
        else{
            printf("%d",g+1);
        }
        for(int j=0;j<mayank->column;j++){
            printf("%c ",mayank->grid[g][j]);
        }
        printf("\n");
    }

}

void cpumap(char* map,myStruct *mayank){
FILE *fp;
char str[2];
int arr[100];
char* cor1=(char*)malloc(sizeof(char)*100);
char* cor2=(char*)malloc(sizeof(char)*100);

fp = fopen(map,"r");
    if (fp==NULL){
    printf("couldn't open file \n");
    exit(30);
}
int k=0, l=0;
while(fgets(str,200,fp)!=NULL){
if(*str=='#')
{
continue;
}
int c=mayank->column+65;
sscanf(str,"%c%d %c",&cor1[k],&arr[k],&cor2[k]);
 mayank->cor1[k]=cor1[k];
 mayank->dig1[k]=arr[k];
mayank->col1[k]=cor2[k];

 if(cor1[k]>c || arr[k]>mayank->row){
        printf("\nblock not found on the board\n");
        exit(120);
 }
 if(cor1[k]<65 || arr[k]<1){
        printf("\nblock not found on the board\n");
        exit(120);
 }
k++;
if(k==mayank->ship){
    break;
}
}
int c=mayank->column+65;
for(int ab=0;ab<mayank->ship;ab++){
  
      if(mayank->col1[ab]==83){
        int d=mayank->dig1[ab]-1;
        int e=mayank->cor1[ab]-65;
        for(l=0;l<mayank->length[ab];l++){           
            if(mayank->cpu[d][e]!=46){
                fprintf(stderr,"Overlap in CPU map file\n");
                    exit(70);
            }
            else if(d>mayank->row || d<0)
            {
                fprintf(stderr,"Out of bounds in CPU map file\n");

                exit(90);
            }
             mayank->cpu[d][e]=ab+1+'0';
            d++;
        }
    }
    
    if(mayank->col1[ab]==69){
        int d=mayank->dig1[ab]-1;
        int e=mayank->cor1[ab]-65;
        for(l=0;l<mayank->length[ab];l++){ 
            if(mayank->cpu[d][e]!=46){
                    fprintf(stderr,"Overlap in CPU map file\n");

                    exit(70);
            }
             else if(e>c || e<0 )
            {
                fprintf(stderr,"Out of bounds in CPU map file\n");
                exit(90);
            }
             mayank->cpu[d][e]=ab+1+'0';
            e++;
        }
    }

    if(mayank->col1[ab]==78){
        int d=mayank->dig1[ab]-1;
        int e=mayank->cor1[ab]-65;
        for(l=0;l<mayank->length[ab];l++){ 
            if(mayank->cpu[d][e]!=46){
                    fprintf(stderr,"Overlap in CPU map file\n");

                    exit(70);
            }
             else if(d>mayank->row || d<0)
            {
                fprintf(stderr,"Out of bounds in CPU map file\n");
                exit(90);
            }

            mayank->cpu[d][e]=ab+1+'0';
            d--;
        }
    }

    if(mayank->col1[ab]==87){
        int d=mayank->dig1[ab]-1;
        int e=mayank->cor1[ab]-65;
        for(l=0;l<mayank->length[ab];l++){
            if(mayank->cpu[d][e]!=46){
                    fprintf(stderr,"Overlap in CPU map file\n");
                    exit(70);
            }
             else if(e>c || e<0 )
            {
                fprintf(stderr,"Out of bounds in CPU map file\n");
                exit(90);
            }

            mayank->cpu[d][e]=ab+1+'0';
            e--;
        }
    }
 mayank->cpu[mayank->dig1[ab]-1][mayank->cor1[ab]-65]=ab+1+'0';
}
}


void cputurns(char* map,myStruct *mayank){
char str[200];
FILE *fp;
int arr[100];
char* cor1=(char*)malloc(sizeof(char)*100);
fp = fopen(map,"r");
    if (fp==NULL){
    fprintf(stderr,"Missing CPU turns file\n");
    exit(40);
}
printf("\n");
int k=0;
while(fgets(str,200,fp)!=NULL){
if(*str=='#')
{
continue;
}
int c=mayank->column+65;
sscanf(str," %c%d",&cor1[k],&arr[k]);
 mayank->cpucol[k]=cor1[k];
 mayank->cpudig[k]=arr[k];
 if(cor1[k]>c || arr[k]>mayank->row){
        fprintf(stderr,"block not found on the board\n");
        exit(120);
 }
 if(cor1[k]<65 || arr[k]<1){
        fprintf(stderr,"Error in turns file\n");
        exit(120);
 }
k++;
if(k==250){
    break;
}
mayank->k=k;
}
}

int main(int argc, char** argv){
if(argc<5){
   fprintf(stderr,"Usage: naval rules playermap cpumap turns\n");
    exit(10);
}
myStruct s;
file(argv[1],&s);
map(argv[2],&s);
cpumap(argv[3],&s);
cputurns(argv[4],&s);
int k=0;
int ab=0;
s.cd=1000;
s.de=1000;
s.ef=1000;
s.fg=1000;
char* pt=(char*)malloc(sizeof(char)*100);
int c[200];
while(k<s.k){
printf("(YOUR MOVE)>");
scanf(" %c%d",&pt[k],&c[k]);
for(int i =0; i<k;i++){
if(pt[k]==pt[i] && c[k]==c[i])
{
    printf("REPEATED GUESS\n");
    printf("(YOUR MOVE)>");
     scanf(" %c%d",&pt[k],&c[k]);
        s.pt[k]=pt[k];
        s.pt1[k]=c[k];

}
}
    if(pt[k]<s.column+66 && c[k]<s.row+1 && pt[k]>=65 &&c[k]>0 ){
       s.pt[k]= pt[k];
        s.pt1[k]=c[k];
        }
    else{
        printf("BAD GUESS\n");
        printf("(YOUR MOVE)>");
        scanf(" %c%d",&pt[k],&c[k]);
        s.pt[k]=pt[k];
        s.pt1[k]=c[k];
        for(int i =0; i<k;i++){
if(pt[k]==pt[i] && c[k]==c[i])
{
    printf("REPEATED GUESS\n");
    printf("(YOUR MOVE)>");
     scanf(" %c%d",&pt[k],&c[k]);
        s.pt[k]=pt[k];
        s.pt1[k]=c[k];

}
}

        }

if(s.cpu[s.pt1[k]-1][s.pt[k]-65]!='.'){
printf("HIT\n");
s.cpu[s.pt1[k]-1][s.pt[k]-65]='*';
s.show[s.pt1[k]-1][s.pt[k]-65]='*';
}
else{
printf("MISS\n");
s.cpu[s.pt1[k]-1][s.pt[k]-65]='/';
s.show[s.pt1[k]-1][s.pt[k]-65]='/';
}
printf("(CPU MOVE)>%c%d\n",s.cpucol[k],s.cpudig[k]);
for(int i =0; i<k;i++){
if(s.cpudig[k]==s.cpudig[i] && s.cpucol[k]==s.cpucol[i])
{
    printf("Repeated Guess\n");
    k++;
    printf("(CPU MOVE)>%c%d\n",s.cpucol[k],s.cpudig[k]);

}
}

if(s.grid[s.cpudig[k]-1][s.cpucol[k]-65]=='.'){
printf("MISS\n");
s.grid[s.cpudig[k]-1][s.cpucol[k]-65]='/';
}
else{
printf("HIT\n");
s.grid[s.cpudig[k]-1][s.cpucol[k]-65]='*';
}
for(ab=0;ab<s.ship;ab++){         
        if((s.col1[ab]==83)){
        int d=s.dig1[ab]-1;
        int e=s.cor1[ab]-65;
        for(int l=0;l<s.length[ab];l++){           
            if(s.cpu[d][e]=='*'){        
            d++;
                }
            else{
                break;
            }
            //printf("this is cd %d and ab %d\n",s.cd,ab);
            if(s.cd!=ab){
                if(l+1==s.length[ab]){
                    printf("SHIP SUNK: %d\n",ab+1);
                    s.cd=ab;
                    break;
                }
            }
            else{
                break;
            }
         }
     }
        if((s.col1[ab]==69)){
        int d=s.dig1[ab]-1;
        int e=s.cor1[ab]-65;
        for(int l=0;l<s.length[ab];l++){           
            if(s.cpu[d][e]=='*'){        
            e++;
                }
            else{
                break;
            }
            if(s.de!=ab){
                if(l+1==s.length[ab]){
                    printf("SHIP SUNK: %d\n",ab+1);
                    s.de=ab;
                    break;
                }

                        }
            else{
                 break;
            }
        }
    }
         if((s.col1[ab]==78)){
        int d=s.dig1[ab]-1;
        int e=s.cor1[ab]-65;
        for(int l=0;l<s.length[ab];l++){           
            if(s.cpu[d][e]=='*'){        
            if(s.length[ab]>1){
                d--;
            }
                }
            else{
                break;
            }
            if(s.ef!=ab){
            if(l+1==s.length[ab]){
                printf("SHIP SUNK: %d\n",ab+1);
                s.ef=ab;
                break;
            }

            }
            else{
               break; 
            }
        }
    }
          if((s.col1[ab]==87)){
        int d=s.dig1[ab]-1;
        int e=s.cor1[ab]-65;
        for(int l=0;l<s.length[ab];l++){           
            if(s.cpu[d][e]=='*'){        
            if(s.length[ab]>1){
                e--;
            }
                }
            else{
                break;
            }
            if(s.fg!=ab){
                if(l+1==s.length[ab]){
                    printf("SHIP SUNK: %d\n",ab+1);
                    s.fg=ab;
                    break;
            }
            else{
                break;
            }
           }
        }
    }         
}
int x=65+s.column;
    printf(" ");
    for(int a=65;a<x;a++){
        printf(" %c",a);
    }
    printf("\n");
for(int g=0;g<s.row;g++){
        if ((g+1)<10){
            printf("%d ",g+1);
        }
        else{
            printf("%d",g+1);
        }
        for(int j=0;j<s.column;j++){
            printf("%c ",s.show[g][j]);
        }
        printf("\n");
    }
printf("===\n");
    printf(" ");
    for(int a=65;a<x;a++){
        printf(" %c",a);
    }
    printf("\n");
for(int g=0;g<s.row;g++){
        if ((g+1)<10){
            printf("%d ",g+1);
        }
        else{
            printf("%d",g+1);
        }
        for(int j=0;j<s.column;j++){
            printf("%c ",s.grid[g][j]);
        }
        printf("\n");
    }
 k++;
}

fprintf(stderr,"CPU player gives up\n");
exit(140);
return 0;
}
