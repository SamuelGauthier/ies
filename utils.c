/*-----------------------------------------------------------------------------
|
| - Author:       Samuel Gauthier
| - Date:         15.02.2016
| - File name:    utils.c
| - Description:  Error handeling file.
|
-----------------------------------------------------------------------------*/

#include "utils.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/*
|   Description:  Function that displays specific error messages in the terminal
|   Arguments:    The chosen error value
|   Return:       Nothing
*/

void error(int errorValue){

  switch(errorValue){

    case 1:
      printf("ERROR - Invalid file format!\n");
      break;
    case 2:
      printf("ERROR - Offset error!\n");
      break;
    case 3:
      printf("ERROR - Error reading pixels of image!\n");
      break;
    case 4:
      printf("ERROR - Cannot open the designated file\n");
      break;
    case 5:
      printf("ERROR - Too few arguments!\n");
      break;
    case 6:
      printf("ERROR - No memory\n");
      break;
    case 7:
      printf("ERROR - Error reading header\n");
      break;
    case 8:
      printf("ERROR - Image is too small!\n");
      break;
    case 9:
      printf("ERROR - Cannot delete designated file!\n");
      break;
    case 10:
      printf("ERROR - Cannot copy the file!\n");
      break;
    default:
      break;
  }

}

/*
|   Description:  Function to make a copy of a bmp file
|   Arguments:    old_filename, name of the old file
|                 new_filename, name of the new file
|   Return:       0 if successfull, ERR_CPY_FILE otherwise
*/

int copy_file(char *old_filename, char  *new_filename){

		FILE *ptr_old, *ptr_new;
		int a;
    int i;
    char answer;

    // Puts the new file name to lowercase
    for (i = 0; i < strlen(new_filename); i++) {

      new_filename[i] = tolower(new_filename[i]);
    }

    // Check if th user wants to overwrite the existing file
    if(strcmp(old_filename, new_filename) == 0) {

      printf("Do you want to overwrite the existing file? y/n ");
      scanf("%c", &answer);
      getchar(); // Eat the newline ignored by scanf, if not present messes up the next call to fgets.
      answer = tolower(answer);

      if(answer == 'n') return ERR_CPY_FILE;
      else if(answer == 'y') return 0;
    }

    // Check if the new file name terminates with ".bmp"
    if(strstr(new_filename, ".bmp") == NULL) return ERR_CPY_FILE;

    // Open the files
		if((ptr_old = fopen(old_filename, "rb")) == NULL){

      fclose(ptr_old);
      return ERR_CPY_FILE;
    }
		if((ptr_new = fopen(new_filename, "wb")) == NULL){

      fclose(ptr_new);
      return ERR_CPY_FILE;
    }

    // Copy the old file data in the new one
		while(1)
		{
			a = fgetc(ptr_old);

			if(!feof(ptr_old)) fputc(a, ptr_new);
			else break;
		}

		fclose(ptr_new);
		fclose(ptr_old);
		return  0;
	}

  /*
  |   Description:  Function to display help and usage of the program
  |   Arguments:    argv
  |   Return:       Nothing
  */

void printHelp(char **argv){

  printf("usage: %s <command>\n\n", argv[0]);
  printf("Available commands to use for the steganography executable:\n");
  printf("\t-e\tencode file \t=> %s -e <filename> <output>\n", argv[0]);
  printf("\t-d\tdecode file \t=> %s -d <filename>\n", argv[0]);
  printf("/!\\ only bmp file format is supported /!\\\n");

}
