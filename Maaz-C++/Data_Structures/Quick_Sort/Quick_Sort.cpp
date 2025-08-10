#include <iostream>
#include <vector>
using namespace std;

template <class type>
class Quick_Sort_class
{
    static unsigned int Partition(vector<type> &, unsigned int, unsigned int);
    /* {
        type pivot = arr[low];
        unsigned int i = low + 1, j = high;

        do
        {
            while (i <= high && arr[i] <= pivot)
                i++;
            while (j >= low && arr[j] > pivot)
                j--;

            if (i < j)
                swap(arr[i], arr[j]);
        } while (i < j);

        swap(arr[low], arr[j]);
        return j;
    } */

public:
    static vector<type> &Quick_Sort(vector<type> &, unsigned int, unsigned int);
    /* {
        if (low < high)
        {
            unsigned int partition_index = Quick_Sort_class<type>::Partition(arr, low, high);
            Quick_Sort_class<type>::Quick_Sort(arr, low, partition_index - 1);
            Quick_Sort_class<type>::Quick_Sort(arr, partition_index + 1, high);
        }

        return arr;
    } */
};

int main()
{
    cout << "Hello, World!" << endl
         << endl;

    vector<int> arr = {3, 6, 8, 10, 1, 2, 1};

    cout << "Unsorted array: {";
    for (unsigned char i = 0; i < arr.size(); i++)
        cout << arr[i] << (i < arr.size() - 1 ? ", " : "}");
    cout << endl;

    Quick_Sort_class<int>::Quick_Sort(arr, 0, arr.size() - 1);

    cout << "Sorted array: {";
    for (unsigned char i = 0; i < arr.size(); i++)
        cout << arr[i] << (i < arr.size() - 1 ? ", " : "}");
    cout << endl;

    cout << endl;

    vector<char> arr_char;
    {
        const char arr_temp[] = "quick sort example";
        arr_char.assign(arr_temp, arr_temp + sizeof(arr_temp) - 1);
    }

    cout << "Unsorted char array: {";
    for (unsigned char i = 0; i < arr_char.size(); i++)
        cout << arr_char[i] << (i < arr_char.size() - 1 ? ", " : "}");
    cout << endl;

    Quick_Sort_class<char>::Quick_Sort(arr_char, 0, arr_char.size() - 1);

    cout << "Sorted char array: {";
    for (unsigned char i = 0; i < arr_char.size(); i++)
        cout << arr_char[i] << (i < arr_char.size() - 1 ? ", " : "}");
    cout << endl;

    cout << endl;

    vector<float> arr_float = {3.1, 6.2, 8.3, 10.4, 1.5, 2.6, 1.7};

    cout << "Unsorted float array: {";
    for (unsigned char i = 0; i < arr_float.size(); i++)
        cout << arr_float[i] << (i < arr_float.size() - 1 ? ", " : "}");
    cout << endl;

    Quick_Sort_class<float>::Quick_Sort(arr_float, 0, arr_float.size() - 1);

    cout << "Sorted float array: {";
    for (unsigned char i = 0; i < arr_float.size(); i++)
        cout << arr_float[i] << (i < arr_float.size() - 1 ? ", " : "}");
    cout << endl;

    return 0;
}

template <class type>
vector<type> &Quick_Sort_class<type>::Quick_Sort(vector<type> &arr, unsigned int low, unsigned int high)
{
    if (low < high)
    {
        unsigned int partition_index = Quick_Sort_class<type>::Partition(arr, low, high);
        Quick_Sort_class<type>::Quick_Sort(arr, low, partition_index > 0 ? partition_index - 1 : 0);
        Quick_Sort_class<type>::Quick_Sort(arr, partition_index + 1, high);
    }

    return arr;
}

template <class type>
unsigned int Quick_Sort_class<type>::Partition(vector<type> &arr, unsigned int low, unsigned int high)
{
    type pivot = arr[low];
    unsigned int i = low + 1, j = high;

    do
    {
        while (i < high && arr[i] < pivot)
            i++;
        while (j > low && arr[j] > pivot)
            j--;

        if (i < j)
            swap(arr[i], arr[j]);
    } while (i < j);

    swap(arr[low], arr[j]);
    return j;
}