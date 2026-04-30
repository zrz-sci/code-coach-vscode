# 379. Design Phone Directory - 电话目录管理系统

## 核心思路
设计一个号码池管理系统，支持 O(1) 分配、查询和释放号码。核心数据结构：HashSet（快速查询可用性）+ Queue/LinkedList（快速获取可用号码）。

## 思维链
1. 需要三个操作：get() 获取可用号码、check() 检查号码是否可用、release() 释放号码
2. get() 需要快速取一个可用号码 → 队列/集合
3. check() 需要 O(1) 判断某号码是否可用 → HashSet
4. release() 需要 O(1) 将号码加回可用池 → 同时更新两个数据结构
5. 方案：用 queue 存可用号码（保证 get O(1)），用 set 标记已使用的号码（保证 check O(1)）
6. 或者：只用一个 HashSet 存可用号码，get 时取 begin() 迭代器

## 解法概览表 ⭐

| 解法 | get() | check() | release() | 空间 | 特点 |
|------|-------|---------|-----------|------|------|
| Queue + HashSet | O(1) | O(1) | O(1) | O(n) | 最优解 |
| 纯 HashSet | O(1) 均摊 | O(1) | O(1) | O(n) | 实现更简单 |
| Bitset | O(n) 最坏 | O(1) | O(1) | O(n/8) | 空间最优 |

## 关键提示
1. **Queue 保证 get O(1)**：初始化时把 0~maxNumbers-1 都入队
2. **HashSet 保证 check O(1)**：存已使用或可用的号码集合
3. **release 要防重复释放**：已经可用的号码不应重复入队
4. **get 要处理空队列**：没有可用号码时返回 -1

## 解法详解

### 解法一：Queue + HashSet（推荐）⭐
**思路**：queue 维护可用号码，set 记录已使用号码。

```cpp
class PhoneDirectory {
    queue<int> available;
    unordered_set<int> used;
    int maxNum;
    
public:
    PhoneDirectory(int maxNumbers) : maxNum(maxNumbers) {
        for (int i = 0; i < maxNumbers; i++) {
            available.push(i);
        }
    }
    
    int get() {
        if (available.empty()) return -1;
        int num = available.front();
        available.pop();
        used.insert(num);
        return num;
    }
    
    bool check(int number) {
        return used.find(number) == used.end();
    }
    
    void release(int number) {
        if (used.count(number)) {
            used.erase(number);
            available.push(number);
        }
    }
};
```

### 解法二：纯 HashSet
**思路**：只用一个 set 存可用号码。

```cpp
class PhoneDirectory {
    unordered_set<int> available;
    
public:
    PhoneDirectory(int maxNumbers) {
        for (int i = 0; i < maxNumbers; i++) {
            available.insert(i);
        }
    }
    
    int get() {
        if (available.empty()) return -1;
        int num = *available.begin();
        available.erase(available.begin());
        return num;
    }
    
    bool check(int number) {
        return available.count(number);
    }
    
    void release(int number) {
        available.insert(number); // set 自动去重
    }
};
```

### 解法三：数组 + 交换技巧
**思路**：用数组存可用号码，维护可用区间 [0, idx)，get 时取尾部并缩小范围。

```cpp
class PhoneDirectory {
    vector<int> nums;       // 可用号码数组
    vector<int> pos;        // 号码到数组位置的映射
    int idx;                // 可用号码数量
    
public:
    PhoneDirectory(int maxNumbers) : nums(maxNumbers), pos(maxNumbers), idx(maxNumbers) {
        for (int i = 0; i < maxNumbers; i++) {
            nums[i] = pos[i] = i;
        }
    }
    
    int get() {
        if (idx == 0) return -1;
        int num = nums[--idx];
        return num;
    }
    
    bool check(int number) {
        return pos[number] < idx;
    }
    
    void release(int number) {
        if (pos[number] >= idx) {
            // 将 number 交换到可用区域
            swap(nums[pos[number]], nums[idx]);
            swap(pos[nums[pos[number]]], pos[nums[idx]]);
            idx++;
        }
    }
};
```

## 易错点
- ✗ release 不检查号码是否已释放，重复入队 → ✓ 检查 used.count() 再决定是否入队
- ✗ get 不检查队列为空就取元素 → ✓ 先判断 available.empty()
- ✗ check 的正反逻辑搞反（available vs used）→ ✓ check 返回 true 表示可用（未被使用）
- ✗ number 参数没有范围检查 → ✓ 题目保证 0 <= number < maxNumbers

## 面试追问

**Q1: 如果 maxNumbers 非常大（如 10^9），初始化 O(n) 不可接受怎么办？**
A: 改用惰性初始化。维护一个计数器 next 表示下一个可分配的号码，get 优先从回收池取，没有再用 next++。这样初始化 O(1)。

**Q2: 如何保证线程安全？**
A: 给 get/check/release 加互斥锁。或者用 CAS 操作实现无锁版本。get 和 release 的原子性最关键。

**Q3: 对比 Queue+Set 和纯 Set 方案的优劣？**
A: Queue+Set 的 get() 严格 O(1)；纯 Set 的 get() 取 begin() 在 unordered_set 上也是 O(1) 均摊但常数可能更大。Queue+Set 需要两个数据结构同步维护，纯 Set 更简洁但 get 的确定性略差。

## 相关题型
- [380. Insert Delete GetRandom O(1)](../0380_insert_delete_getrandom_o1/) - 类似的 O(1) 设计
- [381. Insert Delete GetRandom O(1) - Duplicates allowed](../0381_insert_delete_getrandom_o1_duplicates_allowed/) - 允许重复的 O(1) 设计
- [355. Design Twitter](../0355_design_twitter/) - 综合数据结构设计
