# 517. 超级洗衣机 (Super Washing Machines)

**难度**: Hard | **标签**: Greedy, Array

## 核心思路

将问题转化为"每个位置需要通过的净流量"。答案是所有位置的 max(|前缀差|, 单机需输出量)。关键洞察：每一步可以同时移动多台机器的衣服，但单台机器每步最多只能向一个方向传递一件。

## 思维链

1. 总数不能整除 n → 返回 -1
2. 目标值 `avg = sum / n`，每台机器的差值 `diff[i] = machines[i] - avg`
3. 考虑位置 i 左边的"净需求" = `prefix_sum(diff[0..i-1])`
   - 这就是必须穿过 i 和 i-1 之间边的净流量
4. 瓶颈来源有两个：
   - `|prefix_sum|`：因为左右两侧的净流量可以双向并行，取绝对值
   - `diff[i] > 0`（即 `machines[i] - avg > 0`）：单机输出不可并行，每步只能给出1件
5. 答案 = `max(max(|prefix_sum|), max(diff[i] when diff[i]>0))`

## 解法概览

| 解法 | 时间 | 空间 | 推荐 |
|------|------|------|------|
| 贪心 — 前缀和 + 净流量 | O(n) | O(1) | ⭐⭐⭐ |

## 关键提示

1. **为什么取 max(|prefix|, diff[i])**：prefix 代表"边上"的流量瓶颈，diff[i]>0 代表"点上"的输出瓶颈
2. 多台机器可以同时操作 → 边的流量可以并行 → 取绝对值即可
3. 单台机器每步最多输出1件 → 如果某台多出 k 件，至少需要 k 步

## 解法详解

### 解法1: 贪心 — 前缀和

```cpp
class Solution {
public:
    int findMinMoves(vector<int>& machines) {
        int n = machines.size();
        int sum = 0;
        for (int x : machines) sum += x;
        if (sum % n != 0) return -1;
        
        int avg = sum / n;
        int res = 0, prefix = 0;
        for (int i = 0; i < n; i++) {
            int diff = machines[i] - avg;
            prefix += diff;
            res = max(res, max(abs(prefix), diff));
        }
        return res;
    }
};
```

## 易错点

| 错误 | 正确 |
|------|------|
| ✗ 只取 `max(abs(prefix))` | ✓ 还要取 `max(diff)`（单机输出瓶颈） |
| ✗ `res = max(res, abs(diff))` | ✓ `res = max(res, diff)`（只有多出来的才是瓶颈，缺衣服可并行接收） |
| ✗ 忘记判断 `sum % n != 0` | ✓ 不能整除时返回 -1 |

## 面试追问

- **Q1**: 为什么 `diff < 0` 不算瓶颈？
  → 缺衣服的机器可以同时从左右接收，不是瓶颈；但多出衣服的机器每步只能给出1件
- **Q2**: 前缀和的物理意义是什么？
  → `prefix[i]` 表示前 i 台机器整体需要从右侧净借/还的衣服数量，即穿过边 (i, i+1) 的净流量
- **Q3**: 如果每步只能选1台机器操作呢？
  → 答案变成 `sum(|diff[i]|) / 2`，完全不同的问题

## 相关题型

- [453. 最小操作次数使数组元素相等](https://leetcode.com/problems/minimum-moves-to-equal-array-elements/)
- [462. 最小操作次数使数组元素相等 II](https://leetcode.com/problems/minimum-moves-to-equal-array-elements-ii/)
