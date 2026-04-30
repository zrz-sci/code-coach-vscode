/*
 * LeetCode 378: Kth Smallest Element in a Sorted Matrix (有序矩阵中第K小的元素)
 * 难度：Medium | 标签：Array, Binary Search, Matrix, Sorting, Heap
 *
 * ============================================================
 * 题目描述：
 * 给定 n x n 矩阵，每行每列均按升序排列，找到矩阵中第 k 小的元素。
 * 是排序后的第 k 小元素，不是第 k 个不同元素。
 * 要求空间复杂度优于 O(n^2)。
 *
 * 约束：
 * - n == matrix.length == matrix[i].length
 * - 1 <= n <= 300
 * - -10^9 <= matrix[i][j] <= 10^9
 * - 行和列均非递减排列
 * - 1 <= k <= n^2
 *
 * Follow-up：O(1) 空间？O(n) 时间？
 * ============================================================
 *
 * 矩阵示例：
 *
 *      col 0  col 1  col 2
 * row 0: [ 1     5      9  ]   → 行内递增
 * row 1: [ 10    11    13  ]   → 行内递增
 * row 2: [ 12    13    15  ]   → 行内递增
 *           ↓      ↓     ↓
 *          列内递增
 *
 * 所有元素排序: [1, 5, 9, 10, 11, 12, 13, 13, 15]
 * k=8 → 第8小 = 13
 *
 * ============================================================
 */


/*
 * ============================================================
 * 解法一：二分搜索值域 + 左下角阶梯统计（面试首选）
 * ============================================================
 *
 * 核心思路：
 * 1. 在值域 [matrix[0][0], matrix[n-1][n-1]] 上做二分
 * 2. 对于每个 mid，统计矩阵中 <= mid 的元素个数
 * 3. 利用矩阵有序性，从左下角出发 O(n) 统计
 *
 * 搜索空间收缩过程（k=8）：
 *
 *   值域: [1 ━━━━━━━━━━━━━━━━━━━━━━ 15]
 *
 *   mid=8:  count(<=8)=2 < 8  → lo=9
 *           [9 ━━━━━━━━━━━━━━━━━ 15]
 *
 *   mid=12: count(<=12)=6 < 8 → lo=13
 *           [13 ━━━━━ 15]
 *
 *   mid=14: count(<=14)=8 >= 8 → hi=14
 *           [13 ━━ 14]
 *
 *   mid=13: count(<=13)=8 >= 8 → hi=13
 *           [13]
 *
 *   lo == hi == 13 → 答案
 *
 * 时间复杂度：O(n * log(max - min))
 *   - 二分次数: log(max - min)，最多约 60 次 (10^9 范围)
 *   - 每次统计: O(n)
 * 空间复杂度：O(1) — 满足 Follow-up 要求
 */
class Solution {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        int n = matrix.size();
        int lo = matrix[0][0], hi = matrix[n - 1][n - 1];

        // 二分搜索值域
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;  // 防溢出
            int count = countLessOrEqual(matrix, mid, n);

            if (count < k) {
                lo = mid + 1;  // 第k小的值一定 > mid
            } else {
                hi = mid;      // 第k小的值 <= mid，收缩右边界
            }
        }
        // 循环结束时 lo == hi，且 lo 一定是矩阵中存在的值
        return lo;
    }

private:
    /*
     * 从左下角出发，O(n) 统计矩阵中 <= mid 的元素个数
     *
     * 路径示意（统计 <= 13）：
     *
     *       c0   c1   c2
     *  r0 [  1    5    9 ]     count += 1 (col 2, row 0~0)
     *  r1 [ 10   11   13 ] ←  13<=13, count += 2 (col 2, row 0~1)
     *  r2 [ 12   13   15 ]    15>13, ↑上走
     *       ↑    →
     *      起点
     *
     * 实际走法：先从(2,0)开始
     *   (2,0)=12<=13 → count+=3, c=1
     *   (2,1)=13<=13 → count+=3, c=2
     *   (2,2)=15>13  → r=1
     *   (1,2)=13<=13 → count+=2, c=3 → 退出
     *   总 count = 3+3+2 = 8
     */
    int countLessOrEqual(vector<vector<int>>& matrix, int mid, int n) {
        int count = 0;
        int r = n - 1, c = 0;  // 从左下角开始

        while (r >= 0 && c < n) {
            if (matrix[r][c] <= mid) {
                // 这一列从 row 0 到 row r 都 <= mid
                count += r + 1;
                c++;   // 向右移动（看下一列）
            } else {
                r--;   // 向上移动（当前列还没找到 <= mid 的行）
            }
        }
        return count;
    }
};


/*
 * ============================================================
 * 解法二：最小堆 K 路归并
 * ============================================================
 *
 * 核心思路：
 * 把矩阵看作 n 条有序链表的合并问题。
 * 每行是一条有序链表，用最小堆维护当前 n 个行的最小元素。
 *
 * 堆的工作过程（k=8）：
 *
 * 初始堆: {1(0,0), 10(1,0), 12(2,0)}
 *
 * Pop  1(0,0) → push  5(0,1)    堆:[5, 10, 12]    count=1
 * Pop  5(0,1) → push  9(0,2)    堆:[9, 10, 12]    count=2
 * Pop  9(0,2) → 行末             堆:[10, 12]       count=3
 * Pop 10(1,0) → push 11(1,1)    堆:[11, 12]       count=4
 * Pop 11(1,1) → push 13(1,2)    堆:[12, 13]       count=5
 * Pop 12(2,0) → push 13(2,1)    堆:[13, 13]       count=6
 * Pop 13(1,2) → 行末             堆:[13]           count=7
 * Pop 13(2,1) → push 15(2,2)    堆:[15]           count=8 → 答案=13
 *
 * 时间复杂度：O(k * log n)
 *   - 弹出 k 次，每次堆操作 O(log n)
 *   - 当 k 远小于 n^2 时效率很高
 * 空间复杂度：O(n) — 堆中最多 n 个元素
 */
class Solution_Heap {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        int n = matrix.size();

        // 最小堆：{值, 行号, 列号}
        auto cmp = [](const tuple<int,int,int>& a, const tuple<int,int,int>& b) {
            return get<0>(a) > get<0>(b);  // 小顶堆
        };
        priority_queue<tuple<int,int,int>,
                       vector<tuple<int,int,int>>,
                       decltype(cmp)> pq(cmp);

        // 初始化：每行的第一个元素入堆
        // 优化：最多只需要 min(n, k) 行
        for (int i = 0; i < min(n, k); i++) {
            pq.push({matrix[i][0], i, 0});
        }

        int result = 0;
        // 弹出 k 次
        for (int i = 0; i < k; i++) {
            auto [val, row, col] = pq.top();
            pq.pop();
            result = val;

            // 将同行的下一个元素入堆
            if (col + 1 < n) {
                pq.push({matrix[row][col + 1], row, col + 1});
            }
        }

        return result;
    }
};


/*
 * ============================================================
 * 解法三：二分 + 右上角统计（变体写法）
 * ============================================================
 *
 * 与解法一思路相同，但从右上角出发统计 <= mid 的个数
 *
 * 右上角 (0, n-1) 出发：
 * - matrix[r][c] > mid → c--（向左走，排除这列）
 * - matrix[r][c] <= mid → count += c+1, r++（这行有 c+1 个 <= mid）
 */
class Solution_TopRight {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        int n = matrix.size();
        int lo = matrix[0][0], hi = matrix[n - 1][n - 1];

        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            int count = countFromTopRight(matrix, mid, n);
            if (count < k) lo = mid + 1;
            else hi = mid;
        }
        return lo;
    }

private:
    int countFromTopRight(vector<vector<int>>& matrix, int mid, int n) {
        int count = 0;
        int r = 0, c = n - 1;  // 从右上角开始

        while (r < n && c >= 0) {
            if (matrix[r][c] <= mid) {
                count += c + 1;  // 这一行从 col 0 到 col c 都 <= mid
                r++;             // 向下移动（看下一行）
            } else {
                c--;             // 向左移动
            }
        }
        return count;
    }
};


/*
 * ============================================================
 * 解法四：直接排序（暴力基线）
 * ============================================================
 *
 * 时间复杂度：O(n^2 * log(n^2)) = O(n^2 * log n)
 * 空间复杂度：O(n^2)
 *
 * 不满足题目"空间优于 O(n^2)"的要求，但可以用来验证其他解法的正确性
 */
class Solution_Brute {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        vector<int> all;
        for (auto& row : matrix) {
            for (int x : row) {
                all.push_back(x);
            }
        }
        sort(all.begin(), all.end());
        return all[k - 1];
    }
};


/*
 * ============================================================
 * 解法五：二分 + 每行 upper_bound（列不有序时的变体）
 * ============================================================
 *
 * 如果矩阵只有行有序（列不一定有序），左下角走法不再适用。
 * 此时每行用 upper_bound 统计 <= mid 的个数。
 *
 * 时间复杂度：O(n * log n * log(max - min))
 * 空间复杂度：O(1)
 *
 * 面试追问：如果列不有序怎么办？ → 用这个变体
 */
class Solution_RowOnly {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        int n = matrix.size();
        int lo = matrix[0][0], hi = matrix[n - 1][n - 1];

        // 找实际的 min 和 max（列不有序时左上角不一定是最小）
        for (int i = 0; i < n; i++) {
            lo = min(lo, matrix[i][0]);
            hi = max(hi, matrix[i][n - 1]);
        }

        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            int count = 0;
            for (int i = 0; i < n; i++) {
                // 每行用 upper_bound 统计 <= mid 的个数
                count += upper_bound(matrix[i].begin(), matrix[i].end(), mid)
                       - matrix[i].begin();
            }
            if (count < k) lo = mid + 1;
            else hi = mid;
        }
        return lo;
    }
};


/*
 * ============================================================
 * 常见错误与陷阱
 * ============================================================
 *
 * 错误1: 二分下标而非值
 *   int lo = 0, hi = n*n-1;  // 错！应该二分值域
 *
 * 错误2: count += r 而非 r+1
 *   count += r;  // 少算了 row 0, 正确是 count += r + 1
 *
 * 错误3: mid 溢出
 *   int mid = (lo + hi) / 2;  // 当 lo+hi > INT_MAX 时溢出
 *   int mid = lo + (hi - lo) / 2;  // 安全写法
 *
 * 错误4: 二分条件写反
 *   if (count <= k) hi = mid;  // 错！应该是 count < k → lo=mid+1
 *
 * 错误5: 堆解法忘记处理行末
 *   弹出 (row, col) 后，如果 col+1 >= n 就不应再 push
 *
 * ============================================================
 * 复杂度对比
 * ============================================================
 *
 * | 解法           | 时间                   | 空间    | 适用场景      |
 * |---------------|------------------------|---------|--------------|
 * | 二分+左下角     | O(n*log(max-min))      | O(1)   | 面试首选      |
 * | 最小堆K路归并   | O(k*log n)             | O(n)   | k小时高效     |
 * | 二分+upper_bound| O(n*logn*log(max-min)) | O(1)   | 列不有序时    |
 * | 直接排序        | O(n^2*log n)           | O(n^2) | 暴力验证      |
 *
 * ============================================================
 * 相关题目
 * ============================================================
 *
 * 240. Search a 2D Matrix II     —— 左下角搜索法
 * 373. Find K Pairs Smallest Sums —— 最小堆 K路归并
 * 668. Kth Smallest in Mult Table —— 二分答案 + 统计
 * 719. K-th Smallest Pair Distance—— 二分答案经典
 * 786. K-th Smallest Prime Fraction —— 二分 + 双指针
 *  23. Merge k Sorted Lists       —— K路归并模板
 *
 * ============================================================
 * 总结：解法对比 & 面试要点
 * ============================================================
 *
 * 【解法选择决策树】
 *
 *   行列都有序？
 *    ├─ YES → 二分值域 + 左下角阶梯 O(n*log(max-min)), O(1)  ← 面试首选
 *    │         优点: 空间O(1)，满足Follow-up
 *    │         堆K路归并 O(k*logn), O(n)                      ← k很小时更快
 *    │         优点: 当k<<n²时实际效率更高
 *    └─ 只有行有序 → 二分 + 每行upper_bound O(n*logn*log(max-min))
 *
 * 【核心易错点】
 * 1. 二分的对象是"值域"而非"下标" — 这是本题最关键的洞察
 *    错: lo=0, hi=n*n-1    对: lo=matrix[0][0], hi=matrix[n-1][n-1]
 * 2. 左下角统计时 count += r+1 不是 r — row 是 0-indexed
 * 3. mid 溢出 — lo+hi 可能超 INT_MAX，必须用 lo+(hi-lo)/2
 * 4. 二分收敛性 — 为什么 lo 最终一定是矩阵中存在的值？
 *    因为 lo 只在 count<k 时变为 mid+1，而 hi 只在 count>=k 时变为 mid，
 *    收敛时 lo==hi，且这个值是使得 count>=k 的最小值，一定在矩阵中
 * 5. 堆解法中弹出后要检查 col+1<n，行末不再 push
 *
 * 【面试追问链 Q1 → Q2 → Q3】
 *
 * Q1: 为什么二分值域而不是二分下标？
 *   → 矩阵不是一维有序的，无法用下标映射到唯一值。
 *     但值域是连续的，可以对值做二分，配合"统计<=mid个数"来定位。
 *
 * Q2: 左下角统计的 O(n) 原理？能否从其他角开始？
 *   → 左下角(n-1,0)和右上角(0,n-1)是两个"拐点"：一个方向增、一个方向减。
 *     左上角(0,0)两个方向都增，右下角(n-1,n-1)两个方向都减，无法分叉搜索。
 *     所以只有左下和右上可以 O(n) 统计。
 *
 * Q3: 如果要求 O(n) 时间（不依赖值域范围），有可能吗？
 *   → 理论上存在基于中位数的 O(n) 算法（类似 BFPRT），但实现极其复杂，
 *     面试中不要求。实际中二分值域的 60n 常数项已经非常快。
 */
