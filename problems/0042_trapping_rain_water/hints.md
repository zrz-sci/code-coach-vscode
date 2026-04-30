# 42. 接雨水

## 核心思路

每个位置能接的雨水 = `min(左边最高柱子, 右边最高柱子) - 当前柱子高度`。整道题的本质就是：**如何高效地求出每个位置左右两边的最大值**。

## 思维链

1. **读完题第一反应**：每个位置头上能不能存水？取决于它左右两边有没有比它高的"墙"。一个位置的水位高度 = 左右两边最高墙的较矮者。
2. **暴力怎么做？** 对每个位置 i，向左扫描找 leftMax，向右扫描找 rightMax，算 `min(leftMax, rightMax) - height[i]`。时间 O(n²)。
3. **瓶颈在哪？** 每个位置都要重复扫描整个左/右半边去找最大值，大量重复计算。
4. **怎么消除重复？** 预处理！用两个数组 `leftMax[]` 和 `rightMax[]` 分别从左到右、从右到左一次遍历就能算好。时间 O(n)，空间 O(n)。
5. **能否进一步优化空间？** 双指针！从两端向中间逼近，利用"短板决定水位"的性质，只需要 O(1) 空间。
6. **换个视角？** 单调栈可以按"层"而非按"列"来计算积水，横向累加每一层的面积。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力 | 对每个位置左右扫描找最大值 | O(n²) | O(1) | 能说出即可 |
| 预处理数组 (DP) | 预计算 leftMax[] 和 rightMax[] | O(n) | O(n) | 能说出即可 |
| 双指针 | 两端向中间逼近，短板决定水位 | O(n) | O(1) | ⭐ 必须写出 |
| 单调递减栈 | 横向按层计算积水面积 | O(n) | O(n) | 加分项 |

## 关键提示

1. **按列思考**：不要想"水在哪些格子里"，而是想"第 i 列头上能积多少水"。答案 = `min(leftMax, rightMax) - height[i]`，如果为负则为 0。

2. **预处理的核心**：`leftMax[i] = max(leftMax[i-1], height[i])`，一次从左到右扫就够了。rightMax 类似。

3. **双指针的关键洞察**：如果 `height[left] < height[right]`，那么不管右边更远处有什么，left 位置的水位一定由 leftMax 决定（因为右边至少有 height[right] ≥ leftMax 来"兜底"）。

4. **单调栈的视角**：遇到比栈顶高的柱子 → 栈顶形成了一个"凹槽"，可以在这个凹槽里横向蓄水。宽度 = 当前位置 - 新栈顶位置 - 1，高度 = min(两侧高度) - 凹槽底。

5. **ASCII 图解**（示例1）：

```
柱子高度: [0,1,0,2,1,0,1,3,2,1,2,1]
索引:      0 1 2 3 4 5 6 7 8 9 10 11

              3
     2  ██    ██
  ██~██~██~██~████~██
  ██~██~████~██████~██
──────────────────────
  0  1  2  3  4  5  6  7  8  9 10 11

~表示水，██表示柱子
第2列: min(1,3)-0 = 1
第4列: min(2,3)-1 = 1
第5列: min(2,3)-0 = 2
第6列: min(2,3)-1 = 1
第9列: min(3,2)-1 = 1
总计: 1+1+2+1+0+1 = 6
```

## 解法详解

### 解法1: 暴力法 — O(n²) / O(1)

**思考过程**: 最直觉的方法——对每个位置，向左向右分别找最高的柱子，取较小值减去当前高度。

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        int n = height.size(), ans = 0;
        for (int i = 0; i < n; i++) {
            int leftMax = 0, rightMax = 0;
            // 向左扫描找最高柱子（包含自身）
            for (int j = 0; j <= i; j++)
                leftMax = max(leftMax, height[j]);
            // 向右扫描找最高柱子（包含自身）
            for (int j = i; j < n; j++)
                rightMax = max(rightMax, height[j]);
            // 当前列能积的水 = 两边最高的较矮者 - 自身高度
            ans += min(leftMax, rightMax) - height[i];
        }
        return ans;
    }
};
```

**关键点**: leftMax 和 rightMax 都包含 height[i] 自身，这样 `min(leftMax, rightMax) - height[i]` 永远 ≥ 0，无需额外判断负值。

---

### 解法2: 预处理数组 (DP) — O(n) / O(n)

**从解法1优化**: 解法1的瓶颈是每个位置都要重新扫描 O(n) 来找左右最大值。我们可以用两次遍历预计算好。

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        int n = height.size(), ans = 0;
        vector<int> leftMax(n), rightMax(n);
        
        // 从左到右：leftMax[i] = max(height[0..i])
        leftMax[0] = height[0];
        for (int i = 1; i < n; i++)
            leftMax[i] = max(leftMax[i - 1], height[i]);
        
        // 从右到左：rightMax[i] = max(height[i..n-1])
        rightMax[n - 1] = height[n - 1];
        for (int i = n - 2; i >= 0; i--)
            rightMax[i] = max(rightMax[i + 1], height[i]);
        
        // 每个位置的水 = min(leftMax, rightMax) - height
        for (int i = 0; i < n; i++)
            ans += min(leftMax[i], rightMax[i]) - height[i];
        
        return ans;
    }
};
```

**关键点**: 
- `leftMax[i]` 的递推只依赖 `leftMax[i-1]`，天然是从左到右填。
- `rightMax[i]` 只依赖 `rightMax[i+1]`，天然是从右到左填。

---

### 解法3: 双指针 — O(n) / O(1) ⭐ 面试首选

**从解法2优化**: 解法2用了 O(n) 额外空间存两个数组。能否只用常量空间？

**核心洞察**: 对于位置 i 的水位，我们只需要 `min(leftMax, rightMax)`。如果 `leftMax < rightMax`，水位由 leftMax 决定，rightMax 具体是多少根本不重要（反正更大）。双指针从两端向中间走，每次移动"短板"那一侧。

```
// 双指针过程演示 (height = [0,1,0,2,1,0,1,3,2,1,2,1]):
//
// 初始: left=0, right=11, lMax=0, rMax=0
// 
// Step1: h[left]=0 <= h[right]=1, lMax=max(0,0)=0, water+=0-0=0, left++
// Step2: h[left]=1 <= h[right]=1, lMax=max(0,1)=1, water+=1-1=0, left++
// Step3: h[left]=0 <= h[right]=1, lMax=1, water+=1-0=1, left++
// Step4: h[left]=2 > h[right]=1,  rMax=max(0,1)=1, water+=1-1=0, right--
// Step5: h[left]=2 > h[right]=2,  rMax=max(1,2)=2, water+=2-2=0, right--
// Step6: h[left]=2 > h[right]=1,  rMax=2, water+=2-1=1, right--
// Step7: h[left]=2 <= h[right]=2, lMax=max(1,2)=2, water+=2-2=0, left++
// Step8: h[left]=1 <= h[right]=2, lMax=2, water+=2-1=1, left++
// Step9: h[left]=0 <= h[right]=2, lMax=2, water+=2-0=2, left++
// Step10:h[left]=1 <= h[right]=2, lMax=2, water+=2-1=1, left++
// Step11:h[left]=3 > h[right]=2,  rMax=2, water+=2-2=0, right--
// left > right, 结束. total = 0+0+1+0+0+1+0+1+2+1+0 = 6 ✓
```

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        int left = 0, right = height.size() - 1;
        int leftMax = 0, rightMax = 0;
        int ans = 0;
        
        while (left <= right) {
            if (height[left] <= height[right]) {
                // 左边是短板，left位置的水位由leftMax决定
                // 因为右边至少有height[right] >= height[left]来"兜底"
                leftMax = max(leftMax, height[left]);
                ans += leftMax - height[left];
                left++;
            } else {
                // 右边是短板，right位置的水位由rightMax决定
                rightMax = max(rightMax, height[right]);
                ans += rightMax - height[right];
                right--;
            }
        }
        return ans;
    }
};
```

**为什么正确？**
- 当 `height[left] <= height[right]` 时，right 那边至少有一个柱子（height[right]）不低于 height[left]。所以 rightMax ≥ height[right] ≥ height[left]。
- 因此 `min(leftMax, rightMax) = leftMax`（如果 leftMax ≤ rightMax 的话）。但这恰恰就是我们在走"短板"那一侧！leftMax 已经足够确定 left 位置的水位了。

---

### 解法4: 单调递减栈 — O(n) / O(n)

**换个角度**: 前面三种解法都是**按列**算水量（每列头上积多少水）。单调栈是**按层**算——找到凹槽，横向填水。

```
// 单调栈的视角：横向按层填水
// 
// height = [0,1,0,2,1,0,1,3,2,1,2,1]
// 
// 遇到 height[3]=2 时，栈中有 [1(idx1), 0(idx2)]
//   弹出 0(idx2)：凹槽底=0, 左墙=1(idx1), 右墙=2(idx3)
//   宽度=3-1-1=1, 高度=min(1,2)-0=1, 面积=1
//
// 遇到 height[7]=3 时，依次弹出形成多层横向积水
//
// 栈中始终维护递减序列，遇到更高的柱子就"结算"凹槽
```

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        stack<int> stk; // 存索引，对应高度单调递减
        int ans = 0;
        
        for (int i = 0; i < height.size(); i++) {
            // 当前柱子比栈顶高 → 栈顶是凹槽底部
            while (!stk.empty() && height[i] > height[stk.top()]) {
                int bottom = stk.top(); // 凹槽底部
                stk.pop();
                if (stk.empty()) break; // 左边没有墙，无法积水
                
                int leftWall = stk.top();  // 左墙
                int width = i - leftWall - 1;
                int h = min(height[leftWall], height[i]) - height[bottom];
                ans += width * h;
            }
            stk.push(i);
        }
        return ans;
    }
};
```

**关键点**: 
- 栈中存的是**索引**，不是高度（需要索引来算宽度）。
- 弹出栈顶后，新的栈顶就是左墙。如果栈空了说明左边没有更高的墙，这层水接不了。

---

## 解法对比

| | 暴力 | 预处理DP | 双指针 ⭐ | 单调栈 |
|---|---|---|---|---|
| 时间 | O(n²) | O(n) | O(n) | O(n) |
| 空间 | O(1) | O(n) | **O(1)** | O(n) |
| 思维方式 | 按列，逐个扫描 | 按列，预计算 | 按列，短板原理 | 按层，横向填充 |
| 代码难度 | 简单 | 简单 | 中等 | 较难 |
| 面试推荐 | 开场说思路 | 过渡方案 | **首选** | 若面试官追问栈 |

**选择建议**: 
- 面试时先说暴力思路（30秒），然后说预处理优化（展示思维过程），最后写双指针（最优解）。
- 如果面试官明确问"用栈怎么做"，再写单调栈。

## 易错点

1. **双指针的 `<=` vs `<`**
   - ✗ `while (left < right)` — 漏掉 left == right 那个位置的水
   - ✓ `while (left <= right)` — 每个位置都要计算

2. **leftMax/rightMax 是否包含当前位置**
   - ✗ 先算水再更新 max：`ans += leftMax - height[left]; leftMax = max(leftMax, height[left]);`
   - ✓ 先更新 max 再算水：`leftMax = max(leftMax, height[left]); ans += leftMax - height[left];`
   - 因为当前柱子本身也是"左边的墙"之一，先更新保证 leftMax >= height[left]，差值非负。

3. **单调栈弹出后忘记检查栈空**
   - ✗ 弹出 bottom 后直接取 `stk.top()` → 栈空时 crash
   - ✓ 弹出 bottom 后先 `if (stk.empty()) break;`

4. **单调栈的单调方向搞反**
   - 本题是**单调递减栈**（从底到顶递减），遇到更高的元素才弹出结算。
   - 如果搞成递增栈，逻辑完全不对。

## 面试追问

**Q1: 暴力法能过吗？瓶颈在哪？**
> 暴力 O(n²) 对 n ≤ 2×10⁴ 勉强能过（约 4×10⁸ 运算量偏大），但面试不会接受。瓶颈是每个位置都要重新扫描左右找最大值，存在大量重复计算。

**Q2: 预处理 DP 用了 O(n) 空间，能优化到 O(1) 吗？**
> 双指针。核心观察：`min(leftMax, rightMax)` 只需要知道谁更小。从两端向中间走，短板那一侧可以直接确定水位，因为另一侧至少有一个不低于短板的柱子"兜底"。

**Q3: 如果这道题变成二维（矩阵接雨水），怎么做？**
> 这就是 LeetCode 407（接雨水 II）。思路类似但用优先队列（最小堆）：从矩阵四周边界入手，每次取最矮的边界向内扩展，类似"木桶原理"——最短的板决定水位。时间 O(mn·log(mn))。

**Q4: 双指针的正确性如何严格证明？**
> 不变量：在任意时刻，如果 `height[left] <= height[right]`，则 `rightMax >= height[right] >= height[left]`。所以 `min(leftMax, rightMax)` 中的 rightMax 一定 ≥ leftMax（否则我们走的是 right 那边）。因此 left 位置的水位就是 `leftMax - height[left]`。对称地，right 那侧同理。

## 相关题型

- **LeetCode 11 盛最多水的容器** — 同样用双指针+短板原理，但 11 题是求两根柱子之间的最大面积，42 题是求所有柱子上方的总积水量。核心区别：11 题只看两根柱子，42 题每个位置都要考虑。
- **LeetCode 407 接雨水 II** — 42 题的二维版本，从双指针升级为优先队列(BFS)，复用"短板决定水位"的思想。
- **LeetCode 84 柱状图中最大的矩形** — 同样使用单调栈，但方向相反：84 题用单调递增栈找每个柱子能扩展的左右边界，42 题用单调递减栈找凹槽。两道题是单调栈的正反面。
- **LeetCode 739 每日温度** — 单调栈入门题，复用"遇到更大元素就弹出结算"的模式。先做 739 再做 42 的栈解法会更容易理解。