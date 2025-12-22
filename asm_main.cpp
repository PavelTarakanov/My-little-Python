#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "stack.h"
#include "asm.h"

static bool check_file_founded(int argc, int number_of_files);

const int NUMBER_OF_FILES = 2;

int main(int argc, char* argv[])
{
    FILE* input_address = NULL;
    FILE* output_address = NULL;
    char* buffer = NULL;
    long long int number_of_symbols = 0;

    if (check_file_founded(argc, NUMBER_OF_FILES))
        return NO_FILE_ERROR;
    if (check_file_opening(argv[1], &input_address, "r"))
        return FILE_OPENING_ERROR;
    if (check_file_opening(argv[2], &output_address, "w"))
        return FILE_OPENING_ERROR;

    number_of_symbols = read_text(input_address, &buffer);

    if (!number_of_symbols)
    {
        printf("Error while reading from file\n");
        return 0;
    }

    if (make_mashine_code(output_address, buffer, number_of_symbols))
        return MAKING_MASHINE_CODE_ERROR;

    if (check_file_closing(input_address))
        return FILE_CLOSING_ERROR;
    if (check_file_closing(output_address))
        return FILE_CLOSING_ERROR;

    return 0;
}

bool check_file_founded(int argc, int number_of_files)
{
    if (argc < number_of_files)
    {
        fprintf(stderr, "Files not founded. Please, give programm file with asm code\n");
        return 1;
    }

    return 0;
}
