#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct diff{
    long offset;              /* offset of the difference in file starting from zero*/
    unsigned char orig_value; /* value of the byte in ORIG */
    unsigned char new_value;  /* value of the byte in NEW */
} diff;

typedef struct node node;
 
struct node {
    diff *diff_data; /* pointer to a struct containing the offset and the value of the bytes in each of the files*/
    node *next;
};

void list_print(node *diff_list, FILE* output) {
    /* Print the nodes in diff_list in the following format: byte POSITION ORIG_VALUE NEW_VALUE. 
    Each item followed by a newline character. */
    node *current_node = diff_list;
    while (current_node != NULL) {
        int curr_offset = current_node->diff_data->offset;
        fprintf(output , "byte %d %02x %02x\n", curr_offset, current_node->diff_data->orig_value, current_node->diff_data->new_value);
        current_node = current_node->next;
    }
}

node *list_append(node *diff_list, diff *data)
{
    /* Add a new node with the given data to the list,
and return a pointer to the list (i.e., the first node in the list).
If the list is null - create a new entry and return a pointer to the entry.*/

    node *node_to_add = malloc(sizeof(node));
    node_to_add->diff_data = data; 
    node_to_add->next = NULL;
    
    if (diff_list == NULL)
    {
        diff_list = node_to_add;
        return diff_list;
    };
    node *current_node = diff_list;
    node *previous_node = NULL;

    while (current_node)
    {
        previous_node = current_node;
        current_node = current_node->next;
    }
    previous_node->next = node_to_add; 

    return diff_list; 
}

void list_free(node *diff_list){
/* Free the memory allocated by and for the list. */

    while (diff_list) //while not null
    {
        node *next_node = diff_list->next;
        free(diff_list->diff_data);
        free(diff_list);
        diff_list = next_node;
    }
}
    
long get_file_len(FILE *file) {
    fseek(file , 0 , SEEK_END);
    long len = ftell(file);
    fseek(file , 0 , SEEK_SET);

    return len;
}

long get_max_size(long len1, long len2) {
    if (len1 < len2)
        return len2;

    return len1;
}

int main(int argc , char **argv) {
    if (argc < 3) {
        printf("need to provide 2 files\n");
        return 0;
    }

    FILE *output = stdout;
    FILE *originFile = fopen(argv[1], "rb");
    FILE *newFile = fopen(argv[2], "rb+");
    long len_originFile = get_file_len(originFile);
    long len_newFile = get_file_len(newFile);
    long overallSize = get_max_size(len_originFile, len_newFile);

    node *diff_list = NULL;
    for (int i = 0; i < overallSize; i++){
        unsigned char charOrigin;
        unsigned char charNew;
        fread(&charOrigin , 1 , 1 , originFile);
        fread(&charNew , 1 , 1 , newFile);
        if (charNew != charOrigin) {
            diff* diff_node = malloc(sizeof(diff));
            diff_node->offset = i;
            diff_node->orig_value = charOrigin;
            diff_node->new_value = charNew;
            diff_list = list_append(diff_list, diff_node);
        }
    }
    list_print(diff_list , output);
    list_free(diff_list);
    return 0;
}