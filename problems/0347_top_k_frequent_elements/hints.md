# 347. 前 K 个高频元素

## 核心思路

本质上是一个 **Top K 问题**：先统计每个元素的出现频率，然后从所有频率中找出最大的 K 个对应的元素。问题的关键不在于"统计频率"（哈希表 O(n) 搞定），而在于"怎么高效地从 n 个频率中选出 Top K"。

## 思维链

1. **读完题第一反应**：先用哈希表统计每个元素出现的次数，然后按次数排序，取前 K 个。排序是 O(n log n)，能不能更快？

2. **暴力解的瓶颈**：排序是 O(n log n)，但我们只需要 Top K 而不需要完全有序。对所有元素排序做了多余的工作——我们不关心第 K+1 到第 n 的相对顺序，也不关心前 K 个之间的相对顺序。

3. **突破方向1 — 堆**：只需要 Top K，经典做法是用一个大小为 K 的小顶堆。遍历所有频率，堆满了就和堆顶比：比堆顶大就替换。这样堆里始终维护着"见过的最大的 K 个"，时间 O(n log k)。

4. **突破方向2 — 桶排序**：频率的范围是 [1, n]，可以开一个大小为 n+1 的桶数组，`bucket[i]` 存所有出现 i 次的元素。然后从高频到低频遍历桶，凑够 K 个就停。时间 O(n)！

5. **突破方向3 — 快速选择**：类似快排的 partition，随机选 pivot，把频率大于 pivot 的放一边、小于的放另一边。只递归需要的那一半，期望 O(n)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 排序法 | 统计频率 + 按频率排序 | O(n log n) | O(n) | 能说出即可 |
| 小顶堆 | 维护大小为 K 的小顶堆 | O(n log k) | O(n) | ⭐ 必须写出 |
| 桶排序 | 频率做下标，从高到低收集 | O(n) | O(n) | 加分项 |
| 快速选择 | 类快排 partition | O(n) 期望 | O(n) | 加分项 |

## 关键提示

1. **第一步永远是统计频率**：不管用什么方法，`unordered_map<int, int>` 统计每个数出现次数是前置步骤，O(n) 完成。

2. **堆的大小是 K 不是 N**：很多人用大顶堆把所有元素放进去再弹 K 个，这是 O(n log n)。正确做法是维护大小为 K 的**小顶堆**，堆顶是"Top K 中最小的"，新来的比堆顶大才有资格替换它。

3. **为什么用小顶堆而不是大顶堆？** 小顶堆堆顶是最小值，充当"门槛"。比门槛大的才能进来，进来时把最小的踢出去。最终剩下的就是最大的 K 个。

4. **桶排序的关键洞察**：频率范围有限（最多 n），所以可以用频率当数组下标。这是计数排序的思想——当值域有限时，排序可以 O(n)。

5. **进阶要求 O(n log n) 以内**：排序法刚好卡在 O(n log n)，不满足"优于"的要求。堆法 O(n log k) 满足，桶排序 O(n) 更优。

## 解法详解

### 解法1: 排序法 — O(n log n) / O(n)

**思考过程**: 最直觉的方法——统计完频率后，把 (元素, 频率) 对按频率从大到小排序，取前 K 个。

```cpp
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        // Step1: 统计频率
        unordered_map<int, int> freq;
        for (int num : nums) freq[num]++;
        
        // Step2: 把所有 (元素, 频率) 对放入数组
        vector<pair<int, int>> freqVec(freq.begin(), freq.end());
        
        // Step3: 按频率降序排序
        sort(freqVec.begin(), freqVec.end(), 
             [](const pair<int,int>& a, const pair<int,int>& b) {
                 return a.second > b.second;
             });
        
        // Step4: 取前 K 个
        vector<int> result;
        for (int i = 0; i < k; i++) {
            result.push_back(freqVec[i].first);
        }
        return result;
    }
};
```

**关键点**: 这个方法简单直接，但排序是 O(n log n)，不满足 Follow-up 要求。而且我们对前 K 个之后的元素做了不必要的排序工作。

---

### 解法2: 小顶堆 — O(n log k) / O(n)  ⭐ 面试首选

**从解法1优化**: 排序是 O(n log n) 因为要对所有 n 个不同元素排序。但我们只要 Top K 个——用小顶堆维护"当前最大的 K 个"，每次插入/删除只需 O(log k)。

```
// 堆的工作过程 (nums=[1,1,1,2,2,3], k=2):
// 频率: {1:3, 2:2, 3:1}
// 
// 遍历频率表:
//   (1,3) → 堆: [(1,3)]              堆未满，直接加入
//   (2,2) → 堆: [(2,2), (1,3)]       堆未满，直接加入（小顶堆，堆顶是频率最小的）
//   (3,1) → 1 < 堆顶频率2 → 不加入    频率不够大，没资格进 Top K
// 
// 最终堆中: [(2,2), (1,3)] → 结果 [2, 1]
```

```cpp
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        // Step1: 统计频率
        unordered_map<int, int> freq;
        for (int num : nums) freq[num]++;
        
        // Step2: 小顶堆，按频率排序，堆顶是频率最小的
        // pair<频率, 元素值>，默认按 first 比较，所以频率放前面
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> minHeap;
        
        for (auto& [num, cnt] : freq) {
            minHeap.push({cnt, num});
            // 堆大小超过 K 就弹出堆顶（频率最小的），保持堆里是最大的 K 个
            if ((int)minHeap.size() > k) {
                minHeap.pop();
            }
        }
        
        // Step3: 堆中剩下的就是 Top K
        vector<int> result;
        while (!minHeap.empty()) {
            result.push_back(minHeap.top().second);
            minHeap.pop();
        }
        return result;
    }
};
```

**关键点**: 
- 用 `pair<频率, 元素>` 而不是 `pair<元素, 频率>`，因为 priority_queue 默认按 first 排序
- 小顶堆用 `greater<>`，堆顶是最小值
- 堆大小维持在 K，所以每次 push/pop 是 O(log k) 而不是 O(log n)

---

### 解法3: 桶排序 — O(n) / O(n)  加分项

**关键洞察**: 频率值的范围是 [1, n]（n 是数组长度），这是一个有限且不大的范围。我们可以用频率作为桶的下标：`bucket[f]` 存储所有出现 f 次的元素。然后从高频到低频遍历桶，凑够 K 个就停。

```
// 桶排序过程 (nums=[1,1,1,2,2,3], k=2):
// 频率: {1:3, 2:2, 3:1}
// 
// 桶 (下标=频率):
// bucket[0]: []
// bucket[1]: [3]       ← 出现1次的元素
// bucket[2]: [2]       ← 出现2次的元素
// bucket[3]: [1]       ← 出现3次的元素
// bucket[4]: []
// bucket[5]: []
// bucket[6]: []
// 
// 从高到低遍历:
// bucket[6]=[] → 跳过
// bucket[5]=[] → 跳过
// bucket[4]=[] → 跳过
// bucket[3]=[1] → 加入结果, 已收集1个
// bucket[2]=[2] → 加入结果, 已收集2个 == k → 停！
// 
// 结果: [1, 2]
```

```cpp
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        // Step1: 统计频率
        unordered_map<int, int> freq;
        for (int num : nums) freq[num]++;
        
        // Step2: 建桶，bucket[i] = 出现 i 次的所有元素
        int n = nums.size();
        vector<vector<int>> bucket(n + 1);
        for (auto& [num, cnt] : freq) {
            bucket[cnt].push_back(num);
        }
        
        // Step3: 从高频到低频遍历桶，凑够 K 个
        vector<int> result;
        for (int i = n; i >= 1 && (int)result.size() < k; i--) {
            for (int num : bucket[i]) {
                result.push_back(num);
                if ((int)result.size() == k) break;
            }
        }
        return result;
    }
};
```

**关键点**: 
- 桶的大小是 n+1（频率最大为 n，即所有元素都相同时）
- 时间 O(n) 是因为建桶和遍历桶都是线性的
- 空间 O(n)：桶数组的总容量是 O(n)（所有桶中元素总数等于不同元素个数）

---

### 解法4: 快速选择 — O(n) 期望 / O(n)

**思路**: 类似快排的 partition，找到频率第 K 大的"分界线"。partition 一次后，如果 pivot 左边刚好有 K 个元素，就找到了；否则只递归需要的那一半。期望 O(n)，最坏 O(n²)。

```cpp
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> freq;
        for (int num : nums) freq[num]++;
        
        // 把所有不同元素放入数组
        vector<int> unique;
        for (auto& [num, cnt] : freq) unique.push_back(num);
        
        int n = unique.size();
        // 目标：让前 k 个位置放频率最高的 k 个元素（不要求内部有序）
        quickSelect(unique, freq, 0, n - 1, k);
        
        return vector<int>(unique.begin(), unique.begin() + k);
    }
    
private:
    void quickSelect(vector<int>& arr, unordered_map<int,int>& freq, 
                     int left, int right, int k) {
        if (left >= right) return;
        
        // 随机选 pivot 避免最坏情况
        int pivotIdx = left + rand() % (right - left + 1);
        int pivotFreq = freq[arr[pivotIdx]];
        swap(arr[pivotIdx], arr[right]);
        
        // partition：把频率 >= pivotFreq 的放左边
        int storeIdx = left;
        for (int i = left; i < right; i++) {
            if (freq[arr[i]] >= pivotFreq) {
                swap(arr[i], arr[storeIdx]);
                storeIdx++;
            }
        }
        swap(arr[storeIdx], arr[right]);
        
        // storeIdx 左边有 (storeIdx - left) 个频率 >= pivot 的元素
        int count = storeIdx - left + 1; // 包含 pivot 本身
        if (count == k) {
            return; // 前 k 个就是答案
        } else if (count > k) {
            quickSelect(arr, freq, left, storeIdx - 1, k); // 答案在左半
        } else {
            quickSelect(arr, freq, storeIdx + 1, right, k - count); // 还需要右半凑
        }
    }
};
```

**关键点**: 快速选择期望 O(n) 但最坏 O(n²)，随机化选 pivot 可以大幅降低最坏情况概率。面试时一般堆或桶排序更稳妥。

---

## 解法对比

| | 排序法 | 小顶堆 | 桶排序 | 快速选择 |
|---|---|---|---|---|
| 时间 | O(n log n) | O(n log k) | O(n) | O(n) 期望 |
| 空间 | O(n) | O(n) | O(n) | O(n) |
| 满足 Follow-up | ✗ | ✓ | ✓ | ✓ |
| 代码难度 | 简单 | 中等 | 简单 | 较难 |
| 面试推荐 | 开场白 | **首选** | 加分 | 了解即可 |

**什么时候选哪个？**
- **面试**：先说排序法表示理解题意，然后写小顶堆。如果面试官追问能否 O(n)，给出桶排序。
- **实际工程**：如果 k 远小于 n（如 n=10⁶, k=10），堆法非常高效；如果 n 不大，桶排序代码最简洁。

## 易错点

1. **小顶堆 vs 大顶堆搞反**
   - ✗ 用大顶堆，把所有元素放进去，弹出 K 个 → O(n log n)，没有优化效果
   - ✓ 用小顶堆，维护大小为 K，堆顶是"门槛"，新元素比门槛大才替换

2. **pair 顺序写反**
   - ✗ `priority_queue` 中存 `{num, freq}`，默认按 num 排序 → 结果错误
   - ✓ 存 `{freq, num}`，确保按频率排序

3. **桶排序桶大小开错**
   - ✗ `vector<vector<int>> bucket(n)` → 频率为 n 时越界
   - ✓ `vector<vector<int>> bucket(n + 1)` → 频率范围 [0, n]

4. **快速选择中 partition 方向搞反**
   - ✗ 把频率小的放左边（升序 partition）→ 前 K 个是最小的
   - ✓ 把频率大的放左边（降序 partition）→ 前 K 个是最大的

## 面试追问

**Q1: 暴力排序法是 O(n log n)，瓶颈在哪？**
> 瓶颈在排序步骤。统计频率只需 O(n)，但排序需要 O(n log n)。而我们只需要 Top K，不需要完全排序，所以排序做了多余的工作。

**Q2: 用堆优化到 O(n log k)，为什么用小顶堆不用大顶堆？**
> 大顶堆要把所有 n 个元素放进去再弹 K 次，总复杂度 O(n log n + k log n)。小顶堆维持大小 K，堆顶是"第 K 大"的门槛值，只有比它大的才能进入，总复杂度 O(n log k)。当 k << n 时差距巨大。

**Q3: 能否做到严格 O(n)？不能用堆。**
> 可以，用桶排序。频率范围 [1, n] 有限，用频率做桶下标，O(n) 建桶，O(n) 从高到低遍历收集 K 个。或者用快速选择算法，期望 O(n)。

**Q4: 如果这个查询会被调用很多次（频率表不变，但 k 每次不同），怎么优化？**
> 预先对频率表建好桶（或者排序好），之后每次查询只需 O(k) 遍历即可。也可以建一个频率的前缀和，二分查找第 K 大频率对应的桶位置。

## 相关题型

- **215. 数组中的第K个最大元素** — 复用小顶堆/快速选择的 Top K 框架，区别：347 先要统计频率再做 Top K，215 直接在值上做 Top K
- **692. 前K个高频单词** — 几乎相同，区别：频率相同时要按字典序排序，需要自定义比较函数
- **451. 根据字符出现频率排序** — 复用桶排序框架，区别：要输出所有字符（k=n），且同频字符要重复输出
- **973. 最接近原点的 K 个点** — 复用小顶堆 Top K 框架，区别：比较对象从频率变成了距离