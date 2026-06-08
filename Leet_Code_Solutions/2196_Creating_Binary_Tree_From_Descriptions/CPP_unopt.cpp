/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left),
 * right(right) {}
 * };
 */

class Solution {
public:
    TreeNode* equal_TreeNode(TreeNode* x, int val) {
        int8_t a = 0;
        TreeNode* matched_node = nullptr;

        if (x->left) {
            a |= 1;
            matched_node = equal_TreeNode(x->left, val);
            if (matched_node)
                return matched_node;
        }

        if (x->right) {
            a |= 2;
            matched_node = equal_TreeNode(x->right, val);
            if (matched_node)
                return matched_node;
        }

        if (a == 0 && x->val == val) {
            return x;
        }

        return nullptr;
    }

    static void TreeNode_traversal(TreeNode* x) {
        if (x == nullptr) {
            return;
        }

        TreeNode_traversal(x->left);

        cout << x->val << "\n";

        TreeNode_traversal(x->right);
    }

    TreeNode* createBinaryTree(vector<vector<int>>& descriptions) {

        vector<TreeNode> trees(descriptions.size(), TreeNode(-1));
        vector<bool> used((int)1e5 | 1);

        int trees_curr = 0;

        for (vector<int>& x : descriptions) {

            if (!used[x[0]]) {
                trees[trees_curr] = TreeNode(x[0]);
                *(x[2] ? &trees[trees_curr].left : &trees[trees_curr].right) =
                    new TreeNode(x[1]);

                ++trees_curr;
                used[x[0]] = true;
                // cout << "Used Parent: " << x[0] << "\n";
            } else {
                for (TreeNode& y : trees) {
                    if (y.val == x[0]) {
                        *(x[2] ? &y.left : &y.right) = new TreeNode(x[1]);

                        break;
                    }
                }
            }
        }

        // for (int i = 0; i < trees_curr; ++i) {
        //     bool left_half = !trees[i].left ? 1 : 0,
        //          right_half = !trees[i].right ? 1 : 0;

        //     cout << "trees[" << i << "] = " << trees[i].val << "\ntrees[" << i
        //          << "].left = " << (!left_half ? trees[i].left->val : -1)
        //          << "\ntrees[" << i
        //          << "].right = " << (!right_half ? trees[i].right->val : -1)
        //          << "\n";
        // }

        for (int i = 0; i < trees_curr; ++i) {
            int j = 0;
            TreeNode* child = nullptr;

            // cout << "Into the while loop:\n";
            do {
                child = equal_TreeNode(&trees[i], trees[j].val);
                ++j;
            } while (!child && j < trees_curr);

            if (child) {
                int matched = j - 1;

                // bool left_null = !trees[i].left ? 1 : 0,
                //      right_null = !trees[i].right ? 1 : 0;
                // bool left_null_matched = !trees[matched].left ? 1 : 0,
                //      right_null_matched = !trees[matched].right ? 1 : 0;

                // cout << "trees[i=" << i << "] = " << trees[i].val
                //      << "\ntrees[i=" << i
                //      << "].left = " << (!left_null ? trees[i].left->val : -1)
                //      << "\ntrees[i=" << i
                //      << "].right = " << (!right_null ? trees[i].right->val : -1)
                //      << "\n"
                //      << "trees[matched=" << matched
                //      << "] = " << trees[matched].val
                //      << "\ntrees[matched=" << matched << "].left = "
                //      << (!left_null_matched ? trees[matched].left->val : -1)
                //      << "\ntrees[matched=" << matched << "].right = "
                //      << (!right_null_matched ? trees[matched].right->val : -1)
                //      << "\n";

                // cout << "Matched " << matched << " For " << i << "\n";

                child->left = trees[matched].left;
                child->right = trees[matched].right;

                // cout << "\n\n";
                // Solution::TreeNode_traversal(&trees[i]);
                // cout << "\n\n";

                int k = matched;
                while (k < trees_curr - 1) {
                    trees[k] = trees[k + 1];
                    ++k;
                }
                --trees_curr;
                i = -1;
            } else {
                // cout << "Found no match for trees[i=" << i << "]";
                // Solution::TreeNode_traversal(&trees[i]);
            }
        }

        // cout << "\n\n";
        // for (TreeNode& y : trees) {
        //     Solution::TreeNode_traversal(&y);
        //     cout << "\n\n";
        // }

        TreeNode* head =
            new TreeNode(trees[0].val, trees[0].left, trees[0].right);

        // Solution::TreeNode_traversal(head);

        return head;
    }
};

