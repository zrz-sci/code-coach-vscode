# 1376. 通知所有员工所需的时间

## 核心思路

这道题本质上是在一棵多叉树（组织架构树）中，求**从根到所有叶子节点的最长路径**（路径权重是每个节点的 `informTime`）。

## 思维链

1. **读完题第一反应**：消息从 CEO（根节点）往下传播，每个管理者通知下属需要时间，最终所有人都收到消息的总时间 = 消息传到最后一个人的时间。这就是树上从根到叶子的最长加权路径。

2. **怎么建树？** 题目给的是 `manager` 数组（每个员工指向自己的上司），这是"孩子指向父亲"的表示。要做 DFS/BFS 需要"父亲找孩子"，所以先建邻接表：遍历 `manager`，把每个员工加入其上司的子节点列表。

3. **从根 DFS 下去**：对每个节点，通知它所有直属下属需要 `informTime[node]` 时间，然后这些下属**同时**开始通知各自的下属。所以当前节点的总传播时间 = `informTime[node]` + max(子节点的传播时间)。

4. **BFS 也行**：层序遍历，在队列中传播"到达该节点时已经花了多少时间"，最终所有叶子节点中最大的到达时间就是答案。

5. **还有一种自底向上的做法**：不建树，直接从每个叶子节点沿着 `manager` 数组往上走到根，累加路径上的 `informTime`，取所有路径的最大值。加上记忆化可以避免重复计算。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS（自顶向下） | 建邻接表，从根 DFS，取子树最大值 | O(n) | O(n) | ⭐ 必须写出 |
| BFS（层序遍历） | 建邻接表，BFS 传播累计时间 | O(n) | O(n) | ⭐ 必须写出 |
| 自底向上 + 记忆化 | 不建树，从叶子往上走到根 | O(n) | O(n) | 加分项 |

## 关键提示

1. **建树是关键一步**：`manager` 数组是"孩子→父亲"的映射，你需要反转成"父亲→孩子列表"才能从根往下遍历。

2. **并行传播**：一个管理者通知完所有直属下属后，这些下属是**同时**开始通知各自的下属的。所以取 `max` 而不是 `sum`。

3. **叶子节点的 `informTime` 一定是 0**：因为没有下属需要通知。这是天然的递归终止条件。

4. **树的结构示意**（以示例2为例）：
```
   manager = [2, 2, -1, 2, 2, 2]
   informTime = [0, 0, 1, 0, 0, 0]

   建出的树：
          2 (informTime=1)
       / | | \ \
      0  1  3  4  5    (都是叶子, informTime=0)

   从根节点2出发，通知所有下属花 1 分钟
   下属都是叶子节点，不需要再通知
   答案 = 1
```

5. **一棵更深的树的例子**：
```
   假设：
          0 (time=5)
         / \
        1    2 (time=3)
  (time=2)    \
       |       3 (time=0, 叶子)
       4
  (time=0, 叶子)

   路径0→1→4: 5+2+0 = 7
   路径0→2→3: 5+3+0 = 8
   答案 = max(7, 8) = 8
```

## 解法详解

### 解法1: DFS 自顶向下 — O(n) / O(n) ⭐ 面试首选

**思考过程**：组织架构就是一棵多叉树，消息从根传到叶子，每条路径的传播时间是路径上 `informTime` 之和。我们需要所有路径中最长的那条。自然想到 DFS：对于当前节点，递归求出每个子树的最大传播时间，取最大值再加上当前节点的 `informTime`。

```cpp
class Solution {
public:
    int numOfMinutes(int n, int headID, vector<int>& manager, vector<int>& informTime) {
        // 1. 建邻接表：父亲 → 孩子列表
        vector<vector<int>> children(n);
        for (int i = 0; i < n; i++) {
            if (manager[i] != -1) {
                children[manager[i]].push_back(i);
            }
        }
        // 2. 从根节点开始 DFS
        return dfs(headID, children, informTime);
    }

private:
    int dfs(int node, vector<vector<int>>& children, vector<int>& informTime) {
        // 叶子节点：没有下属，informTime[node]=0，直接返回0
        // 实际上不需要特判，下面的循环不执行时 maxChild=0，结果就是 informTime[node]=0
        int maxChild = 0;
        for (int child : children[node]) {
            // 每个子节点独立并行传播，取最大值
            maxChild = max(maxChild, dfs(child, children, informTime));
        }
        // 当前节点通知下属的时间 + 下属中最慢那条链的时间
        return informTime[node] + maxChild;
    }
};
```

**关键点**：
- 叶子节点不需要特判：`children[node]` 为空时循环不执行，`maxChild = 0`，加上叶子的 `informTime[node] = 0`，自然返回 0。
- 是 `max` 不是 `sum`，因为下属是并行传播的。

---

### 解法2: BFS 层序遍历 — O(n) / O(n) ⭐ 面试首选

**思考过程**：BFS 天然模拟"逐层传播"的过程。我们在队列中记录 `(节点, 到达该节点的累计时间)`，每次出队时将累计时间传播给子节点。最终所有节点中的最大累计时间就是答案。

```cpp
class Solution {
public:
    int numOfMinutes(int n, int headID, vector<int>& manager, vector<int>& informTime) {
        // 1. 建邻接表
        vector<vector<int>> children(n);
        for (int i = 0; i < n; i++) {
            if (manager[i] != -1) {
                children[manager[i]].push_back(i);
            }
        }

        // 2. BFS：队列存 {节点, 到达该节点的累计时间}
        queue<pair<int, int>> q;
        q.push({headID, 0});
        int ans = 0;

        while (!q.empty()) {
            auto [node, time] = q.front();
            q.pop();
            // 更新全局最大值（叶子节点时 time 就是到达它的时间）
            ans = max(ans, time);
            for (int child : children[node]) {
                // 子节点收到消息的时间 = 父节点的累计时间 + 父节点的通知时间
                q.push({child, time + informTime[node]});
            }
        }
        return ans;
    }
};
```

**关键点**：
- 队列中传递的是**累计时间**，不是当前节点的 `informTime`。
- 答案不是最后出队的时间（BFS 不保证最后出队的是最深节点），而是所有节点中 `time` 的最大值。

---

### 解法3: 自底向上 + 记忆化 — O(n) / O(n)

**从不同角度思考**：如果不建树呢？我们可以从每个叶子节点出发，沿着 `manager` 数组往上走到根，累加路径上每个节点的 `informTime`。对所有叶子取最大值。为了避免重复计算（多个叶子共享部分路径），使用记忆化。

```cpp
class Solution {
public:
    int numOfMinutes(int n, int headID, vector<int>& manager, vector<int>& informTime) {
        // memo[i] = 从根到节点i的总传播时间
        // -1 表示还没算过
        vector<int> memo(n, -1);
        memo[headID] = 0; // 根节点：消息已经在这里了，累计时间为0

        int ans = 0;
        for (int i = 0; i < n; i++) {
            ans = max(ans, getTime(i, manager, informTime, memo));
        }
        return ans;
    }

private:
    int getTime(int node, vector<int>& manager, vector<int>& informTime, vector<int>& memo) {
        if (memo[node] != -1) return memo[node];
        // 从根到当前节点的时间 = 从根到父节点的时间 + 父节点通知下属的时间
        memo[node] = getTime(manager[node], manager, informTime, memo) + informTime[manager[node]];
        return memo[node];
    }
};
```

**关键点**：
- `memo[i]` 存的是"从根到节点 i 的传播时间"，不是"从节点 i 到叶子的时间"。
- 递归关系：`到达i的时间 = 到达i的父亲的时间 + 父亲的informTime`。
- 不需要特别找叶子节点，直接遍历所有节点取最大值即可。

## 解法对比

| | DFS 自顶向下 | BFS | 自底向上+记忆化 |
|---|---|---|---|
| 建树 | 需要 | 需要 | **不需要** |
| 方向 | 从根到叶 | 从根到叶 | 从叶到根 |
| 实现 | 递归 | 迭代 | 递归 |
| 代码量 | 最简洁 | 中等 | 中等 |
| 栈深度风险 | 树深 | 无（迭代） | 树深 |
| 面试推荐 | ⭐ 首选 | ⭐ 追问迭代时用 | 展示不同思维 |

**什么时候选哪个**：
- 面试首选 DFS，代码最简洁，思路最直观
- 如果面试官追问"能否不用递归"，给出 BFS
- 如果追问"能否不建树"，给出自底向上解法

## 易错点

1. **取 max 还是 sum**：
   - ✗ `total += dfs(child, ...)` — 下属是**同时**传播的，不是串行
   - ✓ `maxChild = max(maxChild, dfs(child, ...))` — 取最慢的那条链

2. **BFS 中 ans 的更新位置**：
   - ✗ 只在出队最后一个元素时记录 — BFS 最后出队的不一定是最深叶子
   - ✓ 每次出队都更新 `ans = max(ans, time)`

3. **自底向上解法忘记加父节点的 informTime**：
   - ✗ `memo[node] = getTime(manager[node], ...) + informTime[node]` — 加了自己的 time，但自己的 time 应该留给自己的子树用
   - ✓ `memo[node] = getTime(manager[node], ...) + informTime[manager[node]]` — 加的是父亲通知下属（包括自己）的时间

4. **建邻接表时忘记跳过 headID**：
   - `manager[headID] = -1`，如果不跳过会尝试访问 `children[-1]`
   - 一定要判断 `if (manager[i] != -1)`

## 面试追问

**Q1: 暴力解能过吗？这道题有暴力解吗？**
DFS/BFS 本身就是 O(n) 的，已经是最优时间复杂度。真要说"暴力"的话，可以不建邻接表，对每个叶子节点沿 `manager` 向上走到根——这也是 O(n) 但常数大且代码不优雅。建邻接表是关键的预处理步骤。

**Q2: 如果树非常深（n=10^5 的链状树），递归会栈溢出，怎么办？**
改用 BFS（解法2），完全迭代，不存在栈深度问题。或者用显式栈的 DFS 迭代写法。

**Q3: 如果不只是通知时间，还要输出"最后收到消息的员工是谁"呢？**
在 DFS/BFS 中额外记录取到最大值时对应的节点 ID。DFS 中递归返回 `{time, nodeID}` 的 pair，BFS 中在更新 `ans` 时同步记录 `lastEmployee`。

**Q4: 如果某些员工可以"跳级"直接联系非直属下属呢？**
那就不是树结构了，变成了一般的 DAG（有向无环图）。需要用拓扑排序 + DP 来求最长路径。

## 相关题型

- **104. 二叉树的最大深度** — 本题的简化版，多叉树的"最大深度"带权值版本。复用同样的"DFS 递归取子树 max"框架
- **559. N 叉树的最大深度** — 更直接的对应，区别：559 每层深度是 1，本题每层深度是 `informTime[node]`
- **543. 二叉树的直径** — 同样是树上路径问题，但需要考虑经过根的路径（取两个最大子树之和），本题只需要根到叶子的最长路径
- **1129. 颜色交替的最短路径** — BFS 在图上的应用，传播累计权重的思路类似