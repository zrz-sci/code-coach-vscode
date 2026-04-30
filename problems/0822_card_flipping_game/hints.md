# 822. Card Flipping Game

## 核心思路

一个数字不可能成为"good"的**唯一情况**是：某张卡片正反面相同。因为无论怎么翻，该数字必然朝上。排除这些数字后，在所有正反面数字中找最小值即可。本质是**排除不可能 + 取最小值**。

## 思维链

1. **识别不可行数字**：遍历所有卡片，若 `fronts[i] == backs[i]`，则该数字永远无法被隐藏（翻不翻都朝上），加入"禁止集合"
2. **理解翻转操作**：任意卡片可以翻转，翻转后正反互换。某个数字只要不在禁止集合中，就一定可以通过翻转让它仅出现在背面
3. **候选数字收集**：遍历所有 fronts[i] 和 backs[i]，只要不在禁止集合中，都是候选
4. **取最小值**：在所有候选数字中取最小值
5. **处理边界**：若无候选数字，返回 0

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| HashSet + 一次遍历 ⭐ | O(N) | O(N) | 最优解，思路清晰 |
| 暴力枚举 | O(N^2) | O(1) | 逐个数字验证是否可行 |

## 关键提示

- **关键洞察**：只有正反面相同的卡片上的数字是绝对不可能的。其他数字总可以通过翻转操作变成"good"
- 翻转是全局操作：可以翻转任意数量的卡片来配合
- 禁止集合只包含正反面相同的卡片上的数字
- 候选数字来源于所有 fronts 和 backs 中不在禁止集合里的值

## 解法详解

### 解法一：HashSet 排除法（推荐）

```cpp
class Solution {
public:
    int flipgame(vector<int>& fronts, vector<int>& backs) {
        int n = fronts.size();
        unordered_set<int> forbidden; // 正反面相同的数字
        
        // Step 1: 找出所有正反面相同的数字
        for (int i = 0; i < n; i++) {
            if (fronts[i] == backs[i]) {
                forbidden.insert(fronts[i]);
            }
        }
        
        // Step 2: 在所有数字中找不在禁止集合中的最小值
        int result = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (forbidden.find(fronts[i]) == forbidden.end()) {
                result = min(result, fronts[i]);
            }
            if (forbidden.find(backs[i]) == forbidden.end()) {
                result = min(result, backs[i]);
            }
        }
        
        return result == INT_MAX ? 0 : result;
    }
};
```

### 解法二：暴力枚举验证

```cpp
class Solution {
public:
    int flipgame(vector<int>& fronts, vector<int>& backs) {
        int n = fronts.size();
        int result = INT_MAX;
        
        // 枚举所有可能的候选值
        for (int i = 0; i < n; i++) {
            // 检查 fronts[i] 是否可以成为 good
            if (canBeGood(fronts[i], fronts, backs)) {
                result = min(result, fronts[i]);
            }
            // 检查 backs[i] 是否可以成为 good
            if (canBeGood(backs[i], fronts, backs)) {
                result = min(result, backs[i]);
            }
        }
        return result == INT_MAX ? 0 : result;
    }
    
private:
    bool canBeGood(int val, vector<int>& fronts, vector<int>& backs) {
        // 检查是否存在某张卡片正反面都是 val（无法隐藏）
        for (int i = 0; i < (int)fronts.size(); i++) {
            if (fronts[i] == val && backs[i] == val) {
                return false;
            }
        }
        return true;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 只检查 fronts 中的候选 | ✓ fronts 和 backs 都要检查 | backs 中也可能有最小候选值 |
| ✗ 把所有出现在 fronts 中的数字都排除 | ✓ 只排除正反面相同的卡片上的数字 | 出现在某张卡片正面的数字可以通过翻转该卡片移到背面 |
| ✗ `forbidden.count(fronts[i])` 后忘记检查 backs[i] | ✓ 两者都要独立检查 | 每个数字独立判断 |
| ✗ 返回 -1 表示无解 | ✓ 返回 0 表示无解 | 题目要求返回 0 |

## 面试追问

**Q1: 为什么正反面相同的卡片上的数字一定不能成为 good？**
> 因为无论翻不翻这张卡片，该数字都会出现在正面（翻转只是正反互换，但值相同）。题目要求 good 数字不能出现在任何正面上。

**Q2: 如果可以移除卡片（而不是翻转），问题如何变化？**
> 移除卡片意味着可以完全消除某个数字的正面出现。此时只需要找所有 backs 中的最小值（不在所有未被移除卡片的 fronts 中），变成一个更复杂的集合覆盖问题。

**Q3: 如何证明除了正反面相同的数字外，其他数字都可以通过翻转实现？**
> 对于数字 x，如果没有任何卡片正反面都是 x，则每张含有 x 的卡片要么只在正面有 x（翻转它），要么只在背面有 x（不翻），要么正面是 x 但背面不同（翻转使 x 朝下）。总能让 x 不出现在任何正面。

## 相关题型

- [LC 136] Single Number（集合思维 / 异或去重）
- [LC 349] Intersection of Two Arrays（集合操作）
- [LC 1700] Number of Students Unable to Eat Lunch（模拟 + 排除法）
- [LC 575] Distribute Candies（集合计数）
