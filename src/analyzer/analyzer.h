#ifndef _ANALYZER_H_
#define _ANALYZER_H_

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <assert.h>
#include <ctype.h>
#include <signal.h>

#define READ 0
#define WRITE 1
#define CLUSTER 5

#define BOOL char
#define FALSE 0
#define TRUE 1
#define MAXLEN 12*sizeof(int)
#define MAXQ 10000

//costanti funzionali
/*int N = 3;
int M = 4;
int n_arg = 1;*/

//define per il sender
#define ProjectId 123
#define PathName  "/root/src/analyzer/analyzer.h"  /* any existing, accessible file would do */
#define PathName2  "/root/src/analyzer/analyzer.h"
#define MAX_MSG_SIZE 100

typedef struct {
  long type;                  //must be of type long 
  char payload[MAX_MSG_SIZE]; // bytes in the message 
} queuedMessage;

typedef char * string;

// https://www.hackerearth.com/practice/data-structures/linked-list/singly-linked-list/tutorial/
struct LinkedList {
    char *str;
    struct LinkedList *next;
};

typedef struct LinkedList *node;

BOOL isTxt(string file, int length);
BOOL isCpp(string file, int length);
BOOL isC(string file, int length);
BOOL isJava(string file, int length);
BOOL isPy(string file, int length);

//funzione per cartelle

node createNode();
node addNode(node head, char *new_str);
int file_exists(char *filename);
int is_folder(char *folder);
char *concat(const char *s1, const char *s2, const char *s3);
node listFiles(char *path);

BOOL containsDot(string str);

void report_and_exit(const char* msg);

void sendConfirm(string messaggio, int projID);

string recConfirm(int projID);

//funzioni per aggiungere roba al volo

string addThingsToCounter();

string addFile();

string changeNM();

BOOL stringIsInt(char *str);

void sendSignal(int signal);
void ignoreSignal(int signal);
#endif
