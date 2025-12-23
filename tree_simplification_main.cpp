#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include "file_using.h"
#include "language.h"

static bool check_file_founded(int argc, int number_of_files);

const int NUMBER_OF_FILES = 1;

int main(int argc, char* argv[])
{
    tree_t* tree = NULL;
    FILE* output_address = NULL;

    if (check_file_founded(argc, NUMBER_OF_FILES))
        return FILES_NOT_FOUNDED_ERROR;

    if (tree_init(&tree))
        return ALLOCATION_ERROR;

    if (tree_input(tree, argv[1]))
        return TREE_READING_ERROR;

    tree->root = equation_simplification(tree->root, tree);

    if (check_file_opening(argv[1], &output_address, "w+"))
        return FILE_OPENING_ERROR;

    node_output(tree->root, tree, output_address);

    if (check_file_closing(output_address))
        printf("Error while closing file!");

    tree_dump(tree);

    printf("Tree simplification is finished\nMiddle end success!\n");

    tree_destroy(tree);

    return NO_ERROR;
}

bool check_file_founded(int argc, int number_of_files)
{
    if (argc < number_of_files)
    {
        fprintf(stderr, "Files not founded. Please, give programm file with code tree\n");
        return 1;
    }

    return 0;
}
