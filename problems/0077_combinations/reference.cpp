/*
 * LeetCode 77: 组合 (Combinations)
 *
 * 【题目本质】
 * 从 [1, n] 中选 k 个数的所有组合 = 在决策树上做回溯搜索，
 * 用 start 参数保证组合中数字递增，避免重复。
 *
 * 【解法总览】
 * 解法1: 回溯（for循环枚举选择）   — O(k·C(n,k)) / O(k) — 组合回溯标准模板
 * 解法2: 回溯 + 剪枝              — O(k·C(n,k)) / O(k) — 面试首选(加分)
 * 解法3: 回溯（选/不选二叉决策）   — O(k·C(n,k)) / O(n) — 子集视角
 * 解法4: 迭代（字典序枚举）        — O(k·C(n,k)) / O(k) — 无递归
 */

// ============================================================
// 解法1: 回溯（for循环枚举选择）— 组合回溯标准模板
// 时间: O(k · C(n,k))  空间: O(k) 递归栈深度
//
// 【思路】
// 组合中的数字保持升序就不会重复。
// 每一层递归用 for 循环从 start 到 n 枚举"这个位置选哪个数"，
// 选完后 start 变成 i+1，保证后续只选更大的数。
// 选够 k 个就收集结果。
//
// 【决策树 (n=4, k=2)】
//                    []
//          /       /     \      \
//        [1]     [2]     [3]    [4]     ← 第1层: i 从 1 到 4
//       /|\      / \      |
//   [1,2][1,3][1,4] [2,3][2,4] [3,4]   ← 第2层: path.size()==k, 收集
//
// [4] 在第1层时, 进入第2层 for(i=5; i<=4) 不执行，直接返回
// 这就是解法2要优化的无效递归
// ============================================================
class Solution1 {
public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(n, k, 1, path, result);
        return result;
    }

private:
    void backtrack(int n, int k, int start, vector<int>& path,
                   vector<vector<int>>& result) {
        if ((int)path.size() == k) {
            result.push_back(path);
            return;
        }
        // 从 start 开始枚举，保证组合中数字严格递增
        for (int i = start; i <= n; i++) {
            path.push_back(i);                      // 做选择
            backtrack(n, k, i + 1, path, result);    // 递归，下一层从 i+1 开始
            path.pop_back();                         // 撤销选择
        }
    }
};

// ============================================================
// 解法2: 回溯 + 剪枝 — 面试首选
// 时间: O(k · C(n,k))  空间: O(k)
//
// 【思路】
// 解法1的问题：比如 n=4,k=3 时，第1层选了 [3]，第2层选了 [3,4]，
// 第3层 for(i=5; i<=4) 啥也不做就返回了——这次递归完全浪费。
//
// 优化：在 for 循环中提前计算上界，如果剩余数字不够选，直接不进入。
// 还需要选 need = k - path.size() 个数
// 从 i 到 n 有 n - i + 1 个数
// 需要 n - i + 1 >= need  →  i <= n - need + 1
//
// 【剪枝示例 (n=7, k=4, path=[1])】
// 还需选 3 个，上界 = 7 - 3 + 1 = 5
// i=2,3,4,5 ✓  i=6,7 ✗（从6开始只有6,7两个数，不够3个）
// ============================================================
class Solution2 {
public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(n, k, 1, path, result);
        return result;
    }

private:
    void backtrack(int n, int k, int start, vector<int>& path,
                   vector<vector<int>>& result) {
        if ((int)path.size() == k) {
            result.push_back(path);
            return;
        }

        int need = k - (int)path.size();  // 还需要选几个
        // 关键剪枝: 上界从 n 收紧到 n - need + 1
        for (int i = start; i <= n - need + 1; i++) {
            path.push_back(i);
            backtrack(n, k, i + 1, path, result);
            path.pop_back();
        }
    }
};

// ============================================================
// 解法3: 回溯（选/不选二叉决策）— 子集视角
// 时间: O(k · C(n,k))  空间: O(n) 递归栈深度最深到 n
//
// 【思路】
// 换一个视角：遍历 1 到 n 的每个数，对每个数做"选"或"不选"两个分支。
// 当选够 k 个就收集，当剩余数字不够就剪枝。
//
// 这和 LeetCode 78 子集是同一个框架，
// 区别仅在于收集条件：78 是遍历完所有数就收集，77 是 path.size()==k 就收集。
//
// 【二叉决策树 (n=4, k=2, 只画左半)】
//                    (cur=1, [])
//           选1 /                \ 不选1
//       (cur=2, [1])          (cur=2, [])
//      选2 /     \ 不选2     选2 /      \ 不选2
//   (cur=3,[1,2]) (cur=3,[1]) (cur=3,[2]) (cur=3,[])
//     收集✓      选3/ \不选3   选3/ \不选3  ...
//           [1,3]✓ (cur=4,[1]) [2,3]✓
//                选4/ \不选4
//            [1,4]✓    X(剪枝:剩0个,还需1个)
// ============================================================
class Solution3 {
public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> result;
        vector<int> path;
        dfs(n, k, 1, path, result);
        return result;
    }

private:
    void dfs(int n, int k, int cur, vector<int>& path,
             vector<vector<int>>& result) {
        // 剪枝: 即使把 cur~n 全选上也不够 k 个
        if ((int)path.size() + (n - cur + 1) < k) return;

        // 选够了 k 个
        if ((int)path.size() == k) {
            result.push_back(path);
            return;
        }

        // 分支1: 选 cur
        path.push_back(cur);
        dfs(n, k, cur + 1, path, result);
        path.pop_back();

        // 分支2: 不选 cur
        dfs(n, k, cur + 1, path, result);
    }
};

// ============================================================
// 解法4: 迭代（字典序枚举）— 无递归
// 时间: O(k · C(n,k))  空间: O(k)
//
// 【思路】
// 维护一个长度为 k 的数组 comb，初始为 [1,2,...,k]。
// 每次输出当前组合后，生成字典序中的下一个组合：
// 从右往左找到第一个还没到上界的位置 i，将 comb[i]++，
// 然后 i 右边的所有位置依次递增填充。
// 类似于"进位"操作。
//
// 【示例 (n=5, k=3)】
// [1,2,3] → [1,2,4] → [1,2,5] → [1,3,4] → [1,3,5] → [1,4,5]
//                          ↑ comb[2]==5到上界
//                            comb[1]从2变3, comb[2]=4
// → [2,3,4] → [2,3,5] → [2,4,5] → [3,4,5] → 结束
// ============================================================
class Solution4 {
public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> result;
        vector<int> comb(k);

        // 初始化: [1, 2, ..., k]
        for (int i = 0; i < k; i++) {
            comb[i] = i + 1;
        }

        while (true) {
            result.push_back(comb);

            // 从最右边往左找第一个还没到上界的位置
            // comb[i] 的上界 = n - (k - 1 - i)
            // 例如 k=3, n=5: 上界分别是 3, 4, 5
            int i = k - 1;
            while (i >= 0 && comb[i] == n - (k - 1 - i)) {
                i--;
            }

            if (i < 0) break;  // 所有位置都到上界，枚举完毕

            comb[i]++;  // 第 i 位进 1

            // i 后面的位置依次递增
            for (int j = i + 1; j < k; j++) {
                comb[j] = comb[j - 1] + 1;
            }
        }

        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 核心思想 | 适用面 | 面试推荐 |
// |------|----------|--------|----------|
// | 1-基础回溯 | for循环选择 | 组合/排列/子集通用 | ⭐ 首选 |
// | 2-回溯+剪枝 | 1的基础上收紧上界 | 同上 | ⭐⭐ 追问必答 |
// | 3-选/不选 | 二叉决策树 | 子集/组合 | 理解即可 |
// | 4-迭代字典序 | 模拟进位 | 仅组合 | 加分项 |
//
// 面试中: 先写解法1，被追问优化时给出解法2的剪枝。
// 如果面试官问"还有其他思路吗"，给出解法3的选/不选视角。
//
// 【易错点】
// 1. for循环起点: ✗ for(int i = 1; ...) 每层从1开始 → 产生 [2,1] 重复
//                 ✓ for(int i = start; ...) 从start开始保证递增
//
// 2. 剪枝上界: ✗ i <= n - k + 1  忘了减去已选数量
//              ✓ i <= n - (k - path.size()) + 1  用"还需选几个"计算
//
// 3. path.size()类型: ✗ path.size() == k  size_t vs int 隐式转换
//                     ✓ (int)path.size() == k  显式转换更安全
//
// 4. 忘记撤销选择: ✗ 只有 push_back 没有 pop_back → 路径越来越长
//                  ✓ push 和 pop 必须成对出现（回溯的核心）
//
// 5. 递归参数传 i 还是 i+1:
//    ✗ backtrack(n, k, i, ...)  → 同一个数会被重复选
//    ✓ backtrack(n, k, i + 1, ...)  → 每个数最多选一次
//    (注: 如果题目允许重复选同一个数如LC39, 才传 i)
//
// 【面试追问 — 递进链】
//
// Q1(基础): 你的回溯怎么保证不产生重复组合?
//   → start 参数保证每次只从上一个选的数的下一个位置开始,
//     使得组合中数字严格递增, 从而[1,2]和[2,1]不会同时出现。
//
// Q2(优化): 有什么可以优化的地方?
//   → 剪枝: 如果从当前 i 到 n 的剩余数字不够凑满 k 个,
//     提前终止。for 循环上界从 n 改成 n - (k-path.size()) + 1。
//
// Q3(变体): 如果数组中有重复元素怎么去重? (→ LeetCode 40)
//   → 先排序, for循环中加: if(i > start && nums[i]==nums[i-1]) continue;
//     同一层中相同的数字只选第一个, 后面的跳过。
//
// Q4(拓展): 你能用非递归方式实现吗?
//   → 方法1: 字典序枚举(解法4), 模拟进位。
//     方法2: 用显式栈模拟递归调用栈。
// ============================================================
