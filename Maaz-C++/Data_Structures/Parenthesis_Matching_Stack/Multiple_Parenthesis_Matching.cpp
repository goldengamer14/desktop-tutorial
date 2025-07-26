#include <iostream>
#include "Stack_Linked_List.cpp"
using namespace std;

struct Two_Holding
{
    struct Stack_Element
    {
        enum Braces
        {
            curly,
            square,
            curved
        };
        enum Braces brace_type;
        int index;
    };
    bool valid = true;
    Stack<int> *invalid_indices = nullptr;
};

struct Two_Holding Parenthesice_Match(string exp)
{
    struct Two_Holding returning;
    returning.invalid_indices = new Stack<int>;

    Stack<struct Two_Holding::Stack_Element> *push_pop_stack = new Stack<struct Two_Holding::Stack_Element>;
    struct Two_Holding::Stack_Element brace_here;
    bool open;

    for (int i = 0; i < exp.size(); i++)
    {
        switch (exp[i])
        {
        case '(':
            brace_here.brace_type = Two_Holding::Stack_Element::curved;
            open = true;
            break;

        case ')':
            brace_here.brace_type = Two_Holding::Stack_Element::curved;
            open = false;
            break;

        case '[':
            brace_here.brace_type = Two_Holding::Stack_Element::square;
            open = true;
            break;

        case ']':
            brace_here.brace_type = Two_Holding::Stack_Element::square;
            open = false;
            break;
        case '{':
            brace_here.brace_type = Two_Holding::Stack_Element::curly;
            open = true;
            break;
        case '}':
            brace_here.brace_type = Two_Holding::Stack_Element::curly;
            open = false;
            break;

        default:
            continue;
        };

        brace_here.index = i;

        if (open)
        {
            push_pop_stack->push(brace_here);
        }
        else if (push_pop_stack->size() > 0 && push_pop_stack->stack_top().brace_type == brace_here.brace_type)
        {
            push_pop_stack->pop();
        }
        else if (push_pop_stack->size() > 1)
        {
            Stack<struct Two_Holding::Stack_Element> *temp_sp = new Stack<struct Two_Holding::Stack_Element>;
            do
            {
                temp_sp->push(push_pop_stack->stack_top());
                push_pop_stack->pop();
            } while (push_pop_stack->stack_top().brace_type != brace_here.brace_type && push_pop_stack->size() > 0);

            if (push_pop_stack->stack_top().brace_type == brace_here.brace_type)
            {
                push_pop_stack->pop();
                for (int i = 0; i < temp_sp->size(); i++)
                {
                    push_pop_stack->push(temp_sp->stack_top());
                    temp_sp->pop();
                }
            }

            delete temp_sp;
        }
        else
        {
            returning.invalid_indices->push(i);
            returning.valid = false;
        }
    };

    push_pop_stack->size() > 0 || returning.invalid_indices->size() > 0 ? returning.valid = false : 0;

    for (int i = 0; i < push_pop_stack->size(); i++)
    {
        returning.invalid_indices->push(push_pop_stack->stack_top().index);
        push_pop_stack->pop();
    }

    delete push_pop_stack;

    return returning;
}

int main()
{
    cout << endl
         << "Hello, World!" << endl
         << endl;

    // string my_exp = "{7 - (3 - 2) + [8 + (99 - 11)]}";
    // string my_exp = "{7 - 3 - 2) + [8 + (99 - 11)]}";
    // string my_exp = "{7 - 3} - 2) + [8 + (99 - 11)]}";
    string my_exp = "{7 - [(3 - 2) + [8 + (99 - 11)]}";

    struct Two_Holding result = Parenthesice_Match(my_exp);

    cout << "The expression " << my_exp << " is " << (result.valid ? "Balanced!!!" : "Imbalanced!!!") << endl;

    if (!result.valid)
    {
        cout << "At the indices: ";
        while (result.invalid_indices->size() > 0)
        {
            cout << result.invalid_indices->stack_top() << (result.invalid_indices->size() > 1 ? ", " : "");
            result.invalid_indices->pop();
        }
        cout << endl;
    };
    cout << endl;

    delete result.invalid_indices;

    return 0;
}