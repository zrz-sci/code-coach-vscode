/*
 * LCR 079: 子集 (同主站 78. Subsets)
 * 难度: Medium
 *
 * 题目: 给定元素互不相同的整数数组 nums, 返回所有可能的子集(幂集)
 * 约束: 1 <= nums.length <= 10, 元素互不相同
 *
 * 三种经典解法: 回溯法 / 二进制枚举 / 迭代法
 * 时间复杂度均为 O(n * 2^n)
 */



// =====================================================
// 解法一: 回溯法 (最经典, 面试推荐)
// =====================================================
// 思路: 在每个递归节点收集当前路径, 选择当前元素后递归再撤销
//
// 决策树 (nums = [1, 2, 3]):
//
//                      bt(0, [])
//                      收集 []
//               ┌────────┼────────┐
//           i=0 │    i=1 │    i=2 │
//           选1 │    选2 │    选3 │
//               ▼        ▼        ▼
//        bt(1,[1])  bt(2,[2])  bt(3,[3])
//        收集[1]    收集[2]    收集[3]
//        ┌────┐        │
//    i=1 │i=2 │    i=2 │
//    选2 │选3 │    选3 │
//        ▼    ▼        ▼
//  bt(2,[1,2]) bt(3,[1,3]) bt(3,[2,3])
//  收集[1,2]   收集[1,3]   收集[2,3]
//     │
// i=2 │选3
//     ▼
// bt(3,[1,2,3])
// 收集[1,2,3]
//
// 收集顺序: [], [1], [1,2], [1,2,3], [1,3], [2], [2,3], [3]
// 共 8 = 2^3 个子集 ✓
//
// 时间: O(n * 2^n)  空间: O(n) 递归栈

class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(nums, 0, path, result);
        return result;
    }

private:
    void backtrack(vector<int>& nums, int start,
                   vector<int>& path, vector<vector<int>>& result) {
        // 每个节点都收集 (子集问题特点: 不只收集叶子!)
        result.push_back(path);

        // 从 start 开始选择, 保证只向后选 (避免重复)
        for (int i = start; i < (int)nums.size(); i++) {
            path.push_back(nums[i]);               // 做选择
            backtrack(nums, i + 1, path, result);  // 递归 (i+1 避免重复)
            path.pop_back();                        // 撤销选择 (回溯!)
        }
    }
};

/*
 * 回溯 trace (nums = [1,2,3]):
 *
 * bt(0,[])        → 收集 []
 *   push(1) → bt(1,[1])     → 收集 [1]
 *     push(2) → bt(2,[1,2]) → 收集 [1,2]
 *       push(3) → bt(3,[1,2,3]) → 收集 [1,2,3], return
 *       pop(3) → path=[1,2]
 *     pop(2) → path=[1]               ← 回溯!
 *     push(3) → bt(3,[1,3]) → 收集 [1,3], return
 *     pop(3) → path=[1]
 *   pop(1) → path=[]                  ← 回溯!
 *   push(2) → bt(2,[2])     → 收集 [2]
 *     push(3) → bt(3,[2,3]) → 收集 [2,3], return
 *     pop(3) → path=[2]
 *   pop(2) → path=[]                  ← 回溯!
 *   push(3) → bt(3,[3])     → 收集 [3], return
 *   pop(3) → path=[]
 *
 * 最终: [[], [1], [1,2], [1,2,3], [1,3], [2], [2,3], [3]]
 */


// =====================================================
// 解法二: 二进制枚举法
// =====================================================
// 思路: n 个元素有 2^n 个子集, 用 0~2^n-1 的二进制位表示选/不选
//
// 枚举表 (nums = [1, 2, 3]):
//   mask  二进制  bit0 bit1 bit2  子集
//    0     000     0    0    0    []
//    1     001     1    0    0    [1]
//    2     010     0    1    0    [2]
//    3     011     1    1    0    [1,2]
//    4     100     0    0    1    [3]
//    5     101     1    0    1    [1,3]
//    6     110     0    1    1    [2,3]
//    7     111     1    1    1    [1,2,3]
//
// 时间: O(n * 2^n)  空间: O(1) (不计输出)

class Solution2 {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        int n = nums.size();
        int total = 1 << n;  // 2^n
        vector<vector<int>> result;

        for (int mask = 0; mask < total; mask++) {
            vector<int> subset;
            for (int i = 0; i < n; i++) {
                // 第 i 位为 1 → 选择 nums[i]
                if (mask & (1 << i)) {
                    subset.push_back(nums[i]);
                }
            }
            result.push_back(subset);
        }

        return result;
    }
};


// =====================================================
// 解法三: 迭代法 (逐步构建)
// =====================================================
// 思路: 从空集开始, 每个新元素让已有子集数量翻倍
//
// 构建过程:
//   初始:     [[]]                          (1个)
//   加入 1:   [[], [1]]                     (2个)
//   加入 2:   [[], [1], [2], [1,2]]         (4个)
//   加入 3:   [[], [1], [2], [1,2],
//              [3], [1,3], [2,3], [1,2,3]]  (8个)
//
// 时间: O(n * 2^n)  空间: O(1) (不计输出)

class Solution3 {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> result = {{}};  // 从空集开始

        for (int num : nums) {
            int size = result.size();
            // 把现有的每个子集复制一份, 加上当前元素
            for (int i = 0; i < size; i++) {
                vector<int> newSubset = result[i];  // 复制已有子集
                newSubset.push_back(num);            // 加入新元素
                result.push_back(newSubset);         // 放入结果
            }
        }

        return result;
    }
};


// =====================================================
// 扩展: 子集 II (有重复元素, 同 LC 90)
// =====================================================
// 与本题的区别: nums 可能有重复, 需要排序 + 跳过同层相同元素

class Solution_SubsetsII {
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        sort(nums.begin(), nums.end());  // 排序使相同元素相邻
        vector<vector<int>> result;
        vector<int> path;
        backtrack(nums, 0, path, result);
        return result;
    }

private:
    void backtrack(vector<int>& nums, int start,
                   vector<int>& path, vector<vector<int>>& result) {
        result.push_back(path);
        for (int i = start; i < (int)nums.size(); i++) {
            // 关键: 跳过同一层的重复元素
            if (i > start && nums[i] == nums[i - 1]) continue;
            path.push_back(nums[i]);
            backtrack(nums, i + 1, path, result);
            path.pop_back();
        }
    }
};


// =====================================================
// 复杂度与关联题目
// =====================================================
/*
 * 三种解法复杂度相同:
 *   时间: O(n * 2^n) — 2^n 个子集, 每个平均 O(n) 复制
 *   空间: O(n) — 递归栈 / 临时变量 (不计输出)
 *   子集总数: 2^n (n <= 10, 最多 1024 个)
 *
 * 方法对比:
 *   回溯法:     最经典, 面试首选, 可扩展到子集II/组合/排列
 *   二进制枚举: 代码最短, 适合 n 小时
 *   迭代法:     不用递归, 构建过程直观
 *
 * 面试追问:
 *   Q: 有重复元素? → LC90, 排序 + if(i>start && nums[i]==nums[i-1]) continue
 *   Q: 子集长度恰好 k? → LC77, 收集条件 path.size()==k, 超过 k 剪枝
 *   Q: 子集和恰好 target? → LC39/40, 累加 sum, sum>target 时剪枝
 *
 * 易错点:
 *   ✗ 只在叶子节点收集 → 子集要在每个节点收集, 不只是叶子
 *   ✗ push 了忘 pop → 回溯核心: 递归返回后必须撤销选择
 *   ✗ 收集引用而非拷贝 → Java: new ArrayList<>(path), Python: path[:]
 *   ✗ start 写成 0 → 每次从 0 开始会产生重复子集如 [1,2] 和 [2,1]
 *
 * 关联题目:
 *   77.  Combinations — 子集中固定长度 k 的 (组合)
 *   78.  Subsets — 本题主站版
 *   90.  Subsets II — 有重复元素的子集 (排序+跳重复)
 *   46.  Permutations — 全排列 (有序, 用 visited 数组)
 *   39.  Combination Sum — 组合总和 (元素可重复使用)
 *   40.  Combination Sum II — 组合总和 (有重复, 每个只用一次)
 */
