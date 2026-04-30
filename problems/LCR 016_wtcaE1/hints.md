# LCR 016. 无重复字符的最长子串 (同主站第 3 题)

## 难度: Medium | 标签: Hash Table, String, Sliding Window

---

## 核心思路

经典滑动窗口题：维护一个窗口 `[left, right]`，保证窗口内**无重复字符**。右指针扩展时若遇到重复字符，左指针跳到重复位置的下一个，保证窗口始终合法。关键优化是用**数组记录每个字符最后出现的位置**，实现 O(1) 跳转。

---

## 思维链

1. **读完题第一反应**：暴力是 O(n^2) 检查每个子串。但"最长子串"问题，窗口只需要向右扩展，遇到重复时收缩左边界——自然想到滑动窗口。

2. **窗口不变量**：窗口 `[left, right]` 内所有字符互不相同。

3. **遇到重复怎么办**？
   - **方案 A（HashSet 逐步收缩）**：左指针一个个右移，每次从 Set 中删除 `s[left]`，直到窗口内不含 `s[right]`。
   - **方案 B（数组跳跃）**：记录每个字符最后出现的位置 `last[c]`，当 `s[right]` 已在窗口内时，直接跳 `left = max(left, last[s[right]] + 1)`。

4. **选择判断**：方案 B 更优——左指针直接跳跃，避免逐步收缩。时间复杂度虽然都是 O(n)，但常数更小。

5. **为什么要 `max(left, ...)`**？因为 `last[c]` 记录的可能是窗口之外的旧位置（已被 left 越过），不能让 left 回退。

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| ⭐ 解法1: 滑动窗口 + 数组记录位置 | last[128] 记录最后位置，左指针跳跃 | O(n) | O(128) | 首选 |
| 解法2: 滑动窗口 + HashSet | unordered_set 判断重复，逐步收缩 | O(n) | O(min(n,128)) | 通用 |
| 解法3: 滑动窗口 + HashMap | unordered_map 记录位置，跳跃 | O(n) | O(min(n,128)) | Python常用 |

---

## 关键提示

- **提示1 (入门)**：什么时候需要收缩窗口？当 `s[right]` 在窗口 `[left, right-1]` 中已经出现过。

- **提示2 (优化)**：不需要逐步收缩。如果知道重复字符上次出现在位置 `p`，直接 `left = p + 1`。

- **提示3 (防回退)**：`left = max(left, last[s[right]] + 1)` 中的 `max` 是关键——防止 left 回到已越过的位置。

- **提示4 (字符集)**：题目说"英文字母、数字、符号和空格"，ASCII 码范围 0-127，用 `int[128]` 足够。

- **提示5 (空串)**：`s = ""` 时应返回 0。循环不执行，`ans` 保持初始值 0。

---

## 解法详解

### 滑动窗口执行过程（s = "abcabcbb"）

```
初始: left=0, ans=0, last[] 全部 -1

right=0, s[0]='a': last['a']=-1, left=max(0,-1+1)=0, ans=max(0,0-0+1)=1, last['a']=0
  窗口: [a]

right=1, s[1]='b': last['b']=-1, left=max(0,-1+1)=0, ans=max(1,1-0+1)=2, last['b']=1
  窗口: [a,b]

right=2, s[2]='c': last['c']=-1, left=max(0,-1+1)=0, ans=max(2,2-0+1)=3, last['c']=2
  窗口: [a,b,c]

right=3, s[3]='a': last['a']=0, left=max(0,0+1)=1, ans=max(3,3-1+1)=3, last['a']=3
  窗口: [b,c,a]  ← left 跳过了旧的 'a'

right=4, s[4]='b': last['b']=1, left=max(1,1+1)=2, ans=max(3,4-2+1)=3, last['b']=4
  窗口: [c,a,b]

right=5, s[5]='c': last['c']=2, left=max(2,2+1)=3, ans=max(3,5-3+1)=3, last['c']=5
  窗口: [a,b,c]

right=6, s[6]='b': last['b']=4, left=max(3,4+1)=5, ans=max(3,6-5+1)=2, last['b']=6
  窗口: [c,b]

right=7, s[7]='b': last['b']=6, left=max(5,6+1)=7, ans=max(3,7-7+1)=1, last['b']=7
  窗口: [b]

最终 ans=3, 最长无重复子串 "abc"
```

### 为什么需要 max(left, ...)？

```
反例: s = "abba"

right=0, 'a': left=0, last['a']=0   窗口: [a]
right=1, 'b': left=0, last['b']=1   窗口: [a,b]
right=2, 'b': last['b']=1, left=max(0,2)=2, last['b']=2  窗口: [b]
right=3, 'a': last['a']=0, left=max(2,1)=2  ← 关键! 不是 left=1

如果没有 max, left 会回退到 1, 窗口变成 [b,b,a] 包含重复!
有 max, left 保持 2, 窗口是 [b,a] ✓
```

---

### 解法1: 滑动窗口 + 数组记录位置 (推荐)

**思考过程**：
> "用 last[128] 记录每个字符最后出现的索引，初始化为 -1。遇到字符 c 时，left 跳到 max(left, last[c]+1)，然后更新 last[c] = right。每步计算窗口长度取最大值。"

```cpp
// 解法1: 滑动窗口 + 数组 — O(n) / O(128)
// last[c]: 字符 c 最后出现的索引, 初始 -1
// left 跳跃: left = max(left, last[s[right]] + 1)
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        vector<int> last(128, -1);  // ASCII 字符集
        int ans = 0, left = 0;
        for (int right = 0; right < (int)s.size(); right++) {
            // 如果 s[right] 在窗口内出现过, left 跳到其后面
            left = max(left, last[s[right]] + 1);
            // 更新答案
            ans = max(ans, right - left + 1);
            // 记录当前字符的位置
            last[s[right]] = right;
        }
        return ans;
    }
};
```

**关键点**：
- `last` 初始化为 -1：表示字符还没出现过。`-1 + 1 = 0`，left 不变。
- `max(left, ...)` 防止 left 回退到窗口之外的旧位置。
- 只需一次遍历，left 单调递增，总体 O(n)。

---

### 解法2: 滑动窗口 + HashSet (通用)

**思考过程**：
> "用 Set 记录窗口内的字符。右指针扩展时，如果 s[right] 在 Set 中，就逐步移出 s[left] 直到 Set 中不含 s[right]。"

```cpp
// 解法2: 滑动窗口 + HashSet — O(n) / O(min(n,128))
// 逐步收缩: 左指针一个个右移, 从 Set 中删除
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_set<char> window;
        int ans = 0, left = 0;
        for (int right = 0; right < (int)s.size(); right++) {
            // 如果 s[right] 已在窗口中, 收缩左边界
            while (window.count(s[right])) {
                window.erase(s[left]);
                left++;
            }
            window.insert(s[right]);
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};
```

**关键点**：
- 逐步收缩：while 循环可能执行多次，但总体每个字符最多被加入和删除各一次，O(n)。
- 代码逻辑更直观，适合初学者理解。
- 缺点：HashSet 的常数开销比数组大。

---

### 解法3: 滑动窗口 + HashMap (跳跃式)

```cpp
// 解法3: 滑动窗口 + HashMap — O(n) / O(min(n,128))
// HashMap 记录字符最后出现的位置, 跳跃式更新 left
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_map<char, int> lastPos;
        int ans = 0, left = 0;
        for (int right = 0; right < (int)s.size(); right++) {
            if (lastPos.count(s[right]) && lastPos[s[right]] >= left) {
                left = lastPos[s[right]] + 1;
            }
            ans = max(ans, right - left + 1);
            lastPos[s[right]] = right;
        }
        return ans;
    }
};
```

**关键点**：
- 和解法1 逻辑相同，但用 HashMap 而非数组。
- `lastPos[s[right]] >= left` 判断是否在窗口内，等价于 `max(left, ...)`。
- Python 中常用 dict，效果相同。

---

## 解法对比

| 维度 | 解法1 数组跳跃 | 解法2 HashSet 收缩 | 解法3 HashMap 跳跃 |
|------|---------------|-------------------|-------------------|
| 时间复杂度 | O(n) | O(n) | O(n) |
| 空间复杂度 | O(128) 固定 | O(min(n,128)) | O(min(n,128)) |
| left 更新 | 直接跳跃 | 逐步收缩 | 直接跳跃 |
| 常数因子 | 最小 | 较大（hash） | 中等 |
| 面试推荐 | 首选 | 通用 | Python 常用 |
| 适用字符集 | 固定小字符集 | 任意字符集 | 任意字符集 |

---

## 易错点

1. **left 回退导致窗口含重复**
   ```cpp
   // 错误: 没有 max, left 可能回退
   left = last[s[right]] + 1;
   // 反例: s="abba", right=3('a'), last['a']=0, left 回退到 1
   // 窗口变成 [b,b,a] 含重复!
   // 正确:
   left = max(left, last[s[right]] + 1);
   ```

2. **last 数组大小不够**
   ```cpp
   // 错误: 只开 26 个字母
   vector<int> last(26, -1);
   // 题目包含数字、符号、空格, ASCII 0-127
   // 正确:
   vector<int> last(128, -1);
   ```

3. **忘记处理空字符串**
   ```cpp
   // s="" 时循环不执行, ans=0, 正确
   // 但如果用了 s[0] 做特殊处理, 需要先判空
   ```

4. **HashSet 解法忘记在收缩时删除 left 处的字符**
   ```cpp
   // 错误: 只移了 left, 没从 Set 删
   while (window.count(s[right])) { left++; }
   // 正确: 同时删
   while (window.count(s[right])) { window.erase(s[left++]); }
   ```

5. **窗口长度计算差一**
   ```cpp
   // 错误: right - left (少算了 1)
   // 正确: right - left + 1 (闭区间 [left, right])
   ```

---

## 面试追问

1. **"如果字符集是 Unicode（不止 ASCII）怎么办？"**
   - 不能用固定大小数组。改用 `unordered_map<char, int>` 或 `unordered_map<wchar_t, int>`。
   - 解法2 和解法3 天然支持。

2. **"最多包含 K 个不同字符的最长子串？"**
   - 滑动窗口 + HashMap 计数。窗口内不同字符数超过 K 时收缩左边界。
   - 即 LC 340 / LC 159。

3. **"如果要返回最长子串本身而非长度？"**
   - 记录 `ans` 更新时的 `left` 位置，最后 `s.substr(bestLeft, ans)`。

4. **"时间复杂度真的是 O(n) 吗？解法2 的 while 循环呢？"**
   - 是 O(n)。每个字符最多被 left 和 right 各访问一次，总操作 <= 2n。
   - 这是滑动窗口的经典摊还分析。

5. **"空间能做到 O(1) 吗？"**
   - 如果字符集固定（如 ASCII），数组大小是常数 128，已经是 O(1)。
   - 如果字符集不固定，空间至少 O(字符集大小)。

---

## 相关题型

| 题目 | 关联 | 复用技巧 |
|------|------|----------|
| [3. 无重复字符的最长子串](../0003_longest_substring_without_repeating_characters/) | 原题 | 完全相同 |
| [76. 最小覆盖子串](../0076_minimum_window_substring/) | 滑动窗口 | 窗口收缩条件不同 |
| [159. 至多包含两个不同字符的最长子串](../0159_longest_substring_with_at_most_two_distinct_characters/) | 窗口 + 计数 | K=2 的特例 |
| [340. 至多包含 K 个不同字符的最长子串](../0340_longest_substring_with_at_most_k_distinct_characters/) | 窗口 + 计数 | 本题的泛化 |
| [424. 替换后的最长重复字符](../0424_longest_repeating_character_replacement/) | 滑动窗口 | 窗口不变量不同 |
| [567. 字符串的排列](../0567_permutation_in_string/) | 窗口 + 频率 | 定长窗口变体 |
