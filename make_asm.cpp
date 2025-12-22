#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include "language.h"

tree_errors node_assemblating(node_t* node, tree_t* tree, FILE* asm_address, int* label_number)
{
    assert(node);
    assert(tree);

    int label_number_now = *label_number;

    if (node->type == NUMBER_TYPE)
        fprintf(asm_address, "PUSH %d\n", node->value.number_value);
    else if (node->type == VARIABLE_TYPE)
        fprintf(asm_address, "PUSH %d\n"
                             "POPR RAX\n"
                             "PUSHM [RAX]\n", node->value.variable_number);
    else if (node->type == OPERATOR_TYPE)
    {
        switch (node->value.operator_name)
        {
        case ADD:
            node_assemblating(node->left, tree, asm_address, label_number);
            node_assemblating(node->right, tree, asm_address, label_number);
            fprintf(asm_address, "ADD\n");
            break;
        case SUB:
            node_assemblating(node->left, tree, asm_address, label_number);
            node_assemblating(node->right, tree, asm_address, label_number);
            fprintf(asm_address, "SUB\n");
            break;
        case MUL:
            node_assemblating(node->left, tree, asm_address, label_number);
            node_assemblating(node->right, tree, asm_address, label_number);
            fprintf(asm_address, "MUL\n");
            break;
        case DIV:
            node_assemblating(node->left, tree, asm_address, label_number);
            node_assemblating(node->right, tree, asm_address, label_number);
            fprintf(asm_address, "DIV\n");
            break;
        case ASSIGNMENT:
            node_assemblating(node->right, tree, asm_address, label_number);
            fprintf(asm_address, "PUSH %d\n"
                                 "POPR RAX\n"
                                 "POPM [RAX]\n", node->left->value.variable_number);
            break;
        case IF:
            (*label_number)++;
            node_assemblating(node->left, tree, asm_address, label_number);
            fprintf(asm_address, "PUSH 0\n"
                                 "JE :%d\n", label_number_now);
            node_assemblating(node->right, tree, asm_address, label_number);
            fprintf(asm_address, ":%d\n", label_number_now);
            break;
        case WHILE:
            (*label_number)++;
            node_assemblating(node->left, tree, asm_address, label_number);
            fprintf(asm_address, "PUSH 0\n"
                                 "JE :%d\n", label_number_now);
            (*label_number)++;
            fprintf(asm_address, ":%d\n", label_number_now + 1);
            node_assemblating(node->right, tree, asm_address, label_number);
            node_assemblating(node->left, tree, asm_address, label_number);
            fprintf(asm_address, "PUSH 0\n"
                                 "JE :%d\n", label_number_now + 1);
            fprintf(asm_address, ":%d\n", label_number_now);
            break;
        case END_OF_PROGRAMM:
            fprintf(asm_address, "HLT");
            break;
        case OP_END:
            node_assemblating(node->left, tree, asm_address, label_number);
            if (node->right != NULL) node_assemblating(node->right, tree, asm_address, label_number);
            break;
        case PRINT:
            node_assemblating(node->left, tree, asm_address, label_number);
            fprintf(asm_address, "OUT\n");
            break;
        case INPUT:
            fprintf(asm_address, "IN\n"
                                 "PUSH %d\n"
                                 "POPR RAX\n"
                                 "POPM [RAX]\n", node->left->value.variable_number);
            break;
        case PAR_OPEN:
        case PAR_CLOSE:
        case COPMLEX_OPERATOR_OPEN:
        case COPMLEX_OPERATOR_CLOSE:
        case FUNC:
        default:
            printf("ERROR: unknown operator");
            return UNKNOWN_OPERATOR_ERROR;
        }
    }
    else
    {
        printf("Error! Unknown node type - %d\n", node->type);
        return TYPE_ERROR;
    }

    return NO_ERROR;
}
