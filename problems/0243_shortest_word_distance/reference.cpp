/*
 * 【题目本质】
 * 在字符串数组中找两个不同单词出现位置的最小索引差。
 * 核心：一次扫描，维护两个词各自最近出现的位置索引。
 *
 * 【解法总览】
 * 解法1: 一次扫描双指针        - O(n) 时间, O(1) 空间 ⭐推荐
 * 解法2: 收集索引 + 双指针合并  - O(n) 时间, O(n) 空间（多次查询适用）
 */

// ==================== 解法1: 一次扫描 ====================
// 思路: 维护 word1/word2 最近出现的位置，每次更新时计算距离
// 关键: 初始化为 -1，两者都出现过后才计算
class Solution1 {
public:
    int shortestDistance(vector<string>& wordsDict, string word1, string word2) {
        int idx1 = -1, idx2 = -1;
        int minDist = INT_MAX;

        for (int i = 0; i < (int)wordsDict.size(); i++) {
            if (wordsDict[i] == word1) {
                idx1 = i;
            } else if (wordsDict[i] == word2) {
                idx2 = i;
            }

            // 两个词都至少出现过一次时才计算
            if (idx1 != -1 && idx2 != -1) {
                minDist = min(minDist, abs(idx1 - idx2));
            }
        }

        return minDist;
    }
};

// ==================== 解法2: 收集索引 + 双指针合并 ====================
// 思路: 先收集两个词的所有位置，再对两个有序列表做合并找最小差
// 优势: 预处理后可支持多次查询 (LC 244 思路基础)
class Solution2 {
public:
    int shortestDistance(vector<string>& wordsDict, string word1, string word2) {
        vector<int> pos1, pos2;

        // 收集两个词的所有位置
        for (int i = 0; i < (int)wordsDict.size(); i++) {
            if (wordsDict[i] == word1) pos1.push_back(i);
            else if (wordsDict[i] == word2) pos2.push_back(i);
        }

        // 双指针在两个有序数组中找最小差
        int minDist = INT_MAX;
        int i = 0, j = 0;
        while (i < (int)pos1.size() && j < (int)pos2.size()) {
            minDist = min(minDist, abs(pos1[i] - pos2[j]));
            // 移动值较小的指针，尝试缩小差距
            if (pos1[i] < pos2[j]) i++;
            else j++;
        }

        return minDist;
    }
};

/*
 * 【解法对比】
 * ┌─────────────────┬──────────┬──────────┬──────────────────────────┐
 * │ 解法            │ 时间     │ 空间     │ 特点                     │
 * ├─────────────────┼──────────┼──────────┼──────────────────────────┤
 * │ 一次扫描        │ O(n)     │ O(1)    │ 单次查询最优              │
 * │ 收集+双指针     │ O(n)     │ O(n)    │ 多次查询可复用位置列表    │
 * └─────────────────┴──────────┴──────────┴──────────────────────────┘
 *
 * 【易错点】
 * 1. 初始索引设为0而非-1 -> 0是合法位置，idx1=0时会误算距离
 * 2. 用 if + if 而非 if + else if -> 虽然题目保证 word1!=word2 结果不变，
 *    但 else if 语义更清晰
 * 3. 忘记检查两个词都出现过 -> abs(-1 - i) 产生错误大值
 * 4. 字符串比较用 == 而非 strcmp -> C++ string 的 == 重载是正确的
 *
 * 【面试追问】
 * Q: 多次查询不同词对怎么优化？(LC 244)
 * A: 预处理 HashMap<string, vector<int>> 存每个词的位置列表，
 *    查询时对两个有序列表做双指针合并。预处理 O(n), 查询 O(m1+m2)。
 *
 * Q: word1 == word2 怎么办？(LC 245)
 * A: 不能用 else if。遇到同一个词时，用前一次位置和当前位置计算距离。
 *    关键是区分"这是同一个词的两次出现"的情况。
 */
