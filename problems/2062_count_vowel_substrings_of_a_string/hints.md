# 2062. 统计字符串中的元音子字符串

## 核心思路

本题本质上是：在字符串中找出所有**仅由元音字母组成**且**包含全部5种元音**的连续子串的数量。两个约束条件缺一不可——"全是元音"限定了子串的边界，"五种都有"限定了子串的内容。

## 思维链

1. **读完题第一反应**：枚举所有子串，逐个检查是否满足两个条件（全是元音 + 包含全部5种）。数据量只有100，O(n³) 暴力完全能过。

2. **暴力解的瓶颈在哪？** 对每个子串，需要遍历一遍来检查条件，三重循环。虽然 n=100 能过，但面试中我们应该展示更好的思路。

3. **观察关键性质**：字符串天然被辅音字母分割成若干段"纯元音段"。比如 `"cuaieuouac"` 被 `c` 分割成 `["uaieuoua"]`。只有在这些纯元音段内部的子串才可能满足条件一。

4. **在纯元音段内怎么高效计数？** 这变成了经典的**滑动窗口**问题：在一个纯元音段内，找所有包含5种元音的子串。可以用"恰好K种 = 至多K种 - 至多(K-1)种"的转换技巧，配合滑动窗口在 O(n) 内完成。

5. **最终方案**：先按辅音分段，每段内用滑动窗口统计，总时间 O(n)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力三重循环 | 枚举所有子串，逐个检查 | O(n³) | O(1) | 能说出即可 |
| 暴力优化(双重循环+set) | 枚举起点，扩展终点时维护set | O(n²) | O(1) | 能说出即可 |
| 滑动窗口(atMost技巧) | "恰好5种 = atMost(5) - atMost(4)" | O(n) | O(1) | ⭐ 必须写出 |

## 关键提示

1. **先想条件一**：子串必须全是元音。辅音字母天然是"隔板"——任何包含辅音的子串一定不合法。所以你可以先把字符串按辅音切割成若干纯元音段，只在段内找答案。

2. **再想条件二**：在纯元音段内，需要找包含恰好5种元音的子串。"恰好K种"问题用**atMost(K) - atMost(K-1)** 转换是经典技巧。

3. **atMost(k) 滑动窗口**：维护窗口内不同元音的种类数。当种类数 > k 时收缩左端点。对于每个右端点 `r`，满足条件的子串数 = `r - l + 1`（以 r 结尾、起点在 [l, r] 范围内的所有子串）。

4. **别忘了在遇到辅音时重置窗口**：辅音字母意味着当前段结束，左右指针都要跳到辅音后面。

5. 示例 `"cuaieuouac"` 的纯元音段：
   ```
   c | u a i e u o u a | c
     ↑               ↑
     纯元音段: "uaieuoua"
   在这个段内找包含全部5种元音的子串 → 7个
   ```

## 解法详解

### 解法1: 暴力枚举 — O(n³) / O(1)

**思考过程**：最直觉的做法——枚举所有可能的子串 (i, j)，对每个子串检查两个条件。n ≤ 100，O(n³) ≈ 10⁶ 完全没问题。

```cpp
class Solution {
public:
    int countVowelSubstrings(string word) {
        int n = word.size(), count = 0;
        string vowels = "aeiou";
        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {
                // 检查 word[i..j] 是否全是元音且包含全部5种
                string sub = word.substr(i, j - i + 1);
                bool allVowel = true;
                for (char c : sub) {
                    if (vowels.find(c) == string::npos) {
                        allVowel = false;
                        break;
                    }
                }
                if (!allVowel) continue;
                
                set<char> st(sub.begin(), sub.end());
                if (st.size() == 5) count++;
            }
        }
        return count;
    }
};
```

**关键点**：一旦 `word[j]` 是辅音，`j` 继续增大也不会有新的合法子串（只要包含辅音就不合法），可以直接 break 优化内层循环。

### 解法2: 双重循环 + 增量维护 — O(n²) / O(1)

**从解法1优化**：解法1对每个子串重新扫描，浪费了"子串扩展时信息可以增量更新"的性质。固定起点 `i`，右移 `j` 时只需看新加入的字符 `word[j]`。

```cpp
class Solution {
public:
    int countVowelSubstrings(string word) {
        int n = word.size(), count = 0;
        set<char> vowelSet = {'a', 'e', 'i', 'o', 'u'};
        
        for (int i = 0; i < n; i++) {
            set<char> seen;
            for (int j = i; j < n; j++) {
                // 遇到辅音，以 i 为起点的所有更长子串都不合法
                if (vowelSet.find(word[j]) == vowelSet.end()) break;
                
                seen.insert(word[j]);
                if (seen.size() == 5) count++;
            }
        }
        return count;
    }
};
```

**关键点**：遇到辅音直接 `break`——这是因为子串必须连续，一旦碰到辅音，后面的子串一定包含这个辅音。

### 解法3: 滑动窗口(atMost技巧) — O(n) / O(1) ⭐ 面试首选

**从解法2优化**：解法2的瓶颈是 O(n²) 双重枚举。经典的"恰好K种"转换可以把问题变成两次滑动窗口：

> **包含恰好5种元音的子串数 = atMost(5) - atMost(4)**

其中 `atMost(k)` = "纯元音子串中不同元音种类 ≤ k 的子串数"。

**为什么这个等式成立？** atMost(5) 包含了种类数为 0,1,2,3,4,5 的所有子串。atMost(4) 包含种类数为 0,1,2,3,4 的。两者相减，剩下的就是恰好5种的。

`atMost(k)` 是标准滑动窗口：右指针扩展，当窗口内不同种类 > k 或遇到辅音时收缩/重置左指针。

```cpp
class Solution {
public:
    int countVowelSubstrings(string word) {
        // 恰好5种 = atMost(5) - atMost(4)
        return atMost(word, 5) - atMost(word, 4);
    }
    
private:
    bool isVowel(char c) {
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
    }
    
    // 统计：纯元音子串中，不同元音种类数 <= k 的子串个数
    int atMost(const string& word, int k) {
        int n = word.size(), count = 0, left = 0;
        unordered_map<char, int> freq;
        
        for (int right = 0; right < n; right++) {
            if (!isVowel(word[right])) {
                // 辅音是隔板，重置窗口
                freq.clear();
                left = right + 1;
                continue;
            }
            
            freq[word[right]]++;
            
            // 种类数超过 k，收缩左端点
            while ((int)freq.size() > k) {
                freq[word[left]]--;
                if (freq[word[left]] == 0) freq.erase(word[left]);
                left++;
            }
            
            // 以 right 结尾、起点在 [left, right] 范围的子串都满足
            count += right - left + 1;
        }
        return count;
    }
};
```

**关键点**：
- 遇到辅音时不是收缩窗口，而是**彻底重置**（清空 freq，left 跳到辅音后面）
- `count += right - left + 1` 是滑动窗口计数的核心：窗口 `[left, right]` 内，以 `right` 结尾的合法子串有 `right - left + 1` 个

## 解法对比

| | 解法1 暴力 | 解法2 双重循环 | 解法3 滑动窗口 |
|---|---|---|---|
| 时间 | O(n³) | O(n²) | O(n) |
| 空间 | O(n) substr | O(1) set最多5元素 | O(1) map最多5个key |
| 适用 | n ≤ 100 | n ≤ 10⁴ | 任意 n |
| 关键优化 | — | 遇辅音break | atMost转换 |

**什么时候选哪个**：n ≤ 100 的 Easy 题，解法2已经足够。但如果面试官追问"如果 n = 10⁵ 怎么办"，就需要解法3。

## 易错点

1. **✗ 遇到辅音时只移动 left 不清空 freq**：
   ```cpp
   // 错误：left 跳过辅音但 freq 还记着之前的元音
   if (!isVowel(word[right])) { left = right + 1; continue; }
   ```
   **✓ 必须同时清空 freq**：辅音是隔板，新段和旧段完全独立。

2. **✗ atMost 中忘记 (int) 强转 freq.size()**：
   ```cpp
   while (freq.size() > k)  // freq.size() 是 size_t(无符号)，k=0时比较有坑
   ```
   **✓ 写成 `(int)freq.size() > k`** 更安全。

3. **✗ 解法2中遇到辅音时用 continue 而不是 break**：
   ```cpp
   if (!isVowel(word[j])) continue;  // 错！跳过辅音继续看后面的，但子串已经断了
   ```
   **✓ 应该 break**：固定起点 i，子串是连续的，碰到辅音后面都不用看了。

## 面试追问

**Q1 (基础理解)**：暴力解的时间复杂度是多少？能优化到什么程度？
> O(n³) 或优化到 O(n²)。瓶颈在于枚举子串的双重循环。

**Q2 (优化)**：如果 n = 10⁵，O(n²) 过不了，怎么办？
> 用"恰好K种 = atMost(K) - atMost(K-1)"转换，配合滑动窗口做到 O(n)。

**Q3 (变体)**：如果题目改为"包含至少3种元音的纯元音子串数"呢？
> 同样的思路：atMost(5) - atMost(2)。atMost 函数不用改，只改外层调用的参数。

**Q4 (扩展)**：如果不要求"全是元音"，只要求"包含全部5种元音"的子串数？
> 去掉辅音隔板的逻辑，直接做"包含恰好5种元音"的滑动窗口。但此时辅音可以出现在子串中，atMost 的定义需要调整——只计算元音的种类数，辅音不影响种类计数。

## 相关题型

- **LeetCode 2062 → 2063 (Vowel of All Substrings)**：2062 要求全是元音+5种都有，2063 只统计所有子串中元音出现次数。思路完全不同，2063 是贡献法。

- **LeetCode 992 (K 个不同整数的子数组)**：直接复用 atMost(K) - atMost(K-1) 这个转换技巧，模板几乎一样。区别：992 没有"辅音隔板"的逻辑，但核心的 atMost 滑动窗口完全相同。

- **LeetCode 3 (无重复字符的最长子串)**：同样是滑动窗口+哈希表维护窗口内容，但目标不同（求最长 vs 求计数）。

- **LeetCode 76 (最小覆盖子串)**：也是"窗口内必须包含所有目标字符"，复用"用哈希表计数+满足条件时收缩"的模式。