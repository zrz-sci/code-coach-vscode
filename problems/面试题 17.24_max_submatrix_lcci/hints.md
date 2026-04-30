# 面试题 17.24. 最大子矩阵 (Max Submatrix LCCI)

## 难度：Hard | 标签：Array, Dynamic Programming, Matrix, Prefix Sum

---

## 题目描述

给定一个 N x M 的正整数、负整数和 0 组成的矩阵，找出元素总和最大的子矩阵。

返回 `[r1, c1, r2, c2]`，其中 `(r1, c1)` 是子矩阵左上角，`(r2, c2)` 是右下角。若有多个答案，返回任意一个。

**约束**：`1 <= matrix.length, matrix[0].length <= 200`

---

## Hint 1：一维基础 -- Kadane 算法回顾

这道二维题的根基是「最大子数组和」(LC 53)。如果你没做过，先搞定它。

**Kadane 核心思想**：遍历数组，维护一个 `curSum`。当 `curSum < 0` 时，前面的累计只会拖后腿，果断丢弃，从当前元素重新开始。

```
数组:  [3, -4, 5, -1, 2]

遍历:
  i=0: curSum = 3       (首个元素)       best = 3
  i=1: curSum = 3+(-4) = -1              best = 3
  i=2: curSum < 0, 丢弃! curSum = 5     best = 5
  i=3: curSum = 5+(-1) = 4              best = 5
  i=4: curSum = 4+2 = 6                 best = 6  <-- 答案
```

**记住这个直觉**：`curSum < 0` 意味着"止损"，不如从新位置重新出发。

---

## Hint 2：降维 -- 两条水平线"夹"住矩阵

二维矩阵怎么转化成一维问题？

想象用两条水平线夹住矩阵的第 `top` 行到第 `bottom` 行，然后把每一列的数字「压扁」成一个数（求和）。这些列和排成一行，就是一个一维数组。

```
原矩阵 (3x5):
     c0   c1   c2   c3   c4
r0 [  9,  -8,   1,   3,  -2 ]
r1 [ -3,   7,   6,  -2,   4 ]
r2 [  6,  -4,  -4,   8,  -7 ]

══ 选 top=0, bottom=2（所有三行都夹住）══

列压缩:
  col[0] = 9+(-3)+6  = 12
  col[1] = -8+7+(-4) = -5
  col[2] = 1+6+(-4)  = 3
  col[3] = 3+(-2)+8  = 9
  col[4] = -2+4+(-7) = -5

压缩结果: [12, -5, 3, 9, -5]
           ↓
   这就是一个一维数组，对它跑 Kadane!
```

**关键洞察**：枚举所有 `(top, bottom)` 组合，每组做一次 Kadane，全局最大值就是答案。

---

## Hint 3：增量累加 -- 不要重复计算列和

固定 `top` 后，`bottom` 从 `top` 向下滑动。每次 `bottom` 增加 1，只需把新一行的值加到 `col[]` 上：

```
top = 0:

  bottom = 0: col[] = [9, -8, 1, 3, -2]    (直接是 row[0])
  bottom = 1: col[] += row[1]
              col[] = [9+(-3), -8+7, 1+6, 3+(-2), -2+4]
                    = [6, -1, 7, 1, 2]
  bottom = 2: col[] += row[2]
              col[] = [6+6, -1+(-4), 7+(-4), 1+8, 2+(-7)]
                    = [12, -5, 3, 9, -5]
```

这样每扩展一行只需 O(M) 加法，不用重新求 `top` 到 `bottom` 全部行的列和。

---

## Hint 4：Kadane 中追踪子矩阵坐标

普通 Kadane 只返回最大和。但本题需要 `[r1, c1, r2, c2]`。

- `r1 = top, r2 = bottom` 由外层循环确定
- `c1, c2` 需要在 Kadane 内部追踪

**方法**：用 `tempStart` 记录「当前这段连续子数组的起始列」。

```
Kadane 带坐标追踪:

  curSum = 0, tempStart = 0

  for j = 0 to M-1:
      curSum += col[j]

      if curSum > globalMax:     // 发现新的全局最大
          globalMax = curSum
          答案 = [top, tempStart, bottom, j]

      if curSum < 0:             // 止损，重新开始
          curSum = 0
          tempStart = j + 1      // 下一列是新的起点
```

**注意**：`tempStart` 在 `curSum < 0` 时更新为 `j + 1`（不是 `j`），因为当前列 `j` 对应的 `curSum` 是负的，新段应从下一列开始。

---

## Hint 5：完整算法 + 手算示例

```
算法:
  globalMax = INT_MIN
  for top = 0 to N-1:
      col[0..M-1] = 0
      for bottom = top to N-1:
          for j = 0 to M-1: col[j] += matrix[bottom][j]
          对 col[] 跑带坐标的 Kadane
  return 答案坐标
```

**手算 matrix = [[9,-8,1,3,-2],[-3,7,6,-2,4],[6,-4,-4,8,-7]]**:

```
top=0, bot=0: col=[9,-8,1,3,-2]
  Kadane: max=9 at [0,0,0,0]                     globalMax=9

top=0, bot=1: col=[6,-1,7,1,2]
  Kadane: 6 -> 5 -> 12 -> 13 -> 15
  max=15 at [0,0,1,4]                            globalMax=15

top=0, bot=2: col=[12,-5,3,9,-5]
  j=0: cur=12, globalMax 更新? 12<15, NO
  j=1: cur=12+(-5)=7
  j=2: cur=7+3=10
  j=3: cur=10+9=19 > 15, YES! 答案=[0,0,2,3]   globalMax=19
  j=4: cur=19+(-5)=14

top=1, bot=1: col=[-3,7,6,-2,4]
  Kadane: -3<0 重置, 7, 13, 11, 15 → max=15 < 19

top=1, bot=2: col=[3,3,2,6,-3]
  Kadane: 3, 6, 8, 14, 11 → max=14 < 19

top=2, bot=2: col=[6,-4,-4,8,-7]
  Kadane: 6, 2, -2<0 重置, 8, 1 → max=8 < 19

最终答案: [0, 0, 2, 3], sum = 19
```

---

## 决策树：何时重置 vs 继续累加

```
            处理列 j
                |
        curSum += col[j]
                |
        curSum > globalMax ?
       /                    \
     YES                    NO
  更新坐标                 跳过
       \                  /
        curSum < 0 ?
       /            \
     YES             NO
  重置 curSum=0     继续
  tempStart=j+1     不动
```

---

## 完整代码

```cpp
class Solution {
public:
    vector<int> getMaxMatrix(vector<vector<int>>& matrix) {
        int N = matrix.size(), M = matrix[0].size();
        int globalMax = INT_MIN;
        int r1 = 0, c1 = 0, r2 = 0, c2 = 0;

        for (int top = 0; top < N; top++) {
            vector<int> col(M, 0);  // 列压缩数组，每换 top 清零
            for (int bottom = top; bottom < N; bottom++) {
                // 增量累加当前行
                for (int j = 0; j < M; j++) {
                    col[j] += matrix[bottom][j];
                }

                // 对 col[] 跑带坐标追踪的 Kadane
                int curSum = 0;
                int tempStart = 0;
                for (int j = 0; j < M; j++) {
                    curSum += col[j];
                    if (curSum > globalMax) {
                        globalMax = curSum;
                        r1 = top;
                        c1 = tempStart;
                        r2 = bottom;
                        c2 = j;
                    }
                    if (curSum < 0) {
                        curSum = 0;
                        tempStart = j + 1;
                    }
                }
            }
        }

        return {r1, c1, r2, c2};
    }
};
```

---

## 复杂度分析

| 维度 | 复杂度 | 说明 |
|------|--------|------|
| 时间 | O(N^2 * M) | 枚举 O(N^2) 个行范围，每次 Kadane O(M) |
| 空间 | O(M) | 列压缩数组 |

N = 行数，M = 列数。若 N > M，可交换行列使复杂度 O(min(N,M)^2 * max(N,M))。

---

## 关联题目

| 题目 | 关联 |
|------|------|
| 53. 最大子数组和 | 本题的一维基础版，必须先掌握 Kadane |
| 363. 矩形区域不超过 K 的最大数值和 | 同样列压缩+Kadane，但有上限约束需用 set+二分 |
| 304. 二维区域和检索 | 前缀和基础，列压缩是一种特化的前缀和思路 |

---

## 常见错误

1. **globalMax 初始化为 0**：矩阵可能全是负数，必须用 `INT_MIN`。
2. **重新计算整个列和**：`bottom` 变化时应增量累加，否则变成 O(N^3 * M)。
3. **tempStart 更新为 j 而非 j+1**：`curSum < 0` 时当前列也被抛弃，新段从下一列开始。
4. **Kadane 中先判断 curSum > globalMax 还是先判断 curSum < 0**：应该先更新答案再检查是否重置，否则可能漏掉某些合法答案。

---

## 面试追问

| 追问 | 关键点 |
|------|--------|
| 矩阵很大 (N=M=1000)? | O(N^2*M) = 10^9，需要剪枝或近似算法 |
| 子矩阵和不超过 K? | LC 363，Kadane 步骤换成有序集合 + 二分查找 |
| 要求是正方形子矩阵? | 额外约束 `bottom - top == right - left`，枚举时加限制 |
| globalMax 能初始化为 0 吗? | 不能，矩阵可能全负，必须 INT_MIN |
