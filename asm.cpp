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

long long int read_text(FILE* input_address, char** buffer)
{
    assert(input_address);

    struct stat statistics = {};
    long long int number_of_symbols = 0;

    if(fstat(fileno(input_address), &statistics) == -1)
        return 0;

    //printf("%ld\n", statistics.st_size);

    *buffer = (char*) calloc(statistics.st_size, sizeof(char));
    if (*buffer == NULL)
        return 0;

    number_of_symbols = fread(*buffer, sizeof(char), statistics.st_size, input_address);

    /*
    for (long long int i = 0; i < number_of_symbols; i++)
    {
        putchar((*buffer)[i]);
    }
    */
    return number_of_symbols;
}

bool make_mashine_code(FILE* output_address, char* buffer, long long int number_of_symbols)
{
    assert(output_address);
    assert(buffer);

    int start_number_of_command = 10;
    int start_number_of_labels = 10;
    int* mashine_code = (int*) calloc(start_number_of_command, sizeof(int));
    int* labels = (int*) calloc(start_number_of_labels, sizeof(int));
    if (labels == NULL || mashine_code == NULL)
        return 1;

    bool error = 0;

    error = make_code_massive(buffer, &mashine_code, &labels, &start_number_of_command, &start_number_of_labels, number_of_symbols);
    error = make_code_massive(buffer, &mashine_code, &labels, &start_number_of_command, &start_number_of_labels, number_of_symbols);

    if (error)
        return 1;

    for (int i = 0; i < mashine_code[0]+1; i++)
    {
        fprintf(output_address, "%d ", mashine_code[i]);
        //printf("%d ", mashine_code[i]);
        //getchar();
    }
    fprintf(output_address, "%d", -1);

    free(mashine_code);
    free(labels);

    return 0;
}

bool make_code_massive(char* buffer, int** mashine_code, int** labels, int* start_number_of_command,
                               int* start_number_of_labels, long long int number_of_symbols)
{
    assert(buffer);
    assert(mashine_code);
    assert(labels);
    assert(start_number_of_command);
    assert(start_number_of_labels);

    char command[COMMAND_SIZE] = {};
    int number_of_command = 1;
    int number_of_str = 0;
    int number_of_label = -1;
    bool error = false;
    char* buffer_end = buffer + number_of_symbols*sizeof(char);

    while(buffer < buffer_end)
    {
        if (sscanf(buffer,"%s", command) != 1)
            error = 1;

        //printf("cmd = '%s'\n", command);
        //getchar();
        if (massive_upgrade(mashine_code, start_number_of_command, number_of_command))
        {
            error = true;
            return error;
        }
        number_of_str++;

        if (command[0] == ';')
        {
            while (*buffer != '\n')
            {
                buffer += sizeof(char);
            }

            buffer += sizeof(char);
            /*printf("[%c] [%d]", *buffer, *buffer);
            getchar();
            */
            continue;
            number_of_command--;
        }
        else if (command[0] == ':')
        {
            number_of_command--;
            sscanf(command+1, "%d", &number_of_label);
            if (label_upgrade(labels, start_number_of_labels, number_of_label))
                error = 1;
            (*labels)[number_of_label] = number_of_command;
        }
        else if (strcmp(command, "HLT") == 0)
        {
            (*mashine_code)[number_of_command] = HLT;
        }
        else if (strcmp(command, "POP") == 0)
        {
            (*mashine_code)[number_of_command] = POP;
        }
        else if (strcmp(command, "PUSH") == 0)
        {
            error = do_push_code(command, mashine_code, &start_number_of_command, &number_of_command, &buffer);
        }
        else if (strcmp(command, "OUT") == 0)
        {
            (*mashine_code)[number_of_command] = OUT;
        }
        else if (strcmp(command, "ADD") == 0)
        {
            (*mashine_code)[number_of_command] = ADD;
        }
        else if (strcmp(command, "MUL") == 0)
        {
            (*mashine_code)[number_of_command] = MUL;
        }
        else if (strcmp(command, "SUB") == 0)
        {
            (*mashine_code)[number_of_command] = SUB;
        }
        else if (strcmp(command, "DIV") == 0)
        {
            (*mashine_code)[number_of_command] = DIV;
        }
        else if (strcmp(command, "IN") == 0)
        {
            (*mashine_code)[number_of_command] = IN;
        }
        else if (strcmp(command, "RET") == 0)
        {
            (*mashine_code)[number_of_command] = RET;
        }
        else if (strcmp(command, "SQRT") == 0)
        {
            (*mashine_code)[number_of_command] = SQRT;
        }
        else if (strcmp(command, "PAUSE") == 0)
        {
            (*mashine_code)[number_of_command] = PAUSE;
        }
        else if (strcmp(command, "POPR") == 0)
        {
            error = do_popr_code(command, mashine_code, &start_number_of_command, &number_of_command, &buffer);
        }
        else if (strcmp(command, "PUSHR") == 0)
        {
            error = do_pushr_code(command, mashine_code, &start_number_of_command, &number_of_command, &buffer);
        }
        else if (strcmp(command, "JB") == 0)
        {
            error = do_jb_code(command, mashine_code, &start_number_of_command, &number_of_command, &buffer,
                               labels, start_number_of_labels, number_of_label);
        }
        else if (strcmp(command, "JA") == 0)
        {
            error = do_ja_code(command, mashine_code, &start_number_of_command, &number_of_command, &buffer,
                               labels, start_number_of_labels, number_of_label);
        }
        else if (strcmp(command, "JAE") == 0)
        {
            error = do_jae_code(command, mashine_code, &start_number_of_command, &number_of_command, &buffer,
                               labels, start_number_of_labels, number_of_label);
        }
        else if (strcmp(command, "JE") == 0)
        {
            error = do_je_code(command, mashine_code, &start_number_of_command, &number_of_command, &buffer,
                               labels, start_number_of_labels, number_of_label);
        }
        else if (strcmp(command, "JNE") == 0)
        {
            error = do_jne_code(command, mashine_code, &start_number_of_command, &number_of_command, &buffer,
                               labels, start_number_of_labels, number_of_label);
        }
        else if (strcmp(command, "JMP") == 0)
        {
            error = do_jmp_code(command, mashine_code, &start_number_of_command, &number_of_command, &buffer,
                               labels, start_number_of_labels, number_of_label);
        }
        else if (strcmp(command, "CALL") == 0)
        {
            error = do_call_code(command, mashine_code, &start_number_of_command, &number_of_command, &buffer,
                                 labels, start_number_of_labels, number_of_label);
        }
        else if (strcmp(command, "POPM") == 0)
        {
            error = do_popm_code(command, mashine_code, &start_number_of_command, &number_of_command, &buffer);
        }
        else if (strcmp(command, "PUSHM") == 0)
        {
            error = do_pushm_code(command, mashine_code, &start_number_of_command, &number_of_command, &buffer);
        }
        else if (strcmp(command, "VIDEO") == 0)
        {
            (*mashine_code)[number_of_command] = VIDEO;
        }
        else
        {
            printf("stars.asm:%d: syntax_error\n", number_of_str);
            error = true;
        }

        number_of_command++;
        buffer += (strlen(command)+1)*sizeof(char);
    }

    number_of_command--;
    (*mashine_code)[number_of_command] = -1;
    (*mashine_code)[0] = number_of_command;

    return error;
}

bool massive_upgrade(int** mashine_code, int* start_number_of_command, int number_of_command)
{
    assert(mashine_code);
    assert(start_number_of_command);

    if (number_of_command >= *start_number_of_command)
    {
        *start_number_of_command *= 2;
        //printf("%p %d\n", *mashine_code, *start_number_of_command);
        *mashine_code = (int*) realloc(*mashine_code, (*start_number_of_command)*sizeof(int));
        if (*mashine_code == NULL)
        {
            free(*mashine_code);
            return 1;
        }
    }

    return 0;
}

bool label_upgrade(int** labels, int* start_number_of_labels, int number_of_label)
{
    assert(labels);
    assert(start_number_of_labels);

    if (number_of_label >= *start_number_of_labels)
    {
        *start_number_of_labels = number_of_label + 1;
        //printf("%p %d\n", *mashine_code, *start_number_of_command);
        *labels = (int*) realloc(*labels, (*start_number_of_labels)*sizeof(int));
        if (*labels == NULL)
            return 1;
    }

    return 0;
}
