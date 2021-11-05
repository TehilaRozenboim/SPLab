#include <stdio.h>
#include <stdlib.h>

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
    node *curr = diff_list;
    while (curr != NULL) {
        int currOffset = curr->diff_data->offset;
        fprintf(output , "byte %d %02x %02x\n", currOffset, curr->diff_data->orig_value, curr->diff_data->new_value);
        curr = curr->next;
    }
}

node *list_append(node *diff_list, diff *data)
{
    /* Add a new node with the given data to the list,
and return a pointer to the list (i.e., the first node in the list).
If the list is null - create a new entry and return a pointer to the entry.*/

    node *n = malloc(sizeof(node));
    n->diff_data = data; 
    n->next = NULL;
    
    if (diff_list == NULL)
    {
        diff_list = n;
        return n;
    };
    node *current = diff_list;
    node *previous = NULL;

    while (current) //while not null
    {
        previous = current;
        current = current->next;
    }
    previous->next = n; //added node to end of the list

    return diff_list; //return pointer to head of list 
}

void list_free(node *diff_list){
/* Free the memory allocated by and for the list. */

    while (diff_list) //while not null
    {
        node *next = diff_list->next;
        free(diff_list->diff_data);
        free(diff_list);
        diff_list = next;
    }
}
//this is to test the functions not sure took it from reference 
// int main(int argc, char **argv){
//     diff* diff_data1 = (diff*)malloc(sizeof(diff));
//     diff_data1->offset = 1;
//     diff_data1->orig_value = 'a';
//     diff_data1->new_value = 'b';


//     diff* diff_data2 = (diff*)malloc(sizeof(diff));
//     diff_data2->offset = 2;
//     diff_data2->orig_value = 'c';
//     diff_data2->new_value = 'd';

//     node* list = list_append(NULL, diff_data1);
//     list_append(list, diff_data2);
//     list_print(list, stdout);
//     list_free(list);
//     return 0;
// }
int main(int argc, char **argv) {
    
	diff* first = (diff*) malloc(sizeof(diff));
	diff* second = (diff*) malloc(sizeof(diff));
	node* list = NULL;

    first->offset = 1; first->orig_value='c'; first->new_value = 'a';
    second->offset = 2; second->orig_value='g'; second->new_value = 'e'; 

	list = list_append(list, first);
	list = list_append(list, second);

	list_print(list, stdout);
	list_free(list);

	return 0;
}