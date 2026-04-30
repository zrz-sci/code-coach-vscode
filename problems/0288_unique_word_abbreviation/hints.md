# 288. Unique Word Abbreviation (单词的唯一缩写)

**难度**: Medium | **标签**: Design, Array, Hash Table, String

## 核心思路

本质是**哈希映射 + 唯一性判定**。将每个单词转换为缩写形式（首字母+中间字母数+末字母），用 HashMap 记录每个缩写对应的原始单词集合。判断唯一性时，若缩写不存在于字典中或字典中所有同缩写的单词都与查询词相同，则返回 true。

## 思维链

1. 缩写规则：`dog -> d1g`，`internationalization -> i18n`，`it -> it`（<=2字符不缩写）
2. 预处理字典：建立 `缩写 -> 原词集合` 的映射
3. 查询 `isUnique(word)` 时：
   - 计算 word 的缩写
   - 如果该缩写不在字典中 -> true（没有冲突）
   - 如果在字典中，检查所有同缩写的词是否都等于 word -> 是则 true
4. 关键：用 `unordered_set` 存原词，因为字典可能有重复

```
字典: ["deer", "door", "cake", "card"]

缩写映射:
  "d2r" -> {"deer", "door"}  (两个不同的词)
  "c2e" -> {"cake"}
  "c2d" -> {"card"}

isUnique("dear"): abbr="d2r", 映射中有{"deer","door"}, "dear"不在其中 -> false
isUnique("cart"): abbr="c2t", 映射中不存在 -> true
isUnique("cane"): abbr="c2e", 映射中有{"cake"}, "cane"!="cake" -> false
isUnique("make"): abbr="m2e", 映射中不存在 -> true
isUnique("cake"): abbr="c2e", 映射中有{"cake"}, 只有"cake"且与word相同 -> true
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 解法1: HashMap+Set | abbr -> set<string> | O(1) 查询 | O(n) | ⭐⭐⭐ |
| 解法2: HashMap+单词 | abbr -> 唯一词或冲突标记 | O(1) 查询 | O(n) | ⭐⭐ |

## 关键提示

```
缩写规则:

单词长度 <= 2: 缩写 = 原词本身
  "it" -> "it"
  "a"  -> "a"

单词长度 > 2: 首字母 + 中间字符数 + 末字母
  "dog"  -> "d" + "1" + "g" = "d1g"
  "deer" -> "d" + "2" + "r" = "d2r"
  "internationalization" -> "i" + "18" + "n" = "i18n"

isUnique 判定逻辑:
  +-----------------+     +---+
  | abbr 不在字典中? | --> | YES | -> return true
  +-----------------+     +---+
         | NO
         v
  +---------------------------+     +---+
  | 字典中同缩写的词都=word? | --> | YES | -> return true
  +---------------------------+     +---+
         | NO
         v
     return false
```

## 解法详解

### 解法1: HashMap + Set (推荐)

```cpp
class ValidWordAbbr {
    unordered_map<string, unordered_set<string>> abbrMap;

    string getAbbr(const string& word) {
        int n = word.size();
        if (n <= 2) return word;
        return word[0] + to_string(n - 2) + word[n - 1];
    }

public:
    ValidWordAbbr(vector<string>& dictionary) {
        for (const string& w : dictionary) {
            abbrMap[getAbbr(w)].insert(w);
        }
    }

    bool isUnique(string word) {
        string abbr = getAbbr(word);
        auto it = abbrMap.find(abbr);
        if (it == abbrMap.end()) return true;
        // 同缩写的集合中只有 word 自身（或为空但不可能）
        return it->second.size() == 1 && it->second.count(word);
    }
};
```

### 解法2: HashMap + 单词/冲突标记

```cpp
class ValidWordAbbr {
    // value 为空字符串 "" 表示冲突（多个不同词有同一缩写）
    unordered_map<string, string> abbrMap;

    string getAbbr(const string& word) {
        int n = word.size();
        if (n <= 2) return word;
        return word[0] + to_string(n - 2) + word[n - 1];
    }

public:
    ValidWordAbbr(vector<string>& dictionary) {
        for (const string& w : dictionary) {
            string abbr = getAbbr(w);
            if (abbrMap.find(abbr) == abbrMap.end()) {
                abbrMap[abbr] = w;
            } else if (abbrMap[abbr] != w) {
                abbrMap[abbr] = "";  // 冲突标记
            }
        }
    }

    bool isUnique(string word) {
        string abbr = getAbbr(word);
        auto it = abbrMap.find(abbr);
        if (it == abbrMap.end()) return true;
        return it->second == word;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|----------|----------|------|
| ✗ 只检查缩写是否存在 | ✓ 还要检查同缩写词是否都等于 word | 字典中的 word 本身不算冲突 |
| ✗ 不处理字典重复词 | ✓ 用 set 去重 | ["deer","deer"] 中 "deer" 应该是 unique |
| ✗ 长度<=2 时也缩写 | ✓ 长度<=2 直接返回原词 | "it" 的缩写就是 "it" |
| ✗ `word[0] + to_string(...)` 类型问题 | ✓ `string(1,word[0]) + ...` 或直接拼接 | char + string 可能有类型问题 |

## 面试追问

**Q1: 如果字典非常大(百万级)，如何优化？**
-> 解法2（单词/冲突标记法）更省内存，因为不需要为每个缩写存整个 set。冲突的缩写直接标记为无效。

**Q2: 如果 isUnique 调用频率远高于构造怎么办？**
-> 预处理越充分越好。两种解法的查询都是 O(1)，已经最优。

**Q3: 缩写可能冲突的概率分析？**
-> 取决于首尾字母和长度的分布。常见英语单词中，同长度且首尾相同的词较多（如 dear/deer/door 等），缩写冲突率较高。

## 相关题型

- [170. Two Sum III - Data structure design](https://leetcode.com/problems/two-sum-iii-data-structure-design/) - 设计类 + 哈希
- [244. Shortest Word Distance II](https://leetcode.com/problems/shortest-word-distance-ii/) - 预处理 + 查询设计
- [359. Logger Rate Limiter](https://leetcode.com/problems/logger-rate-limiter/) - 设计 + 哈希映射
