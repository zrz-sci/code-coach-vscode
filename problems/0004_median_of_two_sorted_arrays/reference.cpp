/*
 * LeetCode 4: 寻找两个正序数组的中位数 (Median of Two Sorted Arrays)
 * 
 * 【题目本质】
 * 在两个已排序数组的"虚拟合并"中，找到中位数。
 * 核心挑战：不能真的合并（那样是 O(m+n)），必须用二分思想达到 O(log)。
 *
 * 【解法总览】
 * 解法1: 合并排序       — O(m+n) / O(m+n)   — 最直觉，正确性基准
 * 解法2: 双指针不合并   — O(m+n) / O(1)     — 空间优化
 * 解法3: 找第K小        — O(log(m+n)) / O(log(m+n)) — 面试首选
 * 解法4: 划分数组       — O(log(min(m,n))) / O(1) — 最优解
 */

// ============================================================
// 解法1: 合并排序 — 合并两个有序数组后直接取中位数
// 时间: O(m+n)  空间: O(m+n)
// 
// 【思路】
// 最直觉的做法：既然要找中位数，那就先把两个数组合并成一个有序数组。
// 这就是归并排序中 merge 的过程：双指针分别指向两个数组头部，
// 每次取较小的放进新数组。合并完后直接按下标取中位数。
//
// 瓶颈：合并需要遍历所有元素，O(m+n)，不满足题目 O(log) 要求。
// 但这是面试中展示"我理解了题目"的第一步。
// ============================================================
class Solution1 {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        vector<int> merged;
        int i = 0, j = 0;
        int m = nums1.size(), n = nums2.size();
        
        // 归并两个有序数组
        while (i < m && j < n) {
            if (nums1[i] <= nums2[j]) {
                merged.push_back(nums1[i++]);
            } else {
                merged.push_back(nums2[j++]);
            }
        }
        while (i < m) merged.push_back(nums1[i++]);
        while (j < n) merged.push_back(nums2[j++]);
        
        // 直接按下标取中位数
        int total = m + n;
        if (total % 2 == 1) {
            return merged[total / 2];
        } else {
            return (merged[total / 2 - 1] + merged[total / 2]) / 2.0;
        }
    }
};

// ============================================================
// 解法2: 双指针不合并 — 只走到中间位置，不创建合并数组
// 时间: O(m+n)  空间: O(1)
//
// 【思路】
// 解法1 的空间瓶颈：创建了完整的合并数组，其实我们只需要中间的
// 1~2 个元素的值。用双指针模拟合并过程，只走 (m+n)/2 + 1 步，
// 记录最后两个值（处理偶数长度时需要两个中间元素的均值）。
// ============================================================
class Solution2 {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size();
        int total = m + n;
        int prev = 0, curr = 0; // 只需记录最近两个值
        int p1 = 0, p2 = 0;
        
        // 走 total/2 + 1 步（0-indexed 中位数在 total/2 位置）
        for (int count = 0; count <= total / 2; count++) {
            prev = curr; // prev 保存上一步的值，处理偶数情况
            // 从两个数组中取较小的（注意一个数组可能已走完）
            if (p1 < m && (p2 >= n || nums1[p1] <= nums2[p2])) {
                curr = nums1[p1++];
            } else {
                curr = nums2[p2++];
            }
        }
        
        if (total % 2 == 1) return curr;
        return (prev + curr) / 2.0;
    }
};

// ============================================================
// 解法3: 找第K小 — 每次排除 k/2 个元素，O(log(m+n))
// 时间: O(log(m+n))  空间: O(log(m+n)) [递归栈]
//
// 【思路】
// 中位数 = 第 k 小的元素，其中 k = (m+n+1)/2（奇数时），
// 或者第 k 和第 k+1 小的均值（偶数时）。
//
// 关键观察：两个有序数组各取前 k/2 个，比较末尾元素：
//   - 如果 nums1[k/2-1] < nums2[k/2-1]，
//     nums1 的前 k/2 个元素最多排在总体第 k-1 位（因为 nums2 也贡献了最多 k/2-1 个更小的），
//     所以它们不可能是第 k 小的，安全排除。
//   - 反之亦然。
//
// 每次排除 k/2 个，k 至少减半，所以 O(log k) = O(log(m+n))。
//
// 搜索空间收缩示例:
// nums1 = [1,3,5,7,9], nums2 = [2,4,6,8,10], 找第 k=5 小
//
// Round1: k=5, half=2
//   nums1[s1+1] = 3  vs  nums2[s2+1] = 4
//   3 < 4 → 排除 nums1 前 2 个 [1,3]，k=5-2=3
//   nums1 剩余: [5,7,9]
//
// Round2: k=3, half=1
//   nums1[s1+0] = 5  vs  nums2[s2+0] = 2
//   2 < 5 → 排除 nums2 前 1 个 [2]，k=3-1=2
//   nums2 剩余: [4,6,8,10]
//
// Round3: k=2, half=1
//   nums1[s1+0] = 5  vs  nums2[s2+0] = 4
//   4 < 5 → 排除 nums2 前 1 个 [4]，k=2-1=1
//   nums2 剩余: [6,8,10]
//
// Round4: k=1 → 返回 min(nums1[s1], nums2[s2]) = min(5,6) = 5 ✓
// 验证: 合并=[1,2,3,4,5,6,7,8,9,10], 第5小=5 ✓
// ============================================================
class Solution3 {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size();
        int total = m + n;
        
        if (total % 2 == 1) {
            // 奇数：找第 (total/2 + 1) 小
            return findKth(nums1, 0, nums2, 0, total / 2 + 1);
        } else {
            // 偶数：找第 total/2 和 total/2+1 小的均值
            return (findKth(nums1, 0, nums2, 0, total / 2) +
                    findKth(nums1, 0, nums2, 0, total / 2 + 1)) / 2.0;
        }
    }
    
private:
    // 在 nums1[s1:] 和 nums2[s2:] 中找第 k 小的元素（k 从 1 开始）
    int findKth(vector<int>& nums1, int s1, vector<int>& nums2, int s2, int k) {
        int m = nums1.size(), n = nums2.size();
        
        // 边界1: nums1 的所有元素已被排除，答案在 nums2 中
        if (s1 >= m) return nums2[s2 + k - 1];
        // 边界2: nums2 的所有元素已被排除，答案在 nums1 中
        if (s2 >= n) return nums1[s1 + k - 1];
        // 边界3: k=1，取两个数组当前首元素的较小值
        if (k == 1) return min(nums1[s1], nums2[s2]);
        
        int half = k / 2;
        
        // 各看前 half 个元素的末尾值
        // 如果剩余不足 half 个，设为 INT_MAX，保证排除另一边
        // （不足 half 个的那一边暂时不排除，因为可能包含答案）
        int val1 = (s1 + half - 1 < m) ? nums1[s1 + half - 1] : INT_MAX;
        int val2 = (s2 + half - 1 < n) ? nums2[s2 + half - 1] : INT_MAX;
        
        if (val1 <= val2) {
            // nums1 前 half 个不可能是第 k 小的，排除
            return findKth(nums1, s1 + half, nums2, s2, k - half);
        } else {
            // nums2 前 half 个不可能是第 k 小的，排除
            return findKth(nums1, s1, nums2, s2 + half, k - half);
        }
    }
};

// ============================================================
// 解法4: 划分数组（二分分割线）— 在较短数组上二分找分割位置
// 时间: O(log(min(m,n)))  空间: O(1)
//
// 【思路】
// 换一个角度：中位数把所有元素分成大小相等的左右两半。
// 
// 设 nums1 贡献 i 个元素到左半边，nums2 贡献 j 个，
// 则 i + j = half = (m+n+1)/2。
// j = half - i，所以 i 确定后 j 自动确定。
//
// 合法分割的条件：
//   nums1[i-1] <= nums2[j]  （nums1 左边最大 ≤ nums2 右边最小）
//   nums2[j-1] <= nums1[i]  （nums2 左边最大 ≤ nums1 右边最小）
//
// 在 nums1 上二分搜索 i ∈ [0, m]：
//   - 如果 nums1[i-1] > nums2[j]：i 太大，左移
//   - 如果 nums2[j-1] > nums1[i]：i 太小，右移
//   - 否则：找到合法分割
//
// 为什么在较短数组上二分？
// 因为 j = half - i，如果 nums1 更长（m > half），i 取大值时 j 会变负。
// 在较短数组上二分保证 0 <= i <= m <= half，从而 j >= 0。
//
// 图示：
//
//  nums1: [ a1, a2, ..., a(i-1) | a(i), ..., a(m-1) ]
//  nums2: [ b1, b2, ..., b(j-1) | b(j), ..., b(n-1) ]
//          ←—— 左半边 (共 half 个) ——→ ←—— 右半边 ——→
//
//  合法条件: a(i-1) <= b(j) 且 b(j-1) <= a(i)
//
//  中位数:
//    奇数: max(a(i-1), b(j-1))
//    偶数: (max(a(i-1), b(j-1)) + min(a(i), b(j))) / 2
//
// 二分过程示例:
// nums1 = [1,3], nums2 = [2]
// m=2, n=1, half=(2+1+1)/2=2
//
// lo=0, hi=2
// Round1: i=1, j=2-1=1
//   nums1Left=1, nums1Right=3, nums2Left=2, nums2Right=INT_MAX
//   1<=INT_MAX ✓, 2<=3 ✓ → 找到!
//   奇数: max(1,2) = 2 ✓
// ============================================================
class Solution4 {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        // 保证 nums1 是较短的数组
        if (nums1.size() > nums2.size()) {
            return findMedianSortedArrays(nums2, nums1);
        }
        
        int m = nums1.size(), n = nums2.size();
        // 左半边元素个数（+1 使奇数时左半多一个，中位数直接取左半最大值）
        int half = (m + n + 1) / 2;
        
        // 在 nums1 上二分: i ∈ [0, m]
        int lo = 0, hi = m;
        
        while (lo <= hi) {
            int i = lo + (hi - lo) / 2; // nums1 取前 i 个到左半边
            int j = half - i;           // nums2 取前 j 个（自动确定）
            
            // 四个关键边界值
            // i=0: nums1 不贡献左半边 → 左边"不存在" → 用 INT_MIN（不影响 max）
            // i=m: nums1 全在左半边 → 右边"不存在" → 用 INT_MAX（不影响 min）
            int nums1Left  = (i == 0) ? INT_MIN : nums1[i - 1];
            int nums1Right = (i == m) ? INT_MAX : nums1[i];
            int nums2Left  = (j == 0) ? INT_MIN : nums2[j - 1];
            int nums2Right = (j == n) ? INT_MAX : nums2[j];
            
            if (nums1Left <= nums2Right && nums2Left <= nums1Right) {
                // 合法分割，直接算中位数
                if ((m + n) % 2 == 1) {
                    // 奇数：左半最大值就是中位数
                    return (double)max(nums1Left, nums2Left);
                } else {
                    // 偶数：左半最大 + 右半最小 的均值
                    return (max(nums1Left, nums2Left) + 
                            min(nums1Right, nums2Right)) / 2.0;
                }
            } else if (nums1Left > nums2Right) {
                // nums1 左侧最大值太大，说明 i 取多了，左移
                hi = i - 1;
            } else {
                // nums2Left > nums1Right
                // nums2 左侧最大值太大，说明 i 取少了，右移
                lo = i + 1;
            }
        }
        
        return -1.0; // 题目保证有解，不会到这里
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 vs 解法2:
//   都是 O(m+n) 时间，但解法2 节省 O(m+n) 空间。
//   本质一样，只是解法2 不存储完整合并数组。
//
// 解法3 vs 解法4:
//   两者都是 O(log) 级别，但角度不同：
//   - 解法3 把问题转化为"找第 k 小"，每次排除 k/2 个元素。
//     思路直观，递归实现，适合面试现场写。
//   - 解法4 直接在短数组上二分找分割线，O(log(min(m,n)))。
//     理论最优，但边界条件多，面试时容易写错。
//
//   当 m 和 n 差距很大时（如 m=10, n=10^6），
//   解法4 显著更快（log(10) vs log(10^6)）。
//
// 面试策略:
//   1. 先说解法1 展示理解
//   2. 写解法3（找第K小），代码清晰，不易出错
//   3. 如果面试官追问最优解或面试时间充裕，说解法4
//
// 【易错点】
//
// 1. findKth 中数组剩余不足 k/2 个时直接访问越界:
//    ✗ int val1 = nums1[s1 + half - 1];  // s1 + half - 1 可能 >= m
//    ✓ int val1 = (s1 + half - 1 < m) ? nums1[s1 + half - 1] : INT_MAX;
//    为什么设 INT_MAX: 保证不足的那一侧不被排除（因为它可能包含答案）
//
// 2. 划分法忘记让 nums1 是较短数组:
//    ✗ 直接二分 i ∈ [0, m]，当 m > half 时 j = half - i < 0
//    ✓ 开头加: if (m > n) return findMedianSortedArrays(nums2, nums1);
//
// 3. 奇偶混淆（找第K小法）:
//    ✗ return findKth(nums1, 0, nums2, 0, total / 2);  // 当 total=5 时，这找的是第2小
//    ✓ 奇数时找 total/2 + 1（k 是 1-indexed）
//
// 4. 划分法的 half 用 (m+n)/2 而非 (m+n+1)/2:
//    ✗ int half = (m + n) / 2;  // 奇数时左半少一个，后面的奇数返回逻辑不对
//    ✓ int half = (m + n + 1) / 2;  // 保证奇数时左半多一个，中位数 = max(左半)
//
// 5. 返回类型忘记转 double:
//    ✗ return (max(a, b) + min(c, d)) / 2;  // 整数除法，截断小数
//    ✓ return (max(a, b) + min(c, d)) / 2.0;
//
// 【面试追问】
//
// Q1: "先说一下暴力做法？"
// → 合并两个有序数组 O(m+n)，取 arr[(m+n)/2]。空间 O(m+n)。
//    不满足 O(log) 要求，但说明了对中位数定义的理解。
//
// Q2: "怎么优化到 O(log(m+n))？"
// → 找第 k 小：中位数 = 第 (m+n+1)/2 小。
//    两个数组各看前 k/2 个，较小端全部排除，k 减半。
//
