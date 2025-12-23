#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <sys/stat.h>
#include "file_using.h"
#include "language.h"

#define LV node_calculate(node->left, tree)
#define RV node_calculate(node->right, tree)
//если функция одного аргумента, то он - левый

static node_t* number_calculate(node_t* node, tree_t* tree);
static node_t* delete_zeros(node_t** node, tree_t* tree);
static int node_calculate(node_t* node, tree_t* tree);

const double INACCURACY = 10e-5;

node_t* equation_simplification(node_t* node, tree_t* tree)
{
    assert(node);
    assert(tree);

    node = number_calculate(node, tree);
    node = delete_zeros(&node, tree);
    node = number_calculate(node, tree);
    node = delete_zeros(&node, tree);

    return node;
}

node_t* number_calculate(node_t* node, tree_t* tree)
{
    assert(node);
    assert(tree);

    if (node->type != OPERATOR_TYPE)
        return node;

    if (node->left != NULL) node->left = number_calculate(node->left, tree);
    if (node->right != NULL) node->right = number_calculate(node->right, tree);

    if (node->type == OPERATOR_TYPE && node->value.operator_name < ASSIGNMENT &&
        node->left->type == NUMBER_TYPE && node->right->type == NUMBER_TYPE)
    {
        node->value.number_value = node_calculate(node, tree);
        node->type = NUMBER_TYPE;
        free(node->left);
        free(node->right);
        node->left = NULL;
        node->right = NULL;
    }

    return node;
}

node_t* delete_zeros(node_t** node, tree_t* tree)
{
    assert(node);
    assert(*node);
    assert(tree);

    node_t* left = (*node)->left;
    node_t* right = (*node)->right;
    node_t* old_node = *node;

    if ((*node)->left != NULL) delete_zeros(&(*node)->left, tree);
    if ((*node)->right != NULL) delete_zeros(&(*node)->right, tree);

    *node = number_calculate(*node, tree);

    left = (*node)->left;
    right = (*node)->right;
    old_node = *node;

    if ((*node)->right == NULL || (*node)->left == NULL)
        return *node;

    if ((*node)->type == OPERATOR_TYPE)
    {
        if ((*node)->value.operator_name == ADD)
        {
            if ((*node)->left->type == NUMBER_TYPE && (*node)->left->value.number_value < INACCURACY)
            {
                *node = (*node)->right;

                free(left);
                free(old_node);
            }
            if ((*node)->right->type == NUMBER_TYPE && (*node)->right->value.number_value < INACCURACY)
            {
                *node = (*node)->left;

                free(right);
                free(old_node);
            }
        }
        else if ((*node)->value.operator_name == SUB)
        {
            if ((*node)->right->type == NUMBER_TYPE && (*node)->right->value.number_value < INACCURACY)
            {
                *node = (*node)->left;

                free(right);
                free(old_node);
            }
        }
        else if ((*node)->value.operator_name == MUL)
        {
            if ((*node)->right->type == NUMBER_TYPE && (*node)->right->value.number_value < INACCURACY)
            {
                *node = (*node)->right;

                node_destroy(left);
                free(old_node);
            }
            else if ((*node)->left->type == NUMBER_TYPE && (*node)->left->value.number_value < INACCURACY)
            {
                *node = (*node)->left;

                node_destroy(right);
                free(old_node);
            }
            else if ((*node)->right->type == NUMBER_TYPE && abs((*node)->right->value.number_value - 1) < INACCURACY)
            {
                *node = (*node)->left;

                free(right);
                free(old_node);
            }
            else if ((*node)->left->type == NUMBER_TYPE && abs((*node)->left->value.number_value - 1) < INACCURACY)
            {
                *node = (*node)->right;

                free(left);
                free(old_node);
            }
        }
        else if ((*node)->value.operator_name == DIV)
        {
            if ((*node)->left->type == NUMBER_TYPE && (*node)->left->value.number_value < INACCURACY)
            {
                *node = (*node)->left;

                node_destroy(right);
                free(old_node);
            }
            else if ((*node)->right->type == NUMBER_TYPE && abs((*node)->right->value.number_value - 1) < INACCURACY)
            {
                *node = (*node)->left;

                free(right);
                free(old_node);
            }
        }
    }

    return *node;
}

int node_calculate(node_t* node, tree_t* tree)
{
    assert(node);
    assert(tree);

    if (node->type == NUMBER_TYPE)
        return node->value.number_value;
    else if (node->type == VARIABLE_TYPE)
        return tree->variable_list[node->value.variable_number].var_value;
    else if (node->type == OPERATOR_TYPE)
    {
        switch(node->value.operator_name)
        {
        case ADD:
            return LV + RV;
        case SUB:
            return LV - RV;
        case MUL:
            return LV * RV;
        case DIV:
            return LV / RV;
        case ASSIGNMENT:
        case IF:
        case WHILE:
        case END_OF_PROGRAMM:
        case PAR_OPEN:
        case PAR_CLOSE:
        case OP_END:
        case COPMLEX_OPERATOR_OPEN:
        case COPMLEX_OPERATOR_CLOSE:
        case FUNC:
        case PRINT:
        case INPUT:
        default:
            printf("ERROR: unknown operator - %d\n", node->value.operator_name);
            return UNKNOWN_OPERATOR_ERROR;
        }
    }

    return TYPE_ERROR;
}

#undef LV
#undef RV
