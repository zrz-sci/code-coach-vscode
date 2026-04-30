# 487. 最大连续1的个数 II (Max Consecutive Ones II)

## 核心思路

滑动窗口：维护一个最多包含 1 个 0 的窗口，窗口内就是翻转一个 0 后能得到的最长连续 1。
当窗口内 0 的个数超过 1 时，移动左指针缩小窗口。

## 思维链

1. **问题转化**：翻转至多一个 0 后的最长连续 1 = 找最长子数组使得其中至多有 1 个 0
2. **滑动窗口模型**：维护窗口 [left, right]，记录窗口内 0 的个数
3. **扩展右边界**：right 向右移动，若 nums[right] == 0 则 zero_count++
4. **收缩左边界**：当 zero_count > 1 时，left 右移直到吐出一个 0
5. **更新答案**：ans = max(ans, right - left + 1)
6. **流式处理优化**：不需要存储整个数组，只需记录上一个 0 的位置

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 滑动窗口 | O(n) | O(1) | ⭐ 最优解 |
| DP (两个状态) | O(n) | O(1) | 思路直接 |
| 记录前一个0位置 | O(n) | O(1) | 适合流式输入 |

## 关键提示

1. 这是 LC 485 (Max Consecutive Ones) 的进阶版，允许翻转一个 0
2. 滑动窗口适用条件：子数组问题 + 单调性（区间越大越可能不满足）
3. Follow-up 要求流式处理：不能存储整个数组，用"记录上一个0位置"的方法
4. 可推广到 LC 1004 (翻转至多 k 个 0)

## 解法详解

### 解法1: 滑动窗口

```cpp
class Solution1 {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int left = 0, right = 0;
        int zeroCount = 0;
        int ans = 0;
        
        while (right < (int)nums.size()) {
            if (nums[right] == 0) zeroCount++;
            
            // 窗口内 0 超过 1 个, 收缩左边界
            while (zeroCount > 1) {
                if (nums[left] == 0) zeroCount--;
                left++;
            }
            
            ans = max(ans, right - left + 1);
            right++;
        }
        
        return ans;
    }
};
```

### 解法2: DP (两个状态)

```cpp
class Solution2 {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        // dp0: 以当前位置结尾、未翻转过0的最长连续1
        // dp1: 以当前位置结尾、已翻转过1个0的最长连续1
        int dp0 = 0, dp1 = 0;
        int ans = 0;
        
        for (int num : nums) {
            if (num == 1) {
                dp0++;
                dp1++;
            } else {
                // 翻转当前 0: dp1 = dp0 + 1 (在之前未翻转的基础上翻转这个0)
                dp1 = dp0 + 1;
                dp0 = 0;  // 不翻转则连续1断了
            }
            ans = max(ans, dp1);
        }
        
        return ans;
    }
};
```

### 解法3: 记录上一个0的位置（流式友好）

```cpp
class Solution3 {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int ans = 0;
        int prevZeroIdx = -1;  // 上一个 0 的位置
        int left = 0;          // 当前段的左边界
        
        for (int i = 0; i < (int)nums.size(); i++) {
            if (nums[i] == 0) {
                left = prevZeroIdx + 1;  // 新的左边界
                prevZeroIdx = i;
            }
            ans = max(ans, i - left + 1);
        }
        
        return ans;
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 原因 |
|-----------|-----------|------|
| `while (zeroCount >= 1)` | `while (zeroCount > 1)` | 允许翻转 1 个 0，所以窗口内允许有 1 个 0 |
| 遇到 0 就 `dp1 = 0` | 遇到 0 时 `dp1 = dp0 + 1` | 翻转当前 0 后，连续1长度 = 之前未翻转段 + 1 |
| `prevZeroIdx = 0` 初始化 | `prevZeroIdx = -1` 初始化 | -1 表示还没遇到过 0 |
| 全是 1 的情况没处理 | 滑动窗口自然处理 | 没有 0 时窗口扩展到整个数组 |

## 面试追问

**Q1: 如何处理 follow-up 的流式输入？**
A: 用解法3（记录上一个0位置）或解法2（DP），只需 O(1) 空间，不需要存储整个数组。每来一个数更新状态即可。

**Q2: 如果允许翻转 k 个 0 呢？**
A: 这就是 LC 1004。滑动窗口中把 `zeroCount > 1` 改为 `zeroCount > k` 即可。时间仍为 O(n)。

**Q3: 解法2中 dp0 和 dp1 的含义能否用更通俗的方式解释？**
A: dp0 是"当前位置往左连续1的长度（没动过0）"，dp1 是"当前位置往左连续1的长度（已经翻了1个0）"。遇到 0 时，dp1 可以在 dp0 的基础上"吃掉"这个 0。

## 相关题型

- [485. 最大连续1的个数](../0485_max_consecutive_ones/) - 基础版
- [1004. 最大连续1的个数 III](../1004_max_consecutive_ones_iii/) - 翻转 k 个 0
- [340. 至多包含 K 个不同字符的最长子串](../0340_longest_substring_with_at_most_k_distinct_characters/) - 滑动窗口
- [424. 替换后的最长重复字符](../0424_longest_repeating_character_replacement/) - 滑动窗口变体
