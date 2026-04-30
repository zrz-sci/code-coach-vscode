/*
 * 【题目本质】
 * 排名问题：给每个分数按降序分配名次，前三名用特殊字符串表示。
 * 核心是"排序时保留原始索引，然后映射回原位置"。
 *
 * 【解法总览】
 * 解法1: 排序 + 索引数组  O(n log n) / O(n)  ⭐推荐
 * 解法2: 最大堆           O(n log n) / O(n)
 */

// ===================== 解法1: 排序 + 索引数组 =====================
// 思路：索引按分数降序排列，遍历分配名次到原始位置
class Solution1 {
public:
    vector<string> findRelativeRanks(vector<int>& score) {
        int n = score.size();
        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        // 按分数降序
        sort(idx.begin(), idx.end(), [&](int a, int b) {
            return score[a] > score[b];
        });

        vector<string> result(n);
        for (int i = 0; i < n; i++) {
            if (i == 0)      result[idx[i]] = "Gold Medal";
            else if (i == 1) result[idx[i]] = "Silver Medal";
            else if (i == 2) result[idx[i]] = "Bronze Medal";
            else             result[idx[i]] = to_string(i + 1);
        }

        return result;
    }
};

// ===================== 解法2: 最大堆 =====================
// 思路：最大堆按分数排序，依次弹出分配名次
class Solution2 {
public:
    vector<string> findRelativeRanks(vector<int>& score) {
        int n = score.size();
        priority_queue<pair<int,int>> pq; // (分数, 原始索引)
        for (int i = 0; i < n; i++) {
            pq.push({score[i], i});
        }

        vector<string> result(n);
        int rank = 1;
        while (!pq.empty()) {
            auto [s, i] = pq.top(); pq.pop();
            if (rank == 1)      result[i] = "Gold Medal";
            else if (rank == 2) result[i] = "Silver Medal";
            else if (rank == 3) result[i] = "Bronze Medal";
            else                result[i] = to_string(rank);
            rank++;
        }

        return result;
    }
};

/*
 * 【解法对比】
 * 解法1（排序+索引）：代码最简洁，排序后一次遍历即可，推荐
 * 解法2（最大堆）：思路更直观（每次取最大分配名次），适合堆的练习
 * 两者时间复杂度相同，解法1常数更小
 *
 * 【易错点】
 * 1. 名次写到 result[idx[i]]（原始位置），不是 result[i]（排序位置）
 * 2. 名次从 1 开始 → to_string(i + 1)，不是 to_string(i)
 * 3. 前三名字符串注意拼写："Gold Medal"（有空格，M 大写）
 * 4. 排序方向：降序（第 1 名 = 分数最高）
 *
 * 【面试追问】
 * Q1: 分数并列 → 同分同名次，后续名次跳过
 * Q2: O(n) 解法 → 计数排序/桶排序（分数范围有限时）
 * Q3: 只需前 k 名 → partial_sort 或 nth_element
 */
