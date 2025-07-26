#include <iostream>
using namespace std;

template <class var>
class Stack
{
    long int size = 0;
    long int top = -1;

    var *arr = nullptr;

public:
    Stack(long int size) : size(size), arr(new var[size]) {}

    void push(var data)
    {
        if (top == size - 1)
        {
            cout << "The stack is already FULL!" << endl
                 << endl;
            return;
        }

        arr[++top] = data;
    }

    void pop()
    {
        if (top == -1)
        {
            cout << "The stack is already EMPTY!" << endl
                 << endl;
            return;
        }

        top--;
    }

    void resize(long int new_size)
    {
        var *old_arr = arr;
        arr = new var[new_size];

        new_size < top ? top = new_size - 1 : 0;

        for (long int i = 0; i <= top; i++)
        {
            arr[i] = old_arr[i];
        }

        delete[] old_arr;

        size = new_size;
    }

    var stack_top()
    {
        return arr[top];
    }

    var stack_bottom()
    {
        return arr[0];
    }

    var peek(int i)
    {
        if (top + 1 - i < 0)
        {
            cout << "Invalid index in peek function!!!" << endl
                 << endl;
            return arr[top];
        }

        return arr[top + 1 - i];
    }

    void display_Stack()
    {
        cout << "{";
        for (long int i = top; i > -1; i--)
        {
            cout << arr[i] << (i > 0 ? ", " : "");
        }
        cout << "}" << endl
             << endl;
    }

    long int length() { return top + 1; }

    ~Stack()
    {
        delete[] arr;

        cout << "Stack of the type \"" << typeid(*arr).name() << "\" Destroyed!!!" << endl
             << endl;
    }
};

int main()
{
    cout << "Hello, World!" << endl
         << endl;

    Stack<int> *stack_int = new Stack<int>(6);

    stack_int->push(5);
    stack_int->push(67);
    stack_int->push(289);
    stack_int->push(34);
    stack_int->push(123);
    stack_int->push(764);

    stack_int->display_Stack();

    stack_int->pop();
    stack_int->pop();

    stack_int->display_Stack();

    for (int i = 1; i <= stack_int->length() + 1; i++)
    {
        cout << "The " << i << "th element of the stack is: " << stack_int->peek(i) << endl;
    }
    cout << endl;

    cout << "The topmost element of stack is: " << stack_int->stack_top() << endl;
    cout << "The bottommost element of stack is: " << stack_int->stack_bottom() << endl
         << endl;

    delete stack_int;
    stack_int = nullptr;

    Stack<char> *stack_char = new Stack<char>(8);

    stack_char->push('b');
    stack_char->push('v');
    stack_char->push('a');
    stack_char->push('y');
    stack_char->push('p');
    stack_char->push('x');
    stack_char->push('z');
    stack_char->push('i');

    stack_char->display_Stack();

    stack_char->pop();
    stack_char->pop();

    stack_char->display_Stack();

    stack_char->resize(3);

    stack_char->display_Stack();

    for (int i = 1; i <= stack_char->length() + 1; i++)
    {
        cout << "The " << i << "th element of the stack is: " << stack_char->peek(i) << endl;
    }
    cout << endl;

    cout << "The topmost element of stack is: " << stack_char->stack_top() << endl;
    cout << "The bottommost element of stack is: " << stack_char->stack_bottom() << endl
         << endl;

    delete stack_char;
    stack_char = nullptr;

    return 0;
}