/*
 * 【题目本质】
 * 设计双向映射：N 叉树 ↔ 二叉树，核心是 "左孩子右兄弟" 表示法
 *
 * 【解法总览】
 * 解法1: DFS 递归（左孩子右兄弟）⭐ — O(n) / O(h) — 面试首选
 * 解法2: BFS 迭代 — O(n) / O(n) — 用队列层序处理
 */

// ============================================================
// 解法1: DFS 递归（左孩子右兄弟）⭐
// 思路：left 存第一个 child，right 串联后续 siblings
// 时间 O(n)，空间 O(h)
// ============================================================
class Codec_v1 {
public:
    TreeNode* encode(Node* root) {
        if (!root) return nullptr;
        TreeNode* bNode = new TreeNode(root->val);
        if (!root->children.empty()) {
            bNode->left = encode(root->children[0]);
        }
        TreeNode* cur = bNode->left;
        for (int i = 1; i < (int)root->children.size(); i++) {
            cur->right = encode(root->children[i]);
            cur = cur->right;
        }
        return bNode;
    }

    Node* decode(TreeNode* root) {
        if (!root) return nullptr;
        Node* nNode = new Node(root->val);
        TreeNode* cur = root->left;
        while (cur) {
            nNode->children.push_back(decode(cur));
            cur = cur->right;
        }
        return nNode;
    }
};

// ============================================================
// 解法2: BFS 迭代
// 思路：层序遍历，用队列同步处理 N 叉树和二叉树节点
// 时间 O(n)，空间 O(n)
// ============================================================
class Codec_v2 {
public:
    TreeNode* encode(Node* root) {
        if (!root) return nullptr;
        TreeNode* bRoot = new TreeNode(root->val);
        queue<pair<Node*, TreeNode*>> q;
        q.push({root, bRoot});
        while (!q.empty()) {
            auto [nNode, bNode] = q.front(); q.pop();
            TreeNode* prev = nullptr;
            for (int i = 0; i < (int)nNode->children.size(); i++) {
                TreeNode* child = new TreeNode(nNode->children[i]->val);
                if (i == 0) {
                    bNode->left = child;
                } else {
                    prev->right = child;
                }
                prev = child;
                q.push({nNode->children[i], child});
            }
        }
        return bRoot;
    }

    Node* decode(TreeNode* root) {
        if (!root) return nullptr;
        Node* nRoot = new Node(root->val);
        queue<pair<TreeNode*, Node*>> q;
        q.push({root, nRoot});
        while (!q.empty()) {
            auto [bNode, nNode] = q.front(); q.pop();
            TreeNode* cur = bNode->left;
            while (cur) {
                Node* child = new Node(cur->val);
                nNode->children.push_back(child);
                q.push({cur, child});
                cur = cur->right;
            }
        }
        return nRoot;
    }
};

/*
 * 【解法对比】
 * DFS 递归：代码简洁，栈深度 O(h)，面试中最推荐
 * BFS 迭代：避免栈溢出风险（树高 ≤ 1000 时不成问题），代码稍长
 *
 * 【易错点】
 * 1. encode 时 children 为空要跳过 left 设置，否则空指针
 * 2. decode 时 right 链遍历漏掉 → 必须 while(cur) 循环
 * 3. N-ary Node 定义中可能是 children 或 neighbors，注意区分
 *
 * 【面试追问】
 * Q: 编码后二叉树最坏高度？ A: O(n)，原树扁宽时 right 链很长
 * Q: 是否支持值重复？ A: 支持，结构恢复不依赖值唯一性
 * Q: 能否只用一个函数同时 encode/decode？ A: 不行，输入输出类型不同
 */
