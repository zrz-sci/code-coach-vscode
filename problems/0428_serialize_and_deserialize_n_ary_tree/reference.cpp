/*
 * LeetCode 428: 序列化和反序列化 N 叉树
 *
 * 【题目本质】
 * 设计编码格式，将 N 叉树无歧义地转为字符串并还原。
 * 核心难点：孩子数量不固定，必须编码"每个节点有几个孩子"或"孩子何时结束"。
 *
 * 【解法总览】
 * 解法1: DFS + 孩子数 — O(n) / O(n) — 面试首选，最简洁
 * 解法2: DFS + 哨兵标记 — O(n) / O(n) — 另一种经典思路
 * 解法3: BFS + null 分隔 — O(n) / O(n) — 与 LeetCode 官方格式一致
 */

/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> children;

    Node() {}
    Node(int _val) : val(_val) {}
    Node(int _val, vector<Node*> _children) : val(_val), children(_children) {}
};
*/

// ============================================================
// 解法1: DFS + 孩子数 — 面试首选
// 时间: O(n)  空间: O(n)（递归栈 + 字符串）
//
// 【思路】
// 前序遍历 N 叉树，对每个节点记录两个信息：val 和 children.size()。
// 反序列化时，读到 val 创建节点，读到 size 就循环递归 size 次。
//
// 为什么 childrenCount 是必要的？
// 二叉树固定 2 个孩子，可以用 null 占位。
// N 叉树孩子数不确定，不记录的话反序列化时不知道递归几次。
//
// 示例树:
//        1
//      / | \
//     3  2  4
//    / \
//   5   6
//
// 序列化过程 (前序: 节点值 孩子数):
//   visit 1 → "1 3"  (值=1, 3个孩子)
//   visit 3 → "3 2"  (值=3, 2个孩子)
//   visit 5 → "5 0"  (值=5, 0个孩子, 叶子)
//   visit 6 → "6 0"  (值=6, 0个孩子, 叶子)
//   visit 2 → "2 0"  (值=2, 0个孩子)
//   visit 4 → "4 0"  (值=4, 0个孩子)
//
// 最终字符串: "1 3 3 2 5 0 6 0 2 0 4 0"
//
// 反序列化过程 (从 istringstream 顺序读取):
//   read "1 3" → 创建节点1, 循环3次递归创建子节点
//     read "3 2" → 创建节点3, 循环2次
//       read "5 0" → 创建节点5, 循环0次 → 返回
//       read "6 0" → 创建节点6, 循环0次 → 返回
//     → 节点3的children = [5, 6]
//     read "2 0" → 创建节点2, 循环0次 → 返回
//     read "4 0" → 创建节点4, 循环0次 → 返回
//   → 节点1的children = [3, 2, 4]
// ============================================================
class Codec {
public:
    string serialize(Node* root) {
        if (!root) return "";   // 空树 → 空字符串
        string res;
        serializeDFS(root, res);
        return res;
    }
    
    Node* deserialize(string data) {
        if (data.empty()) return nullptr;  // 空字符串 → 空树
        istringstream iss(data);
        return deserializeDFS(iss);
    }

private:
    void serializeDFS(Node* node, string& res) {
        // 每个节点输出: "val childrenCount"
        if (!res.empty()) res += " ";
        res += to_string(node->val) + " " + to_string(node->children.size());
        
        // 前序遍历: 先处理自己，再递归所有孩子
        for (Node* child : node->children) {
            serializeDFS(child, res);
        }
    }
    
    Node* deserializeDFS(istringstream& iss) {
        int val, size;
        iss >> val >> size;  // 读取 val 和 childrenCount
        
        Node* node = new Node(val);
        // 精确地递归 size 次 —— 这就是记录孩子数的好处
        for (int i = 0; i < size; i++) {
            node->children.push_back(deserializeDFS(iss));
        }
        return node;
    }
};

// ============================================================
// 解法2: DFS + 哨兵标记 — 用 "#" 表示孩子列表结束
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 和解法1的区别：不显式记录孩子数，而是在每个节点的孩子遍历
// 完毕后放一个哨兵 "#"。反序列化时持续递归创建子节点直到遇到 "#"。
//
// 示例树 (同上):
//        1
//      / | \
//     3  2  4
//    / \
//   5   6
//
// 序列化: "1 3 5 # 6 # # 2 # 4 # #"
//          ↑       ↑ ↑   ↑ ↑ ↑ ↑ ↑ ↑
//          1的孩子: 3的孩子: 5(无孩子→#) 6(无孩子→#)  3孩子结束→#
//                  2(无孩子→#)  4(无孩子→#)  1孩子结束→#
//
// 优点: 不需要预知孩子数
// 缺点: 序列化字符串略长（每个节点多一个 #）
// ============================================================
class Codec2 {
public:
    string serialize(Node* root) {
        if (!root) return "";
        string res;
        serializeDFS(root, res);
        return res;
    }
    
    Node* deserialize(string data) {
        if (data.empty()) return nullptr;
        istringstream iss(data);
        return deserializeDFS(iss);
    }

private:
    void serializeDFS(Node* node, string& res) {
        if (!res.empty()) res += " ";
        res += to_string(node->val);       // 输出节点值
        for (Node* child : node->children) {
            serializeDFS(child, res);       // 递归所有孩子
        }
        res += " #";                        // 孩子列表结束的哨兵
    }
    
    Node* deserializeDFS(istringstream& iss) {
        string token;
        iss >> token;
        
        // 哨兵标记: 当前节点的孩子已结束
        if (token == "#") return nullptr;
        
        Node* node = new Node(stoi(token));
        
        // 不知道有几个孩子, 循环直到遇到 "#"
        while (true) {
            Node* child = deserializeDFS(iss);
            if (!child) break;  // 遇到了 "#", 返回了 nullptr
            node->children.push_back(child);
        }
        return node;
    }
};

// ============================================================
// 解法3: BFS + null 分隔 — 层序遍历, 与 LeetCode 格式一致
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 层序遍历, 每个节点入队后, 输出其所有孩子的值, 最后加一个 "#" 分隔。
// 反序列化: 用队列逐层处理, 对每个 parent, 读取孩子值直到遇到 "#"。
//
// 示例树:
//        1
//      / | \
//     3  2  4
//    / \
//   5   6
//
// BFS 序列化过程:
//   Level 0: 1    → 输出 "1"
//   处理节点1的孩子: 3, 2, 4  → 输出 "3 2 4 #"
//   处理节点3的孩子: 5, 6     → 输出 "5 6 #"
//   处理节点2的孩子: (无)     → 输出 "#"
//   处理节点4的孩子: (无)     → 输出 "#"
//   处理节点5的孩子: (无)     → 输出 "#"
//   处理节点6的孩子: (无)     → 输出 "#"
//
// 最终: "1 3 2 4 # 5 6 # # # # #"
//
// 反序列化:
//   读 "1" → root=1, 入队
//   队首=1, 读直到 "#": 3,2,4 → 1.children=[3,2,4], 入队3,2,4
//   队首=3, 读直到 "#": 5,6   → 3.children=[5,6], 入队5,6
//   队首=2, 读直到 "#": (空)  → 2.children=[]
//   队首=4, 读直到 "#": (空)  → 4.children=[]
//   队首=5, 读直到 "#": (空)  → 5.children=[]
//   队首=6, 读直到 "#": (空)  → 6.children=[]
// ============================================================
class Codec3 {
public:
    string serialize(Node* root) {
        if (!root) return "";
        
        string res = to_string(root->val);
        queue<Node*> q;
        q.push(root);
        
        while (!q.empty()) {
            Node* node = q.front();
            q.pop();
            // 输出当前节点的所有孩子
            for (Node* child : node->children) {
                res += " " + to_string(child->val);
                q.push(child);
            }
            res += " #";  // 孩子列表结束标记
        }
        return res;
    }
    
    Node* deserialize(string data) {
        if (data.empty()) return nullptr;
        
        istringstream iss(data);
        string token;
        
        // 读取根节点
        iss >> token;
        Node* root = new Node(stoi(token));
        
        queue<Node*> q;
        q.push(root);
        
        while (!q.empty()) {
            Node* parent = q.front();
            q.pop();
            
            // 读取 parent 的所有孩子, 直到遇到 "#"
            while (iss >> token && token != "#") {
                Node* child = new Node(stoi(token));
                parent->children.push_back(child);
                q.push(child);
            }
        }
        return root;
    }
};

// ============================================================
// 【解法对比】
//
// | 方面       | 解法1(DFS+孩子数) | 解法2(DFS+哨兵) | 解法3(BFS+null分隔) |
// |-----------|------------------|----------------|-------------------|
// | 编码方式    | val + count      | val + "#"结束    | val + "#"分组       |
// | 代码行数    | 最少 ⭐           | 中等             | 最多               |
// | 递归/迭代   | 递归              | 递归             | 迭代(队列)          |
// | 面试推荐    | ⭐ 首选           | 备选             | 要求BFS时用         |
//
// 面试中推荐解法1:
// - 代码最短，逻辑最清晰
// - "读 val, 读 count, 循环 count 次递归" — 3 行核心逻辑
// - 不需要处理哨兵的特殊返回值
//
// 【易错点】
//
// 1. 空树特判遗漏:
//    ✗ serialize 直接访问 root->val → 空指针崩溃
//    ✓ 开头 if (!root) return "";
//
// 2. 哨兵方案中 deserialize 返回 nullptr 但调用者没检查:
//    ✗ node->children.push_back(deserializeDFS(iss));  // 可能 push nullptr
//    ✓ Node* child = deserializeDFS(iss); if (!child) break;
//
// 3. 多位数解析错误:
//    ✗ 一次读一个字符 → val=10 被拆成 '1' 和 '0'
//    ✓ 用 istringstream >> int 或 stoi() 读完整数字
//
// 4. 字符串拼接时忘记分隔符:
//    ✗ res += to_string(val) + to_string(size); → "13" 分不清 val=1,size=3 还是 val=13
//    ✓ res += to_string(val) + " " + to_string(size);
//
// 5. BFS 方案中队列顺序和读取顺序不对应:
//    ✗ 序列化时先输出所有节点再输出 "#" → 反序列化时无法正确配对
//    ✓ 每处理完一个节点的孩子就立刻加 "#"
//
// 【面试追问 — 递进链】
//
// Q1: 这道题和二叉树序列化(297)的核心区别是什么?
// A: 二叉树固定2个孩子, 用null占位即可确定结构。
//    N叉树孩子数不固定, 必须额外编码孩子数量或结束标记。
//
// Q2: 你的方案能处理节点值为负数的情况吗?
// A: 可以。istringstream >> int 能正确解析负号。
//    但如果用单字符分割, 需要确保 "-" 不会被误当成分隔符。
//
// Q3: 如何最小化序列化后的字符串长度?
// A: 1) 用二进制编码(固定字节) 代替文本(可变长度的十进制字符串)
//    2) 用 varint 编码小数字只需1字节, 大数字多字节
//    3) 对整个字符串做通用压缩(如 gzip)
//    4) 如果树的形状有规律(如完全树), 可以只存值数组, 形状隐含在索引中
//
// Q4: 如果要序列化到文件, 反序列化时文件很大无法全部加载到内存, 怎么办?
// A: 流式处理。DFS方案天然支持: serialize 写入输出流, 
//    deserialize 从输入流边读边建树, 不需要把整个字符串存到内存。
//    这也是 DFS 方案优于 BFS 方案的一个点 —— BFS 需要维护队列,
//    而 DFS 只需要递归栈(深度有限)。
// ============================================================
