# 159. 至多包含两个不同字符的最长子串

## 核心思路
本质是滑动窗口问题：维护一个最多包含两种字符的窗口，右边扩展、左边收缩，追踪最大窗口长度。

## 思维链
1. **暴力思考**：枚举所有子串，检查每个子串的不同字符数 <= 2，取最长的。O(n^3) 显然不可接受。
2. **观察单调性**：如果 [i, j] 满足条件，那么 [i, j-1] 也满足 —— 这是典型的滑动窗口特征。
3. **窗口维护**：用哈希表记录窗口内每个字符的出现次数，当不同字符超过 2 种时收缩左边界。
4. **收缩策略**：左指针右移，减少对应字符计数，计数为 0 时从哈希表中删除该字符。
5. **优化记录**：每次窗口合法时更新 ans = max(ans, right - left + 1)。
6. **泛化思考**：这个模板可以直接推广到 "至多 K 个不同字符" (LC 340)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 滑动窗口 + 哈希表 | 维护字符频率计数的窗口 | O(n) | O(1) | ⭐ 必知 |
| 滑动窗口 + 最后出现位置 | 记录每个字符最后出现的索引 | O(n) | O(1) | 推荐 |

## 关键提示
1. **窗口不变量**：窗口内 distinct 字符数 <= 2，一旦违反就收缩左边界
2. **删除时机**：当 `count[ch] == 0` 时必须从 map 中 erase，否则 `map.size()` 不准确
3. **第二种解法的精髓**：不用计数，用"最后出现位置"。当需要淘汰一个字符时，淘汰最后出现位置最小的那个字符，并将 left 移到该位置 + 1
4. **边界**：字符串长度 <= 2 时可以直接返回 s.length()

```
示例: s = "eceba"

窗口滑动过程:
e c e b a
[e]           -> {e:1}, len=1
[e c]         -> {e:1, c:1}, len=2
[e c e]       -> {e:2, c:1}, len=3  <-- 当前最大
[e c e b]     -> {e:2, c:1, b:1} 3种! 收缩左边
  [c e b]     -> 还是3种，继续收缩
    [e b]     -> {e:1, b:1}, len=2
    [e b a]   -> {e:1, b:1, a:1} 3种! 收缩
      [b a]   -> {b:1, a:1}, len=2
答案: 3
```

## 解法详解

### 解法1: 滑动窗口 + 哈希计数 (经典模板)

**思路过程**：
- 这是"至多K个不同字符"的通用模板，K=2 只是特例
- 右指针不断扩展窗口，左指针在窗口违规时收缩
- 哈希表记录窗口内每个字符的频率

```cpp
class Solution1 {
public:
    int lengthOfLongestSubstringTwoDistinct(string s) {
        // 为什么用 unordered_map：需要快速查看当前窗口有几种字符
        unordered_map<char, int> freq;
        int left = 0, ans = 0;
        
        for (int right = 0; right < (int)s.size(); ++right) {
            freq[s[right]]++;
            
            // 为什么用 while 不是 if：可能需要连续收缩多次才能满足条件
            while (freq.size() > 2) {
                freq[s[left]]--;
                // 关键：计数为0必须删除，否则 size() 不会减小
                if (freq[s[left]] == 0) {
                    freq.erase(s[left]);
                }
                left++;
            }
            
            ans = max(ans, right - left + 1);
        }
        
        return ans;
    }
};
```

**复杂度分析**：
- 时间 O(n)：每个字符最多被 left 和 right 各访问一次
- 空间 O(1)：哈希表最多 3 个键（超过 2 立即收缩）

### 解法2: 滑动窗口 + 最后出现位置

**思路过程**：
- 不记录频率，而是记录每个字符最后出现的下标
- 当需要淘汰时，找到"最后出现位置最靠左"的字符，删除它
- left 直接跳到被淘汰字符的最后出现位置 + 1

```cpp
class Solution2 {
public:
    int lengthOfLongestSubstringTwoDistinct(string s) {
        // lastPos[ch] = ch 最后一次出现的下标
        unordered_map<char, int> lastPos;
        int left = 0, ans = 0;
        
        for (int right = 0; right < (int)s.size(); ++right) {
            lastPos[s[right]] = right;
            
            if (lastPos.size() > 2) {
                // 找最后出现位置最小的字符 —— 它是最"过期"的
                int minIdx = INT_MAX;
                char minChar = 0;
                for (auto& [ch, idx] : lastPos) {
                    if (idx < minIdx) {
                        minIdx = idx;
                        minChar = ch;
                    }
                }
                // left 跳到被淘汰字符的下一个位置
                left = minIdx + 1;
                lastPos.erase(minChar);
            }
            
            ans = max(ans, right - left + 1);
        }
        
        return ans;
    }
};
```

**复杂度分析**：
- 时间 O(n)：遍历内层循环最多 3 次（map 最多 3 个元素）
- 空间 O(1)：哈希表最多 3 个键

## 易错点

1. **忘记删除计数为0的键**
```
✗ 错误：
freq[s[left]]--;
left++;
// 没有 erase，map.size() 永远不减小

✓ 正确：
freq[s[left]]--;
if (freq[s[left]] == 0) freq.erase(s[left]);
left++;
```

2. **用 if 代替 while**
```
✗ 错误：
if (freq.size() > 2) { ... left++; }
// 一次收缩可能不够

✓ 正确：
while (freq.size() > 2) { ... left++; }
```

3. **窗口长度计算错误**
```
✗ 错误：ans = max(ans, right - left);
✓ 正确：ans = max(ans, right - left + 1);
```

## 面试追问

**Q1: 如果改成"至多 K 个不同字符"怎么办？**
A: 把 `freq.size() > 2` 改成 `freq.size() > k` 即可，模板完全一样。就是 LC 340。

**Q2: 如果要求"恰好 K 个不同字符"怎么办？**
A: 用 `atMost(K) - atMost(K-1)` 的技巧。恰好K个 = 至多K个 - 至多(K-1)个。

**Q3: 如果字符集不是英文字母而是 Unicode，空间复杂度会变吗？**
A: 哈希表存储空间取决于窗口内不同字符数，这里始终 <= K+1 = 3，所以仍然 O(1)。但如果 K 与 n 同阶，空间就是 O(n)。

## 相关题型
- [3. 无重复字符的最长子串](../0003_longest_substring_without_repeating_characters/) - 滑动窗口基础版
- [340. 至多包含 K 个不同字符的最长子串](../0340_longest_substring_with_at_most_k_distinct_characters/) - 本题的泛化
- [76. 最小覆盖子串](../0076_minimum_window_substring/) - 滑动窗口 + 哈希表高级应用
- [424. 替换后的最长重复字符](../0424_longest_repeating_character_replacement/) - 滑动窗口变体
