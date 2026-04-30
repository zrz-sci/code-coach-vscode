// ============================================================
// 364. Nested List Weight Sum II
// ============================================================
// 标签: Stack, Depth-First Search, Breadth-First Search
// 难度: Medium
//
// 【题目本质】
//   嵌套列表中每个整数乘以"反向权重"(越浅权重越大)再求和。
//   权重 = maxDepth - depth + 1。
//   核心挑战: 不预先知道 maxDepth，如何一遍搞定？
//
// 【解法总览】
//   解法1: BFS 层级累加 -- levelSum 不重置，自动实现反向权重
//   解法2: 两遍 DFS -- 先求 maxDepth，再算加权和
// ============================================================


// ============================================================
// 解法1: BFS 层级累加 (推荐)
//
// 核心思想:
//   BFS 逐层遍历，维护 levelSum (不重置):
//   - 每层: levelSum += 当前层所有整数
//   - 每层: totalSum += levelSum
//
//   第 d 层的整数在 levelSum 中存活 maxDepth-d+1 层,
//   被累加到 totalSum 恰好 maxDepth-d+1 次 = 反向权重
//
// 过程可视化 (nestedList = [1,[4,[6]]]):
//
//   层1: 整数={1}, levelSum=0+1=1, totalSum=0+1=1
//   层2: 整数={4}, levelSum=1+4=5, totalSum=1+5=6
//   层3: 整数={6}, levelSum=5+6=11, totalSum=6+11=17
//
//   1被累加3次, 4被累加2次, 6被累加1次
//   = 1*3 + 4*2 + 6*1 = 17 ✓
//
// 时间: O(n) -- 每个元素访问一次
// 空间: O(n) -- 队列最大宽度
// ============================================================
class Solution {
public:
    int depthSumInverse(vector<NestedInteger>& nestedList) {
        int totalSum = 0;   // 最终答案
        int levelSum = 0;   // 累积的层级和（不重置!）

        // 初始化队列
        queue<NestedInteger> q;
        for (auto& ni : nestedList) {
            q.push(ni);
        }

        while (!q.empty()) {
            int size = q.size();

            for (int i = 0; i < size; i++) {
                NestedInteger curr = q.front();
                q.pop();

                if (curr.isInteger()) {
                    // 整数: 累加到 levelSum
                    levelSum += curr.getInteger();
                } else {
                    // 列表: 子元素入队
                    for (auto& child : curr.getList()) {
                        q.push(child);
                    }
                }
            }

            // 每层结束后累加（levelSum 包含所有之前层的整数）
            totalSum += levelSum;
        }

        return totalSum;
    }
};


// ============================================================
// 解法2: 两遍 DFS
//
// 思路:
//   第一遍 DFS: 求 maxDepth
//   第二遍 DFS: 用 maxDepth 计算加权和
//     weight = maxDepth - depth + 1
//     sum += integer * weight
//
// 过程可视化 (nestedList = [[1,1],2,[1,1]]):
//
//   第一遍: maxDepth = 2
//     [1,1] -> depth=2, 2 -> depth=1, [1,1] -> depth=2
//
//   第二遍:
//     2 at depth=1: weight=2-1+1=2, 贡献 2*2=4
//     1 at depth=2: weight=2-2+1=1, 贡献 1*1=1 (x4个)
//     total = 4 + 1*4 = 8 ✓
//
// 时间: O(n)
// 空间: O(d) -- d 为最大嵌套深度，递归栈
// ============================================================
class Solution2 {
public:
    int depthSumInverse(vector<NestedInteger>& nestedList) {
        int maxDepth = getMaxDepth(nestedList, 1);
        return weightedSum(nestedList, 1, maxDepth);
    }

private:
    // 第一遍: 求最大深度
    int getMaxDepth(vector<NestedInteger>& list, int depth) {
        int maxD = depth;
        for (auto& ni : list) {
            if (!ni.isInteger()) {
                // getList() 返回 const 引用，需要处理
                auto& children = ni.getList();
                maxD = max(maxD, getMaxDepth(
                    const_cast<vector<NestedInteger>&>(children),
                    depth + 1));
            }
        }
        return maxD;
    }

    // 第二遍: 计算反向加权和
    int weightedSum(vector<NestedInteger>& list, int depth, int maxDepth) {
        int sum = 0;
        for (auto& ni : list) {
            if (ni.isInteger()) {
                sum += ni.getInteger() * (maxDepth - depth + 1);
            } else {
                auto& children = ni.getList();
                sum += weightedSum(
                    const_cast<vector<NestedInteger>&>(children),
                    depth + 1, maxDepth);
            }
        }
        return sum;
    }
};


// ============================================================
// 【解法对比】
//   解法1 (BFS累加): O(n) 时间, O(n) 空间 -- 一遍搞定, 最推荐
//   解法2 (两遍DFS): O(n) 时间, O(d) 空间 -- 直观但需两遍
//
// 【易错点】
//   1. BFS 中 levelSum 重置: 绝对不能在每层开始时置零!
//      这是核心技巧 -- levelSum 累积使浅层整数被多次叠加。
//   2. 深度起始值不一致: 深度从1开始还是0开始要统一。
//      BFS 法不依赖 depth, 天然正确。
//   3. DFS const 引用: getList() 返回 const vector<>&，
//      需要 const_cast 或改用 const 参数。
//   4. 空列表处理: 空列表不贡献整数也不增加有效深度。
//   5. 负数整数: 权重乘以负数是合法的，不要abs。
//
// 【面试追问】
//   Q1: BFS 为什么不需要知道 maxDepth?
//       → levelSum 不重置，浅层整数在后续每层都会被再加一次。
//         第d层整数自动被累加 maxDepth-d+1 次 = 反向权重。
//
//   Q2: 能否用一遍 DFS 完成?
//       → 可以。用数学转换: 反向和 = (maxDepth+1)*flatSum - 正向和。
//         但需要一遍 DFS 同时求 maxDepth, flatSum, 和正向加权和,
//         实际上也是变相的两遍（先求maxDepth再回溯算）。
//
//   Q3: 与 LC 339 (正向加权和) 的关系?
//       → 339 的结果记为 S_fwd，所有整数之和为 F。
//         本题结果 = (maxDepth+1)*F - S_fwd。
//         如果已有 339 的代码，加一行转换即可。
// ============================================================
