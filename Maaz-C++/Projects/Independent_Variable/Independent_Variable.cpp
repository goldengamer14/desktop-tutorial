#include <iostream>
using namespace std;

class Type_Var
{
    enum
    {
        INT,
        CHAR,
        STRING,
        DOUBLE
    } active_type;

    union U
    {
        int integer;
        char character;
        string str;
        double dbl;

        U() {};
        ~U() {};
    } u;

    void destruct_string()
    {
        if (active_type == STRING)
            u.str.~string();
    }

public:
    Type_Var()
    {
        u.integer = int();
        active_type = INT;
    }

    Type_Var &operator=(int data)
    {
        destruct_string();
        u.integer = data;
        active_type = INT;
        return *this;
    }
    Type_Var &operator=(char data)
    {
        destruct_string();
        u.character = data;
        active_type = CHAR;
        return *this;
    }
    Type_Var &operator=(string data)
    {
        destruct_string();
        new (&u.str) string(data);
        active_type = STRING;
        return *this;
    }
    Type_Var &operator=(double data)
    {
        destruct_string();
        u.dbl = data;
        active_type = DOUBLE;
        return *this;
    }

    operator int() const
    {
        if (active_type == INT || active_type == DOUBLE)
            return u.integer;
        throw bad_cast();
    }

    operator char() const
    {
        if (active_type == CHAR)
            return u.character;
        throw bad_cast();
    }

    operator string() const
    {
        if (active_type == STRING)
            return u.str;
        throw bad_cast();
    }

    operator double() const
    {
        if (active_type == DOUBLE)
            return u.dbl;
        else if (active_type == INT)
            return u.integer;
        throw bad_cast();
    }

    operator float() const
    {
        if (active_type == DOUBLE)
            return u.dbl;
        else if (active_type == INT)
            return u.integer;
        throw bad_cast();
    }

    friend ostream &operator<<(ostream &os, const Type_Var &v);

    ~Type_Var()
    {
        destruct_string();
    }
};

ostream &operator<<(ostream &os, const Type_Var &v)
{
    switch (v.active_type)
    {
    case Type_Var::INT:
        os << v.u.integer;
        break;
    case Type_Var::CHAR:
        os << v.u.character;
        break;
    case Type_Var::DOUBLE:
        os << v.u.dbl;
        break;
    case Type_Var::STRING:
        os << v.u.str;
        break;
    default:
        os << "[unknown type]";
    }
    return os;
}

int main()
{
    cout << "Hello, World!" << endl
         << endl;

    Type_Var variable;

    variable = 45;
    cout << variable << endl;

    variable = 'f';
    cout << variable << endl;

    variable = "Hello, World!";
    cout << variable << endl;

    variable = 4.5;
    cout << variable << endl;

    variable = (double)45;
    cout << variable << endl;

    return 0;
}

// void change_active_type_to(string data)
// {
//     destruct_string();
//     new (&u.str) string(data);
//     active_type = STRING;
// }

// template <class var>
// void change_active_type_to(var typeiy)
// {
//     destruct_string();
//     switch (typeid(typeiy).name())
//     {
//     case 'i':
//         u.integer = data;
//         active_type = INT;
//         break;
//     case 'c':
//         u.character = data;
//         active_type = CHAR;
//         break;
//     case 'd':
//         u.dbl = data;
//         active_type = DOUBLE;
//         break;
//     default:
//         break;
//     }
// }

// template <class var>
// var operator~()
// {
//     switch (active_type)
//     {
//     case INT:
//         return u.integer;
//     case CHAR:
//         return u.character;
//     case STRING:
//         return u.str;
//     case DOUBLE:
//         return u.dbl;
//     default:
//         break;
//     }
// }