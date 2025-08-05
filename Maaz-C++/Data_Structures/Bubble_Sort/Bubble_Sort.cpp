#include <iostream>
#include <vector>
using namespace std;

template <class type>
vector<type>& Bubble_Sort_ascend(vector<type> &arr, unsigned int n)
{
    if (n > 1)
    {
        signed char swapped;
        do
        {
            swapped = 0;
            for (unsigned int j = 0; j < n - 1; j++)
            {
                if (arr[j] > arr[j + 1])
                {
                    swap(arr[j], arr[j + 1]);
                    swapped = 1;
                }
            }
        } while (--n > 1 && swapped);
    }
    return arr;
}

int main()
{
    cout << "\nHello, World!\n"
         << endl;

    vector<int> arr_int = {64, 34, 25, 12, -59, 22, 11, 90};
    Bubble_Sort_ascend(arr_int, arr_int.size());
    cout << "Sorted integer array: \n{";
    for (int i = 0; i < arr_int.size(); i++)
        cout << arr_int[i] << (i < arr_int.size() - 1 ? ", " : "}\n");

    vector<double> arr_double = {64.5, 34.2, 25.1, 12.3, 22.4, 11.0, 90.6, -43.2};
    Bubble_Sort_ascend(arr_double, arr_double.size());
    cout << "Sorted double array: \n{";
    for (int i = 0; i < arr_double.size(); i++)
        cout << arr_double[i] << (i < arr_double.size() - 1 ? ", " : "}\n");
    cout << endl;

    return 0;
}