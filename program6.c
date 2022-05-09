#include <stdio.h>
#include <pwd.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>

// **
// For the sixth system, modify the program from homework #4 part two (add timers)
// **

char * mypath;

//this is just the old PDC
void printDirectoryContents();

void iMDying() {
    //do nothing... say finished
    printf("Finished working with %s...\n", mypath);
}

int main(int argc, char * argv[]) {
    char * path = argv[argc-1];
    printDirectoryContents(path);

    //End the timer, i.e., record the ending time
    clock_t end = clock();
}

void printDirectoryContents(char * path) {

    //print my path!
    printf("%s\n",path);
    //initalize and set global
    mypath = malloc(sizeof(char)*255);
    //set contents
    //do not set mypath = path (pointers)
    strcpy(mypath,path);
    //set up atexit
    atexit(iMDying);
    //need children counter
    int counter = 0;

    //open the directory!
    DIR * dir = opendir(path);
    //read the directory
    struct dirent * dirEntry = readdir(dir);
    //read all the contents of the directory
    while (dirEntry != NULL) {
        //need the full path:
        char * fullName = malloc(255 * sizeof(char));
        strcat(fullName, path);
        //silly unix... need the slash on files, but not directories :)
        //if (dirEntry->d_type != DT_DIR) {
        strcat(fullName, "/");
        //} 
        strcat(fullName, dirEntry->d_name);
        
        //not truly necessary, but it cleans up our output
        if ((strcmp(dirEntry->d_name,".") != 0) && (strcmp(dirEntry->d_name,"..") != 0)) {
            //actually print the name
            printf("%s\n",fullName);
        } 
        
        //if the entry is a directory
        if (dirEntry->d_type == DT_DIR) {
            //make sure to skip . and ..
            if ((strcmp(dirEntry->d_name,".") != 0) && (strcmp(dirEntry->d_name,"..") != 0)) {
                //recurse
                pid_t pid = fork();
                if (pid == 0) {
                    //child
                    //recurse
                    printDirectoryContents(fullName);
                    //quit
                    exit(0);
                } else {
                    //parent
                    counter++;
                }
            }
        }      
        //go to next entry in the directory
        dirEntry = readdir(dir);
    }

    //need a status
    int status;
    //while counter is greater than zero
    if (counter > 0) {
        //wait for the next child to finish
        wait(&status);
        //when it finishes
        counter--; 
    }
}