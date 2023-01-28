/*
    Author - Joseph Gullo
    Course CSE 30341
    Date: 1/27/2023
    Description: Opens a file of up to 25kB and conts the number of times a given byte value occurs in the file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {


    // Check the number of arguments passed. Prints error if less than 3 arguments are passed
    // also prints example of how to use the program
    if (argc != 3) {
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

    // Sets the file pointer back to the beginning
    fseek(fp, 0, SEEK_SET);

    //checks to make sure the file size is correct
    if (fileSize > 25000) {
        printf("Error: The file is over 25 kB (file size was %ld).\n", fileSize);
        fclose(fp);
        return 1;
    }

    // Get the byte value from the command line argument
    int input_byte = (int)strtol(argv[2], NULL, 0);

    // Error checking statement to make sure strtol is reading in the byte value and casting it as an integer correctly
    // printf("%d\n",input_byte);

    // Checks to make sure the byte length is exactly 4 characters long ()
    if(strlen(argv[2]) != 4){
        printf("Error: Invalid hexadecimal value. It should be 4 characters long.\n Example inputs: 0x20, 0x41, 0x05... \n");
        return 1;
    }

    // Checks to make sure the first two characters of the byte input are 0x, 
    // And that the value stored in input_byte is not 0 (if the user inputs  an inalid hex number like 0xGG, a 0 is stored in input byte)
    if(argv[2][0] != '0' || argv[2][1] != 'x' || input_byte == 0){
        printf("Error: Invalid hexadecimal value.\nExample inputs: 0x20, 0x41, 0x05... \n");
        return 1;
    }

    // Allocates a buffer to read data into
    const int buff_size = 1024;
    char buff[buff_size];

    // Counter variable for the number of bytes we read in
    size_t bytes_read;

    // Counter variable for the number of times the desired ascii code occurs
    int count = 0;

    while((bytes_read = fread(buff, sizeof(char), buff_size, fp)) > 0) {
        //printf("%ld\n", bytes_read);
        // Iterate through the bufer and count how many times the input byte value occurs 
        for (int i = 0; i < bytes_read; i++) {

            //Checks to see if the value being checked in the bufferbuffer 
            if (buff[i] == input_byte) {
                //printf("%d\n", buff[i]);
                count++;
            }
        }
    }

    // Prints the number of times the byte value occurs in the file
    printf("The byte value 0x%x occurs %d times in %s\n", input_byte, count, argv[1]);

    // Closes the file
    fclose(fp);
    
    return 0;
}

