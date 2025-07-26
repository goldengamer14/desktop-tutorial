#include <iostream>
using namespace std;

template <class var>
class Linked_List
{
    template <class variable>
    friend void display_Linked_List(Linked_List<variable> *);

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

    Linked_List *peek(int index)
    {
        if (index > size || index < 0)
        {
            cout << "****Invalid Index!****" << endl
                 << endl;
            return this;
        }
        else if (index == 0)
            return this;

        Linked_List *ptr = this;
        for (index; index > -1; index--)
        {
            ptr = ptr->next;
        }
        return ptr;
    }

    inline long int length() { return size; }

    inline var get_data()
    {
        return data;
    }

    ~Linked_List()
    {
        if (next != nullptr)
            delete next;

        next = nullptr;
        cout << "The element with data " << data << " !!!Destroyed!!!" << endl;
    }
};

int main()
{
    cout << "Hello, World!" << endl
         << endl;

    Linked_List<int> *head = new Linked_List<int>(45);
    head->push(89);
    head->push(12);
    head->push(67);

    display_Linked_List(head);

    head->pop();
    head->pop();
    head->pop();
    head->pop();

    display_Linked_List(head);

    head->unshift(3);
    head->unshift(367);
    head->unshift(589);
    head->unshift(7);

    display_Linked_List(head);

    head->shift();
    head->shift();

    display_Linked_List(head);

    head->insert(10, 1);
    head->insert(35, 3);
    head->insert(7, 0);
    head->insert(345, head->length() - 1);

    display_Linked_List(head);

    head->remove(1);
    head->remove(0);
    head->remove(head->length() - 1);

    display_Linked_List(head);

    delete head;

    return 0;
}

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