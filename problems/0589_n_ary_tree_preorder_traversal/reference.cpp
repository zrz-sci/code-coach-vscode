// 589. N-ary Tree Preorder Traversal
// 解法一: 递归  时间 O(n)  空间 O(h)

class Solution {
public:
    vector<int> preorder(Node* root) {
        vector<int> result;
        dfs(root, result);
        return result;
    }

private:
    void dfs(Node* node, vector<int>& result) {
        if (!node) return;
        result.push_back(node->val);           // 先访问根
        for (Node* child : node->children) {   // 再依次访问子节点
            dfs(child, result);
        }
    }
};

// ============================================================
// 解法二: 迭代（显式栈）  时间 O(n)  空间 O(n)
// 面试推荐: 展示栈的使用和对遍历顺序的理解
// ============================================================
/*
class Solution {
public:
    vector<int> preorder(Node* root) {
        vector<int> result;
        if (!root) return result;

        stack<Node*> stk;
        stk.push(root);

        while (!stk.empty()) {
            Node* node = stk.top();
            stk.pop();
            result.push_back(node->val);

            // 子节点从右到左入栈, 出栈时就是从左到右
            for (int i = node->children.size() - 1; i >= 0; --i) {
                if (node->children[i]) {
                    stk.push(node->children[i]);
                }
            }
        }
        return result;
    }
};
*/
