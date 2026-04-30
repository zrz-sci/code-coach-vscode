# 314. 二叉树的垂直遍历

## 核心思路

给每个节点分配一个"列号"（根节点为0，左孩子列号-1，右孩子列号+1），然后按列号从小到大收集结果。关键在于**同一列内的顺序**：从上到下，同行同列则从左到右——这恰好就是 BFS 的天然遍历顺序。

## 思维链

1. **读完题第一反应**：每个节点有一个"垂直位置"（列号），根节点列号为0，往左走-1，往右走+1。我需要把同一列的节点收集在一起。

2. **怎么给节点编列号？** 无论 DFS 还是 BFS，都可以在遍历时传递当前列号。用哈希表 `map<int, vector<int>>` 把同一列的值收集起来。

3. **关键问题：同一列内的顺序是什么？** 题目要求"从上到下"，同行同列"从左到右"。这就是 BFS（层序遍历）的天然顺序！BFS 逐层处理，同一层内先访问左再访问右，完美满足要求。

4. **DFS 能不能做？** 可以，但 DFS 不保证按"行"的顺序遍历，所以需要额外记录每个节点的行号，最后排序。代码更复杂，且多了排序开销。

5. **最终方案：BFS + HashMap**。BFS 遍历时维护每个节点的列号，用 map 按列号收集，最后按列号从小到大输出。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS + 排序 | DFS记录(col, row, val)，排序后分组 | O(n log n) | O(n) | 能说出即可 |
| BFS + TreeMap | BFS天然保证行序，TreeMap按列排序 | O(n log n) | O(n) | ⭐ 必须写出 |
| BFS + HashMap + minCol/maxCol | 用min/max列号代替TreeMap | O(n) | O(n) | 加分项 |

## 关键提示

- **列号编码**：根节点列号=0，左孩子=父列号-1，右孩子=父列号+1。想象把树投影到一条水平线上。
  ```
  //       3(col=0)
  //      / \
  //   9(-1) 20(+1)
  //         / \
  //      15(0) 7(+2)
  //
  // 按列号分组: {-1:[9], 0:[3,15], 1:[20], 2:[7]}
  ```

- **为什么 BFS 优于 DFS？** 同一列中，题目要求先上后下、同层先左后右。BFS 逐层、从左到右遍历，天然满足这个顺序，无需额外排序。DFS 则需要记录行号并排序。

- **列号的范围**：不需要提前知道，遍历时用 `minCol` 和 `maxCol` 跟踪即可，最后从 `minCol` 到 `maxCol` 依次输出。

- **注意区分本题(314)和 987 题**：987 题要求同行同列时按值排序，本题要求按从左到右（即 BFS 访问顺序）。这个区别决定了本题 BFS 更优。

- **空树边界**：root 为空时直接返回空数组。

## 解法详解

### 解法1: DFS + HashMap + 排序 — O(n log n) / O(n)

**思考过程**：最直觉的想法——DFS 遍历整棵树，给每个节点记录 (列号, 行号, 值)，然后按 (列号, 行号) 排序分组。

```cpp
class Solution {
public:
    vector<vector<int>> verticalOrder(TreeNode* root) {
        if (!root) return {};
        
        // 收集 (col, row, val) 三元组
        vector<tuple<int,int,int>> nodes;
        
        // DFS 遍历，记录每个节点的列号和行号
        function<void(TreeNode*, int, int)> dfs = [&](TreeNode* node, int col, int row) {
            if (!node) return;
            nodes.push_back({col, row, node->val});
            dfs(node->left, col - 1, row + 1);
            dfs(node->right, col + 1, row + 1);
        };
        dfs(root, 0, 0);
        
        // 按 (col, row) 排序
        // 注意：同col同row的节点，题目要求从左到右
        // DFS 的先左后右保证了同 (col, row) 的相对顺序正确吗？
        // 不一定！所以需要稳定排序，且只按 (col, row) 排序
        stable_sort(nodes.begin(), nodes.end(), [](auto& a, auto& b) {
            if (get<0>(a) != get<0>(b)) return get<0>(a) < get<0>(b);
            return get<1>(a) < get<1>(b);
        });
        
        // 分组输出
        vector<vector<int>> result;
        int prevCol = INT_MIN;
        for (auto& [col, row, val] : nodes) {
            if (col != prevCol) {
                result.push_back({});
                prevCol = col;
            }
            result.back().push_back(val);
        }
        return result;
    }
};
```

**关键点**：
- DFS 不保证同一列中节点按行号有序，必须排序。
- 同行同列的节点，题目要求"从左到右"，但 DFS 中左右子树的交叉路径可能打乱这个顺序。需要 `stable_sort` 只按 (col, row) 排序来尽量维持，但严格来说 DFS 在某些边界情况下仍可能不正确（见易错点）。

### 解法2: BFS + TreeMap — O(n log n) / O(n) ⭐ 面试首选

**从解法1优化**：DFS 的瓶颈是需要额外排序来保证行序。BFS 天然按层遍历，同层先左后右，直接满足题目的顺序要求，无需记录行号和排序。

```
// BFS 遍历过程（示例2）:
//         3(col=0)
//        / \
//     9(-1) 8(+1)
//    / \   / \
//  4(-2) 0(0) 1(0) 7(+2)
//
// BFS 队列变化:
// 初始:   [(3, col=0)]
// 第1层:  弹出3 → 加入9(col=-1), 8(col=1)
// 第2层:  弹出9 → 加入4(col=-2), 0(col=0)
//         弹出8 → 加入1(col=0), 7(col=2)
//
// HashMap 收集过程:
// col=-2: [4]
// col=-1: [9]
// col= 0: [3, 0, 1]  ← 0在1前面，因为BFS先处理9(左)再处理8(右)
// col= 1: [8]
// col= 2: [7]
```

```cpp
class Solution {
public:
    vector<vector<int>> verticalOrder(TreeNode* root) {
        if (!root) return {};
        
        // 用 map（有序）按列号自动排序
        map<int, vector<int>> colMap;
        
        // BFS: 队列中存 (节点, 列号)
        queue<pair<TreeNode*, int>> q;
        q.push({root, 0});
        
        while (!q.empty()) {
            auto [node, col] = q.front();
            q.pop();
            
            colMap[col].push_back(node->val);
            
            // 先左后右，保证同行同列从左到右
            if (node->left)  q.push({node->left, col - 1});
            if (node->right) q.push({node->right, col + 1});
        }
        
        // map 已按 key(列号) 排序，直接提取
        vector<vector<int>> result;
        for (auto& [col, vals] : colMap) {
            result.push_back(vals);
        }
        return result;
    }
};
```

**关键点**：BFS 的遍历顺序天然满足"上到下、左到右"，所以我们只需要用一个按列号排序的 map，BFS 中直接 push_back 即可。

### 解法3: BFS + HashMap + minCol/maxCol — O(n) / O(n)

**从解法2优化**：解法2用了 `map`（红黑树），每次插入 O(log n)。其实我们可以用普通 `unordered_map`，同时追踪 `minCol` 和 `maxCol`，最后从 min 到 max 遍历输出，避免了排序开销。

```cpp
class Solution {
public:
    vector<vector<int>> verticalOrder(TreeNode* root) {
        if (!root) return {};
        
        unordered_map<int, vector<int>> colMap;
        int minCol = 0, maxCol = 0;
        
        queue<pair<TreeNode*, int>> q;
        q.push({root, 0});
        
        while (!q.empty()) {
            auto [node, col] = q.front();
            q.pop();
            
            colMap[col].push_back(node->val);
            minCol = min(minCol, col);
            maxCol = max(maxCol, col);
            
            if (node->left)  q.push({node->left, col - 1});
            if (node->right) q.push({node->right, col + 1});
        }
        
        // 从 minCol 到 maxCol 依次输出，保证列号有序
        vector<vector<int>> result;
        for (int col = minCol; col <= maxCol; col++) {
            result.push_back(colMap[col]);
        }
        return result;
    }
};
```

**关键点**：用 `minCol/maxCol` 追踪列号范围，避免使用有序 map，总体时间复杂度降为 O(n)。

## 解法对比

| | DFS + 排序 | BFS + TreeMap | BFS + HashMap |
|---|---|---|---|
| 时间 | O(n log n) | O(n log n) | **O(n)** |
| 空间 | O(n) | O(n) | O(n) |
| 正确性 | DFS 在同行同列时可能乱序，需小心 | ✅ BFS 天然正确 | ✅ BFS 天然正确 |
| 代码量 | 多（需记录row，排序） | 简洁 | 略多（维护min/max） |
| 面试推荐 | ❌ | ⭐ 首选（清晰正确） | 加分（追问优化时给出） |

**核心洞察**：本题的"从上到下、同行从左到右"这个顺序要求，恰好就是 BFS 的遍历顺序。选择 BFS 就自动满足了排序需求，这是本题最关键的思维突破点。

## 易错点

1. **DFS 同行同列顺序错误**
   - ✗ 用 DFS 遍历，同 (col, row) 的节点直接按 DFS 序输出
   - ✓ DFS 无法保证同行同列从左到右（考虑一个节点从右子树经过多次左转到达某位置），必须用 BFS 或者给 DFS 加更多元数据排序
   
   ```
   //       1(0,0)
   //      / \
   //    2(-1,1)  3(1,1)
   //      \       /
   //     5(0,2) 9(0,2)    ← 同col=0, 同row=2
   // DFS先序: 先访问5再访问9 ✓（这里碰巧对了）
   // 但如果结构不同，DFS可能先右后左到达同位置
   ```

2. **忘记处理空树**
   - ✗ 直接 BFS，root 为 null 时队列操作出错
   - ✓ 开头加 `if (!root) return {};`

3. **用 unordered_map 但忘记排序输出**
   - ✗ 直接遍历 unordered_map 输出（无序！）
   - ✓ 要么用 `map`（有序），要么用 minCol/maxCol 遍历

4. **列号范围计算错误**
   - ✗ 假设列号范围是 [-n, n]，浪费空间
   - ✓ 动态维护 minCol 和 maxCol

## 面试追问

**Q1: 为什么选 BFS 而不是 DFS？**
BFS 逐层遍历、同层先左后右，天然满足"从上到下、同行从左到右"的顺序要求。DFS 遍历顺序不保证这一点，需要额外排序。

**Q2: 能否将 TreeMap 优化掉？时间复杂度能做到 O(n) 吗？**
可以。用 unordered_map + 维护 minCol/maxCol，最后从 minCol 到 maxCol 线性遍历，总复杂度 O(n)。（即解法3）

**Q3: 如果要求同行同列的节点按值排序呢？（这就变成了 LeetCode 987）**
那 BFS 的天然顺序就不够了。需要记录 (col, row, val) 三元组，排序时 col 优先，row 其次，val 最后。这时候 DFS 或 BFS 都需要排序，复杂度 O(n log n)。

**Q4: 如果树非常深（可能导致递归栈溢出），DFS 解法怎么办？**
用 BFS 就天然避免了递归深度问题。BFS 只用队列，空间与树的最大宽度成正比，不受深度限制。

## 相关题型

- **102. 二叉树的层序遍历** — 复用 BFS 模板，本题在此基础上增加了"列号"维度
- **987. 二叉树的垂直遍历（Hard）** — 本题的升级版，区别：987 要求同行同列按值排序，需要将排序条件从 `(col, BFS顺序)` 改为 `(col, row, val)`
- **199. 二叉树的右视图** — 同样是 BFS + 特定维度的分组，复用层序遍历框架