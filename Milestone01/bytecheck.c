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
    

    // Open the file specified in the user input in binary mode
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

    // Get the byte value from the command line argument
    int input_byte = (int)strtol(argv[2], NULL, 0);

    // Error checking statement to make sure strtol is reading in the byte value and casting it as an integer correctly
    // printf("%d\n",input_byte);

    //checks to make sure the byte length is exactly 4 characters long ()
    if(strlen(argv[2]) != 4){
        printf("Error: Invalid hexadecimal value. It should be 4 characters long.\n Example inputs: 0x20, 0x41, 0x05... \n");
        return 1;
    }
    //checks to make sure the first two characters of the byte input are 0x, 
    //and that the value stored in input_byte is not 0 (if the user inputs 0xGG, a 0 is stored in input byte)
    if(argv[2][0] != 0 || argv[2][1] != 'x' || input_byte == 0){
        printf("Error: Invalid hexadecimal value.\nExample inputs: 0x20, 0x41, 0x05... \n");
        return 1;
    }




    // Close the file
    fclose(fp);

    return 0;
}

