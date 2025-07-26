#include <stdio.h>
#include <stdlib.h>

struct ListNode
{
    int val;
    struct ListNode *next;
};

// No. of Nodes = [1, 100];
// val = [0, 9];

struct ListNode *addTwoNumbers(struct ListNode *l1, struct ListNode *l2)
{
    unsigned char add;
    unsigned char next_add = 0;
    struct ListNode *result_last = (struct ListNode *)malloc(sizeof(struct ListNode));
    struct ListNode *result = result_last;

    while (1)
    {
        add = ((l1 != NULL ? l1->val : 0) + (l2 != NULL ? l2->val : 0));
        // {
        result_last->val = (add % 10 + next_add) % 10;
        next_add = (add + next_add) / 10;

        l1 = l1 != NULL ? l1->next : l1;
        l2 = l2 != NULL ? l2->next : l2;
        if (l1 == NULL && l2 == NULL)
        {
            if (next_add > 0)
            {
                result_last->next = result_last->next = (struct ListNode *)malloc(sizeof(struct ListNode));
                result_last->next->val = next_add;
                result_last->next->next = NULL;
            }
            else
                result_last->next = NULL;

            break;
        }

        result_last->next = (struct ListNode *)malloc(sizeof(struct ListNode));
        result_last = result_last->next;
    }
    return result;
}

int main()
{
    printf("\nHello, World!\n\n");

    return 0;
}