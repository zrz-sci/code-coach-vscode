# 384. Shuffle an Array - 打乱数组

## 核心思路
设计一个能将数组随机打乱的算法，使所有排列等概率出现。核心是 **Fisher-Yates 洗牌算法**：从后往前（或从前往后），每个位置与一个随机位置交换。

## 思维链
1. 需要所有 n! 种排列等概率 → 不能简单随机交换
2. 错误做法：每个位置随机选 [0, n-1] 交换 → n^n 种可能，不能整除 n!
3. 正确做法：位置 i 从 [i, n-1] 中随机选一个交换（Fisher-Yates）
4. 这样恰好产生 n * (n-1) * ... * 1 = n! 种等概率结果
5. reset 只需保存原数组的副本

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ Fisher-Yates 洗牌 | O(n) | O(n) | 标准算法，等概率保证 |
| 暴力随机（错误） | O(n) | O(n) | 不等概率，面试会被追问 |

## 关键提示
1. Fisher-Yates 核心：`for i from n-1 to 1: swap(arr[i], arr[rand(0..i)])`
2. 必须保存原数组的**深拷贝**用于 reset
3. 随机范围是 `[i, n-1]` 或 `[0, i]`，不能是 `[0, n-1]`
4. 每次 shuffle 应该在副本上操作，不影响原数组

## 解法详解

### 解法一：Fisher-Yates 洗牌（从后往前）
```cpp
class Solution {
    vector<int> original;
    vector<int> arr;
public:
    Solution(vector<int>& nums) : original(nums), arr(nums) {}
    
    vector<int> reset() {
        arr = original;
        return arr;
    }
    
    vector<int> shuffle() {
        for (int i = arr.size() - 1; i > 0; i--) {
            int j = rand() % (i + 1);  // [0, i]
            swap(arr[i], arr[j]);
        }
        return arr;
    }
};
```

### 解法二：Fisher-Yates 洗牌（从前往后）
```cpp
class Solution {
    vector<int> original;
    vector<int> arr;
public:
    Solution(vector<int>& nums) : original(nums), arr(nums) {}
    
    vector<int> reset() {
        arr = original;
        return arr;
    }
    
    vector<int> shuffle() {
        int n = arr.size();
        for (int i = 0; i < n; i++) {
            int j = i + rand() % (n - i);  // [i, n-1]
            swap(arr[i], arr[j]);
        }
        return arr;
    }
};
```

## 易错点
| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `j = rand() % n` | ✓ `j = rand() % (i + 1)` | 随机范围必须逐步缩小，否则不等概率 |
| ✗ reset 返回 arr（未还原） | ✓ `arr = original; return arr;` | 必须从保存的原始数组恢复 |
| ✗ 构造时 `original = nums`（浅拷贝引用） | ✓ 值拷贝 `original(nums)` | C++ vector 默认深拷贝，但需注意指针/引用场景 |
| ✗ 在 original 上 shuffle | ✓ 在 arr 副本上 shuffle | 不能破坏原始数组 |

## 面试追问

**Q1: 为什么 `rand() % n` 的做法是错误的？**
> 每个位置从 n 个中选，总共 n^n 种选法。n^n 不一定能被 n! 整除（例如 n=3 时 27 不能被 6 整除），所以不是等概率的。

**Q2: Fisher-Yates 算法的时间复杂度能优化吗？**
> O(n) 已经是最优的，因为必须至少访问每个元素一次。如果只需要前 k 个随机元素，可以只做 k 步交换，得到 O(k) 的部分洗牌。

**Q3: 如何验证你的洗牌算法是等概率的？**
> 统计学方法：运行大量次数，统计每种排列出现频率，做卡方检验。理论上每种排列出现概率应为 1/n!。

## 相关题型
- [382. Linked List Random Node](https://leetcode.com/problems/linked-list-random-node/) - 蓄水池抽样
- [398. Random Pick Index](https://leetcode.com/problems/random-pick-index/) - 随机选择
- [528. Random Pick with Weight](https://leetcode.com/problems/random-pick-with-weight/) - 带权随机
