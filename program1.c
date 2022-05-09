#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <ctype.h>
#include <dirent.h>

//need a frequency
int frequency[26] = {0};

void processFile(char * fn) {
    //whole process file
    printf("Processing File: %s\n",fn);

    //open the file
    FILE * stream = fopen(fn ,"rb");

    char c = 0;
    //read the file char by char
    //will stop when C == EOF
    while ((c = fgetc(stream)) != EOF) {
        if (isalpha(c)) {
            //it's an alpha character (a-z)
            c = tolower(c);
            //make darn sure it's a-z
            //stoopid alt char sets
            if (c >= 'a' && c <= 'z') {
                //increment frequency
                frequency[c - 'a']++;
            }
        }
    }

    //close the file
    fclose(stream);


}

void processDirectory(char * fn) {
    //identify directory
    printf("Working with directory: %s\n",fn);
    //open directory
    DIR * dir = opendir(fn);
    //need a directory entry pointer
    struct dirent * dentry;
    //actually read an entry
    dentry = readdir(dir);
    //loop until dentry is null (no more directory entries)
    while (dentry != NULL) {
        char buffer[1000] = {0};
        strcat(buffer, fn);
        strcat(buffer, "/");
        strcat(buffer,dentry->d_name);
        //printf("%s\n",buffer);
        if (dentry->d_type == DT_DIR) {
            if ((strcmp(dentry->d_name,".") != 0) && (strcmp(dentry->d_name,"..") != 0)) {
                processDirectory(buffer);
            }
            
        } else if (dentry->d_type == DT_REG) {
            processFile(buffer);
        }
        
        //actually read an entry
        dentry = readdir(dir);
    }
    //close directory
    closedir(dir);


}


int main() {
    //need a buffer to read
    char buffer[256];
    //get working direct (CWD)
    char * fn = getcwd(buffer, 256);

    //call method
    processDirectory(fn);

    //End the timer, i.e., record the ending time
    clock_t end = clock();

    //output the frequency
    for (int i=0; i<26; i++) {
        printf("%c: %d\n", ('a' + i), frequency[i]);
    }

}