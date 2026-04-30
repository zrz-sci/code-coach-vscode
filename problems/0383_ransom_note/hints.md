# 383. 赎金信

## 核心思路

本质上就是一个**字符频次匹配**问题：ransomNote 中每个字符出现的次数，是否都 ≤ magazine 中对应字符出现的次数？

## 思维链

1. **读完题第一反应**：ransomNote 的每个字符都要在 magazine 中找到一个"供应"，且每个 magazine 字符只能用一次 → 最朴素想法：对 ransomNote 的每个字符，去 magazine 里逐个找并标记已用。
2. **暴力解的瓶颈**：对 ransomNote 的每个字符都要在 magazine 中线性搜索，最坏 O(m×n)。瓶颈在于"查找某字符是否还有剩余"这个操作。
3. **怎么突破**：如果提前统计好 magazine 中每个字符有多少个，查找就变成 O(1) 了。用什么统计？哈希表或长度为 26 的数组。
4. **进一步简化**：只有 26 个小写字母，用 `int[26]` 比 `unordered_map` 更快更简洁。
5. **核心逻辑**：先统计 magazine 的字符频次，再遍历 ransomNote 逐个消耗，任何字符不够就返回 false。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力查找 | 逐字符在 magazine 中搜索并标记 | O(m×n) | O(n) | 能说出即可 |
| 哈希表计数 | unordered_map 统计频次再比较 | O(m+n) | O(1)* | 能说出即可 |
| 数组计数 | int[26] 统计频次再消耗 | O(m+n) | O(1) | ⭐ 必须写出 |

> *哈希表空间严格说是 O(字符集大小)=O(26)=O(1)，但常数比数组大。

## 关键提示

1. **只有 26 个小写字母** — 这意味着你不需要通用的哈希表，一个长度 26 的数组就够了，且更快。
2. **"每个字符只能用一次"** — 这说明需要的是频次匹配，不是简单的存在性判断。比如 ransomNote="aa"，magazine="ab"，虽然 'a' 存在于 magazine，但数量不够。
3. **谁减谁？** — 先统计 magazine（供应方），再用 ransomNote 去消耗（需求方）。消耗到负数就说明不够。
4. **能否只遍历一次？** — 不行，因为 magazine 的字符需要先全部统计完，才能确认够不够用（想想如果 ransomNote 需要的字符出现在 magazine 末尾的情况）。

## 解法详解

### 解法1: 暴力查找 — O(m×n) / O(n)

**思考过程**：最直觉的做法 — 把 magazine 变成一个可修改的"字符池"。对 ransomNote 的每个字符，在池子里找，找到就删掉（标记已用），找不到就返回 false。

```cpp
class Solution {
public:
    bool canConstruct(string ransomNote, string magazine) {
        // 对 ransomNote 的每个字符，去 magazine 里找
        for (char c : ransomNote) {
            // find 返回第一个匹配的位置，找不到返回 npos
            size_t pos = magazine.find(c);
            if (pos == string::npos) {
                return false;  // magazine 中没有这个字符了
            }
            // 找到了，把它"用掉"（删除该位置的字符）
            magazine.erase(pos, 1);
        }
        return true;
    }
};
```

**关键点**：
- `erase` 操作本身是 O(n)（要移动后面的字符），所以总体是 O(m×n)。
- 这种做法修改了 magazine 字符串，面试中可以提一下"是否允许修改输入"。

---

### 解法2: 哈希表计数 — O(m+n) / O(1)

**从解法1优化**：解法1的瓶颈是每次 `find` 都要 O(n) 搜索。如果我们提前把 magazine 的字符频次记下来，每次查询就是 O(1)。

```cpp
class Solution {
public:
    bool canConstruct(string ransomNote, string magazine) {
        unordered_map<char, int> freq;
        
        // 第一步：统计 magazine 中每个字符的出现次数
        for (char c : magazine) {
            freq[c]++;
        }
        
        // 第二步：消耗 ransomNote 需要的字符
        for (char c : ransomNote) {
            if (freq[c] <= 0) {
                return false;  // 供不应求
            }
            freq[c]--;  // 用掉一个
        }
        return true;
    }
};
```

**关键点**：`freq[c] <= 0` 而不是 `freq.find(c) == freq.end()`，因为字符可能存在但已经用完了（次数减为 0）。

---

### 解法3: 数组计数 — O(m+n) / O(1) ⭐ 面试首选

**进一步优化**：题目限定了只有小写字母，用 `int[26]` 替代 `unordered_map`，省去哈希计算的开销，代码更简洁。

```cpp
class Solution {
public:
    bool canConstruct(string ransomNote, string magazine) {
        int count[26] = {0};  // count[i] = 字符 'a'+i 在 magazine 中的剩余可用次数
        
        // 统计 magazine 的供应量
        for (char c : magazine) {
            count[c - 'a']++;
        }
        
        // 消耗 ransomNote 的需求量
        for (char c : ransomNote) {
            count[c - 'a']--;
            if (count[c - 'a'] < 0) {
                return false;  // 这个字符供不应求
            }
        }
        return true;
    }
};
```

**为什么 `c - 'a'` 有效**：小写字母 'a'-'z' 的 ASCII 码是连续的（97-122），减去 'a' 后映射到 0-25，正好做数组下标。

---

## 解法对比

|  | 暴力查找 | 哈希表计数 | 数组计数 |
|--|---------|-----------|---------|
| 时间 | O(m×n) | O(m+n) | O(m+n) |
| 空间 | O(n)（修改了magazine） | O(26) | O(26) |
| 实际速度 | 最慢 | 中等（哈希有常数开销） | 最快 |
| 适用场景 | 理解题意 | 字符集不确定时 | 字符集小且固定时 |

**面试建议**：直接写解法3（数组计数），然后主动提一句"如果字符集是 Unicode 而非仅小写字母，可以改用 unordered_map"，展示你考虑了扩展性。

## 易错点

1. **✗ 判断条件写成 `count[c-'a'] == 0` 就 return false**
   ```cpp
   // 错误：先减再判断时，应该判断 < 0
   count[c - 'a']--;
   if (count[c - 'a'] == 0) return false;  // ✗ 等于0说明刚好用完，是合法的！
   ```
   **✓ 正确**：`if (count[c - 'a'] < 0) return false;` — 小于 0 才是不够用。

2. **✗ 先减再判断 vs 先判断再减 的逻辑不同**
   ```cpp
   // 写法A：先减再判断 < 0
   count[c - 'a']--;
   if (count[c - 'a'] < 0) return false;
   
   // 写法B：先判断 <= 0 再减（等价）
   if (count[c - 'a'] <= 0) return false;
   count[c - 'a']--;
   ```
   两种都对，但判断的阈值不同（< 0 vs <= 0），别搞混。

3. **✗ 忘记初始化数组**
   ```cpp
   int count[26];  // ✗ 未初始化，值是垃圾
   int count[26] = {0};  // ✓ 全部初始化为 0
   ```

## 面试追问

**Q1（基础理解）**：为什么不能只检查"ransomNote 的每个字符是否存在于 magazine 中"？
> 因为题目说"每个字符只能用一次"，存在性不够，需要频次匹配。比如 ransomNote="aa", magazine="ab"，'a' 存在但只有 1 个，不够用。

**Q2（优化）**：如果 ransomNote 比 magazine 长，能否提前返回？
> 可以！如果 `ransomNote.length() > magazine.length()`，直接返回 false。因为即使 magazine 的每个字符都能用上，总量也不够。这是一个 O(1) 的快速剪枝。

**Q3（扩展）**：如果字符集不是 26 个小写字母，而是整个 Unicode，怎么改？
> 把 `int[26]` 换成 `unordered_map<char, int>`（或 `unordered_map<char32_t, int>`），逻辑完全一样，只是容器不同。

**Q4（变体）**：如果允许 magazine 的字符重复使用（无限供应），怎么改？
> 那就只需要检查 ransomNote 的每个字符是否**存在**于 magazine 中，不需要计数，用 `set` 即可。

## 相关题型

- **242. 有效的字母异位词 (Valid Anagram)** — 同样的数组计数框架，区别：242 要求两个字符串的频次**完全相同**，383 只要求 ransomNote 的频次 ≤ magazine 的频次。代码几乎一样，只是最终判断条件不同。
- **691. 贴纸拼词 (Stickers to Spell Word)** — 383 的升级版：不是一个 magazine 而是多个 sticker（每个 sticker 可以用多次），需要用回溯/DP 来选最少的 sticker 数量。
- **1160. 拼写单词 (Find Words That Can Be Formed by Characters)** — 反过来：给定 chars（相当于 magazine），判断 words 中哪些单词能被拼出。复用同样的频次计数技巧。