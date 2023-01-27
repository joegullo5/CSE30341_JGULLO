#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {


    // Check the number of arguments passed. Prints error if less than 3 arguments are passed
    // also prints example of how to use the program
    if (argc < 3) {
        printf("Error: File name and byte value must be specified.\nExample: $ ./bytecheck Test.txt 0x20\n");
        return 1;
    }
    
    // Open the file specified in the user input
    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("Error: File does not exist\n");
        return 1;
    }

    // Get the size of the file
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    
    fseek(fp, 0, SEEK_SET);
    if (fileSize > 25000) {
        printf("Error: The file is over 25 kB (file size was %ld).\n", fileSize);
        fclose(fp);
        return 1;
    }

    return 0;
}

