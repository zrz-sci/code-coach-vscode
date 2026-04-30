# 565. Array Nesting - 数组嵌套

## 核心思路
数组是 [0, n-1] 的排列，每个元素的链 i -> nums[i] -> nums[nums[i]] -> ... 必然形成环。遍历每个环计算长度，已访问节点标记跳过。

## 思维链
1. nums 是 [0, n-1] 的排列 -> 每个值恰好出现一次
2. 从任意 i 出发，沿 i -> nums[i] 走，一定会回到起点（形成环）
3. 同一个环中任意起点得到的集合大小相同
4. 只需遍历每个环一次，标记已访问，取最大环长度

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 原地标记遍历环 | O(n) | O(1) | 最优解，修改原数组 |
| visited 数组 | O(n) | O(n) | 不修改原数组 |

## 关键提示
1. 排列的函数图一定由若干不相交环组成
2. 已经在某个环中被访问的元素，不可能属于另一个更长的环
3. 原地标记：将访问过的元素置为 -1 或 n（越界值），省去 visited 数组

## 解法详解

### 解法1: 原地标记（最优） ⭐
```cpp
class Solution {
public:
    int arrayNesting(vector<int>& nums) {
        int maxLen = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] == -1) continue;  // 已访问
            int count = 0, j = i;
            while (nums[j] != -1) {
                int next = nums[j];
                nums[j] = -1;  // 标记已访问
                j = next;
                count++;
            }
            maxLen = max(maxLen, count);
        }
        return maxLen;
    }
};
```

### 解法2: visited 数组
```cpp
class Solution {
public:
    int arrayNesting(vector<int>& nums) {
        int n = nums.size(), maxLen = 0;
        vector<bool> visited(n, false);
        for (int i = 0; i < n; ++i) {
            if (visited[i]) continue;
            int count = 0, j = i;
            while (!visited[j]) {
                visited[j] = true;
                j = nums[j];
                count++;
            }
            maxLen = max(maxLen, count);
        }
        return maxLen;
    }
};
```

## 易错点
- ✗ 对每个起点都重新遍历链 → ✓ O(n^2)，必须标记已访问元素
- ✗ 以为链可能不成环 → ✓ 排列的函数图一定成环
- ✗ 标记时机错误导致死循环 → ✓ 先保存 next 再标记当前

## 面试追问

**Q1: 为什么排列一定形成环？**
n 个节点 n 条边，每个节点出度 1 入度 1，图论中这必然是若干不相交环的并。

**Q2: 能否不修改原数组且 O(1) 空间？**
不能。不修改数组就需要额外空间记录访问状态。可以遍历后恢复原数组。

**Q3: 时间复杂度为什么是 O(n) 而不是 O(n^2)？**
每个元素最多被访问一次（被标记后跳过），所以所有内层循环总计 O(n)。

## 相关题型
- [287. Find the Duplicate Number](https://leetcode.com/problems/find-the-duplicate-number/) - 环检测
- [442. Find All Duplicates in an Array](https://leetcode.com/problems/find-all-duplicates-in-an-array/) - 原地标记技巧
- [457. Circular Array Loop](https://leetcode.com/problems/circular-array-loop/) - 数组中的环
