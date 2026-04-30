# 567. 字符串的排列

## 核心思路

**排列的本质是字符频次相同**。s1 的某个排列是 s2 的子串，等价于 s2 中存在一个长度等于 s1 的连续子串，使得该子串的字符频次与 s1 完全一致。这就把"排列匹配"问题转化成了"固定长度滑动窗口内的频次比较"问题。

## 思维链

1. **读完题第一反应**：生成 s1 的所有排列，逐一检查是否是 s2 的子串？——排列数是 n! 级别，完全不可行。
2. **换个角度理解"排列"**：两个字符串互为排列 ⟺ 它们的字符频次完全相同。所以问题变成：s2 中是否存在一个长度为 `len(s1)` 的子串，其字符频次和 s1 一样？
3. **暴力检查每个子串**：枚举 s2 中每个长度为 `len(s1)` 的子串，统计频次并比较。每次重新统计是 O(n)，总共 O(m×n)。瓶颈在于每次重新统计。
4. **滑动窗口优化**：窗口右移一格，只需加入新字符、移除旧字符，O(1) 更新频次。总体 O(m)。
5. **进一步优化比较操作**：用一个 `diff` 计数器记录"有多少个字符的频次还不匹配"，避免每次比较 26 个位置。当 `diff == 0` 时说明完全匹配。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力排列枚举 | 生成所有排列逐一匹配 | O(n! × m) | O(n!) | 不可行，仅说明 |
| 滑窗 + 频次数组比较 | 固定窗口，每次比较26个字符 | O(26 × m) | O(1) | 能说出即可 |
| 滑窗 + diff 计数器 | 固定窗口，O(1)判断匹配 | O(m) | O(1) | ⭐ 必须写出 |

> n = len(s1)，m = len(s2)

## 关键提示

1. **排列 = 频次相同**：不要真的去生成排列，两个字符串互为排列当且仅当每个字符出现的次数一样。
2. **窗口大小固定**：子串长度必须等于 s1 的长度，所以这是一个**固定大小**的滑动窗口，比可变窗口更简单。
3. **增量更新**：窗口滑动时，只有一个字符进入、一个字符离开，不需要重新统计整个窗口。
4. **diff 技巧**：维护一个整数 `diff` 表示"频次不匹配的字符种数"。加入/移除字符时，观察该字符的差值是否从非零变为零（diff--）或从零变为非零（diff++）。
5. **ASCII 示意**：
```
s1 = "ab",  s2 = "eidbaooo"
窗口大小 = 2，在 s2 上滑动：

  e i d b a o o o
  [e i]             频次: e1,i1  vs a1,b1 → 不匹配
    [i d]           频次: i1,d1  → 不匹配
      [d b]         频次: d1,b1  → 不匹配
        [b a]       频次: b1,a1  vs a1,b1 → ✓ 匹配! 返回true
```

## 解法详解

### 解法1: 滑动窗口 + 频次数组比较 — O(26×m) / O(1)

**思考过程**：既然"排列 = 频次相同"，我们先统计 s1 的频次，然后用一个固定大小为 `len(s1)` 的窗口在 s2 上滑动，每次比较窗口内的频次数组和 s1 的频次数组是否完全相等。

```cpp
class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        int n = s1.size(), m = s2.size();
        if (n > m) return false;
        
        vector<int> cnt1(26, 0), cnt2(26, 0);
        // 统计 s1 的频次，以及 s2 前 n 个字符的频次
        for (int i = 0; i < n; i++) {
            cnt1[s1[i] - 'a']++;
            cnt2[s2[i] - 'a']++;
        }
        if (cnt1 == cnt2) return true;
        
        // 窗口右移：加入 s2[i]，移除 s2[i-n]
        for (int i = n; i < m; i++) {
            cnt2[s2[i] - 'a']++;       // 新字符进入窗口
            cnt2[s2[i - n] - 'a']--;   // 旧字符离开窗口
            if (cnt1 == cnt2) return true; // 比较 26 个位置
        }
        return false;
    }
};
```

**关键点**：`cnt1 == cnt2` 在 C++ 中对 vector 逐元素比较，每次 O(26)。虽然常数很小，但可以进一步优化。

---

### 解法2: 滑动窗口 + diff 计数器 — O(m) / O(1) ⭐ 面试首选

**从解法1优化**：解法1每次滑动后要比较 26 个位置，瓶颈在比较。我们用一个 `diff` 变量记录"有多少种字符的频次不匹配"，每次滑动只需 O(1) 更新 diff。

**核心思想**：维护一个差值数组 `cnt[c] = s1中c的次数 - 窗口中c的次数`。当 `cnt[c] == 0` 时字符 c 匹配。`diff` 记录有多少个 c 使得 `cnt[c] != 0`。

```cpp
class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        int n = s1.size(), m = s2.size();
        if (n > m) return false;
        
        vector<int> cnt(26, 0);
        // cnt[c] = s1中c的频次 - 窗口中c的频次
        // cnt[c] == 0 表示字符c完全匹配
        for (int i = 0; i < n; i++) {
            cnt[s1[i] - 'a']++;    // s1 贡献 +1
            cnt[s2[i] - 'a']--;    // 窗口贡献 -1
        }
        
        // 统计初始有多少种字符不匹配
        int diff = 0;
        for (int i = 0; i < 26; i++) {
            if (cnt[i] != 0) diff++;
        }
        if (diff == 0) return true;
        
        for (int i = n; i < m; i++) {
            // 新字符 s2[i] 进入窗口 → cnt 对应位 -1
            int in_char = s2[i] - 'a';
            if (cnt[in_char] == 0) diff++;      // 从匹配变成不匹配
            cnt[in_char]--;
            if (cnt[in_char] == 0) diff--;      // 变化后恰好匹配
            
            // 旧字符 s2[i-n] 离开窗口 → cnt 对应位 +1
            int out_char = s2[i - n] - 'a';
            if (cnt[out_char] == 0) diff++;     // 从匹配变成不匹配
            cnt[out_char]++;
            if (cnt[out_char] == 0) diff--;     // 变化后恰好匹配
            
            if (diff == 0) return true;
        }
        return false;
    }
};
```

**diff 更新的四种情况图解**：
```
对于进入窗口的字符 c (cnt[c]--)：
  更新前 cnt[c]==0 → 原本匹配，现在要变了 → diff++
  更新后 cnt[c]==0 → 变化后又匹配了 → diff--

对于离开窗口的字符 c (cnt[c]++)：
  更新前 cnt[c]==0 → 原本匹配，现在要变了 → diff++  
  更新后 cnt[c]==0 → 变化后又匹配了 → diff--
```

---

### 解法3（变体）: 滑动窗口 + match 计数 — O(m) / O(1)

**另一种 O(1) 判断思路**：用 `match` 记录"有多少种字符的频次已经匹配"。当 `match == 26` 时返回 true。逻辑和 diff 是对偶的。

```cpp
class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        int n = s1.size(), m = s2.size();
        if (n > m) return false;
        
        vector<int> cnt1(26, 0), cnt2(26, 0);
        for (int i = 0; i < n; i++) {
            cnt1[s1[i] - 'a']++;
            cnt2[s2[i] - 'a']++;
        }
        
        int match = 0;
        for (int i = 0; i < 26; i++) {
            if (cnt1[i] == cnt2[i]) match++;
        }
        if (match == 26) return true;
        
        for (int i = n; i < m; i++) {
            int in_c = s2[i] - 'a';
            int out_c = s2[i - n] - 'a';
            
            // 加入新字符
            cnt2[in_c]++;
            if (cnt2[in_c] == cnt1[in_c]) match++;       // 变匹配了
            else if (cnt2[in_c] == cnt1[in_c] + 1) match--; // 刚超过，不匹配了
            
            // 移除旧字符
            cnt2[out_c]--;
            if (cnt2[out_c] == cnt1[out_c]) match++;
            else if (cnt2[out_c] == cnt1[out_c] - 1) match--;
            
            if (match == 26) return true;
        }
        return false;
    }
};
```

## 解法对比

| | 解法1: 频次比较 | 解法2: diff 计数 | 解法3: match 计数 |
|---|---|---|---|
| 时间 | O(26m) | O(m) | O(m) |
| 空间 | O(1) | O(1) | O(1) |
| 比较方式 | 每次比较26个位置 | 维护不匹配种数 | 维护匹配种数 |
| 代码复杂度 | 最简单 | 中等 | 中等 |
| 面试推荐 | 能过，但不够优 | ⭐ 首选 | 同样优秀 |

**实际面试建议**：先写解法1确保正确，然后口述"可以用 diff 计数器优化到真正的 O(m)"。如果面试官追问，再写解法2。

## 易错点

1. **忘记判断 `n > m`**：
   - ✗ 直接开始滑窗，当 s1 比 s2 长时数组越界
   - ✓ 开头 `if (n > m) return false;`

2. **diff 更新顺序搞反**：
   - ✗ 先修改 cnt，再判断修改前是否为 0 → 逻辑混乱
   - ✓ **先判断修改前**是否为 0（diff++），**再修改** cnt，**再判断修改后**是否为 0（diff--）

3. **窗口左边界算错**：
   - ✗ `s2[i - n + 1]` 离开窗口 → 窗口大小变成 n+1
   - ✓ `s2[i - n]` 离开窗口。窗口是 `[i-n+1, i]`，所以移除的是 `i-n`

4. **match 计数初始化漏掉频次为 0 的字符**：
   - 两个频次数组中 `cnt1[c] == 0 && cnt2[c] == 0` 的字符也算匹配，初始化时 match 应该包含它们。这就是为什么循环 26 个字符全部比较。

## 面试追问

**Q1: 暴力法怎么做？为什么不行？**
> 生成 s1 所有排列(n! 个)，逐一在 s2 中查找。时间 O(n!×m)，当 n=10 时 n!=3628800，完全不可行。但由此引出核心洞察：排列 = 频次相同，不需要真的生成排列。

**Q2: 如果不用 diff 计数器，每次比较 26 个位置，面试官会满意吗？**
> 一般会满意，因为 26 是常数。但严格来说时间是 O(26m) vs O(m)，如果面试官追问"能不能真正 O(m)"，就需要 diff/match 技巧。在实际面试中，先写出能过的版本更重要。

**Q3: 如果字符集不是 26 个小写字母，而是任意 Unicode 字符呢？**
> 用 `unordered_map` 替代长度 26 的数组。diff 计数器的思路完全不变，只是底层数据结构换了。此时 diff 的优势更明显——不用遍历整个 map 来判断是否匹配。

**Q4: 如果要返回所有匹配位置（而不只是 true/false），怎么改？**
> 这就是 [438. 找到字符串中所有字母异位词](https://leetcode.com/problems/find-all-anagrams-in-a-string/)。代码几乎一样，把 `return true` 改成 `result.push_back(i - n + 1)` 即可。

## 相关题型

- **[438. 找到字符串中所有字母异位词](https://leetcode.com/problems/find-all-anagrams-in-a-string/)** — 本题的升级版，从"判断是否存在"变成"找出所有位置"。直接复用本题的滑窗+diff框架，把 `return true` 改成记录起始索引。
- **[76. 最小覆盖子串](https://leetcode.com/problems/minimum-window-substring/)** — 从"固定窗口"升级为"可变窗口"。窗口不再是固定大小，需要右扩左缩。但频次比较的思路完全复用。
- **[242. 有效的字母异位词](https://leetcode.com/problems/valid-anagram/)** — 本题的简化版：直接比较两个字符串的频次是否相同，不涉及滑窗。
- **[3. 无重复字符的最长子串](https://leetcode.com/problems/longest-substring-without-repeating-characters/)** — 经典可变滑动窗口题，可以对比固定 vs 可变窗口的模板差异。