/*
 * 【题目本质】
 * 利用 BST 的有序性质，用最紧凑的方式序列化/反序列化。
 * 核心：前序遍历 + 值域范围递归重建，无需 null 标记。
 *
 * 【解法总览】
 * Solution1: 前序遍历 + 值域递归 O(n)/O(n) ⭐ 最紧凑
 * Solution2: BFS 层序 + null标记 O(n)/O(n) 通用但冗余
 */

// ===================== Solution 1: 前序遍历 + 值域递归 =====================
// 序列化：前序 DFS，逗号分隔
// 反序列化：维护索引 idx 和值域 [lo, hi]，值在范围内才创建节点
class Codec1 {
public:
    string serialize(TreeNode* root) {
        string s;
        serHelper(root, s);
        return s;
    }

    TreeNode* deserialize(string data) {
        vector<int> vals;
        stringstream ss(data);
        string token;
        while (getline(ss, token, ',')) {
            if (!token.empty()) vals.push_back(stoi(token));
        }
        int idx = 0;
        return desHelper(vals, idx, INT_MIN, INT_MAX);
    }

private:
    void serHelper(TreeNode* node, string& s) {
        if (!node) return;
        s += to_string(node->val) + ",";
        serHelper(node->left, s);
        serHelper(node->right, s);
    }

    TreeNode* desHelper(vector<int>& vals, int& idx, int lo, int hi) {
        if (idx >= (int)vals.size() || vals[idx] < lo || vals[idx] > hi)
            return nullptr;
        TreeNode* node = new TreeNode(vals[idx++]);
        node->left  = desHelper(vals, idx, lo, node->val);
        node->right = desHelper(vals, idx, node->val, hi);
        return node;
    }
};

// ===================== Solution 2: BFS 层序 + null 标记 =====================
// 标准 BFS 序列化，null 用 "#" 标记
// 适用于通用二叉树，但对 BST 不够紧凑
class Codec2 {
public:
    string serialize(TreeNode* root) {
        if (!root) return "";
        string s;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            auto node = q.front(); q.pop();
            if (node) {
                s += to_string(node->val) + ",";
                q.push(node->left);
                q.push(node->right);
            } else {
                s += "#,";
            }
        }
        return s;
    }

    TreeNode* deserialize(string data) {
        if (data.empty()) return nullptr;
        stringstream ss(data);
        string token;
        getline(ss, token, ',');
        TreeNode* root = new TreeNode(stoi(token));
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            auto node = q.front(); q.pop();
            if (getline(ss, token, ',') && token != "#") {
                node->left = new TreeNode(stoi(token));
                q.push(node->left);
            }
            if (getline(ss, token, ',') && token != "#") {
                node->right = new TreeNode(stoi(token));
                q.push(node->right);
            }
        }
        return root;
    }
};

/*
 * 【解法对比】
 * Solution1: 前序无 null 标记，编码最紧凑，利用 BST 值域递归 O(n)
 * Solution2: BFS 带 null 标记，通用但冗余，适合通用二叉树
 *
 * 【易错点】
 * 1. 反序列化 idx 必须用引用传递，确保全局推进
 * 2. 值域边界：左子树 [lo, node->val]，右子树 [node->val, hi]
 * 3. 空树处理：空字符串应返回 nullptr
 *
 * 【面试追问】
 * Q1: 如何让编码更紧凑？用 2 字节二进制代替字符串
 * Q2: BST vs 通用二叉树序列化的本质区别？
 *     BST 前序遍历可唯一确定结构，通用树必须额外信息
 * Q3: 反序列化的时间复杂度？O(n)，idx 单调递增，每节点仅访问一次
 */
