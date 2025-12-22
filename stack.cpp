#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "stack.h"


Stack_Error_Code stack_init(stack_t* stk, unsigned int capacity)
{
    assert(stk);

    stk->size = 1;
    stk->capacity = capacity;
    stk->data = (int*) calloc(capacity + 2, sizeof(int*));

    if (stk->data == NULL)
    {
        printf("Error while memory allocation\n");
        return ALLOCATION_ERROR;
    }

    stk->data[0] = LEFT_BIRD;
    stk->data[capacity+1] = RIGTH_BIRD;
    //StackDump(stk);

    return NO_ERROR;
}

Stack_Error_Code stack_push(stack_t* stk, int value)
{
    assert(stk);

    Stack_Error_Code err = stack_verify(stk);

    if (err)
    {
        stack_dump(stk);
    }

    if (stk->size == stk->capacity)
        if (stack_upgrade(stk))
            return REALLOCATION_ERROR;

    stk->data[stk->size++] = value;

    err = stack_verify(stk);

    if (err)
    {
        stack_dump(stk);
    }

    return err;
}

Stack_Error_Code stack_pop(stack_t* stk, int* address)
{
    assert(stk);
    assert(address);

    Stack_Error_Code err = stack_verify(stk);

    if (err)
    {
        stack_dump(stk);
    }

    *address = stk->data[--stk->size];
    stk->data[stk->size] = 0;

    if (stk->size <= stk->capacity/2 && stk->size > START_STACK_SIZE)
        if (stack_degrade(stk))
            return REALLOCATION_ERROR;

    err = stack_verify(stk);

    if (err)
    {
        stack_dump(stk);
    }

    return err;
}

Stack_Error_Code stack_verify(stack_t* stk)
{
    assert(stk);

    if (stk == NULL)
        return STK_ADDRESS_ERROR;

    if (stk->size <= 0)
        return STK_SIZE_LESS_ZERO_ERROR;

    if (stk->size > stk->capacity)
        return STK_SIZE_MORE_CAPACITY_ERROR;

    if (stk->data[0] != LEFT_BIRD)
        return LEFT_BIRD_ERROR;

    if (stk->data[stk->capacity+1] != RIGTH_BIRD)
        return RIGTH_BIRD_ERROR;

    return NO_ERROR;
}

void stack_dump(stack_t* stk)
{
    assert(stk);

    printf("stack [%p]\n"
            "{\n"
            "   size = %d\n"
            "   capasity = %d\n"
            "   data[%p]\n"
            "   {\n"
            , stk, stk->size, stk->capacity, stk->data);
    for (unsigned int i = 0; i < stk->capacity+2; i++)
    {
        if (stk->data[i] != 0)
            printf("        *[%d] = %d\n", i, stk->data[i]);

        else
            printf("         [%d] = %d\n", i, stk->data[i]);
    }
    printf("    }\n"
           "}\n");

    return;
}

Stack_Error_Code stack_upgrade(stack_t* stk)
{
    assert(stk);

    stk->data[stk->capacity+1] = 0;
    stk->capacity = stk->capacity*2;
    stk->data = (int*) realloc(stk->data, (stk->capacity + 2)*sizeof(int));
    stk->data[stk->capacity+1] = RIGTH_BIRD;

    if (stk->data == NULL)
    {
        return REALLOCATION_ERROR;
    }

    return stack_verify(stk);
}

Stack_Error_Code stack_degrade(stack_t* stk)
{
    assert(stk);

    stk->data[stk->capacity+1] = 0;
    stk->capacity = stk->capacity/2;
    stk->data = (int*) realloc(stk->data, (stk->capacity + 2)*sizeof(int));
    stk->data[stk->capacity+1] = RIGTH_BIRD;

    if (stk->data == NULL)
    {
        return REALLOCATION_ERROR;
    }

    return stack_verify(stk);
}
