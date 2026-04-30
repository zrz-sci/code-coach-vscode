/*
 * 401. Binary Watch
 *
 * 【题目本质】
 * 10 个 LED（4小时+6分钟）中选 turnedOn 个亮灯，枚举所有合法时间。
 * 本质是组合枚举 + 合法性检查。
 *
 * 【解法总览】
 * Solution1: 枚举所有时间 — O(720), 统计 popcount 匹配
 * Solution2: 回溯法       — O(2^10), 从 10 个 LED 中选 k 个
 */

// ===================== Solution 1: 枚举所有时间 =====================
// 思路：遍历 12*60=720 种时间，检查 h 和 m 的二进制 1 个数之和
// 时间：O(720) ≈ O(1) | 空间：O(1)
class Solution1 {
public:
    vector<string> readBinaryWatch(int turnedOn) {
        vector<string> res;
        for (int h = 0; h < 12; h++) {
            for (int m = 0; m < 60; m++) {
                if (__builtin_popcount(h) + __builtin_popcount(m) == turnedOn) {
                    res.push_back(to_string(h) + ":" + (m < 10 ? "0" : "") + to_string(m));
                }
            }
        }
        return res;
    }
};

// ===================== Solution 2: 回溯法 =====================
// 思路：10 个 LED 中选 turnedOn 个，前 4 个代表小时，后 6 个代表分钟
// 时间：O(C(10, turnedOn)) | 空间：O(10) 递归栈
class Solution2 {
    vector<string> res;
    int hours[4] = {1, 2, 4, 8};
    int mins[6]  = {1, 2, 4, 8, 16, 32};
public:
    void dfs(int num, int idx, int h, int m) {
        if (h > 11 || m > 59) return;   // 剪枝：超出合法范围
        if (num == 0) {
            res.push_back(to_string(h) + ":" + (m < 10 ? "0" : "") + to_string(m));
            return;
        }
        for (int i = idx; i < 10; i++) {
            if (i < 4)
                dfs(num - 1, i + 1, h + hours[i], m);
            else
                dfs(num - 1, i + 1, h, m + mins[i - 4]);
        }
    }

    vector<string> readBinaryWatch(int turnedOn) {
        dfs(turnedOn, 0, 0, 0);
        return res;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | 枚举 | O(720) | O(1) | 代码最简，不易出错 |
 * | 回溯 | O(C(10,k)) | O(10) | 体现组合思想 |
 *
 * 【易错点】
 * 1. 分钟格式必须两位：m=5 => "05" 不是 "5"
 * 2. 小时范围 0-11（不是 0-15），分钟范围 0-59（不是 0-63）
 * 3. 回溯法中 hours 和 mins 数组的值是权重不是索引
 *
 * 【面试追问】
 * Q1: 枚举法和回溯法哪个更好？ => 枚举法更好，搜索空间更小更直观
 * Q2: __builtin_popcount 不可用时怎么办？ => n &= (n-1) 循环计数
 * Q3: 如果 LED 数量不固定呢？ => 回溯法更通用，可处理任意数量
 */
