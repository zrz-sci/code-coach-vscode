# 339. 嵌套列表加权和

## 核心思路

本质上是对一个**树形结构**进行遍历，每个整数的贡献 = 值 × 它所在的嵌套深度。嵌套列表天然是递归定义的，用 DFS 或 BFS 按层遍历即可。

## 思维链

1. **读完题第一反应**：嵌套列表像俄罗斯套娃，每一层里的元素要么是整数要么又是一个列表。我需要知道每个整数在第几层，然后 `值 × 深度` 求和。

2. **这和什么结构类似？** 嵌套列表本质上是一棵**多叉树**——每个列表是一个内部节点，每个整数是叶子节点。"深度"就是树中从根到该节点的路径长度。

3. **最直觉的做法**：递归遍历（DFS）。遇到整数就 `值 × 当前深度` 累加；遇到列表就递归进去，深度 +1。这就是树的前序遍历。

4. **能不能用 BFS？** 可以！用队列逐层处理，每处理一层 depth 就 +1。这和二叉树的层序遍历完全一样的思路。

5. **有没有瓶颈？** 无论 DFS 还是 BFS，都是遍历每个元素恰好一次，时间 O(N)。这道题本身就是基础遍历题，没有需要优化的瓶颈。关键在于**正确处理递归结构**和**正确传递深度信息**。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS 递归 | 递归遍历，传递当前深度 | O(N) | O(D) | ⭐ 必须写出 |
| BFS 层序 | 队列逐层处理，层数即深度 | O(N) | O(N) | 能说出即可 |

> N = 所有元素（整数+列表）总数，D = 最大嵌套深度

## 关键提示

1. **把嵌套列表想象成一棵树**：最外层是第 1 层（depth=1），每嵌套一层 depth +1。
   ```
   示例: [[1,1], 2, [1,1]]  想象成树:
   
         root (depth=1)
        /  |  \
      [1,1] 2  [1,1]      ← 2 在 depth=1
      / \       / \
     1   1     1   1       ← 这些 1 在 depth=2
   ```

2. **NestedInteger 接口**有三个方法：`isInteger()`、`getInteger()`、`getList()`。先判断类型再操作，不要搞混。

3. **DFS 的递归参数**：不需要返回值累加，传入 depth 参数，直接在递归中累加到一个总和变量即可。或者让递归函数返回子树的加权和。

4. **BFS 的技巧**：队列中存 `NestedInteger` 对象。遇到列表时，把列表的子元素全部入队（相当于"展开一层"），此时深度 +1。

5. **注意初始深度是 1 不是 0**：`nestedList` 本身是最外层列表，里面的元素起始 depth = 1。

## 解法详解

### 解法1: DFS 递归 — O(N) / O(D) ⭐ 面试首选

**思考过程**：嵌套列表是递归定义的（列表里可以包含列表），用递归处理最自然。对每个元素：如果是整数，贡献 = 值 × 深度；如果是列表，递归处理其子元素，深度 +1。

```
遍历过程 (示例: [1, [4, [6]]]):

dfs(nestedList, depth=1):
  ├── 元素 1 (整数) → 1 × 1 = 1
  └── 元素 [4, [6]] (列表) → dfs([4, [6]], depth=2):
        ├── 元素 4 (整数) → 4 × 2 = 8
        └── 元素 [6] (列表) → dfs([6], depth=3):
              └── 元素 6 (整数) → 6 × 3 = 18

总和 = 1 + 8 + 18 = 27 ✓
```

```cpp
/**
 * // This is the interface that allows for creating nested lists.
 * // You should not implement it, or speculate about its implementation
 * class NestedInteger {
 *   public:
 *     bool isInteger() const;
 *     int getInteger() const;
 *     const vector<NestedInteger> &getList() const;
 * };
 */
class Solution {
public:
    int depthSum(vector<NestedInteger>& nestedList) {
        return dfs(nestedList, 1);
    }
    
    int dfs(vector<NestedInteger>& list, int depth) {
        int sum = 0;
        for (auto& ni : list) {
            if (ni.isInteger()) {
                // 整数：值 × 当前深度
                sum += ni.getInteger() * depth;
            } else {
                // 列表：递归进去，深度 + 1
                sum += dfs(ni.getList(), depth + 1);
            }
        }
        return sum;
    }
};
```

**关键点**：
- 初始调用 `dfs(nestedList, 1)`，depth 从 1 开始
- `getList()` 返回的是引用，直接传给递归即可
- 递归终止：当 list 为空时 for 循环不执行，自然返回 0

### 解法2: BFS 层序遍历 — O(N) / O(N)

**从 DFS 到 BFS**：DFS 用递归栈隐式地追踪深度，BFS 用队列显式地逐层处理。每处理完一层，depth +1。遇到列表元素时，把它的子元素全部入队到下一层。

```
BFS 过程 (示例: [[1,1], 2, [1,1]]):

初始队列: [[1,1], 2, [1,1]]    depth=1
  → 2 是整数: sum += 2×1 = 2
  → [1,1] 是列表: 展开, 把 1,1 入队
  → [1,1] 是列表: 展开, 把 1,1 入队

队列: [1, 1, 1, 1]              depth=2
  → 1 是整数: sum += 1×2 = 2
  → 1 是整数: sum += 1×2 = 2
  → 1 是整数: sum += 1×2 = 2
  → 1 是整数: sum += 1×2 = 2

总和 = 2 + 2 + 2 + 2 + 2 = 10 ✓
```

```cpp
class Solution {
public:
    int depthSum(vector<NestedInteger>& nestedList) {
        int sum = 0, depth = 1;
        queue<NestedInteger> q;
        
        // 把最外层所有元素入队
        for (auto& ni : nestedList) {
            q.push(ni);
        }
        
        while (!q.empty()) {
            int size = q.size(); // 当前层的元素数
            for (int i = 0; i < size; i++) {
                NestedInteger curr = q.front();
                q.pop();
                if (curr.isInteger()) {
                    sum += curr.getInteger() * depth;
                } else {
                    // 展开列表，子元素入队到下一层
                    for (auto& child : curr.getList()) {
                        q.push(child);
                    }
                }
            }
            depth++; // 处理完一层，深度 +1
        }
        return sum;
    }
};
```

**关键点**：
- 用 `size` 变量锁定当前层的元素数量，这样新入队的子元素属于下一层
- BFS 中列表元素被"展开"——它自己不贡献值，只是把子元素推入队列

## 解法对比

| | DFS 递归 | BFS 层序 |
|---|---|---|
| 时间 | O(N) | O(N) |
| 空间 | O(D) 递归栈深度 | O(N) 队列最大宽度 |
| 代码量 | 更短，更自然 | 稍长，但逻辑清晰 |
| 面试推荐 | ⭐ 首选 | 作为 follow-up |

**什么时候选哪个？**
- 默认写 DFS，代码简洁且空间通常更优（D ≤ 50 远小于 N）
- 如果面试官追问"不用递归怎么做"，切换到 BFS

## 易错点

1. **初始深度写成 0**：
   - ✗ `dfs(nestedList, 0)` → 最外层整数贡献为 0，答案偏小
   - ✓ `dfs(nestedList, 1)` → 最外层元素的深度是 1

2. **BFS 忘记用 size 锁定当前层**：
   - ✗ 直接 `while (!q.empty())` 一个个处理，不区分层 → depth 无法正确递增
   - ✓ 每层开始前 `int size = q.size()`，for 循环只处理 size 个

3. **对列表元素调用 getInteger()**：
   - ✗ 不判断 `isInteger()` 就调用 `getInteger()` → 未定义行为
   - ✓ 先 `if (ni.isInteger())` 再取值

4. **DFS 中忘记累加子问题的返回值**：
   - ✗ `dfs(ni.getList(), depth + 1)` 没有 `sum +=` → 子树结果丢失
   - ✓ `sum += dfs(ni.getList(), depth + 1)`

## 面试追问

**Q1: 时间和空间复杂度分别是多少？DFS 和 BFS 有什么区别？**
→ 时间都是 O(N)，遍历每个元素一次。空间上 DFS 是 O(D)（D = 最大深度，最坏 O(N)），BFS 是 O(N)（队列可能存储整层元素）。一般嵌套结构"深而窄"时 DFS 更优，"浅而宽"时 BFS 更优。

**Q2: 如果改成"反向加权"——最深层权重为 1，最浅层权重最大——怎么做？**
→ 这就是 [364. 嵌套列表加权和 II]。两种方法：(1) 先遍历一次求最大深度 maxDepth，再遍历一次用 (maxDepth - depth + 1) 作为权重；(2) 用 BFS 逐层累加不乘权重，每层把之前所有层的 sum 再加一次（巧妙的数学技巧）。

**Q3: 如果 NestedInteger 不提供 isInteger() 接口，只提供 getInteger()（列表时返回 null）和 getList()（整数时返回 null），代码怎么改？**
→ 用 `getInteger() != null` 来判断类型，逻辑完全一样，只是判断条件换了。这考察对接口的灵活运用。

## 相关题型

- **[364. 嵌套列表加权和 II](https://leetcode.com/problems/nested-list-weight-sum-ii/)** — 反向加权，复用本题的遍历框架，区别在于权重计算方式。见 Q2 的讨论。
- **[341. 扁平化嵌套列表迭代器](https://leetcode.com/problems/flatten-nested-list-iterator/)** — 同样的 NestedInteger 结构，但要求设计迭代器逐个输出所有整数。复用 DFS 的递归展开思路，但用栈模拟。
- **[104. 二叉树的最大深度](https://leetcode.com/problems/maximum-depth-of-binary-tree/)** — 同样的"递归求深度"框架，区别：树是二叉固定结构，嵌套列表是多叉。
- **[690. 员工的重要性](https://leetcode.com/problems/employee-importance/)** — 类似的 DFS/BFS 遍历嵌套结构求加权和。