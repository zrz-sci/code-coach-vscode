# 666. 路径总和 IV (Path Sum IV)

[Medium] | Tree, Depth-First Search, Array, Hash Table, Binary Tree

## 核心思路

用三位数编码表示二叉树节点：百位=深度，十位=位置，个位=值。用 HashMap 存储 `(depth, pos) -> value` 的映射，然后 DFS 遍历树，累加所有根到叶子的路径和。关键是利用完全二叉树的位置关系找到子节点：深度 d、位置 p 的节点，左子是 `(d+1, 2p-1)`，右子是 `(d+1, 2p)`。

---

## 思维链

1. 解析每个三位数：`depth = num/100`, `pos = (num/10)%10`, `val = num%10`
2. 用 HashMap 存储 `(depth*10 + pos) -> val`（或用 `depth*10+pos` 作为 key）
3. 从根节点 `(1, 1)` 开始 DFS
4. 对每个节点，计算左子 `(d+1, 2*p-1)` 和右子 `(d+1, 2*p)` 的 key
5. 如果左右子都不存在，当前节点是叶子，把路径和加入结果
6. 否则递归进入存在的子节点

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 解法一 | HashMap + DFS | O(n) | O(n) | ⭐⭐⭐ 首选 |
| 解法二 | HashMap + BFS | O(n) | O(n) | ⭐⭐ 备选 |

---

## 关键提示

### 提示 1 - 编码规则
每个数 = `depth * 100 + pos * 10 + val`。深度从 1 开始，位置从 1 开始（对应满二叉树的位置编号）。

### 提示 2 - 子节点位置关系
在满二叉树中，位置 `p` 的节点：
- 左子位置: `2*p - 1`
- 右子位置: `2*p`

### 提示 3 - 叶子节点判断
如果一个节点的左子和右子的 key 都不在 HashMap 中，则它是叶子节点。

### 提示 4 - HashMap 的 key 设计
用 `depth * 10 + pos` 作为 key（两位数足够，因为 depth<=4, pos<=8），value 存节点值。

---

## 解法详解

### 解法一: HashMap + DFS

```cpp
class Solution {
public:
    int pathSum(vector<int>& nums) {
        unordered_map<int, int> mp; // key = depth*10+pos, val = node value
        for (int num : nums) {
            int key = num / 10;   // depth*10 + pos
            int val = num % 10;
            mp[key] = val;
        }

        int result = 0;
        dfs(mp, nums[0] / 10, 0, result);
        return result;
    }

    void dfs(unordered_map<int, int>& mp, int key, int pathSum, int& result) {
        if (mp.find(key) == mp.end()) return;

        int depth = key / 10;
        int pos = key % 10;
        pathSum += mp[key];

        int leftKey = (depth + 1) * 10 + (2 * pos - 1);
        int rightKey = (depth + 1) * 10 + (2 * pos);

        if (mp.find(leftKey) == mp.end() && mp.find(rightKey) == mp.end()) {
            result += pathSum; // 叶子节点
            return;
        }

        dfs(mp, leftKey, pathSum, result);
        dfs(mp, rightKey, pathSum, result);
    }
};
```

### 解法二: HashMap + BFS

用队列遍历，每个队列元素存 `(key, 累计路径和)`：

```cpp
class Solution {
public:
    int pathSum(vector<int>& nums) {
        unordered_map<int, int> mp;
        for (int num : nums) {
            mp[num / 10] = num % 10;
        }

        int result = 0;
        queue<pair<int, int>> q; // (key, pathSum)
        q.push({nums[0] / 10, mp[nums[0] / 10]});

        while (!q.empty()) {
            auto [key, sum] = q.front(); q.pop();
            int d = key / 10, p = key % 10;
            int lk = (d + 1) * 10 + (2 * p - 1);
            int rk = (d + 1) * 10 + (2 * p);
            bool isLeaf = true;
            if (mp.count(lk)) { q.push({lk, sum + mp[lk]}); isLeaf = false; }
            if (mp.count(rk)) { q.push({rk, sum + mp[rk]}); isLeaf = false; }
            if (isLeaf) result += sum;
        }
        return result;
    }
};
```

---

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 左子位置 = `2*p`, 右子 = `2*p+1` | ✓ 左子 = `2*p-1`, 右子 = `2*p` | 位置从 1 开始，不是从 0 开始 |
| ✗ 把每个节点的值都加入 result | ✓ 只在叶子节点把路径和加入 result | 只统计根到叶子的完整路径 |
| ✗ 忘记检查根节点是否也可能是叶子 | ✓ 只有一个节点时，根就是叶子 | `nums = [111]` 应返回 1 |

---

## 面试追问

**Q1: 为什么用 HashMap 而不是构建真正的树？**
→ 因为树的深度最多为 4（最多 15 个节点），用 HashMap 比构建树节点更简洁。通过编码中的 (depth, pos) 可以直接计算子节点的 key，不需要指针。

**Q2: 子节点位置公式怎么推导的？**
→ 满二叉树中，第 d 层第 p 个节点（1-indexed），其左子在第 d+1 层的位置是 `2p-1`，右子是 `2p`。这是因为第 d 层前 p-1 个节点各贡献 2 个子节点，占据了位置 1 到 2(p-1)，所以第 p 个节点的左子从 2p-1 开始。

**Q3: 如果深度不限于 5 怎么办？**
→ 编码方式需要改变（三位数不够），但算法思路不变。可以用 `(depth, pos)` 的 pair 作为 HashMap 的 key，或用字符串编码。时间空间复杂度仍为 O(n)。

---

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 112 | Path Sum | 基础路径和判断 |
| 113 | Path Sum II | 收集所有路径 |
| 437 | Path Sum III | 任意节点开始的路径 |
| 124 | Binary Tree Maximum Path Sum | 最大路径和 |
