# 380. O(1) 时间插入、删除和获取随机元素

## 核心思路

这道题本质在问：**如何设计一个数据结构，使得插入、删除、随机访问三个操作都达到平均 O(1)**。没有任何单一的标准数据结构能同时满足这三个需求，必须组合使用。

## 思维链

1. **读完题第一反应**：插入和删除 O(1)？哈希表（`unordered_set`）可以做到。但哈希表能 O(1) 随机取一个元素吗？不能！哈希表的元素分布在不同的桶中，无法通过随机索引直接访问。

2. **随机访问 O(1) 需要什么**？数组！`rand() % size` 就能等概率取到任意元素。但数组的删除是 O(n)（要移动元素填补空位）。

3. **瓶颈在哪**？数组删除 O(n) 的原因是要保持元素的连续性。但我们真的需要保持顺序吗？**不需要！** 随机访问只要求元素连续存储，不要求有序。

4. **突破关键**：如果把要删除的元素和数组最后一个元素交换，然后 `pop_back()`，删除就是 O(1)！但交换需要知道目标元素在数组的哪个位置——这正是哈希表擅长的。

5. **最终设计**：**数组 + 哈希表**。数组存元素（支持随机访问），哈希表存 `{值 → 数组中的下标}`（支持 O(1) 定位元素位置）。

6. **删除的精妙操作**：把待删除元素与末尾元素交换 → 更新末尾元素在哈希表中的下标 → 删除末尾 → 删除哈希表条目。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 数组 + 哈希表 | 数组支持随机访问，哈希表支持 O(1) 查找定位 | O(1) 均摊 | O(n) | ⭐ 必须写出 |

> 这道题本质只有一种核心解法（数组+哈希表），但实现细节是面试重点。

## 关键提示

1. **哈希表能 O(1) 插入删除，但不能 O(1) 随机取元素**（元素散列在桶中，不连续）。

2. **数组能 O(1) 随机取元素，但不能 O(1) 任意位置删除**（要移动元素填空）。

3. **关键洞察：我们不需要维护元素的顺序！** 如果不在乎顺序，数组的删除可以通过"与末尾交换 + pop_back"实现 O(1)。

4. **交换后别忘了更新哈希表**：末尾元素被交换到了新位置，它在哈希表中的索引也要更新。

5. **删除操作的细节顺序很重要**：先做交换和更新，再做删除，避免特殊情况（如删除的恰好是末尾元素）出问题。

```
// 数据结构示意:
//
// 数组 nums:  [3, 7, 1, 9]     ← 随机访问: rand() % 4
//              0  1  2  3
//
// 哈希表 map: {3→0, 7→1, 1→2, 9→3}  ← O(1) 查找位置
//
// 删除元素 7 (在索引1):
//   Step1: 把末尾元素 9 放到索引1 → nums = [3, 9, 1, 9]
//   Step2: 更新哈希表 map[9] = 1
//   Step3: pop_back → nums = [3, 9, 1]
//   Step4: 删除 map[7]
//   最终: nums = [3, 9, 1], map = {3→0, 9→1, 1→2}
```

## 解法详解

### 解法1: 数组 + 哈希表 — O(1) / O(n) ⭐ 面试首选

**思考过程**：

我们需要三个操作都是 O(1)：
- `insert`：检查是否存在 → 哈希表 O(1)；添加元素 → 数组尾部 push_back O(1) + 哈希表记录下标 O(1)
- `remove`：检查是否存在 → 哈希表 O(1)；从数组删除 → **交换到末尾再 pop_back** O(1) + 更新哈希表 O(1)
- `getRandom`：在数组上随机取下标 → O(1)

```cpp
class RandomizedSet {
public:
    RandomizedSet() {}
    
    bool insert(int val) {
        // 已存在，返回 false
        if (valToIndex.count(val)) return false;
        
        // 追加到数组末尾
        nums.push_back(val);
        // 记录 val 在数组中的下标
        valToIndex[val] = nums.size() - 1;
        return true;
    }
    
    bool remove(int val) {
        // 不存在，返回 false
        if (!valToIndex.count(val)) return false;
        
        // 找到 val 的下标和末尾元素
        int idx = valToIndex[val];
        int lastVal = nums.back();
        
        // 把末尾元素放到 idx 位置（覆盖要删除的元素）
        nums[idx] = lastVal;
        valToIndex[lastVal] = idx;
        
        // 删除末尾元素
        nums.pop_back();
        valToIndex.erase(val);
        
        return true;
    }
    
    int getRandom() {
        // 等概率随机取一个下标
        int randomIdx = rand() % nums.size();
        return nums[randomIdx];
    }
    
private:
    vector<int> nums;                    // 存储实际元素，支持随机访问
    unordered_map<int, int> valToIndex;  // 值 → 数组下标，支持 O(1) 定位
};
```

**关键点**：
- 删除操作的 "swap with last + pop_back" 是这道题的精髓
- 即使 val 恰好是末尾元素，上面的代码也能正确处理（`nums[idx] = lastVal` 相当于自己赋值给自己，`valToIndex[lastVal] = idx` 也不会出错，然后正常 pop 和 erase）

## 解法对比

这道题核心只有一种解法思路，但有一些变体考虑：

| 对比项 | 数组+哈希表 | 纯哈希表 (unordered_set) |
|--------|-----------|----------------------|
| insert | O(1) ✓ | O(1) ✓ |
| remove | O(1) ✓ | O(1) ✓ |
| getRandom | O(1) ✓ | **O(n) ✗** — 需遍历到随机位置 |
| 结论 | **正解** | 不满足题目要求 |

为什么 `unordered_set` 不能 O(1) getRandom？因为哈希表的迭代器不支持随机访问，`std::advance(it, k)` 是 O(k) 的。

## 易错点

1. **删除时忘记更新被交换元素的哈希表映射**：
   - ✗ 只做 `nums[idx] = lastVal; nums.pop_back();` — 此时 `valToIndex[lastVal]` 还指向旧的末尾位置，后续操作会出错
   - ✓ 必须加 `valToIndex[lastVal] = idx;`

2. **删除时先 erase 哈希表再做交换**：
   - ✗ `valToIndex.erase(val); nums[idx] = lastVal; valToIndex[lastVal] = idx;`
   - 当 `val == lastVal`（删除末尾元素）时，先 erase 了 val，然后 `valToIndex[lastVal] = idx` 又插回去了，导致删不掉！
   - ✓ 先做交换和更新，**最后** erase val

3. **getRandom 使用 `rand()` 的均匀性**：
   - `rand() % n` 在 n 不整除 `RAND_MAX+1` 时有微小偏差，但题目只要求平均 O(1) 且 LeetCode 不会卡这个
   - 如果面试官追问，可以提到用 `uniform_int_distribution`

4. **删除时先 pop_back 再访问 lastVal**：
   - ✗ `int lastVal = nums.back(); nums.pop_back(); nums[idx] = lastVal;` — 如果 idx == nums.size()（删的是末尾），pop_back 后 idx 越界
   - ✓ 先做所有赋值操作，**最后** pop_back

## 面试追问

**Q1（基础理解）：为什么不能只用一个 `unordered_set`？**
> 哈希表不支持按索引随机访问。要随机取元素，需要先转成数组或遍历到随机位置，这是 O(n) 的。

**Q2（设计细节）：删除操作为什么要和末尾交换？直接删除数组中间元素不行吗？**
> 直接删除中间元素需要把后面的元素前移，是 O(n)。和末尾交换后 pop_back 是 O(1)，代价是牺牲了元素顺序——但我们不需要保持顺序。

**Q3（进阶变体）：如果要支持允许重复元素的 `insert`/`remove`/`getRandom`，怎么改？**
> 这就是 [LeetCode 381](https://leetcode.com/problems/insert-delete-getrandom-o1-duplicates-allowed/)。哈希表的 value 从单个 index 改成 `unordered_set<int>` 存储所有下标。删除时从集合中取一个下标做交换，注意更新逻辑更复杂。

**Q4（随机性追问）：`rand() % n` 是等概率的吗？如何做到严格等概率？**
> 不严格等概率（模偏差问题）。可以用 C++11 的 `<random>` 库：`uniform_int_distribution<int> dist(0, n-1); dist(gen);`

## 相关题型

- **381. O(1) 时间插入、删除和获取随机元素 - 允许重复** — 直接扩展本题，哈希表 value 从 `int` 改成 `unordered_set<int>` 存多个下标，删除逻辑更复杂
- **710. 黑名单中的随机数** — 复用"交换到末尾"的思想，把黑名单映射到尾部区域外
- **384. 打乱数组** — 同样是随机化设计题，考察 Fisher-Yates 洗牌算法
- **146. LRU 缓存** — 同样是组合数据结构的设计题（HashMap + 双向链表），复用"用哈希表加速定位"的核心思想