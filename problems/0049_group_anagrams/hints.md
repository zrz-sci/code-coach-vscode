# 49. 字母异位词分组

## 核心思路

本质上这道题在问：**如何快速判断两个字符串是否是字母异位词（即包含完全相同的字母和频次），并把相同组的归到一起？** 关键在于找到一种方法，为每个字符串生成一个"标准化的身份标识"，使得所有异位词共享同一个标识。

## 思维链

1. **读完题第一反应**：对于每个字符串，我需要和其他所有字符串比较，看它们是不是异位词。最朴素的做法就是两两比较。
2. **暴力解的瓶颈**：两两比较需要 O(n²) 对，每对比较还需要 O(k) 时间（k 是字符串长度），总共 O(n²·k)，太慢了。
3. **关键观察**：两个异位词如果排好序，结果一定一样！比如 "eat" 和 "tea" 排序后都是 "aet"。所以排序后的字符串可以作为"身份标识"。
4. **自然引出哈希表**：有了身份标识，我只需要一个哈希表 `map<标识, 字符串列表>`，遍历一遍数组，每个字符串算出标识，扔进对应的桶里。
5. **还能更快吗？** 排序每个字符串需要 O(k·log k)。如果用字符计数作为标识（26个字母各出现几次），可以 O(k) 生成标识，避免排序。
6. **权衡**：排序法代码简单，计数法理论更快。面试中排序法是首选（简洁不易错），被追问再给计数法。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力两两比较 | 每对字符串比较字符频次 | O(n²·k) | O(n·k) | 能说出即可 |
| 排序作为 key | 对每个字符串排序，排序结果作为哈希表的 key | O(n·k·log k) | O(n·k) | ⭐ 必须写出 |
| 字符计数作为 key | 统计每个字符串的字母频次，编码为字符串作 key | O(n·k) | O(n·k) | 加分项 |

> 其中 n = strs.length，k = 字符串最大长度

## 关键提示

1. **异位词的本质是什么？** —— 字母种类和频次完全相同，只是顺序不同。那什么操作能"消除顺序差异"？
2. **分组问题的标准套路**：为每个元素计算一个"分组 key"，然后用哈希表 `key → 列表` 自动分组。
3. **排序 vs 计数**：排序是 O(k·log k)，计数是 O(k)。当 k 很小（≤100）时差距不大；当 k 很大时计数法优势明显。
4. **计数法的 key 怎么编码？** 不能直接用 `vector<int>` 作 map 的 key（可以用 `map`，但更常见的是编码成字符串，如 `"1#0#0#...#0#1#0#..."`）。
5. **边界**：空字符串 `""` 也是合法输入，排序后还是 `""`，计数后是全零——都能正确处理。

## 解法详解

### 解法1: 排序作为 Key — O(n·k·log k) / O(n·k) ⭐ 面试首选

**思考过程**：异位词的共同特征是什么？字母相同、顺序不同。如果我把每个字符串的字母排序，异位词就会变成完全一样的字符串。有了这个"标准化形式"，用哈希表就能自动分组。

```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> groups;
        
        for (const string& s : strs) {
            string key = s;
            sort(key.begin(), key.end());  // "eat" → "aet", "tea" → "aet"
            groups[key].push_back(s);      // 相同 key 的归到一组
        }
        
        vector<vector<string>> result;
        for (auto& [key, group] : groups) {
            result.push_back(move(group));
        }
        return result;
    }
};
```

**过程演示**：
```
输入: ["eat", "tea", "tan", "ate", "nat", "bat"]

排序生成 key:
  "eat" → sort → "aet"
  "tea" → sort → "aet"    ← 和 "eat" 同 key!
  "tan" → sort → "ant"
  "ate" → sort → "aet"    ← 和 "eat", "tea" 同 key!
  "nat" → sort → "ant"    ← 和 "tan" 同 key!
  "bat" → sort → "abt"

哈希表:
  "aet" → ["eat", "tea", "ate"]
  "ant" → ["tan", "nat"]
  "abt" → ["bat"]
```

**关键点**：
- `sort(key.begin(), key.end())` 是对副本排序，不要修改原始字符串。
- `unordered_map` 比 `map` 快：前者 O(1) 查找，后者 O(log n)。

---

### 解法2: 字符计数作为 Key — O(n·k) / O(n·k)

**从解法1优化**：解法1的瓶颈在于每个字符串都要排序 O(k·log k)。能不能 O(k) 就生成 key？可以！统计每个字母出现次数，将计数结果编码为字符串。

```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> groups;
        
        for (const string& s : strs) {
            // O(k) 统计字符频次
            int count[26] = {0};
            for (char c : s) {
                count[c - 'a']++;
            }
            
            // 将频次编码为字符串作为 key
            // 例如 "eat" → "1#0#0#0#1#0#...#1#0#..."
            //        a=1           e=1         t=1
            string key;
            for (int i = 0; i < 26; i++) {
                key += to_string(count[i]);
                key += '#';  // 分隔符防止歧义: "1#12" vs "11#2"
            }
            
            groups[key].push_back(s);
        }
        
        vector<vector<string>> result;
        for (auto& [key, group] : groups) {
            result.push_back(move(group));
        }
        return result;
    }
};
```

**为什么需要分隔符 `#`？**
```
没有分隔符时:
  字母a出现1次, b出现12次 → "112..."
  字母a出现11次, b出现2次 → "112..."  ← 冲突!

加分隔符:
  "1#12#..."  vs  "11#2#..."  ← 不同!
```

**关键点**：
- 这种编码方式使得 key 长度固定为 O(26)（常数），与字符串内容无关。
- 生成 key 的时间是 O(k)（遍历字符串）+ O(26)（拼接），总共 O(k)。

---

### 解法0（了解即可）: 暴力两两比较 — O(n²·k) / O(n·k)

**思考过程**：最直觉的做法——对每个字符串，和已有的每一组的"代表"比较，看是否是异位词。

```cpp
class Solution {
public:
    // 判断两个字符串是否是异位词 — O(k)
    bool isAnagram(const string& a, const string& b) {
        if (a.size() != b.size()) return false;
        int count[26] = {0};
        for (int i = 0; i < a.size(); i++) {
            count[a[i] - 'a']++;
            count[b[i] - 'a']--;
        }
        for (int i = 0; i < 26; i++) {
            if (count[i] != 0) return false;
        }
        return true;
    }
    
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> result;
        vector<bool> used(strs.size(), false);
        
        for (int i = 0; i < strs.size(); i++) {
            if (used[i]) continue;
            vector<string> group = {strs[i]};
            for (int j = i + 1; j < strs.size(); j++) {
                if (!used[j] && isAnagram(strs[i], strs[j])) {
                    group.push_back(strs[j]);
                    used[j] = true;
                }
            }
            result.push_back(group);
        }
        return result;
    }
};
```

**为什么不推荐**：O(n²·k) 在 n=10⁴ 时约 10⁸ 次操作，会 TLE。但面试中能说出这个思路并分析瓶颈，展示你理解了问题。

## 解法对比

|  | 排序法 | 计数法 | 暴力法 |
|--|--------|--------|--------|
| 时间 | O(n·k·log k) | O(n·k) | O(n²·k) |
| 空间 | O(n·k) | O(n·k) | O(n·k) |
| key 生成 | 排序字符串 | 编码计数数组 | 不需要 key |
| 代码量 | 最少 | 稍多（编码逻辑） | 中等 |
| 面试推荐 | ⭐ 首选 | 追问时给出 | 分析用 |

**什么时候选哪个？**
- **面试默认写排序法**：代码最简洁，不容易出错，面试官能快速看懂。
- **被问"能否更快"时切换计数法**：省去排序开销，从 O(k·log k) 降到 O(k)。
- **k 很小（如本题 ≤100）时两者差距不大**，排序法甚至可能因常数更小而更快。

## 易错点

1. **排序法：修改了原字符串**
   - ✗ `sort(s.begin(), s.end()); groups[s].push_back(s);` — 原字符串被排序了！
   - ✓ `string key = s; sort(key.begin(), key.end()); groups[key].push_back(s);` — 对副本排序

2. **计数法：缺少分隔符导致 key 冲突**
   - ✗ `key += to_string(count[i]);` — "1" + "12" 和 "11" + "2" 都生成 "112"
   - ✓ `key += to_string(count[i]) + '#';` — "1#12#" vs "11#2#"

3. **忘记处理空字符串**
   - `""` 排序后仍是 `""`，计数后全零，两种方法都能正确处理。但如果手写比较函数，要确保 `isAnagram("", "")` 返回 true。

4. **用 `map` 而不是 `unordered_map`**
   - 不会出错，但 `map` 内部用红黑树，查找 O(log n)；`unordered_map` 用哈希表，查找均摊 O(1)。面试中推荐后者。

## 面试追问

**Q1: 暴力法能过吗？瓶颈在哪？**
> 暴力两两比较 O(n²·k)，n=10⁴ 时约 10⁸，大概率 TLE。瓶颈在于每个字符串都要和之前所有组比较——本质上"查找属于哪一组"这个操作是 O(n) 的。

**Q2: 排序法的时间能否进一步优化？**
> 可以。排序每个字符串 O(k·log k) 是瓶颈。因为字符集只有 26 个字母，可以用计数排序（其实就是计数法）将 key 生成降到 O(k)。总时间从 O(n·k·log k) 降到 O(n·k)。

**Q3: 如果字符集不是 26 个小写字母，而是 Unicode 全字符集呢？**
> 计数法中的固定长度 26 数组不再适用。可以改用 `map<char, int>` 计数，然后序列化为字符串。或者回退到排序法——排序对任何字符集都适用，且代码更简洁。排序法在字符集很大时反而是更实用的选择。

**Q4: 如果要做海量字符串的流式分组（字符串一个个到来），怎么设计？**
> 核心不变：维护一个全局哈希表 `map<key, group>`。每来一个字符串，O(k·log k) 或 O(k) 计算 key，插入对应组。这就是在线算法，无需等所有数据到齐。

## 相关题型

- **242. 有效的字母异位词 (Valid Anagram)** — 本题的子问题：判断两个字符串是否是异位词。可以复用计数法的核心逻辑（26 位计数数组比较）。
- **438. 找到字符串中所有字母异位词 (Find All Anagrams in a String)** — 滑动窗口 + 字符计数。复用计数法思路，但在窗口滑动时增量更新计数数组，而不是每次重新计算。
- **249. 移位字符串分组 (Group Shifted Strings)** — 同样的"设计 key + 哈希表分组"框架，区别在于 key 的定义不同：这里 key 是相邻字符差值序列，而非排序/计数。