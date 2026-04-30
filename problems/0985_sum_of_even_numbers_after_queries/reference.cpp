/*
 * LeetCode 985: 查询后的偶数和 (Sum of Even Numbers After Queries)
 *
 * 【题目本质】
 * 数组单点修改 + 全局偶数和查询。核心：维护偶数和的增量更新。
 * 每次查询只改一个元素，O(1) 计算偶数和变化。
 *
 * 【解法总览】
 * 解法1: 暴力模拟       — O(n*q) / O(q)  — 每次遍历整个数组
 * 解法2: 增量维护偶数和  — O(n+q) / O(q)  — 面试首选 ⭐
 */

// ============================================================
// 解法1: 暴力模拟
// 时间: O(n * q)  空间: O(q)
//
// 【思路】
// 最直接的做法：每次查询后遍历数组求偶数和。
// n = q = 10^4 时约 10^8 次操作，可能 TLE。
// ============================================================
class Solution1 {
public:
    vector<int> sumEvenAfterQueries(vector<int>& nums, vector<vector<int>>& queries) {
        vector<int> answer;
        for (auto& q : queries) {
            int val = q[0], index = q[1];
            nums[index] += val; // 修改数组

            // 遍历求偶数和
            int evenSum = 0;
            for (int x : nums) {
                if (x % 2 == 0) evenSum += x;
            }
            answer.push_back(evenSum);
        }
        return answer;
    }
};

// ============================================================
// 解法2: 增量维护偶数和 — 面试首选 ⭐
// 时间: O(n + q)  空间: O(q)
//
// 【思路】
// 为什么想到增量更新？
// 1. 每次查询只修改一个元素 → 偶数和的变化只与该元素有关
// 2. 不需要重新扫描整个数组
// 3. O(1) 处理每个查询: 减去旧贡献 + 加上新贡献
//
// 四种情况分析（实际代码不需要分支，自然覆盖）:
//   旧值偶 + val偶 → 新值偶:  evenSum = evenSum - old + new = evenSum + val
//   旧值偶 + val奇 → 新值奇:  evenSum = evenSum - old
//   旧值奇 + val偶 → 新值奇:  evenSum 不变
//   旧值奇 + val奇 → 新值偶:  evenSum = evenSum + new
//
// 示例: nums = [1, 2, 3, 4], 初始 evenSum = 2 + 4 = 6
//
//   查询[1,0]: old=1(奇)不减, new=2(偶)加 → evenSum = 6+2 = 8
//   查询[-3,1]: old=2(偶)减2, new=-1(奇)不加 → evenSum = 8-2 = 6
//   查询[-4,0]: old=2(偶)减2, new=-2(偶)加(-2) → evenSum = 6-2+(-2) = 2
//   查询[2,3]: old=4(偶)减4, new=6(偶)加6 → evenSum = 2-4+6 = 4
//   结果: [8, 6, 2, 4] ✓
// ============================================================
class Solution2 {
public:
    vector<int> sumEvenAfterQueries(vector<int>& nums, vector<vector<int>>& queries) {
        // 第一步：计算初始偶数和
        int evenSum = 0;
        for (int x : nums) {
            if (x % 2 == 0) evenSum += x;
        }

        vector<int> answer;
        answer.reserve(queries.size()); // 预分配，避免扩容

        for (auto& q : queries) {
            int val = q[0], index = q[1];
            int oldVal = nums[index];

            // 如果旧值是偶数，从偶数和中减去它
            // （因为这个元素即将被修改，它的旧贡献不再有效）
            if (oldVal % 2 == 0) {
                evenSum -= oldVal;
            }

            // 执行修改
            nums[index] = oldVal + val;

            // 如果新值是偶数，将其加入偶数和
            if (nums[index] % 2 == 0) {
                evenSum += nums[index];
            }

            answer.push_back(evenSum);
        }
        return answer;
    }
};

// ============================================================
// 解法2的等价简洁写法
// 利用位运算判断奇偶: (x & 1) == 0 表示偶数
// ============================================================
class Solution3 {
public:
    vector<int> sumEvenAfterQueries(vector<int>& nums, vector<vector<int>>& queries) {
        int evenSum = 0;
        for (int x : nums) {
            if ((x & 1) == 0) evenSum += x;
        }

        vector<int> answer;
        answer.reserve(queries.size());

        for (auto& q : queries) {
            int val = q[0], idx = q[1];

            // 旧值是偶数 → 移除贡献
            if ((nums[idx] & 1) == 0) evenSum -= nums[idx];

            // 更新元素
            nums[idx] += val;

            // 新值是偶数 → 加入贡献
            if ((nums[idx] & 1) == 0) evenSum += nums[idx];

            answer.push_back(evenSum);
        }
        return answer;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 代码量 | 推荐场景 |
// |------|------|------|--------|----------|
// | 暴力 | O(n*q) | O(q) | 少 | 说思路 |
// | 增量维护 | O(n+q) | O(q) | 中等 | ⭐面试首选 |
// | 位运算版 | O(n+q) | O(q) | 中等 | 追求简洁 |
//
// 【易错点】
//
// 1. 先修改数组再判断旧值
//    ✗ nums[idx] += val; if (nums[idx]-val was even)... → 逻辑混乱
//    ✓ 先保存/判断旧值，再修改数组，再判断新值
//
// 2. queries格式搞反
//    ✗ int index = q[0], val = q[1]; → index和val反了
//    ✓ queries[i] = [val, index]，val在前，index在后
//
// 3. 负数偶数判断
//    ✗ 认为负数不是偶数 → -4 是偶数，-4 % 2 == 0 ✓
//    ✓ x % 2 == 0 对正负数都正确判断偶数
//
// 4. 忘记实际更新 nums[index]
//    ✗ 只更新了 evenSum 但没改 nums → 后续查询读到旧值
//    ✓ 必须 nums[index] += val (或 nums[index] = newVal)
//
// 5. 偶数和用 unsigned 类型
//    ✗ unsigned evenSum → 负偶数 -4 加入后溢出
//    ✓ 用 int，偶数和可能是负数
//
// 【面试追问】
//
// Q1: 为什么不需要四种 if-else？
// → 代码中只有两个独立的 if：
//    if (旧值偶) 减去旧值;
//    if (新值偶) 加上新值;
//    这两个条件互相独立，四种组合自然被覆盖。
//
// Q2: 如果改成区间修改（给 [l, r] 都加 val）？
// → O(1) 增量更新不再可行，需要线段树维护区间偶数和。
//    或者用差分数组 + 离线处理。
//
// Q3: 如果同时要求偶数和与奇数和？
// → 维护 totalSum = sum(nums)，每次 totalSum += val。
//    奇数和 = totalSum - evenSum。只需一个额外变量。
//
// Q4: 空间能优化到 O(1) 吗？
// → 题目要求返回 vector<int> answer，大小为 q，
//    所以 O(q) 的输出空间是必需的。额外空间已经是 O(1)。
// ============================================================
