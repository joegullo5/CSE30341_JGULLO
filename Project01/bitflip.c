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

// Define the default maximum size of the input file ()
#define DEFAULT_MAX_SIZE 25000

void help();

int main(int argc, char *argv[]) {
    if(argc < 2)
    {
      help();
      return 0;
    }
    char out_file_name[100];
    char in_file_name[100];
    int max_size = DEFAULT_MAX_SIZE;
    int reverse_bytes = 0;
    int flip_bits = 1;
    bool o_flag = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-help") == 0) {
            help();
            return 0;
        } else if (strcmp(argv[i], "-o") == 0) {
            o_flag = true;
            strcpy(out_file_name, argv[++i]);
        } else if (strcmp(argv[i], "-maxsize") == 0) {
            max_size = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-bfr") == 0) {
            if (flip_bits==0)
            {
              printf("The -bfr and -r flags cannot both be used\n");
              return 1;
            }
            reverse_bytes = 1;
            flip_bits = 1;
        } else if (strcmp(argv[i], "-r") == 0) {
            if (reverse_bytes == 1)
            {
              printf("The -bfr and -r flags cannot both be used\n");
              return 1;
            }
            flip_bits = 0;
            reverse_bytes = 1;
        } else {
          strcpy(in_file_name, argv[i]);
        }
    }


    if(o_flag==false)
    { 
      if((flip_bits & reverse_bytes) == 1)
      {
        strcpy(out_file_name, in_file_name);
        strcat(out_file_name, ".bfr");
      }
      else if ((flip_bits == 0) & (reverse_bytes == 1))
      {
        strcpy(out_file_name, in_file_name);
        strcat(out_file_name, ".r");
      }
      else
      {
        strcpy(out_file_name, in_file_name);
        strcat(out_file_name, ".bf");
      }
    }

    FILE *ifp = fopen(in_file_name, "rb");
    if (ifp == NULL) {
        printf("Error: The file %s does not exist or is not accessible.\n", in_file_name);
        return 1;
    }

    // Get the size of the file
    fseek(ifp, 0, SEEK_END);
    long fileSize = ftell(ifp);


    // Sets the file pointer back to the beginning
    fseek(ifp, 0, SEEK_SET);

    //checks to make sure the file size is correct
    if (fileSize > max_size) {
        printf("Error: Input file %s was too large (more than %ld bytes)! \n", in_file_name, fileSize);
        fclose(ifp);
        return 1;
    }

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
    
    ofp = fopen(out_file_name, "wb");
    
    // Sets the file pointer back to the beginning
    fseek(ifp, 0, SEEK_SET);

    char byte;
    if(reverse_bytes==1)
    {
      for (long i = fileSize - 1; i >= 0; i--) {
        fseek(ifp, i, SEEK_SET);
        byte = fgetc(ifp);
        if(flip_bits==1)
        {
          byte ^= 0xFF;
        }
        fputc(byte, ofp);
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