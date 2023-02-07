/*
    Author - Joseph Gullo
    Course CSE 30341
    Date: 2/3/2023
    Description: 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


// Define the default maximum size of the input file
#define DEFAULT_MAX_SIZE 25000

void help();

int main(int argc, char *argv[]) {
  //Check To make sure an input file was specified
  if(argc < 2)
  {
    help();
    return 0;
  }

  //Define variables to store filenames and flags
  char out_file_name[100];
  char in_file_name[100];
  long max_size = DEFAULT_MAX_SIZE;
  int reverse_bytes = 0;
  int flip_bits = 1;
  bool o_flag = false; //rename flag

  //cycles through cmd line arguments, verifies inputs, and sets variables
  for (int i = 1; i < argc; i++) {

    //triggers help flag
    if (strcmp(argv[i], "-help") == 0) 
    {
        help();
        return 0;

    } else if (strcmp(argv[i], "-o") == 0) //triggers manual file rename
    {

        o_flag = true; //sets rename flag to true for later use
        strcpy(out_file_name, argv[++i]); //sets the output file name

    } else if (strcmp(argv[i], "-maxsize") == 0) 
    {
        if (!isdigit(*argv[i+1])) //Ensures Max size input by the user is a numeric value
        {
          printf("Error: Max Size must be a number.\n");
          return 1;
        }

        max_size = atoi(argv[++i]);

        if (max_size > 250000) //Max Size is limited to 250kb
        {
          printf("Error: Max size must be less than 250kb\n");
        }

    } else if (strcmp(argv[i], "-bfr") == 0) // Toggles bit flip and reverse
    {
        if (flip_bits==0)
        {
          printf("The -bfr and -r flags cannot both be used\n");
          return 1;
        }
        reverse_bytes = 1;
        flip_bits = 1;

    } else if (strcmp(argv[i], "-r") == 0)// Toggles reverse on, and bit flip off
    {
        if (reverse_bytes == 1)
        {
          printf("The -bfr and -r flags cannot both be used\n");
          return 1;
        }
        flip_bits = 0;
        reverse_bytes = 1;
    } else //sets the input file name
    {
      strcpy(in_file_name, argv[i]);
    }
  }

  // If the name was not manually changed, these lines set the extensions
  if(o_flag==false)
  { 
    if((flip_bits & reverse_bytes) == 1) // -bfr flag
    {
      strcpy(out_file_name, in_file_name);
      strcat(out_file_name, ".bfr");
    }
    else if ((flip_bits == 0) & (reverse_bytes == 1))// -r flag
    {
      strcpy(out_file_name, in_file_name);
      strcat(out_file_name, ".r");
    }
    else // Default case
    {
      strcpy(out_file_name, in_file_name);
      strcat(out_file_name, ".bf");
    }
  }

  // Opens up file in read binaray mode
  FILE *ifp = fopen(in_file_name, "rb");

  //Ensures file properly opened
  if (ifp == NULL) {
      printf("Error: The file %s does not exist or is not accessible.\n", in_file_name);
      return 1;
  }

  // Get the size of the input file
  fseek(ifp, 0, SEEK_END);
  long fileSize = ftell(ifp);


  // Sets the file pointer back to the beginning
  fseek(ifp, 0, SEEK_SET);

  //checks to make sure the file size is correct
  if (fileSize > max_size) {
      printf("Error: Input file %s was too large (more than %ld bytes)! \n", in_file_name, max_size);
      fclose(ifp);
      return 1;
  }

  // Open the file in Binary mode to ensure it doesn't exist.
  //If it doesn't, reopen it in write binary mode
  FILE* ofp = fopen(out_file_name, "rb");
  if (ofp) {
      printf("Error: %s already exists\n", out_file_name);
      fclose(ofp);
      return 1;
  } else {
      ofp = fopen(out_file_name, "wb");
  }
  
  int buffer_size = 1024;
  unsigned char buffer[buffer_size];
  int bytesRead;
  
  // Sets the file pointer back to the beginning
  fseek(ifp, 0, SEEK_SET);

  char byte;
  
  // If the reverse flag is set, this section reads the input file started from the end and appends each byte to the beginning of the otput file
  if(reverse_bytes==1)
  {
    for (long i = fileSize - 1; i >= 0; i--) {
      fseek(ifp, i, SEEK_SET); // Sets the file pointer at the correct location
      byte = fgetc(ifp);
      if(flip_bits==1)// If the flip bit flag is also set, this flips the bits before 
      {
        byte ^= 0xFF;
      }
      fputc(byte, ofp);// Adds the character to the file
    }
  }


  // Read the contents of the input file into the buffer
  if( (flip_bits == 1) & (reverse_bytes == 0))
    {
      while ((bytesRead = fread(buffer, 1, buffer_size, ifp)) > 0) {
          // Flip each byte in the buffer
          for (int i = 0; i < bytesRead; i++) {
              buffer[i] ^= 0xFF;
          }
          
          // Write the flipped buffer to the output file
          fwrite(buffer, 1, bytesRead, ofp);
      }
  }
  
  
  // Close the input and output files
  fclose(ifp);
  fclose(ofp);
  printf("Input: %s was %ld bytes\n",in_file_name,fileSize);
  printf("Output: %s was output successfully\n", out_file_name);
  return 0;
}




void help()
{
    printf("Usage: ./bitflip <input file> <optional arguments>\n");
    printf("-help: The help / usage for the executable\n");
    printf("-o: Override of the output file name\n");
    printf("-maxsize: Override of the maximum file size allowed for input (Enter size in bytes)\n");
    printf("-bfr: Reverse the order of the bytes and bit-flip the bits in the file. May not be used with -r.\n");
    printf("-r: Reverse the order of the bytes in the file without doing a bit-flip. May not be used with -bfr. \n");
}