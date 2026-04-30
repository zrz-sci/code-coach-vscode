/*
 * 【题目本质】
 * LC 243 的变体：word1 和 word2 可能相同。
 * 相同时需追踪同一个词的两次不同出现之间的最小距离。
 * 关键技巧：遇到 word1 时，若 word1==word2，先把旧位置存入 idx2 再更新 idx1。
 *
 * 【解法总览】
 * 解法1: 统一一次扫描  - O(n) 时间, O(1) 空间 ⭐推荐
 * 解法2: 分类讨论      - O(n) 时间, O(1) 空间
 */

// ==================== 解法1: 统一一次扫描 ====================
// 思路: 遇到 word1 时，若 same 则先 idx2=idx1 保存旧位置，再 idx1=i
// 这样 idx1/idx2 总是"最近两次不同出现的位置"
class Solution1 {
public:
    int shortestWordDistance(vector<string>& wordsDict, string word1, string word2) {
        int idx1 = -1, idx2 = -1;
        int minDist = INT_MAX;
        bool same = (word1 == word2);

        for (int i = 0; i < (int)wordsDict.size(); i++) {
            if (wordsDict[i] == word1) {
                if (same) {
                    // 同一个词: 旧的 idx1 变成 idx2
                    idx2 = idx1;
                }
                idx1 = i;
            } else if (wordsDict[i] == word2) {
                // 不同词时才走这里
                idx2 = i;
            }

            if (idx1 != -1 && idx2 != -1) {
                minDist = min(minDist, abs(idx1 - idx2));
            }
        }

        return minDist;
    }
};

// ==================== 解法2: 分类讨论 ====================
// 思路: 显式分为 word1==word2 和 word1!=word2 两种情况
// 优点: 逻辑清晰，容易理解
class Solution2 {
public:
    int shortestWordDistance(vector<string>& wordsDict, string word1, string word2) {
        int minDist = INT_MAX;

        if (word1 == word2) {
            // Case: 同一个词的两次出现之间的最小距离
            int prev = -1;
            for (int i = 0; i < (int)wordsDict.size(); i++) {
                if (wordsDict[i] == word1) {
                    if (prev != -1) {
                        minDist = min(minDist, i - prev);
                    }
                    prev = i;
                }
            }
        } else {
            // Case: 不同词（与 LC 243 完全相同）
            int idx1 = -1, idx2 = -1;
            for (int i = 0; i < (int)wordsDict.size(); i++) {
                if (wordsDict[i] == word1) idx1 = i;
                else if (wordsDict[i] == word2) idx2 = i;

                if (idx1 != -1 && idx2 != -1) {
                    minDist = min(minDist, abs(idx1 - idx2));
                }
            }
        }

        return minDist;
    }
};

/*
 * 【解法对比】
 * ┌──────────────┬──────────┬──────────┬──────────────────────────────┐
 * │ 解法         │ 时间     │ 空间     │ 特点                         │
 * ├──────────────┼──────────┼──────────┼──────────────────────────────┤
 * │ 统一扫描     │ O(n)     │ O(1)    │ 代码简洁，一套逻辑兼容两种   │
 * │ 分类讨论     │ O(n)     │ O(1)    │ 逻辑清晰，面试容易解释       │
 * └──────────────┴──────────┴──────────┴──────────────────────────────┘
 *
 * 【易错点】
 * 1. word1==word2 时仍用 else if -> idx2 永远不会被赋值
 *    修正: 同一个词时，先 idx2=idx1 保存旧值，再 idx1=i
 * 2. 不保存旧位置直接覆盖 idx1 -> 丢失了前一次出现的信息
 * 3. 当 word1==word2 且该词只出现一次时 -> 题目保证两个独立词，至少出现两次
 * 4. 分类讨论时 word1==word2 分支忘记检查 prev!=-1 -> 第一次出现不能计算距离
 *
 * 【面试追问】
 * Q: 统一解法如何保证正确性？
 * A: 当 same=true 时，每次遇到该词:
 *    - idx2 = idx1 (旧位置)
 *    - idx1 = i (新位置)
 *    此时 idx1 和 idx2 就是"最近两次出现"，dist = idx1 - idx2 > 0
 *    当 same=false 时，idx2 = idx1 不执行，退化为 LC 243
 *
 * Q: 这个系列(243/244/245)的设计思想？
 * A: 243: 基础单次查询
 *    244: 多次查询 -> 预处理 HashMap + 双指针
 *    245: 边界条件 word1==word2
 *    面试常考三道一起问，考察对问题变体的应变能力
 */
