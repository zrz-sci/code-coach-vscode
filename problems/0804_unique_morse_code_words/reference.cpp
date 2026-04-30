/*
 * 【题目本质】
 * 将每个单词按字母映射为摩尔斯电码字符串，统计不同电码字符串的数量。
 * 核心操作：字符串映射 + 去重计数。
 *
 * 【解法总览】
 * Solution1: HashSet 去重（推荐）—— 逐字符映射拼接，插入 set 去重
 * Solution2: 排序去重 —— 收集所有电码后排序 + unique
 */

// ===================== Solution1: HashSet 去重（推荐） =====================
// 思路：对每个单词逐字符查表拼接摩尔斯电码，用 unordered_set 收集去重
// 时间 O(S)，S 为所有字符总数；空间 O(N*L)，N 为单词数，L 为平均电码长度
class Solution1 {
public:
    int uniqueMorseRepresentations(vector<string>& words) {
        // 硬编码 26 个字母的摩尔斯电码
        vector<string> morse = {
            ".-","-...","-.-.","-..",".","..-.","--.","....",
            "..",".---","-.-",".-..","--","-.","---",".--.",
            "--.-",".-.","...","-","..-","...-",".--","-..-",
            "-.--","--.."
        };

        unordered_set<string> seen;
        for (const string& word : words) {
            string code;
            code.reserve(word.size() * 4); // 预分配空间，摩尔斯电码最长4字符
            for (char c : word) {
                code += morse[c - 'a'];
            }
            seen.insert(move(code)); // move 避免拷贝
        }
        return seen.size();
    }
};

// ===================== Solution2: 排序去重 =====================
// 思路：先收集所有摩尔斯电码字符串，排序后用 unique 去重
// 时间 O(S + NlogN * L)，排序时字符串比较需 O(L)；空间 O(N*L)
class Solution2 {
public:
    int uniqueMorseRepresentations(vector<string>& words) {
        vector<string> morse = {
            ".-","-...","-.-.","-..",".","..-.","--.","....",
            "..",".---","-.-",".-..","--","-.","---",".--.",
            "--.-",".-.","...","-","..-","...-",".--","-..-",
            "-.--","--.."
        };

        vector<string> codes;
        codes.reserve(words.size());
        for (const string& word : words) {
            string code;
            for (char c : word) {
                code += morse[c - 'a'];
            }
            codes.push_back(move(code));
        }

        // 排序后去重
        sort(codes.begin(), codes.end());
        auto last = unique(codes.begin(), codes.end());
        return distance(codes.begin(), last);
    }
};

/*
 * 【解法对比】
 * ┌──────────┬──────────────┬──────────────┬─────────────────────────┐
 * │ 解法     │ 时间复杂度   │ 空间复杂度   │ 适用场景                │
 * ├──────────┼──────────────┼──────────────┼─────────────────────────┤
 * │ HashSet  │ O(S)         │ O(N*L)       │ 通用，期望 O(1) 插入    │
 * │ 排序去重 │ O(S+NlogN*L) │ O(N*L)       │ 需要有序输出时          │
 * └──────────┴──────────────┴──────────────┴─────────────────────────┘
 *
 * 【易错点】
 * 1. 字符索引映射：必须用 c - 'a' 而不是直接用 c 作为数组下标
 * 2. 每个单词的 code 必须在循环内重新初始化，否则会拼接多个单词的电码
 * 3. reserve 预分配可以减少 string 的动态扩容开销
 * 4. unordered_set 比 set 更高效（O(1) vs O(logN) 插入）
 *
 * 【面试追问】
 * Q1: 如果要处理大小写混合的输入？
 *     → 先统一转小写 tolower(c)，再做映射
 *
 * Q2: 如果输入包含非字母字符？
 *     → 增加过滤逻辑 if (isalpha(c))，跳过非字母字符
 *
 * Q3: 如何验证摩尔斯电码映射表的正确性？
 *     → 可以用已知的 "sos" = "...---..." 作为 sanity check
 *
 * Q4: 如果要返回出现次数最多的摩尔斯电码变换？
 *     → 改用 unordered_map<string, int> 统计频次，遍历取最大值
 */
