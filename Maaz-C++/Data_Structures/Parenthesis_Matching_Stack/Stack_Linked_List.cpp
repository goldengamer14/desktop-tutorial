#include <iostream>
#include "Linked_List.cpp"
using namespace std;

template <class var>
class Stack
{
    long int top = -1;
    Linked_List<var> *head = nullptr;

public:
    Stack() : head(new Linked_List<var>(var())) {}
    Stack(var data) : head(new Linked_List<var>(data)) { top++; }

    void push(var data)
    {
        if (top == -1)
        {
            head->data = data;
            top++;
            return;
        }

        head->unshift(data);
        top++;
    }

    void pop()
    {
        if (top == -1)
        {
            cout << "Stack underflow!!!" << endl
                 << endl;
            return;
        }
        else if (top == 0)
        {
            delete head;
            head = new Linked_List<var>(var());
            top--;
            return;
        }

        head->shift();
        top--;
    }

    var stack_top()
    {
        if (top == -1)
            cout << "Stack underflow!!!" << endl
                 << endl;
        return head->get_data();
    }

    var stack_bottom()
    {
        Linked_List<var> *ptr = head;

        if (top == -1)
            cout << "Stack underflow!!!" << endl
                 << endl;

        while (ptr->next != nullptr)
        {
            ptr = ptr->next;
        }

        return ptr->get_data();
    }

    var peek(int i)
    {
        if (top < i || i < 0)
        {
            cout << "Stack Underflow!\t";
            return this->stack_bottom();
        }

        Linked_List<var> *ptr = head;

        for (i; i > 0; i--)
            ptr = ptr->next;

        return ptr->get_data();
    }

    long int size() const { return top + 1; }

    void display_Stack()
    {
        Linked_List<var> *ptr = head;

        if (top == -1)
        {
            cout << "{}" << endl
                 << endl;
        }

        cout << "{";
        while (ptr->next != nullptr)
        {
            cout << ptr->data << ", ";
            ptr = ptr->next;
        }
        cout << ptr->data << "}" << endl
             << endl;
    }

    ~Stack()
    {
        delete head;
    }
};