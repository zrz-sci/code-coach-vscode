/*
 * LeetCode 679: 24 点游戏 (24 Game)
 *
 * 【题目本质】
 * 4个数字，3次二元运算（+、-、*、/），任意括号方案（即任意运算顺序），
 * 判断能否得到24。本质是穷举所有可能的"表达式树"。
 *
 * 【解法总览】
 * 解法1: 回溯穷举（选2合1递归）— O(1) / O(1) — 面试首选⭐
 * 解法2: 枚举所有表达式树结构   — O(1) / O(1) — 更直观但代码长
 *
 * 【决策树示例】以 [4, 1, 8, 7] 为例的一条成功路径:
 *
 *   [4, 1, 8, 7]
 *       ↓  选 8,4 做 8-4=4
 *   [1, 7, 4]
 *       ↓  选 7,1 做 7-1=6
 *   [4, 6]
 *       ↓  选 4,6 做 4*6=24
 *   [24]  → fabs(24-24) < EPS → return true!
 *
 *   完整决策树第一层（4个数选有序对 × 4种运算）:
 *
 *                        [4, 1, 8, 7]
 *            /         /       |        \         \       ...
 *     pick(4,1)   pick(4,8)  pick(4,7) pick(1,8) pick(8,4) ...
 *      +: 5        +: 12      +: 11     +: 9      -: 4
 *      -: 3        -: -4      -: -3     -: -7     *: 32
 *      *: 4        *: 32      *: 28     *: 8      /: 2
 *      /: 4        /: 0.5     /: 0.57   /: 0.125  ...
 *       ↓           ↓          ↓         ↓
 *   [8,7,5]    [1,7,12]   [1,8,11]  [4,7,9]   [1,7,4]
 *     ...         ...        ...       ...     → 继续递归...
 *
 *   每条路径深度为3（合并3次），叶子节点判断是否=24。
 */

// ============================================================
// 解法1: 回溯穷举 — 每次从列表中选2个数合并成1个，递归到1个数
// 时间: O(1)  空间: O(1)  （输入固定4个数，搜索空间 ≤ 9216）
//
// 【思路】
// 核心洞察：括号的作用是决定运算顺序，而"每次选2个数合并"天然
// 覆盖了所有可能的运算顺序。
//
// 任何由4个数和3个运算符组成的合法表达式，都对应一棵二叉树：
//   - 4个叶子 = 4个数
//   - 3个内部节点 = 3个运算符
//   - 树的形态决定了括号方案
//
// "选2合1"的过程就是自底向上构建这棵树：
//   4个数 → 选2合1 → 3个数 → 选2合1 → 2个数 → 选2合1 → 1个数
//
// 为什么遍历有序对(i,j)而不是无序对？
//   因为 a-b ≠ b-a，a/b ≠ b/a。有序对自然覆盖两个方向。
//   加法和乘法会重复计算（a+b = b+a），但不影响正确性，
//   且总搜索量极小，无需优化。
// ============================================================
class Solution1 {
public:
    static constexpr double EPS = 1e-6;

    bool judgePoint24(vector<int>& cards) {
        vector<double> nums(cards.begin(), cards.end());
        return backtrack(nums);
    }

private:
    bool backtrack(vector<double>& nums) {
        int n = nums.size();
        // 递归终止：只剩1个数，判断是否为24
        if (n == 1) {
            return fabs(nums[0] - 24.0) < EPS;
        }

        // 枚举所有有序对 (i, j)，i ≠ j
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) continue;

                // 构建新列表：去掉位置i和j的数，保留其余
                vector<double> next;
                for (int k = 0; k < n; k++) {
                    if (k != i && k != j) {
                        next.push_back(nums[k]);
                    }
                }

                double a = nums[i], b = nums[j];

                // 尝试4种运算，将结果追加到next末尾
                // 加法
                next.push_back(a + b);
                if (backtrack(next)) return true;
                next.pop_back();

                // 减法（有序对已经覆盖 a-b 和 b-a）
                next.push_back(a - b);
                if (backtrack(next)) return true;
                next.pop_back();

                // 乘法
                next.push_back(a * b);
                if (backtrack(next)) return true;
                next.pop_back();

                // 除法（需要除零保护：中间运算可能产生0）
                if (fabs(b) > EPS) {
                    next.push_back(a / b);
                    if (backtrack(next)) return true;
                    next.pop_back();
                }
            }
        }
        return false;
    }
};

// ============================================================
// 解法2: 枚举所有表达式树结构 — 更显式地覆盖5种树形态
// 时间: O(1)  空间: O(1)
//
// 【思路】
// 4个数3次运算，所有可能的二叉树形态只有5种：
//
//   形态1         形态2         形态3         形态4         形态5
//      ○             ○            ○             ○             ○
//     / \           / \          / \           / \           / \
//    ○   d         ○   d       ○   ○         a   ○         a   ○
//   / \           / \         / \ / \           / \           / \
//  ○   c         a   ○      a  b c  d         ○   d         b   ○
// / \               / \                      / \               / \
// a  b             b   c                    b   c             c   d
//
// ((a○b)○c)○d  (a○(b○c))○d  (a○b)○(c○d)  a○((b○c)○d)  a○(b○(c○d))
//
// 对每种形态，枚举4!=24种排列 × 4^3=64种运算符 = 1536种。
// 5种形态共 7680 种（上界，含重复）。
//
// 这种方法更适合理解"为什么只有这几种情况"，但代码较长。
// 解法1的回溯自动覆盖了这5种形态。
// ============================================================
class Solution2 {
public:
    static constexpr double EPS = 1e-6;

    // 对两个数执行所有可能的二元运算
    vector<double> compute(double a, double b) {
        vector<double> res = {a + b, a - b, a * b};
        if (fabs(b) > EPS) res.push_back(a / b);
        return res;
    }

    bool judgePoint24(vector<int>& cards) {
        vector<double> nums(cards.begin(), cards.end());
        // 排序以便 next_permutation 能枚举所有排列
        sort(nums.begin(), nums.end());

        do {
            double a = nums[0], b = nums[1], c = nums[2], d = nums[3];

            // ---- 形态1: ((a○b)○c)○d ----
            for (double ab : compute(a, b)) {
                for (double abc : compute(ab, c)) {
                    for (double abcd : compute(abc, d)) {
                        if (fabs(abcd - 24.0) < EPS) return true;
                    }
                }
            }

            // ---- 形态3: (a○b)○(c○d) ----
            for (double ab : compute(a, b)) {
                for (double cd : compute(c, d)) {
                    for (double abcd : compute(ab, cd)) {
                        if (fabs(abcd - 24.0) < EPS) return true;
                    }
                }
            }

            // ---- 形态2: (a○(b○c))○d ----
            for (double bc : compute(b, c)) {
                for (double abc : compute(a, bc)) {
                    for (double abcd : compute(abc, d)) {
                        if (fabs(abcd - 24.0) < EPS) return true;
                    }
                }
            }

            // ---- 形态4: a○((b○c)○d) ----
            for (double bc : compute(b, c)) {
                for (double bcd : compute(bc, d)) {
                    for (double abcd : compute(a, bcd)) {
                        if (fabs(abcd - 24.0) < EPS) return true;
                    }
                }
            }

            // ---- 形态5: a○(b○(c○d)) ----
            for (double cd : compute(c, d)) {
                for (double bcd : compute(b, cd)) {
                    for (double abcd : compute(a, bcd)) {
                        if (fabs(abcd - 24.0) < EPS) return true;
                    }
                }
            }

        } while (next_permutation(nums.begin(), nums.end()));

        return false;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 解法1: 选2合1回溯          | 解法2: 枚举表达式树     |
// |-------------|---------------------------|----------------------|
// | 代码量       | ~30行，简洁               | ~50行，较长            |
// | 正确性保证   | 天然覆盖所有树形态         | 需手动确保5种形态全列出  |
// | 可扩展到N数  | 直接推广，改递归参数即可    | N增大后形态数爆炸       |
// | 面试推荐     | ⭐首选                    | 适合讨论分析           |
//
// 两种方法的搜索空间上界都在万级以下，不需要任何剪枝优化。
//
// ============================================================
// 【易错点】
//
// 1. 只枚举无序对 (i < j) 导致漏解：
//    ✗ for(j=i+1;...) 只算 a-b, a/b
//    ✓ for(j=0;j<n;j++) if(j!=i) → 有序对自动覆盖 a-b 和 b-a
//    原因: 8/2=4 vs 2/8=0.25 完全不同，漏一个就可能错过正确表达式
//
// 2. 浮点比较用 ==：
//    ✗ return nums[0] == 24.0;
//    ✓ return fabs(nums[0] - 24.0) < 1e-6;
//    原因: 连续除法如 8/(1-2/3) 在浮点中可能产生微小误差
//
// 3. 忘记除零保护：
//    ✗ results.push_back(a / b);  // b可能是中间运算得到的0
//    ✓ if (fabs(b) > EPS) results.push_back(a / b);
//    原因: cards=[1,1,1,1] 时 1-1=0，后续除以0得到inf
//
// 4. 解法2中漏掉某种树形态：
//    5种形态必须全覆盖。容易漏掉形态4: a○((b○c)○d)
//    建议: 用解法1，天然不会漏
//
// ============================================================
// 【面试追问】
//
// Q1: "选2合1"为什么能覆盖所有括号方案？
// A1: 任何表达式都对应一棵二叉表达式树。树的构建过程就是
//     反复选两个子树合并。我们的回溯恰好在枚举所有可能的
//     合并顺序，因此覆盖了所有树的形态（即所有括号方案）。
//
// Q2: 搜索空间能精确算出来吗？有没有重复计算？
// A2: 上界 = P(4,2)×4 × P(3,2)×4 × P(2,2)×4 = 12×4 × 6×4 × 2×4
//     = 48 × 24 × 8 = 9216。其中加法/乘法的交换律导致重复，
//     但搜索量太小无需优化。
//
// Q3: 如果推广到N个数字得到目标T，怎么做？
// A3: 同样的回溯框架，每步从N个数中选2个合并。搜索空间约
//     P(N,2)×4 × P(N-1,2)×4 × ... 指数增长。优化方向：
//     (1) 剪枝：当前最大可能值 < T 或最小可能值 > T 时剪
//     (2) 记忆化：对数字集合（排序后hash）做缓存
//     (3) Meet-in-the-middle：N较大时分两半分别枚举再合并
// ============================================================
