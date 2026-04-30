/*
 * LeetCode 890: Find and Replace Pattern (查找和替换模式)
 *
 * 【题目本质】
 * 双射映射（Bijection）检测：判断 word 和 pattern 之间
 * 是否存在字符的一一对应关系（同构字符串判定）。
 *
 * 【解法总览】
 * 解法1: 双哈希映射      — O(NK) / O(K)  — 面试首选
 * 解法2: 规范化编码      — O(NK) / O(K)  — 面试首选（更优雅）
 * 解法3: 单映射+值集合   — O(NK) / O(K)  — 可选方案
 */

// ============================================================
// 解法1: 双哈希映射 — 正向+反向映射检查双射（⭐ 面试首选）
// 时间: O(NK)  空间: O(K)
//
// 【思路】
// 对每个 word，建立两个映射:
//   p2w: pattern 字符 → word 字符（正向映射）
//   w2p: word 字符 → pattern 字符（反向映射）
//
// 逐字符检查:
//   如果 p2w[pc] 已存在且 != wc → 正向冲突 → 不匹配
//   如果 w2p[wc] 已存在且 != pc → 反向冲突 → 不匹配
//   否则建立/确认映射
//
// 为什么需要双向映射？
//   pattern="ab", word="cc":
//   正向: a→c (新), b→c (b是新字符，映射到c，p2w[b]不存在，无冲突!)
//   反向: c→a (新), c→b (c已映射到a，但现在要映射到b → 冲突!)
//   只有反向映射才能检测到"多对一"的问题。
//
// 示例: pattern="abb", word="mee"
//   i=0: p='a',w='m' → p2w[a]=m, w2p[m]=a ✓
//   i=1: p='b',w='e' → p2w[b]=e, w2p[e]=b ✓
//   i=2: p='b',w='e' → p2w[b]=e ✓, w2p[e]=b ✓
//   全部通过 → 匹配!
//
// 示例: pattern="abb", word="ccc"
//   i=0: p='a',w='c' → p2w[a]=c, w2p[c]=a ✓
//   i=1: p='b',w='c' → w2p[c]=a, 但现在 pc='b' ≠ 'a' → 反向冲突!
//   → 不匹配!
// ============================================================
class Solution1 {
public:
    vector<string> findAndReplacePattern(vector<string>& words, string pattern) {
        vector<string> result;

        for (const string& word : words) {
            if (isMatch(word, pattern)) {
                result.push_back(word);
            }
        }

        return result;
    }

private:
    bool isMatch(const string& word, const string& pattern) {
        // 正向映射: pattern → word
        unordered_map<char, char> p2w;
        // 反向映射: word → pattern
        unordered_map<char, char> w2p;

        for (int i = 0; i < (int)pattern.size(); i++) {
            char pc = pattern[i];
            char wc = word[i];

            // 检查正向映射一致性
            if (p2w.count(pc)) {
                if (p2w[pc] != wc) return false;
            }

            // 检查反向映射一致性
            if (w2p.count(wc)) {
                if (w2p[wc] != pc) return false;
            }

            // 建立/确认映射
            p2w[pc] = wc;
            w2p[wc] = pc;
        }

        return true;
    }
};

// ============================================================
// 解法2: 规范化编码 — 将字符串转换为标准形式后比较
// 时间: O(NK)  空间: O(K)
//
// 【思路】
// 将字符串中的字符替换为"首次出现的序号":
//   "abb" → [0, 1, 1]
//   "mee" → [0, 1, 1]  → 相同 → 匹配
//   "ccc" → [0, 0, 0]  → 不同 → 不匹配
//   "xyz" → [0, 1, 2]  → 不同 → 不匹配
//   "dkd" → [0, 1, 0]  → 不同 → 不匹配
//
// 为什么这样就保证了双射？
// 规范化保留了"字符间的相等/不等关系"。
// 两个字符串规范化相同 ⟺ 它们的相等/不等模式相同 ⟺ 存在双射映射。
//
// 优势: 不需要两个映射，一个 encode 函数搞定一切。
// ============================================================
class Solution2 {
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
    // 将字符串编码为"首次出现序号"列表
    // "abb" → [0, 1, 1]
    // "xyz" → [0, 1, 2]
    // "aaa" → [0, 0, 0]
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

// ============================================================
// 解法3: 单映射 + 值集合 — 折中方案
// 时间: O(NK)  空间: O(K)
//
// 【思路】
// 只维护一个 pattern→word 映射（正向），
// 另外用一个 set 记录"已经被映射到的 word 字符"。
//
// 检查规则:
//   如果 pc 已有映射:
//     mapping[pc] != wc → 冲突
//   如果 pc 没有映射:
//     wc 已在 usedValues 中 → 说明已有其他 pc' 映射到 wc → 不是双射
//     否则: 建立映射 mapping[pc]=wc, usedValues.insert(wc)
//
// 这种方式用 set 替代了反向映射 map。
// ============================================================
class Solution3 {
public:
    vector<string> findAndReplacePattern(vector<string>& words, string pattern) {
        vector<string> result;

        for (const string& word : words) {
            if (isMatch(word, pattern)) {
                result.push_back(word);
            }
        }

        return result;
    }

private:
    bool isMatch(const string& word, const string& pattern) {
        unordered_map<char, char> mapping;  // pattern → word
        unordered_set<char> usedValues;     // word 中已被占用的字符

        for (int i = 0; i < (int)pattern.size(); i++) {
            char pc = pattern[i];
            char wc = word[i];

            if (mapping.count(pc)) {
                // pattern 字符已有映射，必须一致
                if (mapping[pc] != wc) return false;
            } else {
                // pattern 字符首次出现
                // 检查 word 字符是否已被其他 pattern 字符占用
                if (usedValues.count(wc)) return false;

                mapping[pc] = wc;
                usedValues.insert(wc);
            }
        }

        return true;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间   | 空间  | 特点                  |
// |-------------|-------|------|----------------------|
// | 双哈希映射   | O(NK) | O(K) | ⭐面试首选，最直观     |
// | 规范化编码   | O(NK) | O(K) | ⭐最优雅，代码最简     |
// | 单映射+集合  | O(NK) | O(K) | 折中方案             |
//
// 面试建议:
//   先说清楚"双射"的概念 → 写双哈希映射解法
//   → 追问更简洁的方法 → 给规范化编码
//
// 【易错点】
// 1. 只检查单向映射:
//    ✗ 只有 p2w → pattern="ab", word="cc" 通过了
//       (a→c 新建, b→c 新建, p2w[a]=c 和 p2w[b]=c 不冲突)
//    ✓ 加 w2p 反向映射 → c→a 已存在, 但要映射到 b → 冲突
//
// 2. 规范化时用字符代替数字:
//    ✗ 用 'a','b','c'... → 和原始字符冲突，比较失败
//    ✓ 用 int 编号 (0,1,2,...)
//
// 3. 映射没有重置:
//    ✗ 对所有 word 共用一个映射 → word 之间互相污染
//    ✓ 每个 word 创建新的映射
//
// 4. 忘记 encode pattern:
//    ✗ 只 encode word，和原始 pattern 字符串比较
//    ✓ word 和 pattern 都要 encode
//
// 【面试追问 Interview Follow-ups】
// Q1: 和 LC 205 (Isomorphic Strings) 的关系？
// A1: 完全相同的核心判定。205 判断两个字符串同构，
//     本题在 words 中筛选和 pattern 同构的。
//
// Q2: 如果 pattern 可以包含 '.' 通配符？
// A2: '.' 匹配任意字符但不建立映射绑定。
//     在映射检查中跳过 '.' 位置即可。
//
// Q3: 规范化方法可以推广到哪些问题？
// A3: 所有涉及"结构等价"的问题：同构字符串、单词规律(LC 290)、
//     句子相似性等。规范化是一个通用的"消除具体值、保留关系"的技巧。
// ============================================================
