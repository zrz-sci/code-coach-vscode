# 170. 两数之和 III - 数据结构设计

## 核心思路
设计一个数据结构，在动态添加元素的同时支持高效查询是否存在两数之和等于目标值——本质是在 add/find 操作频率的权衡中选择最优的数据结构。

## 思维链
1. **暴力思路**: 每次 find 时双重循环遍历所有元素对，O(n^2) 查找，O(1) 添加
2. **排序+双指针**: 维护有序数组，find 时用双指针 O(n)，但 add 需要插入排序 O(n)
3. **哈希表优化**: 用 HashMap 记录每个数出现次数，find 时遍历 key 查 complement 是否存在，O(n) 查找，O(1) 添加
4. **预计算 vs 懒计算**: 如果 find 远多于 add，可以在 add 时预计算所有可能的 sum 存入 HashSet，find 变 O(1)，但 add 变 O(n)
5. **关键决策**: 根据 add 和 find 的调用频率比选择策略——本题两者都最多 10^4 次，哈希表方案最均衡

## 解法概览
| 解法 | 思路 | 时间(add/find) | 空间 | 面试 |
|------|------|---------------|------|------|
| 哈希表计数 ⭐ | map 存数字出现次数，find 遍历 key | O(1) / O(n) | O(n) | 必知 |
| 预计算所有和 | add 时算出所有可能 sum | O(n) / O(1) | O(n^2) | 了解 |
| 排序+双指针 | 维护有序数组，双指针查找 | O(n) / O(n) | O(n) | 了解 |

## 关键提示
1. **重复元素**: 当 value = 2 * num 时，需要检查该数字是否出现了至少 2 次
2. **整数溢出**: value 范围是 [-2^31, 2^31-1]，complement = value - num 可能溢出，但本题 number 范围小 [-10^5, 10^5] 不会溢出
3. **add 多 vs find 多**: 如果 add 调用远多于 find，用方案1(懒计算)；如果 find 远多于 add，用方案2(预计算)
4. **自身配对**: `[3]` 中 find(6) 应返回 false，因为只有一个 3

## 解法详解

### 解法1: 哈希表计数 (推荐)

**思路**: 用 unordered_map 存储每个数字出现的次数。find 时遍历所有 key，对每个 num 检查 complement = value - num 是否存在。

```cpp
class TwoSum {
    unordered_map<int, int> cnt;
public:
    TwoSum() {}
    
    void add(int number) {
        cnt[number]++;
    }
    
    bool find(int value) {
        for (auto& [num, freq] : cnt) {
            int comp = value - num;
            if (comp == num) {
                // 需要同一个数出现至少两次
                if (freq >= 2) return true;
            } else {
                if (cnt.count(comp)) return true;
            }
        }
        return false;
    }
};
```

**思考过程**: 
- 为什么用 map 而不是 set？因为需要处理 `value = 2 * num` 的情况，需要知道同一个数是否出现了多次
- 遍历 map 的 key 而不是原始数组，避免重复检查

**复杂度**: add O(1), find O(n), 空间 O(n)

### 解法2: 预计算所有和

**思路**: 在 add 时就把新数字与已有数字的所有和存入 HashSet，find 时直接查询 O(1)。

```cpp
class TwoSum {
    vector<int> nums;
    unordered_set<int> sums;
public:
    TwoSum() {}
    
    void add(int number) {
        for (int num : nums) {
            sums.insert(num + number);
        }
        nums.push_back(number);
    }
    
    bool find(int value) {
        return sums.count(value);
    }
};
```

**思考过程**:
- 适用于 find 调用频率远高于 add 的场景
- 代价是 add 从 O(1) 变成 O(n)，且空间可能到 O(n^2)

**复杂度**: add O(n), find O(1), 空间 O(n^2)

## 易错点
- ✗ `if (cnt.count(value - num))` 没有处理 num == value - num 的情况
- ✓ 当 complement 等于自身时，必须检查 `freq >= 2`
- ✗ 用 set 存储数字，丢失出现次数信息
- ✓ 用 map 存储数字到出现次数的映射

## 面试追问
- **Q1**: add 多还是 find 多时如何选择？
  - A: add 多用懒计算(哈希表)，find 多用预计算(HashSet 存所有和)
- **Q2**: 如果需要返回具体的 index pair 而不是 bool？
  - A: map 存 value -> list of indices，find 时返回两个 index
- **Q3**: 如果数据流是多线程环境？
  - A: 需要加锁或使用 concurrent hash map，注意 find 遍历期间 add 的线程安全

## 相关题型
- [1. Two Sum](../0001_two_sum/) - 静态数组版本，用哈希表一次遍历
- [167. Two Sum II](../0167_two_sum_ii_input_array_is_sorted/) - 有序数组版本，双指针
- [653. Two Sum IV - Input is a BST](../0653_two_sum_iv_input_is_a_bst/) - BST 上找两数之和，中序遍历 + 双指针
