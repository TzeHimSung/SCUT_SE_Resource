/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution
{
public:
    int ans=INT_MIN;
    int maxPathSum(TreeNode* root)
    {
        if (root==nullptr)
            return 0;
        else
        {
            solve(root);
            return ans;
        }
    }

    int solve(TreeNode* node)
    {
        if (node==nullptr)
            return 0;
        int currentValue=node->val;
        int lmax=solve(node->left), rmax=solve(node->right);
        if (lmax>0) currentValue+=lmax;
        if (rmax>0) currentValue+=rmax;
        ans=max(currentValue,ans);
        int re1=node->val+lmax, re2=node->val+rmax;
        return max(max(re1,re2),node->val);
    }
};
