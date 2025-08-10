#include <iostream>
#include <vector>
using namespace std;

template <class type>
vector<type> &Selection_Sort_ascend(vector<type> &arr)
{
    if (arr.size() < 2)
    {
        cerr << "Don't pass an empty or single element array." << endl;
        return arr;
    }

    for (unsigned int i = 0; i < arr.size() - 1; i++)
    {
        unsigned int min_index = i;
        for (unsigned int j = i + 1; j < arr.size(); j++)
            if (arr[j] < arr[min_index])
                min_index = j;
        swap(arr[i], arr[min_index]);
    }
    return arr;
}

int main()
{
    cout << "Hello, World!" << endl
         << endl;

    vector<int> arr = {64, 25, 12, 22, 11};

    cout << "Unsorted array: {";
    for (unsigned char i = 0; i < arr.size(); i++)
        cout << arr[i] << (i < arr.size() - 1 ? ", " : "}");
    cout << endl;

    Selection_Sort_ascend<int>(arr);

    cout << "Sorted array: {";
    for (unsigned char i = 0; i < arr.size(); i++)
        cout << arr[i] << (i < arr.size() - 1 ? ", " : "}");
    cout << endl;

    cout << endl;

    vector<char> arr_char = {'d', 'a', 'c', 'b'};

    cout << "Unsorted char array: {";
    for (unsigned char i = 0; i < arr_char.size(); i++)
        cout << arr_char[i] << (i < arr_char.size() - 1 ? ", " : "}");
    cout << endl;

    Selection_Sort_ascend<char>(arr_char);

    cout << "Sorted char array: {";
    for (unsigned char i = 0; i < arr_char.size(); i++)
        cout << arr_char[i] << (i < arr_char.size() - 1 ? ", " : "}");
    cout << endl;

    cout << endl;

    vector<int> arr_empty;
    Selection_Sort_ascend<int>(arr_empty);
    cout << endl;

    vector<int> arr_single = {42};
    Selection_Sort_ascend<int>(arr_single);
    cout << endl;

    cout << endl;

    return 0;
}