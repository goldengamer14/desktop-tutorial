#include <iostream>
#include <cstring>
using namespace std;

template <class type>
class DE_Queue
{
private:
    type *arr;
    unsigned int max_size;
    unsigned int front;
    unsigned int back;

    template <class type_name>
    friend ostream &operator<<(ostream &os, DE_Queue<type_name> &self);

public:
    unsigned int size()
    {
        return (back >= front ? back - front : max_size - (front - back));
    }

    DE_Queue(unsigned int size)
    {
        if (size < 1)
            cerr << "You can't initialize an empty Queue!!!" << endl;
        max_size = size + 1;
        arr = new type[max_size]{0};
        front = back = 0;
    }

    DE_Queue &push(type data)
    {
        if ((back + 1) % max_size == front)
            cerr << "Queue Overflow!!!" << endl;
        else
        {
            arr[back] = data;
            back = (back + 1) % max_size;
        }

        return *this;
    }

    DE_Queue &unshift(type data)
    {
        if ((back + 1) % max_size == front)
            cerr << "Queue Overflow!!!" << endl;
        else
        {
            if (front < 1)
                front = max_size - 1;
            else
                front--;

            arr[front] = data;
        }

        return *this;
    }

    type pop()
    {
        type return_data;
        if (front == back)
        {
            cerr << "Queue Underflow!!!" << endl;
            memset(&return_data, 0, sizeof(return_data));
        }
        else
        {
            if (back < 1)
                back = max_size - 1;
            else
                back--;

            return_data = arr[back];
        }

        return return_data;
    }

    type shift()
    {
        type return_data;
        if (front == back)
        {
            cerr << "Queue Underflow!!!" << endl;
            memset(&return_data, 0, sizeof(return_data));
        }
        else
        {
            return_data = arr[front];
            front = (front + 1) % max_size;
        }

        return return_data;
    }

    ~DE_Queue()
    {
        if (arr && arr != nullptr)
            delete[] arr;
        front = back = max_size = 0;
    }
};

template <class type>
ostream &operator<<(ostream &os, DE_Queue<type> &self)
{
    if (self.front == self.back)
        os << "Empty Queue!!!" << endl;
    else
    {
        os << "{";
        for (unsigned int i = self.front; i != self.back; i = (i + 1) % self.max_size)
            os << self.arr[i] << ((i + 1) % self.max_size == self.back ? "}" : ", ");
        os << endl;
    }

    return os;
}

int main()
{
    cout << "Hello, World!" << endl
         << endl;

    DE_Queue<float> self(5);

    cout << "****PUSH****" << endl;
    self.push(32.525);
    self.push(47.479);
    self.push(5782.546);

    cout << self << endl;

    cout << "****POP****" << endl;
    cout << "Popped elements: {" << self.pop() << ", " << self.pop() << "}" << endl;

    cout << self << endl;

    cout << "****UNSHIFT****" << endl;
    self.unshift(7656.67679);
    self.unshift(547.5457);
    self.unshift(123.432);

    cout << self << endl;

    cout << "****SHIFT****" << endl;
    cout << "Shifted elements: {" << self.shift() << ", " << self.shift() << ", " << self.shift() << ", " << self.shift() << "}" << endl;

    cout << self << endl;

    return 0;
}