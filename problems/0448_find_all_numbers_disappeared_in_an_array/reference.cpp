/*
 * LeetCode 448: 找到所有数组中消失的数字
 * (Find All Numbers Disappeared in an Array)
 *
 * 【题目本质】
 * nums[i] 在 [1, n] 范围内，利用数组索引作为哈希表进行原地标记，
 * 找出 [1, n] 中未出现的数字。经典"原地哈希"技巧。
 *
 * 【解法总览】
 * 解法1: HashSet         — O(n) / O(n)  — 最直觉，不满足 Follow-up
 * 解法2: 原地标记（取负） — O(n) / O(1)  — 面试首选
 * 解法3: 原地标记（加 n） — O(n) / O(1)  — 变体
 * 解法4: 交换归位         — O(n) / O(1)  — 变体
 */

// ============================================================
// 解法1: HashSet — 最直觉的做法
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 把所有数存入 Set，再遍历 1~n 检查哪些不在 Set 中。
// 简单但不满足 Follow-up 的 O(1) 额外空间要求。
// ============================================================
class Solution1 {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        unordered_set<int> seen(nums.begin(), nums.end());
        vector<int> result;
        int n = nums.size();
        for (int i = 1; i <= n; i++) {
            if (seen.find(i) == seen.end()) {
                result.push_back(i);
            }
        }
        return result;
    }
};

// ============================================================
// 解法2: 原地标记（取负）— O(n) / O(1) ⭐ 面试首选
//
// 【思路】
// 数字 x 出现了 → 把 nums[x-1] 标记为负数。
// 标记完后仍为正数的位置 i，说明 i+1 未出现。
//
// 为什么用"取负"？
// → 取负是可逆的：abs() 能恢复原值，不丢失信息。
//   如果置零则丢失原始值，后续元素无法正确映射。
//
// 标记过程 (nums = [4,3,2,7,8,2,3,1]):
//   i=0: nums[0]=4  → mark nums[3]=-7   → [4,3,2,-7,8,2,3,1]
//   i=1: nums[1]=3  → mark nums[2]=-2   → [4,3,-2,-7,8,2,3,1]
//   i=2: |nums[2]|=2 → mark nums[1]=-3  → [4,-3,-2,-7,8,2,3,1]
//   i=3: |nums[3]|=7 → mark nums[6]=-3  → [4,-3,-2,-7,8,2,-3,1]
//   i=4: nums[4]=8  → mark nums[7]=-1   → [4,-3,-2,-7,8,2,-3,-1]
//   i=5: |nums[5]|=2 → nums[1] already neg → skip
//   i=6: |nums[6]|=3 → nums[2] already neg → skip
//   i=7: |nums[7]|=1 → mark nums[0]=-4  → [-4,-3,-2,-7,8,2,-3,-1]
//
//   Positive positions: idx=4 (→5), idx=5 (→6) → result = [5, 6]
// ============================================================
class Solution2 {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        int n = nums.size();

        // 第一遍：标记出现过的数字
        for (int i = 0; i < n; i++) {
            // abs 还原可能已被标记为负的值
            int idx = abs(nums[i]) - 1;  // 数字 x → 索引 x-1
            if (nums[idx] > 0) {
                nums[idx] = -nums[idx];  // 标记为负
            }
        }

        // 第二遍：仍为正的位置对应缺失的数字
        vector<int> result;
        for (int i = 0; i < n; i++) {
            if (nums[i] > 0) {
                result.push_back(i + 1);  // 索引 i → 数字 i+1
            }
        }

        return result;
    }
};

// ============================================================
// 解法3: 原地标记（加 n）— O(n) / O(1) 变体
//
// 【思路】
// 不改变正负号，而是给出现过的数字对应位置加 n。
// 最终值 <= n 的位置说明对应数字从未出现。
//
// 好处：不改变正负号，逻辑更直观。
// 注意：需要 %n 来还原被加过的值。
// ============================================================
class Solution3 {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        int n = nums.size();

        // 第一遍：出现的数字对应位置加 n
        for (int i = 0; i < n; i++) {
            int idx = (nums[i] - 1) % n;  // %n 还原可能已加过 n 的值
            nums[idx] += n;
        }

        // 第二遍：值 <= n 表示从未被标记
        vector<int> result;
        for (int i = 0; i < n; i++) {
            if (nums[i] <= n) {
                result.push_back(i + 1);
            }
        }

        return result;
    }
};

// ============================================================
// 解法4: 交换归位 — O(n) / O(1) 变体
//
// 【思路】
// 把数字 x 放到索引 x-1 的位置（原地排序）。
// 最终 nums[i] != i+1 的位置就是缺失的。
//
// 关键：while 条件用 nums[i] != nums[nums[i]-1]
// 而非 nums[i] != i+1，避免重复数字死循环。
// ============================================================
class Solution4 {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        int n = nums.size();

        for (int i = 0; i < n; i++) {
            // 只要当前数不在正确位置，且目标位置的数不同，就交换
            while (nums[i] != nums[nums[i] - 1]) {
                swap(nums[i], nums[nums[i] - 1]);
            }
        }

        vector<int> result;
        for (int i = 0; i < n; i++) {
            if (nums[i] != i + 1) {
                result.push_back(i + 1);
            }
        }

        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间  | 空间  | 修改原数组 | 可恢复 |
// |------------|-------|-------|------------|--------|
// | HashSet    | O(n)  | O(n)  | 否         | N/A    |
// | 取负标记   | O(n)  | O(1)  | 是         | 是     |
// | 加 n 标记  | O(n)  | O(1)  | 是         | 是     |
// | 交换归位   | O(n)  | O(1)  | 是         | 否     |
//
// 【易错点】
// 1. 忘记 abs 导致索引为负:
//    ✗ int idx = nums[i] - 1;  → 如果 nums[i] 已被标记为负，索引变负
//    ✓ int idx = abs(nums[i]) - 1;
//
// 2. 重复取负导致标记失效:
//    ✗ nums[idx] = -nums[idx]; (不检查) → 取负两次变回正
//    ✓ if (nums[idx] > 0) nums[idx] = -nums[idx];
//
// 3. 加 n 方案忘记 %n:
//    ✗ int idx = nums[i] - 1;  → nums[i] 可能已被加过 n
//    ✓ int idx = (nums[i] - 1) % n;
//
// 4. 交换归位死循环:
//    ✗ while (nums[i] != i + 1) → 重复数字永远放不到"正确位置"
//    ✓ while (nums[i] != nums[nums[i] - 1]) → 目标已有同值就停
//
// 5. 索引偏移搞错:
//    ✗ result.push_back(i)  → 忘了+1
//    ✓ result.push_back(i + 1)  → 索引 i 对应数字 i+1
//
// 【面试追问】
// Q1: 为什么用取负而不是置零？
//     → 取负可逆(abs恢复)，置零丢失原值，后续映射出错。
//
// Q2: 数组只读怎么办？
//     → 只能用 HashSet O(n) 空间，或排序 O(n log n) 时间。
//
// Q3: 与 442(找重复) 的关系？
//     → 同样取负标记。448收集正数位置(缺失)，442收集已为负的位置(重复)。
//
// Q4: 这个技巧的适用范围？
//     → nums[i] 在 [1,n] 或 [0,n-1] 时均可原地哈希。
//       LC 41/268/287/442/645 都用此类技巧。
// ============================================================
