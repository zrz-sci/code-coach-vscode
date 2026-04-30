# 364. 嵌套列表加权和 II

## 题目理解

给定整数嵌套列表 `nestedList`，每个元素要么是整数，要么是嵌套列表。整数的**权重** = `maxDepth - 当前深度 + 1`（越浅权重越大）。求所有整数乘以其权重的总和。

**本质**：与 LC 339（Nested List Weight Sum I）相反 -- 339是深层权重大，本题是浅层权重大。关键挑战是**不预先知道最大深度**，需要巧妙处理。

```
示例1: nestedList = [[1,1],2,[1,1]]
maxDepth = 2
深度2处有: 1,1,1,1 (权重=2-2+1=1)
深度1处有: 2       (权重=2-1+1=2)
输出: 1*1+1*1+2*2+1*1+1*1 = 8

示例2: nestedList = [1,[4,[6]]]
maxDepth = 3
6在深度3(权重1), 4在深度2(权重2), 1在深度1(权重3)
输出: 1*3+4*2+6*1 = 17
```

---

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|------|-----------|-----------|--------|
| ⭐ BFS 层级累加 | 每层整数累加到下一层，自动实现反向加权 | O(n) | O(n) | 面试首选 |
| 两遍 DFS | 第一遍求 maxDepth，第二遍算加权和 | O(n) | O(d) | 直观 |
| 数学转换 | 利用 (maxDepth+1)*sum - weightedSum | O(n) | O(d) | 巧妙 |

---

## 渐进提示

### 提示 1 -- 最直观：两遍 DFS

<details>
<summary>展开</summary>

既然权重 = maxDepth - depth + 1，那就：
1. 第一遍 DFS：找到 maxDepth
2. 第二遍 DFS：带着 maxDepth 计算加权和

```
// 第一遍
int getMaxDepth(nestedList, depth=1):
    maxD = depth
    for each item in nestedList:
        if item.isList():
            maxD = max(maxD, getMaxDepth(item.getList(), depth+1))
    return maxD

// 第二遍
int weightedSum(nestedList, depth, maxDepth):
    sum = 0
    for each item in nestedList:
        if item.isInteger():
            sum += item.getInteger() * (maxDepth - depth + 1)
        else:
            sum += weightedSum(item.getList(), depth+1, maxDepth)
    return sum
```

正确但需要两次遍历，面试官可能追问能否一遍搞定。

</details>

### 提示 2 -- BFS 层级累加（核心技巧）

<details>
<summary>展开</summary>

**关键观察**：不需要知道 maxDepth！

BFS 逐层遍历，维护一个 `levelSum` 变量：
- 每一层把当前层的整数加到 `levelSum`
- 每一层都把 `levelSum` 累加到 `totalSum`

这样，第1层的整数被累加了 maxDepth 次（因为后面每层都会再加一次），第2层被累加 maxDepth-1 次...恰好实现了反向权重！

```
nestedList = [[1,1],2,[1,1]]

BFS:
层1: 整数有 2          levelSum = 2,  totalSum = 2
层2: 整数有 1,1,1,1    levelSum = 2+4=6, totalSum = 2+6=8

最终 totalSum = 8 ✓
```

**为什么有效？**
```
层1的整数 a 在 levelSum 中存活了 maxDepth 层
  -> 被累加到 totalSum 共 maxDepth 次
  -> 等价于 a * maxDepth

层2的整数 b 在 levelSum 中存活了 maxDepth-1 层
  -> 被累加 maxDepth-1 次
  -> 等价于 b * (maxDepth-1)

这恰好等于 a*(maxDepth-1+1) + b*(maxDepth-2+1) = 反向加权和
```

</details>

### 提示 3 -- 数学转换法

<details>
<summary>展开</summary>

设 `sumI` = LC 339 的正向加权和（深层权重大），`flatSum` = 所有整数之和。

正向：每个整数 x 在深度 d，贡献 `x * d`
反向：每个整数 x 在深度 d，贡献 `x * (maxDepth - d + 1)`

反向加权和 = sum(x * (maxDepth - d + 1))
           = sum(x * (maxDepth + 1)) - sum(x * d)
           = (maxDepth + 1) * flatSum - sumI

只需要一遍 DFS 同时算出 flatSum、sumI 和 maxDepth。

</details>

---

## 关键思维模式

### 1. "层级累加"模式 (BFS 核心)

不需要知道总深度，通过重复累加自动实现 "越浅 -> 被加越多次 -> 权重越大"。

```
每层处理:
  levelSum += 当前层所有整数
  totalSum += levelSum   // levelSum 包含了之前所有层的整数!
```

### 2. "正反转换"模式

反向加权和 = (maxDepth+1) * 所有整数之和 - 正向加权和。把难题转换为已知解的组合。

### 3. BFS vs DFS 选择

- BFS 天然按层处理 -> 适合层级相关的权重计算
- DFS 天然传递深度 -> 适合需要 maxDepth 的方案

---

## 完整代码 (C++)

```cpp
// 解法1: BFS 层级累加 (推荐)
class Solution {
public:
    int depthSumInverse(vector<NestedInteger>& nestedList) {
        int totalSum = 0, levelSum = 0;
        queue<NestedInteger> q;

        for (auto& ni : nestedList) q.push(ni);

        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                NestedInteger curr = q.front(); q.pop();
                if (curr.isInteger()) {
                    levelSum += curr.getInteger();
                } else {
                    for (auto& child : curr.getList())
                        q.push(child);
                }
            }
            totalSum += levelSum;  // levelSum 包含当前层 + 所有之前层的整数
        }
        return totalSum;
    }
};
```

---

## 算法图解

### BFS 层级累加过程

```
nestedList = [1, [4, [6]]]

初始队列: [1, [4,[6]]]

层1: 取出 1 和 [4,[6]]
     1 是整数 -> levelSum += 1 => levelSum = 1
     [4,[6]] 是列表 -> 子元素 4, [6] 入队
     totalSum += levelSum => totalSum = 1

层2: 取出 4 和 [6]
     4 是整数 -> levelSum += 4 => levelSum = 1+4 = 5
     [6] 是列表 -> 子元素 6 入队
     totalSum += levelSum => totalSum = 1+5 = 6

层3: 取出 6
     6 是整数 -> levelSum += 6 => levelSum = 5+6 = 11
     totalSum += levelSum => totalSum = 6+11 = 17

最终 totalSum = 17 ✓

验证: 1*3 + 4*2 + 6*1 = 3+8+6 = 17 ✓

注意 levelSum 不重置! 这是核心:
  1 被累加了3次(层1/2/3), 4被累加了2次(层2/3), 6被累加1次(层3)
  = 1*3 + 4*2 + 6*1 = 17
```

### 数学转换验证

```
nestedList = [[1,1],2,[1,1]]
maxDepth = 2, flatSum = 1+1+2+1+1 = 6

正向加权和 (LC 339): 2*1 + (1+1+1+1)*2 = 2+8 = 10
  不对... 正向: 深度1的权重=1, 深度2的权重=2
  sumI = 2*1 + 1*2+1*2+1*2+1*2 = 2+8 = 10

反向 = (maxDepth+1)*flatSum - sumI = 3*6 - 10 = 18-10 = 8 ✓
```

---

## 常见错误

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 每层重置 levelSum = 0 | ✓ levelSum 跨层累加 | 重置就变成了等权重求和 |
| ✗ DFS 忘记第一遍求 maxDepth | ✓ 先求 maxDepth 再算加权和 | 不知道 maxDepth 无法算权重 |
| ✗ 深度从 0 开始 | ✓ 深度从 1 开始（或调整公式） | 影响权重计算 |
| ✗ 数学法中用 maxDepth 而非 maxDepth+1 | ✓ (maxDepth+1)*flatSum - sumI | 公式推导要仔细 |

---

## 面试追问

**Q1: BFS 方法为什么不需要知道 maxDepth？**
> 因为 levelSum 不重置，浅层整数会被反复累加到 totalSum。第 d 层的整数被累加 maxDepth - d + 1 次，恰好等于反向权重。这是一种隐式计数技巧。

**Q2: 如果嵌套可以有空列表呢？**
> 空列表不产生整数也不增加有效深度，BFS 中空列表会被展开但不入队任何子元素，不影响结果。题目保证没有空列表。

**Q3: 与 LC 339 (Nested List Weight Sum) 的关系？**
> 339 是正向加权（深层权重大），用简单 DFS depth*value 即可。本题反向加权，可以用 `(maxDepth+1)*flatSum - 339的结果` 转换，或用 BFS 层级累加一遍搞定。

---

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 339 | Nested List Weight Sum | 正向加权版本 |
| 341 | Flatten Nested List Iterator | 嵌套列表迭代器 |
| 565 | Array Nesting | 嵌套结构遍历 |
| 690 | Employee Importance | BFS/DFS 层级遍历 |
