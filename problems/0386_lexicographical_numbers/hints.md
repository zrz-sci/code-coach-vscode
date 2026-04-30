# 386. Lexicographical Numbers - 字典序排数

## 核心思路
将 1 到 n 的数字按字典序排列。把数字想象成一棵**10叉字典树（Trie）**，根节点的子节点是 1-9，每个节点的子节点是在末尾追加 0-9。字典序遍历就是这棵树的**前序遍历（DFS）**。

## 思维链
1. 字典序 = 把数字当字符串排序
2. 排序做法 O(n log n) 不满足 O(n) 要求
3. 观察规律：1→10→100→...→10x→11→12→...→19→2→20→...
4. 这就是一棵 10 叉树的 DFS 前序遍历
5. 树的根的子节点是 1-9，每个节点 x 的子节点是 x*10+0 到 x*10+9
6. DFS 时剪枝：子节点值 > n 则不继续

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 迭代模拟 DFS | O(n) | O(1) | 最优，无递归开销 |
| 递归 DFS | O(n) | O(log n) | 直观，递归栈深度为数字位数 |

## 关键提示
1. 迭代法核心：当前数 cur，优先尝试 cur*10（深入子节点）
2. 如果 cur*10 > n，尝试 cur+1（兄弟节点）
3. 如果 cur+1 的末尾是 0（如 20→变成了下一层），或 cur+1 > n，需要回溯（cur/10）
4. 回溯条件：`cur % 10 == 9` 或 `cur + 1 > n` 时需要除以 10 回退

## 解法详解

### 解法一：迭代模拟 DFS
```cpp
class Solution {
public:
    vector<int> lexicalOrder(int n) {
        vector<int> result;
        int cur = 1;
        for (int i = 0; i < n; i++) {
            result.push_back(cur);
            if (cur * 10 <= n) {
                cur *= 10;           // 深入：1→10→100
            } else {
                while (cur % 10 == 9 || cur + 1 > n) {
                    cur /= 10;       // 回溯
                }
                cur++;               // 兄弟：11→12
            }
        }
        return result;
    }
};
```

### 解法二：递归 DFS
```cpp
class Solution {
    void dfs(int cur, int n, vector<int>& result) {
        if (cur > n) return;
        result.push_back(cur);
        for (int i = 0; i <= 9; i++) {
            if (cur * 10 + i > n) return;
            dfs(cur * 10 + i, n, result);
        }
    }
public:
    vector<int> lexicalOrder(int n) {
        vector<int> result;
        for (int i = 1; i <= 9; i++) {
            dfs(i, n, result);
        }
        return result;
    }
};
```

## 易错点
| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 回溯时只 `cur /= 10` 一次 | ✓ `while (cur%10==9 \|\| cur+1>n) cur/=10` | 可能需要连续回溯多层 |
| ✗ DFS 起始从 0 开始 | ✓ 从 1 到 9 开始 | 字典序中没有前导零 |
| ✗ 不检查 `cur+1 > n` | ✓ 回溯条件包含 `cur+1 > n` | 例如 n=13 时 cur=13 需要回溯 |
| ✗ 递归中 `for i from 1 to 9` | ✓ `for i from 0 to 9`（子节点追加0-9） | 只有根的子节点从1开始 |

## 面试追问

**Q1: 为什么迭代解法的空间复杂度是 O(1)？**
> 除了结果数组外只用了一个变量 cur，没有递归栈也没有额外数据结构。结果数组是必须的输出，不计入额外空间。

**Q2: 如果要求返回第 k 个字典序数字而不是全部，能优化吗？**
> 是的，可以用"计算子树大小"的方法在 O(log^2 n) 时间内找到第 k 个。这就是 LeetCode 440 题。

**Q3: 这道题和 Trie 有什么关系？**
> 字典序排列等价于一棵隐式 10 叉 Trie 的前序遍历。每个数字是一个节点，其子节点是在末尾追加 0-9。理解这个关系有助于解决类似的字典序问题。

## 相关题型
- [440. K-th Smallest in Lexicographical Order](https://leetcode.com/problems/k-th-smallest-in-lexicographical-order/) - 字典序第 k 小
- [14. Longest Common Prefix](https://leetcode.com/problems/longest-common-prefix/) - 字典序相关
- [1032. Stream of Characters](https://leetcode.com/problems/stream-of-characters/) - Trie 应用
