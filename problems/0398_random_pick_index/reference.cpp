/*
 * 398. Random Pick Index
 *
 * 【题目本质】
 * 含重复元素的数组中，等概率随机返回目标值的索引。
 * 核心考点：蓄水池抽样 vs 哈希表空间换时间。
 *
 * 【解法总览】
 * Solution1: 哈希表预处理 — init O(n), pick O(1), 空间 O(n)
 * Solution2: 蓄水池抽样   — init O(1), pick O(n), 额外空间 O(1)
 */

// ===================== Solution 1: 哈希表预处理 =====================
// 思路：构造时将每个值的所有索引存入哈希表，pick 时直接随机选一个
// 时间：init O(n), pick O(1) | 空间：O(n)
class Solution1 {
    unordered_map<int, vector<int>> indices;
public:
    Solution1(vector<int>& nums) {
        for (int i = 0; i < (int)nums.size(); i++) {
            indices[nums[i]].push_back(i);
        }
    }

    int pick(int target) {
        auto& v = indices[target];
        return v[rand() % v.size()];
    }
};

// ===================== Solution 2: 蓄水池抽样 =====================
// 思路：每次 pick 遍历数组，遇到第 cnt 个 target 以 1/cnt 概率更新结果
// 时间：init O(1), pick O(n) | 空间：O(1) extra（仅存引用）
class Solution2 {
    vector<int> nums;
public:
    Solution2(vector<int>& nums) : nums(nums) {}

    int pick(int target) {
        int cnt = 0, res = -1;
        for (int i = 0; i < (int)nums.size(); i++) {
            if (nums[i] == target) {
                cnt++;
                if (rand() % cnt == 0) {
                    res = i;
                }
            }
        }
        return res;
    }
};

/*
 * 【解法对比】
 * | 解法 | init | pick | 空间 | 适用场景 |
 * |------|------|------|------|---------|
 * | 哈希表 | O(n) | O(1) | O(n) | pick频繁、内存充足 |
 * | 蓄水池 | O(1) | O(n) | O(1) | 流式数据、内存受限 |
 *
 * 【易错点】
 * 1. 蓄水池中 cnt 必须先自增再判断 rand()%cnt==0
 * 2. 判断条件是 ==0 不是 ==1，否则第一个元素无法被选中
 * 3. 蓄水池法仍需保存整个数组（或引用），因为每次 pick 都要遍历
 *
 * 【面试追问】
 * Q1: 证明蓄水池抽样的等概率性？
 *     第k个元素最终被选中 = (1/k) * 连乘(k/(k+1) * ... * (n-1)/n) = 1/n
 * Q2: 如果要选m个不重复索引？=> Reservoir Sampling-K
 * Q3: 数据量极大时如何优化？=> 分布式蓄水池抽样
 */
