/*
 * LeetCode 349: 两个数组的交集 (Intersection of Two Arrays)
 *
 * 【题目本质】
 * 找出两个数组中共同出现的元素，结果去重。
 * 核心是一个"查找"问题：对一个数组中的元素，判断是否存在于另一个数组中。
 *
 * 【解法总览】
 * 解法1: 暴力双循环        — O(m×n) / O(min(m,n)) — 最直觉
 * 解法2: 哈希表(Set)       — O(m+n) / O(m)       — 面试首选 ⭐
 * 解法3: 排序+双指针       — O(m·logm+n·logn)/O(1)— 不用额外空间
 * 解法4: 排序+二分查找     — O((m+n)·logn)/O(m)   — 另一种思路
 */

// ============================================================
// 解法1: 暴力双循环
// 时间: O(m × n)  空间: O(min(m, n))
//
// 【思路】
// 最直觉的想法：对 nums1 中的每个元素，遍历整个 nums2 看是否存在。
// 用 set 存结果来自动去重。
// 瓶颈：每次查找是 O(n)，总共做 m 次 → O(m×n)。
// ============================================================
class Solution1 {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        unordered_set<int> resultSet;
        for (int x : nums1) {
            for (int y : nums2) {
                if (x == y) {
                    resultSet.insert(x); // set 自动去重
                    break;               // 找到匹配就不用继续了
                }
            }
        }
        return vector<int>(resultSet.begin(), resultSet.end());
    }
};

// ============================================================
// 解法2: 哈希表(Set) — 面试首选 ⭐
// 时间: O(m + n)  空间: O(m)
//
// 【思路】
// 解法1的瓶颈是"在 nums2 中查找"需要 O(n)。
// 用 unordered_set 存储一个数组，查找从 O(n) 降到 O(1)。
// 
// 步骤：
// 1. 把 nums1 放入 set1（去重 + 快速查找）
// 2. 遍历 nums2，每个元素在 set1 中 O(1) 查找
// 3. 找到的放入结果 set（自动去重）
//
// 优化：把较小的数组放入 set，减少空间开销。
// ============================================================
class Solution2 {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        // 让 nums1 是较小的数组，减少 set 大小
        if (nums1.size() > nums2.size()) return intersection(nums2, nums1);
        
        // 把较小的数组放入 set
        unordered_set<int> set1(nums1.begin(), nums1.end());
        unordered_set<int> resultSet;
        
        for (int x : nums2) {
            if (set1.count(x)) {      // O(1) 查找，这就是优化的关键！
                resultSet.insert(x);   // set 自动去重
            }
        }
        
        return vector<int>(resultSet.begin(), resultSet.end());
    }
};

// ============================================================
// 解法3: 排序 + 双指针
// 时间: O(m·log m + n·log n)  空间: O(1)（不计输出）
//
// 【思路】
// 面试追问"能否不用额外空间"时的解法。
// 两个数组都排序后，用双指针同步扫描，类似归并排序的 merge 步骤。
//
// 为什么双指针不会漏解？
// 两个数组都有序，如果 nums1[i] < nums2[j]，
// 那 nums1[i] 不可能等于 nums2[j] 之后的任何元素（后面更大），
// 所以 i 可以安全前进。
//
// 示例演示：
// 排序后: nums1 = [4, 5, 9],  nums2 = [4, 4, 8, 9, 9]
//                  i                    j
// Step1: 4==4 → 加入[4],     i++, j++
// Step2: 5>4  →              j++
// Step3: 5<8  → i++
// Step4: 9>8  →              j++
// Step5: 9==9 → 加入[4,9],   i++, j++
// 结束! 结果=[4,9]
// ============================================================
class Solution3 {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        sort(nums1.begin(), nums1.end());
        sort(nums2.begin(), nums2.end());
        
        vector<int> result;
        int i = 0, j = 0;
        
        while (i < (int)nums1.size() && j < (int)nums2.size()) {
            if (nums1[i] < nums2[j]) {
                i++;                   // nums1 当前值小，追赶 nums2
            } else if (nums1[i] > nums2[j]) {
                j++;                   // nums2 当前值小，追赶 nums1
            } else {
                // 相等 → 交集元素，但需要去重
                // 为什么要检查 result.empty()？因为空 vector 调 back() 是 UB
                if (result.empty() || result.back() != nums1[i]) {
                    result.push_back(nums1[i]);
                }
                i++;
                j++;
            }
        }
        
        return result;
    }
};

// ============================================================
// 解法4: 排序 + 二分查找
// 时间: O((m + n) · log n)  空间: O(m)（用于 nums1 去重）
//
// 【思路】
// 只排序一个数组(nums2)，然后对另一个数组(nums1)的每个不重复元素，
// 在已排序的 nums2 中进行二分查找。
//
// 搜索空间收缩示例：
// 在 nums2 = [4, 4, 8, 9, 9] 中查找 5:
// Round1: [4, 4, 8, 9, 9]  mid=8 > 5  → right=mid-1
//                  ↑
// Round2: [4, 4]  mid=4 < 5  → left=mid+1
//          ↑
// Round3: left > right → 不存在!
//
// 什么时候选这个？
// 当 nums2 非常大但 nums1 很小时，排序 nums2 后每次 O(log n) 查找
// 可能比哈希表更缓存友好。
// ============================================================
class Solution4 {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        // 先对 nums1 去重，避免重复查找
        unordered_set<int> set1(nums1.begin(), nums1.end());
        // 排序 nums2 使其支持二分查找
        sort(nums2.begin(), nums2.end());
        
        vector<int> result;
        for (int x : set1) {
            // STL 的 binary_search 返回 bool，底层就是标准二分
            if (binary_search(nums2.begin(), nums2.end(), x)) {
                result.push_back(x);
            }
        }
        
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间              | 空间    | 核心操作         |
// |------------|-------------------|---------|------------------|
// | 暴力       | O(m×n)            | O(min)  | 线性查找         |
// | 哈希表 ⭐  | O(m+n)            | O(m)    | 哈希查找 O(1)    |
// | 排序+双指针| O(m·logm+n·logn)  | O(1)    | 双指针同步扫描   |
// | 排序+二分  | O((m+n)·logn)     | O(m)    | 二分查找 O(logn) |
//
// 面试推荐：先写解法2(哈希表)，被追问空间优化时给解法3(双指针)。
//
// 【易错点】
// 1. 双指针去重时忘记检查 result.empty()：
//    ✗ if (result.back() != nums1[i])  — 空 vector 调 back() 是 UB
//    ✓ if (result.empty() || result.back() != nums1[i])
//
// 2. 混淆 349 和 350：
//    ✗ 349 用 map 计数保留重复 — 不符合题意，349 要求结果唯一
//    ✓ 349 用 set 去重，350 才需要 map 计数
//
// 3. 哈希表解法中同时用两个 set 不如一个 set + 一个遍历：
//    ✗ 两个数组都建 set 再求交集 — 浪费空间
//    ✓ 只把一个放入 set，遍历另一个查找
//
// 【面试追问】
// Q1: "暴力能过吗？瓶颈在哪？"
//     → 数据 ≤ 1000，O(10^6) 能过。瓶颈在查找 O(n)，用 set 优化到 O(1)。
//
// Q2: "如果两个数组已经有序，最优做法？"
//     → 双指针 O(m+n) 时间 O(1) 空间，比哈希表更优。
//
// Q3: "如果 nums2 有几十亿元素，存不进内存怎么办？"
//     → 把较小的 nums1 放内存中的 set，分块读取 nums2 做查找。
//       或者对 nums2 外排序后用二分/双指针。
//
// Q4: "如果不要求去重(即 350 题)，代码怎么改？"
//     → set → map 计数，匹配时 count--。双指针去掉跳过重复逻辑。
//       具体改动: unordered_set<int> → unordered_map<int,int>，
//       查到后 if (map1[x]-- > 0) result.push_back(x);
//
// 【相关题型】
// - 350. 两个数组的交集 II: 不去重版本，set→map 计数
// - 1. 两数之和: 同样用"哈希表加速查找"，查找 target-num
// - 2540. 最小公共值: 两有序数组找最小公共元素，直接复用双指针
// ============================================================
