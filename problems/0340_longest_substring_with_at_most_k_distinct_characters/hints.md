# 340. 至多包含 K 个不同字符的最长子串

## 核心思路
维护一个滑动窗口，用哈希表记录窗口内每个字符的出现次数。当窗口内不同字符数超过 k 时，收缩左边界直到满足条件。在整个过程中记录满足条件的最大窗口长度。

## 思维链
1. 看到"最长子串 + 约束条件" → 经典滑动窗口模板
2. 约束条件是"至多 k 个不同字符" → 需要一个数据结构来追踪窗口内字符种类数
3. 用 `unordered_map<char, int>` 记录每个字符的频次，map 的 size 就是不同字符数
4. 右指针扩张窗口 → 加入新字符 → 检查是否违约 → 若违约，左指针收缩（减频次，频次为 0 则删除键）
5. 每步更新 `ans = max(ans, right - left + 1)`

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 滑动窗口 + HashMap | O(n) | O(k) | 标准做法，面试首选 |
| 滑动窗口 + 有序Map记录最右位置 | O(n log k) | O(k) | 可精确定位收缩目标 |

## 关键提示
1. **边界情况**：`k == 0` 时直接返回 0，不要遗漏
2. **删除时机**：当某字符频次减为 0 时，必须从 map 中 `erase`，否则 `map.size()` 不准确
3. **窗口定义**：`[left, right]` 闭区间，窗口大小 = `right - left + 1`
4. **本题是 LC 159 的推广版**：159 是 k=2 的特殊情况

## 解法详解

### 解法一：滑动窗口 + HashMap（推荐）

**思路**：维护一个频次表，右指针不断扩张，当不同字符数 > k 时收缩左指针。

```cpp
class Solution {
public:
    int lengthOfLongestSubstringKDistinct(string s, int k) {
        if (k == 0) return 0;
        unordered_map<char, int> freq;
        int left = 0, ans = 0;
        for (int right = 0; right < s.size(); right++) {
            freq[s[right]]++;
            while (freq.size() > k) {
                char c = s[left];
                if (--freq[c] == 0) freq.erase(c);
                left++;
            }
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};
```

**复杂度**：时间 O(n)，空间 O(k)

### 解法二：滑动窗口 + 有序Map记录最右出现位置

**思路**：用 `map<char, int>` 记录每个字符在窗口中最后一次出现的位置。当字符种类超过 k 时，找到最右位置最小的字符，将 left 移到该位置的下一个。

```cpp
class Solution {
public:
    int lengthOfLongestSubstringKDistinct(string s, int k) {
        if (k == 0) return 0;
        unordered_map<char, int> lastPos; // char -> 最右出现位置
        int left = 0, ans = 0;
        for (int right = 0; right < s.size(); right++) {
            lastPos[s[right]] = right;
            if (lastPos.size() > k) {
                // 找最右位置最小的字符
                int minPos = INT_MAX;
                char minChar;
                for (auto& [c, pos] : lastPos) {
                    if (pos < minPos) { minPos = pos; minChar = c; }
                }
                lastPos.erase(minChar);
                left = minPos + 1;
            }
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};
```

**复杂度**：时间 O(n*k)（内层查找最小位置），空间 O(k)

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 不检查 `k == 0` | `if (k == 0) return 0;` | k=0 时不应进入循环 |
| `freq[c]--; if (freq[c] == 0) ...` | `if (--freq[c] == 0) freq.erase(c);` | 必须 erase 才能让 size 正确 |
| `while (freq.size() >= k)` | `while (freq.size() > k)` | 是"至多 k 个"，不是"少于 k 个" |
| 用 `int[256]` + 手动计数种类数 | 可以，但要在频次从 0→1 和 1→0 时更新 count | 容易漏更新 |

## 面试追问

**Q1: 如果改成"恰好 k 个不同字符"怎么办？**
> 用"恰好 k = 至多 k - 至多 (k-1)"的技巧。分别调用两次滑动窗口函数，结果相减即可。这是一个非常经典的转化思路。

**Q2: 如果字符集不是 ASCII 而是 Unicode，有什么影响？**
> 逻辑不变，用 `unordered_map` 本身就支持任意字符。但如果用数组 `int[256]` 的优化就不适用了。时间复杂度不受影响，空间从 O(256) 变为 O(字符集大小)。

**Q3: 能否做到 O(n) 且不用 while 循环收缩？**
> 可以。用"不缩小窗口"的滑动窗口变体：当 `freq.size() > k` 时，左边界只移动一步（而非用 while 收缩到合法）。最终 `right - left` 就是答案。这种写法 `right` 和 `left` 各自最多移动 n 次，但代码更精简。注意此时窗口不保证总是合法的，但最终结果正确。

## 相关题型
- [3. 无重复字符的最长子串](https://leetcode.com/problems/longest-substring-without-repeating-characters/) - k = 字符种类数时的特例
- [159. 至多包含两个不同字符的最长子串](https://leetcode.com/problems/longest-substring-with-at-most-two-distinct-characters/) - k=2 特例
- [76. 最小覆盖子串](https://leetcode.com/problems/minimum-window-substring/) - 滑动窗口经典题
- [424. 替换后的最长重复字符](https://leetcode.com/problems/longest-repeating-character-replacement/) - 滑动窗口变种
- [992. K 个不同整数的子数组](https://leetcode.com/problems/subarrays-with-k-different-integers/) - 恰好 k 个 = 至多 k - 至多(k-1)
