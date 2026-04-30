# 519. 随机翻转矩阵 (Random Flip Matrix)

**难度**: Medium | **标签**: Reservoir Sampling, Hash Table, Math, Randomized

## 核心思路

将 m*n 矩阵展平为一维数组 [0, total-1]，每次 flip 用 Fisher-Yates 洗牌思想：随机选一个位置，将其与"末尾"交换，然后缩小可选范围。用 HashMap 记录交换映射，避免 O(m*n) 空间。

## 思维链

1. 矩阵扁平化：`(i, j)` ↔ `i * n + j`，问题变成从 [0, total-1] 中不重复随机选
2. 朴素做法：用 set 记录已选 → 可能反复碰撞，效率不稳定
3. Fisher-Yates 思想：将选中的位置与"尾部"交换，缩小范围
   - `rand(0, remain-1)` → 选中 r → 将 map[r] 和 map[remain-1] 交换 → remain--
4. 用 HashMap 只记录被交换过的位置，空间 O(flip次数) 而非 O(m*n)
5. reset 时清空 HashMap，恢复 remain = total

## 解法概览

| 解法 | 时间 | 空间 | 推荐 |
|------|------|------|------|
| Fisher-Yates + HashMap | flip O(1), reset O(k) | O(k) k=flip次数 | ⭐⭐⭐ |
| HashSet 拒绝采样 | flip 期望 O(1) 但最坏 O(k) | O(k) | ⭐ |

## 关键提示

1. `map[r]` 不存在时默认映射到自己（即 r）
2. 交换后 `map[r] = map[remain-1]`（若不存在则 = remain-1）
3. reset 只需清空 map + 重置 remain，不需要 O(m*n)

## 解法详解

### 解法1: Fisher-Yates + HashMap

```cpp
class Solution {
    int rows, cols, remain;
    unordered_map<int, int> mp;
public:
    Solution(int m, int n) : rows(m), cols(n), remain(m * n) {}

    vector<int> flip() {
        int r = rand() % remain;
        // map[r] 存在则用映射值，否则就是 r 本身
        int val = mp.count(r) ? mp[r] : r;
        // 将 r 映射到"末尾"位置的实际值
        remain--;
        mp[r] = mp.count(remain) ? mp[remain] : remain;
        return {val / cols, val % cols};
    }

    void reset() {
        mp.clear();
        remain = rows * cols;
    }
};
```

### 解法2: HashSet 拒绝采样

```cpp
class Solution {
    int rows, cols, total;
    unordered_set<int> used;
public:
    Solution(int m, int n) : rows(m), cols(n), total(m * n) {}

    vector<int> flip() {
        int r;
        do {
            r = rand() % total;
        } while (used.count(r));
        used.insert(r);
        return {r / cols, r % cols};
    }

    void reset() {
        used.clear();
    }
};
```

## 易错点

| 错误 | 正确 |
|------|------|
| ✗ 用 `rand() % total` 而不更新范围 | ✓ 用 `rand() % remain`，每次 remain-- |
| ✗ 交换时忘记检查 `map[remain]` 是否存在 | ✓ `mp.count(remain) ? mp[remain] : remain` |
| ✗ 将坐标转换为 `val / rows, val % rows` | ✓ 应该是 `val / cols, val % cols` |

## 面试追问

- **Q1**: 为什么用 Fisher-Yates 比 HashSet 拒绝采样好？
  → Fisher-Yates 保证 O(1) 最坏复杂度，拒绝采样在接近满时碰撞严重
- **Q2**: 如何保证等概率？
  → Fisher-Yates 洗牌的数学证明：每个位置被选中的概率均为 1/remain
- **Q3**: 如果 m*n 非常大但 flip 次数很少，空间如何？
  → HashMap 只存 O(flip次数) 个映射，远小于 O(m*n)

## 相关题型

- [384. 打乱数组](https://leetcode.com/problems/shuffle-an-array/)
- [382. 链表随机节点](https://leetcode.com/problems/linked-list-random-node/)
- [398. 随机数索引](https://leetcode.com/problems/random-pick-index/)
