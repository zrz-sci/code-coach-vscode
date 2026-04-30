/*
 * LeetCode 553: Optimal Division (最优除法)
 * 难度: Medium
 * 标签: Array, Math, Dynamic Programming
 *
 * =============================================
 * 解法一: 数学贪心 (推荐)
 * =============================================
 *
 * 核心数学:
 *   连除本质: a/b/c/d = a * (1/b) * (1/c) * (1/d)
 *   加括号后: a/(b/c/d) = a * c * d / b
 *   → 把 nums[0] 留在分子, nums[1] 放分母, 其余全变乘法因子
 *   → 分子最大化, 分母最小化, 结果最大
 *
 * 因为所有数 >= 2, 乘法因子越多越大, 所以把 nums[2..n-1]
 * 全部变成分子的乘法因子是最优策略。
 *
 * 最优方案: nums[0] / (nums[1] / nums[2] / ... / nums[n-1])
 * 特判: n==1 直接返回; n==2 不需要括号
 *
 * 时间: O(n) — 一次遍历构造字符串
 * 空间: O(n) — 输出字符串
 */

class Solution {
public:
    string optimalDivision(vector<int>& nums) {
        int n = nums.size();

        // 特判: 只有一个数, 没有除法操作
        if (n == 1) return to_string(nums[0]);

        // 特判: 两个数, "a/b" 不需要括号
        if (n == 2) return to_string(nums[0]) + "/" + to_string(nums[1]);

        // 一般情况: "a/(b/c/d/...)"
        // 将 nums[1..n-1] 用括号包起来, 内部用 "/" 连接
        string result = to_string(nums[0]) + "/(";
        for (int i = 1; i < n; i++) {
            result += to_string(nums[i]);
            if (i < n - 1) result += "/";
        }
        result += ")";

        return result;
    }
};

/*
 * trace: nums = [1000, 100, 10, 2]
 *
 * n = 4 >= 3
 * result = "1000/" + "(" + "100/10/2" + ")"
 *        = "1000/(100/10/2)"
 *
 * 验证: 1000 / (100/10/2) = 1000 / ((100/10)/2) = 1000 / (10/2) = 1000/5 = 200
 * 其他方案:
 *   1000/100/10/2 = 0.5
 *   1000/(100/10)/2 = 50
 *   1000/(100/(10/2)) = 50
 *   1000/100/(10/2) = 2
 * → 200 是最大值 ✓
 *
 * -----------------------------------------------
 * trace: nums = [2, 3, 4]
 *
 * result = "2/(3/4)"
 * 验证: 2 / (3/4) = 2 * 4/3 = 8/3 ≈ 2.667  ✓
 *
 * -----------------------------------------------
 * trace: nums = [5]
 *
 * n == 1 → return "5"  ✓
 *
 * -----------------------------------------------
 * trace: nums = [3, 7]
 *
 * n == 2 → return "3/7"  ✓ (不加括号)
 */


/*
 * =============================================
 * 解法二: 区间 DP (通用解法)
 * =============================================
 *
 * 当数字可能 < 1 或有其他运算符时, 贪心不成立, 需要区间 DP
 *
 * dpMax[i][j]: nums[i..j] 能得到的最大值
 * dpMin[i][j]: nums[i..j] 能得到的最小值
 *
 * 转移: 枚举分割点 k ∈ [i, j)
 *   最大值 = max(dpMax[i][k] / dpMin[k+1][j])  (大 / 小 → 大)
 *   最小值 = min(dpMin[i][k] / dpMax[k+1][j])  (小 / 大 → 小)
 *
 * 同时追踪对应的表达式字符串
 *
 * 时间: O(n^3)
 * 空间: O(n^2)
 */

class Solution2 {
public:
    string optimalDivision(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return to_string(nums[0]);

        // DP 表: 同时追踪最大值、最小值及对应表达式
        vector<vector<double>> dpMax(n, vector<double>(n, 0));
        vector<vector<double>> dpMin(n, vector<double>(n, 1e18));
        vector<vector<string>> exprMax(n, vector<string>(n));
        vector<vector<string>> exprMin(n, vector<string>(n));

        // 初始化: 单个元素
        for (int i = 0; i < n; i++) {
            dpMax[i][i] = dpMin[i][i] = nums[i];
            exprMax[i][i] = exprMin[i][i] = to_string(nums[i]);
        }

        // 枚举区间长度 (从小到大)
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                for (int k = i; k < j; k++) {
                    // 最大值: 左边最大 / 右边最小
                    double val = dpMax[i][k] / dpMin[k + 1][j];
                    if (val > dpMax[i][j]) {
                        dpMax[i][j] = val;
                        // 右半部分超过一个元素时加括号
                        string right = (k + 1 == j)
                            ? exprMin[k + 1][j]
                            : "(" + exprMin[k + 1][j] + ")";
                        exprMax[i][j] = exprMax[i][k] + "/" + right;
                    }
                    // 最小值: 左边最小 / 右边最大
                    val = dpMin[i][k] / dpMax[k + 1][j];
                    if (val < dpMin[i][j]) {
                        dpMin[i][j] = val;
                        string right = (k + 1 == j)
                            ? exprMax[k + 1][j]
                            : "(" + exprMax[k + 1][j] + ")";
                        exprMin[i][j] = exprMin[i][k] + "/" + right;
                    }
                }
            }
        }

        return exprMax[0][n - 1];
    }
};


/*
 * =============================================
 * 解法对比
 * =============================================
 *
 * | 解法     | 时间   | 空间   | 适用范围              |
 * |---------|--------|--------|--------------------|
 * | 数学贪心 | O(n)   | O(n)   | 所有数 >= 2 (本题)    |
 * | 区间 DP  | O(n^3) | O(n^2) | 通用 (数可 <1, 有其他运算) |
 *
 *
 * =============================================
 * 易错点
 * =============================================
 *
 * 1. n==2 加了冗余括号: "a/(b)" 应该是 "a/b"
 *    ✗ 统一处理 n>=2 都加括号
 *    ✓ n==2 特判, 不加括号 (题目要求无冗余)
 *
 * 2. n==1 忘记特判: 单个数没有除法
 *    ✗ 直接构造 "a/(" 格式
 *    ✓ n==1 直接返回数字字符串
 *
 * 3. 区间 DP 中只追踪最大值:
 *    ✗ 只有 dpMax
 *    ✓ 必须同时追踪 dpMax 和 dpMin (max/min 组合才能得到极值)
 *
 * 4. 以为是复杂 DP 题:
 *    ✗ 直接写 O(n^3) 区间 DP
 *    ✓ 先做数学分析, 发现所有数>=2时答案固定, O(n) 即可
 *
 * 5. 区间 DP 括号判断错误:
 *    ✗ 右半部分无论长度都加括号
 *    ✓ 右半部分只有一个元素时不加括号 (k+1 == j)
 *
 *
 * =============================================
 * 面试追问
 * =============================================
 *
 * Q1: 如果数字可以是小数或 0, 贪心还成立吗?
 *     → 不一定. 数 < 1 时放分母反而增大结果, 需要区间 DP
 *
 * Q2: 如果运算符不止除法, 还有加减乘?
 *     → 需要区间 DP, 类似"矩阵链乘法"思路, O(n^3)
 *
 * Q3: 和 241. Different Ways to Add Parentheses 的关系?
 *     → 241 是分治枚举所有加括号方式, 返回所有可能结果
 *     → 本题可用类似方法 (区间 DP), 但贪心是最优解
 *
 * 关联题目:
 *   241. Different Ways to Add Parentheses — 加括号枚举
 *   312. Burst Balloons — 经典区间 DP
 *   282. Expression Add Operators — 表达式类
 *   1039. Minimum Score Triangulation — 区间 DP
 *   1246. Palindrome Removal — 区间 DP
 */
