// =============================================================================
// 606. Construct String from Binary Tree
// =============================================================================
// 核心思路: 前序遍历二叉树, 子树用括号包裹
//          省略不必要的空括号, 但无左有右时必须保留左边的空括号 "()"
// =============================================================================

// =============================================================================
// 解法一: 递归 DFS (推荐, 最简洁)
// =============================================================================
// 前序遍历: 先输出节点值, 再递归处理左右子树
// 括号规则:
//   - 有左或有右 -> 左子树加括号 "(left)"
//   - 有右       -> 右子树加括号 "(right)"
//   - 叶子节点   -> 只输出值, 不加任何括号
// 时间: O(n), 空间: O(h) 递归栈
class Solution {
public:
    string tree2str(TreeNode* root) {
        if (!root) return "";

        string result = to_string(root->val);

        // 有左子树, 或者有右子树时左边需要空括号占位
        if (root->left || root->right) {
            result += "(" + tree2str(root->left) + ")";
        }
        // 有右子树
        if (root->right) {
            result += "(" + tree2str(root->right) + ")";
        }
        return result;
    }
};

// =============================================================================
// 解法二: 递归 + 引用优化 (避免字符串拷贝, 性能更优)
// =============================================================================
// 用 string 引用 += 追加, 避免递归中大量临时字符串创建
// 时间: O(n), 空间: O(h) 递归栈
class Solution_v2 {
public:
    string tree2str(TreeNode* root) {
        string result;
        dfs(root, result);
        return result;
    }

private:
    void dfs(TreeNode* node, string& s) {
        if (!node) return;

        s += to_string(node->val);

        if (node->left || node->right) {
            s += '(';
            dfs(node->left, s);
            s += ')';
        }
        if (node->right) {
            s += '(';
            dfs(node->right, s);
            s += ')';
        }
    }
};

// =============================================================================
// 解法三: 迭代 DFS (显式栈, 避免递归栈溢出)
// =============================================================================
// 用栈模拟前序遍历, visited 集合标记已处理节点
// 第一次遇到节点: 加 '(' + val, 压入子节点
// 第二次遇到节点(从栈弹出): 加 ')'
// 最后去掉最外层多余括号
// 时间: O(n), 空间: O(n) 栈 + visited 集合
class Solution_v3 {
public:
    string tree2str(TreeNode* root) {
        if (!root) return "";
        stack<TreeNode*> stk;
        stk.push(root);
        unordered_set<TreeNode*> visited;
        string result;

        while (!stk.empty()) {
            TreeNode* node = stk.top();
            if (visited.count(node)) {
                stk.pop();
                result += ')';
            } else {
                visited.insert(node);
                result += '(' + to_string(node->val);
                // 无左有右时添加空括号占位
                if (!node->left && node->right) {
                    result += "()";
                }
                // 栈是后进先出, 先压右再压左, 保证左先处理
                if (node->right) stk.push(node->right);
                if (node->left) stk.push(node->left);
            }
        }
        // 去掉最外层的 '(' 和 ')'
        return result.substr(1, result.size() - 2);
    }
};
