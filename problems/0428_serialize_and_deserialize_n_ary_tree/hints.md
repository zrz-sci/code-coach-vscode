# 428. 序列化和反序列化 N 叉树

## 核心思路

本质上是要设计一种**编码格式**，能够无歧义地将 N 叉树的结构信息（节点值 + 每个节点有几个孩子）编码为字符串，并能从字符串完美还原。关键难点在于：二叉树只有左右两个孩子，位置固定；而 N 叉树每个节点的孩子数量不固定，必须额外编码"孩子数量"信息。

## 思维链

1. **读完题第一反应**：和 LeetCode 297（序列化二叉树）很像，但 N 叉树的孩子数不固定，不能简单用 null 占位来标记左右——因为没有"左右"的概念，只有一个 children 列表。

2. **二叉树的方法为什么不能直接搬？** 二叉树序列化时，每个节点固定有 2 个"槽位"（左、右），用 null 填空即可。N 叉树每个节点的孩子数不同，我们必须知道"这个节点有几个孩子"才能正确划分子树。

3. **核心问题：如何编码"孩子数量"？** 两种主流思路：
   - **DFS + 记录孩子数**：前序遍历时，对每个节点记录 `[val, childrenCount]`，反序列化时读到 childrenCount 就知道接下来递归几次。
   - **DFS + 哨兵标记**：每个节点的孩子遍历完后放一个特殊标记（如 `]` 或 `#`），反序列化时遇到哨兵就知道当前节点的孩子已结束。

4. **BFS 也能做**：层序遍历，每个节点的孩子用 null 分隔（题目给出的 LeetCode 格式就是这种），但实现起来比 DFS 稍复杂。

5. **选哪个？** DFS + 孩子数量最简洁、最不容易出错，面试中推荐。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS + 孩子数 | 前序遍历，每个节点记 `val childrenCount` | O(n) | O(n) | ⭐ 必须写出 |
| DFS + 哨兵标记 | 前序遍历，孩子结束放 `#` | O(n) | O(n) | 能说出即可 |
| BFS + null 分隔 | 层序遍历，每组孩子用 null 分隔 | O(n) | O(n) | 加分项 |

## 关键提示

1. **和二叉树序列化的本质区别**：二叉树每个节点固定 2 个孩子，用 null 占空位就行；N 叉树孩子数不定，必须编码"这个节点有多少个孩子"或"孩子到哪里结束"。

2. **DFS + 孩子数方案的格式示例**：
   ```
   //        1
   //      / | \
   //     3  2  4
   //    / \
   //   5   6
   //
   // 序列化: "1 3 3 2 5 0 6 0 2 0 4 0"
   //          ↑ ↑   ↑   ↑   ↑
   //         val=1, children=3 → 接下来递归 3 个子树
   //              val=3, children=2 → 递归 2 个子树
   //                   val=5, children=0
   //                        val=6, children=0
   //              val=2, children=0
   //              val=4, children=0
   ```

3. **反序列化的关键**：用一个索引（或 stringstream）顺序读取 token，先读 val 创建节点，再读 childrenCount，然后循环 childrenCount 次递归——这就是前序遍历的逆过程。

4. **哨兵方案的格式**：`"1 [ 3 [ 5 [] 6 [] ] 2 [] 4 [] ]"` 或简化为 `"1 3 5 # 6 # # 2 # 4 # #"`，每个 `#` 表示当前节点孩子列表结束。

5. **边界条件**：空树 → 返回空字符串 `""`；只有根节点 → `"1 0"` 或 `"1 #"`。

## 解法详解

### 解法1: DFS + 孩子数 — O(n) / O(n) ⭐ 面试首选

**思考过程**：前序遍历 N 叉树，对每个节点输出两个信息：`val` 和 `children.size()`。反序列化时，读到 val 创建节点，读到 size 就知道要递归几次创建子节点。这是最干净的方案——不需要特殊标记，不需要 null 占位。

```cpp
class Codec {
public:
    // 序列化：前序遍历，每个节点记录 "val childrenCount"
    string serialize(Node* root) {
        if (!root) return "";
        string res;
        serializeDFS(root, res);
        return res;
    }
    
    // 反序列化：按相同顺序读取，递归重建
    Node* deserialize(string data) {
        if (data.empty()) return nullptr;
        istringstream iss(data);
        return deserializeDFS(iss);
    }

private:
    void serializeDFS(Node* node, string& res) {
        if (!res.empty()) res += " ";
        res += to_string(node->val) + " " + to_string(node->children.size());
        for (Node* child : node->children) {
            serializeDFS(child, res);
        }
    }
    
    Node* deserializeDFS(istringstream& iss) {
        int val, size;
        iss >> val >> size;
        Node* node = new Node(val);
        for (int i = 0; i < size; i++) {
            node->children.push_back(deserializeDFS(iss));
        }
        return node;
    }
};
```

**关键点**：
- `istringstream` 天然按空格分割 token，非常适合这种场景
- 递归天然地把前序序列正确切分给各子树——读完一个子树的所有 token 后，流的位置刚好在下一个子树的起始处

### 解法2: DFS + 哨兵标记 — O(n) / O(n)

**从解法1变化**：不记录孩子数，改用一个特殊标记 `#` 表示"当前节点的孩子列表结束"。反序列化时，循环创建子节点直到遇到 `#`。

```cpp
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
        res += to_string(node->val);
        for (Node* child : node->children) {
            serializeDFS(child, res);
        }
        res += " #";  // 孩子列表结束的哨兵
    }
    
    Node* deserializeDFS(istringstream& iss) {
        string token;
        iss >> token;
        // 遇到哨兵返回 nullptr 表示当前层结束
        if (token == "#") return nullptr;
        
        Node* node = new Node(stoi(token));
        // 持续递归直到遇到哨兵
        while (true) {
            Node* child = deserializeDFS(iss);
            if (!child) break;  // 遇到 # 了
            node->children.push_back(child);
        }
        return node;
    }
};
```

**关键点**：哨兵方案不需要预先知道孩子数，但每个节点多输出一个 `#` token，序列化字符串更长。

### 解法3: BFS + null 分隔 — O(n) / O(n)

**不同角度**：用层序遍历，每个节点的孩子入队后放一个 null 分隔符（类似题目描述中的 LeetCode 格式）。

```cpp
class Codec3 {
public:
    string serialize(Node* root) {
        if (!root) return "";
        string res = to_string(root->val);
        queue<Node*> q;
        q.push(root);
        // 根节点后紧跟一个 null 分隔
        // 格式: val null children... null children... null ...
        while (!q.empty()) {
            Node* node = q.front(); q.pop();
            for (Node* child : node->children) {
                res += " " + to_string(child->val);
                q.push(child);
            }
            res += " #";  // 当前节点的孩子结束
        }
        return res;
    }
    
    Node* deserialize(string data) {
        if (data.empty()) return nullptr;
        istringstream iss(data);
        string token;
        iss >> token;
        Node* root = new Node(stoi(token));
        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            Node* parent = q.front(); q.pop();
            while (iss >> token && token != "#") {
                Node* child = new Node(stoi(token));
                parent->children.push_back(child);
                q.push(child);
            }
        }
        return root;
    }
};
```

**关键点**：BFS 方案的序列化顺序和 LeetCode 官方格式对齐，阅读直观；但代码比 DFS 方案略长。

## 解法对比

| | DFS+孩子数 | DFS+哨兵 | BFS+null分隔 |
|--|-----------|---------|-------------|
| 编码信息 | 显式记录 childrenCount | 用 `#` 标记结束 | 用 `#` 分组 |
| 序列化长度 | 每个节点 2 个 token | 每个节点 1 个 val + 1 个 `#` | 类似 |
| 代码简洁度 | ⭐ 最简洁 | 中等 | 最长 |
| 面试推荐 | ⭐ 首选 | 备选 | 如果面试官要求 BFS |

## 易错点

1. **空树没有特判**：
   - ✗ 直接对 `root` 调用 `root->val` → 空指针崩溃
   - ✓ `if (!root) return "";` 和 `if (data.empty()) return nullptr;`

2. **哨兵方案中递归返回值处理不当**：
   - ✗ `deserializeDFS` 遇到 `#` 时直接 return 但没告诉调用者"结束了" → 孩子列表无法正确终止
   - ✓ 遇到 `#` 返回 `nullptr`，调用者检查返回值决定是否 break

3. **数值分隔问题**：
   - ✗ 用逗号分隔但 `istringstream` 默认按空格分割 → 解析失败
   - ✓ 统一用空格分隔，或者自行实现分割逻辑

4. **多位数的处理**：
   - ✗ 一次读一个字符 → `val=10` 被拆成 `1` 和 `0`
   - ✓ 用 `istringstream >> val` 或 `stoi()` 正确读取完整数字

## 面试追问

**Q1: 为什么不能直接套二叉树序列化的 null 占位方法？**
A: 二叉树每个节点固定有左右两个"槽位"，null 填充空位即可确定结构。N 叉树孩子数不固定，如果只用 null 标记空位，无法区分"第2个孩子为空"和"只有1个孩子"。

**Q2: 三种方案的序列化字符串长度有区别吗？哪个更紧凑？**
A: DFS+孩子数每个节点输出 2 个 token（val + count）；DFS+哨兵每个节点也是约 2 个 token（val + `#`）；BFS 类似。实际长度差不多，但如果孩子数很大（如 1000），`childrenCount` 是一个数字 vs `#` 是 1 个字符，DFS 孩子数方案可能略长。总体差别不大。

**Q3: 如果节点值可能包含特殊字符（如空格、`#`），怎么处理？**
A: 需要转义或用长度前缀编码。例如 `"3:abc"` 表示接下来 3 个字符是值 `abc`。或者用 JSON 格式 `{"val":"abc","children":[...]}` 但效率低。面试中一般假设值为整数，不需要处理这种情况，但提出来说明你考虑得全面。

**Q4: 如果要最小化序列化字符串的大小（网络传输场景），怎么优化？**
A: 可以用二进制编码代替文本。每个节点用 4 字节存 val + 2 字节存 childrenCount。或用变长编码（varint）。还可以压缩（gzip）。

## 相关题型

- **297. 序列化和反序列化二叉树** — 本题的简化版，每个节点固定 2 个孩子。学会 428 后做 297 可以直接把 childrenCount 固定为 2（或用 null 占位，更简单）。
- **449. 序列化和反序列化 BST** — 利用 BST 性质（中序有序），可以只存前序序列不需要 null 标记。复用"前序遍历 + 额外信息重建"的思路。
- **341. 扁平化嵌套列表迭代器** — 嵌套列表本质上就是 N 叉树结构，扁平化 = 前序遍历。
- **589/590. N 叉树的前序/后序遍历** — 本题序列化的基础操作，确保你能熟练写 N 叉树的 DFS 遍历。