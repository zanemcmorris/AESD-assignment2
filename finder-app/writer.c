/*
    This file was created by Zane McMorris on 1/21/26 for AESD
*/

/*
Instructions for this assignment are inherited from the requirements for Writer.sh

# Accepts the following arguments: the first argument is a full path to a file (including filename) on the filesystem, referred to below as writefile;
#  the second argument is a text string which will be written within this file, referred to below as writestr

# Exits with value 1 error and print statements if any of the arguments above were not specified
# Creates a new file with name and path writefile with content writestr, overwriting any existing file and creating the path if it doesn’t exist. Exits with value 1 and error print statement if the file could not be created.

Additionally,

One difference from the write.sh instructions in Assignment 1: 
 You do not need to make your "writer" utility create directories which do not exist.
   You can assume the directory is created by the caller.

Setup syslog logging for your utility using the LOG_USER facility.

Use the syslog capability to write a message “Writing <string> to <file>” where <string>
 is the text string written to file (second argument) and <file> is the file created by the script.
 This should be written with LOG_DEBUG level.

Use the syslog capability to log any unexpected errors with LOG_ERR level.

*/

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char *argv[]){
    // Arg0 = program name
    // arg1 = full-path-to-thing
    // arg2 = string to write to file

    openlog("writer.c", LOG_PERROR, LOG_USER);

    if (argc != 3){
        syslog(LOG_DEBUG, "%d arguments passed to writer.c. Expected exactly 2.\n", argc);
        return 1;
    }


    char *path = argv[1];
    char *textToWrite = argv[2];

    if (path == NULL || textToWrite == NULL){
        syslog(LOG_DEBUG, "Empty strings passed into writer.c");
        return 1;
    }

    if (path[0] != '/'){
        syslog(LOG_DEBUG, "Path passed was not absolute. Try again.\n");
        return 1;
    }

    // printf("Path to write to: %s\n", path);
    // printf("Text to write: %s\n", textToWrite);

    FILE *file = fopen(path, "w");
    if (file == NULL){
        // Could not open file - use errno to capture the reason
        syslog(LOG_DEBUG, "Could not open file at specified path with reason: %s\n", strerror(errno));
        return 1;
    }

    int charsWritten = fprintf(file, "%s\n", textToWrite);

    // printf("Wrote %d chars to file.\n", charsWritten);
    if (charsWritten == 0){
        syslog(LOG_DEBUG, "Error. Wrote 0 chars to file.\n");
        return 1;
    }

    syslog(LOG_DEBUG, "Writing %s to %s\n", textToWrite, path);

    fclose(file);

    return 0;
}