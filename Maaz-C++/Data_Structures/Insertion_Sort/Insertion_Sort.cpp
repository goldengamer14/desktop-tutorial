#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

template <class type>
vector<type> &insert_sort_ascend(vector<type> &arr, type data, unsigned int size)
{
    while (size > 0 && arr[size - 1] > data)
    {
        arr[size] = arr[size - 1];
        size--;
    }

    arr[size] = data;
    return arr;
}

int main()
{
    cout << "Hello, World!" << endl
         << endl;

    vector<int> arr_int(12);
    for (unsigned char i = 0; i < 12; i++)
        insert_sort_ascend<int>(arr_int, int(rand() % 100), i);

    cout << "Sorted array: {";
    for (unsigned char i = 0; i < 12; i++)
        cout << arr_int[i] << (i < 11 ? ", " : "}\n");
    cout << endl;

    vector<float> arr_float(12);
    for (unsigned char i = 0; i < 12; i++)
    {
        int rand_num = rand() % 100;
        float rand_float = static_cast<float>(rand_num) + static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        insert_sort_ascend<float>(arr_float, rand_float, i);
    }

    cout << "Sorted array: {";
    for (unsigned char i = 0; i < 12; i++)
        cout << arr_float[i] << (i < 11 ? ", " : "}\n");
    cout << endl;

    return 0;
}