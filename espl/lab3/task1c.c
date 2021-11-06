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

void print_first_k(node *diff_list, FILE* output, int k) {
    node *current_node = diff_list;
    while (current_node != NULL && k > 0) {
        int currOffset = current_node->diff_data->offset;
        fprintf(output , "byte %d %02x %02x\n", currOffset, current_node->diff_data->orig_value, current_node->diff_data->new_value);
        current_node = current_node->next;
        k--;
    }
}

void print_total_diff(node *diff_list, FILE* output) {
    int i = 0;
    node *curr = diff_list;
    while (curr != NULL) {
        i++;
        curr = curr->next;
    }
    fprintf(output, "%d\n", i);
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
    int oFlag = 0;
    int tFlag = 0;
    int kFlag = 0;
    int output_file_index = 0;
    int kValue = 0;
    int origin_file_index = 0;
    int new_file_index = 0;
    node *diff_list = NULL;
	for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (argc <= i + 1) {
                printf("output filename!\n");
                return 0;
            }
            output_file_index = i + 1;
            oFlag = 1;
            i++;
        } else if (strcmp(argv[i], "-t") == 0) {
            tFlag = 1;
        } else if (strcmp(argv[i], "-k") == 0) {
            if (argc <= i + 1) {
                printf("value for -k flag!\n");
                return 0;
            }
            kValue = i + 1;
            kFlag = 1;
            i++;
        } else if (!origin_file_index) {
            origin_file_index = i;
        } else if (!new_file_index) {
            new_file_index = i;
        }
    }

    if (!new_file_index) {
        printf("provide 2 files\n");
        return 0;
    }
    FILE *output = oFlag ? fopen(argv[output_file_index], "w") : stdout;
    FILE *originFile = fopen(argv[origin_file_index], "rb");
    FILE *newFile = fopen(argv[new_file_index], "rb+");
    long len_originFile = get_file_len(originFile);
    long len_newFile = get_file_len(newFile);
    long overallSize = get_max_size(len_originFile, len_newFile);
    long size_of_diff = 0;
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
            size_of_diff++;
        }
    }   
    if (tFlag)
        print_total_diff(diff_list, output);
    else if (kFlag)
        print_first_k(diff_list, output, atoi(argv[kValue]));
    else {
        list_print(diff_list, output);
    }
    list_free(diff_list);
    return 0;
}
