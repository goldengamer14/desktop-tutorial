#include <iostream>
using namespace std;

template <class var>
class Circular_Linked_List
{
    template <class variable>
    friend void display_CLL(Circular_Linked_List<variable> *);

    var data;
    Circular_Linked_List *next = this;
    Circular_Linked_List *head = this;
    int size = 1;

public:
    Circular_Linked_List(var data) : data(data) {}

    Circular_Linked_List(var data_arr[], int size)
    {
        this->data = data_arr[0];

        for (int index = 1; index < size; index++)
        {
            push(data_arr[index]);
        }
    }

    void display();

    void push(var data)
    {
        Circular_Linked_List *last_element = this;

        while (last_element->next != this)
        {
            last_element = last_element->next;
        }

        last_element->next = new Circular_Linked_List(data);
        last_element->next->next = this;
        last_element->next->head = this;

        size++;
    }

    void unshift(var data)
    {
        Circular_Linked_List *new_element = new Circular_Linked_List(this->data);

        new_element->head = head;
        new_element->next = next;
        next = new_element;
        this->data = data;

        size++;
    }

    void insert(var data, int index)
    {
        if (index >= size || index < 0)
        {
            cout << "****Invalid index****" << endl
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

        Circular_Linked_List *previous_element = get_element(index);
        Circular_Linked_List *new_element = new Circular_Linked_List(data);

        new_element->next = previous_element->next;
        new_element->head = head;
        previous_element->next = new_element;

        size++;
    }

    void pop()
    {
        Circular_Linked_List *last_element = this;

        while (last_element->next->next != head)
            last_element = last_element->next;

        delete last_element->next;
        last_element->next = head;

        size--;
    }

    void shift()
    {
        Circular_Linked_List *delete_element = next;

        swap(data, delete_element->data);
        next = delete_element->next;

        delete_element->next = head;
        delete delete_element;

        size--;
    }

    void remove(int index)
    {
        if (index >= size || index < 0)
        {
            cout << "****Invalid index****" << endl
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

        Circular_Linked_List *element = get_element(index - 1);
        Circular_Linked_List *delete_element = element->next;

        element->next = delete_element->next;

        delete_element->next = head;
        delete delete_element;

        size--;
    }

    Circular_Linked_List *get_element(int index)
    {
        if (index >= size || index < 0)
        {
            cout << "****Invalid index****" << endl
                 << endl;
        }

        Circular_Linked_List *element = this;

        for (int i = 0; i < index; i++)
        {
            element = element->next;
        }
        return element;
    }

    inline int length() { return size; }

    ~Circular_Linked_List()
    {
        cout << "The element with data " << data << " Destroyed!!!" << endl
             << endl;

        if (next != head)
            delete next;
    }
};

template <class var>
void Circular_Linked_List<var>::display()
{
    cout << "{" << endl
         << "\tData: " << data << ";" << endl
         << "\tNext: " << next->data << ";" << endl
         << "}" << endl
         << endl;
}

template <class var>
void display_CLL(Circular_Linked_List<var> *);

int main()
{
    cout << "Hello, World!" << endl
         << endl;

    Circular_Linked_List<short int> *head = new Circular_Linked_List<short int>(45);
    head->push(12);
    head->push(71);
    head->push(28);
    head->push(34);
    head->push(89);

    display_CLL(head);

    head->pop();
    head->pop();

    display_CLL(head);

    head->unshift(21);
    head->unshift(67);
    head->unshift(343);

    display_CLL(head);

    head->shift();
    head->shift();

    display_CLL(head);

    head->insert(56, head->length() / 2 - 1);
    head->insert(84, 0);
    head->insert(97, head->length() - 1);

    display_CLL(head);

    head->remove(0);
    head->remove(head->length() - 1);
    head->remove((head->length() - 1) / 2);

    display_CLL(head);

    delete head;
    head = nullptr;

    char datas_char[5] = {'c', 'j', 'l', 'e', 'a'};
    Circular_Linked_List<char> *head_char = new Circular_Linked_List<char>(datas_char, 5);

    display_CLL(head_char);

    head_char->pop();
    head_char->pop();

    display_CLL(head_char);

    head_char->unshift('n');
    head_char->unshift('y');
    head_char->unshift('p');

    display_CLL(head_char);

    head_char->shift();
    head_char->shift();

    display_CLL(head_char);

    head_char->insert('v', 0);
    head_char->insert('o', head_char->length() - 1);
    head_char->insert('q', head_char->length() / 2 - 1);

    display_CLL(head_char);

    head_char->remove(0);
    head_char->remove(head_char->length() - 1);
    head_char->remove((head_char->length() - 1) / 2);

    display_CLL(head_char);

    delete head_char;
    head_char = nullptr;

    return 0;
}

template <class var>
void display_CLL(Circular_Linked_List<var> *head)
{
    cout << "{";
    for (int i = 0; i < head->length(); i++)
    {
        cout << head->get_element(i)->data << (i < head->length() - 1 ? ", " : "");
    }
    cout << "}" << endl
         << endl;
};