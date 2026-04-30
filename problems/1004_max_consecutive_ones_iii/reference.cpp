/*
 * LeetCode 1004: 最大连续1的个数 III (Max Consecutive Ones III)
 *
 * 【题目本质】
 * 找最长子数组，使得子数组中0的个数不超过k。
 * 把"翻转操作"转化为"子数组内0的个数约束"是关键一步。
 *
 * 【解法总览】
 * 解法1: 暴力枚举         — O(n²) / O(1)   — 最直觉，超时
 * 解法2: 前缀和 + 二分答案 — O(n log n) / O(n) — 利用单调性二分
 * 解法3: 滑动窗口          — O(n) / O(1)   — 面试首选 ⭐
 */

// ============================================================
// 解法1: 暴力枚举 — 枚举所有子数组，统计0的个数
// 时间: O(n²)  空间: O(1)
//
// 【思路】
// 最直接的想法：对每个起点 left，向右扩展 right，
// 维护窗口内0的个数。当0的个数超过k时停止。
// 瓶颈：left 每移动一位就要重新扫描，大量重复计算。
// n = 10^5 时 O(n²) = 10^10，会超时。
//
// 滑动窗口过程示意 (nums = [1,1,1,0,0,0,1,1,1,1,0], k=2):
//
//   left=0: [1,1,1,0,0] zeros=2 ✓ len=5
//           [1,1,1,0,0,0] zeros=3 ✗ → stop, 最长=5
//
//   left=1: [1,1,0,0] zeros=2 ✓ len=4
//           [1,1,0,0,0] zeros=3 ✗ → stop, 最长=4
//   ...
//   left=5: [0,1,1,1,1,0] zeros=2 ✓ len=6 ← 最优!
//
//   答案 = 6
// ============================================================
class Solution1 {
public:
    int longestOnes(vector<int>& nums, int k) {
        int n = nums.size();
        int maxLen = 0;
        for (int left = 0; left < n; left++) {
            int zeros = 0;
            for (int right = left; right < n; right++) {
                if (nums[right] == 0) zeros++;
                if (zeros > k) break;
                maxLen = max(maxLen, right - left + 1);
            }
        }
        return maxLen;
    }
};

// ============================================================
// 解法2: 前缀和 + 二分答案 — 二分窗口长度，用前缀和 O(1) 检验
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 观察：如果长度为 len 的窗口可以满足（存在一个位置使其中0≤k个），
// 那么长度为 len-1 的窗口一定也可以满足（更短更容易）。
// → 答案关于 len 具有单调性 → 可以二分答案。
//
// 对于每个候选长度 mid，我们滑动一个长度为 mid 的窗口，
// 用前缀和 O(1) 查询每个窗口内0的个数，看是否存在 ≤ k 的。
//
// 二分搜索空间收缩示意 (nums = [1,1,1,0,0,0,1,1,1,1,0], k=2):
// 
// lo=0, hi=11
// Round1: mid=6, 检查长度6 → [0,0,1,1,1,1]=2个0 ≤ 2 ✓ → lo=6
// Round2: mid=9, 检查长度9 → 所有窗口0个数都>2 ✗ → hi=8
// Round3: mid=7, 检查长度7 → 所有窗口0个数都>2 ✗ → hi=6
// lo==hi==6 → 答案=6
// ============================================================
class Solution2 {
public:
    int longestOnes(vector<int>& nums, int k) {
        int n = nums.size();
        
        // prefix[i] = nums[0..i-1] 中 0 的个数
        vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + (nums[i] == 0 ? 1 : 0);
        }
        
        // 二分答案长度
        int lo = 0, hi = n;
        while (lo < hi) {
            int mid = lo + (hi - lo + 1) / 2;  // 上取整，因为 lo = mid 时需要避免死循环
            
            // 检查：是否存在长度为 mid 的窗口，0的个数 ≤ k
            bool found = false;
            for (int i = 0; i + mid <= n; i++) {
                // 区间 [i, i+mid-1] 中0的个数 = prefix[i+mid] - prefix[i]
                if (prefix[i + mid] - prefix[i] <= k) {
                    found = true;
                    break;
                }
            }
            
            if (found) {
                lo = mid;   // 长度 mid 可以 → 尝试更长
            } else {
                hi = mid - 1;  // 长度 mid 不行 → 缩短
            }
        }
        return lo;
    }
};

// ============================================================
// 解法3: 滑动窗口 — 维护窗口内0的个数 ≤ k ⭐ 面试首选
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 从暴力解的瓶颈出发：暴力法中 left 每次右移，right 要从 left 重新开始。
// 但其实 left 右移只是"踢掉"了最左边的元素，窗口内0的个数最多减少1。
// 所以 right 不需要回退！
//
// 关键性质（为什么能用滑动窗口）：
// - right 右移：窗口内0的个数可能+1（单调不减）
// - left 右移：窗口内0的个数可能-1（单调不增）
// 这种单调性保证了 left 和 right 都只需要单调右移，总移动 O(n)。
//
// 滑动窗口过程 (nums = [1,1,1,0,0,0,1,1,1,1,0], k=2):
//
//  right=0: [1]                 zeros=0  len=1
//  right=1: [1,1]               zeros=0  len=2
//  right=2: [1,1,1]             zeros=0  len=3
//  right=3: [1,1,1,0]           zeros=1  len=4
//  right=4: [1,1,1,0,0]         zeros=2  len=5
//  right=5: [1,1,1,0,0,0]       zeros=3 > 2!
//           → 收缩: nums[0]=1, left=1   zeros=3
//           → 收缩: nums[1]=1, left=2   zeros=3
//           → 收缩: nums[2]=1, left=3   zeros=3
//           → 收缩: nums[3]=0, left=4   zeros=2 ✓  len=2
//  right=6: [0,0,1]             zeros=2  len=3
//  right=7: [0,0,1,1]           zeros=2  len=4
//  right=8: [0,0,1,1,1]         zeros=2  len=5
//  right=9: [0,0,1,1,1,1]       zeros=2  len=6 ← 最优!
//  right=10:[0,0,1,1,1,1,0]     zeros=3 > 2!
//           → 收缩: nums[4]=0, left=5   zeros=2 ✓  len=6
//
//  答案 = 6
// ============================================================
class Solution3 {
public:
    int longestOnes(vector<int>& nums, int k) {
        int left = 0, maxLen = 0, zeros = 0;
        
        for (int right = 0; right < (int)nums.size(); right++) {
            // 右边界扩展：新进来的元素如果是0，计数+1
            if (nums[right] == 0) zeros++;
            
            // 左边界收缩：当0的个数超过k时，必须缩小窗口
            // 为什么用 while 而不是 if：虽然本题每次 right 移动最多新增1个0，
            // 用 if 碰巧也对，但 while 是滑动窗口的标准范式，更安全通用
            while (zeros > k) {
                if (nums[left] == 0) zeros--;  // 踢出的如果是0，计数-1
                left++;
            }
            
            // 更新答案：此时 [left, right] 是以 right 结尾的最长合法窗口
            // 窗口长度 = right - left + 1（闭区间）
            maxLen = max(maxLen, right - left + 1);
        }
        
        return maxLen;
    }
};

// ============================================================
// 解法3变体: 滑动窗口（不收缩版本）— 只保持窗口不缩小
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 一个巧妙的变体：窗口只扩大或平移，不收缩。
// 当 zeros > k 时，left 和 right 同时右移（窗口平移而非收缩）。
// 这样窗口大小只增不减，最终窗口大小就是答案。
//
// 为什么正确：一旦我们找到了长度为 L 的合法窗口，
// 之后我们只关心 "能不能找到更长的"，所以没必要让窗口缩小到比 L 还小。
// ============================================================
class Solution3b {
public:
    int longestOnes(vector<int>& nums, int k) {
        int left = 0, zeros = 0;
        
        for (int right = 0; right < (int)nums.size(); right++) {
            if (nums[right] == 0) zeros++;
            
            // 注意：这里用 if 而不是 while
            // 当 zeros > k 时，窗口平移：left 和 right 都右移一步
            // 窗口大小不变（不缩小，只平移或扩大）
            if (zeros > k) {
                if (nums[left] == 0) zeros--;
                left++;
            }
        }
        
        // 最终窗口大小就是答案
        return (int)nums.size() - left;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法      | 时间       | 空间  | 核心思想             |
// |-----------|-----------|-------|---------------------|
// | 暴力      | O(n²)     | O(1)  | 穷举所有子数组        |
// | 前缀和+二分| O(n log n)| O(n)  | 二分答案长度          |
// | 滑动窗口  | O(n)      | O(1)  | 双指针单调移动 ⭐     |
// | 不收缩变体 | O(n)      | O(1)  | 窗口只增不减          |
//
// 面试首选解法3（标准滑动窗口），代码简洁，逻辑清晰。
// 解法3b（不收缩版本）更巧妙但不太直观，适合作为追问讨论。
//
// 【易错点】
// 1. 窗口长度计算：right - left + 1，不是 right - left。
//    ✗ maxLen = max(maxLen, right - left);
//    ✓ maxLen = max(maxLen, right - left + 1);
//
// 2. 收缩时忘记更新 zeros：
//    ✗ while (zeros > k) { left++; }  // 踢掉的是0也不减计数
//    ✓ while (zeros > k) { if (nums[left] == 0) zeros--; left++; }
//
// 3. 前缀和索引错位：prefix[i] 代表前 i 个元素（nums[0..i-1]）中0的个数。
//    区间 [l, r] 中0的个数 = prefix[r+1] - prefix[l]，不是 prefix[r] - prefix[l]。
//
// 4. 二分答案上取整导致死循环：
//    ✗ int mid = lo + (hi - lo) / 2;  // 当 lo = mid 时，lo 不变 → 死循环
//    ✓ int mid = lo + (hi - lo + 1) / 2;  // 上取整，保证 mid > lo
//
// 【面试追问】
// Q1(基础): 暴力解的瓶颈是什么？为什么滑动窗口是 O(n)？
// → 暴力法每个 left 都重新扫描 right，O(n²)。滑动窗口中 left 和 right
//   都只单调右移，各最多移动 n 次，总共 O(2n) = O(n)。
//
// Q2(优化): while 能不能改成 if？
// → 本题可以，因为每次 right 移动最多新增1个0（zeros 从 k 变成 k+1），
//   只需要收缩一次。但一般滑动窗口题用 while 更安全通用。
//   解法3b 展示了用 if 的另一种理解方式（窗口只平移不收缩）。
//
// Q3(变体): 如果要求"恰好翻转k个0"呢？
// → 用 atMost(k) - atMost(k-1) 的技巧：
//   "恰好k个0的最长子数组" = "至多k个0的最长" - "至多k-1个0的最长"
//   不对！这个技巧适用于"计数"而不是"最长"。
//   对于"恰好k个0的最长子数组"，需要同时维护两个窗口，
//   或者在收缩条件改为 zeros > k 的同时，只在 zeros == k 时更新答案。
//
// Q4(扩展): 如果数组不是0/1，而是任意整数，要求翻转最多k个非target值？
// → 完全一样的滑动窗口，只需把 nums[i] == 0 改成 nums[i] != target。
// ============================================================
