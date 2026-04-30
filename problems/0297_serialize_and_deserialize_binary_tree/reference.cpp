/*
 * LeetCode 297: 二叉树的序列化与反序列化 (Serialize and Deserialize Binary Tree)
 *
 * 【题目本质】
 * 设计编码/解码算法，将二叉树无损转换为字符串并还原。
 * 关键：必须记录 null 节点信息，否则单一遍历序列无法唯一确定树结构。
 *
 * 【解法总览】
 * 解法1: DFS 前序遍历 — O(n) / O(n) — 递归，代码最简洁 ⭐ 面试首选
 * 解法2: BFS 层序遍历 — O(n) / O(n) — 队列，和 LeetCode 格式一致
 *
 * 【示例树】
 *       1
 *      / \
 *     2   3
 *        / \
 *       4   5
 *
 * DFS前序序列化: "1,2,#,#,3,4,#,#,5,#,#"
 * BFS层序序列化: "1,2,3,#,#,4,5,#,#,#,#"
 */


// ============================================================
// 解法1: DFS 前序遍历 — 递归序列化/反序列化
// 时间: O(n)  空间: O(n) — 字符串 + 递归栈
//
// 【思路】
// 序列化：前序遍历（根→左→右），每个节点写值，null 写 "#"。
// 反序列化：顺序读取 token，当前 token 就是当前节点。
//   递归构建左子树（会自动消费掉左子树的所有 token），然后构建右子树。
//   和序列化时的遍历顺序完全一致，所以用一个队列顺序 pop 就行。
//
// 为什么前序 + null 标记能唯一确定树？
// 因为第一个 token 一定是根，然后左子树是一段完整的递归序列
// （以 # 结尾），右子树紧随其后。null 标记作为"终止符"，
// 精确标识了每棵子树的边界。
//
// 【序列化过程可视化】
//       1                  输出序列（从左到右生成）:
//      / \
//     2   3                visit(1) → "1,"
//        / \               visit(2) → "1,2,"
//       4   5              2.left=null → "1,2,#,"
//                          2.right=null → "1,2,#,#,"
//                          visit(3) → "1,2,#,#,3,"
//                          visit(4) → "1,2,#,#,3,4,"
//                          4.left=null → "1,2,#,#,3,4,#,"
//                          4.right=null → "1,2,#,#,3,4,#,#,"
//                          visit(5) → "1,2,#,#,3,4,#,#,5,"
//                          5.left=null → "1,2,#,#,3,4,#,#,5,#,"
//                          5.right=null → "1,2,#,#,3,4,#,#,5,#,#,"
//
// 【反序列化过程可视化】
// tokens: [1, 2, #, #, 3, 4, #, #, 5, #, #]
//          ^
// Step1: pop "1" → 创建节点1, 递归建左子树
//             ^
// Step2: pop "2" → 创建节点2, 递归建左子树
//                ^
// Step3: pop "#" → 返回null (节点2的左孩子=null)
//                   ^
// Step4: pop "#" → 返回null (节点2的右孩子=null)
//         → 节点2构建完毕，返回给节点1作为左孩子
//                      ^
// Step5: pop "3" → 创建节点3, 递归建左子树
//                         ^
// Step6: pop "4" → 创建节点4, 递归建左/右子树
//                            ^  ^
// Step7-8: pop "#","#" → 节点4左右=null, 返回给节点3.left
//                                  ^
// Step9: pop "5" → 创建节点5, 递归建左/右子树
//                                     ^  ^
// Step10-11: pop "#","#" → 节点5左右=null, 返回给节点3.right
//         → 节点3构建完毕，返回给节点1作为右孩子
//         → 树构建完毕！
// ============================================================

class Codec1 {
public:
    // ---- 序列化 ----
    string serialize(TreeNode* root) {
        string result;
        serializeDFS(root, result);
        return result;
    }

    void serializeDFS(TreeNode* node, string& result) {
        if (!node) {
            // 关键：null 也要记录，否则无法区分不同结构
            // 例如 [1,2,null] vs [1,null,2] 前序都是 "1,2" 如果不记 null
            result += "#,";
            return;
        }
        // 前序：先写根的值
        result += to_string(node->val) + ",";
        serializeDFS(node->left, result);
        serializeDFS(node->right, result);
    }

    // ---- 反序列化 ----
    TreeNode* deserialize(string data) {
        // 用 queue 顺序消费 token，比数组+索引更不容易出错
        queue<string> tokens;
        string token;
        for (char c : data) {
            if (c == ',') {
                tokens.push(token);
                token.clear();
            } else {
                token += c;
            }
        }
        // 处理末尾没有逗号的情况
        if (!token.empty()) tokens.push(token);
        return deserializeDFS(tokens);
    }

    TreeNode* deserializeDFS(queue<string>& tokens) {
        if (tokens.empty()) return nullptr;

        string val = tokens.front();
        tokens.pop();  // 消费当前 token

        if (val == "#") return nullptr;  // null 节点

        TreeNode* node = new TreeNode(stoi(val));
        // 先构建左子树 → 递归会消费完左子树所有 token
        // 然后构建右子树 → 消费右子树所有 token
        // 顺序必须和序列化时一致（先左后右）！
        node->left = deserializeDFS(tokens);
        node->right = deserializeDFS(tokens);
        return node;
    }
};


// ============================================================
// 解法2: BFS 层序遍历 — 队列序列化/反序列化
// 时间: O(n)  空间: O(n) — 字符串 + 队列
//
// 【思路】
// 序列化：标准 BFS，逐层输出。null 节点也入队并输出 "#"。
// 反序列化：从第一个 token 创建根节点入队。
//   队列每弹出一个 parent，连续消费 2 个 token 作为左右孩子。
//   非 null 的孩子入队，将来给它们连孩子。
//
// 和 DFS 的区别：
// - DFS 的序列化结果是前序序列，反序列化靠递归的调用顺序来定位
// - BFS 的序列化结果是层序序列，反序列化靠队列的弹出顺序来定位
//
// 【BFS 序列化过程】
//       1              Queue: [1]
//      / \             pop 1 → 写"1", push 2,3 → Queue: [2,3]
//     2   3            pop 2 → 写"2", push #,# → Queue: [3]  (null不入队)
//        / \           pop 3 → 写"3", push 4,5 → Queue: [4,5]
//       4   5          pop 4 → 写"4", push #,# → Queue: [5]
//                      pop 5 → 写"5", push #,# → Queue: []
//                      结果: "1,2,3,#,#,4,5,#,#,#,#"
//
// 【BFS 反序列化过程】
// tokens: [1, 2, 3, #, #, 4, 5, #, #, #, #]
//
// Step1: 创建 root=1, Queue: [1]
// Step2: pop 1, 读 token "2" → 1.left=2, 读 token "3" → 1.right=3
//        Queue: [2, 3]
// Step3: pop 2, 读 "#" → 2.left=null, 读 "#" → 2.right=null
//        Queue: [3]
// Step4: pop 3, 读 "4" → 3.left=4, 读 "5" → 3.right=5
//        Queue: [4, 5]
// Step5: pop 4, 读 "#" → 4.left=null, 读 "#" → 4.right=null
//        Queue: [5]
// Step6: pop 5, 读 "#" → 5.left=null, 读 "#" → 5.right=null
//        Queue: []  → 完成!
// ============================================================

class Codec2 {
public:
    // ---- 序列化 ----
    string serialize(TreeNode* root) {
        if (!root) return "#";

        string result;
        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();

            if (!node) {
                result += "#,";
            } else {
                result += to_string(node->val) + ",";
                // 关键：null 孩子也入队！
                // 这样反序列化时 token 和 parent 的对应关系才正确
                // 但注意：只有非 null 的节点才会在反序列化时从队列弹出去连孩子
                q.push(node->left);   // 可能是 null
                q.push(node->right);  // 可能是 null
            }
        }
        return result;
    }

    // ---- 反序列化 ----
    TreeNode* deserialize(string data) {
        if (data.empty() || data == "#") return nullptr;

        // 拆分 token
        vector<string> tokens;
        string token;
        for (char c : data) {
            if (c == ',') {
                if (!token.empty()) {  // 跳过空 token，防止末尾逗号问题
                    tokens.push_back(token);
                    token.clear();
                }
            } else {
                token += c;
            }
        }
        if (!token.empty()) tokens.push_back(token);

        if (tokens.empty() || tokens[0] == "#") return nullptr;

        TreeNode* root = new TreeNode(stoi(tokens[0]));
        queue<TreeNode*> q;
        q.push(root);

        int i = 1;
        while (!q.empty() && i < (int)tokens.size()) {
            TreeNode* parent = q.front();
            q.pop();

            // 消费 2 个 token：左孩子和右孩子
            // 左孩子
            if (tokens[i] != "#") {
                parent->left = new TreeNode(stoi(tokens[i]));
                q.push(parent->left);  // 只有非 null 才入队
            }
            // else: parent->left 默认就是 nullptr，不用处理
            i++;

            // 右孩子
            if (i < (int)tokens.size() && tokens[i] != "#") {
                parent->right = new TreeNode(stoi(tokens[i]));
                q.push(parent->right);
            }
            i++;
        }

        return root;
    }
};


// ============================================================
// 【解法对比】
//
// | 维度         | DFS 前序 (解法1)              | BFS 层序 (解法2)              |
// |-------------|------------------------------|------------------------------|
// | 核心数据结构 | 递归调用栈（隐式栈）           | 显式队列                      |
// | 序列化结果   | 前序序列 + null 标记           | 层序序列 + null 标记           |
// | 反序列化方式 | 递归消费 token                 | 队列逐层 + 索引消费 token      |
// | 代码量       | 更短，递归天然简洁              | 稍长，需要管理索引              |
// | 递归深度风险 | 极度不平衡树可能爆栈            | 无此问题                      |
// | 面试推荐     | ⭐ 首选（短小不易错）           | 追问时展示（或特殊要求时用）    |
//
// 两种方法时间空间复杂度相同，都是 O(n)。
// 面试中先写 DFS 版本（代码量少、逻辑清晰），
// 面试官追问时再说 BFS 方案展示知识面。
//
// ============================================================
// 【易错点】
//
// 1. 忘记序列化 null 节点：
//    ✗ 只写非空节点 → 前序 "1,2,3" 无法区分多种树结构
//    ✓ null 写 "#" → "1,2,#,#,3,4,#,#,5,#,#" 唯一确定
//
// 2. 反序列化左右顺序搞反：
//    ✗ node->right = dfs(tokens); node->left = dfs(tokens);
//    ✓ 先 left 再 right，和序列化的写入顺序一致
//
// 3. 负数值的解析：
//    ✗ 按字符 split 把 '-' 当成分隔符 → "-1000" 被拆坏
//    ✓ 按逗号 split，stoi("-1000") 正确处理
//
// 4. BFS 反序列化时把 null 节点也入队：
//    ✗ parent->left = nullptr; q.push(nullptr); → 后续 pop 出 null 去连孩子，崩溃
//    ✓ 只有 tokens[i] != "#" 时才创建节点并入队
//
// 5. 末尾逗号导致空 token：
//    ✗ "1,2,#," → split 后有空字符串 → stoi("") 抛异常
//    ✓ split 时 if (!token.empty()) 跳过空 token
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 为什么单一遍历+null标记就够了？不需要两种遍历序列吗？
// A1: 因为 null 标记精确标识了每棵子树的边界。前序的第一个
//     元素是根，然后左子树是一段以 # 结尾的完整递归序列，
//     右子树紧随其后。不像 105 题（前序+中序重建）需要中序
//     来划分左右子树，这里 null 标记替代了中序的作用。
//
// Q2: 如果树可能非常深（10^5 层的链状树），DFS 递归会怎样？
// A2: 会栈溢出。改用 BFS 层序方案（天然不受深度影响），
//     或用栈模拟 DFS 的迭代版本。
//
// Q3: 能否进一步压缩序列化结果？
// A3: 可以。用二进制编码代替字符串（每个 int 占 4 字节），
//     用位图记录 null 位置（每个节点只需 1 bit），
//     或者利用 BST 性质省略 null 标记（见 449 题）。
//
// Q4: 这个设计是否线程安全？如果多个线程同时序列化不同的树呢？
// A4: 当前实现是线程安全的——序列化只读树结构，反序列化创建
//     新树。只要不共享可变状态（如全局变量），就没问题。
//     但如果用了成员变量存中间状态（如 index），则不安全。
// ============================================================
