/*
 * LeetCode 992: K 个不同整数的子数组 (Subarrays with K Different Integers)
 *
 * 【题目本质】
 * 统计数组中恰好包含 k 种不同整数的连续子数组个数。
 * 核心难点："恰好 k 个"不直接适用滑动窗口，需要转化为"最多 k 个"的差值。
 *
 * 【解法总览】
 * 解法1: 暴力枚举         — O(n²) / O(n) — 最直觉，但可能超时
 * 解法2: atMost 差值法     — O(n) / O(n)  — 经典转化，面试首选 ⭐
 * 解法3: 三指针法          — O(n) / O(n)  — 一次遍历，加分项
 */

// ============================================================
// 解法1: 暴力枚举 — 枚举所有子数组，哈希表计数
// 时间: O(n²)  空间: O(n)
//
// 【思路】
// 最直觉的做法：固定左端点 i，右端点 j 从 i 向右扩展。
// 用哈希表维护 [i, j] 窗口内每个数出现的次数，同时维护 distinct 计数。
// 当 distinct == k 时计入答案；> k 时 break（继续扩大只会让种类更多）。
//
// 瓶颈：O(n²) 个子数组，n = 2×10⁴ 时约 4×10⁸ 次操作，大概率 TLE。
// ============================================================
class Solution1 {
public:
    int subarraysWithKDistinct(vector<int>& nums, int k) {
        int n = nums.size(), count = 0;
        for (int i = 0; i < n; i++) {
            unordered_map<int, int> freq;
            int distinct = 0;
            for (int j = i; j < n; j++) {
                // 新元素入窗：如果之前出现次数为 0，说明是新种类
                if (freq[nums[j]]++ == 0) distinct++;
                if (distinct == k) count++;
                // 种类 > k 后继续扩大只会 ≥ k+1，不可能回到 k，直接剪枝
                if (distinct > k) break;
            }
        }
        return count;
    }
};

// ============================================================
// 解法2: atMost 差值法 — exactly(k) = atMost(k) - atMost(k-1)
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 为什么想到这个？
// 1. 子数组 + 条件约束 → 滑动窗口
// 2. "恰好 k 个"不具有滑窗所需的单调性（窗口扩大时种类可增可不变）
// 3. 但"最多 k 个"有单调性：种类 > k 时必须缩左指针
// 4. 关键等式：exactly(k) = atMost(k) - atMost(k-1)
//    因为 atMost(k) 包含 exactly(0) + exactly(1) + ... + exactly(k)
//    减去 atMost(k-1) = exactly(0) + ... + exactly(k-1)
//    差值就是 exactly(k)
//
// atMost(K) 的滑窗过程：
//   nums = [1, 2, 1, 2, 3], K = 2
//
//   right=0: window=[1]         distinct=1 ≤ 2, left=0, 贡献=0-0+1=1
//   right=1: window=[1,2]       distinct=2 ≤ 2, left=0, 贡献=1-0+1=2
//   right=2: window=[1,2,1]     distinct=2 ≤ 2, left=0, 贡献=2-0+1=3
//   right=3: window=[1,2,1,2]   distinct=2 ≤ 2, left=0, 贡献=3-0+1=4
//   right=4: window=[1,2,1,2,3] distinct=3 > 2 → 缩左
//            移除1→[2,1,2,3] distinct=3 > 2 → 继续缩
//            移除2→[1,2,3]   distinct=3 > 2 → 继续缩
//            移除1→[2,3]     distinct=2 ≤ 2, left=3, 贡献=4-3+1=2
//   atMost(2) = 1+2+3+4+2 = 12
//
//   atMost(1) = 1+1+1+1+1 = 5  (每个右端点最多贡献连续相同元素的长度)
//
//   exactly(2) = 12 - 5 = 7 ✓
// ============================================================
class Solution2 {
public:
    int subarraysWithKDistinct(vector<int>& nums, int k) {
        return atMost(nums, k) - atMost(nums, k - 1);
    }

private:
    int atMost(vector<int>& nums, int k) {
        int n = nums.size(), left = 0, result = 0;
        unordered_map<int, int> freq;
        int distinct = 0;

        for (int right = 0; right < n; right++) {
            // 右指针扩展：新元素入窗
            // freq[x]++ == 0 表示 x 之前不在窗口中，是新种类
            if (freq[nums[right]]++ == 0) distinct++;

            // 种类超过 k 时，左指针收缩
            while (distinct > k) {
                // 移除左端元素，如果计数降为 0，种类减一
                if (--freq[nums[left]] == 0) distinct--;
                left++;
            }

            // 关键：以 right 为右端点的合法子数组个数
            // 左端点可以是 left, left+1, ..., right，共 right-left+1 个
            // 这些子数组的不同数个数都 ≤ k（因为窗口越小种类越少）
            result += right - left + 1;
        }
        return result;
    }
};

// ============================================================
// 解法3: 三指针法 — 一次遍历，两个左指针
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 解法2 调用了两次 atMost，虽然总体 O(n)，但遍历了两次数组。
// 能否一次遍历？可以！用两个左指针：
//   - left1: [left1, right] 内不同数 ≤ k（即 atMost(k) 的左边界）
//   - left2: [left2, right] 内不同数 ≤ k-1（即 atMost(k-1) 的左边界）
// 
// 对于每个 right：
//   以 right 结尾、不同数 ≤ k 的子数组起点范围: [left1, right]
//   以 right 结尾、不同数 ≤ k-1 的子数组起点范围: [left2, right]
//   差值 = left2 - left1 就是恰好 k 种的子数组个数
//
// 示例: nums = [1, 2, 1, 2, 3], k = 2
//
//   right=0 (nums[right]=1):
//     left1=0, distinct1=1 ≤ 2     left2=0, distinct2=1 ≤ 1
//     贡献 = 0 - 0 = 0
//
//   right=1 (nums[right]=2):
//     left1=0, distinct1=2 ≤ 2     left2=2, distinct2=0 ≤ 1 (缩过了1和2)
//     贡献 = 2 - 0 = 2  → [1,2], [2]... 等等
//     实际: left2缩到2, 窗口[1,2]有2种>1, 缩left2→窗口[2]有1种>1? 不对
//     重新: left2从0开始, freq2加入2→distinct2=2>1, 缩: 移除1→distinct2=1≤1, left2=1
//            还是>1? freq2={2:1}, distinct2=1 ≤ 1 ✓, left2=1
//     贡献 = 1 - 0 = 1  → [1,2] 恰好2种 ✓
//
//   right=4 (nums[right]=3):
//     left1=3 (窗口[2,3], 2种≤2)
//     left2=5? 不对, left2=4 (窗口[3], 1种≤1)
//     实际 left2 缩到使得 distinct2 ≤ 1:
//       窗口需要只有1种, 即 [3], left2=4
//     贡献 = 4 - 3 = 1  → [2,3] 恰好2种 ✓
// ============================================================
class Solution3 {
public:
    int subarraysWithKDistinct(vector<int>& nums, int k) {
        int n = nums.size();
        unordered_map<int, int> freq1, freq2;
        int left1 = 0, left2 = 0;
        int distinct1 = 0, distinct2 = 0;
        int result = 0;

        for (int right = 0; right < n; right++) {
            // 两个窗口同时扩展右端
            if (freq1[nums[right]]++ == 0) distinct1++;
            if (freq2[nums[right]]++ == 0) distinct2++;

            // left1 维护 atMost(k) 的左边界
            while (distinct1 > k) {
                if (--freq1[nums[left1]] == 0) distinct1--;
                left1++;
            }

            // left2 维护 atMost(k-1) 的左边界
            while (distinct2 > k - 1) {
                if (--freq2[nums[left2]] == 0) distinct2--;
                left2++;
            }

            // left2 - left1 = 以 right 结尾、恰好 k 种不同数的子数组个数
            // 因为起点在 [left1, left2) 范围内都恰好有 k 种
            result += left2 - left1;
        }
        return result;
    }
};

// ============================================================
// 解法2 的数组优化版本（面试时常数更小）
// 由于 1 ≤ nums[i] ≤ n，可以用数组代替 unordered_map
// ============================================================
class Solution4 {
public:
    int subarraysWithKDistinct(vector<int>& nums, int k) {
        return atMost(nums, k) - atMost(nums, k - 1);
    }

private:
    int atMost(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> freq(n + 1, 0); // nums[i] ∈ [1, n]，用数组代替哈希表
        int left = 0, result = 0, distinct = 0;

        for (int right = 0; right < n; right++) {
            if (freq[nums[right]]++ == 0) distinct++;

            while (distinct > k) {
                if (--freq[nums[left]] == 0) distinct--;
                left++;
            }

            result += right - left + 1;
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 遍历次数 | 代码复杂度 | 推荐场景 |
// |------|------|------|----------|------------|----------|
// | 暴力 | O(n²)| O(n) | 1        | 简单       | 说思路   |
// | atMost| O(n)| O(n) | 2        | 中等       | ⭐面试首选|
// | 三指针| O(n)| O(n) | 1        | 较复杂     | 追问时用  |
// | 数组版| O(n)| O(n) | 2        | 中等       | 极致优化  |
//
// atMost 差值法是最通用的：exactly(k) = atMost(k) - atMost(k-1)
// 这个思路可以直接复用到 LC 930, LC 1248 等题目。
//
// 【易错点】
//
// 1. result += right - left + 1 写成 result += 1
//    ✗ result += 1;  // 只统计了窗口本身，漏掉了窗口内所有以right结尾的子数组
//    ✓ result += right - left + 1;  // left到right之间每个起点都是合法子数组
//
// 2. 哈希表计数减到0时没更新distinct
//    ✗ freq[nums[left]]--; left++;  // distinct永远不减，while变死循环或窗口过度收缩
//    ✓ if (--freq[nums[left]] == 0) distinct--; left++;
//
// 3. 三指针法共用一个哈希表
//    ✗ 用同一个freq → 两个left的状态互相干扰
//    ✓ 必须维护独立的freq1和freq2
//
// 4. atMost(0) 时的行为
//    当 k=1 时调用 atMost(0)，while (distinct > 0) 会让left追上right，
//    每步贡献0，最终返回0。这是正确的，不需要特判 k <= 0。
//
// 【面试追问】
//
// Q1: 暴力解能过吗？
// → n=2×10⁴, O(n²)≈4×10⁸, 大概率TLE。说明理解题意即可，需要优化。
//
// Q2: 为什么"恰好k个"不能直接滑窗？
// → 种类=k时窗口可扩可缩（加入已有元素不改变种类数），没有单调性。
//    而"最多k个"有单调性：种类>k时必须缩，缩了种类只减不增。
//
// Q3: 能否只遍历一次？
// → 三指针法：同时维护两个左指针和两个哈希表，
//    差值 left2-left1 就是当前right的贡献。
//
// Q4: unordered_map 太慢怎么办？
// → 因为 1 ≤ nums[i] ≤ n，改用 vector<int> freq(n+1, 0) 替代哈希表，
//    O(1) 访问，常数因子小很多。（见 Solution4）
//
// Q5: 这个 atMost 差值技巧还能用在哪？
// → LC 930: 和相同的二元子数组 — atMost(goal) - atMost(goal-1)
//   LC 1248: 统计优美子数组 — 奇数个数恰好k个
//   通用模式：任何"恰好=k"的子数组计数都可以转化
// ============================================================
