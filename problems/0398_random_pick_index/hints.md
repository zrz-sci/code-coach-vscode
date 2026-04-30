# 398. Random Pick Index - 随机数索引

## 核心思路
给定含重复元素的数组，要求等概率随机返回 target 值的某个索引。核心在于**蓄水池抽样（Reservoir Sampling）**：遍历时遇到第 cnt 个 target，以 1/cnt 概率替换当前选中索引，保证每个索引被选中概率相等。

## 思维链
1. 最直观：用哈希表预存 `val -> [indices]`，pick 时随机选一个 => O(n) 空间
2. 如果数组巨大、无法全部存储呢？=> 蓄水池抽样，O(1) 额外空间
3. 蓄水池的数学证明：第 i 个元素最终被选中概率 = 1/i * (不被后续替换的概率) = 1/n

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 哈希表预处理 | init O(n), pick O(1) | O(n) | 简单直接，面试首选 |
| ⭐ 蓄水池抽样 | init O(1), pick O(n) | O(1) extra | 经典算法，适合流式数据 |

## 关键提示
1. **蓄水池核心**：遇到第 cnt 个目标值，以 `rand() % cnt == 0` 的概率更新结果
2. **哈希表法**：构造函数中一次性建好 `unordered_map<int, vector<int>>`
3. **随机数生成**：C++ 中用 `rand()` 即可，注意取模的均匀性

## 解法详解

### 解法一：哈希表预处理
```cpp
class Solution {
    unordered_map<int, vector<int>> indices;
public:
    Solution(vector<int>& nums) {
        for (int i = 0; i < nums.size(); i++)
            indices[nums[i]].push_back(i);
    }
    int pick(int target) {
        auto& v = indices[target];
        return v[rand() % v.size()];
    }
};
```

### 解法二：蓄水池抽样（Reservoir Sampling）
```cpp
class Solution {
    vector<int> nums;
public:
    Solution(vector<int>& nums) : nums(nums) {}
    int pick(int target) {
        int cnt = 0, res = -1;
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] == target) {
                cnt++;
                if (rand() % cnt == 0) res = i;
            }
        }
        return res;
    }
};
```

## 易错点
| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `rand() % cnt == 1` | `rand() % cnt == 0` | 必须与 0 比较，保证第一个元素一定被选中 |
| 先 `cnt++` 再判断 `rand()%cnt==0` | 顺序正确 ✓ | cnt 必须先自增为当前计数再取模 |
| 忘记存储整个 nums 数组 | 蓄水池法需要保存原数组引用 | pick 每次需重新遍历 |

## 面试追问

**Q1: 蓄水池抽样的数学证明？**
> 第 k 个目标元素最终被选中的概率 = (1/k) * (k/(k+1)) * ((k+1)/(k+2)) * ... * ((n-1)/n) = 1/n。即选中概率 1/k 乘以不被后续每一步替换的概率，连乘后消掉 = 1/n。

**Q2: 如果要随机选 m 个不重复索引怎么办？**
> 推广为 Reservoir Sampling-K：维护大小为 m 的蓄水池，第 i 个元素以 m/i 概率替换池中随机一个。

**Q3: 两种解法如何选择？**
> 如果 pick 调用频繁且内存充足 => 哈希表法（O(1) pick）。如果数据是流式的、内存有限 => 蓄水池抽样。

## 相关题型
- [382. Linked List Random Node](https://leetcode.com/problems/linked-list-random-node/) - 蓄水池抽样经典
- [384. Shuffle an Array](https://leetcode.com/problems/shuffle-an-array/) - Fisher-Yates 洗牌
- [528. Random Pick with Weight](https://leetcode.com/problems/random-pick-with-weight/) - 带权随机
