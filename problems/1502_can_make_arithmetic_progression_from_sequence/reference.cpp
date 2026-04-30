/*
 * LeetCode 1502: 判断能否形成等差数列 (Can Make Arithmetic Progression From Sequence)
 *
 * 【题目本质】
 * 给定一组数，判断能否重新排列成等差数列。
 * 等差数列排序后相邻差恒定 → 本质是"排序后是否均匀分布"。
 *
 * 【解法总览】
 * 解法1: 排序 + 遍历      — O(n log n) / O(1) — 最直觉，面试首选
 * 解法2: 哈希集合验证      — O(n) / O(n)      — 不排序的线性做法
 * 解法3: 原地交换(桶思想)  — O(n) / O(1)      — 最优，面试加分项
 */

// ============================================================
// 解法1: 排序 + 遍历 — 最自然的思路
// 时间: O(n log n)  空间: O(1)（原地排序）
//
// 【思路】
// 等差数列排序后，相邻元素之差恒定。
// 所以：排序 → 算第一对差 → 遍历检查所有相邻差是否相同。
//
// 示例 arr = [3, 5, 1]:
//   排序: [1, 3, 5]
//   差值:   2  2    ← 全相等 → true
//
// 示例 arr = [1, 2, 4]:
//   排序: [1, 2, 4]
//   差值:   1  2    ← 不等 → false
// ============================================================
class Solution1 {
public:
    bool canMakeArithmeticProgression(vector<int>& arr) {
        sort(arr.begin(), arr.end());
        
        // 公差 = 排序后前两个元素之差
        int diff = arr[1] - arr[0];
        
        // 从第3个元素开始，检查每对相邻差是否等于公差
        for (int i = 2; i < (int)arr.size(); i++) {
            if (arr[i] - arr[i - 1] != diff) {
                return false;
            }
        }
        return true;
    }
};

// ============================================================
// 解法2: 哈希集合验证 — O(n) 时间，不需要排序
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 等差数列由 min, max, n 唯一确定:
//   公差 d = (max - min) / (n - 1)
//   期望元素: min, min+d, min+2d, ..., max
//
// 只需验证:
//   1. (max - min) 能被 (n - 1) 整除（否则公差不是整数）
//   2. 每个元素 x 满足 (x - min) % d == 0（是等差数列的一员）
//   3. 无重复元素（公差≠0时，等差数列每项唯一）
//
// 示例 arr = [3, 5, 1]:
//   min=1, max=5, n=3, d=(5-1)/(3-1)=2
//   检查: 3→(3-1)%2==0 ✓, 5→(5-1)%2==0 ✓, 1→(1-1)%2==0 ✓
//   无重复 → true
// ============================================================
class Solution2 {
public:
    bool canMakeArithmeticProgression(vector<int>& arr) {
        int n = arr.size();
        int minVal = *min_element(arr.begin(), arr.end());
        int maxVal = *max_element(arr.begin(), arr.end());
        
        // 特判: 所有元素相同 → 公差为0，必定是等差数列
        // 也避免了后面 d=0 导致的除零问题
        if (maxVal == minVal) return true;
        
        // 公差必须是整数
        if ((maxVal - minVal) % (n - 1) != 0) return false;
        
        int d = (maxVal - minVal) / (n - 1);
        
        unordered_set<int> seen;
        for (int x : arr) {
            // x 减去 min 必须是 d 的整数倍，否则 x 不属于这个等差数列
            if ((x - minVal) % d != 0) return false;
            // 重复检测: insert 返回 <iterator, bool>，bool 为 false 表示已存在
            if (!seen.insert(x).second) return false;
        }
        return true;
    }
};

// ============================================================
// 解法3: 原地交换（桶排序思想）— O(n) 时间 O(1) 空间
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 在解法2的基础上省掉哈希集合。
// 核心想法: 等差数列中，元素 x 应该排在位置 (x - min) / d。
// 我们把每个元素"交换"到它应该在的位置，类似桶排序。
// 如果最终每个位置都放了正确的值，就是等差数列。
//
// 示例 arr = [3, 5, 1], min=1, d=2:
//   i=0: arr[0]=3, target=(3-1)/2=1, swap → arr=[5,3,1]
//        arr[0]=5, target=(5-1)/2=2, swap → arr=[1,3,5]
//        arr[0]=1, target=(1-1)/2=0, 已就位! i++
//   i=1: arr[1]=3, target=1, 已就位! i++
//   i=2: arr[2]=5, target=2, 已就位! i++
//   全部就位 → true
// ============================================================
class Solution3 {
public:
    bool canMakeArithmeticProgression(vector<int>& arr) {
        int n = arr.size();
        int minVal = *min_element(arr.begin(), arr.end());
        int maxVal = *max_element(arr.begin(), arr.end());
        
        // 全部相同 → 等差数列（公差0）
        if (maxVal == minVal) return true;
        // 公差必须整除
        if ((maxVal - minVal) % (n - 1) != 0) return false;
        
        int d = (maxVal - minVal) / (n - 1);
        
        for (int i = 0; i < n; ) {
            // 算出当前元素应该在的位置
            if ((arr[i] - minVal) % d != 0) return false; // 不是 d 的倍数
            
            int targetIdx = (arr[i] - minVal) / d;
            if (targetIdx < 0 || targetIdx >= n) return false; // 越界
            
            if (targetIdx == i) {
                // 已在正确位置，处理下一个
                i++;
            } else if (arr[targetIdx] == arr[i]) {
                // 目标位置已有相同值 → 出现重复，不可能是等差数列
                return false;
            } else {
                // 交换到目标位置，不递增 i（交换来的新值还需处理）
                swap(arr[i], arr[targetIdx]);
            }
        }
        return true;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间        | 空间   | 改原数组 | 代码复杂度 |
// |---------------|-------------|--------|----------|------------|
// | 排序+遍历     | O(n log n)  | O(1)*  | 是(排序) | 极简       |
// | 哈希集合      | O(n)        | O(n)   | 否       | 中等       |
// | 原地交换      | O(n)        | O(1)   | 是(交换) | 较高       |
//
// 面试策略: 先写排序解法(30秒)，再口述O(n)思路。
//
// 【易错点】
// 1. 哈希解法中 d=0 导致除零:
//    ✗ 直接算 (x - minVal) % d → 当 maxVal==minVal 时 d=0, 运行时错误
//    ✓ 先特判 maxVal == minVal 返回 true
//
// 2. 哈希解法忘记判断整除:
//    ✗ d = (maxVal - minVal) / (n - 1) 直接用 → 整数截断丢失精度
//    ✓ 先判 (maxVal - minVal) % (n - 1) != 0 则 return false
//
// 3. 原地交换不检测重复导致死循环:
//    ✗ arr[i] 和 arr[targetIdx] 相等时不断交换，i 永远不递增
//    ✓ 加 arr[targetIdx] == arr[i] 判断，发现重复直接 return false
//
// 4. 排序解法的循环起点:
//    ✗ 从 i=1 开始，用 arr[i]-arr[i-1] != arr[1]-arr[0]，
//       但 i=1 时永远自比自等，漏掉第一对的检查
//    ✓ diff 提前算好，循环从 i=2 开始
//
// 【面试追问】
// Q1(基础): 最简单的做法是什么？复杂度？
//    → 排序后检查相邻差，O(n log n) 时间 O(1) 空间。
//
// Q2(优化): 能否 O(n) 时间？
//    → 用 min/max 算公差，哈希集合验证。O(n) 时间 O(n) 空间。
//
// Q3(进阶): O(n) 时间 + O(1) 空间？
//    → 原地交换，桶排序思想，每个元素放到 (x-min)/d 位置。
//
// Q4(变体): 不允许修改原数组呢？
//    → 不修改 + O(1)空间 + O(n)时间三者不可兼得，必须在空间或时间上妥协。
// ============================================================
