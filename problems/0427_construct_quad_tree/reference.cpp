/*
 * LeetCode 427: 建立四叉树 (Construct Quad Tree)
 * 
 * 【题目本质】
 * 分治问题：对 n×n 网格，如果区域全同则为叶节点，否则四分递归构建四叉树。
 *
 * 【解法总览】
 * 解法1: 朴素分治     — O(n² log n) / O(log n) — 直接按定义递归，面试首选
 * 解法2: 前缀和+分治  — O(n²) / O(n²)          — 用前缀和 O(1) 判断全同，最优
 * 解法3: 先递归后合并  — O(n² log n) / O(log n) — 不预检查，递归后尝试合并
 */

/*
// Definition for a Quad-Tree node.
class Node {
public:
    bool val;
    bool isLeaf;
    Node* topLeft;
    Node* topRight;
    Node* bottomLeft;
    Node* bottomRight;
    
    Node() : val(false), isLeaf(false), topLeft(NULL), topRight(NULL), 
             bottomLeft(NULL), bottomRight(NULL) {}
    Node(bool _val, bool _isLeaf) : val(_val), isLeaf(_isLeaf), 
             topLeft(NULL), topRight(NULL), bottomLeft(NULL), bottomRight(NULL) {}
    Node(bool _val, bool _isLeaf, Node* _topLeft, Node* _topRight, 
         Node* _bottomLeft, Node* _bottomRight)
        : val(_val), isLeaf(_isLeaf), topLeft(_topLeft), topRight(_topRight), 
          bottomLeft(_bottomLeft), bottomRight(_bottomRight) {}
};
*/

// ============================================================
// 解法1: 朴素分治 — 直接按定义递归
// 时间: O(n² log n)  空间: O(log n) 递归栈
//
// 【思路】
// 题目的构建规则本身就是递归定义：
//   1. 检查当前子网格是否所有值相同
//   2. 若全同 → 叶节点
//   3. 若不全同 → 分成四块递归
//
// 直接翻译成代码。递归函数参数为 (r, c, size)，
// 描述"以 (r,c) 为左上角、边长为 size 的子网格"。
//
// 示意图（以 4×4 网格为例）：
//
//   整个 4×4:
//   +---+---+
//   | 1 1 | 0 0 |   不全同 → 四分
//   | 1 1 | 0 0 |
//   +---+---+
//   | 1 1 | 1 1 |
//   | 1 1 | 1 1 |
//   +---+---+
//
//   topLeft 2×2:    topRight 2×2:
//   1 1 → 全1→叶    0 0 → 全0→叶
//   1 1              0 0
//
//   bottomLeft 2×2:  bottomRight 2×2:
//   1 1 → 全1→叶     1 1 → 全1→叶
//   1 1               1 1
//
//   结果：根节点(非叶) → 四个叶子节点
// ============================================================
class Solution1 {
public:
    Node* construct(vector<vector<int>>& grid) {
        return build(grid, 0, 0, grid.size());
    }

private:
    Node* build(vector<vector<int>>& grid, int r, int c, int size) {
        // 遍历子网格，检查是否全部相同
        if (allSame(grid, r, c, size)) {
            // 全同 → 创建叶节点，val 取区域中任意一个值
            return new Node(grid[r][c] == 1, true);
        }

        // 不全同 → 分成四个子网格递归
        // 四个子网格的坐标关系：
        //   (r, c)           (r, c+half)
        //   (r+half, c)      (r+half, c+half)
        int half = size / 2;
        Node* tl = build(grid, r,        c,        half);
        Node* tr = build(grid, r,        c + half, half);
        Node* bl = build(grid, r + half, c,        half);
        Node* br = build(grid, r + half, c + half, half);

        // 非叶节点，val 可以是任意值
        return new Node(true, false, tl, tr, bl, br);
    }

    // 检查 grid[r..r+size-1][c..c+size-1] 是否所有值相同
    bool allSame(vector<vector<int>>& grid, int r, int c, int size) {
        int first = grid[r][c];
        for (int i = r; i < r + size; i++) {
            for (int j = c; j < c + size; j++) {
                if (grid[i][j] != first) return false;
                // 提前返回，平均情况远好于最坏
            }
        }
        return true;
    }
};

// ============================================================
// 解法2: 前缀和 + 分治 — O(1) 判断区域是否全同
// 时间: O(n²)  空间: O(n²) 前缀和数组
//
// 【思路】
// 解法1的瓶颈：每次递归都要遍历子区域判断是否全同，O(size²)。
// 优化：预处理二维前缀和，O(1) 查询任意子矩形的元素和。
//   - 和为 0         → 全部是 0
//   - 和为 size*size → 全部是 1
//   - 否则           → 不全同
//
// 二维前缀和 prefix[i][j] = grid[0..i-1][0..j-1] 的元素总和
// （使用 1-indexed 简化边界处理）
//
// 查询 grid[r..r+sz-1][c..c+sz-1] 的和：
//   prefix[r+sz][c+sz] - prefix[r][c+sz] - prefix[r+sz][c] + prefix[r][c]
//
// 这样递归中每个节点只做 O(1) 工作，四叉树最多 O(n²) 个节点，
// 加上预处理 O(n²)，总时间 O(n²)。
// ============================================================
class Solution2 {
public:
    Node* construct(vector<vector<int>>& grid) {
        int n = grid.size();

        // 构建二维前缀和（1-indexed）
        vector<vector<int>> prefix(n + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                prefix[i][j] = grid[i-1][j-1]
                             + prefix[i-1][j]
                             + prefix[i][j-1]
                             - prefix[i-1][j-1];
            }
        }

        return build(grid, prefix, 0, 0, n);
    }

private:
    // 查询 grid[r..r+sz-1][c..c+sz-1] 的元素和
    // 注意：r, c 是 0-indexed 的 grid 坐标，prefix 是 1-indexed
    int regionSum(vector<vector<int>>& prefix, int r, int c, int sz) {
        return prefix[r + sz][c + sz]
             - prefix[r][c + sz]
             - prefix[r + sz][c]
             + prefix[r][c];
    }

    Node* build(vector<vector<int>>& grid, vector<vector<int>>& prefix,
                int r, int c, int size) {
        int sum = regionSum(prefix, r, c, size);

        // O(1) 判断：全 0 或全 1 → 叶节点
        if (sum == 0) {
            return new Node(false, true);
        }
        if (sum == size * size) {
            return new Node(true, true);
        }

        // 不全同 → 四分递归
        int half = size / 2;
        return new Node(true, false,
            build(grid, prefix, r,        c,        half),
            build(grid, prefix, r,        c + half, half),
            build(grid, prefix, r + half, c,        half),
            build(grid, prefix, r + half, c + half, half)
        );
    }
};

// ============================================================
// 解法3: 先递归后合并 — 自底向上风格
// 时间: O(n² log n) 最坏  空间: O(log n) 递归栈
//
// 【思路】
// 换一个角度：不预先检查是否全同，而是：
//   1. 递归到底（1×1 格子一定是叶节点）
//   2. 回溯时检查四个子节点：
//      如果四个都是叶节点且 val 相同 → 合并为一个叶节点
//      否则 → 保持为非叶节点
//
// 这种写法不需要 allSame 函数，代码更简洁。
// 缺点：最坏情况一定会递归到所有 1×1 格子。
//
// 决策树示意（2×2 网格 [[0,1],[1,0]]）：
//
//   build(0,0,2)
//   ├── build(0,0,1) → 叶(val=0)
//   ├── build(0,1,1) → 叶(val=1)
//   ├── build(1,0,1) → 叶(val=1)
//   └── build(1,1,1) → 叶(val=0)
//   四个叶节点 val 不全同 → 不合并 → 返回非叶节点
// ============================================================
class Solution3 {
public:
    Node* construct(vector<vector<int>>& grid) {
        return build(grid, 0, 0, grid.size());
    }

private:
    Node* build(vector<vector<int>>& grid, int r, int c, int size) {
        // 递归终止：1×1 一定是叶节点
        if (size == 1) {
            return new Node(grid[r][c] == 1, true);
        }

        int half = size / 2;
        Node* tl = build(grid, r,        c,        half);
        Node* tr = build(grid, r,        c + half, half);
        Node* bl = build(grid, r + half, c,        half);
        Node* br = build(grid, r + half, c + half, half);

        // 尝试合并：四个子节点都是叶节点且值相同
        if (tl->isLeaf && tr->isLeaf && bl->isLeaf && br->isLeaf
            && tl->val == tr->val && tr->val == bl->val && bl->val == br->val) {
            // 合并为一个叶节点
            // 工程上应该 delete tl, tr, bl, br 释放内存
            return new Node(tl->val, true);
        }

        // 不能合并 → 非叶节点
        return new Node(true, false, tl, tr, bl, br);
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间         | 空间    | 特点                           |
// |-----------|-------------|---------|-------------------------------|
// | 朴素分治   | O(n²logn)   | O(logn) | 直接翻译定义，面试首选           |
// | 前缀和+分治| O(n²)       | O(n²)   | 最优时间，需要额外空间           |
// | 先递归后合并| O(n²logn)   | O(logn) | 代码最简洁，自底向上风格         |
//
// 面试建议：先写解法1或3（5分钟内），被追问优化时给出解法2。
//
// 【易错点】
// 1. 子网格坐标计算：
//    ✗ topRight 写成 (r, c + size)  — 应该是 c + half
//    ✓ half = size / 2; topRight 起始列是 c + half
//
// 2. 前缀和索引偏移（解法2）：
//    ✗ prefix[r+sz][c+sz] - prefix[r-1][c+sz] — r=0 时越界！
//    ✓ 使用 1-indexed：prefix[r+sz][c+sz] - prefix[r][c+sz] - prefix[r+sz][c] + prefix[r][c]
//       其中 r, c 是 0-indexed 的 grid 坐标
//
// 3. 合并条件遗漏（解法3）：
//    ✗ 只检查 val 相同，忘了检查四个都是 isLeaf
//    ✓ 必须 tl->isLeaf && tr->isLeaf && bl->isLeaf && br->isLeaf 全部为 true
//       否则子节点是非叶节点，即使其代表的区域最终值相同也不能简单合并
//
// 4. 叶节点 val 类型：
//    ✗ new Node(grid[r][c], true) — grid 值是 int，可能产生隐式转换警告
//    ✓ new Node(grid[r][c] == 1, true) — 显式转为 bool
//
// 5. 内存泄漏（解法3合并时）：
//    合并时丢弃了四个子节点的指针，应该先 delete 再返回新节点
//    LeetCode 不检查，但面试中提一句展示工程素养
//
// 【面试追问】
// Q1(基础): allSame 遍历子区域的时间是 O(size²)，整体复杂度怎么分析？
//    → 递归树有 log n 层，每层所有子区域面积之和为 n²，所以 O(n² log n)
//
// Q2(优化): 能否把判断全同优化到 O(1)？
//    → 二维前缀和预处理 O(n²)，之后每次查询 O(1)，总时间降到 O(n²)
//
// Q3(变体): 如果不预先检查，有没有别的写法？
//    → 解法3：先递归到底再合并。1×1 是天然叶节点，回溯时四个叶节点
//      值相同就合并为一个叶节点。代码更简洁但最坏时间不变。
//
// Q4(工程): 合并时被丢弃的子节点怎么处理？
//    → C++ 中应 delete 释放内存。可以用 unique_ptr 自动管理。
//      面试时提这一点展示工程意识。
// ============================================================
