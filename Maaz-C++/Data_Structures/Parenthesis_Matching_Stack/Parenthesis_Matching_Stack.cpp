#include <iostream>
#include "Stack_Linked_List.cpp"
using namespace std;

struct Two_Holding
{
    bool boolean = true;
    Stack<int> *sp = nullptr;
    Stack<int> *sp_closed = nullptr;
};

struct Two_Holding Parenthesis_Match(string exp)
{
    Stack<int> *sp_ptr = new Stack<int>;
    Stack<int> &sp = (*sp_ptr);
    Stack<int> *sp_closed_ptr = new Stack<int>;
    Stack<int> &sp_closed = (*sp_closed_ptr);
    struct Two_Holding returning;

    for (int i = 0; i < exp.size(); i++)
    {
        if (exp[i] == '(')
        {
            sp.push(i);
        }
        else if (exp[i] == ')')
        {
            if (sp_closed.size() > sp.size())
                returning.boolean = false;
            sp_closed.push(i);
        }
    }

    if (sp.size() != sp_closed.size())
        returning.boolean = false;

    returning.sp = sp_ptr;
    returning.sp_closed = sp_closed_ptr;

    return returning;
}

int main()
{
    cout << endl
         << endl
         << "Hello, World!" << endl
         << endl;

    // string my_exp = "(6 (+ 8 (* (9 - 0))";
    // string my_exp = "6 + 8 (* (9) - )0))";
    string my_exp = "(6 + 8 * (9 - 0))";

    struct Two_Holding result = Parenthesis_Match(my_exp);

    cout << "The expression is " << my_exp.length() << " characters long" << endl
         << endl;

    cout << "The expression is " << (result.boolean ? "Balanced!!!\n" : "Imbalanced!!!") << endl;

    if (!result.boolean)
    {
        cout << endl;

        if (result.sp->size() > result.sp_closed->size())
        {
            cout << "Open brackets at the indices: ";
            for (int i = 0; i < result.sp->size() - result.sp_closed->size(); i++)
            {
                cout << result.sp->peek(i) << (i < result.sp->size() - result.sp_closed->size() - 1 ? ", " : "");
            }
        }
        else if (result.sp_closed->size() > result.sp->size())
        {
            cout << "Closed brackets at the indices: ";
            for (int i = 0; i < result.sp_closed->size() - result.sp->size(); i++)
            {
                cout << result.sp_closed->peek(i) << (i < result.sp_closed->size() - result.sp->size() - 1 ? ", " : "");
            }
        }

        cout << endl
             << endl;
    }
    delete result.sp;
    delete result.sp_closed;

    cout << endl;

    return 0;
}