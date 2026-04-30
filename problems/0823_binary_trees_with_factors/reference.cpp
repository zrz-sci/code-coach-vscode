/*
 * 【题目本质】
 * 给定一组唯一整数(均>1)，构造二叉树使得每个非叶节点的值等于两个子节点值的乘积。
 * 每个数字可重复使用。求总共能构造多少棵不同的二叉树（mod 1e9+7）。
 * 核心：排序 + 动态规划 + 因子枚举。
 *
 * 【解法总览】
 * Solution1: 排序 + DP + HashMap（推荐）—— dp[val] 表示以 val 为根的树数量
 * Solution2: 排序 + DP + 索引数组 —— 用数组下标代替 HashMap
 */

// ===================== Solution1: 排序 + DP + HashMap（推荐） =====================
// 思路：
// 1. 排序确保处理顺序从小到大
// 2. dp[val] = 1（单节点）+ sum(dp[left] * dp[right]) 对所有 left*right == val
// 3. 最终答案 = sum(dp[val]) for all val
// 时间 O(N^2)，空间 O(N)
class Solution1 {
public:
    int numFactoredBinaryTrees(vector<int>& arr) {
        const int MOD = 1e9 + 7;
        sort(arr.begin(), arr.end());

        // dp[val] = 以 val 为根可以构造的二叉树数量
        unordered_map<long long, long long> dp;
        for (int x : arr) dp[x] = 1; // 每个值至少是一棵单节点树

        for (int i = 0; i < (int)arr.size(); i++) {
            long long val = arr[i];
            // 枚举左子节点 arr[j]，检查右子节点 val/arr[j] 是否存在
            for (int j = 0; j < i; j++) {
                long long left = arr[j];
                if (val % left == 0) {
                    long long right = val / left;
                    if (dp.count(right)) {
                        // 左子树方案 * 右子树方案
                        dp[val] = (dp[val] + dp[left] % MOD * (dp[right] % MOD)) % MOD;
                    }
                }
            }
        }

        long long result = 0;
        for (auto& [k, v] : dp) {
            result = (result + v) % MOD;
        }
        return (int)result;
    }
};

// ===================== Solution2: 排序 + DP + 索引数组 =====================
// 思路：用数组存 dp 值，用 HashMap 做值到索引的映射
// 时间 O(N^2)，空间 O(N)
class Solution2 {
public:
    int numFactoredBinaryTrees(vector<int>& arr) {
        const int MOD = 1e9 + 7;
        int n = arr.size();
        sort(arr.begin(), arr.end());

        // 值 → 排序后的索引
        unordered_map<int, int> idx;
        for (int i = 0; i < n; i++) idx[arr[i]] = i;

        vector<long long> dp(n, 1);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < i; j++) {
                // 防止 int 溢出
                if ((long long)arr[i] % arr[j] == 0) {
                    int right = arr[i] / arr[j];
                    auto it = idx.find(right);
                    if (it != idx.end()) {
                        dp[i] = (dp[i] + dp[j] * dp[it->second]) % MOD;
                    }
                }
            }
        }

        long long result = 0;
        for (int i = 0; i < n; i++) {
            result = (result + dp[i]) % MOD;
        }
        return (int)result;
    }
};

/*
 * 【解法对比】
 * ┌──────────────────┬──────────────┬──────────────┬────────────────────────┐
 * │ 解法             │ 时间复杂度   │ 空间复杂度   │ 适用场景               │
 * ├──────────────────┼──────────────┼──────────────┼────────────────────────┤
 * │ DP + HashMap     │ O(N^2)       │ O(N)         │ 代码简洁，直觉清晰     │
 * │ DP + 索引数组    │ O(N^2)       │ O(N)         │ 数组访问更快           │
 * └──────────────────┴──────────────┴──────────────┴────────────────────────┘
 *
 * 【易错点】
 * 1. 必须先排序：DP 依赖"从小到大"的处理顺序，因为 left < val, right < val。
 * 2. long long 溢出：arr[i] 最大 1e9，两个 int 相乘可达 1e18，必须用 long long。
 * 3. 取模时机：乘法结果立即取模，避免两个 long long 相乘再相加后溢出。
 * 4. 初始值 dp[x] = 1：单节点也是合法二叉树，不要初始化为 0。
 * 5. 左右子树有序：(2,5) 和 (5,2) 是两棵不同的树，不要除以 2 去重。
 *
 * 【面试追问】
 * Q1: 能否用记忆化搜索代替自底向上 DP？
 *     → 可以，但需要先建 HashSet 判断因子是否在数组中，递归拆解每个值。
 *       排序后的迭代 DP 更直观且无递归栈开销。
 *
 * Q2: 如果数组中允许值为 1，对算法有何影响？
 *     → 值为 1 时，任何节点都可以有无穷个"1"子节点（1*x = x），导致答案无穷大。
 *       题目约束 arr[i] >= 2 正是为了避免这个问题。
 *
 * Q3: 如何输出具体的树结构而非数量？
 *     → 将 dp[val] 改为存储树的列表而非计数。对每对因子 (left, right)，
 *       笛卡尔积生成所有子树组合。注意组合爆炸问题。
 *
 * Q4: 这道题与"整数拆分"类题目的关系？
 *     → 都是因子分解 + DP 的模式。区别在于本题是乘法分解（二叉树结构），
 *       整数拆分是加法分解。DP 转移方程不同但框架类似。
 */
