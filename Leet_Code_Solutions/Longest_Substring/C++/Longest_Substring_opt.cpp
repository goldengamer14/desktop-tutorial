#include <iostream>
#include "problem.h"

using namespace std;
class Solution
{
public:
    int lengthOfLongestSubstring(string s)
    {
        unsigned int all_length = s.length() + 1;
        unsigned int max_length = 0;

        if (all_length > 1)
        {
            string subs = "\0";

            unsigned int ind = 0;
            while (ind < all_length)
                for (unsigned int i = ind; i < all_length; i++)
                {
                    if (s[i] == '\0' && i >= all_length - 2)
                    {
                        i = ind;
                        ind++;
                    }
                    if (s[i] == '\0')
                        continue;

                    int find_index = subs.find(s[i]);
                    if (find_index >= 0 && find_index < all_length - 1)
                        subs = "\0";
                    else
                        subs += s[i];
                    if (subs.length() > max_length)
                    {
                        max_length = subs.length();
                    }
                    if ((find_index >= 0 && find_index < all_length - 1) ||
                        i >= all_length - 1)
                    {
                        i = ind;
                        ind++;
                    }
                }
        }
        return max_length;
    }
};

int main()
{
    cout << endl
         << "Hello, World!" << endl
         << endl;

    Solution s;
    int size = s.lengthOfLongestSubstring("abcabcbb");
    cout << endl;

    size = s.lengthOfLongestSubstring("bbbbb");
    cout << "size = " << size << endl;

    size = s.lengthOfLongestSubstring("pwwkew");
    cout << "size = " << size << endl;

    size = s.lengthOfLongestSubstring(" ");
    cout << "size = " << size << endl;

    size = s.lengthOfLongestSubstring("dvdf");
    cout << "size = " << size << endl;

    size = s.lengthOfLongestSubstring("");
    cout << "size = " << size << endl;

    size = s.lengthOfLongestSubstring("tmmzuxt");
    cout << "size = " << size << endl;

    size = s.lengthOfLongestSubstring(problem);
    cout << "size = " << size << endl;

    return 0;
}