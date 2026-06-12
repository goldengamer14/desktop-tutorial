#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <algorithm>
using namespace std;

/**
 * @param events Vector of {player, frame, attack_value}
 * @param H      Starting HP for both players
 * @return vector {hp1, hp2} each clamped to min 0
 */

/*
    Input: events: list of (player, frame, attack_value) in arbitrary order (due to
    network jitter)

    Output: player1_hp, player2_hp
    1: stable sort events by frame number
    2: for all event in events do
    3: if player1_hp ≤ 0 OR player2_hp ≤ 0 then
    4: break
    5: end if
    6: opponent_hp ← opponent_hp - event.attack_value
    7: end for
    8: return player1_hp, player2_hp

    list<int> processGame(list<tuple<int,int,int>> events, int H)
    • events: A list of tuples (player, frame, attack_value) where player is 1 or 2, frame
              is a non-negative integer, and attack_value is a positive integer.
    • H: HP for both players before the function call (a positive integer).
    • Return a list of two integers: [hp1, hp2], each clamped to a minimum of 0
*/

int compare_frames(const vector<int> &a, const vector<int> &b)
{
    return a[1] < b[1];
}

vector<int> processGame(vector<vector<int>> events, int H)
{

    stable_sort(events.begin(), events.end(), compare_frames);

    // TO BE REMOVED: Debug print of sorted events
    for (const vector<int> &event : events)
    {
        cout << setw(3) << event[1] << ". " << "Player " << event[0] << " attacks with " << setw(3) << event[2] << " damage.\n";
    }

    // TO BE REMOVED: Extra newline for better readability
    cout << "\n"
         << endl;

    vector<int> result(2, H);
    int &player1_hp = result[0],
        &player2_hp = result[1];

    for (int i = 0; i < events.size(); i++)
    {
        const int &attack_value = events[i][2];
        int &opponent_hp = (events[i][0] == 1) ? player2_hp : player1_hp;

        opponent_hp -= attack_value;
        if ((player1_hp <= 0 || player2_hp <= 0) && events[i][1] != events[i + 1][1])
            break;
    }

    // TO BE REMOVED: Extra newline for better readability
    cout << "\n"
         << endl;

    return result;
}

// --- Main execution block. DO NOT MODIFY ---
int main()
{
    try
    {
        int H, n;
        cin >> H >> n;
        vector<vector<int>> events(n, vector<int>(3));
        for (int i = 0; i < n; i++)
        {
            cin >> events[i][0] >> events[i][1] >> events[i][2];
        }

        vector<int> result = processGame(events, H);
        cout << result[0] << " " << result[1] << endl;
    }
    catch (const exception &e)
    {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
