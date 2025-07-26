#include <iostream>
#include "Linked_List.cpp"
using namespace std;

template <class var>
class Queue
{
    Linked_List<var> *head;
    long int size;

public:
    Queue() : head(nullptr), size(0) {}
    Queue(var data) : head(new Linked_List<var>(data)), size(1) {}

    Queue &enqueue(var data)
    {
        if (head == nullptr)
            head = new Linked_List<var>(data);
        else
            head->push(data);

        size++;
        return (*this);
    }

    Queue &dequeue()
    {
        if (head == nullptr)
        {
            ~Queue();
            throw out_of_range("Queue Underflow!!!");
        }
        head->pop();
        size--;

        return (*this);
    }

    var peek(long int index)
    {
        if (index < 0 || index > size - 1)
            throw out_of_range("Unfair index");
        return head->peek(index)->get_data();
    }

    inline long int length() { return size; }

    ~Queue()
    {
        if (head != nullptr)
            delete head;
    }
};

int main()
{
    cout << "Hello, World!" << endl
         << endl;

    Queue<string> interviewees("Aryan");
    interviewees.enqueue("Zain");
    interviewees.enqueue("Virat");
    interviewees.enqueue("Ayat");
    interviewees.enqueue("Mithali");

    cout << "Interviewees: ";
    for (int i = 0; i < interviewees.length(); i++)
        cout << interviewees.peek(i) << (i < interviewees.length() - 1 ? ", " : "");
    cout << endl
         << endl;

    return 0;
}