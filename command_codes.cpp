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

bool do_push_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer)
{
    int value = 0;

    *buffer += (strlen(command)+1)*sizeof(char);
    (*mashine_code)[*number_of_command] = PUSH;

    (*number_of_command)++;
    if (massive_upgrade(mashine_code, *start_number_of_command, *number_of_command))
        return true;

    if (sscanf(*buffer, "%d", &value) != 1)
        return true;
    if (sscanf(*buffer, "%s", command) != 1)
        return true;

    (*mashine_code)[*number_of_command] = value;

    return false;
}

bool do_popm_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer)
{
    int value = 0;

    *buffer += (strlen(command)+1)*sizeof(char);
    (*mashine_code)[*number_of_command] = POPM;

    (*number_of_command)++;
    if (massive_upgrade(mashine_code, *start_number_of_command, *number_of_command))
        return true;

    if(sscanf(*buffer, "%s", command) != 1)
        return true;


    if (command[0] != '[' || command[strlen(command)-1] != ']')
    {
        printf("stars.asm:%d: syntax_error\n", *number_of_command);
        return true;
    }

    value = reg_number(command);
    if (value == UNKNOWN_REG)
    {
        printf("stars.asm:%d: syntax_error\n", *number_of_command);
        return true;
    }

    (*mashine_code)[*number_of_command] = value;

    return false;
}

bool do_pushm_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer)
{
    int value = 0;

    *buffer += (strlen(command)+1)*sizeof(char);
    (*mashine_code)[*number_of_command] = PUSHM;

    (*number_of_command)++;
    if (massive_upgrade(mashine_code, *start_number_of_command, *number_of_command))
        return true;

    if(sscanf(*buffer, "%s", command) != 1)
        return true;

    if (command[0] != '[' || command[strlen(command)-1] != ']')
    {
        printf("stars.asm:%d: syntax_error\n", *number_of_command);
        return true;
    }

    value = reg_number(command);
    if (value == UNKNOWN_REG)
    {
        printf("stars.asm:%d: syntax_error\n", *number_of_command);
        return true;
    }
    (*mashine_code)[*number_of_command] = value;

    return false;
}

bool do_pushr_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer)
{
    int value = 0;

    *buffer += (strlen(command)+1)*sizeof(char);
    (*mashine_code)[*number_of_command] = PUSHR;

    (*number_of_command)++;
    if (massive_upgrade(mashine_code, *start_number_of_command, *number_of_command))
        return true;

    if (sscanf(*buffer, "%s", command) != 1)
        return true;

    value = reg_number(command);
    if (value == UNKNOWN_REG)
    {
        printf("stars.asm:%d: syntax_error\n", *number_of_command);
        return true;
    }

    (*mashine_code)[*number_of_command] = value;

    return false;
}

bool do_jb_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label)
{
    int value = 0;

    *buffer += (strlen(command)+1)*sizeof(char);
    //printf("Write 1\n");
    (*mashine_code)[*number_of_command] = JB;

    (*number_of_command)++;
    if (massive_upgrade(mashine_code, *start_number_of_command, *number_of_command))
        return true;

    sscanf(*buffer, "%d", &value);//проверка не делается так как если метка, то возвращяет ноль и это нормально

    if (sscanf(*buffer, "%s", command) != 1)
        return true;

    if (command[0] == ':')
    {
        if (sscanf(command+1, "%d", &number_of_label) != 1)
            return true;
        if (label_upgrade(labels, start_number_of_labels, number_of_label))
            return true;
        (*mashine_code)[*number_of_command] = (*labels)[number_of_label];
    }
    else
    {
        (*mashine_code)[*number_of_command] = value;
    }

    return false;
}

bool do_jbe_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label)
{
    int value = 0;

    *buffer += (strlen(command)+1)*sizeof(char);
    //printf("Write 1\n");
    (*mashine_code)[*number_of_command] = JBE;

    (*number_of_command)++;
    if (massive_upgrade(mashine_code, *start_number_of_command, *number_of_command))
        return true;

    sscanf(*buffer, "%d", &value);//проверка не делается так как если метка, то возвращяет ноль и это нормально

    if (sscanf(*buffer, "%s", command) != 1)
        return true;

    if (command[0] == ':')
    {
        if (sscanf(command+1, "%d", &number_of_label) != 1)
            return true;
        if (label_upgrade(labels, start_number_of_labels, number_of_label))
            return true;
        (*mashine_code)[*number_of_command] = (*labels)[number_of_label];
    }
    else
    {
        (*mashine_code)[*number_of_command] = value;
    }

    return false;
}

bool do_ja_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label)
{
    int value = 0;

    *buffer += (strlen(command)+1)*sizeof(char);
    //printf("Write 1\n");
    (*mashine_code)[*number_of_command] = JA;

    (*number_of_command)++;
    if (massive_upgrade(mashine_code, *start_number_of_command, *number_of_command))
        return true;

    sscanf(*buffer, "%d", &value);//проверка не делается так как если метка, то возвращяет ноль и это нормально

    if (sscanf(*buffer, "%s", command) != 1)
        return true;

    if (command[0] == ':')
    {
        if (sscanf(command+1, "%d", &number_of_label) != 1)
            return true;
        if (label_upgrade(labels, start_number_of_labels, number_of_label))
            return true;
        (*mashine_code)[*number_of_command] = (*labels)[number_of_label];
    }
    else
    {
        (*mashine_code)[*number_of_command] = value;
    }

    return false;
}

bool do_jae_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label)
{
    int value = 0;

    *buffer += (strlen(command)+1)*sizeof(char);
    //printf("Write 1\n");
    (*mashine_code)[*number_of_command] = JAE;

    (*number_of_command)++;
    if (massive_upgrade(mashine_code, *start_number_of_command, *number_of_command))
        return true;

    sscanf(*buffer, "%d", &value);//проверка не делается так как если метка, то возвращяет ноль и это нормально

    if (sscanf(*buffer, "%s", command) != 1)
        return true;

    if (command[0] == ':')
    {
        if (sscanf(command+1, "%d", &number_of_label) != 1)
            return true;
        if (label_upgrade(labels, start_number_of_labels, number_of_label))
            return true;
        (*mashine_code)[*number_of_command] = (*labels)[number_of_label];
    }
    else
    {
        (*mashine_code)[*number_of_command] = value;
    }

    return false;
}

bool do_je_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label)
{
    int value = 0;

    *buffer += (strlen(command)+1)*sizeof(char);
    //printf("Write 1\n");
    (*mashine_code)[*number_of_command] = JE;

    (*number_of_command)++;
    if (massive_upgrade(mashine_code, *start_number_of_command, *number_of_command))
        return true;

    sscanf(*buffer, "%d", &value);//проверка не делается так как если метка, то возвращяет ноль и это нормально

    if (sscanf(*buffer, "%s", command) != 1)
        return true;

    if (command[0] == ':')
    {
        if (sscanf(command+1, "%d", &number_of_label) != 1)
            return true;
        if (label_upgrade(labels, start_number_of_labels, number_of_label))
            return true;
        (*mashine_code)[*number_of_command] = (*labels)[number_of_label];
    }
    else
    {
        (*mashine_code)[*number_of_command] = value;
    }

    return false;
}

bool do_jne_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label)
{
    int value = 0;

    *buffer += (strlen(command)+1)*sizeof(char);
    //printf("Write 1\n");
    (*mashine_code)[*number_of_command] = JNE;

    (*number_of_command)++;
    if (massive_upgrade(mashine_code, *start_number_of_command, *number_of_command))
        return true;

    sscanf(*buffer, "%d", &value);//проверка не делается так как если метка, то возвращяет ноль и это нормально

    if (sscanf(*buffer, "%s", command) != 1)
        return true;

    if (command[0] == ':')
    {
        if (sscanf(command+1, "%d", &number_of_label) != 1)
            return true;
        if (label_upgrade(labels, start_number_of_labels, number_of_label))
            return true;
        (*mashine_code)[*number_of_command] = (*labels)[number_of_label];
    }
    else
    {
        (*mashine_code)[*number_of_command] = value;
    }

    return false;
}

bool do_jmp_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label)
{
    int value = 0;

    *buffer += (strlen(command)+1)*sizeof(char);
    //printf("Write 1\n");
    (*mashine_code)[*number_of_command] = JMP;

    (*number_of_command)++;
    if (massive_upgrade(mashine_code, *start_number_of_command, *number_of_command))
        return true;

    sscanf(*buffer, "%d", &value);//проверка не делается так как если метка, то возвращяет ноль и это нормально

    if (sscanf(*buffer, "%s", command) != 1)
        return true;

    if (command[0] == ':')
    {
        if (sscanf(command+1, "%d", &number_of_label) != 1)
            return true;
        if (label_upgrade(labels, start_number_of_labels, number_of_label))
            return true;
        (*mashine_code)[*number_of_command] = (*labels)[number_of_label];
    }
    else
    {
        (*mashine_code)[*number_of_command] = value;
    }

    return false;
}

bool do_call_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label)
{
    int value = 0;

    *buffer += (strlen(command)+1)*sizeof(char);
    //printf("Write 1\n");
    (*mashine_code)[*number_of_command] = CALL;

    (*number_of_command)++;
    if (massive_upgrade(mashine_code, *start_number_of_command, *number_of_command))
        return true;

    sscanf(*buffer, "%d", &value);//проверка не делается так как если метка, то возвращает ноль и это нормально

    if (sscanf(*buffer, "%s", command) != 1)
        return true;

    if (command[0] == ':')
    {
        if (sscanf(command+1, "%d", &number_of_label) != 1)
            return true;

        if (label_upgrade(labels, start_number_of_labels, number_of_label))
            return true;
        (*mashine_code)[*number_of_command] = (*labels)[number_of_label];
    }
    else
    {
        (*mashine_code)[*number_of_command] = value;
    }

    return false;
}

bool do_popr_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer)
{
    int value = 0;

    *buffer += (strlen(command)+1)*sizeof(char);
    (*mashine_code)[*number_of_command] = POPR;

    (*number_of_command)++;
    if (massive_upgrade(mashine_code, *start_number_of_command, *number_of_command))
        return true;

    if(sscanf(*buffer, "%s", command) != 1)
        return true;

    value = reg_number(command);
    if (value == UNKNOWN_REG)
    {
        printf("stars.asm:%d: syntax_error\n", *number_of_command);
        return true;
    }
    (*mashine_code)[*number_of_command] = value;

    return false;
}

int reg_number(const char* reg)
{
    if (strcmp(reg, "RAX") == 0 || strcmp(reg, "[RAX]") == 0)
        return RAX;
    else if (strcmp(reg, "RBX") == 0 || strcmp(reg, "[RBX]") == 0)
        return RBX;
    else if (strcmp(reg, "RCX") == 0 || strcmp(reg, "[RCX]") == 0)
        return RCX;
    else if (strcmp(reg, "RDX") == 0 || strcmp(reg, "[RDX]") == 0)
        return RDX;
    else if (strcmp(reg, "RSI") == 0 || strcmp(reg, "[RSI]") == 0)
        return RSI;
    else if (strcmp(reg, "RDI") == 0 || strcmp(reg, "[RDI]") == 0)
        return RDI;
    else if (strcmp(reg, "RBP") == 0 || strcmp(reg, "[RBP]") == 0)
        return RBP;
    else if (strcmp(reg, "RSP") == 0 || strcmp(reg, "[RSP]") == 0)
        return RSP;
    else if (strcmp(reg, "R8") == 0 || strcmp(reg, "[R8]") == 0)
        return R8;
    else if (strcmp(reg, "R9") == 0 || strcmp(reg, "[R9]") == 0)
        return R9;
    else if (strcmp(reg, "R10") == 0 || strcmp(reg, "[R10]") == 0)
        return R10;
    else if (strcmp(reg, "R11") == 0 || strcmp(reg, "[R11]") == 0)
        return R11;
    else if (strcmp(reg, "R12") == 0 || strcmp(reg, "[R12]") == 0)
        return R13;
    else if (strcmp(reg, "R13") == 0 || strcmp(reg, "[R13]") == 0)
        return R14;
    else if (strcmp(reg, "R14") == 0 || strcmp(reg, "[R14]") == 0)
        return R14;
    else if (strcmp(reg, "R15") == 0 || strcmp(reg, "[R15]") == 0)
        return R15;
    else
        return UNKNOWN_REG;
}
