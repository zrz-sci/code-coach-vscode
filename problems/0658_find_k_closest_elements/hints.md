# 658. 找到 K 个最接近的元素

## 核心思路

本质上在问：在一个有序数组中，找到一个长度为 k 的**连续子数组**（窗口），使得这个窗口内的元素是距离 x 最近的 k 个。关键洞察是——**答案一定是原数组中连续的 k 个元素**，因为数组有序，离 x 最近的元素一定聚集在一起。

## 思维链

1. **读完题第一反应**：对每个元素算 `|arr[i] - x|`，然后取最小的 k 个 → 排序/堆都能做，但结果还要排序输出。
2. **意识到关键性质**：数组已排序！离 x 最近的 k 个元素一定是**连续的一段**，因为越远离 x 的元素，距离单调递增。
3. **暴力窗口思路**：既然答案是连续的 k 个元素，我只需找到这个窗口的**左端点**。窗口有 `n - k + 1` 种位置，暴力枚举每种并算总距离？O(nk) 可以接受但不够优。
4. **双指针收缩**：既然答案是长度 k 的窗口，我可以从整个数组（长度 n）开始，每次从两端去掉一个离 x 更远的元素，重复 `n - k` 次。
5. **二分查找窗口左端点**：窗口左端点 `left` 的范围是 `[0, n-k]`。对于某个 `left`，比较 `x - arr[left]` 和 `arr[left+k] - x`：如果左端离 x 更远，说明窗口应该右移 → 这就是标准的二分查找！O(log(n-k))。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 排序法 | 按距离排序取前k个 | O(n log n) | O(n) | 能说出即可 |
| 双指针收缩 | 两端去掉离x更远的 | O(n) | O(1) | ⭐ 必须写出 |
| 二分查找窗口左端点 | 二分找最优窗口位置 | O(log(n-k) + k) | O(1) | 加分项 |

## 关键提示

1. **答案是连续的**：数组有序，最近的 k 个元素一定在原数组中形成一个连续子数组。如果你没意识到这一点，会走很多弯路。
2. **双指针收缩方向**：比较 `x - arr[left]` 和 `arr[right] - x`，谁大就去掉谁。距离相等时去掉右边（因为题目要求相等距离取较小值）。
3. **二分的搜索空间**：不是在整个数组上二分找 x 的位置，而是在 `[0, n-k]` 上二分找**窗口左端点**。
4. **二分的比较条件**：比较 `x - arr[mid]` 和 `arr[mid+k] - x`，**不要取绝对值**，因为不取绝对值天然处理了 x 在窗口外的情况。

```
// 二分搜索空间收缩示例: arr = [1,2,3,4,5], k=4, x=3
// 窗口左端点范围: [0, 1]  (n-k = 5-4 = 1)
//
// Round1: left=0, right=1, mid=0
//   比较: x - arr[0] = 3-1 = 2  vs  arr[0+4] - x = 5-3 = 2
//   相等 → 不右移 (取较小值) → right = mid = 0
//
// Round2: left=0, right=0 → 结束
// 窗口: arr[0..3] = [1,2,3,4] ✓
```

## 解法详解

### 解法1: 排序法 — O(n log n) / O(n)

**思考过程**: 最直觉的方法——计算每个元素到 x 的距离，按距离排序，取前 k 个，再按值排序输出。这完全没利用"数组已排序"的性质，但能 work。

```cpp
class Solution {
public:
    vector<int> findClosestElements(vector<int>& arr, int k, int x) {
        // 按到 x 的距离排序，距离相等取值小的
        vector<int> sorted_arr(arr.begin(), arr.end());
        sort(sorted_arr.begin(), sorted_arr.end(), [x](int a, int b) {
            int da = abs(a - x), db = abs(b - x);
            return da != db ? da < db : a < b;
        });
        
        // 取前 k 个
        vector<int> result(sorted_arr.begin(), sorted_arr.begin() + k);
        
        // 结果要按升序输出
        sort(result.begin(), result.end());
        return result;
    }
};
```

**关键点**: 
- 完全没利用数组有序的性质，面试中说出来展示理解即可，但要主动提出优化。
- 自定义排序的 lambda 要正确处理距离相等的情况。

---

### 解法2: 双指针收缩 — O(n) / O(1) ⭐ 面试首选

**从解法1优化**: 解法1的问题是没利用有序性。既然答案是连续的 k 个元素，我们从长度 n 的数组两端开始，每次去掉离 x 更远的那个端点，收缩 `n - k` 次就得到答案。

```
// 双指针收缩过程: arr = [1,2,3,4,5], k=4, x=3
//
// 初始: left=0, right=4
//   [1, 2, 3, 4, 5]
//    ↑              ↑
//   |1-3|=2  vs  |5-3|=2  → 距离相等,去掉右端(取较小值)
//
// Step1: right=3
//   [1, 2, 3, 4]
//    ↑        ↑
//   结果: [1,2,3,4] ✓
```

```cpp
class Solution {
public:
    vector<int> findClosestElements(vector<int>& arr, int k, int x) {
        int left = 0, right = arr.size() - 1;
        
        // 每次去掉一个离 x 更远的端点，去掉 n-k 次
        while (right - left >= k) {
            // 为什么比较原始差值而不是绝对值？
            // 因为数组有序: arr[left] <= x 的情况下 x-arr[left] >= 0
            //              arr[right] >= x 的情况下 arr[right]-x >= 0
            // 但 x 可能不在 [left, right] 范围内，用绝对值更安全
            if (abs(arr[left] - x) > abs(arr[right] - x)) {
                left++;  // 左端离 x 更远，去掉
            } else if (abs(arr[left] - x) < abs(arr[right] - x)) {
                right--; // 右端离 x 更远，去掉
            } else {
                right--; // 距离相等，去掉右端（保留较小值）
            }
        }
        
        return vector<int>(arr.begin() + left, arr.begin() + right + 1);
    }
};
```

**关键点**:
- 循环条件是 `right - left >= k`（当前窗口大于 k 才需要收缩），不是 `right - left > k`。当 `right - left == k` 时窗口大小是 `k+1`，还需要收缩一次。
- 距离相等时去掉右端 `right--`，因为题目说"距离相同取较小值"，较小值在左边。

---

### 解法3: 二分查找窗口左端点 — O(log(n-k) + k) / O(1)

**从解法2优化**: 解法2是线性扫描 O(n)。但我们要找的是窗口左端点，左端点范围是 `[0, n-k]`，能不能二分搜索这个左端点？

**核心洞察**: 对于候选左端点 `mid`，窗口是 `arr[mid..mid+k-1]`。我们比较窗口**左边界外**的元素 `arr[mid]` 和**右边界外**的元素 `arr[mid+k]` 到 x 的距离：
- 如果 `x - arr[mid] > arr[mid+k] - x`：说明左端太远了，窗口应该右移 → `left = mid + 1`
- 否则：窗口位置合适或应该左移 → `right = mid`

```
// 二分搜索空间收缩:
// arr = [1,1,2,3,4,5], k=4, x=-1
// 窗口左端点范围: [0, 2]  (n-k = 6-4 = 2)
//
// Round1: left=0, right=2, mid=1
//   比较: x - arr[1] = -1-1 = -2  vs  arr[1+4] - x = 4-(-1) = 5
//   -2 <= 5 → right = mid = 1
//
// Round2: left=0, right=1, mid=0
//   比较: x - arr[0] = -1-1 = -2  vs  arr[0+4] - x = 4-(-1) = 5
//   -2 <= 5 → right = mid = 0
//
// Round3: left=0, right=0 → 结束
// 窗口: arr[0..3] = [1,1,2,3] ✓
```

```cpp
class Solution {
public:
    vector<int> findClosestElements(vector<int>& arr, int k, int x) {
        int left = 0, right = arr.size() - k;
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            // 关键：不取绝对值！
            // x - arr[mid] 可能为负（x 在窗口左边）
            // arr[mid+k] - x 可能为负（x 在窗口右边）
            // 不取绝对值恰好能正确处理这些情况
            if (x - arr[mid] > arr[mid + k] - x) {
                left = mid + 1;  // 窗口右移
            } else {
                right = mid;     // 窗口左移或不动
            }
        }
        
        return vector<int>(arr.begin() + left, arr.begin() + left + k);
    }
};
```

**为什么不取绝对值是正确的？**
- 如果 `x` 在 `arr[mid]` 左边：`x - arr[mid] < 0`，一定 `<= arr[mid+k] - x`（正数），所以 `right = mid`，窗口左移 ✓
- 如果 `x` 在 `arr[mid+k]` 右边：`arr[mid+k] - x < 0`，一定 `< x - arr[mid]`（正数），所以 `left = mid + 1`，窗口右移 ✓
- 如果 `x` 在窗口中间：两个值都是正数，直接比较距离 ✓
- 距离相等时：`x - arr[mid] == arr[mid+k] - x`，条件不成立，走 `right = mid`，保留左边（较小值）✓

---

### 解法4: 堆（了解即可）— O(n log k) / O(k)

**思路**: 用大小为 k 的大顶堆，按距离排序。遍历数组，如果当前元素比堆顶更近，替换堆顶。

```cpp
class Solution {
public:
    vector<int> findClosestElements(vector<int>& arr, int k, int x) {
        // 大顶堆: 按距离降序，距离相等按值降序
        auto cmp = [x](int a, int b) {
            int da = abs(a - x), db = abs(b - x);
            return da != db ? da < db : a < b;
        };
        priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);
        
        for (int num : arr) {
            pq.push(num);
            if ((int)pq.size() > k) {
                pq.pop(); // 去掉离 x 最远的
            }
        }
        
        vector<int> result;
        while (!pq.empty()) {
            result.push_back(pq.top());
            pq.pop();
        }
        sort(result.begin(), result.end());
        return result;
    }
};
```

**关键点**: 堆解法没利用数组有序性，是通用的 Top-K 思路。面试中如果忘了二分怎么写，这个方法也能过。

---

## 解法对比

| | 排序法 | 双指针收缩 | 二分查找 | 堆 |
|---|---|---|---|---|
| 时间 | O(n log n) | O(n) | O(log(n-k) + k) | O(n log k) |
| 空间 | O(n) | O(1)* | O(1)* | O(k) |
| 利用有序性 | ✗ | ✓ | ✓✓ | ✗ |
| 代码难度 | 低 | 低 | 中（二分边界易错） | 中 |
| 面试推荐 | 暖场 | ⭐首选 | 追问时给出 | 备选 |

\* 不算输出数组的空间

**什么时候选哪个？**
- 面试先说双指针收缩（简洁、O(n)、不易出错）
- 被追问"能否更快"时给出二分解法
- 数组无序时只能用排序或堆

## 易错点

1. **二分中取绝对值导致错误**：
   - ✗ `if (abs(x - arr[mid]) > abs(arr[mid+k] - x))` — 当 x 在窗口外时，绝对值会导致错误判断
   - ✓ `if (x - arr[mid] > arr[mid+k] - x)` — 不取绝对值，利用符号信息
   - 例：`arr=[1,2,3,4,5], k=4, x=10`，取绝对值时 `|10-1|=9 > |5-10|=5`，不取绝对值时 `10-1=9 > 5-10=-5`，都是 left++，这里恰好一样。但 `arr=[1,2,3,4,5], k=4, x=-10` 时取绝对值 `|-10-1|=11 > |5-(-10)|=15` 为 false 导致 right=mid，不取绝对值 `-10-1=-11 > 5-(-10)=15` 也为 false。看似都对？关键区别在中间情况：`arr=[1,2,3,4,5], k=3, x=3` 时 mid=1，取绝对值 `|3-2|=1 vs |5-3|=2` 和不取绝对值 `3-2=1 vs 5-3=2` 结果一样。实际上取绝对值在大多数情况能 work，但**距离相等时行为不同**——不取绝对值保证了"取较小值"的语义。

2. **双指针循环条件写错**：
   - ✗ `while (right - left > k)` — 当 `right - left == k` 时窗口大小是 `k+1`，还需要收缩一次
   - ✓ `while (right - left >= k)` — 当窗口恰好 k 个元素时停止（`right - left == k - 1`）

3. **二分的 right 初始值写错**：
   - ✗ `right = arr.size() - 1` — 窗口左端点最大是 `n-k`，不是 `n-1`
   - ✓ `right = arr.size() - k` — 确保 `arr[left..left+k-1]` 不越界

4. **二分中访问 `arr[mid+k]` 越界**：
   - 当 `right = n - k` 时，`mid` 最大是 `n - k - 1`（因为 `left < right`），`mid + k` 最大是 `n - 1` ✓ 不会越界
   - 但如果错写 `right = n - k + 1`，就会越界！

## 面试追问

**Q1: 暴力解能过吗？瓶颈在哪？**
> 排序法 O(n log n) 能过（n ≤ 10⁴），但没利用数组有序的条件。面试官会期望你利用有序性。

**Q2: 答案为什么一定是连续的？能证明吗？**
> 反证法：假设答案不连续，即选了 `arr[i]` 和 `arr[j]`（j > i+1）但没选 `arr[i+1]`。因为数组有序，`arr[i+1]` 在 `arr[i]` 和 `arr[j]` 之间，所以 `|arr[i+1]-x| <= max(|arr[i]-x|, |arr[j]-x|)`。这意味着 `arr[i+1]` 至少和 `arr[i]` 或 `arr[j]` 中的一个一样近，应该替换掉它，矛盾。

**Q3: O(n) 的双指针能否更快？**
> 可以！用二分查找窗口左端点，O(log(n-k) + k)。当 k 远小于 n 时这是显著的优化。具体做法是在 `[0, n-k]` 上二分，比较窗口左右边界外的元素到 x 的距离。

**Q4: 如果数组无序怎么办？**
> 无法用双指针/二分。用大小为 k 的堆 O(n log k)，或者 Quick Select 找第 k 近的元素 O(n) 平均，再收集所有不超过该距离的元素。

**Q5: 如果有大量查询（不同的 x 和 k）怎么优化？**
> 数组排序一次后，每次查询用二分查找 O(log(n-k) + k)。如果 k 固定，可以预处理所有长度为 k 的窗口的边界距离信息。

## 相关题型

- **374. 猜数字大小** — 复用二分查找"搜索空间收缩"的框架，区别是本题搜索的是窗口左端点而不是单个值
- **34. 在排序数组中查找元素的第一个和最后一个位置** — 同样是 `left < right` 模板的二分，找边界而不是找确切值
- **347. 前 K 个高频元素** — 复用堆的 Top-K 框架，区别是排序标准从"距离"变成"频率"
- **215. 数组中的第K个最大元素** — Top-K 问题的另一个经典，可以用堆或 Quick Select