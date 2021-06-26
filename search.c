#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef struct {
    char* pattern;
    int i;
    int trigger;
    int sort;
    int count;
    char* file;
    FILE* path;
    int m;
    int k;
    int arrayLen;
    int buf;
    char** sortArry;
    int ret;
} Global;

// function to print the common error
void common_print() {
    fprintf(stderr, "Usage: search [-exact|-prefix|-anywhere] [-sort] pattern "
            "[filename]\n");
    exit(30);
}

// checking if the pattern input is correct
void patterncheck(Global* g) {
    char* str1 = (char*)malloc(sizeof(char) * 100);
    str1 = g->pattern;
    int len = strlen(g->pattern);
    for (int j = 0; j < len; j++) {
        if (str1[j] >= 65 && str1[j] <= 90) {
        } else if (str1[j] >= 97 && str1[j] <= 122) {
        } else if (str1[j] == 63) {
        } else {
            fprintf(stderr, "search: pattern should only contain question "
                    "marks and letters\n");
            exit(30);
        }
    }
    if (g->i == 1) {
        g->trigger = 1;
    }
}

//checking if the output contains alphabets of not
//storing the alphabets in an array if -sort is present
void alphabet(char* words, int j, Global* g) {
    int x = 0;
    while (x < j + 1) {
        if (words[x] >= 65 && words[x] <= 90) {
        } else if (words[x] >= 97 && words[x] <= 122) {
        } else {
            break;
        }
        if (x == j - 1) {
            if (g->sort == 1) {
                if (g->arrayLen == g->buf - 1) {
                    int currbuf = g->buf;
                    g->buf = g->buf * 2;
                    g->sortArry = realloc(g->sortArry, sizeof(char*) * g->buf);
                    for (int i = currbuf; i < g->buf; i++) {
                        g->sortArry[i] = malloc(sizeof(char) * 41);    
                    }
                }
                g->sortArry[g->arrayLen++] = strdup(words);
            } else {
                fprintf(stdout, "%s", words);
            }
        }
        x++; 
    }
}

//exact function finding same words
void exact_same(Global* s){
    FILE* dictionary; 
    char* str = (char*)malloc(sizeof(char) * 100);
    str = s->pattern;
    int y = strlen(s->pattern);
    char* words = (char*)malloc(sizeof(char) * 100);
    char* str2 = (char*)malloc(sizeof(char) * 100);
    dictionary = fopen(s->file, "r");
    if (dictionary == NULL) {
        fprintf(stderr, "search: file \"%s\" can not be opened\n", s->file);
        exit(30);
    }
    while (fgets(words, 41, dictionary) != NULL) {
        int i = 0, j = 0, alpCount = 0;
        int wordCount = strlen(words);
        if (y == wordCount - 1) {
            while (i < y) {
                while (str[i] == 63) {
                    j++;
                    i++; 
                }
                if (isupper(str[i])) {
                    str2[i] = str[i] + 32;
                    alpCount++;
                }
                if (islower(str[i])) {
                    str2[i] = str[i] - 32;
                    alpCount++;
                }
                if (y - j == alpCount && j > 0 && str[y - 1] == 63) {
                    alphabet(words, y, s);
                } 
                if ((words[i] != str[i]) && (words[i] != str2[i])) {
                    break;
                }
                if (i == y - 1) {
                    alphabet(words, y, s);
                }            
                i++;
            }
        }  
    }
    fclose(dictionary);
}

// finding words with the same prefix as the pattern given
void prefix(Global* s) {
    FILE* dictionary; 
    char* str = (char*)malloc(sizeof(char) * 100);
    str = s->pattern;
    int y = strlen(s->pattern);
    char* words = (char*)malloc(sizeof(char) * 100);
    char* str2 = (char*)malloc(sizeof(char) * 100);
    dictionary = fopen(s->file, "r");
    if (dictionary == NULL) {
        fprintf(stderr, "search: file \"%s\" can not be opened\n",s->file);
        exit(30);
    }
    while (fgets(words, 41, dictionary) != NULL) {
        int i = 0, j = 0, k = 0, alpCount = 0;
        int wordCount = strlen(words);
        while (k < wordCount - 1) {
            if (y < 1) {
                alphabet(words, wordCount - 1, s);
                break;
            }
            break;
        }
        if (y <= wordCount - 1) {
            while (i < y) {
                while (str[i] == 63) {
                    j++;
                    i++; 
                }
                if (isupper(str[i])) {
                    str2[i] = str[i] + 32;
                    alpCount++;
                }
                if (islower(str[i])) {
                    str2[i] = str[i] - 32;
                    alpCount++;
                }  
                if (y - j == alpCount && j > 0 && str[y - 1] == 63) {
                    alphabet(words, wordCount - 1, s);
                }
                if ((words[i] != str[i]) && (words[i] != str2[i])) {
                    break;
                }
                if (i == y - 1) {
                    alphabet(words, wordCount - 1, s);
                }
                i++;
            }
        }
    }    
    fclose(dictionary);
}

// finding words which contains the pattern given
void anywhere(Global* s) {
    FILE* dictionary; 
    char* str = (char*)malloc(sizeof(char) * 100);
    str = s->pattern;
    int y = strlen(s->pattern);
    char* words = (char*)malloc(sizeof(char) * 100);
    char* str2 = (char*)malloc(sizeof(char) * 100);
    dictionary = fopen(s->file, "r");
    if (dictionary == NULL) {
        fprintf(stderr, "search: file \"%s\" can not be opened\n",s->file);
        exit(30);
    }
    int t = 0;
    while (str[t] == 63) {
        t++;    
    }
    while (fgets(words, 41, dictionary) != NULL) {
        int i = 0, j = 0, k = 0, l = 0, alpCount = 0;
        int wordCount = strlen(words);
        while (k < wordCount - (y - t)) {
            while (str[l] == 63) {
                l++;
                k++;
            }
            if (l == y && wordCount - 1 >= y) {
                alphabet(words, wordCount - 1, s);
                break;
            }
            if (isupper(str[l])) {
                str2[l] = str[l] + 32;
            }
            if (islower(str[l])) {
                str2[l] = str[l] - 32;
            }
            if ((words[k] == str[l]) || (words[k] == str2[l])) {
                s->m = k;   
                if (k == wordCount - 1) {
                    break;
                }
                if (y <= wordCount - 1) {
                    while (i < y) {
                        while (str[i] == 63) {
                            j++;
                            i++;
                            if (i > l) {
                                s->m++;
                            }
                        }
                        if (isupper(str[i])) {
                            str2[i] = str[i] + 32;
                            alpCount++;
                        }
                        if (islower(str[i])) {
                            str2[i] = str[i] - 32;
                            alpCount++;
                        }   
                        if (j > 0 && str[y - 1] == 63) {
                            if (wordCount - 1 >= s->m && i > y - 2) {
                                alphabet(words, wordCount - 1, s);
                                k = wordCount - 1;
                                break;
                            }
                        }  
                        if (words[s->m] != str[i]) {
                            if (words[s->m] != str2[i]) {
                                i = 0;
                                break;
                            }
                        }
                        if (i == y - 1) {
                            alphabet(words, wordCount - 1, s);
                            k = wordCount - 1;
                            break;
                        }
                        i++;
                        s->m++;
                    }
                }
            }
            k++;
        }     
    }
    fclose(dictionary);
}

//comparing the strings recieved by quicksort and sorting them
//sorting by converting the uppercase into lower case
//making the converted lower case into upper case after sorting
static int compare(const void* cmp1, const void* cmp2) {
    Global g;
    char* word1 = *((char**)cmp1);
    char* word2 = *((char**)cmp2);
    int size = strlen(word1);
    char temp[size];
    int size2 = strlen(word2);
    char temp2[size2];
    for (int i = 0; i < size; i++) {
        temp[i] = word1[i];
        if (isupper(word1[i])) {
            word1[i] = word1[i] + 32;
        }
    }
    for (int i = 0; i < size2; i++) {
        temp2[i] = word2[i];
        if (isupper(word2[i])) {
            word2[i] = word2[i] + 32;
        }
    }
    g.ret = strcmp(*(char* const*)cmp1, *(char* const*)cmp2);
    for (int i = 0; i < size; i++) {
        word1[i] = temp[i];
    }
    for (int i = 0; i < size2; i++) {
        word2[i] = temp2[i];
    }
    return (g.ret);
}

//sorting the words in the array
void sorting(Global* g){
    g->ret = 0;
    qsort(g->sortArry, g->arrayLen, sizeof (char*), compare);
    for (int i = 0; i < g->arrayLen; i++) {
        printf("%s",g->sortArry[i]);
        free(g->sortArry[i]);
    }
}

// checking if the file can be opened or not
void openfile(Global* s){
    FILE* dictionary;
    dictionary = fopen(s->file, "r");
    if (dictionary == NULL) {
        fprintf(stderr, "search: file \"%s\" can not be opened\n",s->file);
        exit(30);
    }
    fclose(dictionary);
}

// checking if the inputs containing - are correct inputs
// if the input is not a valid input, printing the error and exiting
void check(char* argv, Global* g) {
    char* exact = "-exact";
    char* prefix = "-prefix";
    char* anywhere = "-anywhere";
    char* sort = "-sort";
    if (strcmp(argv, exact) == 0) {
        g->trigger = 1;       
    } else if (strcmp(argv, prefix) == 0) {
        g->trigger = 2;
    } else if (strcmp(argv, anywhere) == 0) {
        g->trigger = 3;
    } else if (strcmp(argv, sort) == 0) {
        if (g->count == 1) {
            g->trigger = 1;
        }
        g->arrayLen = 0;
        g->sort = 1;
    } else {
        common_print();
    }

}

// checking if there are more than 2 command line arguments provided
// checking if the those arguments contain -
// Checking if the first 2 inputs contain -
// if it contains - passing it to the check function
// passing the pattern to struct
// calling the functions according to the input provided
void assign(int argc, char** argv) {
    char* temp;
    Global g;
    char* sort = "-sort";
    char* pattern = "?";
    if (argc > 3) {
        for (int i = 3; i < argc; i++) {
            temp = argv[i];
            if (temp[0] == '-') {
                common_print();
            }		       
        }
    }
    g.count = 1;
    for (int i = 1; i < argc; i++) {
        temp = argv[i];
        if (temp[0] == '-') {
            check(argv[i], &g);
            g.count++;
        } else {
            if (g.count == argc) {           
                g.pattern = pattern;
            }
	    g.pattern = argv[i];
	    g.i = i;
	    patterncheck(&g);
    	break;
        }		
    }
    if (g.count > 2) { 
        if ((strcmp(argv[1], sort) == 0) || (strcmp(argv[2], sort) == 0)) {
        } else {
            common_print();
        }
        if ((strcmp(argv[1], sort) == 0) && (strcmp(argv[2], sort) == 0)) {
            common_print();
        }
    }
    if (g.count == argc) {
        common_print();
    }
    if (g.i != argc - 1) {
        char* c = argv[g.i + 1];
        g.file = c;
        if (g.i + 1 != argc - 1) {
	    common_print();
	}
	openfile(&g);
    } else {
        g.file = "/usr/share/dict/words";
        openfile(&g);
    }
    g.buf = 300;
    g.sortArry = malloc(sizeof(char*) * g.buf);
    for (int i = 0; i < g.buf; i++) {
        g.sortArry[i] = malloc(sizeof(char) * 41);
    }
    if (g.trigger == 1) {
        exact_same(&g);
    } else if (g.trigger == 2) {
        prefix(&g);
    } else if (g.trigger == 3) {
        anywhere(&g);
    }
    if (g.sort == 1) {
        sorting(&g);
    }
}

int main(int argc, char** argv) {
    if (argc > 5 || argc < 2) {
        common_print();
    }
    assign(argc, argv);
    return 0;
}
