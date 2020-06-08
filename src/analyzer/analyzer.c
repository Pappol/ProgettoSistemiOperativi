#include "analyzer.h"

BOOL isTxt(string file, int length){
    BOOL ret = FALSE;
    if(file[length-1] == '\n' && file[length - 5] == '.' && file[length -4] == 't' && file[length - 3] == 'x' && file[length - 2] == 't'){
        file[length-1] = '\0';
        return TRUE;
    }
    if (file[length - 4] == '.' && file[length - 3] == 't' && 
        file[length - 2] == 'x' && file[length - 1] == 't') {
        ret = TRUE;
    }  
    return ret;
}

BOOL isCpp(string file, int length){
    BOOL ret = FALSE;
    if(file[length-1] == '\n' && file[length - 5] == '.' && file[length -4] == 'c' && file[length - 3] == 'p' && file[length - 2] == 'p'){
        file[length-1] = '\0';
        return TRUE;
    }
    if (file[length - 4] == '.' && file[length - 3] == 'c' && 
        file[length - 2] == 'p' && file[length - 1] == 'p') {
        ret = TRUE;
    }  
    return ret;
}

BOOL isC(string file, int length){
    BOOL ret = FALSE;
    if(file[length-1] == '\n' && file[length - 3] == '.' && file[length - 2] == 'c'){
        file[length-1] = '\0';
        return TRUE;
    }
    if (file[length - 2] == '.' && file[length - 1] == 'c') {
        ret = TRUE;
    }  
    return ret;
}

BOOL isJava(string file, int length){
    BOOL ret = FALSE;
    if(file[length-1] == '\n' &&
        file[length - 6] == '.' && file[length - 5] == 'j' && 
        file[length - 4] == 'a' && file[length - 3] == 'v' &&
        file[length-2]=='a'){
            file[length-1] = '\0';
        return TRUE;
    }
    if (file[length - 5] == '.' && file[length - 4] == 'j' && 
        file[length - 3] == 'a' && file[length - 2] == 'v' &&
        file[length-1]=='a') {
        ret = TRUE;
    }  
    return ret;
}

BOOL isPy(string file, int length){
    BOOL ret = FALSE;
    if(file[length-1] == '\n' && file[length - 4] == '.' && file[length - 3] == 'p' && file[length - 2] == 'y'){
        file[length-1] = '\0';
        return TRUE;
    }
    if (file[length - 3] == '.' && 
        file[length - 2] == 'p' && 
        file[length - 1] == 'y') {

        ret = TRUE;
    }  
    return ret;
}

//funzioni per cartelle

node createNode() {
    node temp;
    temp = (node)malloc(sizeof(struct LinkedList));
    temp->next = NULL;
    return temp;
}

node addNode(node head, char *new_str) {
    char *dest;  // Create new str
    dest = malloc(sizeof(char) * (strlen(new_str) + 1));
    strcpy(dest, new_str);  // Copy string into a new string

    node temp, p;
    temp = createNode();

    temp->str = dest;  // Pointer to new string
    if (head == NULL) {
        head = temp;
    } else {
        p = head;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = temp;
    }
    return head;
}

// https://stackoverflow.com/a/13098645/7924557
int file_is_executable(char *path) {
    struct stat sb;
    return (stat(path, &sb) == 0 && sb.st_mode & S_IXUSR);
}

// https://stackoverflow.com/a/230070/7924557
int file_exists(char *path) {
    struct stat sb;
    return (stat(path, &sb) == 0);
}

// https://stackoverflow.com/a/4553053/7924557
int is_folder(char *path) {
    struct stat sb;
    if (stat(path, &sb) != 0)
        return 0;
    return S_ISDIR(sb.st_mode);
}

// https://stackoverflow.com/a/3985085/7924557
int is_link(char *path) {
    struct stat sb;
    int x;
    x = lstat(path, &sb);
    return S_ISLNK(sb.st_mode);
}

/*
 * Concatenate 3 strings in 1.
*/
char *concat(const char *s1, const char *s2, const char *s3) {
    char *result = malloc(strlen(s1) + strlen(s2) + strlen(s3) + 1);  // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    strcat(result, s3);
    return result;
}

/*
 * Ginven a path of a directory it returns all files inside it.
*/
node listFiles(char *path) {
    node files_list = createNode();  // Tail of the list
    node head = files_list;
    FILE *fp;

    char *command = concat("find ", path, " -type f");
    fp = popen(command, "r");  // Open pointer to command output
    if (fp == NULL) {
        printf("Failed to run command");
        return files_list;
    }

    char p[1000];  // Max lenght of the path
    while (fgets(p, sizeof(p), fp) != NULL) {
        head = addNode(head, p);
    }

    pclose(fp);               // Close pointer to command output
    return files_list->next;  // First node is NULL, so return second
}

BOOL containsDot(string str){
    int len = strlen(str);
    int i;
    BOOL ret = FALSE;
    for(i=len-1; len>=0 && str[i] != '/'; i--){
        if(str[i] == '.')
            ret = TRUE;
    }
    return ret;
}

//funzioni messaggi per aggiungere roba

void report_and_exit(const char* msg) {
  perror(msg);
  exit(-1); /* EXIT_FAILURE */
}

void sendConfirm(string messaggio, int projID){
    key_t key = ftok(PathName, projID);
    if (key < 0) {      
        printf("err: %s\n", strerror(errno));
        report_and_exit("couldn't get key...");
    }

    int qid = msgget(key, 0666 | IPC_CREAT);
    if (qid < 0) 
        report_and_exit("couldn't get queue id...");

    queuedMessage msg;
    strcpy(msg.payload, messaggio);
    msg.type = 1;

    msgsnd(qid, &msg, strlen(msg.payload)+1, MSG_NOERROR | IPC_NOWAIT);
}

string recConfirm(int projID){
    key_t key = ftok(PathName, projID);
    if (key < 0) {      
        printf("err: %s\n", strerror(errno));
        report_and_exit("couldn't get key...");
    }

    int qid = msgget(key, 0666 | IPC_CREAT);
    if (qid < 0) 
        report_and_exit("couldn't get queue id...");

    queuedMessage msg;
    if (msgrcv(qid, &msg, MAX_MSG_SIZE, 1, MSG_NOERROR) < 0)   puts("AAAAAAAAAAAAAAAAAAAAAAAA trouble...");

    if (msgctl(qid, IPC_RMID, NULL) < 0)  /* NULL = 'no flags' */
        report_and_exit("trouble removing queue...");

    string ret = malloc(strlen(msg.payload));
    strcpy(ret, msg.payload);

    return ret;
}
