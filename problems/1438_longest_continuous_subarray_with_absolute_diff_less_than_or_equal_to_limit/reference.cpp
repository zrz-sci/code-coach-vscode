/*
 * LeetCode 1438: 绝对差不超过限制的最长连续子数组
 * 
 * 【题目本质】
 * 找最长连续窗口 [left, right]，使得 max(窗口) - min(窗口) <= limit。
 * "任意两元素绝对差 <= limit" 等价于 "最大值 - 最小值 <= limit"。
 *
 * 【解法总览】
 * 解法1: 暴力枚举         — O(n²) / O(1)   — 最直觉，超时
 * 解法2: multiset滑动窗口  — O(n log n) / O(n) — 有序集合维护极值
 * 解法3: 堆+懒删除         — O(n log n) / O(n) — 大顶堆max + 小顶堆min
 * 解法4: 单调队列滑动窗口  — O(n) / O(n)    — 最优，面试首选
 */

// ============================================================
// 解法1: 暴力枚举 — 枚举起点，向右扩展维护 max/min
// 时间: O(n²)  空间: O(1)
//
// 【思路】
// 最直觉的做法：固定左端点 i，向右扩展 j，
// 每扩展一步就更新 max 和 min，一旦 max-min > limit 就停止。
// 这里确认了核心观察：只需跟踪窗口的 max 和 min。
// 瓶颈：两层循环 O(n²)，n=10^5 会超时。
// ============================================================
class Solution1 {
public:
    int longestSubarray(vector<int>& nums, int limit) {
        int n = nums.size(), ans = 0;
        for (int i = 0; i < n; i++) {
            int mx = nums[i], mn = nums[i];
            for (int j = i; j < n; j++) {
                mx = max(mx, nums[j]);
                mn = min(mn, nums[j]);
                if (mx - mn > limit) break; // 后面只会更大，剪枝
                ans = max(ans, j - i + 1);
            }
        }
        return ans;
    }
};

// ============================================================
// 解法2: multiset 滑动窗口 — 有序集合维护窗口极值
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 解法1的瓶颈：窗口变化后无法快速获取 max/min。
// multiset 是自动排序的容器（红黑树），*begin() 是 min，*rbegin() 是 max。
// 插入 O(log n)，删除 O(log n)，查询极值 O(1)。
// 
// 滑动窗口流程：
//   右指针扩展 → 插入新元素 → 检查 max-min > limit →
//   若不满足 → 删除左指针元素，left++ → 直到满足
//
// 注意：erase 必须用 find 返回的迭代器，否则会删除所有同值元素！
// ============================================================
class Solution2 {
public:
    int longestSubarray(vector<int>& nums, int limit) {
        multiset<int> window;
        int left = 0, ans = 0;
        for (int right = 0; right < (int)nums.size(); right++) {
            window.insert(nums[right]);
            // max - min > limit → 收缩左边界
            while (*window.rbegin() - *window.begin() > limit) {
                // 关键：只删除一个 nums[left]，不是所有同值元素
                window.erase(window.find(nums[left]));
                left++;
            }
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};

// ============================================================
// 解法3: 堆 + 懒删除 — 大顶堆取 max，小顶堆取 min
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 另一种维护窗口极值的方式：
//   大顶堆 → 堆顶是窗口最大值
//   小顶堆 → 堆顶是窗口最小值
//
// 堆的问题：不支持删除中间元素。
// 解决方案：懒删除 — 堆顶的索引如果 < left，说明已过期，弹掉。
// 存 pair<值, 索引>，每次用堆顶前先检查索引是否在窗口内。
// ============================================================
class Solution3 {
public:
    int longestSubarray(vector<int>& nums, int limit) {
        // pair<值, 索引>
        priority_queue<pair<int,int>> maxH;                                    // 大顶堆
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> minH;  // 小顶堆
        int left = 0, ans = 0;
        for (int right = 0; right < (int)nums.size(); right++) {
            maxH.push({nums[right], right});
            minH.push({nums[right], right});
            // 窗口不合法 → 收缩
            while (maxH.top().first - minH.top().first > limit) {
                left++;
                // 懒删除：弹掉已不在窗口内的堆顶
                while (maxH.top().second < left) maxH.pop();
                while (minH.top().second < left) minH.pop();
            }
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};

// ============================================================
// 解法4: 单调队列（Monotonic Deque） — 均摊 O(1) 维护极值 ⭐
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 核心：用两个 deque 分别维护窗口的最大值和最小值。
//
// maxQ（递减队列）：
//   队首 = 窗口最大值。新元素入队时，弹出队尾所有 <= 它的。
//   为什么弹出？因为这些元素索引更小、值更小，不可能再当 max。
//
// minQ（递增队列）：
//   队首 = 窗口最小值。新元素入队时，弹出队尾所有 >= 它的。
//
// 窗口收缩：left++ 后，如果队首索引 < left，从队首弹出。
//
// 为什么 O(n)？每个元素最多入队1次、出队1次，总操作 ≤ 2n。
//
// 滑动窗口演示（nums = [10,1,2,4,7,2], limit = 5）：
//
// right=0: window=[10]     maxQ=[0]  minQ=[0]  diff=0  ✓ ans=1
// right=1: window=[10,1]   maxQ=[0,1] minQ=[1] diff=9  ✗
//          → left=1, maxQ front=0 < 1 弹出 → maxQ=[1]
//          window=[1]      diff=0  ✓ ans=1
// right=2: window=[1,2]    maxQ=[2]  minQ=[1,2] diff=1  ✓ ans=2
// right=3: window=[1,2,4]  maxQ=[3]  minQ=[1,2,3] diff=3  ✓ ans=3
// right=4: window=[1,2,4,7] maxQ=[4] minQ=[1,2,3,4] diff=6 ✗
//          → left=2, minQ front=1 < 2 弹出 → minQ=[2,3,4]
//          window=[2,4,7]  diff=5  ✓ ans=3
// right=5: window=[2,4,7,2] maxQ=[4,5] minQ=[5]  diff=5  ✓ ans=4
// ============================================================
class Solution4 {
public:
    int longestSubarray(vector<int>& nums, int limit) {
        deque<int> maxQ, minQ; // 存索引，不存值
        int left = 0, ans = 0;
        
        for (int right = 0; right < (int)nums.size(); right++) {
            // 维护递减队列（队首最大）
            // 为什么弹 <=？因为值更小且索引更旧的元素永远不可能再当 max
            while (!maxQ.empty() && nums[maxQ.back()] <= nums[right])
                maxQ.pop_back();
            maxQ.push_back(right);
            
            // 维护递增队列（队首最小）
            // 为什么弹 >=？因为值更大且索引更旧的元素永远不可能再当 min
            while (!minQ.empty() && nums[minQ.back()] >= nums[right])
                minQ.pop_back();
            minQ.push_back(right);
            
            // 条件不满足时收缩左边界
            while (nums[maxQ.front()] - nums[minQ.front()] > limit) {
                left++;
                // 队首过期则弹出（索引已不在窗口 [left, right] 内）
                if (maxQ.front() < left) maxQ.pop_front();
                if (minQ.front() < left) minQ.pop_front();
            }
            
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间        | 空间 | 核心数据结构       | 优势               |
// |-----------|------------|------|-------------------|-------------------|
// | 暴力       | O(n²)      | O(1) | 无                | 简单直觉            |
// | multiset  | O(n log n) | O(n) | 红黑树(multiset)   | 代码最短            |
// | 堆+懒删除  | O(n log n) | O(n) | priority_queue    | 展示懒删除技巧      |
// | 单调队列   | O(n)       | O(n) | deque             | 时间最优，面试首选   |
//
// 面试建议：首选解法4（单调队列），体现对滑动窗口+单调队列的深入理解。
// 如果紧张写不出单调队列，解法2（multiset）代码短、不容易出错，也完全可以。
//
// ============================================================
// 【易错点】
//
// 1. multiset 删除：
//    ✗ window.erase(nums[left])          → 删除所有等于 nums[left] 的元素
//    ✓ window.erase(window.find(nums[left])) → 只删除一个
//
// 2. 单调队列方向搞反：
//    ✗ maxQ 入队时弹 >= nums[right] 的  → 变成递增队列，队首是最小值
//    ✓ maxQ 入队时弹 <= nums[right] 的  → 递减队列，队首是最大值
//
// 3. 队首过期检查遗漏：
//    ✗ left++ 后直接取 maxQ.front() 当 max
//    ✓ left++ 后先 if (maxQ.front() < left) maxQ.pop_front()
//
// 4. 存值 vs 存索引：
//    单调队列存索引更安全，可以直接判断 front() < left 来确认过期。
//    存值的话需要额外比较 front() == nums[left]，遇到重复值容易出错。
//
// 5. 堆解法收缩时的懒删除：
//    ✗ 只在外层 while 判断一次
//    ✓ 每次 left++ 后都要 while 循环清理两个堆顶
//
// ============================================================
// 【面试追问】
//
// Q1: 为什么 max-min <= limit 等价于任意两元素差 <= limit？
// → 窗口中任意两元素差 <= max-min（最大差一定出现在最大值和最小值之间）。
//    所以 max-min <= limit 保证了所有对的差 <= limit。
//
// Q2: 单调队列为什么均摊 O(1)？具体分析。
// → 每个元素最多入队1次（push_back），出队最多1次（pop_back 或 pop_front）。
//    n 个元素总共最多 2n 次操作，均摊到每个元素 O(1)。
//
// Q3: 如果窗口大小固定为 k，怎么改？
// → 去掉 while 条件判断，改成固定 left = right - k + 1。
//    其实就变成了 LC 239 滑动窗口最大值的问题。
//
// Q4: 如果要求的不是"最长"而是"数量"（满足条件的子数组个数）？
// → 滑动窗口不变。对于每个 right，所有 [i, right]（left <= i <= right）
//    都是合法子数组，贡献 right - left + 1 个。累加即可。
// ============================================================
