class Solution {
public:
    int countUnguarded(int m, int n, vector<vector<int>>& guards,
                       vector<vector<int>>& walls) {
        int total_unguarded_cells = 0;
        const int guards_size = guards.size(), walls_size = walls.size();
        vector<vector<char>> cell_matrix(m, vector<char>(n, 'u'));

        for (int w = 0; w < walls_size; w++) {
            vector<int>& wall = walls[w];
            cell_matrix[wall[0]][wall[1]] = 'w';
        }

        for (int g = 0; g < guards_size; g++) {
            vector<int>& guard = guards[g];

            int c = guard[0], r = guard[1];

            while (c < m && cell_matrix[c][r] != 'w')
                cell_matrix[c++][r] = 'g';

            c = guard[0] - 1;

            while (c >= 0 && cell_matrix[c][r] != 'w')
                cell_matrix[c--][r] = 'g';

            c = guard[0];

            while (r < n && cell_matrix[c][r] != 'w')
                cell_matrix[c][r++] = 'g';

            r = guard[1] - 1;

            while (r >= 0 && cell_matrix[c][r] != 'w')
                cell_matrix[c][r--] = 'g';
        }

        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) 
                if (cell_matrix[i][j] == 'u')
                    total_unguarded_cells++;

        // for (int i = 0; i < m; i++)
        //     for (int j = 0; j < n; j++)
        //         cout << "cell_matrix[" << i << "[" << j
        //              << "] = " << cell_matrix[i][j] << "\n";

        return total_unguarded_cells;
    }
};