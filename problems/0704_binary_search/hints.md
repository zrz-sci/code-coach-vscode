# 704. 二分查找

## 核心思路

在一个**有序且无重复元素**的数组中查找目标值。有序 = 每次可以排除一半搜索空间 = 二分查找。

## 思维链

1. **读完题第一反应**：遍历数组，逐个比较，找到就返回下标 → 线性扫描 O(n)。
2. **暴力解的瓶颈**：每个元素都要看一遍，没有利用"数组已排序"这个关键条件。
3. **怎么突破瓶颈？** 既然数组有序，我看中间那个元素：如果中间值 > target，说明 target 只可能在左半边；如果中间值 < target，说明只可能在右半边。一次比较直接排除一半！
4. **具体怎么做？** 维护 `left` 和 `right` 两个边界，每次算 `mid`，根据比较结果缩小搜索范围，直到找到或范围为空。
5. **复杂度变化**：每次搜索空间减半，所以只需 O(log n) 次比较。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 线性扫描 | 逐个比较 | O(n) | O(1) | 能说出即可 |
| 二分查找（闭区间） | `left <= right` | O(log n) | O(1) | ⭐ 必须写出 |
| 二分查找（左闭右开） | `left < right` | O(log n) | O(1) | 加分项（理解变体） |
| 递归二分 | 递归缩小范围 | O(log n) | O(log n) 栈空间 | 能说出即可 |

## 关键提示

- **提示1**：数组有序 + 查找 = 二分的信号。想想每次比较能排除多少元素？
- **提示2**：二分最关键的是确定"搜索区间的定义"——`[left, right]` 闭区间还是 `[left, right)` 左闭右开？定义不同，循环条件和边界更新都不同。
- **提示3**：计算 `mid` 时，`(left + right) / 2` 在大数时可能溢出，用 `left + (right - left) / 2` 更安全。
- **提示4**：搜索空间收缩过程图解（以 `nums = [-1,0,3,5,9,12]`, `target = 9` 为例）：

```
Round1: [-1, 0, 3, 5, 9, 12]   left=0, right=5, mid=2
                  ↑              nums[2]=3 < 9 → left=3
Round2: [_, _, _, 5, 9, 12]     left=3, right=5, mid=4
                     ↑           nums[4]=9 == 9 → 找到! 返回4
```

## 解法详解

### 解法1: 线性扫描 — O(n) / O(1)

**思考过程**：最直觉的方式，从头到尾扫一遍，碰到相等的就返回下标。没用上"有序"这个条件，但能验证我们对题目的理解是否正确。

```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] == target) return i;
        }
        return -1;
    }
};
```

**关键点**：这个解法正确但不满足题目 O(log n) 的要求。它的价值在于：(1) 验证理解；(2) 暴露瓶颈——每个元素都看了一遍，完全没利用有序性。

---

### 解法2: 二分查找（闭区间 `[left, right]`） — O(log n) / O(1) ⭐ 面试首选

**从解法1优化**：线性扫描的瓶颈是"没利用有序性"。数组有序意味着：看一个中间值，就能判断 target 在左半还是右半，一次排除一半。

**搜索区间定义**：`[left, right]` 是闭区间，即 left 和 right 都是合法的待搜索位置。

```
搜索区间定义决定了三个细节：
1. 循环条件：left <= right（区间 [left, left] 仍有一个元素，合法）
2. 左边界更新：left = mid + 1（mid 已经检查过，排除它）
3. 右边界更新：right = mid - 1（mid 已经检查过，排除它）
```

```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = nums.size() - 1;  // 闭区间 [0, n-1]
        
        while (left <= right) {                   // 区间非空就继续
            int mid = left + (right - left) / 2;  // 防溢出写法
            
            if (nums[mid] == target) {
                return mid;                        // 找到了
            } else if (nums[mid] < target) {
                left = mid + 1;   // target 在右半边，排除 mid 及左边
            } else {
                right = mid - 1;  // target 在左半边，排除 mid 及右边
            }
        }
        
        return -1;  // 搜索空间为空，没找到
    }
};
```

**完整收缩过程（示例1：target=9）**：
```
nums = [-1, 0, 3, 5, 9, 12]
         0  1  2  3  4   5

Round1: left=0, right=5 → mid=2, nums[2]=3  < 9 → left=3
  搜索区间: [3, 5] → {5, 9, 12}
  
Round2: left=3, right=5 → mid=4, nums[4]=9 == 9 → return 4 ✓
```

**完整收缩过程（示例2：target=2）**：
```
nums = [-1, 0, 3, 5, 9, 12]
         0  1  2  3  4   5

Round1: left=0, right=5 → mid=2, nums[2]=3  > 2 → right=1
  搜索区间: [0, 1] → {-1, 0}

Round2: left=0, right=1 → mid=0, nums[0]=-1 < 2 → left=1
  搜索区间: [1, 1] → {0}

Round3: left=1, right=1 → mid=1, nums[1]=0  < 2 → left=2
  搜索区间: [2, 1] → 空! left > right → 退出循环

return -1 ✓
```

**关键点**：这是最经典、最推荐的二分写法。面试中首选这个版本，因为逻辑对称、不易出错。

---

### 解法3: 二分查找（左闭右开 `[left, right)`） — O(log n) / O(1)

**换一种区间定义**：`[left, right)` 左闭右开，right 是"第一个不在搜索范围内的位置"。

```
区间定义变了，三个细节跟着变：
1. 初始化：right = nums.size()（而不是 nums.size()-1）
2. 循环条件：left < right（当 left==right 时 [left, left) 是空区间）
3. 右边界更新：right = mid（而不是 mid-1，因为 right 本身不含在区间内）
```

```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = nums.size();  // 左闭右开 [0, n)
        
        while (left < right) {               // [left, left) 为空，停止
            int mid = left + (right - left) / 2;
            
            if (nums[mid] == target) {
                return mid;
            } else if (nums[mid] < target) {
                left = mid + 1;   // 排除 mid，新区间 [mid+1, right)
            } else {
                right = mid;      // 排除 mid，新区间 [left, mid)
                                  // 注意：不是 mid-1，因为 right 本身就不含在内
            }
        }
        
        return -1;
    }
};
```

**关键点**：左闭右开在 STL（如 `std::lower_bound`）中更常用。理解两种写法的差异，面试中能说清楚"为什么 `right = mid` 而不是 `mid - 1`"会加分。

---

### 解法4: 递归二分 — O(log n) / O(log n)

**思考过程**：二分天然是"缩小问题规模"的过程，很适合递归表达。但递归有 O(log n) 的栈空间开销，实际面试中很少选这个。

```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        return binarySearch(nums, target, 0, nums.size() - 1);
    }
    
private:
    int binarySearch(vector<int>& nums, int target, int left, int right) {
        if (left > right) return -1;  // base case: 搜索空间为空
        
        int mid = left + (right - left) / 2;
        
        if (nums[mid] == target) return mid;
        if (nums[mid] < target) 
            return binarySearch(nums, target, mid + 1, right);  // 搜右半
        else 
            return binarySearch(nums, target, left, mid - 1);   // 搜左半
    }
};
```

**关键点**：递归版本和迭代版本逻辑完全一致，只是用函数调用栈代替了 while 循环。实际项目和面试中优先用迭代版（省空间，无栈溢出风险）。

---

## 解法对比

| | 线性扫描 | 闭区间二分 ⭐ | 左闭右开二分 | 递归二分 |
|---|---|---|---|---|
| 时间 | O(n) | O(log n) | O(log n) | O(log n) |
| 空间 | O(1) | O(1) | O(1) | O(log n) |
| 循环条件 | - | `<=` | `<` | 递归终止 |
| right 初始值 | - | `n-1` | `n` | `n-1` |
| right 更新 | - | `mid-1` | `mid` | `mid-1` |
| 适用场景 | 验证正确性 | **面试首选** | STL 风格 | 教学用 |

**核心区别**：闭区间和左闭右开的差异全部源于"搜索区间的定义"。一旦确定定义，循环条件和边界更新就是确定的。

## 易错点

1. **mid 计算溢出**
   - ✗ `int mid = (left + right) / 2;` → 当 left 和 right 都很大时，`left + right` 溢出
   - ✓ `int mid = left + (right - left) / 2;` → 先算差值，不会溢出

2. **闭区间写法中用 `left < right` 导致漏元素**
   - ✗ `while (left < right)` 配合 `right = mid - 1` → 当 `left == right` 时直接跳出循环，漏掉了最后一个待检查的元素
   - ✓ `while (left <= right)` 配合 `right = mid - 1` → `[left, left]` 仍有一个元素需要检查

3. **左闭右开写法中 `right = mid - 1` 导致跳过元素**
   - ✗ `right = mid - 1` 在左闭右开区间中 → `[left, mid-1)` 会漏掉 `mid-1` 这个位置
   - ✓ `right = mid` → `[left, mid)` 正确排除了 mid 且不漏掉 mid-1

4. **死循环陷阱（在找边界的变体中）**
   - 当 `left + 1 == right` 且 `mid = left + (right - left) / 2 = left` 时，如果 `left = mid`（而不是 `mid + 1`）就会死循环
   - 本题因为三路分支（相等直接返回），不会死循环，但要注意变体题

## 面试追问

**Q1: 为什么二分查找是 O(log n)？**
每次比较排除一半搜索空间。初始 n 个元素，经过 k 次后剩 n/2^k 个。当 n/2^k = 1 时，k = log₂n。所以最多比较 log₂n 次。

**Q2: 如果数组有重复元素，要找 target 第一次出现的位置怎么办？**
找到 `nums[mid] == target` 时不立即返回，而是继续缩小右边界：`right = mid`（左闭右开）或 `right = mid - 1`（闭区间但最后要验证）。这就是 `lower_bound` 的思想。→ 参考 LeetCode 34。

**Q3: 如果搜索空间不是数组索引，而是一个连续范围呢？**
比如"求 x 的平方根"，搜索空间是 `[0, x]`，判断条件是 `mid * mid <= x`。二分的本质是"在单调性上排除一半"，不局限于数组。→ 参考 LeetCode 69。

**Q4: 能否用 STL 一行解决？**
```cpp
auto it = lower_bound(nums.begin(), nums.end(), target);
return (it != nums.end() && *it == target) ? it - nums.begin() : -1;
```
面试中先手写二分展示基本功，再提 STL 展示工程素养。

## 相关题型

- **35. 搜索插入位置** — 复用本题二分框架，区别：找不到时不返回 -1，而是返回应该插入的位置（即 `left`）
- **34. 在排序数组中查找元素的第一个和最后一个位置** — 复用二分框架，但要写 `lower_bound` 和 `upper_bound` 两个变体，核心区别：找到 target 时不停止，继续缩小边界
- **69. x 的平方根** — 复用二分思想，但搜索空间不是数组索引而是答案范围 `[0, x]`
- **153. 寻找旋转排序数组中的最小值** — 复用二分框架，但判断条件从"和 target 比"变成"和 right 端点比"
- **33. 搜索旋转排序数组** — 本题的进阶版，数组不完全有序，需要先判断哪半边有序再决定搜索方向