# 95. 不同的二叉搜索树 II (Unique Binary Search Trees II)

## 核心思路

**递归分治 + 笛卡尔积**：对范围 `[lo, hi]`，枚举每个值 `i` 作为根节点，递归生成左子树 `[lo, i-1]` 和右子树 `[i+1, hi]` 的所有可能，再将左右子树做笛卡尔积组合成完整的 BST。这是将 BST 的"左 < 根 < 右"性质与递归生成完美融合的经典分治题。

## 思维链

1. **读题审题**：给定整数 n，需要返回所有由 1 到 n 构成的结构不同的 BST。关键词："所有"——意味着枚举，不是计数。BST 性质：左子树所有值 < 根 < 右子树所有值。

2. **为什么想到递归分治？** 选不同的值做根，会产生不同的结构。选 `i` 做根后，`[1, i-1]` 必须全在左子树，`[i+1, n]` 必须全在右子树。左子树本身又是一个"生成所有 BST"的子问题 → 天然的递归结构。

3. **定义递归函数 `build(lo, hi)`**：生成由连续值 `[lo, hi]` 构成的所有 BST，返回根节点列表。
   - **Base case**: `lo > hi` → 无节点可用，返回 `{nullptr}`
   - **递归步**: 枚举 `i = lo..hi` 为根，`lefts = build(lo, i-1)`，`rights = build(i+1, hi)`，笛卡尔积组合

4. **关键易错点：为什么 base case 返回 `{nullptr}` 而不是 `{}`？**

```
场景: n=3, 选根=1, 左子树范围 [1,0]

如果 build(1, 0) 返回空列表 {}:
  lefts = {}
  rights = build(2, 3) = [...多棵树...]
  for l in lefts:      ← 循环 0 次！
    for r in rights:
      ...
  结果: 根=1 没有产生任何树！  ← BUG!
  但 "根=1, 左子树为空, 右子树不空" 是合法的BST！

如果 build(1, 0) 返回 {nullptr}:
  lefts = {nullptr}
  rights = build(2, 3) = [...多棵树...]
  for nullptr in lefts:     ← 循环 1 次 (null代表空子树)
    for r in rights:
      new TreeNode(1, nullptr, r)  ← 正确生成！
```

5. **n=3 的递归过程可视化**：

```
build(1, 3):
├── 根=1:
│   lefts  = build(1,0) = [null]
│   rights = build(2,3)
│            ├── 根=2: [null] x [3] → Node(2, null, 3)
│            └── 根=3: [2]   x [null] → Node(3, 2, null)
│   组合: null x {Node(2,null,3), Node(3,2,null)} → 2棵树
│
├── 根=2:
│   lefts  = build(1,1) = [Node(1)]
│   rights = build(3,3) = [Node(3)]
│   组合: {Node(1)} x {Node(3)} → 1棵树
│
└── 根=3:
    lefts  = build(1,2)
    │        ├── 根=1: [null] x [2] → Node(1, null, 2)
    │        └── 根=2: [1]   x [null] → Node(2, 1, null)
    rights = build(4,3) = [null]
    组合: {Node(1,null,2), Node(2,1,null)} x {null} → 2棵树

总计: 2 + 1 + 2 = 5 棵树 = 卡特兰数 C(3)
```

6. **n=3 的 5 棵 BST (ASCII 图)**：

```
树1:   1          树2:   1         树3:    2        树4:      3       树5:     3
        \                  \              / \              /                 /
         2                  3            1   3            1                 2
          \                /                               \              /
           3              2                                 2            1
```

验证 BST 性质：
- 树1: 1<2<3, 左空<1, 2<3 (右) -- 合法
- 树3: 1<2, 2<3 -- 合法 (完美平衡)
- 树4: 1<3(根), 1<2(右子树), 左子树<3 -- 合法

7. **卡特兰数**：结构不同的 BST 数量恰好是第 n 个卡特兰数 `C(n)`。

```
n:    0  1  2  3   4   5    6    7     8
C(n): 1  1  2  5  14  42  132  429  1430

递推公式: C(n) = sum_{i=0}^{n-1} C(i) * C(n-1-i)
即: 左子树 i 个节点的方案数 * 右子树 (n-1-i) 个节点的方案数
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试推荐 |
|------|------|------|------|----------|
| 递归分治 | 枚举根 + 笛卡尔积 | O(n * C(n)) | O(n * C(n)) | 必写 |
| 递归+记忆化 | 缓存 (lo,hi) 避免重复 | O(n * C(n)) | 同+缓存 | 优化版 |
| DP + 值偏移 | dp[len] + clone偏移 | O(n * C(n)) | 同 | 进阶 |

## 关键提示

- **提示1**: `lo > hi` 必须返回 `{nullptr}`，不是 `{}`。空子树本身是一种合法选择，作为笛卡尔积的元素参与组合。这是本题最经典的 bug。
- **提示2**: BST 性质自动保证正确性——选 `i` 为根后，`[lo, i-1]` 的所有值 < i，`[i+1, hi]` 的所有值 > i。不需要额外验证。
- **提示3**: 默认实现中不同结果树可能共享子树指针。比如两棵树的某个子树指向同一个 TreeNode 对象。如果后续需要修改节点值，必须深拷贝。
- **提示4**: 本题与 LC 96 的关系——96 只需计算数量（卡特兰数 DP），本题需要实际构建每一棵树。从 96 到 95，是从"计数"到"枚举"的升级。
- **提示5**: 记忆化时用 `memo[{lo, hi}]` 缓存。注意 `build(2, 4)` 可能在 `i=1` 时作为右子树被调用，也可能在 `i=5` 时作为左子树被调用——完全相同的子问题。

```
重复子问题示例 (n=5):
build(1,5) 的根=1 → 需要 build(2,5)
build(1,5) 的根=5 → 需要 build(1,4)
                        根=4 → 需要 build(1,3)
                                根=3 → 需要 build(1,2)
build(1,5) 的根=3 → 需要 build(1,2)  ← 重复！
```

## 解法详解

### 解法1: 递归分治（核心解法）

**算法伪码**：
```
build(lo, hi):
  if lo > hi: return [null]     ← 空子树也是一种合法选择
  result = []
  for i = lo to hi:              ← 枚举每个值为根
    lefts  = build(lo, i-1)      ← 所有左子树
    rights = build(i+1, hi)      ← 所有右子树
    for L in lefts:              ← 笛卡尔积
      for R in rights:
        result.add( TreeNode(i, L, R) )
  return result

main: return build(1, n)
```

**为什么正确**：BST 性质天然由值域分割保证。`[lo, i-1]` 的每个值都 < i，`[i+1, hi]` 的每个值都 > i。

### 解法2: 递归 + 记忆化

**优化动机**: `build(lo, hi)` 可能被多个不同的父调用重复调用。缓存 `memo[{lo, hi}]` 后直接返回。

**注意**: 缓存后子树节点被多棵树共享。如果只读取树结构（比如输出答案），共享是安全的。如果需要独立修改每棵树，组合时需深拷贝。

### 解法3: DP + 值偏移

**核心观察**: `build(1, 3)` 和 `build(4, 6)` 生成的树结构完全相同，只是节点值差了 3。所以可以只计算 `dp[len] = build(1, len)`，使用时对每个节点值加偏移量。

```
build(lo, hi) 等价于 dp[hi-lo+1]，每个节点值加 lo-1

例: build(4, 6) = dp[3] 的每棵树，节点值 +3
    dp[3] 中的树 "2→1,3" → 偏移后 "5→4,6"
```

## 完整代码

```cpp
class Solution {
public:
    vector<TreeNode*> generateTrees(int n) {
        if (n == 0) return {};
        return build(1, n);
    }

private:
    vector<TreeNode*> build(int lo, int hi) {
        // 空范围: 返回包含 null 的列表
        // 不能返回 {} !! null 代表"空子树是一种选择"
        if (lo > hi) return {nullptr};

        vector<TreeNode*> result;
        for (int i = lo; i <= hi; i++) {
            // BST: 左子树值 < 根 < 右子树值
            vector<TreeNode*> lefts  = build(lo, i - 1);
            vector<TreeNode*> rights = build(i + 1, hi);

            // 笛卡尔积: 每种 (左, 右) 组合构成一棵新 BST
            for (TreeNode* l : lefts) {
                for (TreeNode* r : rights) {
                    result.push_back(new TreeNode(i, l, r));
                }
            }
        }
        return result;
    }
};
```

```python
class Solution:
    def generateTrees(self, n: int) -> List[Optional[TreeNode]]:
        def build(lo, hi):
            if lo > hi:
                return [None]  # 空子树也是一种选择!
            
            result = []
            for i in range(lo, hi + 1):
                lefts  = build(lo, i - 1)
                rights = build(i + 1, hi)
                for l in lefts:
                    for r in rights:
                        root = TreeNode(i, l, r)
                        result.append(root)
            return result
        
        return build(1, n)
```

## 面试追问

1. **"只需要数量怎么办？"** → LC 96，DP 计算卡特兰数：`dp[n] = sum(dp[i-1] * dp[n-i])` for i=1..n
2. **"时间复杂度怎么分析？"** → 输出本身就有 C(n) 棵树，每棵 n 个节点，所以 Omega(n * C(n))。无法更快。
3. **"为什么不能用回溯？"** → 回溯适合"构造一棵 → 记录 → 撤销"。但本题需要保留所有树（不能撤销），所以是递归生成 + 组合。
4. **"子树共享安全吗？"** → 只读安全。如果要修改节点值（比如序列化后再反序列化），需要深拷贝。
5. **"n=8 时性能如何？"** → C(8)=1430 棵树 * 8 节点 = ~11K 个 TreeNode，毫秒级完成。

## 相关题目

| 题目 | 关联 |
|------|------|
| 96. Unique Binary Search Trees | 只计数（卡特兰数 DP），本题的计数版 |
| 241. Different Ways to Add Parentheses | 类似的分治 + 笛卡尔积框架 |
| 894. All Possible Full Binary Trees | 枚举所有满二叉树，同样的递归分治 |
| 1569. Number of Ways to Reorder Array to Get Same BST | BST 结构计数的组合数学 |

## 复杂度分析

- **时间**: O(n * C(n))，其中 C(n) 是第 n 个卡特兰数 ~O(4^n / n^(3/2))。每棵树 n 个节点。
- **空间**: O(n * C(n)) 存储所有结果树的节点。递归栈深度 O(n)。
