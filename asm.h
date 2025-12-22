#ifndef ASM_H
#define ASM_H

const int COMMAND_SIZE = 200;

enum Commands
{
    HLT = -1,
    POP = 0,
    PUSH = 1,
    OUT = 2,
    ADD = 3,
    MUL = 4,
    SUB = 5,
    DIV = 6,
    IN = 7,
    CALL = 8,
    RET = 9,
    SQRT = 10,
    POPR = 42,
    POPM = 43,
    PUSHR = 33,
    PUSHM = 34,
    JB = 50,
    JBE = 51,
    JA = 52,
    JAE = 53,
    JE = 54,
    JNE = 55,
    JMP = 56,
    VIDEO = 48,
    PAUSE = 49,
};

enum Regs_Numbers
{
    UNKNOWN_REG = -1,
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3,
    RSI = 4,
    RDI = 5,
    RBP = 6,
    RSP = 7,
    R8  = 8,
    R9  = 9,
    R10 = 10,
    R11 = 11,
    R12 = 12,
    R13 = 13,
    R14 = 14,
    R15 = 15,
};

enum File_Usimng_Error_Code
{
    NO_FILE_ERROR = 0,
    FILE_NOT_FOUND_ERROR = 1,
    FILE_OPENING_ERROR = 2,
    FILE_CLOSING_ERROR = 3,
    FILE_READIND_ERROR = 4,
    INITIALISATION_ERROR = 5,
    UNKNOWN_COMMAND = 6,
    MAKING_MASHINE_CODE_ERROR = 7
};


long long int read_text(FILE* input_address, char** buffer);
bool massive_upgrade(int** mashine_code, int* start_number_of_command, int number_of_command);
bool label_upgrade(int** labels, int* start_number_of_labels, int number_of_label);
bool make_mashine_code(FILE* output_address, char* buffer, long long int number_of_symbols);
bool make_code_massive(char* buffer, int** mashine_code, int** labels, int* start_number_of_command,
                               int* start_number_of_labels, long long int number_of_symbols);

int reg_number(const char* reg);

bool do_push_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer);
bool do_popr_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer);
bool do_pushr_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer);
bool do_jb_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label);
bool do_jbe_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label);
bool do_ja_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label);
bool do_jae_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label);
bool do_je_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label);
bool do_jne_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label);
bool do_jmp_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label);
bool do_call_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label);
bool do_popm_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer);
bool do_pushm_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer);

#endif //ASM_H
