/*
 * LeetCode 209: 长度最小的子数组 (Minimum Size Subarray Sum)
 *
 * 【题目本质】
 * 在正整数数组中找总和 ≥ target 的最短连续子数组长度。
 * 关键：元素全为正 → 子数组和随长度增加单调递增 → 可用滑动窗口。
 *
 * 【解法总览】
 * 解法1: 暴力枚举       — O(n²) / O(1)      — 最直觉，面试讲思路起点
 * 解法2: 滑动窗口        — O(n)  / O(1)      — 面试首选
 * 解法3: 前缀和+二分查找  — O(n log n) / O(n) — Follow-up 解法
 */

// ============================================================
// 解法1: 暴力枚举 — 枚举每个起点，找最短满足条件的终点
// 时间: O(n²)  空间: O(1)
//
// 【思路】
// 最朴素的想法：枚举子数组起点 i，从 i 开始逐个加入元素，
// 一旦累加和 ≥ target 就记录长度并停止（因为更长的一定不优）。
// 瓶颈：对每个起点都要线性扫描，没有复用之前的计算。
// ============================================================
class Solution1 {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int n = nums.size();
        int ans = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            int sum = 0;
            for (int j = i; j < n; j++) {
                sum += nums[j];
                if (sum >= target) {
                    ans = min(ans, j - i + 1);
                    break; // 以 i 为起点的最短已找到，更长无意义
                }
            }
        }
        
        return ans == INT_MAX ? 0 : ans;
    }
};

// ============================================================
// 解法2: 滑动窗口（双指针）— 右扩左缩，维护窗口和
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法1的瓶颈：当起点从 i 移到 i+1 时，丢弃了 [i+1, j] 的累加信息。
// 优化核心：用两个指针 left, right 维护一个窗口 [left, right]。
//   - right 右移：窗口扩大，和增加（因为元素为正）
//   - left 右移：窗口缩小，和减少
//   - 单调性保证：不会漏解。
//
// 为什么是 O(n)？
//   虽然有 for + while 嵌套，但 left 在整个过程中最多从 0 走到 n，
//   不会回退。right 走 n 步 + left 总共走 ≤ n 步 = O(2n) = O(n)。
//
// 滑动窗口过程 (target=7, nums=[2,3,1,2,4,3]):
//
//   right=0: [2]           sum=2  < 7
//   right=1: [2,3]         sum=5  < 7
//   right=2: [2,3,1]       sum=6  < 7
//   right=3: [2,3,1,2]     sum=8  ≥ 7 → len=4, 收缩
//            [3,1,2]       sum=6  < 7
//   right=4: [3,1,2,4]    sum=10 ≥ 7 → len=4, 收缩
//            [1,2,4]       sum=7  ≥ 7 → len=3, 收缩
//            [2,4]         sum=6  < 7
//   right=5: [2,4,3]      sum=9  ≥ 7 → len=3, 收缩
//            [4,3]         sum=7  ≥ 7 → len=2 ← 最优! 收缩
//            [3]           sum=3  < 7
//   结束。答案=2
// ============================================================
class Solution2 {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int n = nums.size();
        int ans = INT_MAX;
        int left = 0;
        int sum = 0;
        
        for (int right = 0; right < n; right++) {
            sum += nums[right]; // 扩大窗口：加入右端元素
            
            // 窗口和满足条件时，尝试收缩左边界以寻找更短的子数组
            // 用 while 不是 if：收缩一次后可能仍满足条件，需要继续收缩
            while (sum >= target) {
                ans = min(ans, right - left + 1);
                sum -= nums[left]; // 收缩窗口：减去左端元素
                left++;
            }
        }
        
        // 如果 ans 没被更新过，说明不存在满足条件的子数组
        return ans == INT_MAX ? 0 : ans;
    }
};

// ============================================================
// 解法3: 前缀和 + 二分查找 — Follow-up O(n log n) 解法
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 题目 Follow-up 要求 O(n log n) 解法。
// 
// 关键观察：元素全为正 → 前缀和严格递增。
// 构建前缀和 prefix[0..n]，其中 prefix[0]=0, prefix[i]=nums[0]+...+nums[i-1]
// 子数组 nums[i..j] 的和 = prefix[j+1] - prefix[i]
// 
// 对于每个起点 i，要找最小的 j 使得：
//   prefix[j+1] - prefix[i] >= target
//   即 prefix[j+1] >= prefix[i] + target
// 
// 因为 prefix 严格递增，可以在 prefix[i+1..n] 上用 lower_bound 二分！
//
// 前缀和示意 (target=7, nums=[2,3,1,2,4,3]):
//   index:    0   1   2   3   4   5   6
//   prefix: [ 0,  2,  5,  6,  8, 12, 15]
//
//   i=0: need=7,  lower_bound→prefix[4]=8,  len=4-0=4
//   i=1: need=9,  lower_bound→prefix[5]=12, len=5-1=4
//   i=2: need=12, lower_bound→prefix[5]=12, len=5-2=3
//   i=3: need=13, lower_bound→prefix[6]=15, len=6-3=3
//   i=4: need=15, lower_bound→prefix[6]=15, len=6-4=2 ← 最优!
//   i=5: need=19, 不存在
//   答案=2
// ============================================================
class Solution3 {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int n = nums.size();
        
        // 构建前缀和数组，prefix[0]=0 表示空前缀
        vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
        
        int ans = INT_MAX;
        for (int i = 0; i < n; i++) {
            // 要找第一个 prefix[j] >= prefix[i] + target, j 在 [i+1, n] 中
            int need = prefix[i] + target;
            
            // lower_bound: 在有序序列中找第一个 >= need 的位置
            auto it = lower_bound(prefix.begin() + i + 1, prefix.end(), need);
            
            if (it != prefix.end()) {
                int j = it - prefix.begin();
                // 子数组 nums[i..j-1]，长度 = j - i
                // 注意不是 j-i+1，因为前缀和索引差就是元素个数
                ans = min(ans, j - i);
            }
        }
        
        return ans == INT_MAX ? 0 : ans;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间       | 空间  | 核心操作                 |
// |---------------|-----------|-------|--------------------------|
// | 暴力枚举      | O(n²)     | O(1)  | 双重循环                 |
// | 滑动窗口      | O(n)      | O(1)  | 双指针同向移动           |
// | 前缀和+二分   | O(n log n)| O(n)  | lower_bound              |
//
// 面试首选解法2（滑动窗口），O(n) 且空间 O(1)，代码简洁。
// 解法3 是 Follow-up，展示前缀和+二分的思维方式。
// 三种解法都依赖"元素全正"这个条件。如果有负数，需要用单调队列（LC 862）。
//
// 【易错点】
// 1. 滑动窗口收缩用 if 而不是 while:
//    ✗ if (sum >= target) { ans=min(...); sum-=nums[left++]; }
//    ✓ while (sum >= target) { ans=min(...); sum-=nums[left++]; }
//    原因：满足条件后连续收缩可能仍满足，if 只收缩一次会漏掉更短答案。
//    反例：target=4, nums=[1,1,1,1,4]，right指向4时sum=8，
//          需要连续收缩left 4次才能发现 len=1。
//
// 2. 前缀和版本子数组长度算错:
//    ✗ ans = min(ans, j - i + 1) （多算了1）
//    ✓ ans = min(ans, j - i)
//    原因：prefix[j] - prefix[i] 对应 nums[i..j-1]，共 j-i 个元素。
//
// 3. 忘记处理无解:
//    ✗ return ans;
//    ✓ return ans == INT_MAX ? 0 : ans;
//
// 4. 前缀和可能溢出:
//    nums[i] ≤ 10^4, n ≤ 10^5 → 前缀和最大 10^9，int 范围内安全。
//    但如果题目范围更大（如 nums[i] ≤ 10^9），需要用 long long。
//
// 【面试追问】
// Q1: 暴力解的瓶颈在哪？→ 每个起点都要线性扫描终点，没复用之前的计算。
// Q2: 滑动窗口为什么是 O(n)？→ left 不回退，整个过程 left 最多走 n 步。
// Q3: 数组有负数怎么办？→ 滑动窗口和二分都失效（无单调性），
//     用单调双端队列+前缀和，维护前缀和递增队列，O(n)。参考 LC 862。
// Q4: 如果要返回具体子数组而不是长度？→ 记录取得最优 ans 时的 left 值即可。
// ============================================================
