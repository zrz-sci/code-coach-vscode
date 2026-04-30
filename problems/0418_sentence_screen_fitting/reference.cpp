// 【题目本质】
// 在有限屏幕上循环放置句子，统计完整句子数。核心是避免逐行逐词模拟的高开销

// 【解法总览】
// Solution1: 字符串游标法 — O(rows) / O(len) — 面试首选
// Solution2: 记忆化 DP — O(n*cols/len + rows) / O(n) — 预处理优化

// ===================== Solution 1: 字符串游标法 =====================
// 将句子拼成循环字符串 "w1 w2 ... wn "，用全局游标 start 滑动
// 每行放 cols 个字符，根据落点位置决定是跳过空格还是回退
class Solution1 {
public:
    int wordsTyping(vector<string>& sentence, int rows, int cols) {
        string s;
        for (auto& w : sentence) s += w + " ";
        int len = s.size(), start = 0;

        for (int i = 0; i < rows; i++) {
            start += cols;  // 本行放 cols 个字符

            if (s[start % len] == ' ') {
                start++;    // 恰好落在空格上，跳过进入下一行
            } else {
                // 落在单词中间，需要回退到上一个空格后
                while (start > 0 && s[(start - 1) % len] != ' ') {
                    start--;
                }
            }
        }

        return start / len;  // 全局消耗字符数 / 句子长度 = 完整句子数
    }
};

// ===================== Solution 2: 记忆化 DP =====================
// 对每个起始单词位置，预计算一行结束后的状态
// dp[i] = {下一行起始单词索引, 本行完成的句子轮数}
class Solution2 {
public:
    int wordsTyping(vector<string>& sentence, int rows, int cols) {
        int n = sentence.size();
        vector<pair<int,int>> dp(n); // {next_word_index, sentences_completed}

        for (int i = 0; i < n; i++) {
            int cur = i, count = 0, remain = cols;
            while (remain >= (int)sentence[cur].size()) {
                remain -= sentence[cur].size(); // 放入单词
                remain--;                        // 扣掉空格
                cur++;
                if (cur == n) {
                    cur = 0;
                    count++;  // 完成一轮
                }
            }
            dp[i] = {cur, count};
        }

        int total = 0, cur = 0;
        for (int i = 0; i < rows; i++) {
            total += dp[cur].second;
            cur = dp[cur].first;
        }
        return total;
    }
};

// 【解法对比】
// Solution1: 代码最简洁，全局游标直觉强，O(rows) 时间
// Solution2: 预处理后查表，适合 rows 极大但 sentence 较短的场景

// 【易错点】
// 1. 拼接字符串末尾必须有空格："hello world " 不是 "hello world"
// 2. 回退判断是 s[(start-1) % len]，不是 s[start % len]
// 3. 记忆化法中 remain-- 扣空格，但最后一个词后的空格可以超出行宽（不需要额外处理）

// 【面试追问】
// Q1: 游标法为什么 start/len 就是答案? → start 是消耗的总字符位，len 是一轮的长度
// Q2: 复杂度? → 每行最多回退 max_word_len(<=10) 次，O(rows * 10) ≈ O(rows)
// Q3: 句子很长时? → 游标法仍 O(rows)，记忆化预处理 O(n*cols/len) 可能更慢
