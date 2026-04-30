# 382. Linked List Random Node - 链表随机节点

## 核心思路
从一个单链表中等概率随机选取一个节点。核心挑战：链表长度未知时如何保证等概率？答案是**蓄水池抽样（Reservoir Sampling）**。

## 思维链
1. 链表不支持随机访问 → 不能直接用下标
2. 如果允许额外空间 → 可以先遍历存到数组
3. 如果不允许额外空间且长度未知 → 蓄水池抽样
4. 遍历第 i 个节点时，以 1/i 概率替换当前选中值
5. 数学归纳法可证明每个节点被选中的概率均为 1/n

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 蓄水池抽样 | init O(1), get O(n) | O(1) | 经典概率算法，无需额外空间 |
| 数组缓存 | init O(n), get O(1) | O(n) | 简单直接，查询快 |

## 关键提示
1. 蓄水池抽样：遍历到第 i 个元素时，以 `1/i` 的概率选择它替换当前结果
2. `rand() % i == 0` 的概率恰好是 `1/i`
3. 数学证明：第 k 个节点最终被保留的概率 = (1/k) * (k/(k+1)) * ((k+1)/(k+2)) * ... * ((n-1)/n) = 1/n

## 解法详解

### 解法一：蓄水池抽样（Reservoir Sampling）
```cpp
class Solution {
    ListNode* head;
public:
    Solution(ListNode* head) : head(head) {}
    
    int getRandom() {
        int result = head->val;
        ListNode* cur = head->next;
        int i = 2;
        while (cur) {
            if (rand() % i == 0) {
                result = cur->val;
            }
            i++;
            cur = cur->next;
        }
        return result;
    }
};
```

### 解法二：数组缓存
```cpp
class Solution {
    vector<int> vals;
public:
    Solution(ListNode* head) {
        while (head) {
            vals.push_back(head->val);
            head = head->next;
        }
    }
    
    int getRandom() {
        return vals[rand() % vals.size()];
    }
};
```

## 易错点
| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `rand() % n` (n固定) | ✓ `rand() % i` (i递增) | 蓄水池抽样的 i 必须是当前遍历到的序号 |
| ✗ i 从 1 开始，第一个也随机 | ✓ i 从 2 开始，第一个默认选中 | 第一个元素必须先选中作为初始结果 |
| ✗ 使用 `rand()` 不加 `srand` | ✓ 生产代码用 `mt19937` | `rand()` 在面试中可接受，但注意随机性质量 |

## 面试追问

**Q1: 如果要从链表中随机选 k 个节点怎么办？**
> 蓄水池抽样推广版（Reservoir Sampling size-k）：维护大小为 k 的数组，遍历到第 i 个元素时以 k/i 的概率替换数组中随机一个位置。

**Q2: 能否证明蓄水池抽样的正确性？**
> 数学归纳法：假设前 n-1 个元素各以 1/(n-1) 概率被选中。第 n 个元素以 1/n 概率被选中，前面每个元素保留概率为 1/(n-1) * (1 - 1/n) = 1/n。

**Q3: 如果链表会动态变化（增删节点），蓄水池抽样还能用吗？**
> 蓄水池抽样每次调用都重新遍历，天然支持动态变化。但如果要高效处理，可以结合跳表或平衡树实现 O(log n) 随机访问。

## 相关题型
- [398. Random Pick Index](https://leetcode.com/problems/random-pick-index/) - 蓄水池抽样变体
- [384. Shuffle an Array](https://leetcode.com/problems/shuffle-an-array/) - Fisher-Yates 洗牌
- [528. Random Pick with Weight](https://leetcode.com/problems/random-pick-with-weight/) - 带权随机
