#include <iostream>
using namespace std;

class String;

template <class var>
class Linked_List
{
    friend class String;
    friend ostream &operator<<(ostream &os, const String &v);

    var data;
    Linked_List *next = nullptr;
    long int size = 1;

public:
    Linked_List() : next(nullptr) { size--; }
    Linked_List(var data) : next(nullptr), data(data) {}

    void push(var data)
    {
        if (size == 0)
        {
            this->data = data;
            size++;
            return;
        }

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
        if (size == 0)
        {
            this->data = data;
            size++;
            return;
        }

        Linked_List *new_element = new Linked_List(this->data);

        this->data = data;
        new_element->next = next;
        next = new_element;

        size++;
    }

    void insert(var data, int index)
    {
        if (index > size || index < 0)
        {
            cout << "****Invalid Index!****" << endl
                 << endl;
            return;
        }
        else if (index == 0)
        {
            unshift(data);
            return;
        }
        else if (index == size)
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
        if (size == 0)
        {
            cout << "The Linked List is already Empty!!!" << endl;
            return;
        }
        else if (size == 1)
        {
            next = nullptr;
            size--;
            data = var();
            return;
        }

        Linked_List *last_element = this;

        while (last_element->next != nullptr && last_element->next->next != nullptr)
        {
            last_element = last_element->next;
        }

        delete last_element->next;
        last_element->next = nullptr;
        size--;
    }

    void shift()
    {
        if (size == 0)
        {
            cout << "The Linked List is already Empty!!!" << endl;
            return;
        }
        else if (size == 1)
        {
            next = nullptr;
            size--;
            data = var();
            return;
        }

        Linked_List *delete_element = next;

        swap(data, delete_element->data);
        next = delete_element->next;
        delete_element->next = nullptr;

        delete delete_element;
        size--;
    }

    void remove(int index)
    {
        if (index > size || index < 0)
        {
            cout << "****Invalid Index!****" << endl
                 << endl;
            return;
        }
        else if (index == 0)
        {
            shift();
            return;
        }
        else if (index == size)
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

    inline var get_data()
    {
        return data;
    }

    ~Linked_List()
    {
        if (next != nullptr)
            delete next;

        next = nullptr;
        // cout << "The element with data " << data << " !!!Destroyed!!!" << endl;
    }
};