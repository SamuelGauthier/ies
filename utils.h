/*-----------------------------------------------------------------------------
|
| - Author:       Samuel Gauthier
| - Date:         01.01.2016
| - File name:    utils.h
| - Description:  This file is used to define some error variables and
|                 functions.
|
-----------------------------------------------------------------------------*/

#ifndef ERRORS
#define ERRORS

#define ERR_FILE_FORMAT 1
#define ERR_OFFSET 2
#define ERR_PIXEL_READ 3
#define ERR_OPEN_FILE 4
#define ERR_ARGS 5
#define ERR_MEM 6
#define ERR_HEADER_READ 7
#define ERR_IMG_SIZE 8
#define ERR_DELETE 9
#define ERR_CPY_FILE 10

#endif

void error(int errorValue);
int copy_file(char *old_filename, char  *new_filename);
short encode(char **argv);
short decode(char **argv);
void printHelp(char **argv);
