#include <iostream>
#include <cstring>
#include "Linked_List.cpp"
using namespace std;

class String
{
    friend ostream &operator<<(ostream &os, const String &v);
    Linked_List<char> *str = nullptr;

public:
    String()
    {
        str = new Linked_List<char>;
    }

    String(char exp[])
    {
        str = new Linked_List<char>;
        for (int i = 0; i < strlen(exp); i++)
            str->push(exp[i]);
    }

    String(String &v)
    {
        str = new Linked_List<char>;

        for (Linked_List<char> *ptr = v.str; ptr != nullptr; ptr = ptr->next)
        {
            str->push(ptr->get_data());
        }
    }

    String operator+(String &v)
    {
        String new_str;
        for (Linked_List<char> *ptr = str; ptr != nullptr; ptr = ptr->next)
        {
            new_str.str->push(ptr->get_data());
        }
        for (Linked_List<char> *ptr = v.str; ptr != nullptr; ptr = ptr->next)
        {
            new_str.str->push(ptr->get_data());
        }

        return new_str;
    }

    String operator+(char ch)
    {
        String new_str;
        for (Linked_List<char> *ptr = str; ptr != nullptr; ptr = ptr->next)
        {
            new_str.str->push(ptr->get_data());
        }
        new_str.str->push(ch);

        return new_str;
    }

    String operator+(char exp[])
    {
        String new_str;
        for (Linked_List<char> *ptr = str; ptr != nullptr; ptr = ptr->next)
        {
            new_str.str->push(ptr->get_data());
        }
        for (long int i = 0; i < strlen(exp); i++)
        {
            new_str.str->push(exp[i]);
        }
        return new_str;
    }

    String& operator+=(String &v)
    {
        for (Linked_List<char> *ptr = v.str; ptr != nullptr; ptr = ptr->next)
        {
            str->push(ptr->get_data());
        }
        return *this;
    }
    
    String& operator+=(char ch)
    {
        str->push(ch);
        return *this;
    }
    
    String& operator+=(char exp[])
    {
        for (long int i = 0; i < strlen(exp); i++)
        {
            str->push(exp[i]);
        }
        return *this;
    }
    
    String& operator=(String &v)
    {
        delete str;
        str = new Linked_List<char>;
        
        for (Linked_List<char> *ptr = v.str; ptr != nullptr; ptr = ptr->next)
        {
            str->push(ptr->get_data());
        }
        return *this;
    }
    
    String& operator=(char exp[])
    {
        delete str;
        str = new Linked_List<char>;
        
        for (long int i = 0; i < strlen(exp); i++)
        {
            str->push(exp[i]);
        }
        return *this;
    }
    
    char operator[](long int index)
    {
        if (index >= str->length())
        throw bad_cast();

        Linked_List<char> *ptr = str;
        for (long int i = 0; i <= index; i++)
            ptr = ptr->next;

        return ptr->get_data();
    }

    operator char()
    {
        return str->get_data();
    }

    operator string()
    {
        string ret_str = "";
        for (Linked_List<char> *ptr = str; ptr != nullptr; ptr = ptr->next)
            ret_str += ptr->get_data();

        return ret_str;
    }

    ~String()
    {
        delete str;
    }
};

ostream &operator<<(ostream &os, const String &v)
{
    for (Linked_List<char> *ptr = v.str; ptr != nullptr; ptr = ptr->next)
    {
        os << ptr->get_data();
    }
    return os;
}

int main()
{
    cout << endl
         << "Hello, World!" << endl
         << endl;

    char my_str[] = "Ohio";
    String my_exp(my_str);
    cout << "my_exp: " << my_exp << endl;

    char your_str[] = " Dattebayo!";
    String your_exp(your_str);
    cout << "my_exp + ',' + your_exp: " << my_exp + ',' + your_exp << endl;

    char Akiramene[] = " Akiramene!";
    cout << "your_exp + \" Akiramene!\": " << your_exp + Akiramene << endl;

    your_exp += Akiramene;
    cout << "your_exp += \" Akiramene!\": " << your_exp << endl;

    char Dattebayo[] = "Dattebayo!";
    your_exp = Dattebayo;
    cout << "your_exp = \"Dattebayo!\":" << your_exp << endl;

    my_exp += your_exp;
    cout << "my_exp += your_exp: " << my_exp << endl;
    my_exp += '\\';
    cout << "my_exp += \\:\t    " << my_exp << endl;

    my_exp = your_exp;
    cout << "my_exp = your_exp: " << my_exp << endl;

    cout << "my_exp[4]:\t" << my_exp[4] << endl
         << endl;

    return 0;
}