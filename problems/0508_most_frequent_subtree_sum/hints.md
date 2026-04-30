## 核心思路

后序遍历计算每个节点的子树和，用哈希表统计频率，返回出现次数最多的子树和。

## 思维链

1. **理解子树和**：每个节点的子树和 = 左子树和 + 右子树和 + 当前节点值
2. **如何计算**：后序遍历（先递归左右子树，再计算当前节点的子树和）
3. **如何统计频率**：用 `unordered_map<int, int>` 记录每个子树和出现的次数
4. **如何找最大频率**：遍历哈希表找最大频率，收集所有频率等于最大值的子树和

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| DFS + 哈希表 | 后序遍历 + 频率统计 | O(n) | O(n) | ⭐ |

## 关键提示

- 后序遍历天然适合"自底向上"汇总信息的场景
- 子树和可能为负数，用 `unordered_map<int,int>` 而非数组
- 可能有多个频率相同的子树和，需全部返回

## 解法详解

### 解法一：DFS + 哈希表（推荐）

```cpp
class Solution {
public:
    vector<int> findFrequentTreeSum(TreeNode* root) {
        unordered_map<int, int> freq;  // 子树和 -> 出现次数
        int maxFreq = 0;
        
        // 后序遍历，返回以 node 为根的子树和
        function<int(TreeNode*)> dfs = [&](TreeNode* node) -> int {
            if (!node) return 0;
            int sum = node->val + dfs(node->left) + dfs(node->right);
            maxFreq = max(maxFreq, ++freq[sum]);
            return sum;
        };
        
        dfs(root);
        
        // 收集所有频率等于 maxFreq 的子树和
        vector<int> res;
        for (auto& [sum, cnt] : freq) {
            if (cnt == maxFreq) res.push_back(sum);
        }
        return res;
    }
};
```

**复杂度分析：**
- 时间：O(n)，每个节点访问一次
- 空间：O(n)，哈希表 + 递归栈

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `if (!node) return;` | `if (!node) return 0;` | 需要返回 0 以便父节点累加 |
| 先序遍历计算子树和 | 后序遍历计算子树和 | 必须先知道子树的和才能算当前节点 |
| 只返回一个最大频率的和 | 返回所有最大频率的和 | 题目要求返回所有并列的 |

## 面试追问

**Q1: 如果要求返回出现次数最少的子树和呢？**
> 改为记录 minFreq，遍历时取最小值即可。

**Q2: 如果树非常深，递归栈溢出怎么办？**
> 可以用迭代后序遍历（两栈法或单栈标记法）替代递归。

**Q3: 能否在一次遍历中同时得到结果，不需要第二次扫描哈希表？**
> 可以。维护 maxFreq 和结果列表，每次更新 freq 时：若等于 maxFreq 则加入结果；若大于 maxFreq 则清空结果重新加入。

## 相关题型

- [508. Most Frequent Subtree Sum](https://leetcode.com/problems/most-frequent-subtree-sum/) - 本题
- [572. Subtree of Another Tree](https://leetcode.com/problems/subtree-of-another-tree/) - 子树判断
- [1161. Maximum Level Sum of a Binary Tree](https://leetcode.com/problems/maximum-level-sum-of-a-binary-tree/) - 层级和
