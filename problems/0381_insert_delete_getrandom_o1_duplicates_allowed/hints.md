# 381. Insert Delete GetRandom O(1) - Duplicates allowed - O(1) 时间插入、删除和获取随机元素（允许重复）

## 核心思路
用 **数组 + 哈希表(值→索引集合)** 实现多重集。插入追加末尾，删除时将要删的元素与末尾交换后 pop_back，getRandom 随机取数组下标。重复值用 unordered_set 存储所有索引。

## 思维链
1. getRandom O(1) 且等概率 → 必须用连续数组，随机下标
2. insert O(1) → 直接 push_back 到数组末尾，更新哈希表
3. remove O(1) → 不能直接删中间元素（O(n)），而是交换到末尾再 pop_back
4. 允许重复 → 哈希表 value 不是单个 index，而是 index 的集合（unordered_set）
5. 删除时交换的细节：更新被交换元素在哈希表中的索引
6. 边界：删除的元素恰好在末尾时要小心处理顺序

## 解法概览表 ⭐

| 解法 | insert | remove | getRandom | 空间 | 特点 |
|------|--------|--------|-----------|------|------|
| Array + HashMap<int, set<int>> | O(1) | O(1) 均摊 | O(1) | O(n) | 标准最优解 |

## 关键提示
1. **数组保证 getRandom O(1)**：`nums[rand() % nums.size()]`
2. **哈希表存索引集合**：`unordered_map<int, unordered_set<int>>`
3. **删除=交换末尾**：将目标位置的值替换为末尾值，pop_back 末尾
4. **更新索引顺序很重要**：先加后删，处理 val == lastVal 的边界
5. **insert 返回值**：第一次插入返回 true，已存在返回 false

## 解法详解

### 解法一：Array + HashMap（唯一解法）⭐
**思路**：核心是 swap-to-back 删除技巧，配合哈希表维护每个值的所有索引。

```cpp
class RandomizedCollection {
    vector<int> nums;
    unordered_map<int, unordered_set<int>> idx; // val -> set of indices
    
public:
    RandomizedCollection() {}
    
    bool insert(int val) {
        bool notPresent = idx[val].empty();
        nums.push_back(val);
        idx[val].insert(nums.size() - 1);
        return notPresent;
    }
    
    bool remove(int val) {
        if (idx[val].empty()) return false;
        
        // 取 val 的一个索引
        int removeIdx = *idx[val].begin();
        int lastVal = nums.back();
        
        // 将末尾元素移到 removeIdx 位置
        nums[removeIdx] = lastVal;
        idx[val].erase(removeIdx);
        idx[lastVal].insert(removeIdx);
        
        // 删除末尾
        idx[lastVal].erase(nums.size() - 1);
        nums.pop_back();
        
        return true;
    }
    
    int getRandom() {
        return nums[rand() % nums.size()];
    }
};
```

### 删除操作详细图解

假设 `nums = [1, 1, 2, 3]`，`idx = {1:{0,1}, 2:{2}, 3:{3}}`，删除 val=1：

```
Step 1: removeIdx = 0 (取 val=1 的一个索引)
Step 2: lastVal = 3 (末尾元素)
Step 3: nums[0] = 3    → nums = [3, 1, 2, 3]
Step 4: idx[1].erase(0) → idx[1] = {1}
Step 5: idx[3].insert(0) → idx[3] = {0, 3}
Step 6: idx[3].erase(3)  → idx[3] = {0}
Step 7: nums.pop_back()  → nums = [3, 1, 2]
结果: nums = [3, 1, 2], idx = {1:{1}, 2:{2}, 3:{0}}
```

## 易错点
- ✗ 删除时先处理末尾再处理 removeIdx，当 val==lastVal 时逻辑错误 → ✓ 先 insert(removeIdx) 再 erase(size-1)，即使 removeIdx == size-1 也正确
- ✗ 用 unordered_map<int, vector<int>> 存索引，删除时 O(n) 查找 → ✓ 用 unordered_set 保证 O(1) 删除
- ✗ remove 后忘记 pop_back 数组 → ✓ 交换后必须 pop_back 保持数组紧凑
- ✗ insert 返回值逻辑搞反 → ✓ 集合为空（首次出现）返回 true
- ✗ getRandom 使用 `rand() % 0` 当数组为空时 → ✓ 题目保证至少有一个元素

## 面试追问

**Q1: 为什么删除操作中索引更新的顺序很重要？**
A: 考虑删除末尾元素自身的情况（removeIdx == nums.size()-1）。如果先 erase(size-1) 再 insert(removeIdx)，当两者相等时会把已删的索引又加回去。正确顺序是先 insert(removeIdx) 再 erase(size-1)，因为 set 的 insert 是幂等的，而最后 erase(size-1) 总是正确的。

**Q2: 与 380 题（不允许重复）的区别是什么？**
A: 380 题 map 存 `int→int`（值→唯一索引），381 题存 `int→set<int>`（值→索引集合）。删除逻辑框架相同（swap-to-back），但索引维护更复杂。insert 返回值语义也不同：380 返回是否已存在，381 每次都插入但返回是否首次出现。

**Q3: 如何保证 getRandom 的等概率性？**
A: 数组中每个元素占一个位置，重复值占多个位置。`rand() % nums.size()` 等概率选中每个位置，所以值被选中的概率正比于其出现次数。这正是题目要求的"线性相关"概率。

## 相关题型
- [380. Insert Delete GetRandom O(1)](../0380_insert_delete_getrandom_o1/) - 不允许重复的版本
- [379. Design Phone Directory](../0379_design_phone_directory/) - 类似的 O(1) 设计
- [710. Random Pick with Blacklist](../0710_random_pick_with_blacklist/) - 带黑名单的随机选取
- [528. Random Pick with Weight](../0528_random_pick_with_weight/) - 带权重的随机选取
