/*
 * LeetCode 1428: 至少有一个 1 的最左端列 (Leftmost Column with at Least a One)
 * 
 * 【题目本质】
 * 在一个每行非递减排序的 0/1 矩阵中，通过有限次 API 调用找到包含 1 的最左列。
 * 核心：利用"行有序"性质减少查询次数。
 *
 * 【解法总览】
 * 解法1: 暴力逐列扫描    — O(R*C) / O(1) — 最直觉，但可能超调用限制
 * 解法2: 每行二分查找     — O(R*logC) / O(1) — 面试首选 ⭐
 * 解法2b: 二分+剪枝优化   — O(R*logC) / O(1) — 解法2的优化版
 * 解法3: 右上角线性遍历   — O(R+C) / O(1) — 最优，加分项
 */

// ============================================================
// 解法1: 暴力逐列扫描 — 从左到右逐列检查
// 时间: O(R*C)  空间: O(1)
//
// 【思路】
// 最直觉的方法：从左到右逐列检查，每列遍历所有行看是否有1。
// 第一个包含1的列就是答案。
// 
// 为什么不够好？最坏情况 100*100=10000 次调用，超过 1000 次限制。
// 瓶颈：没有利用"每行有序"的性质。
// ============================================================
class Solution1 {
public:
    int leftMostColumnWithOne(BinaryMatrix &binaryMatrix) {
        vector<int> dim = binaryMatrix.dimensions();
        int rows = dim[0], cols = dim[1];
        
        for (int col = 0; col < cols; col++) {
            for (int row = 0; row < rows; row++) {
                if (binaryMatrix.get(row, col) == 1) {
                    return col;
                }
            }
        }
        return -1;
    }
};

// ============================================================
// 解法2: 每行二分查找 — 利用行有序性
// 时间: O(R * logC)  空间: O(1)
//
// 【思路】
// 每行是 0...01...1 形态，找"第一个1"就是二分查找左边界。
// 对每行二分找到第一个1的列索引，取所有行的最小值。
// 
// 调用次数：100 行 × log₂(100) ≈ 700 次，远小于 1000。
//
// 二分找左边界示意 (row = [0,0,1,1]):
//   Round1: [0, 0, 1, 1]  lo=0, hi=3, mid=1, val=0 → lo=2
//               ^
//   Round2: [0, 0, 1, 1]  lo=2, hi=3, mid=2, val=1 → hi=2
//                     ^
//   lo==hi==2 → 第一个1在col=2
// ============================================================
class Solution2 {
public:
    int leftMostColumnWithOne(BinaryMatrix &binaryMatrix) {
        vector<int> dim = binaryMatrix.dimensions();
        int rows = dim[0], cols = dim[1];
        int ans = -1;
        
        for (int row = 0; row < rows; row++) {
            // 对当前行二分，找第一个 1 的位置
            int lo = 0, hi = cols - 1;
            while (lo < hi) {
                int mid = lo + (hi - lo) / 2;
                if (binaryMatrix.get(row, mid) == 0) {
                    lo = mid + 1;  // mid处是0，第一个1一定在右边
                } else {
                    hi = mid;      // mid处是1，它可能就是第一个1，不能排除
                }
            }
            // lo == hi，检查这个位置是否真的是 1
            // （如果整行都是0，lo会停在cols-1，那里也是0）
            if (binaryMatrix.get(row, lo) == 1) {
                ans = (ans == -1) ? lo : min(ans, lo);
            }
        }
        return ans;
    }
};

// ============================================================
// 解法2b: 二分 + 剪枝优化 — 用已知答案缩小搜索范围
// 时间: O(R * logC) 最坏不变，但平均更快  空间: O(1)
//
// 【思路】
// 在解法2基础上，如果已经知道 ans=5，那后续行只需在 [0,4] 内搜索。
// 如果某行在 col=4 处是 0，说明该行第一个1在 >=5，不可能更优，直接跳过。
// 这一步优化让实际调用次数大幅减少。
// ============================================================
class Solution2b {
public:
    int leftMostColumnWithOne(BinaryMatrix &binaryMatrix) {
        vector<int> dim = binaryMatrix.dimensions();
        int rows = dim[0], cols = dim[1];
        int ans = cols;  // 用 cols 表示"还没找到"
        
        for (int row = 0; row < rows; row++) {
            // 搜索范围缩小到 [0, ans-1]
            if (ans == 0) break;  // 已经是最左列了，不可能更优
            
            int hi = ans - 1;
            // 快速检查：如果这行在 hi 位置是0，整行不可能比 ans 更优
            if (binaryMatrix.get(row, hi) == 0) continue;
            
            int lo = 0;
            while (lo < hi) {
                int mid = lo + (hi - lo) / 2;
                if (binaryMatrix.get(row, mid) == 0) {
                    lo = mid + 1;
                } else {
                    hi = mid;
                }
            }
            ans = lo;  // 更新答案（这里 lo 处一定是1，因为前面已检查过）
        }
        return ans == cols ? -1 : ans;
    }
};

// ============================================================
// 解法3: 右上角线性遍历 — 利用矩阵全局结构
// 时间: O(R + C)  空间: O(1)
//
// 【思路】
// 换一个视角：不要逐行独立处理，把矩阵看作整体。
// 
// 从右上角 (row=0, col=cols-1) 开始：
//   - 当前格 == 1 → 记录答案，向左移（尝试找更左的1）
//   - 当前格 == 0 → 向下移（本行左边也全是0，不可能更优）
//
// 为什么正确？每步要么排除当前行（下移），要么收窄答案上界（左移）。
// 不会漏解，因为每到一行时，col 已经是当前最优答案的位置，
// 只需检查这一行能否在更左的位置有1。
//
// 遍历示意:
//   col:  0 1 2 3
// row 0:  0 0 0 [1]  ← (0,3)=1, ans=3, 左移
//         0 0 [0] .  ← (0,2)=0, 下移  
// row 1:  0 0 [1] .  ← (1,2)=1, ans=2, 左移
//         0 [0] . .  ← (1,1)=0, 下移
// row 2:  0 [0] . .  ← (2,1)=0, 下移
// row 3:  0 [1] . .  ← (3,1)=1, ans=1, 左移
//        [0] . . .   ← (3,0)=0, 下移
// row=4 越界 → 返回 ans=1
//
// 总调用 7 次 ≤ R+C = 8
// ============================================================
class Solution3 {
public:
    int leftMostColumnWithOne(BinaryMatrix &binaryMatrix) {
        vector<int> dim = binaryMatrix.dimensions();
        int rows = dim[0], cols = dim[1];
        
        int row = 0, col = cols - 1;
        int ans = -1;
        
        while (row < rows && col >= 0) {
            if (binaryMatrix.get(row, col) == 1) {
                ans = col;  // 记录当前列，可能是最终答案
                col--;      // 尝试找更左的列
            } else {
                row++;      // 当前行不可能在更左的位置有1，换下一行
            }
        }
        return ans;
    }
};

// ============================================================
// 【解法对比】
// 
// | 解法      | 时间      | 空间  | API调用次数 | 核心思想           |
// |-----------|-----------|-------|-------------|-------------------|
// | 暴力      | O(R*C)    | O(1)  | 最多10000   | 无优化，超限       |
// | 每行二分  | O(R*logC) | O(1)  | 最多~700    | 利用行有序性       |
// | 二分+剪枝 | O(R*logC) | O(1)  | 实际更少    | 用ans缩小搜索范围  |
// | 右上角    | O(R+C)    | O(1)  | 最多200     | 利用矩阵全局结构   |
//
// 面试中：
//   - 先说暴力 O(R*C) 超限 → 每行二分 O(R*logC) → 提到可以剪枝 → 
//     如果面试官追问最优 → 给出右上角 O(R+C) 
//
// 【易错点】
// 1. 二分时 hi = mid-1 vs hi = mid:
//    ✗ if (val==1) hi = mid-1;  // 可能跳过答案！mid可能就是第一个1
//    ✓ if (val==1) hi = mid;    // 保留mid在搜索范围内
//    配合 while(lo < hi) 使用，否则 while(lo <= hi) + hi=mid 会死循环
//
// 2. 二分后忘记验证:
//    ✗ ans = min(ans, lo);        // 如果整行都是0，lo停在末尾，那里也是0
//    ✓ if (get(row, lo) == 1) ... // 必须确认lo处确实是1
//
// 3. 右上角遍历忘记记录ans:
//    ✗ if (val==1) col--;         // 只左移了但没记录！
//    ✓ if (val==1) { ans = col; col--; }
//
// 4. dimensions()返回值搞反:
//    dim[0]=rows, dim[1]=cols，别弄反了
//
// 【面试追问】
// Q1(基础): 暴力解的调用次数是多少？为什么会超限？
//   → R*C最多10000，限制是1000。瓶颈是没利用行有序性做了线性扫描。
//
// Q2(优化): 二分解法能进一步减少调用吗？
//   → 用当前ans缩小搜索范围。如果某行在ans-1处是0，一次调用就跳过。
//
// Q3(最优): 右上角遍历为什么不会漏解？为什么恰好走R+C步？
//   → 每步要么row+1(排除该行)要么col-1(收窄答案)，两个维度独立递增/递减，
//     总步数=行数+列数。不漏解因为：遇1左移找更优，遇0下移排除不可能行。
//
// Q4(变体): 如果矩阵不是0/1而是每行非递减整数，找最左包含target的列？
//   → 对每行二分 lower_bound(target)；右上角遍历改为 >= target 左移，< target 下移。
// ============================================================
