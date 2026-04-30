# 558. Logical OR of Two Binary Grids Represented as Quad-Trees - 四叉树交集

## 核心思路
递归合并两棵四叉树：叶节点直接用 OR 逻辑判断，非叶节点递归合并四个子树后尝试压缩。

## 思维链
1. 叶节点 OR 叶节点？ → 任一为 true 则结果为 true 叶节点
2. 叶节点 OR 非叶节点？ → 若叶节点值为 true，直接返回 true 叶节点；否则返回非叶节点
3. 非叶节点 OR 非叶节点？ → 递归合并四个象限，若四个子结果都是相同值叶节点则压缩
4. 压缩条件？ → 四个子节点都是叶节点且值相同

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 递归合并 | O(n) | O(log n) | 直接在树上操作，无需还原矩阵 |
| 还原矩阵暴力法 | O(n^2) | O(n^2) | 简单但效率低 |

## 关键提示
1. 叶节点值为 true 时，OR 任何东西都是 true → 直接剪枝
2. 叶节点值为 false 时，OR x = x → 返回对方
3. 递归后要检查四个子节点能否合并为一个叶节点

## 解法详解

### 解法1: 递归合并 ⭐
```cpp
class Solution {
public:
    Node* intersect(Node* t1, Node* t2) {
        if (t1->isLeaf) return t1->val ? t1 : t2;
        if (t2->isLeaf) return t2->val ? t2 : t1;
        
        Node* tl = intersect(t1->topLeft, t2->topLeft);
        Node* tr = intersect(t1->topRight, t2->topRight);
        Node* bl = intersect(t1->bottomLeft, t2->bottomLeft);
        Node* br = intersect(t1->bottomRight, t2->bottomRight);
        
        // 尝试压缩：四个子节点都是相同值叶节点
        if (tl->isLeaf && tr->isLeaf && bl->isLeaf && br->isLeaf &&
            tl->val == tr->val && tr->val == bl->val && bl->val == br->val) {
            return new Node(tl->val, true);
        }
        return new Node(false, false, tl, tr, bl, br);
    }
};
```

### 解法2: 还原矩阵暴力法
```cpp
class Solution {
public:
    // 先将两棵四叉树还原为矩阵，做 OR，再重建四叉树
    // 时间 O(n^2)，空间 O(n^2)，不推荐
};
```

## 易错点
- ✗ 忘记压缩步骤，生成冗余节点 → ✓ 递归后检查四个子节点是否可合并
- ✗ OR 逻辑搞反：叶节点 false OR 非叶节点应返回非叶节点 → ✓ false OR x = x
- ✗ 新建节点时 isLeaf 设为 true → ✓ 内部节点 isLeaf = false

## 面试追问

**Q1: 如果改成 AND 操作，逻辑怎么变？**
叶节点值 false AND 任何 = false（返回 false 叶节点）；叶节点值 true AND x = x（返回对方）。

**Q2: 时间复杂度如何分析？**
最坏情况每个节点都需要访问一次，O(n) 其中 n 是两棵树节点总数。

**Q3: 如何避免内存泄漏？**
在实际项目中应使用 shared_ptr 或在不需要的节点上 delete。LeetCode 环境不需要手动管理。

## 相关题型
- [427. Construct Quad Tree](https://leetcode.com/problems/construct-quad-tree/) - 构建四叉树
- [772. Basic Calculator III](https://leetcode.com/problems/basic-calculator-iii/) - 递归分治
