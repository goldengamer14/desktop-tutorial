#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "Linked_List_c.c"
#include "Linked_List_type.c"

DECLARE_Linked_List(char)

#define Node Node_char
#define Linked_List Linked_List_char
#define construct_Linked_List_initialized construct_Linked_List_char_initialized
#define construct_Linked_List construct_Linked_List_char
#define peek peek_Linked_List_char
#define push push_Linked_List_char
#define push_string push_Linked_List_char_string
#define pop pop_Linked_List_char
#define pop_string pop_Linked_List_char_string
#define unshift unshift_Linked_List_char
#define unshift_string unshift_Linked_List_char_string
#define shift shift_Linked_List_char
#define shift_string shift_Linked_List_char_string
#define insert insert_Linked_List_char
#define insert_string insert_Linked_List_char_string
#define remove remove_Linked_List_char
#define remove_string remove_Linked_List_char_string
#define destruct_Linked_List destruct_Linked_List_char
#define destruct_Linked_List_Node destruct_Linked_List_char_Node

    void display_Linked_List(struct Linked_List *self)
{
    struct Node *ptr = self->head;
    for (long int i = 0; i < self->size; i++)
    {
        printf("%c", ptr->data);
        ptr = ptr->next;
    }
}

int main()
{
    printf("\n\nHello, World!\n\n");

    struct Linked_List list = construct_Linked_List_initialized('L');

    printf("Head element: %c\n\n", peek(&list, 0)->data);

    char exp[] = "ouvre de museum";

    //                      ****PUSH****
    for (long int i = 0; i < strlen(exp); i++)
        push(&list, exp[i]);

    //                      ****PEEK****
    printf("\nLinked_List string after \"ouvre de museum\" push: ");
    for (long int i = 0; i < list.size; i++)
        printf("%c", peek(&list, i)->data);

    //                      ****POP****
    for (long int i = 0; i < strlen(" de museum"); i++)
        pop(&list);

    printf("\nLinked_List string after \" de meuseum\" pop: ");
    display_Linked_List(&list);

    //                      ****END OF PUSH_POP****
    printf("\n");

    //                      ****UNSHIFT****
    strcpy(exp, "Mona Lisa ");

    for (int i = strlen(exp) - 1; i > -1; i--)
        unshift(&list, exp[i]);

    printf("\nLinked_List string after \"Mona Lisa \" unshift: ");
    display_Linked_List(&list);

    //                      ****SHIFT****
    for (int i = 0; i < strlen(exp); i++)
        shift(&list);

    printf("\nLinked_List string after \"Mona Lisa \" shift: ");
    display_Linked_List(&list);

    //                      ****END OF UNSHIFT_SHIFT****
    printf("\n");

    //                      ****INSERT****
    insert(&list, 'l', 1);
    insert(&list, ' ', 2);

    printf("\nLinked_List string after {'l', 1} and {' ', 2} insert: ");
    display_Linked_List(&list);

    //                      ****INSERT_STRING****
    strcpy(exp, "mane");
    insert_string(&list, exp, 3, strlen(exp));
    strcpy(exp, " ehT"); // ****UNSHIFT_STRING****
    insert_string(&list, exp, 0, strlen(exp));
    strcpy(exp, " thing"); // ****PUSH_STRING****
    insert_string(&list, exp, list.size - 1, strlen(exp));

    printf("\nLinked_List string after {\"mane\", 3}, {\"The \", 0} and {\" thing\", list.size - 1} insert: ");
    display_Linked_List(&list);

    //                      ****REMOVE****
    remove(&list, 6);

    printf("\nLinked_List string after 6 remove: ");
    display_Linked_List(&list);

    //                      ****REMOVE_STRING****
    remove_string(&list, 5, 8);
    remove_string(&list, 0, 4);             // ****SHIFT_STRING****
    remove_string(&list, 1, list.size - 1); // ****POP_STRING****

    printf("\nLinked_List string after {5, 8}, {0, 4} and {5, list.size - 1} remove_string: ");
    display_Linked_List(&list);

    printf("\n\n");

    destruct_Linked_List(&list);

    return 0;
}