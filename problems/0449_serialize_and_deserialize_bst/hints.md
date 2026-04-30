# 449. 序列化和反序列化二叉搜索树 (Serialize and Deserialize BST)

## 核心思路
利用 BST 的性质：前序遍历 + 值域范围即可唯一确定树结构，无需存储 null 标记，编码更紧凑。

## 思维链
1. 通用二叉树序列化需要 null 标记 → BST 有序性可省略
2. 前序遍历得到序列 → 反序列化时利用值域 [min, max] 判断左右子树
3. 追求"尽可能紧凑" → 前序遍历无 null 标记 vs BFS 带 null 标记
4. 进阶：4 字节二进制编码代替字符串，更紧凑

## 解法概览表

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| Solution1 | 前序遍历 + 值域递归 ⭐ | O(n) | O(n) | 最优/最紧凑 |
| Solution2 | BFS 层序遍历 + null 标记 | O(n) | O(n) | 通用但不够紧凑 |

## 关键提示
1. **BST 前序遍历可唯一重建**：无需 null 占位符，靠值域范围分割左右
2. **反序列化用索引引用**：传递 index 引用避免拷贝子数组
3. **紧凑性要求**：题目强调 "as compact as possible"，前序无 null 更优

## 解法详解

### Solution 1: 前序遍历 + 值域递归 ⭐
**思路**：
- **序列化**：前序遍历，值之间用逗号分隔
- **反序列化**：维护当前索引和值域 [min, max]，当前值在范围内就创建节点，递归构建左右子树

```cpp
class Codec {
public:
    string serialize(TreeNode* root) {
        string s;
        dfs_ser(root, s);
        return s;
    }
    
    TreeNode* deserialize(string data) {
        int idx = 0;
        vector<int> vals;
        stringstream ss(data);
        string token;
        while (getline(ss, token, ',')) {
            if (!token.empty()) vals.push_back(stoi(token));
        }
        return dfs_de(vals, idx, INT_MIN, INT_MAX);
    }

private:
    void dfs_ser(TreeNode* node, string& s) {
        if (!node) return;
        s += to_string(node->val) + ",";
        dfs_ser(node->left, s);
        dfs_ser(node->right, s);
    }
    
    TreeNode* dfs_de(vector<int>& vals, int& idx, int lo, int hi) {
        if (idx >= vals.size() || vals[idx] < lo || vals[idx] > hi) return nullptr;
        TreeNode* node = new TreeNode(vals[idx++]);
        node->left = dfs_de(vals, idx, lo, node->val);
        node->right = dfs_de(vals, idx, node->val, hi);
        return node;
    }
};
```

### Solution 2: BFS 层序 + null 标记
**思路**：标准 BFS 序列化，null 用 "#" 表示。通用方案但不够紧凑。

```cpp
class Codec {
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
```

## 易错点
- ✗ 反序列化时用 `substr` 切子数组 → ✓ 用索引引用 `int& idx`，O(n) 完成
- ✗ 值域边界用 `<` 和 `>` → ✓ BST 允许等值节点时需注意边界开闭
- ✗ 忽略空树情况 → ✓ data 为空时直接返回 nullptr

## 面试追问
**Q1: 与 297 题（序列化通用二叉树）有什么区别？**
A: BST 有序性使得前序遍历就能唯一确定结构，不需要 null 标记，编码更紧凑。通用二叉树必须用 null 标记或同时提供两种遍历。

**Q2: 如何让编码更紧凑？**
A: 由于 0 <= val <= 10^4，每个值可以用 2 字节（short）二进制存储代替字符串。每个节点从最多 5 字符减少到固定 2 字节。

**Q3: 时间复杂度分析？**
A: 序列化 O(n) 遍历所有节点。反序列化 O(n)：每个值最多被访问一次（idx 单调递增）。总体 O(n)。

## 相关题型
- [297. 二叉树的序列化与反序列化](https://leetcode.com/problems/serialize-and-deserialize-binary-tree/) - 通用版本
- [105. 从前序与中序遍历序列构造二叉树](https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/) - 重建树
- [1008. 前序遍历构造二叉搜索树](https://leetcode.com/problems/construct-binary-search-tree-from-preorder-traversal/) - 同样利用 BST 性质
