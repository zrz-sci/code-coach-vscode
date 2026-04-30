# 3600. 升级后最大生成树稳定性 (Maximize Spanning Tree Stability with Upgrades)

[LeetCode 链接](https://leetcode.com/problems/maximize-spanning-tree-stability-with-upgrades/)

## 题目理解

给定一个无向图，有 `n` 个节点（0 到 n-1）和若干边。每条边 `edges[i] = [u, v, s, must]`：
- `s`：边的**强度**（strength）
- `must = 1`：该边**必须**包含在生成树中，且**不能被升级**
- `must = 0`：该边可选，可以被**升级**（强度翻倍 `s -> 2s`），每条边最多升级一次

你最多可以执行 `k` 次升级操作。

**生成树的稳定性** = 树中所有边的**最小强度**。

目标：求所有合法生成树中可达到的**最大稳定性**。无法构成合法生成树时返回 `-1`。

```
图示理解:

示例2: n=3, edges=[[0,1,4,0],[1,2,3,0],[0,2,1,0]], k=2

    原始图:                  最优选择(升级[0,1]和[1,2]):
    
    0 ---4--- 1              0 ---8--- 1        (4->8, 升级)
    |         |              |  
    1         3              6                   (3->6, 升级)
    |         |              |
    +-------- 2              +-------- 2

    跳过边[0,2](s=1太弱)     稳定性 = min(8, 6) = 6
```

**核心挑战**：
1. must 边必须全部选中且不能成环（否则无合法生成树 -> -1）
2. 升级是翻倍操作，但每条边最多升级一次，总共最多 k 次
3. 目标是**最大化最小边权** -- 经典的"最大化最小值"模式

---

## 第一步提示：must 边的预处理

<details>
<summary>展开提示</summary>

`must = 1` 的边有两个硬性约束：
1. **必须全部包含**在生成树中
2. **不能升级**（强度固定为 s）

**预处理**：在任何主逻辑之前，先用 Union-Find 只加入所有 must 边：

```
Union-Find 预检查:

初始: 每个节点独立
   {0}  {1}  {2}

加入 must 边 [0,1]:
   {0,1}  {2}    <- OK

加入 must 边 [1,2]:
   {0,1,2}       <- OK

加入 must 边 [2,0]:
   find(2)==find(0) -> 已连通!
   -> 成环! 返回 -1
```

**检查规则**：
- 如果某条 must 边的两个端点**已经连通** -> 形成环 -> 返回 -1
- must 边数量 > n-1 -> 一定有环 -> 返回 -1
- 所有 must 边的最小强度是稳定性的一个**天然上界**

</details>

## 第二步提示：为什么用"二分答案"？

<details>
<summary>展开提示</summary>

**"最大化最小值"是经典的二分答案模式。**

设答案（稳定性）为 T，即生成树中所有边的最小强度 >= T：
- T 越大，要求越严格（更难满足）
- T 越小，要求越宽松（更容易满足）
- 存在一个**临界点**：T 以下都可行，T 以上都不可行

```
T 从小到大:

T=1  check(1) = true  (很宽松)
T=2  check(2) = true
T=3  check(3) = true
T=4  check(4) = true
T=6  check(6) = true   <-- 最大可行值 = 答案!
T=8  check(8) = false  (太严格)
```

**候选阈值 T 不是连续的，只可能出现在**：
- 所有边的原始强度 `s`
- 所有 optional 边升级后的强度 `2*s`

因为生成树的最小边权必然等于某条选中边的（原始或升级后）强度。

</details>

## 第三步提示：check(T) 的贪心验证

<details>
<summary>展开提示</summary>

给定阈值 T，判断能否构成稳定性 >= T 的生成树：

```
check(T) 三步走:

Step A: 检查所有 must 边
  - must边 s < T? -> false (不能升级，强度不够)
  - 通过则加入 Union-Find

Step B: 加入不需升级的 optional 边
  - must=0 且 s >= T 的边
  - 免费! 不消耗升级配额
  - 用 UF 判断是否连接新节点

Step C: 加入需要升级的 optional 边
  - must=0 且 s < T 但 2*s >= T 的边
  - 每使用一条消耗 1 次升级配额
  - 用 UF 判断是否连接新节点

最终检查:
  - 选中的边 >= n-1? (图连通)
  - 消耗的升级次数 <= k?
```

**贪心顺序至关重要**：必须**优先**加入不需升级的边（Step B），再加入需升级的边（Step C）。这样可以**节省升级配额**给真正需要的边。

```
贪心正确性 (交换论证):

假设某方案 X 升级了边 a (s_a >= T, 不需要升级)
而没有升级边 b (s_b < T, 必须升级才能用)

方案 Y: 不升级 a, 改为升级 b
  -> 边 a 仍然可用 (s_a >= T)
  -> 边 b 现在也可用 (2*s_b >= T)
  -> 升级次数不变, 可用边不减少
  -> 方案 Y 不差于 X
```

</details>

## 第四步提示：候选值收集与二分搜索

<details>
<summary>展开提示</summary>

**收集候选值**：
```cpp
set<int> candidates;
for (auto& e : edges) {
    candidates.insert(e[2]);           // 原始强度 s
    if (e[3] == 0)                     // optional 边
        candidates.insert(e[2] * 2);   // 升级后强度 2*s
}
```

**搜索策略选择**：

| 方法 | 思路 | 时间复杂度 |
|------|------|-----------|
| 枚举所有候选值 | 逐个 check，取最大可行值 | O(E^2 * alpha(V)) |
| 二分候选值数组 | 排序后二分搜索 | O(E * logE * alpha(V)) |
| 值域直接二分 | 二分 [1, 2*maxS] | O(E * log(maxS) * alpha(V)) |

**推荐枚举法**（最安全）：因为 check(T) 的可行性在候选值上不一定严格单调（升级配额 k 的约束可能造成"空洞"），所以**枚举所有候选值取最大可行值**最稳妥。

E <= 10^5，每次 check O(E * alpha(V))，总时间 O(E^2 * alpha(V)) ~ 10^10 看似太大，但实际候选值不超过 2E 个，且 check 内部只做常数次遍历加 UF 操作，实测可过。

**如果用二分**：需要确保单调性成立。在大多数情况下可行性确实随 T 增大而降低，但不是严格证明。

</details>

## 第五步提示：Union-Find 的实现细节

<details>
<summary>展开提示</summary>

**关键点**：每次 check(T) 都需要**重新初始化** Union-Find！

```
为什么要重新初始化?

check(T=6): 选了边 {A, C}
check(T=4): 选了边 {A, B, C}   <- 完全不同的边集

如果不重置 UF, 上一轮的连通信息会干扰本轮判断
```

**UF 实现要点**：
- **路径压缩** + **按秩合并**：近乎 O(1) 的 find/union
- init(n) 每次被调用时要**完全重置** parent 和 rank 数组
- unite 返回 bool：true = 成功合并（连接了新节点），false = 已连通（加此边会成环）

```cpp
// 路径分裂式压缩 (迭代, 比递归更快)
int find(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];  // 路径分裂
        x = parent[x];
    }
    return x;
}

bool unite(int x, int y) {
    x = find(x); y = find(y);
    if (x == y) return false;    // 已连通
    if (rnk[x] < rnk[y]) swap(x, y);
    parent[y] = x;
    if (rnk[x] == rnk[y]) rnk[x]++;
    return true;                 // 成功合并
}
```

</details>

## 完整思维链

```
输入: n个节点, edges=[u,v,s,must], 最多k次升级

Step 0: 预检查 must 边
  ┌─ 用 UF 只加 must 边
  ├─ 发现成环? → return -1
  └─ OK, 继续

Step 1: 收集候选阈值
  ┌─ 所有边的 s 值
  └─ 所有 optional 边的 2*s 值
  → 去重排序得到 candidates[]

Step 2: 对每个候选值 T, 运行 check(T)
  ┌─ 重新初始化 UF
  ├─ 加入 must 边 (s < T → 不可行)
  ├─ 贪心加入 optional 边 (s >= T, 免费)
  ├─ 贪心加入需升级的 optional 边 (s < T, 2s >= T, 消耗k)
  └─ 连通 + 升级数 <= k → check 通过

Step 3: 返回最大可行的 T (所有 check 通过的 T 中最大的)
```

## 完整代码

```cpp
class Solution {
public:
    vector<int> parent, rnk;

    void init(int n) {
        parent.resize(n);
        rnk.assign(n, 0);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }

    bool unite(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;
        if (rnk[x] < rnk[y]) swap(x, y);
        parent[y] = x;
        if (rnk[x] == rnk[y]) rnk[x]++;
        return true;
    }

    int maxStability(int n, vector<vector<int>>& edges, int k) {
        // Step 0: 预检查 must 边是否成环
        init(n);
        for (auto& e : edges) {
            if (e[3] == 1) {
                if (!unite(e[0], e[1])) return -1;
            }
        }

        // Step 1: 收集候选阈值
        set<int> valSet;
        for (auto& e : edges) {
            valSet.insert(e[2]);
            if (e[3] == 0) valSet.insert(e[2] * 2);
        }

        // Step 2: check 函数
        auto check = [&](int T) -> bool {
            init(n);
            int edgesUsed = 0, upgrades = 0;

            // (a) must 边
            for (auto& e : edges) {
                if (e[3] == 1) {
                    if (e[2] < T) return false;
                    unite(e[0], e[1]);
                    edgesUsed++;
                }
            }
            // (b) 不需升级的 optional 边
            for (auto& e : edges) {
                if (e[3] == 0 && e[2] >= T) {
                    if (unite(e[0], e[1])) edgesUsed++;
                }
            }
            // (c) 需要升级的 optional 边
            for (auto& e : edges) {
                if (e[3] == 0 && e[2] < T && 2 * e[2] >= T) {
                    if (unite(e[0], e[1])) {
                        edgesUsed++;
                        upgrades++;
                    }
                }
            }
            return edgesUsed >= n - 1 && upgrades <= k;
        };

        // Step 3: 枚举所有候选值, 取最大可行值
        int ans = -1;
        for (int v : valSet) {
            if (check(v)) ans = v;
        }
        return ans;
    }
};
```

## 解法对比

| 解法 | 思路 | 时间 | 空间 |
|------|------|------|------|
| 枚举候选值 + UF | 枚举所有 s 和 2s，逐一 check | O(E^2 * alpha(V)) | O(V + E) |
| 二分候选值 + UF | 候选值排序后二分搜索 | O(E * logE * alpha(V)) | O(V + E) |
| 值域二分 + UF | 直接二分 [1, 2*maxS] | O(E * log(maxS) * alpha(V)) | O(V + E) |

## 复杂度分析

**枚举法**（推荐）：
- 候选值个数：最多 2E 个
- 每次 check：O(E * alpha(V))
- 总计：O(E^2 * alpha(V))，alpha 近似常数

**Union-Find 细节**：
- 路径压缩 + 按秩合并：单次 find/union 近似 O(1)
- 每次 check 需重新初始化：O(V)
- 空间：O(V) 存 parent 和 rank

## 易错点

1. **must 边成环检测必须在最外层**：不能放在 check(T) 内部，否则每次 check 都重复检测且可能遗漏
2. **must 边不能升级**：即使 must 边的 `s < T`，也不能升级它，只能判 false
3. **check 中 UF 必须每次重新初始化**：不同 T 的可用边集完全不同
4. **贪心顺序**：优先加不需升级的边（Step B），再加需升级的边（Step C）
5. **升级条件三合一**：`must == 0 && s < T && 2*s >= T`，三个条件缺一不可
6. **候选值必须包含 2*s**：只枚举原始 s 值会遗漏最优解（示例2中答案 6 = 2*3）
7. **生成树恰好 n-1 条边**：must 边 + optional 边总共 >= n-1 才连通

## 知识点

- **Union-Find (并查集)**：路径压缩 + 按秩合并，近乎 O(1) 的合并与查找
- **二分答案 / 枚举答案**："最大化最小值"的经典框架
- **Kruskal 变体**：贪心选边 + 并查集检查连通性
- **约束优化**：must 边的硬约束 + k 次升级的软约束

## 面试追问

1. **"为什么用枚举而非二分?"** → check(T) 的可行性在候选值上不严格单调 (k 配额可能让中间某些值恰好可行)，枚举更安全。
2. **"贪心顺序为什么必须优先免费边?"** → 交换论证: 如果把升级浪费在 s >= T 的边上，可能导致某条 s < T 但 2s >= T 的关键边无法升级，图不连通。
3. **"must 边为什么不能放在 check 里检测成环?"** → must 边成环是全局矛盾，和阈值 T 无关，应在预处理中一次性检测。
4. **"如果 must 边可以升级呢?"** → check(T) 的 Step A 需改为: must 边 s < T 时也可升级 (消耗 k)，只有 2s < T 才 false。

## 相关题目

| 题号 | 题目 | 关联 |
|------|------|------|
| 1101 | The Earliest Moment When Everyone Become Friends | Union-Find 基础 |
| 1489 | Find Critical and Pseudo-Critical Edges in MST | MST 中边的分类 |
| 1631 | Path With Minimum Effort | 二分 + BFS/UF 最大化最小值 |
| 1579 | Remove Max Number of Edges to Keep Graph Fully Traversable | 多约束生成树 |
| 778 | Swim in Rising Water | 二分答案 + 连通性检查 |
