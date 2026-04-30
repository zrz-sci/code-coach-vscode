# 1477. 找两个和为目标值且不重叠的子数组

## 核心思路

在正整数数组中找**两个不重叠的连续子数组**，使得每个子数组的元素和恰好等于 `target`，且两个子数组的**长度之和最小**。利用**滑动窗口**找出所有和为 target 的子数组，再用 **DP 前缀最优**记录每个位置之前的最短合法子数组长度，实现一遍扫描内完成最优组合的查找。

## 思维链

1. **读完题第一反应**：需要找所有和为 target 的子数组，然后选两个不重叠的使总长度最小。暴力做法是先枚举所有子数组（O(n^2)），再两两组合检查不重叠（O(k^2)），太慢了。

2. **关键观察 — 元素都是正整数**：题目约束 `arr[i] >= 1`，这意味着：
   - 子数组和随右端点扩展**单调递增**
   - 适合用**滑动窗口**：当窗口和 > target 时收缩左端点，当和 == target 时找到一个合法子数组
   - 每个右端点最多对应一个合法的左端点

3. **如何保证"不重叠"**：两个子数组不重叠 = 一个完全在另一个左边。如果当前找到子数组 `[left, right]`，那么第二个子数组必须完全在 `left` 之前结束，即在 `[0, left-1]` 范围内。

4. **DP 前缀最优 — 关键数据结构**：维护 `best[i]` = 以索引 i 或更早位置结尾的、和为 target 的最短子数组长度。对于当前子数组 `[left, right]`，能和它组合的最优解是 `best[left-1]`。

5. **一遍扫描完成**：滑动窗口从左到右扫描，每找到一个合法子数组就用 `best[left-1] + curLen` 尝试更新答案，同时更新 `best[right]`。

```
滑动窗口 + DP 前缀最优示意图:

arr = [3, 1, 1, 1, 5, 1, 2, 1],  target = 3

第1步: right=0, sum=3 == target, 找到 [3]  len=1
  窗口: [ 3 ] 1  1  1  5  1  2  1
         ^^^
  best[0] = 1,  无左侧可组合

第2步: right=1, sum=1 < target, 继续扩展
  窗口:   3 [ 1 ] 1  1  5  1  2  1

第3步: right=2, sum=1+1=2 < target
  窗口:   3 [ 1  1 ] 1  5  1  2  1

第4步: right=3, sum=1+1+1=3 == target, 找到 [1,1,1]  len=3
  窗口:   3 [ 1  1  1 ] 5  1  2  1
              ^^^^^^^^
  best[3] = min(best[2], 3) = min(1, 3) = 1
  可组合: len=3 + best[0]=1 = 4  → ans=4

第5步: right=4, sum加5=8>3, 收缩...直到sum=5>3, 继续收缩...
  无法等于target

第6步: right=5, 继续调整窗口
  ...最终找到 [1,2] len=2 (right=6, left=5)
  可组合: len=2 + best[4]=1 = 3  → ans=3 ✓

最终答案: 3  ( [3] + [1,2] )
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试推荐 |
|------|------|------|------|----------|
| 滑动窗口 + DP前缀最优 | 窗口找子数组 + best数组记录前缀最优 | O(n) | O(n) | ⭐ 首选 |
| 前缀和 + HashMap + DP | 用HashMap存前缀和位置 | O(n) | O(n) | 备选（支持含0/负数） |

## 解法详解

### 解法1: 滑动窗口 + DP前缀最优 -- O(n) / O(n)

**思考过程**：正整数数组中，滑动窗口可以高效找到所有和为 target 的子数组。核心难点是如何在找到子数组后快速找到与之不重叠的最短伙伴。

**关键洞察**：维护 `best[i]` 表示以索引 i 或之前任何位置结尾的最短合法子数组。当找到子数组 `[left, right]` 时，不重叠的最优伙伴一定在 `[0, left-1]` 范围内，即 `best[left-1]`。

```
best[i] 的传递机制:

idx:   0   1   2   3   4   5   6   7
arr:   3   1   1   1   5   1   2   1
best:  1   1   1   1   1   1   1   1
       ↑               ↑
    [3] len=1      best向右传递(取min)

当 right=3 找到 [1,1,1]:
  best[3] = min(best[2]=1, len=3) = 1  (前面的 [3] 更短)
  组合: 3 + best[0]=1 = 4

当 right=6 找到 [1,2]:
  best[6] = min(best[5]=1, len=2) = 1
  组合: 2 + best[4]=1 = 3  ← 最优!
```

```cpp
class Solution {
public:
    int minSumOfLengths(vector<int>& arr, int target) {
        int n = arr.size();
        // best[i] = 以 i 或之前结尾的、和为target的最短子数组长度
        vector<int> best(n, INT_MAX);
        int ans = INT_MAX, sum = 0, left = 0;
        
        for (int right = 0; right < n; right++) {
            sum += arr[right];
            // 正整数 → 和单调递增，收缩左端点
            while (sum > target) sum -= arr[left++];
            
            if (sum == target) {
                int len = right - left + 1;
                // 尝试与左侧最优子数组组合
                if (left > 0 && best[left - 1] != INT_MAX)
                    ans = min(ans, len + best[left - 1]);
                // 更新 best[right]
                best[right] = (right > 0) ? min(best[right - 1], len) : len;
            } else {
                // 当前right没有合法子数组，继承前缀最优
                best[right] = (right > 0) ? best[right - 1] : INT_MAX;
            }
        }
        return ans == INT_MAX ? -1 : ans;
    }
};
```

**逐行解析**：
- `best[i] = INT_MAX` 初始化为无穷大，表示前 i+1 个元素中没找到合法子数组
- `while (sum > target)` 利用正整数性质收缩窗口
- `best[left-1]` 是"完全在当前子数组左边"的最优解
- `best[right] = min(best[right-1], len)` 确保前缀最优向右传递

### 解法2: 前缀和 + HashMap -- O(n) / O(n)

**思考过程**：如果题目的数组可能包含 0 或负数（虽然本题不会），滑动窗口就不适用了。此时可以用**前缀和 + HashMap**方法。

**核心思路**：`prefix[j] - prefix[i] == target` 意味着 `arr[i..j-1]` 的和为 target。用 HashMap 存储每个前缀和最后出现的位置，就能快速定位子数组。

```cpp
class Solution {
public:
    int minSumOfLengths(vector<int>& arr, int target) {
        int n = arr.size();
        unordered_map<int, int> prefixMap; // 前缀和 → 位置
        prefixMap[0] = -1; // 前缀和为0在位置-1（虚拟起点）
        
        vector<int> best(n, INT_MAX); // best[i] = [0..i]中最短合法子数组
        int ans = INT_MAX, prefix = 0;
        
        for (int i = 0; i < n; i++) {
            prefix += arr[i];
            
            // 查找前缀和为 prefix - target 的位置
            if (prefixMap.count(prefix - target)) {
                int start = prefixMap[prefix - target] + 1;
                int len = i - start + 1;
                // 尝试与左侧最优组合
                if (start > 0 && best[start - 1] != INT_MAX)
                    ans = min(ans, len + best[start - 1]);
                best[i] = (i > 0) ? min(best[i - 1], len) : len;
            } else {
                best[i] = (i > 0) ? best[i - 1] : INT_MAX;
            }
            
            prefixMap[prefix] = i; // 更新前缀和位置（保留最新的）
        }
        return ans == INT_MAX ? -1 : ans;
    }
};
```

## 关键提示

- **提示1**：数组元素全为正整数（`arr[i] >= 1`），这是滑动窗口能用的前提条件。正整数保证了窗口和随扩展单调递增。
- **提示2**：`best[i]` 记录前缀最优，核心作用是"在 O(1) 时间内找到不重叠的最短伙伴"。
- **提示3**：`best` 数组的值只会**单调不增地向右传递**。一旦找到更短的子数组，后续位置都能享受这个更优值。
- **提示4**：组合检查 `ans = min(ans, len + best[left-1])` 中用 `left-1` 而不是 `left`，因为当前子数组从 `left` 开始，伙伴必须在 `left` 之前结束。
- **提示5**：注意 INT_MAX 溢出保护：`best[left-1] != INT_MAX` 检查必须在加法之前。

## 易错点

1. **best 数组传递遗漏**：忘记在 `sum != target` 时也要传递 `best[right] = best[right-1]`，导致后续位置无法利用前面已找到的子数组。

2. **INT_MAX 溢出**：`len + best[left-1]` 如果 `best[left-1]` 是 INT_MAX，相加会溢出成负数。务必先检查 `!= INT_MAX`。

3. **边界 left=0**：当 `left == 0` 时，没有 `best[left-1]`（即 `best[-1]`），不能组合，需要特判跳过。

4. **误用双重循环**：不要先找出所有子数组再两两配对，这会导致 O(n^2) 甚至更差的复杂度。

5. **滑动窗口不支持负数/零**：如果将来遇到类似题但数组含0或负数，必须改用前缀和+HashMap方法。

## 复杂度分析

| 维度 | 分析 |
|------|------|
| 时间 | O(n)：滑动窗口每个元素最多被 left 和 right 各访问一次 |
| 空间 | O(n)：best 数组长度为 n |

## 相关题型

- **209. 长度最小的子数组** — 滑动窗口基础：找和 >= target 的最短子数组
- **325. 和等于 k 的最长子数组长度** — 前缀和+HashMap：找和为k的最长子数组
- **560. 和为 K 的子数组** — 前缀和+HashMap：统计和为k的子数组个数
- **862. 和至少为 K 的最短子数组** — 含负数，需要单调双端队列

## Follow-up 思考

1. **如果数组含 0 或负数怎么办？** 滑动窗口失效，必须用前缀和+HashMap方法（解法2）。
2. **如果要求找 k 个不重叠子数组（k > 2）？** 需要更复杂的 DP：`dp[i][j]` = 前 i 个元素中选 j 个不重叠子数组的最小总长度。
3. **如果只需要找两个不重叠子数组使和相等（不一定等于target）？** 这是一个完全不同的问题，需要枚举所有可能的和值。
