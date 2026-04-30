/*
 * LeetCode 894: All Possible Full Binary Trees (所有可能的满二叉树)
 *
 * 【题目本质】
 * 递归枚举 + 记忆化：n 个节点的满二叉树 = 根 + 左子树(i) + 右子树(n-1-i)，
 * 其中 i 和 n-1-i 都是正奇数。枚举所有合法分割，递归构造，两两组合。
 * 结果数量为卡塔兰数，增长为指数级。
 *
 * 【解法总览】
 * 解法1: 暴力递归      — O(2^(n/2)) / O(2^(n/2)) — 直觉写法
 * 解法2: 记忆化递归    — O(2^(n/2)) / O(2^(n/2)) — 面试首选
 * 解法3: 迭代 DP       — O(2^(n/2)) / O(2^(n/2)) — 追问时给
 */

// ============================================================
// 解法1: 暴力递归 — 枚举左右子树大小，递归构造
// 时间: O(2^(n/2)) 含重复计算  空间: O(2^(n/2))
//
// 【思路】
// 满二叉树的递归定义:
//   - n=1: 单个叶节点
//   - n>1 且 n 为奇数: 根 + 左子树(i个节点) + 右子树(n-1-i个节点)
//     其中 i 为奇数，范围 [1, n-2]
//
// 对每个合法的 i，递归构造左子树和右子树的所有可能，
// 然后做笛卡尔积（两两组合），每对组合创建一个新的根节点。
//
// 满二叉树数量的递推关系（设 f(n) 为 n 节点满二叉树数）:
//   f(1) = 1
//   f(n) = sum{ f(i) * f(n-1-i) }, i = 1,3,5,...,n-2
//
// 这正是卡塔兰数的递推公式:
//   n=1: 1, n=3: 1, n=5: 2, n=7: 5, n=9: 14, n=11: 42
// ============================================================
class Solution1 {
public:
    vector<TreeNode*> allPossibleFBT(int n) {
        // 偶数节点不可能形成满二叉树
        if (n % 2 == 0) return {};
        // base case: 单节点就是一个叶子
        if (n == 1) return {new TreeNode(0)};

        vector<TreeNode*> result;

        // 枚举左子树使用 i 个节点（奇数）
        for (int i = 1; i < n; i += 2) {
            // 右子树使用 n-1-i 个节点（也是奇数）
            auto left_trees = allPossibleFBT(i);
            auto right_trees = allPossibleFBT(n - 1 - i);

            // 笛卡尔积：每棵左子树 × 每棵右子树
            for (auto* l : left_trees) {
                for (auto* r : right_trees) {
                    // 每种组合创建一个新的根
                    TreeNode* root = new TreeNode(0);
                    root->left = l;
                    root->right = r;
                    result.push_back(root);
                }
            }
        }

        return result;
    }
};

// ============================================================
// 解法2: 记忆化递归 — 缓存每个 n 的结果避免重复构造（⭐ 面试首选）
// 时间: O(2^(n/2))  空间: O(2^(n/2))
//
// 【思路】
// 解法1中，allPossibleFBT(i) 可能被不同的上层调用重复计算。
// 例如 n=9 时:
//   i=1: left=FBT(1), right=FBT(7)
//   i=3: left=FBT(3), right=FBT(5) ← FBT(3) 和 FBT(5) 会在 FBT(7) 内部也被调用
//   i=5: left=FBT(5), right=FBT(3)
//   i=7: left=FBT(7), right=FBT(1)
//
// 用 map 缓存 n → vector<TreeNode*>，每个 n 只构造一次。
//
// 注意:
// - 使用 auto& 返回引用避免拷贝大 vector
// - 子树指针被多棵树共享（不做深拷贝），LeetCode 允许这样做
// ============================================================
class Solution2 {
public:
    unordered_map<int, vector<TreeNode*>> memo;

    vector<TreeNode*> allPossibleFBT(int n) {
        if (n % 2 == 0) return {};

        // 检查缓存
        if (memo.count(n)) return memo[n];

        // base case
        if (n == 1) {
            return memo[1] = {new TreeNode(0)};
        }

        vector<TreeNode*> result;

        for (int i = 1; i < n; i += 2) {
            // 使用引用避免拷贝缓存中的 vector
            auto& left_trees = allPossibleFBT(i);
            auto& right_trees = allPossibleFBT(n - 1 - i);

            for (auto* l : left_trees) {
                for (auto* r : right_trees) {
                    TreeNode* root = new TreeNode(0);
                    root->left = l;
                    root->right = r;
                    result.push_back(root);
                }
            }
        }

        // 存入缓存
        return memo[n] = result;
    }
};

// ============================================================
// 解法3: 迭代 DP — 自底向上构建
// 时间: O(2^(n/2))  空间: O(2^(n/2))
//
// 【思路】
// 将记忆化递归翻转为自底向上的迭代:
//   dp[k] = 所有 k 个节点的满二叉树列表
//
// 从 k=1 开始，依次计算 k=3, 5, 7, ..., n
// 每个 dp[k] 由所有 dp[i] × dp[k-1-i] 组合得到
//
// 优点: 无递归栈开销
// 缺点: 需要分配大小为 n+1 的 vector（大部分偶数下标为空）
//
// 示例 n=7:
//   dp[1] = [{0}]                           (1 棵)
//   dp[3] = [{0, {0}, {0}}]                 (1 棵)
//   dp[5]:
//     i=1: dp[1]×dp[3] → 1×1 = 1 棵
//     i=3: dp[3]×dp[1] → 1×1 = 1 棵
//     共 2 棵
//   dp[7]:
//     i=1: dp[1]×dp[5] → 1×2 = 2 棵
//     i=3: dp[3]×dp[3] → 1×1 = 1 棵
//     i=5: dp[5]×dp[1] → 2×1 = 2 棵
//     共 5 棵 ✓ (卡塔兰数 C_3 = 5)
// ============================================================
class Solution3 {
public:
    vector<TreeNode*> allPossibleFBT(int n) {
        if (n % 2 == 0) return {};

        // dp[k] 存储 k 个节点的所有满二叉树
        vector<vector<TreeNode*>> dp(n + 1);

        // base case: 1 个节点 = 单个叶子
        dp[1] = {new TreeNode(0)};

        // 自底向上，步长 2（只处理奇数）
        for (int total = 3; total <= n; total += 2) {
            // 枚举左子树节点数（奇数）
            for (int left_cnt = 1; left_cnt < total; left_cnt += 2) {
                int right_cnt = total - 1 - left_cnt;

                // 笛卡尔积
                for (auto* l : dp[left_cnt]) {
                    for (auto* r : dp[right_cnt]) {
                        TreeNode* root = new TreeNode(0);
                        root->left = l;
                        root->right = r;
                        dp[total].push_back(root);
                    }
                }
            }
        }

        return dp[n];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间         | 空间         | 特点                  |
// |-----------|-------------|-------------|-----------------------|
// | 暴力递归   | O(2^(n/2)) | O(2^(n/2))  | 简洁但有重复计算       |
// | 记忆化递归 | O(2^(n/2)) | O(2^(n/2))  | ⭐面试首选，自然优化    |
// | 迭代 DP    | O(2^(n/2)) | O(2^(n/2))  | 无递归栈，追问时给出   |
//
// 面试建议:
//   1. 先分析满二叉树的性质（n 必须是奇数）
//   2. 写出暴力递归，解释枚举逻辑
//   3. 加上 memo 变成记忆化递归
//   4. 追问时可以转成迭代 DP
//
// 【易错点】
// 1. 没判断 n 为偶数:
//    ✗ 对 n=4 也递归 → 死循环或空结果但浪费计算
//    ✓ 开头 if (n%2==0) return {}
//
// 2. 枚举范围从 0 或到 n:
//    ✗ i=0 → 左子树为空，不是满二叉树
//    ✗ i=n-1 → 右子树为空
//    ✓ i 从 1 到 n-2，步长 2
//
// 3. 组合时复用根节点:
//    ✗ 在循环外 new TreeNode，循环内改 left/right → 所有组合共享同一个根
//    ✓ 每次组合都 new TreeNode(0)
//
// 4. memo 返回拷贝而非引用:
//    ✗ auto left = allPossibleFBT(i) → 拷贝整个 vector，性能差
//    ✓ auto& left = allPossibleFBT(i) → 返回引用
//
// 5. 混淆满二叉树和完全二叉树:
//    Full Binary Tree: 每个节点 0 或 2 个孩子（本题）
//    Complete Binary Tree: 最后一层从左到右填充
//    Perfect Binary Tree: 所有叶子同层，所有内部节点 2 个孩子
//
// 【面试追问 Interview Follow-ups】
// Q1: n 个节点的满二叉树有多少种？
// A1: 第 (n-1)/2 个卡塔兰数。n=1→1, n=3→1, n=5→2, n=7→5, n=9→14。
//     公式: C_m = C(2m,m)/(m+1), m=(n-1)/2。
//
// Q2: 如果只需要计数不需要构造树呢？
// A2: 直接用 DP: dp[1]=1, dp[n] = sum(dp[i]*dp[n-1-i]) for odd i in [1,n-2]。
//     时间 O(n^2)，空间 O(n)。
//
// Q3: 共享子树指针有什么隐患？
// A3: 只读场景没问题。如果需要修改某棵树的节点值（比如给节点编号），
//     必须深拷贝，否则会影响所有共享该子树的树。
// ============================================================
