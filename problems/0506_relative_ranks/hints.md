# 506. 相对名次

## 核心思路

**排序 + 索引映射**：将分数和原始索引绑定后排序，按排名分配奖牌/名次字符串，写回原位置。

## 思维链

1. **问题分析**：需要知道每个分数在全局中的排名，然后根据排名生成字符串
2. **保留原始索引**：排序会打乱顺序，所以需要记录每个分数的原始下标
3. **排序方式**：按分数降序排列，排序后的位置就是名次（第0个=第1名）
4. **名次映射**：前三名分别是 "Gold Medal"、"Silver Medal"、"Bronze Medal"，其余是数字字符串
5. **写回结果**：根据排序后的索引映射，将名次写到对应的原始位置

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 排序 + 索引数组 | O(n log n) | O(n) | ⭐⭐⭐⭐⭐ |
| 最大堆 | O(n log n) | O(n) | ⭐⭐⭐⭐ |

## 关键提示

- 所有分数**互不相同**，不需要处理并列情况
- 前三名有特殊字符串，注意不要写成 "1"、"2"、"3"
- 名次从 1 开始（不是从 0 开始），第 4 名的字符串是 "4" 不是 "3"
- 可以用 map 建立分数→名次的映射，也可以用索引数组排序

## 解法详解

### 解法1: 排序 + 索引数组（推荐）

**思路**：创建索引数组按分数降序排序，然后遍历排序后的索引分配名次。

```cpp
class Solution {
public:
    vector<string> findRelativeRanks(vector<int>& score) {
        int n = score.size();
        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        // 按分数降序排序索引
        sort(idx.begin(), idx.end(), [&](int a, int b) {
            return score[a] > score[b];
        });
        
        vector<string> result(n);
        for (int i = 0; i < n; i++) {
            if (i == 0) result[idx[i]] = "Gold Medal";
            else if (i == 1) result[idx[i]] = "Silver Medal";
            else if (i == 2) result[idx[i]] = "Bronze Medal";
            else result[idx[i]] = to_string(i + 1);
        }
        
        return result;
    }
};
```

### 解法2: 最大堆

**思路**：用最大堆按分数排序，依次弹出并分配名次。

```cpp
class Solution {
public:
    vector<string> findRelativeRanks(vector<int>& score) {
        int n = score.size();
        // 最大堆：(分数, 原始索引)
        priority_queue<pair<int,int>> pq;
        for (int i = 0; i < n; i++) {
            pq.push({score[i], i});
        }
        
        vector<string> result(n);
        int rank = 1;
        while (!pq.empty()) {
            auto [s, idx] = pq.top(); pq.pop();
            if (rank == 1) result[idx] = "Gold Medal";
            else if (rank == 2) result[idx] = "Silver Medal";
            else if (rank == 3) result[idx] = "Bronze Medal";
            else result[idx] = to_string(rank);
            rank++;
        }
        
        return result;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `result[i] = "Gold Medal"` | ✓ `result[idx[i]] = "Gold Medal"` | 要写到原始位置，不是排序后位置 |
| ✗ `to_string(i)` | ✓ `to_string(i + 1)` | 名次从 1 开始，索引从 0 开始 |
| ✗ 升序排序 | ✓ 降序排序 | 第 1 名是分数最高的 |
| ✗ 前三名也用数字 | ✓ 前三名用 "Gold/Silver/Bronze Medal" | 注意完整拼写含空格 |

## 面试追问

**Q1: 如果分数可以相同（并列），如何处理？**

> 需要定义并列规则：同分同名次。可以先排序，遇到相同分数时分配相同名次，下一个不同分数的名次跳过相应数量。

**Q2: 能否不排序，O(n) 解决？**

> 如果分数范围有限（如 0~10^6），可以用计数排序/桶排序实现 O(n + maxScore)。或者用 map 也可以 O(n) 但需要哈希。

**Q3: 如果只需要返回前 k 名的信息，如何优化？**

> 用大小为 k 的最大堆，或者用 partial_sort / nth_element 做部分排序，O(n + k log n)。

## 相关题型

- [1331. Rank Transform of an Array](https://leetcode.com/problems/rank-transform-of-an-array/) - 数组排名转换
- [1366. Rank Teams by Votes](https://leetcode.com/problems/rank-teams-by-votes/) - 复杂排名规则
- [215. Kth Largest Element in an Array](https://leetcode.com/problems/kth-largest-element-in-an-array/) - 部分排序
