/*
 * 【题目本质】
 * 列举单词的全部广义缩写：每个字符可选择保留或用数字替换
 * 本质是 2^n 的枚举问题，每位二选一
 * 连续被替换的字符合并为一个数字，两个数字不能相邻
 *
 * 【解法总览】
 * Solution1: 回溯法 — 用 count 追踪连续缩写字符数
 * Solution2: 位操作枚举 — 用 bitmask 编码每位的选择
 */

// ===================== Solution1: 回溯法 =====================
// 思路：每个位置两种选择 - 缩写(count++)或保留(输出count+字母)
// 用 count 追踪连续被缩写的字符数
// 时间 O(n * 2^n), 空间 O(n) 递归栈
class Solution1 {
public:
    vector<string> generateAbbreviations(string word) {
        vector<string> result;
        string current;
        backtrack(word, 0, 0, current, result);
        return result;
    }

private:
    void backtrack(const string& word, int pos, int count,
                   string& current, vector<string>& result) {
        int origLen = current.size();

        if (pos == (int)word.size()) {
            // 终止：输出残余的 count
            if (count > 0) current += to_string(count);
            result.push_back(current);
        } else {
            // 分支1: 缩写当前字符 => count + 1
            backtrack(word, pos + 1, count + 1, current, result);

            // 分支2: 保留当前字符 => 先输出 count，再追加字母
            if (count > 0) current += to_string(count);
            current += word[pos];
            backtrack(word, pos + 1, 0, current, result);
        }

        // 回溯：恢复 current 长度
        current.resize(origLen);
    }
};

// ===================== Solution2: 位操作枚举 =====================
// 思路：用 0 ~ 2^n-1 的 bitmask，bit=1 表示缩写，bit=0 表示保留
// 扫描 bitmask 构建缩写字符串
// 时间 O(n * 2^n), 空间 O(n)
class Solution2 {
public:
    vector<string> generateAbbreviations(string word) {
        int n = word.size();
        vector<string> result;

        for (int mask = 0; mask < (1 << n); mask++) {
            string abbr;
            int count = 0;

            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {
                    count++;  // 该位被缩写
                } else {
                    if (count > 0) {
                        abbr += to_string(count);
                        count = 0;
                    }
                    abbr += word[i];  // 保留原字母
                }
            }

            if (count > 0) abbr += to_string(count);
            result.push_back(abbr);
        }

        return result;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | Solution1 回溯 | O(n*2^n) | O(n) | 直观，面试首选 |
 * | Solution2 位操作 | O(n*2^n) | O(n) | 代码简洁，无递归开销 |
 *
 * 【易错点】
 * 1. 末尾残余 count 必须输出，否则漏掉末尾数字
 * 2. 回溯时必须恢复 current 字符串（resize 或传值）
 * 3. 连续缩写字符应合并为一个数字，不是多个相邻数字
 * 4. 结果总数恒为 2^n（每个字符独立二选一）
 *
 * 【面试追问】
 * Q1: 如何验证某个缩写是否匹配单词？(LC 408)
 *     双指针：字母直接比较，数字跳过对应长度
 * Q2: 如何找最短唯一缩写？(LC 411)
 *     在所有 2^n 缩写中，找最短且不与字典中其他词冲突的
 * Q3: 时间复杂度能否优化到低于 O(n * 2^n)？
 *     不能 — 输出本身就有 2^n 个结果，每个平均长度 O(n)
 */
