#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define FALSE 0
#define TRUE 1
typedef char *string;

/**
 * Max size when reading from input.
 * Limitaion given by C99.
 * Credits: https://stackoverflow.com/a/11488687/7924557
 */
#define MAX_INPUT_LENGHT 4096

/**
 * Max size of a path.
 * Credits: https://stackoverflow.com/a/9449307/7924557
 */
#define PATH_MAX 4096

struct LinkedList {
    char *str;
    struct LinkedList *next;
};
typedef struct LinkedList *node;

node createNode();
node addNode(node head, char *new_str);
char *concat(const char *s1, const char *s2, const char *s3);
node listFiles(char *path);
void removeNewline(char *string);
void splitAndSendPaths(char *string);
char *getSelfProcessPath();
void getAnalytics();
void printError(int errNumber);
int runProgram(char **path);
int runProgramAndWait(char **path);
char *baseName(char *path);
char *concatPaths(char *dir, char *file);
int checkIntegrity(char *file);
int executableChecks(char *path);
int pathIsExecutable(char *path);
int pathIsFile(char *path);
int pathIsFolder(char *path);
int pathIsLink(char *path);