/*-----------------------------------------------------------------------------
|
| - Author:       Samuel Gauthier
| - Date:         01.01.2016
| - File name:    ies.c
| - Description:  Small C program (Input Encryption System) to hide data
|                 in a 24-bit bitmap image.
|
-----------------------------------------------------------------------------*/

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){

  if(argc < 3){

    printHelp(argv);
    return EXIT_FAILURE;
  }

  if(strcmp(argv[1], "-e") == 0 && argc == 4){

    // Copy the old file, if an error occurs, abord.
    if(copy_file(argv[2], argv[3]) == ERR_CPY_FILE){

      error(ERR_CPY_FILE);
      return 1;
    }
    return encode(argv) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
  }
  else if(strcmp(argv[1], "-d") == 0 && argc == 3){
    return decode(argv) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
  }
  else{

    printHelp(argv);
    return EXIT_FAILURE;
  }

}

/*
|   Description:  Function to encode text into an bmp image
|   Arguments:    **argv, array of initial arguments from user
|   Return:       0 if successfull, otherwise >0
*/
short encode(char **argv){

  FILE *filePointer;

  filePointer = fopen(argv[2], "r+b");

  // Check if the file exists, prints an error if it doesn't
  if(filePointer == NULL){

    error(ERR_OPEN_FILE);
    fclose(filePointer);
    return 1;
  }
  else {

    // Read the header of the BMP file, to get all the useful infos (length, width, offset, etc)
    unsigned short fileCheck[13];
    if(fread(fileCheck, sizeof(short), 13, filePointer) != 13){

      error(7);
      fclose(filePointer);
      return 1;
    }
    unsigned int width = fileCheck[9];
    unsigned int height = fileCheck[11];
    unsigned int offset = fileCheck[5];

    // Check if the image is a BITMAP image (0x4d42 [Little Endian] = BM)
    if(fileCheck[0] != 0x4D42){

      error(ERR_FILE_FORMAT);
      fclose(filePointer);
      return 1;
    }
    // Go to the offset
    else if(fseek(filePointer, offset, SEEK_SET) != 0){

      error(ERR_OFFSET);
      fclose(filePointer);
      return 1;
    }
    else{

      unsigned int rgbPixelNumber = 3 * (width * height); // Multiply by 3 because of the 24-bit file fomrat (it has no alpha channel, only RGB)
      unsigned int maxInput = (rgbPixelNumber / 4) - 1; // The maximum characters a user can input, leaving the last for the 0 (end of message).

      // Check if the maxInput equals -1 which means the image size is 1x1
      if(maxInput == -1){

        if(remove(argv[2]) != 0){

          error(ERR_DELETE);
          fclose(filePointer);
          return 1;
        }

        error(ERR_IMG_SIZE);
        fclose(filePointer);
        return 1;
      }

      // Create an array of characters with malloc of the size of the max input. Produces a segfault if the array is created with braces "inputText[maxInput]"
      char* inputText = malloc(maxInput);
      printf("Type your phrase: ");
      if(fgets(inputText, maxInput, stdin) == NULL){

        error(ERR_MEM);
        fclose(filePointer);
        free(inputText);
        return 1;
      }

      // Store the pixels into an array of char (char = 1 byte)
      char* pixelData = malloc(rgbPixelNumber);
      unsigned int nbrPixelRead = fread(pixelData, sizeof(char), rgbPixelNumber, filePointer);

      // Check if all the pixels have been read
      if(nbrPixelRead != rgbPixelNumber){

        error(ERR_PIXEL_READ);
        fclose(filePointer);
        free(pixelData);
        free(inputText);
        return 1;
      }
      else{
        int i;
        int j = 0;
        int k = 0;
        unsigned char buffer = 0;

        fseek(filePointer, offset, SEEK_SET);

        for (i = 0; i < (4 * maxInput); i++) {

          // Keep only the higher 6 bits of the color byte, add the 2 bits of the text (which have been shifted to the right accordingly then ANDED to keep only the 2 lowest bits). For 1 byte of text we need 4 bytes of color.
          buffer = (pixelData[i] & 0xFC) | ((inputText[k] >> (0x6 - (j * 2))) & 0x3);
          fprintf(filePointer, "%c", buffer);
          if(j == 3){

            // If we arrive at the end of the text designated by 4 zeros, stop encoding.
            if(inputText[k] == 0x0) break;
            else{
              k++;
              j = 0;
            }
          }
          else j++;
        }

        fclose(filePointer);
        free(pixelData);
        free(inputText);
        return 0;
        }
      }
  }
}

/*
|   Description:  Function to decode text from an bmp image
|   Arguments:    **argv, array of initial arguments from user
|   Return:       0 if successfull, otherwise >0
*/
short decode(char **argv){

  FILE *filePointer;

  filePointer = fopen(argv[2], "r");

  // Check if the file exists, prints an error if it doesn't
  if(filePointer == NULL){
    error(ERR_OPEN_FILE);
    fclose(filePointer);
    return EXIT_FAILURE;
  }
  else {

    // Read the header of the BMP file, to get all the useful infos (length, width, offset, etc)
    unsigned short fileCheck[13];
    if(fread(fileCheck, sizeof(short), 13, filePointer) != 13){

      error(7);
      fclose(filePointer);
      return 1;
    }
    unsigned int width = fileCheck[9];
    unsigned int height = fileCheck[11];
    unsigned int offset = fileCheck[5];

    // Check if the image is a BITMAP image (0x4d42 [Little Endian] = BM)
    if(fileCheck[0] != 0x4D42){

      error(ERR_FILE_FORMAT);
      fclose(filePointer);
      return EXIT_FAILURE;
    }

    // Check if there was an error when trying to move the file pointer to the offset
    else if(fseek(filePointer, offset, SEEK_SET) != 0){

      error(ERR_OFFSET);
      fclose(filePointer);
      return EXIT_FAILURE;
    }

    else{

      unsigned int rgbPixelNumber = 3 * (width * height); // Multiply by 3 because of the 24-bit file fomrat (it has no alpha channel, only RGB)
      char* pixelData = malloc(rgbPixelNumber);
      unsigned int nbrPixelRead = fread(pixelData, sizeof(char), rgbPixelNumber, filePointer);

      // Check if the amount of pixel in the file corresponds to the read ones.
      if(nbrPixelRead != rgbPixelNumber){

        error(ERR_PIXEL_READ);
        fclose(filePointer);
        free(pixelData);
        return EXIT_FAILURE;
      }
      else{
        int i;
        int j = 0;
        unsigned char buffer = 0;

        printf("The phrase was: ");

        for(i = 0; i < rgbPixelNumber; i++){

          // Take a color pixel, keep the lowest 2 bits (0x3), shift to the left by an offset: 0x6 minus 2 times the separate counter j.
          buffer |= (pixelData[i] & 0x3) << (0x6 - (j * 0x2));

          // Print the buffer out when it is full, and if the buffer equals 0 stop the loop.
          if(j == 3){

            if(buffer == 0) break;
            else{
              printf("%c", buffer);
              j = 0;
              buffer = 0;
            }
          }
          else j++;
        }

        fclose(filePointer);
        free(pixelData);
        return 0;
      }
    }
  }
}
