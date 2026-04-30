/*
 * 【题目本质】
 * 给定 N 人的年龄数组，按条件统计好友请求总数。
 * 发送条件简化为：0.5 * age[x] + 7 < age[y] <= age[x]。
 * 利用年龄范围有限（1~120），用桶计数将 O(N^2) 降至 O(N + C^2)。
 *
 * 【解法总览】
 * Solution1: 桶计数 + 年龄对枚举（推荐）—— O(N + 120^2)
 * Solution2: 排序 + 二分查找 —— O(NlogN)，通用方法
 */

// ===================== Solution1: 桶计数 + 年龄对枚举（推荐） =====================
// 思路：
// 1. 统计每个年龄的人数 count[age]
// 2. 枚举发送者年龄 ageA 和接收者年龄 ageB
// 3. 判断条件：0.5 * ageA + 7 < ageB <= ageA
// 4. 同年龄特殊处理：count * (count - 1)，不同年龄：countA * countB
// 时间 O(N + 120^2)，空间 O(120) = O(1)
class Solution1 {
public:
    int numFriendRequests(vector<int>& ages) {
        // 桶计数：count[i] = 年龄为 i 的人数
        vector<int> count(121, 0);
        for (int age : ages) count[age]++;

        int result = 0;
        // 年龄 <= 14 的人不可能发送请求（0.5 * 14 + 7 = 14 >= 14）
        for (int ageA = 15; ageA <= 120; ageA++) {
            if (count[ageA] == 0) continue;

            // 接收者年龄范围：(ageA/2 + 7, ageA]
            int minAge = ageA / 2 + 7 + 1; // 严格大于 ageA/2 + 7
            int maxAge = ageA;

            for (int ageB = minAge; ageB <= maxAge; ageB++) {
                if (count[ageB] == 0) continue;

                if (ageA == ageB) {
                    // 同年龄：N 人中每人向其余 N-1 人发送
                    result += count[ageA] * (count[ageA] - 1);
                } else {
                    // 不同年龄：ageA 的每人向 ageB 的每人发送
                    result += count[ageA] * count[ageB];
                }
            }
        }

        return result;
    }
};

// ===================== Solution2: 排序 + 二分查找 =====================
// 思路：
// 排序后，对每个人 i，二分查找可接收请求的年龄范围
// 时间 O(NlogN)，空间 O(排序所需)
class Solution2 {
public:
    int numFriendRequests(vector<int>& ages) {
        sort(ages.begin(), ages.end());
        int n = ages.size();
        int result = 0;

        for (int i = 0; i < n; i++) {
            int ageX = ages[i];
            if (ageX <= 14) continue;

            // 接收者年龄 y 必须满足：ageX/2 + 7 < y <= ageX
            int lowerBound = ageX / 2 + 7; // y > lowerBound

            // 找第一个 > lowerBound 的位置（左边界）
            int left = upper_bound(ages.begin(), ages.end(), lowerBound) - ages.begin();
            // 找第一个 > ageX 的位置（右边界）
            int right = upper_bound(ages.begin(), ages.end(), ageX) - ages.begin();

            // 范围 [left, right) 中包含自己，需要减 1
            int cnt = right - left - 1;
            if (cnt > 0) result += cnt;
        }

        return result;
    }
};

/*
 * 【解法对比】
 * ┌──────────────────┬──────────────────┬──────────────┬─────────────────────────┐
 * │ 解法             │ 时间复杂度       │ 空间复杂度   │ 适用场景                │
 * ├──────────────────┼──────────────────┼──────────────┼─────────────────────────┤
 * │ 桶计数           │ O(N + 120^2)     │ O(1)         │ 值域有限时最优          │
 * │ 排序 + 二分      │ O(NlogN)         │ O(1)         │ 通用，不依赖值域限制    │
 * └──────────────────┴──────────────────┴──────────────┴─────────────────────────┘
 *
 * 【易错点】
 * 1. 条件简化错误：第三个条件 age[y]>100 && age[x]<100 被第二个条件蕴含，
 *    但很多人会单独处理它，引入不必要的复杂度。
 * 2. 整数除法 vs 浮点：0.5 * ageA + 7 应写为 ageA / 2 + 7（整数除法），
 *    避免浮点精度问题。注意 C++ 中整数除法向零截断。
 * 3. 自发请求：题目明确 x != y，同年龄时必须减 1。
 *    count[a] * (count[a] - 1) 而非 count[a]^2。
 * 4. 年龄下限：age <= 14 的人不可能发送请求。
 *    因为 0.5 * age + 7 >= age 当且仅当 age <= 14。
 * 5. 严格不等号：条件是 age[y] > 0.5*age[x]+7（严格大于），
 *    不是 >= 。对应整数条件为 ageB >= ageA/2 + 7 + 1。
 *
 * 【面试追问】
 * Q1: 能否用前缀和优化桶计数方法？
 *     → 可以。预计算 prefixCount[i] = count[1..i] 的前缀和。
 *       对每个 ageA，接收者范围的总人数 = prefix[ageA] - prefix[ageA/2+7]。
 *       时间复杂度降为 O(N + 120)。
 *
 * Q2: 如果请求是双向的（x 发给 y 则 y 也发给 x），答案如何变化？
 *     → 不变。因为 x→y 和 y→x 需要各自满足条件。
 *       题目已经分别计算了每个方向。
 *
 * Q3: 如果加入新条件"同性别才能发送请求"，如何扩展？
 *     → 按性别分组：分别统计男/女的年龄计数数组。
 *       只在同性别的计数数组内进行年龄对枚举。
 *
 * Q4: 为什么桶计数比排序+二分更快？
 *     → 桶计数避免了排序的 O(NlogN) 开销和每次二分的 O(logN)。
 *       当 N >> C^2 = 14400 时（即 N > 15000），桶计数显著更快。
 */
