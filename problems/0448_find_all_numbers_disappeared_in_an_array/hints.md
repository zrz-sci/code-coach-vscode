# 448. 找到所有数组中消失的数字

## 核心思路

nums[i] 的范围是 [1, n]，刚好可以作为数组索引。核心技巧是**利用数组本身作为哈希表**——把出现过的数对应位置做标记（取负或加 n），最后未被标记的位置就是缺失的数字。这是一类经典的**原地哈希**技巧。

## 思维链

1. **读完题第一反应** → 用 HashSet 存所有出现过的数，再遍历 1~n 找缺失的。O(n) 时间 O(n) 空间。简单但不满足 Follow-up。
2. **Follow-up 要求 O(1) 额外空间** → 不能用额外的 Set/Map。但数组本身就有 n 个位置，且 nums[i] 在 [1, n]，可以建立数字 → 索引的映射。
3. **怎么标记？** → 数字 x 出现了 → 把 `nums[x-1]` 取负，表示"x 已出现"。因为原始值都是正数，取负就是一种标记。
4. **重复出现怎么办？** → 同一个位置可能被标记多次（如 nums = [1,1]），但 `abs()` 可以还原出原始索引，且标记操作是幂等的（已经是负数就不再取负）。
5. **怎么找缺失？** → 第二遍遍历：`nums[i] > 0` 说明 `i+1` 从未出现。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| HashSet | 存出现过的数 | O(n) | O(n) | 基础 |
| 原地标记（取负） | 数组索引当哈希 | O(n) | O(1) | ⭐ 最优 |
| 原地标记（加 n） | 值 > n 表示已出现 | O(n) | O(1) | 变体 |
| 交换归位 | 把 x 放到 nums[x-1] | O(n) | O(1) | 变体 |

## 关键提示

1. **取负标记的核心**: `nums[abs(nums[i]) - 1] = -abs(nums[abs(nums[i]) - 1])`。这里 `abs()` 有两个作用：(a) 计算索引时还原可能已被取负的值；(b) 确保标记操作是幂等的。
2. **为什么不能直接用 `nums[nums[i]-1] = -nums[nums[i]-1]`**：因为 `nums[i]` 可能已经是负数了（被之前的标记改过），所以必须先取 abs。
3. **加 n 变体**：另一种标记方式是 `nums[(nums[i]-1) % n] += n`。第二遍检查 `nums[i] <= n` 的位置就是缺失的。好处是不改变正负号。
4. **交换归位变体**：把数字 x 放到 `nums[x-1]` 的位置（原地排序）。第二遍检查 `nums[i] != i+1` 的位置就是缺失的。
5. **与 LC 442 的联系**：442 找出现两次的数字，用完全相同的"取负标记"技巧，只是收集条件不同——442 收集"已经是负的位置"（标记第二次时发现），448 收集"仍为正的位置"。

## 解法详解

### 解法1: HashSet — O(n) / O(n)

**思考过程**: 最直觉的做法。用 Set 记录出现过的数，再遍历 1~n 找不在 Set 中的。

```cpp
class Solution {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        unordered_set<int> seen(nums.begin(), nums.end());
        vector<int> result;
        int n = nums.size();
        for (int i = 1; i <= n; i++) {
            if (seen.find(i) == seen.end()) {
                result.push_back(i);
            }
        }
        return result;
    }
};
```

**关键点**: 时间 O(n)，空间 O(n)，不满足 Follow-up 的 O(1) 额外空间要求。

---

### 解法2: 原地标记（取负）— O(n) / O(1) ⭐ 面试首选

**从解法1优化**: 不用额外 Set，利用数组索引本身做"哈希表"。数字 x 出现了 → 把 `nums[x-1]` 标记为负。最后正数位置就是缺失的。

```
// 示例: nums = [4,3,2,7,8,2,3,1]
//
// 第一遍标记：
// i=0: nums[0]=4  → 标记 nums[3] = -7    → [4,3,2,-7,8,2,3,1]
// i=1: nums[1]=3  → 标记 nums[2] = -2    → [4,3,-2,-7,8,2,3,1]
// i=2: nums[2]=-2 → abs=2, 标记 nums[1]=-3 → [4,-3,-2,-7,8,2,3,1]
// i=3: nums[3]=-7 → abs=7, 标记 nums[6]=-3 → [4,-3,-2,-7,8,2,-3,1]
// i=4: nums[4]=8  → 标记 nums[7] = -1    → [4,-3,-2,-7,8,2,-3,-1]
// i=5: nums[5]=2  → abs=2, nums[1]已负,跳 → [4,-3,-2,-7,8,2,-3,-1]
// i=6: nums[6]=-3 → abs=3, nums[2]已负,跳 → [4,-3,-2,-7,8,2,-3,-1]
// i=7: nums[7]=-1 → abs=1, 标记 nums[0]=-4 → [-4,-3,-2,-7,8,2,-3,-1]
//
// 第二遍收集：
// nums[4]=8 > 0 → 5 缺失
// nums[5]=2 > 0 → 6 缺失
// 结果: [5, 6]
```

```cpp
class Solution {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        int n = nums.size();
        
        // 第一遍：标记出现过的数字
        for (int i = 0; i < n; i++) {
            int idx = abs(nums[i]) - 1;  // 数字 x 对应索引 x-1
            if (nums[idx] > 0) {
                nums[idx] = -nums[idx];  // 标记为负
            }
        }
        
        // 第二遍：收集仍为正的位置
        vector<int> result;
        for (int i = 0; i < n; i++) {
            if (nums[i] > 0) {
                result.push_back(i + 1);  // 索引 i 对应数字 i+1
            }
        }
        
        return result;
    }
};
```

**关键点**: 
- `abs(nums[i]) - 1` 计算索引，因为 nums[i] 可能已被标记为负。
- 检查 `nums[idx] > 0` 再取负，避免重复取负变回正。
- 第二遍 `nums[i] > 0` 的位置对应的数字 `i+1` 就是缺失的。

---

### 解法3: 原地标记（加 n）— O(n) / O(1) 变体

**另一种标记思路**: 不改变正负号，而是每次加 n。第二遍检查 `nums[i] <= n` 就是缺失的。

```cpp
class Solution {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        int n = nums.size();
        
        // 第一遍：出现的数字对应位置加 n
        for (int i = 0; i < n; i++) {
            int idx = (nums[i] - 1) % n;  // 用 %n 还原可能已加过的值
            nums[idx] += n;
        }
        
        // 第二遍：值 <= n 的位置说明对应数字未出现
        vector<int> result;
        for (int i = 0; i < n; i++) {
            if (nums[i] <= n) {
                result.push_back(i + 1);
            }
        }
        
        return result;
    }
};
```

---

### 解法4: 交换归位 — O(n) / O(1) 变体

**思路**: 把每个数字 x 放到它"应该在"的位置 `nums[x-1]`。最后 `nums[i] != i+1` 的位置就是缺失的。

```cpp
class Solution {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        int n = nums.size();
        
        // 把每个数放到它该在的位置
        for (int i = 0; i < n; i++) {
            while (nums[i] != nums[nums[i] - 1]) {
                swap(nums[i], nums[nums[i] - 1]);
            }
        }
        
        // 收集不在正确位置的索引
        vector<int> result;
        for (int i = 0; i < n; i++) {
            if (nums[i] != i + 1) {
                result.push_back(i + 1);
            }
        }
        
        return result;
    }
};
```

**关键点**: while 循环条件是 `nums[i] != nums[nums[i]-1]`（而非 `nums[i] != i+1`），避免重复数字导致死循环。

## 解法对比

| | HashSet | 取负标记 | 加 n 标记 | 交换归位 |
|---|---|---|---|---|
| 时间 | O(n) | O(n) | O(n) | O(n) |
| 空间 | O(n) | O(1) | O(1) | O(1) |
| 修改原数组 | 否 | 是（可恢复） | 是 | 是 |
| 代码简洁度 | 最简 | 简洁 | 中等 | 中等 |
| 面试推荐 | 起步 | ⭐ 首选 | 加分 | 加分 |

**面试建议**: 先快速说 HashSet 方案（展示理解），然后说"Follow-up 要求 O(1) 空间，我用原地标记"，写出取负方案。如果面试官追问变体，再补充加 n 或交换归位。

## 易错点

1. **忘记 abs 导致索引为负**
   - ✗ `int idx = nums[i] - 1;` → nums[i] 可能已被标记为负，索引变成负数
   - ✓ `int idx = abs(nums[i]) - 1;` → abs 还原原始数值再计算索引

2. **重复取负导致标记失效**
   - ✗ `nums[idx] = -nums[idx];` 不检查已经是负数的情况 → 重复取负变回正数
   - ✓ `if (nums[idx] > 0) nums[idx] = -nums[idx];` 或使用 `-abs(nums[idx])`

3. **加 n 方案中忘记 %n**
   - ✗ `int idx = nums[i] - 1;` → nums[i] 可能已经被加过 n 了
   - ✓ `int idx = (nums[i] - 1) % n;` → %n 还原原始值

4. **交换归位方案的死循环**
   - ✗ `while (nums[i] != i + 1)` → 重复数字永远不在"正确位置"，死循环
   - ✓ `while (nums[i] != nums[nums[i] - 1])` → 目标位置已有相同值就停

5. **索引偏移搞错**
   - ✗ `result.push_back(i)` → 忘了+1，索引从0开始但数字从1开始
   - ✓ `result.push_back(i + 1)` → 索引 i 对应数字 i+1

## 面试追问

**Q1: 为什么选"取负"而不是"置零"或"改为其他值"？**
→ 取负的好处是可逆的：abs 能恢复原值。置零会丢失原始信息，导致后续元素无法正确映射。

**Q2: 如果数组只读，不能修改呢？**
→ 只能用 HashSet，O(n) 额外空间。或者用"排序 + 扫描"但时间变 O(n log n)。

**Q3: 本题与 442（找重复数字）的关系？**
→ 完全相同的"取负标记"技巧。区别在收集条件：448 收集正数位置（未被标记 = 缺失），442 收集第二次标记时已经为负的位置（= 重复）。

**Q4: 如果要求找缺失的同时找到重复的呢？**
→ 一次遍历同时完成。标记时如果发现 `nums[idx]` 已经为负 → idx+1 是重复的。第二遍正数位置 → 缺失的。

**Q5: 这个技巧能推广到什么范围？**
→ 当 nums[i] 在 [1, n] 或 [0, n-1] 范围内时，都可以用"原地哈希"（取负/加 n/交换归位）。经典应用包括 LC 41（缺失的第一个正数）、LC 268（缺失数字）、LC 287（寻找重复数）。

## 相关题型

- [442. 数组中重复的数据](../0442_find_all_duplicates_in_an_array/) — 同样的取负标记技巧，区别：442 收集"第二次标记时已为负"的数（重复），448 收集"仍为正"的位置（缺失）。
- [41. 缺失的第一个正数](../0041_first_missing_positive/) — 升级版，数组范围不再限定在 [1,n]，需要先把值域外的数处理掉，再用交换归位。Hard 难度。
- [268. 丢失的数字](../0268_missing_number/) — 简化版，n 个数中缺少一个，可以用异或或求和。
- [287. 寻找重复数](../0287_find_the_duplicate_number/) — 不能修改数组的约束下，用快慢指针（Floyd 环检测）。
- [645. 错误的集合](../0645_set_mismatch/) — 找一个缺失 + 一个重复，是 442+448 的组合。
