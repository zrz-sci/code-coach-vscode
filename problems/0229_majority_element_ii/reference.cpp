/*
 * 【题目本质】
 * 找数组中所有出现超过 n/3 次的元素。
 * 关键观察：最多只有 2 个这样的元素。
 * 核心：Boyer-Moore 投票法扩展（三三抵消）。
 *
 * 【解法总览】
 * 解法1: Boyer-Moore 投票扩展 ⭐ — O(N) 时间 O(1) 空间
 * 解法2: HashMap 计数 — O(N) 时间 O(N) 空间
 */

// ===================== 解法1: Boyer-Moore 投票扩展 (推荐) =====================
// 思路：
// 1. 维护两个候选人 cand1, cand2 和计数器 cnt1, cnt2
// 2. 遍历数组：
//    - 匹配候选人 → 对应计数器 +1
//    - 某个计数器为 0 → 替换为新候选人
//    - 都不匹配 → 两个计数器都 -1（三三抵消）
// 3. 第二轮遍历验证候选人是否真的超过 n/3
//
// 为什么有效：出现 > n/3 的元素不可能被完全抵消
// （其他元素总数 < 2n/3，不够消耗 > n/3 的元素）
class Solution1 {
public:
    vector<int> majorityElement(vector<int>& nums) {
        int n = nums.size();
        // 初始化两个候选人为不同值
        int cand1 = 0, cand2 = 1;
        int cnt1 = 0, cnt2 = 0;

        // 第一轮：寻找候选人
        for (int num : nums) {
            if (num == cand1) {
                cnt1++;           // 匹配候选人1
            } else if (num == cand2) {
                cnt2++;           // 匹配候选人2
            } else if (cnt1 == 0) {
                cand1 = num;      // 替换候选人1
                cnt1 = 1;
            } else if (cnt2 == 0) {
                cand2 = num;      // 替换候选人2
                cnt2 = 1;
            } else {
                cnt1--;           // 三三抵消
                cnt2--;
            }
        }

        // 第二轮：验证候选人（必须！候选人可能不满足 > n/3）
        cnt1 = cnt2 = 0;
        for (int num : nums) {
            if (num == cand1) cnt1++;
            else if (num == cand2) cnt2++;  // else if 防止 cand1==cand2 重复计数
        }

        vector<int> result;
        if (cnt1 > n / 3) result.push_back(cand1);
        if (cnt2 > n / 3) result.push_back(cand2);
        return result;
    }
};

// ===================== 解法2: HashMap 计数 =====================
// 思路：
// 1. 遍历数组，用 unordered_map 记录每个元素出现次数
// 2. 遍历 map，筛选出 count > n/3 的元素
// 简单直接，但空间 O(N) 不满足 follow-up 要求
class Solution2 {
public:
    vector<int> majorityElement(vector<int>& nums) {
        int n = nums.size();
        unordered_map<int, int> count;
        vector<int> result;

        // 统计频次
        for (int num : nums) {
            count[num]++;
        }

        // 筛选超过 n/3 的
        for (auto& [val, cnt] : count) {
            if (cnt > n / 3) {
                result.push_back(val);
            }
        }
        return result;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 满足 follow-up |
 * |------|------|------|---------------|
 * | Boyer-Moore ⭐ | O(N) | O(1) | 是 |
 * | HashMap | O(N) | O(N) | 否 |
 *
 * 【易错点】
 * 1. cand1 和 cand2 必须初始化为不同值（如 0 和 1）
 * 2. if-else 顺序关键：先匹配候选人，再检查 cnt==0，最后抵消
 *    错误顺序会导致把候选人覆盖掉
 * 3. 第二轮验证必须做！Boyer-Moore 只找候选人，不保证满足条件
 * 4. 验证时用 else if（不是两个独立 if），防止 cand1==cand2 重复计数
 * 5. 条件是严格大于 > n/3，不是 >= n/3
 *
 * 【面试追问】
 * Q1: 推广到 > n/k？ → 维护 k-1 个候选人，k 个一起抵消
 * Q2: 流式数据怎么办？ → Boyer-Moore 给候选人，但无法验证。需要额外假设或第二遍扫描
 * Q3: 证明正确性？ → 出现 > n/3 的元素每次被抵消需要消耗2个其他元素，总消耗 < 2n/3
 */
