#include <iostream>
#include <vector>
#include <string>
#include <sstream>

/**
 * Generates a geometric pattern on an n x n grid.
 *
 * Args:
 *     n:     Grid size (n x n, always odd for diamond)
 *     shape: Either "checkerboard" or "diamond"
 *
 * Returns:
 *     A 2D vector of integers (0 or 1).
 */
std::vector<std::vector<int>> generate_shape(int n, const std::string &shape)
{

    std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));

    if (shape == "checkerboard")
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                matrix[i][j] = (i & 1) ^ (j & 1);
            }
        }
    }
    else if (shape == "diamond")
    {
        const int middle = (n + 1) >> 1;
        int count = 1, lower_bound, upper_bound;

        for (int i = 0; i < middle - 1; ++i)
        {
            lower_bound = middle - (count >> 1);
            upper_bound = middle + (count >> 1);

            for (int j = 0; j < n; ++j)
            {
                matrix[i][j] = (j + 1 >= middle - (count >> 1) && j + 1 <= middle + (count >> 1));
            }
            count += 2;
        }

        for (int j = 0; j < n; j++)
            matrix[middle - 1][j] = 1;

        count = 1;
        for (int i = n - 1; i >= middle; --i)
        {
            lower_bound = middle - (count >> 1);
            upper_bound = middle + (count >> 1);

            for (int j = 0; j < n; ++j)
            {
                matrix[i][j] = (j + 1 >= lower_bound && j + 1 <= upper_bound);
            }
            count += 2;
        }
    }

    return matrix;
}

// --- Main execution block. DO NOT MODIFY ---
int main()
{
    try
    {
        std::string line;
        std::getline(std::cin, line);
        int n = std::stoi(line);

        std::string shape;
        std::getline(std::cin, shape);

        auto result = generate_shape(n, shape);
        for (int i = 0; i < (int)result.size(); i++)
        {
            for (int j = 0; j < (int)result[i].size(); j++)
            {
                if (j > 0)
                    std::cout << " ";
                std::cout << result[i][j];
            }
            std::cout << "\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
