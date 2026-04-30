# 297. 二叉树的序列化与反序列化

## 核心思路

本质上是一个**编码/解码**问题：如何把一棵二叉树（包括其结构信息）无损地编码为一个字符串，再从字符串完美还原。关键在于——**必须记录 null 节点**，否则仅靠值序列无法唯一确定树的结构。

## 思维链

1. **读完题第一反应**：遍历树可以得到值的序列，但中序 `[2,1,3]` 或前序 `[1,2,3]` 单独都无法唯一确定一棵树（除非两个序列配合）。能不能用一种遍历就够？

2. **关键洞察**：如果我们把 **null 也记录下来**（用特殊标记如 `"#"`），那么前序遍历序列就能唯一确定一棵树。因为每个节点要么有值，要么是 `#`（空），递归地消费序列就能精确还原结构。

3. **DFS（前序）方案**：序列化时前序遍历，遇到 null 写 `"#"`；反序列化时顺序读取，递归构建左右子树。这是最简洁直觉的写法。

4. **BFS（层序）方案**：和 LeetCode 官方格式一致——逐层输出节点值，null 写 `"null"`。反序列化时用队列逐层连接左右孩子。

5. **两种方案的选择**：DFS 代码更短、递归思路更自然；BFS 更贴近 LeetCode 格式、层序输出更直观。面试中两种都能过，推荐 DFS 因为代码量少、不容易写错。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS 前序遍历 | 前序 + null 标记，递归序列化/反序列化 | O(n) | O(n) | ⭐ 必须写出 |
| BFS 层序遍历 | 层序 + null 标记，队列序列化/反序列化 | O(n) | O(n) | ⭐ 必须写出 |

## 关键提示

1. **为什么需要记录 null？** 考虑树 `[1,2,null]` 和 `[1,null,2]`，前序遍历都是 `"1,2"`，但结构不同。加上 null 标记后分别是 `"1,2,#,#,#"` 和 `"1,#,2,#,#"`，就能区分了。

2. **DFS 反序列化的核心**：不要用"索引+跳来跳去"的方式，而是用一个**流/迭代器顺序消费**。每次调用递归函数就消费一个 token，左子树递归会自动消费完左边所有 token，然后轮到右子树。

3. **分隔符和解析**：序列化时用逗号 `,` 分隔；反序列化时先 split 成 token 数组或用 `istringstream`。

4. **树的 ASCII 示意图帮助理解序列化结果**：
```
//       1              序列化 (前序 DFS):
//      / \             visit 1 → "1"
//     2   3            visit 2 → "1,2"
//        / \           2 无左孩子 → "1,2,#"
//       4   5          2 无右孩子 → "1,2,#,#"
//                      visit 3 → "1,2,#,#,3"
//                      visit 4 → "1,2,#,#,3,4"
//                      4 无左右 → "1,2,#,#,3,4,#,#"
//                      visit 5 → "1,2,#,#,3,4,#,#,5"
//                      5 无左右 → "1,2,#,#,3,4,#,#,5,#,#"
```

5. **BFS 层序的可视化**：
```
//       1              BFS 序列化 (逐层):
//      / \             Level 0: 1         → "1"
//     2   3            Level 1: 2, 3      → "1,2,3"
//        / \           Level 2: #,#,4,5   → "1,2,3,#,#,4,5"
//       4   5          Level 3: #,#,#,#   → (可省略末尾null)
```

## 解法详解

### 解法1: DFS 前序遍历 — O(n) / O(n) ⭐ 面试首选

**思考过程**：前序遍历（根→左→右）是最自然的递归方式。序列化时，遇到节点写值，遇到 null 写 `"#"`。反序列化时，顺序读取 token，当前 token 就是当前节点，然后递归构建左右子树。关键在于——递归的调用顺序和序列化的写入顺序完全一致，所以只需一个"全局指针"顺序消费即可。

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Codec {
public:
    // ---- 序列化：前序遍历，null 用 "#" 表示 ----
    string serialize(TreeNode* root) {
        string result;
        serializeDFS(root, result);
        return result;
    }
    
    void serializeDFS(TreeNode* node, string& result) {
        if (!node) {
            // null 节点也要记录，否则无法区分不同结构
            result += "#,";
            return;
        }
        // 前序：先写当前值
        result += to_string(node->val) + ",";
        // 再递归左、右
        serializeDFS(node->left, result);
        serializeDFS(node->right, result);
    }
    
    // ---- 反序列化：顺序消费 token，递归构建 ----
    TreeNode* deserialize(string data) {
        // 先把字符串按逗号拆成 token 列表
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
        return deserializeDFS(tokens);
    }
    
    TreeNode* deserializeDFS(queue<string>& tokens) {
        if (tokens.empty()) return nullptr;
        
        string val = tokens.front();
        tokens.pop();
        
        // "#" 代表 null 节点，返回空
        if (val == "#") return nullptr;
        
        // 创建当前节点，然后递归构建左右子树
        // 注意：必须先左后右，和序列化时的顺序一致
        TreeNode* node = new TreeNode(stoi(val));
        node->left = deserializeDFS(tokens);   // 递归会消费完左子树所有token
        node->right = deserializeDFS(tokens);  // 然后轮到右子树
        return node;
    }
};
```

**关键点**：
- 反序列化中用 `queue` 而不是数组+索引，更不容易出错——`pop()` 天然实现"消费一个往后移"
- 递归调用顺序（先左后右）必须和序列化顺序一致

---

### 解法2: BFS 层序遍历 — O(n) / O(n)

**思考过程**：层序遍历更符合 LeetCode 官方的树的表示方式。序列化时用队列逐层输出；反序列化时也用队列，逐层给已创建的节点连接左右孩子。

```cpp
class Codec {
public:
    // ---- 序列化：BFS 层序遍历 ----
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
                // null 孩子也入队，这样反序列化时能对应上位置
                q.push(node->left);
                q.push(node->right);
            }
        }
        return result;
    }
    
    // ---- 反序列化：BFS 逐层构建 ----
    TreeNode* deserialize(string data) {
        if (data == "#") return nullptr;
        
        // 拆分 token
        vector<string> tokens;
        string token;
        for (char c : data) {
            if (c == ',') {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
            } else {
                token += c;
            }
        }
        if (!token.empty()) tokens.push_back(token);
        
        if (tokens.empty() || tokens[0] == "#") return nullptr;
        
        // 第一个 token 是根节点
        TreeNode* root = new TreeNode(stoi(tokens[0]));
        queue<TreeNode*> q;
        q.push(root);
        
        int i = 1;  // 从第2个token开始，每次消费2个（左孩子、右孩子）
        while (!q.empty() && i < tokens.size()) {
            TreeNode* parent = q.front();
            q.pop();
            
            // 左孩子
            if (tokens[i] != "#") {
                parent->left = new TreeNode(stoi(tokens[i]));
                q.push(parent->left);  // 非null才入队，将来给它连孩子
            }
            i++;
            
            // 右孩子
            if (i < tokens.size() && tokens[i] != "#") {
                parent->right = new TreeNode(stoi(tokens[i]));
                q.push(parent->right);
            }
            i++;
        }
        
        return root;
    }
};
```

**关键点**：
- 序列化时 **null 孩子也必须入队**，这样才能保持位置对应关系
- 反序列化时每个 parent 从队列弹出后，连续消费 2 个 token（左、右）
- 只有**非 null** 的孩子节点才入队（将来需要给它们连孩子）

## 解法对比

| | DFS 前序 | BFS 层序 |
|---|---|---|
| 核心思想 | 递归前序遍历 + null 标记 | 队列逐层遍历 + null 标记 |
| 代码量 | 更短，递归天然简洁 | 稍长，需要管理队列和索引 |
| 序列化结果 | 前序序列 `1,2,#,#,3,4,#,#,5,#,#` | 层序序列 `1,2,3,#,#,4,5,#,#,#,#` |
| 可读性 | 序列化结果不太直观 | 和 LeetCode 格式一致，直观 |
| 面试推荐 | ⭐ 首选，代码短不容易写错 | 追问时作为第二种方案 |
| 空间使用 | 递归栈深度 O(h)，最差 O(n) | 队列最大宽度 O(n) |

## 易错点

1. **忘记序列化 null 节点**
   - ✗ 只写非空节点值：前序 `"1,2,3,4,5"` → 无法还原结构
   - ✓ null 也写 `"#"`：`"1,2,#,#,3,4,#,#,5,#,#"` → 结构唯一确定

2. **反序列化时左右顺序搞反**
   - ✗ `node->right = deserializeDFS(tokens); node->left = deserializeDFS(tokens);`
   - ✓ 必须先左后右，和序列化时的遍历顺序一致

3. **负数的解析问题**
   - ✗ 按单字符解析，遇到 `-1000` 时 `-` 被当成分隔符
   - ✓ 按逗号 split，`stoi("-1000")` 能正确处理负数

4. **BFS 反序列化时 null 节点入队**
   - ✗ `parent->left = nullptr; q.push(parent->left);` — null 入队会导致后续 pop 出 null 去连孩子
   - ✓ 只有 `tokens[i] != "#"` 时才创建节点并入队

5. **字符串末尾多余逗号导致空 token**
   - ✗ `"1,2,#,"` split 后最后一个 token 是空字符串，`stoi("")` 会崩溃
   - ✓ split 时跳过空 token，或者序列化时不在末尾加逗号

## 面试追问

**Q1: 为什么前序遍历 + null 标记就能唯一确定一棵树？中序行不行？**

前序的第一个元素一定是根，然后递归地，下一段 token 属于左子树，再下一段属于右子树。null 标记作为"终止符"，告诉我们何时一棵子树结束。所以结构是唯一确定的。

中序遍历**不行**——即使加了 null 标记，中序的根在"中间"某个位置，而这个位置取决于左子树的大小，形成了"鸡生蛋蛋生鸡"的问题，无法无歧义地解析。

**Q2: 如果节点值可能包含逗号或特殊字符，怎么处理？**

使用**长度前缀**编码：先写值的字符长度，再写值本身。例如 `3:abc,5:he,ll` 表示两个值 `"abc"` 和 `"he,ll"`。这样任何字符都不会和分隔符冲突。或者使用转义字符（如 `\,` 表示字面逗号）。

**Q3: 如果要序列化一棵非常深的树（深度可能达 10^5），DFS 递归会爆栈吗？如何解决？**

会！递归深度 = 树的高度，10^5 层可能导致栈溢出。解决方案：
1. **用 BFS 层序方案**，天然不受树深度影响
2. **用栈模拟 DFS 的迭代版本**，避免递归调用栈
3. 某些语言可以增大栈空间（如 `ulimit -s`），但不是根本解决方案

**Q4: 能否压缩序列化结果？**

可以。几种思路：
- **二进制编码**：每个节点值用 2 字节（short）而非字符串，null 用特殊值如 `0x7FFF`
- **位图标记**：先输出所有非 null 值，再用一个位图标记哪些位置是 null（省去重复的 `"null"` 字符串）
- **差值编码 + 变长编码**：如果值的范围小，用差值减少位数

## 相关题型

- **449. 序列化和反序列化 BST** — 复用本题框架，但利用 BST 的有序性可以**不记录 null**（用上下界判断子树范围），序列化结果更短
- **652. 寻找重复的子树** — 复用"序列化子树"的思路：把每棵子树序列化为字符串，用哈希表检测重复
- **105. 从前序与中序遍历序列构造二叉树** — 如果不记录 null，需要两种遍历才能唯一确定树；本题通过记录 null 避免了这个需求
- **428. 序列化和反序列化 N 叉树** — 把二叉树的思路推广到 N 叉树，需要额外记录"孩子数量"或用特殊标记表示子树结束