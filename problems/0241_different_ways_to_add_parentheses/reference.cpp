/*
 * LeetCode 241: 为运算表达式设计优先级 (Different Ways to Add Parentheses)
 *
 * 【题目本质】
 * 给定包含数字和 +/-/* 的表达式字符串，返回所有可能的加括号方式所得到的不同结果。
 * 每种加括号方式 = 一种运算顺序 = 一棵表达式树。
 *
 * 【核心思路】
 * 分治: 选择一个运算符作为"根"(最后执行的运算)，
 * 左边递归产生所有可能值，右边递归产生所有可能值，笛卡尔积组合。
 *
 * 【解法总览】
 * 解法1: 分治递归              — O(C_n * n)  — 面试首选 ⭐
 * 解法2: 分治 + 记忆化          — O(C_n * n)  — 追问优化
 * 解法3: 区间 DP               — O(C_n * n)  — 自底向上
 *
 * C_n = 第 n 个卡特兰数 (n 为运算符个数)
 */

/*
 * 递归树可视化: expression = "2*3-4*5"
 *
 *                          dfs("2*3-4*5")
 *                     /          |          \
 *            op='*'(1)     op='-'(3)      op='*'(5)
 *               /    \       /     \         /    \
 *        dfs("2") dfs("3-4*5") dfs("2*3") dfs("4*5") dfs("2*3-4") dfs("5")
 *          [2]   [-17,-5]      [6]       [20]     [-2, 2]     [5]
 *
 * 分支1: op='*' at 1
 *   左=[2], 右=[-17,-5] → 2*(-17)=-34, 2*(-5)=-10
 *
 * 分支2: op='-' at 3
 *   左=[6], 右=[20] → 6-20=-14
 *
 * 分支3: op='*' at 5
 *   左=[-2,2], 右=[5] → (-2)*5=-10, 2*5=10
 *
 * 最终: [-34, -10, -14, -10, 10]
 *
 * 五种加括号方式:
 *   (2*(3-(4*5)))    = -34
 *   (2*((3-4)*5))    = -10
 *   ((2*3)-(4*5))    = -14
 *   ((2*(3-4))*5)    = -10
 *   (((2*3)-4)*5)    = 10
 */

// ============================================================
// 解法1: 分治递归 ⭐ 面试首选
// 时间: O(C_n * n)  空间: O(C_n * n)
// 其中 C_n 是第 n 个卡特兰数, n 为运算符个数
//
// 【思路】
// 遍历表达式中的每个运算符, 以其为分割点:
// 1. 左半部分递归 → 得到所有可能的值
// 2. 右半部分递归 → 得到所有可能的值
// 3. 左右做笛卡尔积, 用当前运算符组合
//
// base case: 没有运算符 → 纯数字, 直接返回
//
// 【和构建所有表达式树等价】
// 每选一个运算符作为分割点, 就是选了表达式树的"根"。
// 左子树 = 左半表达式的所有树形态
// 右子树 = 右半表达式的所有树形态
// ============================================================
class Solution1 {
public:
    vector<int> diffWaysToCompute(string expression) {
        vector<int> result;

        // 遍历每个字符, 遇到运算符就分割
        for (int i = 0; i < (int)expression.size(); i++) {
            char c = expression[i];
            if (c == '+' || c == '-' || c == '*') {
                // 以位置 i 的运算符为分割点
                // 左半: expression[0..i-1]
                // 右半: expression[i+1..end]
                auto left = diffWaysToCompute(expression.substr(0, i));
                auto right = diffWaysToCompute(expression.substr(i + 1));

                // 笛卡尔积: 左边每个值 × 右边每个值
                for (int l : left) {
                    for (int r : right) {
                        if (c == '+') result.push_back(l + r);
                        else if (c == '-') result.push_back(l - r);
                        else result.push_back(l * r);
                    }
                }
            }
        }

        // base case: 遍历完没有运算符 → 纯数字
        // 利用 result.empty() 判断, 巧妙避免额外的 isDigit 检查
        if (result.empty()) {
            result.push_back(stoi(expression));
        }

        return result;
    }
};

// ============================================================
// 解法2: 分治 + 记忆化
// 时间/空间: 同解法1, 但避免了相同子表达式的重复计算
//
// 【思路】
// 相同的子表达式可能从不同的上层分割路径被递归到。
// 例如 "a+b-c*d" 中, 子表达式 "b-c" 可能被不同分割路径访问。
// 用 unordered_map<string, vector<int>> 缓存结果。
//
// 【和解法1的区别】
// 只加了两行: 开头查缓存 + 结尾存缓存。
// ============================================================
class Solution2 {
    unordered_map<string, vector<int>> memo;

public:
    vector<int> diffWaysToCompute(string expression) {
        // 查缓存
        if (memo.count(expression)) return memo[expression];

        vector<int> result;

        for (int i = 0; i < (int)expression.size(); i++) {
            char c = expression[i];
            if (c == '+' || c == '-' || c == '*') {
                auto left = diffWaysToCompute(expression.substr(0, i));
                auto right = diffWaysToCompute(expression.substr(i + 1));

                for (int l : left) {
                    for (int r : right) {
                        if (c == '+') result.push_back(l + r);
                        else if (c == '-') result.push_back(l - r);
                        else result.push_back(l * r);
                    }
                }
            }
        }

        if (result.empty()) {
            result.push_back(stoi(expression));
        }

        // 存缓存
        memo[expression] = result;
        return result;
    }
};

// ============================================================
// 解法3: 区间 DP — 自底向上
// 时间: O(C_n * n)  空间: O(n^2 * C_n)
//
// 【思路】
// 先预处理, 把表达式拆成数字数组 nums 和运算符数组 ops。
// dp[i][j] = 用第 i 到第 j 个数字 (含) 能得到的所有结果。
//
// 转移: 枚举分割点 k (i <= k < j)
//   对 dp[i][k] 中的每个值 l 和 dp[k+1][j] 中的每个值 r,
//   用 ops[k] 组合, 加入 dp[i][j]
//
// base case: dp[i][i] = {nums[i]}
// 答案: dp[0][n-1]
//
// 【与解法1的关系】
// 解法1 是自顶向下递归, 解法3 是自底向上填表。
// 本质完全等价, 只是遍历顺序不同。
// ============================================================
class Solution3 {
public:
    vector<int> diffWaysToCompute(string expression) {
        // 预处理: 提取数字和运算符
        vector<int> nums;
        vector<char> ops;
        int num = 0;
        for (char c : expression) {
            if (isdigit(c)) {
                num = num * 10 + (c - '0'); // 处理多位数
            } else {
                nums.push_back(num);
                ops.push_back(c);
                num = 0;
            }
        }
        nums.push_back(num); // 最后一个数

        int n = nums.size();
        // dp[i][j] = 第i到第j个数字能产生的所有结果
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(n));

        // base case: 单个数字
        for (int i = 0; i < n; i++) {
            dp[i][i] = {nums[i]};
        }

        // 按区间长度从小到大填表
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                // 枚举分割点: k 处的运算符连接 dp[i][k] 和 dp[k+1][j]
                for (int k = i; k < j; k++) {
                    char op = ops[k];
                    for (int l : dp[i][k]) {
                        for (int r : dp[k + 1][j]) {
                            if (op == '+') dp[i][j].push_back(l + r);
                            else if (op == '-') dp[i][j].push_back(l - r);
                            else dp[i][j].push_back(l * r);
                        }
                    }
                }
            }
        }

        return dp[0][n - 1];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 核心 | 代码量 | 重复计算 | 推荐 |
// |------|------|--------|---------|------|
// | 1.分治递归 ⭐ | substr分割 | 最少 | 可能有 | 面试首选 |
// | 2.分治+记忆化 | +memo缓存 | 少 | 避免 | 追问优化 |
// | 3.区间DP | 自底向上 | 中等 | 无 | 理解DP |
//
// 面试推荐: 先写解法1(代码最简洁), 追问优化写解法2(加两行缓存),
// 追问DP思路再说解法3。
//
// 【易错点】
// 1. 多位数字:
//    数字可以是 10, 99 等两位数, 不能把每个字符当单独数字。
//    解法1 中 stoi(expression) 自然处理。
//    解法3 中预处理时 num = num*10 + (c-'0') 正确累积。
//
// 2. 忘记纯数字 base case:
//    ✗ 没有 if (result.empty()) ... → "42" 这样的输入返回空列表
//    ✓ 循环后检查 result 是否为空 → 空则 push 数字
//
// 3. substr 边界:
//    expression.substr(0, i)  = 运算符左边 (不含运算符)
//    expression.substr(i + 1) = 运算符右边 (不含运算符)
//    i 是运算符在字符串中的位置。
//
// 4. 区间 DP 的运算符下标:
//    ops[k] 是第 k 个运算符, 连接第 k 和第 k+1 个数字。
//    分割点 k: i <= k < j (k 是数字下标, 不是字符串下标)。
//
// 【面试追问】
// Q1: 为什么结果数量是卡特兰数?
//    → 每种加括号方式 = 一棵表达式树。n 个运算符 = n 个内部节点。
//      n 个内部节点的满二叉树数 = 卡特兰数 C(n)。
//
// Q2: 记忆化能提升多少?
//    → 不改变渐近复杂度(因为不同子串数量 O(n^2), 结果总数仍是卡特兰级)。
//      但实际可以减少重复的子表达式计算。
//
// Q3: 如果只要最大值/最小值?
//    → 区间DP, dp[i][j] 只存 max 和 min (不存所有值)。
//      注意负数乘负数可能变最大, 需要同时维护 min 和 max。
//
// Q4: 如果还有除法?
//    → 框架不变, 多加 c=='/' 分支。注意除数为 0 要跳过。
//      如果要精确结果, 可能需要浮点数或有理数。
// ============================================================
