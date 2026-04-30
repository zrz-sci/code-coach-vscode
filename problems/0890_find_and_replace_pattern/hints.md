# 890. 查找和替换模式

## 核心思路

本质上是一个**双射映射（Bijection）检测问题**：判断 word 和 pattern 之间是否存在字符的一一对应关系。不仅 pattern 中的每个字符要映射到 word 中唯一的字符，反过来 word 中的每个字符也要映射到 pattern 中唯一的字符。这是一个**同构字符串（Isomorphic Strings）**的判定问题。

## 思维链

1. **读完题第一反应**：pattern = "abb"，word = "mee" → a->m, b->e。word = "ccc" 不行，因为 a->c 且 b->c，两个不同的 pattern 字符映射到同一个 word 字符，不是双射。

2. **建模为双射**：需要两个映射——pattern→word（正向）和 word→pattern（反向）。只有当两个方向都不冲突时，才是有效的匹配。

3. **简化方法**：也可以将字符串"规范化"——把每个字符串转换成一种标准形式（用首次出现的顺序编号），然后比较两个标准形式是否相同。例如 "abb" → "011"，"mee" → "011"，相同所以匹配。

4. **遍历所有 words**：对每个 word 检查是否和 pattern 匹配，收集匹配的 words。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 双哈希映射 | 正向+反向映射检查双射 | O(N*K) | O(K) | ⭐ 面试首选 |
| 规范化编码 | 转换为标准形式后比较 | O(N*K) | O(K) | ⭐ 面试首选 |
| 单映射 + 反向检查 | 用一个映射 + 值集合 | O(N*K) | O(K) | 可选方案 |

> N = words.length, K = pattern.length（= word.length）

## 关键提示

1. **必须是双射**：不仅要检查 pattern→word 的映射一致性，还要检查 word→pattern 的反向映射。只检查单向会遗漏 "ccc" vs "abb" 这种情况（c 映射到 a 和 b 两个不同字符，但正向映射 a→c 不冲突）。

2. **规范化方法更简洁**：把字符串转换为"首次出现的顺序编号"，例如 "abb" → [0,1,1]，"xyz" → [0,1,2]，"aaa" → [0,0,0]。两个字符串匹配当且仅当它们的规范化形式相同。

3. **只需检查小写字母**：题目保证只有小写英文字母，映射表大小最多 26。

4. **word 和 pattern 等长**：题目保证 `words[i].length == pattern.length`，不需要判断长度。

5. **同构字符串（LC 205）的变体**：LC 205 判断两个字符串是否同构，本题是在多个 word 中筛选和 pattern 同构的。

## 解法详解

### 解法1: 双哈希映射 — O(N*K) / O(K) ⭐ 面试首选

**思考过程**：对每个 word，建立 pattern→word 和 word→pattern 两个映射。逐字符检查，如果出现冲突则不匹配。

```cpp
class Solution {
public:
    vector<string> findAndReplacePattern(vector<string>& words, string pattern) {
        vector<string> result;
        for (const string& word : words) {
            if (matches(word, pattern)) {
                result.push_back(word);
            }
        }
        return result;
    }

private:
    bool matches(const string& word, const string& pattern) {
        unordered_map<char, char> p2w, w2p;
        
        for (int i = 0; i < (int)pattern.size(); i++) {
            char pc = pattern[i], wc = word[i];
            
            // 检查正向映射: pattern -> word
            if (p2w.count(pc) && p2w[pc] != wc) return false;
            // 检查反向映射: word -> pattern
            if (w2p.count(wc) && w2p[wc] != pc) return false;
            
            p2w[pc] = wc;
            w2p[wc] = pc;
        }
        return true;
    }
};
```

**关键点**：两个映射缺一不可。只有正向映射无法检测"多对一"的情况。

### 解法2: 规范化编码 — O(N*K) / O(K) ⭐ 面试首选

**完全不同的思路**：不检查映射，而是把字符串转换为一种"标准形式"——用首次出现的序号替换字符。两个字符串匹配当且仅当标准形式相同。

```cpp
class Solution {
public:
    vector<string> findAndReplacePattern(vector<string>& words, string pattern) {
        vector<string> result;
        vector<int> patternCode = encode(pattern);
        
        for (const string& word : words) {
            if (encode(word) == patternCode) {
                result.push_back(word);
            }
        }
        return result;
    }

private:
    vector<int> encode(const string& s) {
        unordered_map<char, int> charToId;
        vector<int> code;
        int nextId = 0;
        
        for (char c : s) {
            if (!charToId.count(c)) {
                charToId[c] = nextId++;
            }
            code.push_back(charToId[c]);
        }
        return code;
    }
};
```

**关键点**：`encode("abb") = [0,1,1]`，`encode("mee") = [0,1,1]`，相同所以匹配。`encode("ccc") = [0,0,0]`，不等于 `[0,1,1]`，不匹配。规范化天然保证了双射关系。

### 解法3: 单映射 + 值集合检查 — O(N*K) / O(K)

**折中方案**：只维护一个 pattern→word 的映射，但额外维护一个"已被映射的 word 字符集合"。如果新的 pattern 字符要映射到一个已被占用的 word 字符 → 冲突。

```cpp
class Solution {
public:
    vector<string> findAndReplacePattern(vector<string>& words, string pattern) {
        vector<string> result;
        for (const string& word : words) {
            if (matches(word, pattern)) {
                result.push_back(word);
            }
        }
        return result;
    }

private:
    bool matches(const string& word, const string& pattern) {
        unordered_map<char, char> mapping;
        unordered_set<char> usedValues;
        
        for (int i = 0; i < (int)pattern.size(); i++) {
            char pc = pattern[i], wc = word[i];
            
            if (mapping.count(pc)) {
                // pattern 字符已有映射，必须一致
                if (mapping[pc] != wc) return false;
            } else {
                // pattern 字符首次出现
                if (usedValues.count(wc)) return false; // wc 已被其他 pc 占用
                mapping[pc] = wc;
                usedValues.insert(wc);
            }
        }
        return true;
    }
};
```

**关键点**：`usedValues` 保证了映射的单射性——不同的 pattern 字符不会映射到相同的 word 字符。

## 解法对比

| | 双哈希映射 | 规范化编码 | 单映射+集合 |
|--|----------|----------|-----------|
| 时间 | O(NK) | O(NK) | O(NK) |
| 空间 | O(K) | O(K) | O(K) |
| 思路难度 | 低 | 低 | 低 |
| 代码难度 | 低 | 低 | 低 |
| 面试建议 | ⭐首选 | ⭐首选 | 可选 |

**选择建议**：
- 双哈希映射最直观：直接表达"双射"的含义，面试沟通最清晰
- 规范化编码最优雅：代码更简洁，复用性强（同一个 encode 函数）
- 单映射+集合是折中方案，比双映射省一个 map 但多一个 set

## 易错点

1. **只检查单向映射**
   - 错：只维护 pattern→word → "ccc" 和 "abb" 会被判为匹配（c→a, c→b 冲突被发现了，但 a→c, b→c 不冲突）
   - 实际上这个例子正向也冲突了。真正漏检的是 pattern="ab", word="cc"：正向 a→c, b→c 无冲突（因为先设 a→c 再设 b→c 时 b 是新字符），但反向 c 同时映射到 a 和 b → 不是双射
   - 对：必须双向映射或规范化

2. **规范化编码用字符而非数字**
   - 错：用 'a','b','c'... 做编号 → 和原字符冲突
   - 对：用 int 编号（0, 1, 2, ...）避免混淆

3. **忘记 pattern 也要规范化**
   - 错：只规范化 word 然后和原始 pattern 比较 → 永远不匹配
   - 对：word 和 pattern 都要规范化，然后比较规范化结果

4. **映射表没有在每个 word 之间重置**
   - 错：所有 word 共用同一个映射表 → 前面的 word 影响后面的判断
   - 对：每个 word 单独建立映射

## 面试追问

**Q1: 这道题和 LC 205（同构字符串）是什么关系？**
→ 完全相同的核心问题。LC 205 判断两个字符串是否同构（双射映射），本题是在多个 word 中筛选和 pattern 同构的。解法可以直接复用。

**Q2: 规范化方法为什么正确？**
→ 规范化将字符替换为首次出现的序号，保留了"字符间的相等/不等关系"。两个字符串同构 ⟺ 它们的"相等/不等关系模式"相同 ⟺ 规范化编码相同。

**Q3: 如果 pattern 可以包含通配符呢？**
→ 如果通配符匹配任意单个字符，可以在映射检查中跳过通配符位置。如果通配符匹配任意长度，问题变成类似 LC 44 的通配符匹配。

**Q4: 时间复杂度能否优于 O(NK)？**
→ 不能。必须检查每个 word 的每个字符，下界就是 O(NK)。

## 相关题型

- **LeetCode 205 同构字符串** — 完全相同的核心判定逻辑（双射检测）。本题是 205 的"批量筛选"版本。
- **LeetCode 290 单词规律** — 字符级别的同构判定扩展到单词级别：pattern 的字符对应字符串的单词。
- **LeetCode 49 字母异位词分组** — 也用到了"规范化"的思路（排序后的字符串作为 key），但检测的是置换而非双射。
- **LeetCode 242 有效的字母异位词** — 字符频率比较，比双射简单但属于同一类"字符映射关系"问题。
