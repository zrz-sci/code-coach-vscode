/*
 * LeetCode 896: Monotonic Array (单调数列)
 *
 * 【题目本质】
 * 线性扫描判断：遍历数组一次，检查是否全程不下降（单调递增）或全程不上升（单调递减）。
 * 只要两者之一成立就返回 true。
 *
 * 【解法总览】
 * 解法1: 双标志一次遍历   — O(n) / O(1) — 面试首选
 * 解法2: 两次遍历         — O(n) / O(1) — 代码更清晰
 * 解法3: 差分符号法       — O(n) / O(1) — 另一种思路
 */

// ============================================================
// 解法1: 双标志一次遍历 — 同时跟踪递增和递减（⭐ 面试首选）
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 维护两个布尔标志:
//   isInc = true  → "截至目前，数组有可能是单调递增的"
//   isDec = true  → "截至目前，数组有可能是单调递减的"
//
// 遍历所有相邻元素对 (nums[i], nums[i+1]):
//   - 发现下降 (nums[i] > nums[i+1]) → 不可能递增 → isInc = false
//   - 发现上升 (nums[i] < nums[i+1]) → 不可能递减 → isDec = false
//   - 相等 → 两种可能都保留
//
// 最终: isInc || isDec 为 true → 单调
//
// 提前退出: 如果 !isInc && !isDec，直接返回 false。
//
// 示例1: [1,2,2,3]
//   i=0: 1<2 → isDec=false     isInc=true, isDec=false
//   i=1: 2==2 → 无变化          isInc=true, isDec=false
//   i=2: 2<3 → isDec=false     isInc=true, isDec=false
//   return true (isInc=true)
//
// 示例2: [6,5,4,4]
//   i=0: 6>5 → isInc=false     isInc=false, isDec=true
//   i=1: 5>4 → isInc=false     isInc=false, isDec=true
//   i=2: 4==4 → 无变化          isInc=false, isDec=true
//   return true (isDec=true)
//
// 示例3: [1,3,2]
//   i=0: 1<3 → isDec=false     isInc=true, isDec=false
//   i=1: 3>2 → isInc=false     isInc=false, isDec=false
//   提前退出 → return false
// ============================================================
class Solution1 {
public:
    bool isMonotonic(vector<int>& nums) {
        bool isInc = true;  // 可能是单调递增
        bool isDec = true;  // 可能是单调递减

        for (int i = 0; i + 1 < (int)nums.size(); i++) {
            // 下降 → 排除单调递增
            if (nums[i] > nums[i + 1]) isInc = false;
            // 上升 → 排除单调递减
            if (nums[i] < nums[i + 1]) isDec = false;

            // 提前退出: 两种可能都被排除
            if (!isInc && !isDec) return false;
        }

        // 至少一种可能保留 → 单调
        return true; // 等价于 isInc || isDec
    }
};

// ============================================================
// 解法2: 两次遍历 — 分别检查递增和递减
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 最直觉的方法: 写两个辅助函数，分别检查:
//   1. 是否所有 nums[i] <= nums[i+1] (单调递增)
//   2. 是否所有 nums[i] >= nums[i+1] (单调递减)
// 任一成立就返回 true。
//
// 优点: 代码非常清晰，逻辑分离
// 缺点: 最坏情况遍历两次（但常数因子小，实际差异不大）
//
// 注意: C++ STL 中 is_sorted 函数就是做这件事:
//   is_sorted(begin, end) 检查是否按 operator< 排序（即非降序）
//   is_sorted(rbegin, rend) 反向检查即非升序
// ============================================================
class Solution2 {
public:
    bool isMonotonic(vector<int>& nums) {
        return isIncreasing(nums) || isDecreasing(nums);
    }

private:
    // 检查是否单调递增（非严格，允许相等）
    bool isIncreasing(const vector<int>& nums) {
        for (int i = 0; i + 1 < (int)nums.size(); i++) {
            if (nums[i] > nums[i + 1]) return false;
        }
        return true;
    }

    // 检查是否单调递减（非严格，允许相等）
    bool isDecreasing(const vector<int>& nums) {
        for (int i = 0; i + 1 < (int)nums.size(); i++) {
            if (nums[i] < nums[i + 1]) return false;
        }
        return true;
    }
};

// ============================================================
// 解法3: 差分符号法 — 检查所有非零差分方向是否一致
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 计算相邻差 diff = nums[i+1] - nums[i]:
//   - diff > 0 → 上升 (sign = +1)
//   - diff < 0 → 下降 (sign = -1)
//   - diff == 0 → 跳过（不影响方向判断）
//
// 第一个非零 diff 确定方向 (direction)。
// 后续每个非零 diff 的符号必须与 direction 一致，
// 否则数组不单调。
//
// 如果所有 diff 都是 0（全相等数组），direction 始终为 0，
// 最终返回 true（全相等既是递增又是递减）。
//
// 示例: [1, 2, 2, 3]
//   diff: [1, 0, 1]
//   sign:  1, skip, 1
//   direction: 0 → 1 → 1  一致 → true
//
// 示例: [1, 3, 2]
//   diff: [2, -1]
//   sign:  1, -1
//   direction: 0 → 1 → 与 -1 不一致 → false
// ============================================================
class Solution3 {
public:
    bool isMonotonic(vector<int>& nums) {
        int direction = 0;  // 0=未确定, 1=递增, -1=递减

        for (int i = 0; i + 1 < (int)nums.size(); i++) {
            int diff = nums[i + 1] - nums[i];

            // 相等，跳过
            if (diff == 0) continue;

            // 确定本次差分的符号
            int sign = (diff > 0) ? 1 : -1;

            if (direction == 0) {
                // 首次确定方向
                direction = sign;
            } else if (direction != sign) {
                // 方向不一致 → 非单调
                return false;
            }
        }

        return true;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法           | 时间  | 空间  | 遍历次数 | 特点              |
// |---------------|------|------|---------|------------------|
// | 双标志一次遍历 | O(n) | O(1) | 1       | ⭐面试首选，最高效  |
// | 两次遍历       | O(n) | O(1) | ≤ 2     | 代码最清晰        |
// | 差分符号法     | O(n) | O(1) | 1       | 另一种思路        |
//
// 三种方法本质相同:
//   - 都是遍历检查相邻元素对的方向一致性
//   - 都正确处理全相等数组（返回 true）
//   - 时间空间复杂度完全一样
//
// 面试建议:
//   首选双标志法（简洁高效）→ 追问时解释差分符号法
//   如果追问 STL: is_sorted(begin,end) || is_sorted(rbegin,rend)
//
// 【易错点】
// 1. 用严格不等号:
//    ✗ nums[i] < nums[i+1] 作为递增条件
//    → [1,2,2,3] 中 2==2 会误判为非递增
//    ✓ 用 nums[i] <= nums[i+1] (非严格，允许相等)
//
// 2. 全相等数组误判:
//    ✗ 返回条件用异或 isInc ^ isDec → [3,3,3] 两个都是 true，异或为 false
//    ✓ 返回 isInc || isDec
//
// 3. 循环越界:
//    ✗ for (i=0; i<nums.size(); i++) 然后访问 nums[i+1]
//    ✓ for (i=0; i+1<(int)nums.size(); i++)
//    注: (int) 转换是因为 size() 返回 size_t (无符号)，
//        如果 nums 为空，size()-1 会下溢。
//
// 4. 差分溢出:
//    ✗ 当 nums[i] 值域很大时 nums[i+1]-nums[i] 可能溢出 int
//    ✓ 本题 |nums[i]| ≤ 10^5，差值 ≤ 2*10^5，安全
//       但变体题中可能需要 long long
//
// 5. 提前退出条件写反:
//    ✗ if (isInc || isDec) return false → 完全反了
//    ✓ if (!isInc && !isDec) return false → 两种可能都排除才退出
//
// 【面试追问 Interview Follow-ups】
// Q1: 能否用 STL 一行解决？
// A1: return is_sorted(nums.begin(), nums.end()) ||
//            is_sorted(nums.rbegin(), nums.rend());
//     is_sorted 检查是否非降序。对反向迭代器调用则检查是否非升序。
//
// Q2: 如果要判断严格单调呢？
// A2: 把 <= 改为 <，>= 改为 >。
//     双标志法: 发现 nums[i] >= nums[i+1] → isStrictInc = false
//              发现 nums[i] <= nums[i+1] → isStrictDec = false
//     注意: 全相等数组 [3,3,3] 此时应返回 false。
//
// Q3: 如果输入是流（不知道长度），能否一次通过判断？
// A3: 可以。用双标志法，读一对元素就更新标志。
//     不需要知道总长度，逐元素处理即可。
//
// Q4: 时间复杂度能否低于 O(n)？
// A4: 不能。必须检查每个元素（反证: 如果跳过某个元素，
//     可能恰好是那个元素破坏了单调性）。O(n) 是下界。
// ============================================================
