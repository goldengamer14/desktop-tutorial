/*
	// Runtime: 181 ms, Beats 75.89% -> Time Complexity: O(N^2)
	// Memory: 284.24 MB, Beats 32.81% -> Space Complexity O(N)
*/

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
        unordered_map<int, int> used_parent;
        unordered_map<int, TreeNode*> child_tree;

        int trees_curr = 0;

        for (vector<int>& x : descriptions) {

            if (!used_parent.contains(x[0])) {
                trees[trees_curr] = TreeNode(x[0]);

                child_tree[x[1]] = *(x[2] ? &trees[trees_curr].left
                                          : &trees[trees_curr].right) =
                    new TreeNode(x[1]);

                used_parent[x[0]] = trees_curr;
                // cout << "Used Parent: {" << x[0] << ", " << trees_curr << "}"
                // << "\n";
                ++trees_curr;
            } else {
                child_tree[x[1]] = *(x[2] ? &trees[used_parent[x[0]]].left
                                          : &trees[used_parent[x[0]]].right) =
                    new TreeNode(x[1]);
            }
        }

        // for (int i = 0; i < trees_curr; ++i) {
        //     bool left_half = !trees[i].left ? 1 : 0,
        //          right_half = !trees[i].right ? 1 : 0;

        //     cout << "trees[" << i << "] = " << trees[i].val << "\ntrees[" <<
        //     i
        //          << "].left = " << (!left_half ? trees[i].left->val : -1)
        //          << "\ntrees[" << i
        //          << "].right = " << (!right_half ? trees[i].right->val : -1)
        //          << "\n";
        // }

        for (int i = 0; i < trees_curr;) {
            unordered_map<int, TreeNode *>::const_iterator it = child_tree.find(trees[i].val);
            if (it != child_tree.end()) {
                TreeNode* child = it->second;
                child->left = trees[i].left;
                child->right = trees[i].right;
                child_tree.erase(it);
                trees[i] = trees[--trees_curr];
            } else {
                ++i;
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

