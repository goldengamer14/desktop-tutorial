#include <iostream>
#include "Stack_Linked_List.cpp"
using namespace std;

struct Stack_Element
{
    char oper;
    short int preced;
};

string infix_to_postfix(string exp)
{
    Stack<struct Stack_Element> *oper_holder = new Stack<struct Stack_Element>;

    struct Stack_Element this_char;
    string final_exp = "";

    for (int i = 0; i < exp.length(); i++)
    {
        switch (exp[i])
        {
        case '+':
            this_char.preced = 1;
            break;

        case '-':
            this_char.preced = 1;
            break;

        case '%':
            this_char.preced = 2;
            break;

        case '*':
            this_char.preced = 3;
            break;

        case '/':
            this_char.preced = 3;
            break;

        case '(':
            this_char.preced = 4;
            break;

        case ')':
            this_char.preced = 4;
            break;

        case ' ':
            continue;

        default:
            final_exp += exp[i];
            continue;
        }
        this_char.oper = exp[i];

        while (oper_holder->size() > 0 && oper_holder->stack_top().preced >= this_char.preced)
        {
            if (oper_holder->stack_top().oper == ')')
            {
                oper_holder->pop();
                while (oper_holder->size() > 0 && oper_holder->stack_top().oper != '(')
                {
                    final_exp += oper_holder->stack_top().oper;
                    oper_holder->pop();
                }
                oper_holder->pop();
                continue;
            }
            else if (oper_holder->stack_top().oper == '(')
                break;

            final_exp += oper_holder->stack_top().oper;
            oper_holder->pop();
        }
        oper_holder->push(this_char);
    }

    while (oper_holder->size() > 0)
    {
        if (oper_holder->stack_top().preced != 4)
            final_exp += oper_holder->stack_top().oper;
        oper_holder->pop();
    }

    delete oper_holder;
    return final_exp;
}

int main()
{
    cout << "Hello, World!" << endl
         << endl;

    // string exp = "a + b";
    // string exp = "x - y";
    // string exp = "p * q";
    // string exp = "s / t";
    // string exp = "x - y * z";
    // string exp = "p - q - r / a";
    string exp = "(m - n) * (p + q)";

    cout << "Infix: " << exp << endl
         << "Postfix: " << infix_to_postfix(exp) << endl
         << endl;

    return 0;
}