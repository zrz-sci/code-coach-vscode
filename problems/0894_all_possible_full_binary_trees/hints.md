# 894. 所有可能的满二叉树

## 核心思路

本质上是一个**递归枚举 + 记忆化**问题：满二叉树的每个非叶节点恰好有 2 个子节点，因此 n 个节点的满二叉树可以拆分为：根节点 + 左子树(i 个节点) + 右子树(n-1-i 个节点)，其中 i 和 n-1-i 都必须是奇数。枚举所有合法的左右子树组合，递归构造。

## 思维链

1. **读完题第一反应**：满二叉树的性质——每个节点要么是叶子（0 个孩子），要么有恰好 2 个孩子。所以总节点数必须是奇数（否则不可能构成满二叉树）。当 n 为偶数时直接返回空列表。

2. **递归结构**：n 个节点的满二叉树 = 根 + 左子树(i) + 右子树(n-1-i)。根占 1 个节点，剩余 n-1 个分给左右。左子树 i 个节点，右子树 n-1-i 个节点，i 和 n-1-i 都必须是正奇数。所以 i = 1, 3, 5, ..., n-2。

3. **base case**：n = 1 时只有一个叶子节点，返回 `[TreeNode(0)]`。n 为偶数或 n < 1 时返回空列表。

4. **组合**：对每个合法的 i，递归求出所有 i 节点的满二叉树和所有 n-1-i 节点的满二叉树，两两组合，每对组合创建一个新的根节点。

5. **记忆化**：不同的递归调用可能请求相同的 n，用 map 或 vector 缓存已经算过的结果，避免重复构造。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | 枚举左右子树大小，递归构造 | O(2^(n/2)) | O(2^(n/2)) | 能说出即可 |
| 记忆化递归 | 缓存已算过的 n 的结果 | O(2^(n/2)) | O(2^(n/2)) | ⭐ 面试首选 |
| 迭代 DP | 从小到大构建所有奇数 n 的结果 | O(2^(n/2)) | O(2^(n/2)) | 等价写法 |

> 满二叉树的数量是卡塔兰数 C(n/2)，增长约 4^(n/2)/(n/2)^(3/2)，所以任何解法的输出大小都是指数级的。

## 关键提示

1. **n 必须是奇数**：满二叉树的每个非叶节点有 2 个孩子，所以每增加一个内部节点就增加 2 个节点（该节点本身+2个孩子-0个旧叶子...实际上更准确地说，满二叉树总是有 (n+1)/2 个叶子和 (n-1)/2 个内部节点）。总之 n 必须是奇数。

2. **左子树大小的枚举范围**：i = 1, 3, 5, ..., n-2。步长为 2，因为左右子树节点数都必须是奇数。

3. **递归树的结构**：allPossibleFBT(n) 调用 allPossibleFBT(i) 和 allPossibleFBT(n-1-i)，其中 i+j = n-1。由于 i 从 1 跳到 n-2，递归树的分支数较少，但每层都可能产生多个子树。

4. **结果共享 vs 深拷贝**：多个不同的满二叉树可能共享相同的子树指针（因为它们来自同一个缓存列表）。LeetCode 不要求深拷贝，所以直接共享即可。但如果后续需要修改树结构，则需要深拷贝。

5. **卡塔兰数**：n 个节点的满二叉树数量等于第 (n-1)/2 个卡塔兰数。例如 n=1 → 1, n=3 → 1, n=5 → 2, n=7 → 5, n=9 → 14。这决定了输出的大小，也是算法复杂度的下界。

## 解法详解

### 解法1: 暴力递归 — O(2^(n/2)) / O(2^(n/2))

**思考过程**：最直觉的方法——对于 n 个节点的满二叉树，枚举左子树使用 i 个节点（i 为奇数，从 1 到 n-2），右子树使用 n-1-i 个节点。递归求解两边的所有可能子树，然后两两组合。

```cpp
class Solution {
public:
    vector<TreeNode*> allPossibleFBT(int n) {
        // n 必须是奇数
        if (n % 2 == 0) return {};
        // base case: 单节点
        if (n == 1) return {new TreeNode(0)};
        
        vector<TreeNode*> result;
        // 枚举左子树节点数 i (奇数)
        for (int i = 1; i < n; i += 2) {
            vector<TreeNode*> left = allPossibleFBT(i);
            vector<TreeNode*> right = allPossibleFBT(n - 1 - i);
            // 两两组合
            for (auto* l : left) {
                for (auto* r : right) {
                    TreeNode* root = new TreeNode(0);
                    root->left = l;
                    root->right = r;
                    result.push_back(root);
                }
            }
        }
        
        return result;
    }
};
```

**关键点**：枚举步长为 2（只取奇数），base case 为 n=1。缺点是相同的 n 会被重复计算。

### 解法2: 记忆化递归 — O(2^(n/2)) / O(2^(n/2)) ⭐ 面试首选

**从暴力优化**：用 unordered_map<int, vector<TreeNode*>> 缓存每个 n 的结果，相同的 n 只计算一次。

```cpp
class Solution {
public:
    unordered_map<int, vector<TreeNode*>> memo;
    
    vector<TreeNode*> allPossibleFBT(int n) {
        if (n % 2 == 0) return {};
        if (memo.count(n)) return memo[n];
        if (n == 1) return memo[1] = {new TreeNode(0)};
        
        vector<TreeNode*> result;
        for (int i = 1; i < n; i += 2) {
            auto& left = allPossibleFBT(i);
            auto& right = allPossibleFBT(n - 1 - i);
            for (auto* l : left) {
                for (auto* r : right) {
                    TreeNode* root = new TreeNode(0);
                    root->left = l;
                    root->right = r;
                    result.push_back(root);
                }
            }
        }
        
        return memo[n] = result;
    }
};
```

**关键点**：memo 缓存了每个奇数 n 的满二叉树列表。注意使用引用 `auto&` 避免拷贝大 vector。

### 解法3: 迭代 DP — O(2^(n/2)) / O(2^(n/2))

**将递归转为迭代**：自底向上，先算小的 n，再用小的结果构造大的。

```cpp
class Solution {
public:
    vector<TreeNode*> allPossibleFBT(int n) {
        if (n % 2 == 0) return {};
        
        // dp[i] = 所有 i 个节点的满二叉树
        vector<vector<TreeNode*>> dp(n + 1);
        dp[1] = {new TreeNode(0)};
        
        // 从 3 开始，步长 2（只有奇数有满二叉树）
        for (int total = 3; total <= n; total += 2) {
            for (int left = 1; left < total; left += 2) {
                int right = total - 1 - left;
                for (auto* l : dp[left]) {
                    for (auto* r : dp[right]) {
                        TreeNode* root = new TreeNode(0);
                        root->left = l;
                        root->right = r;
                        dp[total].push_back(root);
                    }
                }
            }
        }
        
        return dp[n];
    }
};
```

**关键点**：dp 数组下标为奇数的位置存储对应的满二叉树列表，偶数位置保持为空。

## 解法对比

| | 暴力递归 | 记忆化递归 | 迭代 DP |
|--|---------|----------|---------|
| 时间 | O(2^(n/2)) 含重复 | O(2^(n/2)) | O(2^(n/2)) |
| 空间 | O(2^(n/2)) | O(2^(n/2)) | O(2^(n/2)) |
| 重复计算 | 有 | 无 | 无 |
| 代码简洁 | 最简洁 | ⭐ 适中 | 稍长 |
| 面试建议 | 说思路 | 首选 | 追问时给 |

**选择建议**：
- 面试首选记忆化递归：自然地从暴力递归优化而来，容易讲清思路
- 追问迭代写法时给 DP 版本
- n <= 20，实际输出量不大，暴力递归也能通过，但加 memo 更稳妥

## 易错点

1. **忘记 n 为偶数时返回空**
   - x 没有判断奇偶，对偶数 n 也递归 -> 永远找不到 base case，无限递归
   - o 开头判断 `if (n % 2 == 0) return {};`

2. **左子树枚举范围错误**
   - x `for (int i = 0; i < n; i++)` -> i=0 意味着左子树为空（不是满二叉树），i=n-1 意味着右子树为空
   - o `for (int i = 1; i < n; i += 2)` -> 从 1 开始，步长 2，确保左右都是正奇数

3. **memo 用法错误——值拷贝而非引用**
   - x 在递归中 `auto left = allPossibleFBT(i)` 每次拷贝整个 vector
   - o 使用 `auto& left = allPossibleFBT(i)` 返回引用避免拷贝

4. **组合时忘记创建新的根节点**
   - x 每次组合复用同一个根节点 -> 所有组合指向同一个根
   - o 每次组合都 `new TreeNode(0)`，创建独立的根节点

5. **混淆满二叉树和完全二叉树**
   - 满二叉树(Full Binary Tree): 每个节点有 0 或 2 个孩子
   - 完全二叉树(Complete Binary Tree): 除最后一层外每层都满，最后一层从左到右填充
   - 完美二叉树(Perfect Binary Tree): 所有叶子在同一层，所有内部节点有 2 个孩子

## 面试追问

**Q1: 为什么 n 必须是奇数？**
-> 满二叉树中，每个内部节点有 2 个孩子。设内部节点数为 k，叶子数为 l，则 l = k + 1（因为每个内部节点"产生"2 个分支但"消耗"1 个来自父节点的分支，根节点额外产生 1 个）。总节点 n = k + l = k + (k+1) = 2k+1，必为奇数。

**Q2: n 个节点的满二叉树有多少种？**
-> 第 (n-1)/2 个卡塔兰数 C_m = C(2m, m) / (m+1)，其中 m = (n-1)/2。例如 n=7 时 m=3，C_3 = 5。

**Q3: 子树共享指针会不会有问题？**
-> 在只读场景下没问题。如果需要修改某棵树的节点值，必须深拷贝，否则会影响其他共享同一子树的树。

**Q4: 能不能用位运算或括号序列来枚举满二叉树？**
-> 可以。满二叉树和合法括号序列一一对应（卡塔兰数的组合解释）。n 个节点的满二叉树 ↔ (n-1)/2 对括号的合法序列。但从括号序列构造树结构需要额外解析步骤。

## 相关题型

- **LeetCode 95 不同的二叉搜索树 II** — 非常相似的递归枚举框架：枚举根节点值，递归构造左右子树，两两组合。区别是 BST 有值约束，本题是无值约束的满二叉树。
- **LeetCode 96 不同的二叉搜索树** — 只需计算数量（不构造树），用卡塔兰数公式或 DP。本题如果只问数量也可以用卡塔兰数。
- **LeetCode 241 为运算表达式设计优先级** — 同为"枚举分割点，递归左右两部分，两两组合"的框架，只是操作对象从树变成了表达式。
- **LeetCode 22 括号生成** — 卡塔兰数的经典应用，满二叉树的数量和合法括号序列的数量相同。
