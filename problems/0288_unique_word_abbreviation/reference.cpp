/*
 * 【题目本质】
 * 设计一个数据结构，预处理字典中单词的缩写映射，
 * 支持 O(1) 查询某单词的缩写是否在字典中"唯一"。
 * 核心：缩写 -> 原词映射 + 唯一性判定逻辑。
 *
 * 【解法总览】
 * Solution1: HashMap + Set — 缩写映射到原词集合，通用且清晰 ⭐推荐
 * Solution2: HashMap + 冲突标记 — 更省空间，冲突时标记为空串
 */

// ===================== Solution1: HashMap + Set =====================
// 思路：预处理字典，建立 缩写 -> set<原词> 映射
//       isUnique: 缩写不存在 -> true
//                 缩写存在但集合中只有 word 自身 -> true
//                 否则 -> false
// 时间：构造 O(n*L)，查询 O(L)，n=字典大小，L=平均词长
// 空间：O(n*L)
class ValidWordAbbr1 {
    unordered_map<string, unordered_set<string>> abbrMap;

    string getAbbr(const string& word) {
        int n = word.size();
        if (n <= 2) return word;
        return word.front() + to_string(n - 2) + word.back();
    }

public:
    ValidWordAbbr1(vector<string>& dictionary) {
        for (const string& w : dictionary) {
            abbrMap[getAbbr(w)].insert(w);
        }
    }

    bool isUnique(string word) {
        string abbr = getAbbr(word);
        auto it = abbrMap.find(abbr);
        // 缩写不存在于字典 -> 唯一
        if (it == abbrMap.end()) return true;
        // 同缩写的词集合中只有 word 自身 -> 唯一
        const auto& words = it->second;
        return words.size() == 1 && words.count(word);
    }
};

// ===================== Solution2: HashMap + 冲突标记 =====================
// 思路：对每个缩写，只记录第一个遇到的原词
//       如果再遇到不同词的同缩写，标记为 "" (冲突)
//       查询时：不存在 -> true; 值==word -> true; 否则 -> false
// 时间：构造 O(n*L)，查询 O(L)
// 空间：O(n*L)，但比 Solution1 更省（无 set 开销）
class ValidWordAbbr2 {
    unordered_map<string, string> abbrMap;

    string getAbbr(const string& word) {
        int n = word.size();
        if (n <= 2) return word;
        return word.front() + to_string(n - 2) + word.back();
    }

public:
    ValidWordAbbr2(vector<string>& dictionary) {
        for (const string& w : dictionary) {
            string abbr = getAbbr(w);
            if (abbrMap.find(abbr) == abbrMap.end()) {
                // 第一次见到这个缩写
                abbrMap[abbr] = w;
            } else if (abbrMap[abbr] != w) {
                // 不同词有同一缩写 -> 标记冲突
                abbrMap[abbr] = "";
            }
            // 相同词重复出现 -> 不做任何操作
        }
    }

    bool isUnique(string word) {
        string abbr = getAbbr(word);
        auto it = abbrMap.find(abbr);
        if (it == abbrMap.end()) return true;  // 缩写不存在
        return it->second == word;              // 唯一词且就是 word
    }
};

// ===================== 辅助: 缩写函数详解 =====================
// 缩写规则:
//   长度 <= 2: 返回原词 ("it" -> "it", "a" -> "a")
//   长度 > 2:  首字母 + 中间字符数 + 末字母
//              "dog" -> "d1g"
//              "deer" -> "d2r"
//              "internationalization" -> "i18n"
//
// 注意: word.front() 返回 char，to_string() 返回 string
//       char + string 在 C++ 中行为依赖编译器
//       安全写法: string(1, word.front()) + to_string(n-2) + string(1, word.back())

/*
 * 【解法对比】
 * | 解法 | 构造 | 查询 | 空间 | 特点 |
 * |------|------|------|------|------|
 * | HashMap+Set  | O(nL) | O(L) | O(nL) | 清晰，通用 |
 * | HashMap+标记 | O(nL) | O(L) | O(nL) | 更省内存，适合大字典 |
 *
 * 【易错点】
 * 1. "unique"的定义: 不是"缩写唯一"，而是"同缩写的词都是word本身"
 *    -> ["deer","deer"] 中 isUnique("deer") = true (重复不算冲突)
 * 2. 字典重复词: ["a","a"] 中 "a" 只出现1种词，应该 unique
 *    -> 用 set 自动去重，或冲突标记法中相同词不标记冲突
 * 3. 短词处理: 长度 <= 2 的词缩写就是自身
 *    -> "it" 的缩写不是 "i0t"，而是 "it"
 * 4. getAbbr 的 char+string 拼接: 注意 C++ 类型
 *
 * 【面试追问】
 * Q1: 大字典优化? -> 冲突标记法更省空间
 * Q2: 高频查询优化? -> 两种方案查询都已O(L)，瓶颈在hash
 * Q3: 如何最小化缩写冲突? -> 可以用更多字符(如前两个+后两个)
 */
