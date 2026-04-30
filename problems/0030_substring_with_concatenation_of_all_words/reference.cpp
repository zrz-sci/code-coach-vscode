// =====================================================
// 30. Substring with Concatenation of All Words
// 标签: Hash Table, String, Sliding Window
// 难度: Hard
// =====================================================

// 解法1: 暴力哈希 — 对每个起点截取子串，逐个切分检查频次
// 时间 O(s_len * wordCount * wordLen), 空间 O(wordCount * wordLen)
//
// 思考过程:
//   最直接的方式 — 枚举每个可能的起始位置 i，截取长度 wordLen*wordCount 的子串，
//   按 wordLen 步长切成 wordCount 个块，统计每个块出现的频次，和目标频次表比较。
//   关键剪枝: 一旦发现不在 target 中的词或超频的词，立即跳出内层循环。
class Solution1 {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> res;
        if (words.empty() || s.empty()) return res;

        int wordLen = words[0].size();
        int wordCount = words.size();
        int totalLen = wordLen * wordCount;

        // 构建目标词频表
        unordered_map<string, int> target;
        for (auto& w : words) target[w]++;

        // 枚举每个起始位置
        for (int i = 0; i + totalLen <= (int)s.size(); i++) {
            unordered_map<string, int> seen;
            int j = 0;
            for (; j < wordCount; j++) {
                // 截取第 j 个 "单词槽"
                string word = s.substr(i + j * wordLen, wordLen);

                // 不在 target 中 → 这个起点不行
                if (target.find(word) == target.end()) break;

                seen[word]++;
                // 出现次数超过目标 → 这个起点也不行
                if (seen[word] > target[word]) break;
            }
            // 所有 wordCount 个词都匹配成功
            if (j == wordCount) res.push_back(i);
        }
        return res;
    }
};


// =====================================================
// 解法2: 滑动窗口 + 哈希表 — 最优解 ⭐
// 时间 O(s_len * wordLen), 空间 O(wordCount * wordLen)
//
// 思考过程:
//   暴力法中，起点 i 和 i+wordLen 的检查有大量重叠——窗口只差一个词的进出。
//   利用"所有单词等长"的性质，将字符串按 wordLen 分成 wordLen 组偏移:
//     偏移0: 位置 0, wordLen, 2*wordLen, ...
//     偏移1: 位置 1, 1+wordLen, 1+2*wordLen, ...
//     ...
//   每组偏移内独立运行一个滑动窗口，窗口步长 = wordLen。
//
//   窗口维护:
//   - window: 哈希表记录窗口内各单词的出现次数
//   - count: 窗口内已匹配的单词总数
//   - left: 窗口左边界
//
//   三种情况:
//   (1) 加入的词在 target 中 → 加入 window，如果超频则从左收缩
//   (2) 加入的词不在 target 中 → 整个窗口作废，重置
//   (3) count == wordCount → 找到一个匹配，记录 left
class Solution2 {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> res;
        if (words.empty() || s.empty()) return res;

        int sLen = s.size();
        int wordLen = words[0].size();
        int wordCount = words.size();
        int totalLen = wordLen * wordCount;
        if (sLen < totalLen) return res;

        // 目标频次表
        unordered_map<string, int> target;
        for (auto& w : words) target[w]++;

        // 对每种偏移量 offset = 0, 1, ..., wordLen-1 分别跑滑动窗口
        for (int offset = 0; offset < wordLen; offset++) {
            unordered_map<string, int> window;
            int count = 0;       // 窗口内匹配的单词总数
            int left = offset;   // 窗口左边界

            for (int right = offset; right + wordLen <= sLen; right += wordLen) {
                // 取出右侧新进入的单词
                string wordIn = s.substr(right, wordLen);

                if (target.count(wordIn)) {
                    // 情况1: 有效单词，加入窗口
                    window[wordIn]++;
                    count++;

                    // 如果该单词次数超过目标，从左侧逐个移除
                    while (window[wordIn] > target[wordIn]) {
                        string wordOut = s.substr(left, wordLen);
                        window[wordOut]--;
                        count--;
                        left += wordLen;
                    }

                    // 情况3: 检查是否完全匹配
                    if (count == wordCount) {
                        res.push_back(left);
                        // 注意: 不需要手动收缩，下一轮 right 右移后
                        // count 变成 wordCount+1，while 会自然收缩
                    }
                } else {
                    // 情况2: 无效单词，彻底重置窗口
                    window.clear();
                    count = 0;
                    left = right + wordLen;
                }
            }
        }
        return res;
    }
};


/*
 * =====================================================
 * 解法对比
 * =====================================================
 *
 * | 解法       | 时间                      | 空间          | 适用场景         |
 * |------------|--------------------------|---------------|-----------------|
 * | Solution1  | O(s_len * wordCount * w) | O(wordCount)  | 快速实现，面试保底 |
 * | Solution2  | O(s_len * wordLen)       | O(wordCount)  | 最优解，面试必写   |
 *
 * =====================================================
 * 易错点 (具体 bug)
 * =====================================================
 *
 * 1. 忘记 wordLen 种偏移量:
 *    只从 offset=0 跑窗口，漏掉 s="afoobar" words=["foo","bar"] → 答案 [1]
 *
 * 2. 遇到无效单词只 continue 不重置:
 *    错误: if (!target.count(wordIn)) continue;
 *    正确: window.clear(); count=0; left=right+wordLen;
 *
 * 3. words 有重复但用 set 去重:
 *    words=["word","word"] 需要匹配2个"word"，set 会丢失频次信息
 *
 * 4. substr 越界:
 *    循环条件 right+wordLen <= sLen，不是 < sLen
 *
 * 5. 收缩时移错变量:
 *    while 内必须同时更新 window[wordOut]--, count--, left+=wordLen
 *    遗漏任何一个都会导致死循环或结果错误
 *
 * =====================================================
 * 面试追问
 * =====================================================
 *
 * Q1: words 中单词长度不同?
 * A:  偏移+等步长技巧失效，需要 Aho-Corasick 多模式匹配或 Trie + DFS
 *
 * Q2: 用 rolling hash 优化?
 * A:  可以将 substr 的 O(w) 降到 O(1)，总复杂度 O(s_len)
 *
 * Q3: s 极长(10^6) words 极少(5个)?
 * A:  滑窗 O(s_len*w) = 3*10^6 >> 暴力 O(s_len*n*w) = 1.5*10^7
 *
 * Q4: words 全部相同?
 * A:  频次表只有一项 {w: n}，窗口正常工作，无需特殊处理
 *
 * Q5: 能否 O(s_len) 时间?
 * A:  用 rolling hash 预处理所有单词的哈希值，窗口内用哈希比较而非 substr
 */
