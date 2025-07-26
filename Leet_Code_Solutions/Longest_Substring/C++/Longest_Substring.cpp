#include <iostream>
#include <vector>
using namespace std;

class Solution
{
public:
    // Answer
    int lengthOfLongestSubstring(string s)
    {
        unsigned int all_length = s.length() + 1;

        unsigned int max_length = 0;
        string max_substring = "\0";

        if (all_length > 1)
        {
            string subs[all_length];

            subs[0] = "\0";
            unsigned int index = 0;

            int ind = 0;
            while (ind < all_length)
                for (int i = ind; i < all_length; i++)
                {
                    if (s[i] == '\0' || i >= all_length - 2)
                    {
                        i = ind;
                        ind++;
                    }
                    if (s[i] == '\0')
                        continue;

                    int find_index = subs[index].find(s[i]);
                    if (find_index >= 0 && find_index < all_length - 1)
                        subs[++index] = "\0";
                    else
                        subs[index] += s[i];

                    if (subs[index].length() > max_length)
                    {
                        max_length = subs[index].length();
                        max_substring = subs[index];
                    }

                    if ((find_index >= 0 && find_index < all_length - 1) ||
                        i >= all_length - 1)
                    {
                        i = ind;
                        ind++;
                    }
                }

            cout << endl;

            cout << "subs array = {";
            for (unsigned int i = 0; i <= index; i++)
                cout << subs[i] << (i < index ? ", " : "}\n");
        }
        cout << "The maximum substring = " << max_substring
             << " of size = " << max_length << " from " << s << endl;
        return max_length;
    }

    // Solution
    // int lengthOfLongestSubstring(string s)
    // {
    //     if (s.length() == 0 || s.length() == 1)
    //     {
    //         return s.length();
    //     }
    //     int left = 0;
    //     int right = 0;
    //     int ans = 0;
    //     vector<bool> vis(128, false);
    //     while (right < s.length())
    //     {
    //         while (vis[s[right]] == true)
    //         {
    //             vis[s[left]] = false;
    //             left++;
    //         }
    //         vis[s[right]] = true;
    //         ans = max(ans, right - left + 1);
    //         right++;
    //     }
    //     return ans;
    // }
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

    return 0;
}