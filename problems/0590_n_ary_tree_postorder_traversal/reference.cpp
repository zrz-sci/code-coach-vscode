// 590. N-ary Tree Postorder Traversal
// 解法一: 递归  时间 O(n)  空间 O(h)

class Solution {
public:
    vector<int> postorder(Node* root) {
        vector<int> result;
        dfs(root, result);
        return result;
    }

private:
    void dfs(Node* node, vector<int>& result) {
        if (!node) return;
        for (Node* child : node->children) {   // 先访问所有子节点
            dfs(child, result);
        }
        result.push_back(node->val);           // 最后访问根
    }
};

// ============================================================
// 解法二: 迭代 + 反转  时间 O(n)  空间 O(n)
// 核心: "根-右-左"的收集顺序反转后就是"左-右-根"(后序)
// ============================================================
/*
class Solution {
public:
    vector<int> postorder(Node* root) {
        vector<int> result;
        if (!root) return result;

        stack<Node*> stk;
        stk.push(root);

        while (!stk.empty()) {
            Node* node = stk.top();
            stk.pop();
            result.push_back(node->val);

            // 子节点从左到右入栈 -> 出栈从右到左 -> 收集"根-右-左"
            for (Node* child : node->children) {
                if (child) stk.push(child);
            }
        }
        // 反转 "根-右-左" -> "左-右-根" = 后序
        reverse(result.begin(), result.end());
        return result;
    }
};
*/

// ============================================================
// 解法三: 迭代 + 标记法 (真正的后序迭代遍历)
// 使用 pair<Node*, bool> 标记节点是否已展开过子节点
// ============================================================
/*
class Solution {
public:
    vector<int> postorder(Node* root) {
        vector<int> result;
        if (!root) return result;

        stack<pair<Node*, bool>> stk;
        stk.push({root, false});

        while (!stk.empty()) {
            auto [node, visited] = stk.top();
            stk.pop();

            if (visited) {
                // 子节点已全部处理完, 现在处理根
                result.push_back(node->val);
            } else {
                // 先将根标记为待访问, 压回栈底
                stk.push({node, true});
                // 子节点从右到左入栈, 出栈时从左到右
                for (int i = node->children.size() - 1; i >= 0; --i) {
                    if (node->children[i]) {
                        stk.push({node->children[i], false});
                    }
                }
            }
        }
        return result;
    }
};
*/
