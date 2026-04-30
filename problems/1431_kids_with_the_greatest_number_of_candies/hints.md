# 1431. 拥有最多糖果的孩子

## 核心思路

这道题的本质极其简单：把所有额外糖果给某个孩子后，他的糖果数 `candies[i] + extraCandies` 是否 >= 当前所有孩子中的最大值。只需要**先找到最大值**，然后**逐个判断**即可。

## 思维链

1. **读完题第一反应**：对于每个孩子 i，把 extraCandies 给他，看 `candies[i] + extraCandies` 是否是全局最大。直觉告诉我，暴力做就是对每个孩子，遍历一遍数组找当前最大值，然后比较。O(n^2)。

2. **能不能更快？** 注意到一个关键事实：额外糖果只给**一个**孩子，其他孩子的糖果数不变。所以"当前最大值"其实就是原始数组的最大值 `maxCandy`。不需要每次重新计算！

3. **判断条件简化**：对于第 i 个孩子，给他 extraCandies 后，他拥有 `candies[i] + extraCandies` 个糖果。他能成为最多的条件是：
   ```
   candies[i] + extraCandies >= maxCandy
   ```
   等价于：
   ```
   candies[i] >= maxCandy - extraCandies
   ```

4. **算法确定**：第一遍 O(n) 找最大值，第二遍 O(n) 逐个判断。总时间 O(n)，空间 O(1)（不算输出）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 对每个孩子，遍历数组找最大值比较 | O(n^2) | O(1) | 能说出即可 |
| 先求最大值 | 一遍找最大，一遍判断 | O(n) | O(1) | 必须写出 |
| STL 一行流 | 用 max_element + transform | O(n) | O(1) | 展示 C++ 功底 |

## 关键提示

1. **为什么不需要每次重算最大值？** 因为额外糖果只给"当前这个孩子"，其他孩子的数量不变，所以需要超越的目标值始终是原数组的最大值。

2. **等于也算**：题目说"greatest"，注意是 `>=`，不是 `>`。当 `candies[i] + extraCandies == maxCandy` 时也返回 true，因为"多个孩子可以同时拥有最多的糖果"。

3. **想一想**：如果题目改为"给第 i 个孩子 extraCandies 后，其他孩子不变，第 i 个孩子是否严格最多"，条件变成什么？答案是 `candies[i] + extraCandies > maxCandy`... 但要注意，如果 `candies[i]` 本身就是 maxCandy，那目标值应该是第二大的值！

4. **数据范围观察**：n <= 100, candies[i] <= 100, extraCandies <= 50。数据极小，暴力 O(n^2) 也完全没问题。但面试中应该展示 O(n) 的思维。

5. **具体示例模拟**：
```
candies = [2, 3, 5, 1, 3], extraCandies = 3

Step 1: maxCandy = max(2,3,5,1,3) = 5

Step 2: 逐个判断
  i=0: 2 + 3 = 5 >= 5 ? YES  --> true
  i=1: 3 + 3 = 6 >= 5 ? YES  --> true
  i=2: 5 + 3 = 8 >= 5 ? YES  --> true
  i=3: 1 + 3 = 4 >= 5 ? NO   --> false
  i=4: 3 + 3 = 6 >= 5 ? YES  --> true

结果: [true, true, true, false, true]
```

## 解法详解

### 解法1: 暴力枚举 -- O(n^2) / O(1)

**思考过程**：最直觉的做法 -- 对于每个孩子，假设把额外糖果给他，然后遍历所有孩子看他是不是最多的。

```cpp
class Solution {
public:
    vector<bool> kidsWithCandies(vector<int>& candies, int extraCandies) {
        int n = candies.size();
        vector<bool> result(n);
        for (int i = 0; i < n; i++) {
            bool isGreatest = true;
            for (int j = 0; j < n; j++) {
                if (candies[j] > candies[i] + extraCandies) {
                    isGreatest = false;
                    break;
                }
            }
            result[i] = isGreatest;
        }
        return result;
    }
};
```

**缺点**：两层循环，O(n^2)。虽然 n <= 100 不会超时，但面试中不该止步于此。

**自然过渡**：每次内层循环其实都在做同一件事 -- 找数组最大值并比较。而最大值是固定的，不会因为"假设给第 i 个孩子糖果"而改变（因为假设是独立的）。所以可以预先算好最大值。

---

### 解法2: 先求最大值 -- O(n) / O(1) ⭐ 推荐

**核心观察**：对于第 i 个孩子，判断条件是 `candies[i] + extraCandies >= maxCandy`。这里的 maxCandy 是个常量，只需要算一次。

**算法步骤**：
1. 一次遍历，找到 `maxCandy = max(candies)`
2. 一次遍历，对每个 i 判断 `candies[i] + extraCandies >= maxCandy`

```
遍历1: 找最大值
candies = [2, 3, 5, 1, 3]
                  ^
            maxCandy = 5

遍历2: 逐个判断 (extraCandies = 3)
  2+3=5 >= 5 ✓   3+3=6 >= 5 ✓   5+3=8 >= 5 ✓   1+3=4 >= 5 ✗   3+3=6 >= 5 ✓
  [true,          true,          true,          false,         true]
```

```cpp
class Solution {
public:
    vector<bool> kidsWithCandies(vector<int>& candies, int extraCandies) {
        int maxCandy = *max_element(candies.begin(), candies.end());
        vector<bool> result;
        for (int c : candies) {
            result.push_back(c + extraCandies >= maxCandy);
        }
        return result;
    }
};
```

**为什么 O(n) 是最优？** 至少需要看每个孩子一次来输出答案，所以下界就是 O(n)。

---

### 解法3: STL 一行流 -- O(n) / O(1)

**展示 C++ 能力**：用 `transform` 配合 lambda，把"判断"操作映射到每个元素上。

```cpp
class Solution {
public:
    vector<bool> kidsWithCandies(vector<int>& candies, int extraCandies) {
        int mx = *max_element(candies.begin(), candies.end());
        vector<bool> result(candies.size());
        transform(candies.begin(), candies.end(), result.begin(),
                  [mx, extraCandies](int c) { return c + extraCandies >= mx; });
        return result;
    }
};
```

这种写法在面试中可以作为"更优雅的写法"提出，展示对 STL 的熟练度。

---

## 面试追问

1. **"为什么不需要每次重算最大值?"** → 额外糖果给的是"假设性"操作，每次独立，其他孩子不变，所以最大值是常量。
2. **"如果 extraCandies 可以拆分给多个孩子?"** → 排序 + 贪心。算每人到达当前 max 的差额，从小到大贪心分配。
3. **"如果是流式输入?"** → 维护 running max。max 只增不减，true->false 不可逆。
4. **"数组 10^8 放不下内存?"** → 两遍流式扫描: 第一遍求 max (O(1) 空间)，第二遍逐个判断输出。

## 面试对话模拟

**面试官**：请解释一下你的思路。

**你**：这道题的关键是理解"给第 i 个孩子所有额外糖果"这个操作是独立的 -- 每次只给一个人。所以我只需要先找到原始数组的最大值 maxCandy，然后对每个孩子判断 candies[i] + extraCandies 是否 >= maxCandy。两次遍历，O(n) 时间，O(1) 额外空间。

**面试官**：如果 extraCandies 可以分给多个孩子呢？

**你**：那问题会变成一个分配优化问题，可能需要贪心或 DP。比如"把 extraCandies 分给最多的孩子，使尽可能多的人达到最大值"，就需要排序 + 贪心了。

## 复杂度总结

| | 时间 | 空间 | 说明 |
|---|---|---|---|
| 暴力 | O(n^2) | O(1) | 内层循环冗余 |
| **先求最大值** | **O(n)** | **O(1)** | **最优解，必须掌握** |
| STL | O(n) | O(1) | 同上，写法更简洁 |

## 相关题目

- [2114. 句子中的最多单词数](https://leetcode.cn/problems/maximum-number-of-words-found-in-sentences/) - 同样是"先求最大值再比较"的模式
- [1672. 最富有客户的资产总量](https://leetcode.cn/problems/richest-customer-wealth/) - 求最大值的变体
- [2574. 左右元素和的差值](https://leetcode.cn/problems/left-and-right-sum-differences/) - 数组遍历基础题

## 常见错误

1. **用 `>` 而非 `>=`**：题目说"greatest number"，多个孩子可以并列最多，所以是 `>=`。
2. **忘记加 extraCandies 就比较**：`candies[i] >= maxCandy` 是错的，应该是 `candies[i] + extraCandies >= maxCandy`。
3. **修改了原数组**：有些人会 `candies[i] += extraCandies` 然后比较，但这改了原数组，影响后续判断。注意操作是"假设性"的。
