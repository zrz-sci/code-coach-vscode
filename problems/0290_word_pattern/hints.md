# 290. 单词规律 (Word Pattern)

## 核心思路

本质上在问：`pattern` 中的每个字符和 `s` 中的每个单词之间，是否存在**双射（bijection）**关系。即：每个字符恰好对应一个唯一的单词，每个单词也恰好对应一个唯一的字符，且这种对应在整个序列中保持一致。

关键词：**双向映射** -- 不仅要检查"字符 -> 单词"是否一致，还要检查"单词 -> 字符"是否一致。

## 思维链

1. **读完题第一反应**：把 `pattern` 中的字符和 `s` 中的单词一一配对，看配对是否自洽。这和 LeetCode 205 (Isomorphic Strings) 几乎一模一样，只是把"字符到字符"变成了"字符到单词"。

2. **什么叫"遵循相同规律"？** 就是 `pattern` 和 `s` 的**结构**相同。`pattern = "abba"` 的结构是"第1和第4相同，第2和第3相同，其余不同"。`s` 中的单词序列也必须有完全一样的结构。

3. **直觉做法**：用一个 HashMap 维护 `char -> string` 的映射。遍历时，如果字符已映射过，检查映射的单词是否和当前单词一致。如果没映射过，建立新映射。

4. **等等，这够吗？** 不够！只有单向映射会漏掉一种情况：
   ```
   pattern = "abba", s = "dog dog dog dog"
   ```
   单向映射：a->dog, b->dog，看起来每次都一致。但这违反了双射——两个不同字符映射到了同一个单词。
   
5. **双向映射的必要性**：必须同时维护 `char -> string` 和 `string -> char` 两个映射。或者等价地，用一个 HashSet 记录已被映射的单词，新建映射时检查单词是否已被占用。

6. **另一种思路：编码法**：不用双向映射，而是把 pattern 和单词序列分别"编码"成规范形式，然后比较编码是否相同。比如第一个出现的元素编码为 0，第二个新元素编码为 1...这样 "abba" 编码为 [0,1,1,0]，"dog cat cat dog" 也编码为 [0,1,1,0]。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 双向 HashMap | 维护 char<->word 双向映射 | O(n) | O(n) | ⭐ 必须写出 |
| HashMap + HashSet | 映射 + 已占用单词集合 | O(n) | O(n) | 等价变体 |
| 编码比较法 | 将两序列编码为规范形式比较 | O(n) | O(n) | 能说出即可 |

## 关键提示

1. **先分词再比较**：第一步就是把 `s` 按空格拆成单词数组。如果单词数量和 `pattern` 长度不一致，直接返回 `false`。这是最容易忘的边界条件！

2. **双射 = 单射 + 满射**：只检查 `char -> word` 是单射（不同 char 可能映射到同一 word），你还需要检查反方向。常见实现方式：
   - 两个 HashMap：`map<char, string>` + `map<string, char>`
   - 一个 HashMap + 一个 HashSet：映射 + 已占用单词集合
   
3. **C++ 分词技巧**：用 `istringstream` 可以轻松按空格分词：
   ```cpp
   istringstream iss(s);
   string word;
   while (iss >> word) { words.push_back(word); }
   ```

4. **与 205 题的关系**：LC 205 (Isomorphic Strings) 是 char-to-char 的双射检查，本题是 char-to-word 的双射检查。如果你会 205，本题几乎可以直接套用。

5. **陷阱用例**：
   - `pattern = "abc"`, `s = "dog cat dog"` -- false（a 和 c 映射到同一个 dog）
   - `pattern = "abba"`, `s = "dog dog dog dog"` -- false（a 和 b 映射到同一个 dog）
   - `pattern = "ab"`, `s = "dog"` -- false（长度不匹配）
   - `pattern = "a"`, `s = "dog"` -- true

## 解法详解

### 解法1: 双向 HashMap -- O(n) / O(n) ⭐ 面试首选

**思考过程**：最直观的做法。维护两个映射表：
- `charToWord`：字符到单词的映射
- `wordToChar`：单词到字符的映射

遍历时同时检查两个方向的映射是否一致。

```
模拟 pattern="abba", s="dog cat cat dog":

步骤1: pattern[0]='a', word="dog"
  charToWord 为空 -> 新建映射 a->"dog"
  wordToChar 为空 -> 新建映射 "dog"->a
  charToWord: {a:"dog"}
  wordToChar: {"dog":a}

步骤2: pattern[1]='b', word="cat"
  charToWord 无 'b' -> 新建映射 b->"cat"
  wordToChar 无 "cat" -> 新建映射 "cat"->b
  charToWord: {a:"dog", b:"cat"}
  wordToChar: {"dog":a, "cat":b}

步骤3: pattern[2]='b', word="cat"
  charToWord['b']="cat" == "cat" ✓
  wordToChar["cat"]='b' == 'b' ✓

步骤4: pattern[3]='a', word="dog"
  charToWord['a']="dog" == "dog" ✓
  wordToChar["dog"]='a' == 'a' ✓

全部通过 -> return true
```

```
反例模拟 pattern="abba", s="dog dog dog dog":

步骤1: pattern[0]='a', word="dog"
  新建映射: a->"dog", "dog"->a

步骤2: pattern[1]='b', word="dog"
  charToWord 无 'b' -> 准备新建映射
  但 wordToChar["dog"]='a' 已存在且 != 'b' -> return false!
  
  "dog" 已经被 'a' 占用了，'b' 不能再映射到 "dog"
```

```cpp
class Solution {
public:
    bool wordPattern(string pattern, string s) {
        // 第一步：分词
        vector<string> words;
        istringstream iss(s);
        string word;
        while (iss >> word) {
            words.push_back(word);
        }
        
        // 长度不匹配，直接返回 false
        if (pattern.size() != words.size()) return false;
        
        // 双向映射
        unordered_map<char, string> charToWord;
        unordered_map<string, char> wordToChar;
        
        for (int i = 0; i < pattern.size(); i++) {
            char c = pattern[i];
            string& w = words[i];
            
            // 检查 char -> word 方向
            if (charToWord.count(c)) {
                if (charToWord[c] != w) return false;
            } else {
                charToWord[c] = w;
            }
            
            // 检查 word -> char 方向
            if (wordToChar.count(w)) {
                if (wordToChar[w] != c) return false;
            } else {
                wordToChar[w] = c;
            }
        }
        
        return true;
    }
};
```

**复杂度分析**：
- 时间 O(n * L)：n 为 pattern 长度，L 为单词平均长度（HashMap 操作涉及字符串哈希）
- 空间 O(n * L)：两个 HashMap 最多各存 n 个条目

**关键点**：两个 HashMap 缺一不可。只有 `charToWord` 无法检测"两个不同字符映射到同一单词"的情况。

---

### 解法2: HashMap + HashSet -- O(n) / O(n)

**思考过程**：解法1 的变体。用一个 HashMap 维护 `char -> word` 映射，用一个 HashSet 记录已经被映射的单词。当新建映射时，如果单词已在 HashSet 中（说明被其他字符占用了），返回 false。

```cpp
class Solution {
public:
    bool wordPattern(string pattern, string s) {
        vector<string> words;
        istringstream iss(s);
        string word;
        while (iss >> word) words.push_back(word);
        
        if (pattern.size() != words.size()) return false;
        
        unordered_map<char, string> charToWord;
        unordered_set<string> usedWords;  // 已被占用的单词
        
        for (int i = 0; i < pattern.size(); i++) {
            char c = pattern[i];
            string& w = words[i];
            
            if (charToWord.count(c)) {
                // 字符已有映射，检查是否一致
                if (charToWord[c] != w) return false;
            } else {
                // 字符无映射，检查单词是否已被占用
                if (usedWords.count(w)) return false;
                charToWord[c] = w;
                usedWords.insert(w);
            }
        }
        return true;
    }
};
```

**与解法1的区别**：逻辑等价，但只用了一个 HashMap + 一个 HashSet。代码更简洁一些，思路也更直观 -- "如果这个单词已经被别的字符占了，就冲突了"。

---

### 解法3: 编码比较法（首次出现位置） -- O(n) / O(n)

**思考过程**：完全不同的角度 -- 不建立映射，而是把两个序列"规范化"为相同的编码格式，然后直接比较。

**核心思想**：对于序列中的每个元素，记录它**第一次出现的位置**。如果两个序列的首次出现位置序列相同，则结构相同。

```
pattern = "abba"
  a 首次在 0, b 首次在 1, b 首次在 1, a 首次在 0
  编码: [0, 1, 1, 0]

words = ["dog", "cat", "cat", "dog"]
  "dog" 首次在 0, "cat" 首次在 1, "cat" 首次在 1, "dog" 首次在 0
  编码: [0, 1, 1, 0]

编码相同 -> 结构相同 -> return true
```

```
反例: pattern = "abba", words = ["dog", "dog", "dog", "dog"]
  pattern 编码: [0, 1, 1, 0]
  words 编码:   [0, 0, 0, 0]
  编码不同 -> return false
```

```cpp
class Solution {
public:
    bool wordPattern(string pattern, string s) {
        vector<string> words;
        istringstream iss(s);
        string word;
        while (iss >> word) words.push_back(word);
        
        if (pattern.size() != words.size()) return false;
        
        int n = pattern.size();
        
        // 分别对 pattern 和 words 编码，然后比较
        unordered_map<char, int> charFirst;
        unordered_map<string, int> wordFirst;
        
        for (int i = 0; i < n; i++) {
            // 记录首次出现位置（用 i+1 避免与默认值 0 冲突）
            if (!charFirst.count(pattern[i])) charFirst[pattern[i]] = i + 1;
            if (!wordFirst.count(words[i])) wordFirst[words[i]] = i + 1;
            
            // 比较编码
            if (charFirst[pattern[i]] != wordFirst[words[i]]) return false;
        }
        return true;
    }
};
```

**为什么这也是正确的？** 首次出现位置编码本质上就是在检查"出现模式"是否一致。如果两个序列的出现模式相同，那么它们之间一定存在双射。这种方法优雅地避免了显式地维护双向映射。

---

## 面试追问

1. **"如果是子串对子串映射 (LC 291)?"** → 无法分词 + 哈希，需要回溯: 对 pattern 的每个字符，尝试将 s 中不同长度的子串与之匹配，不一致时回溯。
2. **"跟同构字符串 (LC 205) 有什么区别?"** → LC 205 是 char-to-char 的双射，本题是 char-to-word 的双射。核心逻辑完全相同，编码比较法对两题都适用。
3. **"如果 pattern 可以有通配符?"** → 变成正则匹配问题 (类似 LC 10)，需要 DP。
4. **"只做单向映射会怎样?"** → 漏掉"不同字符映射到同一单词"的情况，如 pattern="abba", s="dog dog dog dog" 会误判为 true。

## 举一反三

| 题目 | 关联 | 说明 |
|------|------|------|
| 205. 同构字符串 | 几乎相同 | char-to-char 双射，可直接套用 |
| 291. 单词规律 II | 进阶版 | pattern 到 s 的映射不再一一对应，需要回溯 |
| 890. 查找和替换模式 | 批量版 | 批量检查多个单词是否匹配同一 pattern |
| 49. 字母异位词分组 | 编码思想 | 也用编码/规范化来分组 |

## 关键决策树

```
题目要求检查两个序列的对应关系？
├── 是 -> 是否需要双射（一一对应）？
│   ├── 是（如本题）-> 双向 HashMap 或编码比较法
│   ├── 否（只要单向）-> 单向 HashMap 即可
│   └── 不确定 -> 仔细读题！"遵循相同规律" = 双射
└── 否 -> 其他类型题目
```

## 易错点总结

1. **忘记检查长度**：pattern 长度和单词数量不一致时要直接返回 false。这是最常见的边界遗漏。

2. **只做单向映射**：只维护 `char -> word` 映射，忘记检查反方向，导致"不同字符映射到同一单词"的情况漏掉。

3. **分词错误**：手写 split 时容易在末尾多出空单词（如果 s 以空格结尾，但题目保证不会）。用 `istringstream` 最安全。

4. **HashMap 的 count 和 []**：在 C++ 中，`map[key]` 会在 key 不存在时自动创建条目！所以判断是否存在要用 `count()` 或 `find()`，不要用 `[]`。

## 复杂度总结

| 操作 | 时间 | 说明 |
|------|------|------|
| 分词 | O(L) | L = s 的总长度 |
| 遍历匹配 | O(n * w) | n = pattern 长度, w = 平均单词长度 |
| 总时间 | O(L) | 因为 n * w 约等于 L |
| 空间 | O(L) | 存储单词 + HashMap |
