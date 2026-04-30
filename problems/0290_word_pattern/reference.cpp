/*
 * LeetCode 290: 单词规律 (Word Pattern)
 *
 * 【题目本质】
 * 判断 pattern 中的字符序列和 s 中的单词序列之间是否存在双射（bijection）。
 * 每个字符恰好对应一个唯一单词，每个单词也恰好对应一个唯一字符。
 *
 * 【解法总览】
 * 解法1: 双向 HashMap      — O(n) / O(n) — char<->word 双向映射检查 ⭐面试首选
 * 解法2: HashMap + HashSet  — O(n) / O(n) — 映射 + 已占用单词集合
 * 解法3: 编码比较法         — O(n) / O(n) — 首次出现位置编码，比较结构
 */



// ============================================================
// 解法1: 双向 HashMap — char<->word 双向映射
// 时间: O(n * L)  空间: O(n * L)  (n=pattern长度, L=平均单词长度)
//
// 【思路】
// 维护两个映射表：charToWord (char -> string) 和 wordToChar (string -> char)。
// 遍历 pattern 和 words 的每一对，检查两个方向的映射是否一致。
//
// 【为什么需要双向？】
// 只检查 char->word 会漏掉 "不同字符映射到同一单词" 的情况：
//   pattern="abba", s="dog dog dog dog"
//   单向检查：a->"dog" ✓, b->"dog" ✓ (新建), b->"dog" ✓, a->"dog" ✓
//   但 a 和 b 映射到了同一个 "dog"，违反双射！
//
// 模拟 pattern="abba", s="dog cat cat dog":
//   i=0: 'a'->"dog", "dog"->'a'  (新建双向映射)
//   i=1: 'b'->"cat", "cat"->'b'  (新建双向映射)
//   i=2: charToWord['b']="cat"=="cat" ✓, wordToChar["cat"]='b'=='b' ✓
//   i=3: charToWord['a']="dog"=="dog" ✓, wordToChar["dog"]='a'=='a' ✓
//   → return true
//
// 模拟 pattern="abba", s="dog dog dog dog":
//   i=0: 'a'->"dog", "dog"->'a'  (新建双向映射)
//   i=1: charToWord 无 'b' → 准备新建
//        但 wordToChar["dog"]='a' != 'b' → return false!
// ============================================================
class Solution1 {
public:
    bool wordPattern(string pattern, string s) {
        // 第一步：按空格分词
        vector<string> words;
        istringstream iss(s);
        string word;
        while (iss >> word) {
            words.push_back(word);
        }

        // 边界：长度不匹配直接返回 false
        // 这是最容易忘记的检查！
        if (pattern.size() != words.size()) return false;

        // 双向映射
        unordered_map<char, string> charToWord;
        unordered_map<string, char> wordToChar;

        for (int i = 0; i < (int)pattern.size(); i++) {
            char c = pattern[i];
            const string& w = words[i];

            // 检查 char -> word 方向
            if (charToWord.count(c)) {
                // 字符已有映射，检查是否一致
                if (charToWord[c] != w) return false;
            } else {
                charToWord[c] = w;
            }

            // 检查 word -> char 方向
            if (wordToChar.count(w)) {
                // 单词已有映射，检查是否一致
                if (wordToChar[w] != c) return false;
            } else {
                wordToChar[w] = c;
            }
        }

        return true;
    }
};

// ============================================================
// 解法2: HashMap + HashSet — 映射 + 已占用单词集合
// 时间: O(n * L)  空间: O(n * L)
//
// 【思路】
// 解法1的变体。用一个 HashMap 维护 char -> word 映射，
// 用一个 HashSet 记录"已被某个字符占用"的单词。
// 新建映射时，如果单词已在 HashSet 中，说明被其他字符占了 → 冲突。
//
// 与解法1的等价性：
//   解法1：wordToChar 检查 "这个单词是否已被映射到其他字符"
//   解法2：usedWords 检查 "这个单词是否已被任何字符占用"
//   逻辑完全等价，但不需要存储反向映射的具体值。
//
// 模拟 pattern="abba", s="dog dog dog dog":
//   i=0: charToWord 无 'a' → usedWords 无 "dog" → 映射 a->"dog", 标记 "dog"
//   i=1: charToWord 无 'b' → usedWords 有 "dog" → return false!
// ============================================================
class Solution2 {
public:
    bool wordPattern(string pattern, string s) {
        vector<string> words;
        istringstream iss(s);
        string word;
        while (iss >> word) words.push_back(word);

        if (pattern.size() != words.size()) return false;

        unordered_map<char, string> charToWord;
        unordered_set<string> usedWords;  // 已被某个字符占用的单词

        for (int i = 0; i < (int)pattern.size(); i++) {
            char c = pattern[i];
            const string& w = words[i];

            if (charToWord.count(c)) {
                // 字符已有映射，检查是否一致
                if (charToWord[c] != w) return false;
            } else {
                // 字符无映射 → 检查单词是否已被其他字符占用
                if (usedWords.count(w)) return false;
                charToWord[c] = w;
                usedWords.insert(w);
            }
        }
        return true;
    }
};

// ============================================================
// 解法3: 编码比较法 — 首次出现位置编码
// 时间: O(n * L)  空间: O(n * L)
//
// 【思路】
// 完全不同的角度：不建立映射，而是把两个序列"规范化"为
// 首次出现位置的编码序列，然后直接比较。
//
// 对于序列中的每个元素，记录它第一次出现的位置索引。
// 如果两个序列的编码序列完全相同，则它们有相同的"结构"，
// 即存在双射关系。
//
// 示例:
//   pattern = "abba"
//     a→位置0, b→位置1, b→位置1, a→位置0 → 编码 [0,1,1,0]
//   words = ["dog","cat","cat","dog"]
//     "dog"→位置0, "cat"→位置1, "cat"→位置1, "dog"→位置0 → 编码 [0,1,1,0]
//   编码相同 → return true
//
// 反例:
//   pattern = "abba" → 编码 [0,1,1,0]
//   words = ["dog","dog","dog","dog"] → 编码 [0,0,0,0]
//   编码不同 → return false
//
// 【为什么正确？】
// 首次出现位置编码捕获了序列的"出现模式"。
// 两个序列出现模式相同 ⟺ 存在双射。
// 这种方法优雅地避免了显式维护双向映射。
// ============================================================
class Solution3 {
public:
    bool wordPattern(string pattern, string s) {
        vector<string> words;
        istringstream iss(s);
        string word;
        while (iss >> word) words.push_back(word);

        if (pattern.size() != words.size()) return false;

        int n = pattern.size();

        // 分别记录每个元素首次出现的位置
        unordered_map<char, int> charFirst;
        unordered_map<string, int> wordFirst;

        for (int i = 0; i < n; i++) {
            // 用 i+1 作为位置标记，避免与 map 默认值 0 冲突
            if (!charFirst.count(pattern[i])) {
                charFirst[pattern[i]] = i + 1;
            }
            if (!wordFirst.count(words[i])) {
                wordFirst[words[i]] = i + 1;
            }

            // 比较两个编码值是否相同
            if (charFirst[pattern[i]] != wordFirst[words[i]]) {
                return false;
            }
        }
        return true;
    }
};

// ============================================================
// 测试代码
// ============================================================
int main() {
    Solution1 sol1;
    Solution2 sol2;
    Solution3 sol3;

    // 测试用例
    struct TestCase {
        string pattern;
        string s;
        bool expected;
    };

    vector<TestCase> tests = {
        {"abba", "dog cat cat dog", true},    // 标准正例
        {"abba", "dog cat cat fish", false},   // a 的映射不一致
        {"aaaa", "dog cat cat dog", false},     // a 不能同时映射 dog 和 cat
        {"abba", "dog dog dog dog", false},     // a,b 映射到同一单词
        {"abc", "dog cat dog", false},          // a,c 映射到同一单词 dog
        {"a", "dog", true},                     // 单元素
        {"ab", "dog", false},                   // 长度不匹配
    };

    cout << "=== 测试解法1: 双向 HashMap ===" << endl;
    for (int i = 0; i < (int)tests.size(); i++) {
        bool result = sol1.wordPattern(tests[i].pattern, tests[i].s);
        cout << "Test " << i + 1 << ": "
             << (result == tests[i].expected ? "PASS" : "FAIL")
             << " (pattern=\"" << tests[i].pattern
             << "\", s=\"" << tests[i].s
             << "\", expected=" << tests[i].expected
             << ", got=" << result << ")" << endl;
    }

    cout << "\n=== 测试解法2: HashMap + HashSet ===" << endl;
    for (int i = 0; i < (int)tests.size(); i++) {
        bool result = sol2.wordPattern(tests[i].pattern, tests[i].s);
        cout << "Test " << i + 1 << ": "
             << (result == tests[i].expected ? "PASS" : "FAIL") << endl;
    }

    cout << "\n=== 测试解法3: 编码比较法 ===" << endl;
    for (int i = 0; i < (int)tests.size(); i++) {
        bool result = sol3.wordPattern(tests[i].pattern, tests[i].s);
        cout << "Test " << i + 1 << ": "
             << (result == tests[i].expected ? "PASS" : "FAIL") << endl;
    }

    return 0;
}

// ============================================================
// 【解法对比】
//
// | 解法          | 时间      | 空间      | 代码复杂度 | 面试推荐 |
// |--------------|-----------|-----------|-----------|---------|
// | 双向 HashMap  | O(n*L)   | O(n*L)    | 直观       | ⭐⭐⭐  |
// | HashMap+Set  | O(n*L)   | O(n*L)    | 稍简       | ⭐⭐    |
// | 编码比较法    | O(n*L)   | O(n*L)    | 巧妙       | ✓ 加分  |
//
// 三种方法复杂度完全相同，区别在于思维模型:
//   解法1: 显式维护双射的两个方向
//   解法2: 正向映射 + 值域占用集合（等价于反向映射的存在性检查）
//   解法3: 不建映射，用首次出现位置编码比较结构同构
//
// 面试策略:
//   1. 先明确「双射」概念 → char<->word 一一对应
//   2. 写双向 HashMap（最直观，不容易遗漏）
//   3. 面试官追问其他写法 → 提编码比较法（展现思维广度）
// ============================================================

// ============================================================
// 【易错点】
//
// 1. 忘记检查长度匹配:
//    ✗ 直接进入循环 → pattern 和 words 长度不同时越界
//    ✓ if (pattern.size() != words.size()) return false;
//
// 2. 只做单向映射检查:
//    ✗ 只检查 charToWord → "abba" / "dog dog dog dog" 会误判为 true
//    ✓ 必须同时检查 wordToChar（或用 usedWords 集合）
//
// 3. 分词遗漏多余空格:
//    ✗ 手动 split 没处理首尾/连续空格
//    ✓ 用 istringstream >> word 自动跳过所有空白字符
//
// 4. 编码比较法中首次位置用 0:
//    ✗ charFirst 默认值也是 0 → 未出现和位置 0 混淆
//    ✓ 用 i+1 作为位置标记，或用 count() 先判断是否存在
// ============================================================

// ============================================================
// 【面试追问】
//
// Q1: 如果 pattern 和 s 的映射不是单个字符对单词，
//     而是子串对子串（LC291 Word Pattern II），怎么做？
// A1: 无法用分词 + 哈希，需要回溯:
//     对 pattern 的每个字符，尝试将 s 中不同长度的子串与之匹配。
//     用两个 HashMap 维护双向映射，不一致时回溯。
//     时间指数级，但 s 长度通常较小（<= 20）。
//
// Q2: 如果要判断的是「同构字符串」（LC205 Isomorphism），
//     和本题有什么区别？
// A2: LC205 是 char<->char 的双射，本题是 char<->word 的双射。
//     核心逻辑完全相同，只是数据类型不同。
//     编码比较法对两题都适用且最统一。
//
// Q3: 如果 pattern 可以包含 '.'（匹配任意单词）和 '*'（重复），
//     怎么处理？
// A3: 这就变成了正则匹配问题（类似 LC10），需要 DP:
//     dp[i][j] = pattern[0..i-1] 是否能匹配 words[0..j-1]。
//     '.' 匹配任意单词，'*' 让前一个字符可重复 0 到多次。
//     时间 O(m*n)，是完全不同的问题类型。
// ============================================================
