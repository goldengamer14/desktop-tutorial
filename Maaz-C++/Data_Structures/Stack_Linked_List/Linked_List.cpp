#include <iostream>
using namespace std;

template <class var>
class Linked_List
{
    template <class vari>
    friend class Stack;

    template <class variable>
    friend void display_Linked_List(Linked_List<variable> *);

    var data;
    Linked_List *next = nullptr;
    long int size = 1;

public:
    Linked_List() {size--;}
    Linked_List(var data) : data(data) {}

    void push(var data)
    {
        Linked_List *last_element = this;

        while (last_element->next != nullptr)
        {
            last_element = last_element->next;
        }

        last_element->next = new Linked_List(data);
        size++;
    }

    void unshift(var data)
    {
        Linked_List *new_element = new Linked_List(this->data);

        this->data = data;
        new_element->next = next;
        next = new_element;

        size++;
    }

    void insert(var data, int index)
    {
        if (index > size - 1 || index < 0)
        {
            cout << "****Invalid Index!****" << endl
                 << endl;
        }
        else if (index == 0)
        {
            unshift(data);
            return;
        }
        else if (index == size - 1)
        {
            push(data);
            return;
        }

        Linked_List *previous_element = this;
        for (int i = 0; i < index - 1; i++)
        {
            previous_element = previous_element->next;
        }

        Linked_List *new_element = new Linked_List(data);
        new_element->next = previous_element->next;
        previous_element->next = new_element;

        size++;
    }

    void pop()
    {
        Linked_List *last_element = this;

        while (last_element->next->next != nullptr)
        {
            last_element = last_element->next;
        }

        delete last_element->next;
        last_element->next = nullptr;
        size--;
    }

    void shift()
    {
        Linked_List *delete_element = next;

        swap(data, delete_element->data);
        next = delete_element->next;
        delete_element->next = nullptr;

        delete delete_element;
        size--;
    }

    void remove(int index)
    {
        if (index > size - 1 || index < 0)
        {
            cout << "****Invalid Index!****" << endl
                 << endl;
        }
        else if (index == 0)
        {
            shift();
            return;
        }
        else if (index == size - 1)
        {
            pop();
            return;
        }

        Linked_List *previous_element = this;
        for (int i = 0; i < index - 1; i++)
        {
            previous_element = previous_element->next;
        }

        Linked_List *delete_element = previous_element->next;
        previous_element->next = delete_element->next;

        delete_element->next = nullptr;
        delete delete_element;

        size--;
    }

    int length() { return size; }

    var get_data()
    {
        return data;
    }

    ~Linked_List()
    {
        cout << "The element with data " << data << " !!!Destroyed!!!" << endl
             << endl;

        if (next != nullptr)
            delete next;
    }
};

template <class var>
void display_Linked_List(Linked_List<var> *head)
{
    Linked_List<int> *element = head;

    cout << "{";
    for (int i = 0; i < head->length(); i++)
    {
        cout << element->data << (i < head->length() - 1 ? ", " : "");
        element = element->next;
    }
    cout << "}" << endl
         << endl;
}