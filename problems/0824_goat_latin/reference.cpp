/*
 * 【题目本质】
 * 将句子中的每个单词按 Goat Latin 规则转换：
 * - 元音开头：加 "ma" + 若干 'a'
 * - 辅音开头：首字母移末尾 + "ma" + 若干 'a'
 * 第 k 个单词追加 k 个 'a'（1-indexed）。
 * 纯字符串模拟题，考察分词和条件处理。
 *
 * 【解法总览】
 * Solution1: istringstream 分词（推荐）—— C++ 惯用流式分词
 * Solution2: 手动遍历分词 —— 不依赖 sstream，手动切割
 */

// ===================== Solution1: istringstream 分词（推荐） =====================
// 思路：用 istringstream 按空格自动分词，逐词处理
// 时间 O(N + K^2)，N 为句子长度，K 为单词数（追加 'a' 的累计长度）
// 空间 O(N)
class Solution1 {
public:
    string toGoatLatin(string sentence) {
        // 元音集合（注意大小写）
        unordered_set<char> vowels = {'a','e','i','o','u','A','E','I','O','U'};

        istringstream iss(sentence);
        string word, result;
        int idx = 1;

        while (iss >> word) {
            // 单词间用空格分隔
            if (idx > 1) result += ' ';

            if (vowels.count(word[0])) {
                // 元音开头：word + "ma"
                result += word;
            } else {
                // 辅音开头：首字母移到末尾
                result += word.substr(1);
                result += word[0];
            }

            result += "ma";

            // 追加 idx 个 'a'
            result.append(idx, 'a');
            idx++;
        }

        return result;
    }
};

// ===================== Solution2: 手动遍历分词 =====================
// 思路：用双指针手动按空格切分单词
// 时间 O(N + K^2)，空间 O(N)
class Solution2 {
public:
    string toGoatLatin(string sentence) {
        auto isVowel = [](char c) -> bool {
            c = tolower(c);
            return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
        };

        string result;
        result.reserve(sentence.size() * 2); // 预分配减少扩容
        int idx = 1;
        int n = sentence.size();
        int i = 0;

        while (i < n) {
            // 找单词边界
            int j = i;
            while (j < n && sentence[j] != ' ') j++;

            if (idx > 1) result += ' ';

            if (isVowel(sentence[i])) {
                // 元音：直接追加整个单词
                result.append(sentence, i, j - i);
            } else {
                // 辅音：跳过首字母，追加剩余部分，再追加首字母
                result.append(sentence, i + 1, j - i - 1);
                result += sentence[i];
            }

            result += "ma";
            result.append(idx, 'a');

            idx++;
            i = j + 1;
        }

        return result;
    }
};

/*
 * 【解法对比】
 * ┌──────────────────┬──────────────┬──────────────┬─────────────────────────────┐
 * │ 解法             │ 时间复杂度   │ 空间复杂度   │ 适用场景                    │
 * ├──────────────────┼──────────────┼──────────────┼─────────────────────────────┤
 * │ istringstream    │ O(N + K^2)   │ O(N)         │ 代码简洁，面试首选          │
 * │ 手动遍历         │ O(N + K^2)   │ O(N)         │ 避免流的开销，性能略优      │
 * └──────────────────┴──────────────┴──────────────┴─────────────────────────────┘
 *
 * 【易错点】
 * 1. 大小写元音：必须检查 'A','E','I','O','U'，题目输入包含大写开头的单词。
 * 2. 'a' 追加从 1 开始：第一个单词追加 "a"（1个），不是 ""（0个）。
 * 3. 空格处理：结果的首个单词前不加空格，末尾不加空格。
 * 4. substr 的拷贝开销：在性能敏感场景下用 append(str, pos, len) 替代。
 * 5. char + string 拼接陷阱：`word[0] + "ma"` 不是字符串拼接，
 *    而是 char(整数) + 指针运算。应写 `string(1, word[0]) + "ma"` 或分步追加。
 *
 * 【面试追问】
 * Q1: istringstream 与手动分词的性能差异？
 *     → istringstream 有流状态管理的开销，但可读性更好。
 *       对于面试场景，优先选择 istringstream。
 *
 * Q2: 如何用 getline 配合自定义分隔符分词？
 *     → `getline(iss, word, ' ')` 可以按空格分词，但会保留空字符串。
 *       `iss >> word` 自动跳过空白字符，更适合本题。
 *
 * Q3: 如果要支持多种分隔符（如逗号、制表符）？
 *     → 用 string::find_first_of 找分隔符位置，或用 regex 分词。
 *
 * Q4: 如何原地修改而不创建新字符串？
 *     → 由于转换可能改变单词长度（辅音移位、追加 'a'），原地修改很困难。
 *       必须从后向前处理或预计算偏移量，不实用。
 */
