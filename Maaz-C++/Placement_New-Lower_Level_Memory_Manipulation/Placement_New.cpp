#include <iostream>
using namespace std;

enum Typer
{
    BOOL,
    CHAR,
    SHORT_INT,
    INT,
    LONG_INT,
    FLOAT,
    DOUBLE,
    LONG_DOUBLE,
    STRING,
};

template <class var>
inline void show(var *a)
{
    cout << "This is a: " << (*a) << endl
         << "This is &a: " << a << endl;
}

int main()
{
    cout << endl
         << endl
         << "Hello, World!" << endl
         << endl;

    char *buffer = new char[64];

    enum Typer *a = new (buffer) Typer(FLOAT);
    show(a);
    a->~Typer();
    char *b = new (buffer) char('d');
    show(b);
    int *c = new (buffer) int(78);
    show(c);
    double *d = new (buffer) double(3.14159);
    show(d);
    string *e = new (buffer) string("STRINGING!!! LOL!!!");
    show(e);
    e->~string();

    cout << endl
         << "The size of the ENUM is: " << sizeof(enum Typer) << endl;

    delete buffer;

    // int *integers[4] = {new (memory_pool) int(12), new (memory_pool + sizeof(int)) int(45)};
    // integers[2] = new (integers[1] + sizeof(int)) int(72);
    // integers[3] = new (integers[2] + sizeof(int)) int(89);

    alignas(int) char memory_pool[sizeof(int) * 4];

    int *integers[4];
    int ints[4] = {12, 45, 72, 89};

    for (int i = 0; i < 4; i++)
        integers[i] = new ((i > 0 ? (integers[i - 1] + 1) : (int *)memory_pool)) int(ints[i]);
    // integers[i] = new (memory_pool + i * sizeof(int)) int(ints[i]);

    for (int i = 0; i < 4; i++)
        cout << "memory_pool[" << i << "]: " << (*integers[i]) << endl;
    cout << endl;

    return 0;
}