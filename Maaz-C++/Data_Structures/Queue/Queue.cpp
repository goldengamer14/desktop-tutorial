#include <iostream>
using namespace std;

template <class var>
class Queue
{
    var *arr = nullptr;
    long int front_index;
    long int back_index;
    long unsigned int max_size;

public:
    inline long int length()
    {
        return (back_index >= front_index ? back_index - front_index + 1 : max_size - front_index + 1 + back_index);
    }

    Queue(long int size) : arr(new var[size]), front_index(-1), back_index(-1), max_size(size) {}
    Queue(var data, long unsigned int size) : arr(new var[size]{data}), front_index(0), back_index(0), max_size(size)
    {
        if (length() > max_size)
            throw out_of_range("Invalid size provided");
    }

    Queue &enqueue(var data)
    {
        if (length() >= max_size)
            throw out_of_range("Queue overflow!!!");
        else if (front_index > -1 && back_index == max_size - 1)
            arr[back_index = 0] = data;
        else if (back_index < max_size - 1)
            if (back_index + 1 != front_index)
                arr[++back_index] = data;
            else
                throw out_of_range("Queue overflow!!!");
        else
            throw out_of_range("Queue overflow!!!");
        return (*this);
    }

    Queue &dequeue()
    {
        if (length() < 1)
            throw out_of_range("Queue Underflow!!!");
        else
            arr[front_index++] = var();
        return (*this);
    }

    var peek(long int index)
    {
        if (index < 0)
            throw out_of_range("Queue Underflow!!!");
        else if (index >= max_size)
            throw out_of_range("Queue Overflow!!!");
        else if (index >= length())
        {
            cout << "Index undefined" << endl;
            return var();
        }
        else if (back_index >= front_index)
            return arr[front_index + index]; // + 1
        else if (max_size - front_index + index - 1 >= max_size)
            return arr[index - front_index - 1];
        else
            return arr[max_size - front_index + index - 1];
    }

    static void display(Queue &q);

    ~Queue()
    {
        if (arr != nullptr)
            delete[] arr;
    }
};

template <class var>
void Queue<var>::display(Queue<var> &q)
{
    for (int i = 0; i < q.length(); i++)
        cout << q.peek(i) << (i < q.length() - 1 ? ", " : "");
    cout << endl
         << "f: " << q.front_index << " b: " << q.back_index << endl;
}

int main()
{
    cout << endl
         << "Hello, World!" << endl
         << endl;

    Queue<string> interviewees("Aryan", 5);

    // front_index = 0; back_index = 0; length() = 1;
    // {Aryan, , , , };
    //   fb

    interviewees.enqueue("Zain");
    interviewees.enqueue("Virat");
    interviewees.enqueue("Ayat");
    interviewees.enqueue("Mithali");

    // front_index = 0; back_index = 4; length() = 5;
    // {Aryan, Zain, Virat, Ayat, Mithali};
    //    f                          b

    cout << "Interviewees: ";
    Queue<string>::display(interviewees);

    interviewees.dequeue();
    interviewees.dequeue();

    // front_index = 2; back_index = 4; length() = 3;
    // { , , Virat, Ayat, Mithali};
    //         f             b

    cout << "Interviewees: ";
    Queue<string>::display(interviewees);

    interviewees.enqueue("Abhimanyu");

    // front_index = 2; back_index = 0; length() = 4;
    // {Abhimanyu, , Virat, Ayat, Mithali};
    //     b            f

    interviewees.enqueue("Daya");

    // front_index = 2; back_index = 1; length() = 5;
    // {Abhimanyu, Daya, Virat, Ayat, Mithali}
    //               b      f

    cout << "Interviewees: ";
    Queue<string>::display(interviewees);

    cout << endl
         << endl;

    return 0;
}