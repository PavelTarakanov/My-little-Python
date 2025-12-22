#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "stack.h"
#include "processor.h"

Stack_Error_Code processor_init(processor_t* processor, const unsigned int capacity)
{
    assert(processor);

    if (stack_init(&processor->stk, capacity))
        return ALLOCATION_ERROR;

    if (stack_init(&processor->ret_stk, capacity))
        return ALLOCATION_ERROR;

    processor->regs = (int*) calloc(NUMBER_OF_REGS, sizeof(int));

    if (processor->regs == NULL)
        return ALLOCATION_ERROR;

    processor->instruction_pointer = 0;

    processor->RAM = (int*) calloc(RAM_SIZE, sizeof(int));

    if (processor->RAM == NULL)
        return ALLOCATION_ERROR;

    return NO_ERROR;
}

void processor_dump(processor_t* processor, int number_of_commands)
{
    printf("processor[%p]\n"
           "programm:\n", processor);
    for (int i = 0; i < number_of_commands; i++)
    {
        printf("    [%d] = %d", i, processor->programm[i]);
        if (i == processor->instruction_pointer)
            printf("<--");
        printf("\n");
    }
    printf("regs:\n");
    for (int i = 0; i < 16; i++)
    {
        printf("    regs[%d] = %d\n", i, processor->regs[i]);
    }
    stack_dump(&processor->stk);

    video_memory(processor->RAM);
}

bool read_programm(FILE* input_address, int** programm, int* number_of_commands)
{
    assert(input_address);
    assert(programm);
    assert(number_of_commands);

    int command = -2;

    if (fscanf(input_address, "%d", number_of_commands) != 1)
        return true;

    *programm = (int*) calloc(*number_of_commands, sizeof(int));

    if (*programm == NULL)
    {
        return true;
    }

    for (int i = 0; i < *number_of_commands; i++)
    {
        if (fscanf(input_address, "%d", &command) != 1)
            return true;
        (*programm)[i] = command;
    }

    return false;
}

void calculator(processor_t* processor)
{
    assert(processor);

    int command = processor->programm[processor->instruction_pointer];

    while (command)
    {
        if (do_user_command(command, processor))
            break;

        if (stack_verify(&processor->stk))
        {
            printf("ERROR!\nStack_Error_Code - %d\n", stack_verify(&processor->stk));
            break;
        }
        processor->instruction_pointer++;
        command = processor->programm[processor->instruction_pointer];
    }

    return;
}

bool do_user_command(int command, processor_t* processor)
{
    assert(processor);

    int value = 0;
    int reg_number = -1;
    int elem_1 = 0;
    int elem_2 = 0;
    int cell_number = -1;

    //printf("command code = %d", command);
    //stack_dump(&processor->stk);
    //printf("command = %d\n", command);

    //getchar();
    switch(command)
    {
        case HLT:
            return 1;
        case POP:
            stack_pop(&processor->stk, &value);

            return 0;
        case PUSH:
            (processor->instruction_pointer)++;
            value = processor->programm[processor->instruction_pointer];

            stack_push(&processor->stk, value);

            return 0;
        case OUT:
            stack_pop(&processor->stk, &value);

            printf("%d\n", value);

            return 0;
        case ADD:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);
            stack_push(&processor->stk, elem_1+elem_2);

            return 0;
        case MUL:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);
            stack_push(&processor->stk, elem_1*elem_2);

            return 0;
        case SUB:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);
            stack_push(&processor->stk, elem_2-elem_1);

            return 0;
        case DIV:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);
            if (elem_1 == 0)
            {
                printf("You can't divide by zero!\n");
                return 1;
            }
            stack_push(&processor->stk, elem_2/elem_1);

            return 0;
        case IN:
            while (scanf("%d", &value) != 1)
                printf("Input number!!!\n");

            stack_push(&processor->stk, value);

            return 0;
        case CALL:
            (processor->instruction_pointer)++;
            stack_push(&processor->ret_stk, processor->instruction_pointer);

            processor->instruction_pointer = processor->programm[processor->instruction_pointer] - 1;

            return 0;
        case RET:
            stack_pop(&processor->ret_stk, &processor->instruction_pointer);

            return 0;
        case SQRT:
            stack_pop(&processor->stk, &elem_1);
            stack_push(&processor->stk, (int) sqrt(elem_1));

            return 0;
        case POPR:
            stack_pop(&processor->stk, &value);

            (processor->instruction_pointer)++;
            reg_number = processor->programm[processor->instruction_pointer];
            if (reg_number > 15 || reg_number < 0)
            {
                printf("ERROR: Incorrect regicter!");
                return 1;
            }

            (processor->regs)[reg_number] = value;
            //printf("POPR %d", value);

            return 0;
        case POPM:
            stack_pop(&processor->stk, &value);

            (processor->instruction_pointer)++;

            cell_number = processor->regs[processor->programm[processor->instruction_pointer]];

            processor->RAM[cell_number] = value;

            return 0;
        case PUSHR:
            (processor->instruction_pointer)++;
            reg_number = processor->programm[processor->instruction_pointer];

            if (reg_number > 15 || reg_number < 0)
            {
                printf("ERROR: Incorrect regicter!");
                return 1;
            }
            stack_push(&processor->stk, (processor->regs)[reg_number]);

            return 0;
        case PUSHM:
            (processor->instruction_pointer)++;
            cell_number = processor->programm[processor->instruction_pointer];

            stack_push(&processor->stk, processor->RAM[processor->regs[cell_number]]);

            return 0;
        case JB:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);

            (processor->instruction_pointer)++;
            if (elem_1 > elem_2)
                processor->instruction_pointer = processor->programm[processor->instruction_pointer] - 1;

            return 0;
        case JBE:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);

            (processor->instruction_pointer)++;
            if (elem_1 >= elem_2)
                processor->instruction_pointer = processor->programm[processor->instruction_pointer] - 1;

            return 0;
        case JA:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);

            (processor->instruction_pointer)++;
            if (elem_1 < elem_2)
                processor->instruction_pointer = processor->programm[processor->instruction_pointer] - 1;

            return 0;
        case JAE:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);

            (processor->instruction_pointer)++;
            if (elem_1 <= elem_2)
                processor->instruction_pointer = processor->programm[processor->instruction_pointer] - 1;

            return 0;
        case JE:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);

            (processor->instruction_pointer)++;
            if (elem_1 == elem_2)
                processor->instruction_pointer = processor->programm[processor->instruction_pointer] - 1;

            return 0;
        case JNE:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);

            (processor->instruction_pointer)++;
            if (elem_1 != elem_2)
                processor->instruction_pointer = processor->programm[processor->instruction_pointer] - 1;

            return 0;
        case JMP:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);

            (processor->instruction_pointer)++;

            processor->instruction_pointer = processor->programm[processor->instruction_pointer] - 1;

            return 0;
        case VIDEO:
            video_memory(processor->RAM);
            return 0;
        case PAUSE:
            struct timespec ts;
            ts.tv_sec = 1;
            ts.tv_nsec = 0;

            nanosleep(&ts, NULL);

            return 0;
        default:
            printf("Print real command!\n");
            return 1;
    }
}

int my_strcmp(const char* str_1, const char* str_2)
{
    assert(str_1);
    assert(str_2);

    int i = 0;
    int j = 0;

    while (str_1[i] != '\n' && str_2[j] != '\n' && str_1[i] != '\0' && str_2[i] != '\0')
    {
        if (str_1[i] > str_2[j])
        {
            return 1;
        }
        else if(str_1[i] < str_2[j])
        {
            return -1;
        }

        i++;
        j++;
    }

    return 0;
}

void cleaner(processor_t* processor)
{
    assert(processor);

    free(processor->stk.data);
    free(processor->ret_stk.data);
    free(processor->regs);
    free(processor->programm);
    free(processor->RAM);
}

void video_memory(int* RAM)
{
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 10000000;

    nanosleep(&ts, NULL);

    for (int i = 0; i < 1600; i++)
    {
        if (i % 40 < 39)
            printf("%c   ", RAM[i]);
        if (i % 40 == 39)
            printf("\n\n");
    }

    return;
}
