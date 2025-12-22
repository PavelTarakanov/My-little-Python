#ifndef STACK_H
#define STACK_H

const unsigned int START_STACK_SIZE = 10;
const int LEFT_BIRD = 2147483647;
const int RIGTH_BIRD = 2147483646;

enum Stack_Error_Code
{
    NO_ERROR = 0,
    STK_ADDRESS_ERROR = 1,
    STK_SIZE_LESS_ZERO_ERROR = 2,
    STK_SIZE_MORE_CAPACITY_ERROR = 3,
    LEFT_BIRD_ERROR = 4,
    RIGTH_BIRD_ERROR = 5,
    ALLOCATION_ERROR = 6,
    REALLOCATION_ERROR = 7
};

struct stack_t{int* data;
               unsigned int size;
               unsigned int capacity;};

bool check_file_opening(const char *input_address, FILE** file_ptr, const char* access_rights);
bool check_file_closing(FILE* input_address);

Stack_Error_Code stack_init(stack_t* stk, unsigned int capacity);

Stack_Error_Code stack_upgrade(stack_t* stk);
Stack_Error_Code stack_degrade(stack_t* stk);
Stack_Error_Code stack_push(stack_t* stk, int value);
Stack_Error_Code stack_pop(stack_t* stk, int* address);
Stack_Error_Code stack_verify(stack_t* stk);
void stack_dump(stack_t* stk);

int my_strcmp(const char* str_1, const char* str_2);

#endif //STACK_H
