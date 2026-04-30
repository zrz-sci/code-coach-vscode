/*
 * LeetCode 287: 寻找重复数 (Find the Duplicate Number)
 *
 * 【题目本质】
 * n+1 个整数存于 [1,n] 值域，恰有一个重复数。不能修改数组 + O(1) 空间。
 * 把 i → nums[i] 看成链表，重复数 = 环入口（经典 Floyd 应用）。
 *
 * 【解法总览】
 * 解法1: 哈希表       — O(n) / O(n)       — 最直觉，不满足空间约束
 * 解法2: 二分答案     — O(n log n) / O(1) — 对值域二分，满足约束
 * 解法3: 位运算       — O(n log n) / O(1) — 逐位统计还原
 * 解法4: Floyd 环检测 — O(n) / O(1)       — 最优，面试首选
 */

// ============================================================
// 解法1: 哈希表 — 记录见过的数，遇到重复即返回
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 最直觉的做法：遍历一次，用集合记录出现过的数。
// 遇到已在集合中的数就是答案。
// 不满足 O(1) 空间约束，但作为面试开场展示理解。
// ============================================================
class Solution1 {
public:
    int findDuplicate(vector<int>& nums) {
        unordered_set<int> seen;
        for (int num : nums) {
            // 如果已经见过，就是重复数
            if (seen.count(num)) return num;
            seen.insert(num);
        }
        return -1; // 题目保证有重复，不会到这里
    }
};

// ============================================================
// 解法2: 二分答案 — 对值域 [1, n] 二分
// 时间: O(n log n)  空间: O(1)
//
// 【思路】
// 搜索空间不是数组索引，而是答案的值域 [1, n]。
// 对于猜测值 mid，统计数组中 ≤ mid 的元素个数 count：
//   - 如果 count > mid：[1, mid] 中的数"太多了"，重复数 ≤ mid
//   - 如果 count ≤ mid：[1, mid] 中没多余的，重复数 > mid
// 每次二分需要 O(n) 扫描，共 O(log n) 次，总 O(n log n)。
//
// 二分搜索空间收缩示例: nums = [1,3,4,2,2], n=4
//   Round1: left=1, right=4, mid=2
//     count(≤2) = 3 > mid=2  → right=2, 答案在 [1,2]
//   Round2: left=1, right=2, mid=1
//     count(≤1) = 1 ≤ mid=1  → left=2, 答案在 [2,2]
//   left==right==2 → 答案 = 2 ✓
// ============================================================
class Solution2 {
public:
    int findDuplicate(vector<int>& nums) {
        int left = 1, right = nums.size() - 1; // 值域 [1, n]，n = nums.size()-1

        while (left < right) {
            int mid = left + (right - left) / 2;

            // 统计数组中 ≤ mid 的元素个数
            int count = 0;
            for (int num : nums) {
                if (num <= mid) count++;
            }

            // count > mid: 根据鸽巢原理，重复数在 [left, mid]
            // 注意 mid 本身可能是答案，所以 right = mid 而非 mid-1
            if (count > mid) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }

        return left; // left == right 时即为答案
    }
};

// ============================================================
// 解法3: 位运算 — 逐位比较 nums 和 [1,n]
// 时间: O(n log n)  空间: O(1)  (准确说是 O(32n) = O(n))
//
// 【思路】
// 对二进制的每一位，分别统计：
//   numsCount: nums 数组中该位为 1 的元素个数
//   baseCount: [1, n] 中该位为 1 的元素个数
// 如果 numsCount > baseCount，说明重复数在该位上为 1。
// 逐位还原重复数。
//
// 为什么正确？nums 比 [1,n] 多出来的那些"1"，
// 全部来自重复数的多余出现次数。
// ============================================================
class Solution3 {
public:
    int findDuplicate(vector<int>& nums) {
        int n = nums.size() - 1;
        int ans = 0;

        for (int bit = 0; bit < 32; bit++) {
            int mask = 1 << bit;
            int numsCount = 0;  // nums 中该位为 1 的个数
            int baseCount = 0;  // [1,n] 中该位为 1 的个数

            for (int i = 0; i <= n; i++) {
                if (nums[i] & mask) numsCount++;
                if (i & mask) baseCount++;  // i 从 1 到 n（i=0 时 i&mask=0，自动跳过）
            }

            // 多出来的 1 来自重复数
            if (numsCount > baseCount) {
                ans |= mask;
            }
        }

        return ans;
    }
};

// ============================================================
// 解法4: Floyd 环检测（快慢指针）— 最优解 ⭐
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 将数组看成隐式链表：节点 i 的下一个节点是 nums[i]。
// 因为值域 [1,n]，索引 0 不会被指向 → 0 是链头。
// 重复数意味着两个不同索引指向同一个值 → 环！
// 环的入口就是重复数。
//
// nums = [1, 3, 4, 2, 2]
//
//   索引: 0   1   2   3   4
//   值:   1   3   4   2   2
//
//   链: 0 → 1 → 3 → 2 → 4 → 2 → 4 → ...
//                     ↑_________↓
//                       环!
//   环入口 = 2，即重复数
//
// 为什么索引 0 安全？
//   值域是 [1,n]，没有 nums[i]=0，所以 0 只出不入，
//   它一定在环外面的"尾巴"上。
//
// Phase 1: 快慢指针找环内相遇点
//   slow 每次 1 步，fast 每次 2 步，必在环内相遇。
//
// Phase 2: 找环入口
//   设 a = 起点到入口距离, b = 入口到相遇点, c = 相遇点到入口
//   相遇时: 2(a+b) = a+b+k(b+c) → a = (k-1)(b+c) + c
//   → 从起点走 a 步 = 从相遇点走 c 步 + 若干整圈 → 都到入口
// ============================================================
class Solution4 {
public:
    int findDuplicate(vector<int>& nums) {
        // Phase 1: 快慢指针，找环内相遇点
        // 用 do-while 保证从同一起点出发后至少走一步再比较
        int slow = 0, fast = 0;
        do {
            slow = nums[slow];             // 走 1 步
            fast = nums[nums[fast]];       // 走 2 步
        } while (slow != fast);

        // Phase 2: 找环入口
        // 一个从起点 (0) 出发，一个从相遇点出发，步长都是 1
        int ptr = 0;
        while (ptr != slow) {
            ptr = nums[ptr];    // 从起点走
            slow = nums[slow];  // 从相遇点走
        }

        // 两者相遇处即为环入口 = 重复数
        return ptr;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法        | 时间        | 空间  | 满足约束 | 思路难度 |
// |-------------|-------------|-------|----------|----------|
// | 哈希表      | O(n)        | O(n)  | ✗ 空间   | 简单     |
// | 二分答案    | O(n log n)  | O(1)  | ✓        | 中等     |
// | 位运算      | O(n log n)  | O(1)  | ✓        | 中等     |
// | Floyd 环    | O(n)        | O(1)  | ✓ 最优   | 较难     |
//
// 面试策略:
//   先说哈希表(展示理解) → 被追问O(1)空间给二分 → 被追问O(n)给Floyd
//
// 【易错点】
// 1. Floyd 起点: slow=0, fast=0 用 do-while，或 slow=nums[0], fast=nums[nums[0]]
//    用 while。两种写法必须配套，混用会出错。
//    ✗ slow=0, fast=0 + while(slow!=fast) → 第一次就相等，直接跳过
//    ✓ slow=0, fast=0 + do-while → 至少走一步再判断
//
// 2. 二分答案方向:
//    ✗ if (count > mid) left = mid + 1;   // 反了! 多的在左边
//    ✓ if (count > mid) right = mid;
//
// 3. 二分答案边界:
//    ✗ right = mid - 1   // mid 可能就是答案
//    ✓ right = mid        // 搭配 while(left < right)
//
// 4. Phase 2 起点:
//    ✗ ptr = slow;  // 两个都在环里，永远找不到入口
//    ✓ ptr = 0;     // 一个必须从链头出发
//
// 【面试追问】
// Q1(基础): 如何证明必有重复？
//   → 鸽巢原理: n+1 个数放入 n 个值域位置。
//
// Q2(优化): 不用额外空间怎么办？
//   → 二分答案: 对值域二分，每次 O(n) 扫描统计。
//
// Q3(最优): 能否 O(n) 时间 + O(1) 空间？
//   → Floyd 环检测: i→nums[i] 构成带环链表，环入口=重复数。
//
// Q4(深入): 为什么 Phase 2 能找到入口？
//   → 数学推导: a = (k-1)(b+c) + c，起点和相遇点同步走 a 步
//      都到达入口。
//
// Q5(变体): 如果允许修改数组呢？
//   → 原地标记法: 访问 nums[i] 时将 nums[abs(nums[i])] 取负，
//      再次访问到负数说明该位置被指向过两次 → 重复数。
//
// Q6(扩展): 如果有多个重复数呢？
//   → Floyd 不适用(多个环入口)。可用位运算变体或放宽空间约束。
// ============================================================
