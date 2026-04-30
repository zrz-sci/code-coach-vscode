# 272. 最接近的二叉搜索树值 II

## 核心思路

本质上是在 BST 的中序有序序列上，找到距离 `target` 最近的 `k` 个值。这等价于在一个有序数组上用滑动窗口/双指针找最近的 k 个元素。

## 思维链

1. **读完题第一反应**：BST 中找最近的 k 个值。最朴素的做法？遍历所有节点，算每个值和 target 的距离，取最近的 k 个 → 全局排序或堆。
2. **暴力的瓶颈**：遍历整棵树 O(n)，排序 O(n log n)，没有利用 BST 的有序性质。
3. **BST 的关键性质**：中序遍历得到有序数组！在有序数组上找最近的 k 个值，就像在排好序的数轴上，以 target 为中心向两侧扩展。
4. **优化方向一：中序 + 滑动窗口**：中序遍历得到有序数组，然后用双指针/滑动窗口在 O(n) 内找到最近 k 个。
5. **优化方向二：中序遍历提前剪枝**：中序遍历时维护大小为 k 的窗口，由于有序性，一旦窗口满且新元素比窗口头更远，可以提前终止。
6. **Follow-up 最优解：双栈模拟双指针**：用两个栈分别模拟"前驱迭代器"和"后继迭代器"，从 target 位置向两侧扩展 k 步，O(k + log n) 时间。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 中序 + 排序/堆 | 遍历所有节点，堆选 top k | O(n log k) | O(n) | 能说出即可 |
| 中序 + 滑动窗口 | 中序有序数组 + 双端队列 | O(n) | O(n) | ⭐ 必须写出 |
| 中序遍历剪枝 | 中序边遍历边维护窗口，提前停止 | O(n) 最坏 / O(k+log n) 平均 | O(k) | ⭐ 必须写出 |
| 双栈前驱后继 | 两个栈模拟双向迭代器 | O(k + log n) | O(log n) | 加分项 |

## 关键提示

- **提示1**：BST 中序遍历 = 有序数组。把树的问题转化成有序数组上的问题。
- **提示2**：在有序数组 `[1,2,3,4,5]` 中找离 `3.7` 最近的 2 个值，你会怎么做？— 从 3.7 附近向两侧扩展。
- **提示3**：中序遍历时，维护一个大小为 k 的双端队列。当队列满了，比较新值和队列头部谁更近 target：
  ```
  中序: 1, 2, 3, 4, 5    target=3.7, k=2
  
  deque: [1]        → 未满，加入
  deque: [1,2]      → 满了
  deque: [2,3]      → |3-3.7|=0.3 < |1-3.7|=2.7，踢掉头部，加入尾部
  deque: [3,4]      → |4-3.7|=0.3 < |2-3.7|=1.7，踢掉头部，加入尾部
  deque: [3,4]      → |5-3.7|=1.3 > |3-3.7|=0.7，停止！
  ```
- **提示4 (Follow-up)**：不需要遍历整棵树。想想二叉搜索中，你可以快速定位到 target 附近，然后只向两侧走 k 步。
- **提示5**：双栈法的核心：一个栈维护"小于等于 target 的前驱序列"（从大到小），另一个栈维护"大于 target 的后继序列"（从小到大），然后像 merge sort 一样合并 k 个。

```
//  示例树:          中序有序序列:
//       4           [1, 2, 3, 4, 5]
//      / \                    ↑
//     2   5          target ≈ 3.7
//    / \
//   1   3           前驱栈(≤3.7): [1, 2, 3]  ← 栈顶3最近
//                   后继栈(>3.7): [5, 4]     ← 栈顶4最近
//                   
//                   比较栈顶: |3-3.7|=0.7 vs |4-3.7|=0.3
//                   选4 → 结果[4], 后继栈弹出4, 推进到5
//                   比较栈顶: |3-3.7|=0.7 vs |5-3.7|=1.3
//                   选3 → 结果[4,3], k=2达到，结束
```

## 解法详解

### 解法1: 中序遍历 + 最大堆 — O(n log k) / O(n)

**思考过程**: 最直觉的做法——遍历所有 n 个节点，用一个大小为 k 的最大堆（按距离排序），维护最近的 k 个值。

```cpp
class Solution {
public:
    vector<int> closestKValues(TreeNode* root, double target, int k) {
        // 最大堆：按 |val - target| 从大到小排
        priority_queue<pair<double, int>> maxHeap;
        
        // 中序遍历（其实任意遍历都行，只要遍历全部节点）
        stack<TreeNode*> stk;
        TreeNode* cur = root;
        while (cur || !stk.empty()) {
            while (cur) {
                stk.push(cur);
                cur = cur->left;
            }
            cur = stk.top(); stk.pop();
            
            double diff = abs(cur->val - target);
            if (maxHeap.size() < k) {
                maxHeap.push({diff, cur->val});
            } else if (diff < maxHeap.top().first) {
                // 新值比堆顶更近，替换
                maxHeap.pop();
                maxHeap.push({diff, cur->val});
            }
            // 注意：因为中序有序，一旦 diff 开始变大且堆满，可以提前停
            // 但堆的做法不容易判断这一点，所以这里遍历全部
            
            cur = cur->right;
        }
        
        vector<int> result;
        while (!maxHeap.empty()) {
            result.push_back(maxHeap.top().second);
            maxHeap.pop();
        }
        return result;
    }
};
```

**关键点**: 堆的做法没利用 BST 有序性，对普通二叉树也适用。缺点是必须遍历全部 n 个节点。

---

### 解法2: 中序遍历 + 滑动窗口 — O(n) / O(k) ⭐ 面试首选

**从解法1优化**: 解法1 没利用 BST 的有序性。中序遍历是有序的，所以最近的 k 个值一定是连续的一段！用双端队列维护大小为 k 的窗口，利用有序性可以提前终止。

```cpp
class Solution {
public:
    vector<int> closestKValues(TreeNode* root, double target, int k) {
        deque<int> dq; // 维护最近的k个值（有序窗口）
        
        // 中序遍历（迭代版）
        stack<TreeNode*> stk;
        TreeNode* cur = root;
        while (cur || !stk.empty()) {
            while (cur) {
                stk.push(cur);
                cur = cur->left;
            }
            cur = stk.top(); stk.pop();
            
            if (dq.size() < k) {
                // 窗口未满，直接加入
                dq.push_back(cur->val);
            } else {
                // 窗口已满：比较新值和窗口最左端谁更近target
                // 因为中序有序，新值 >= 队列中所有值
                // 如果新值更近，踢掉最左端（最远的那个）
                if (abs(cur->val - target) < abs(dq.front() - target)) {
                    dq.pop_front();
                    dq.push_back(cur->val);
                } else {
                    // 新值已经比队列头更远了
                    // 由于有序性，后续值只会更远 → 提前终止！
                    break;
                }
            }
            
            cur = cur->right;
        }
        
        return vector<int>(dq.begin(), dq.end());
    }
};
```

**关键点**:
- 为什么能提前终止？中序有序 → 队列头是窗口中离 target 最远的（在 target 左侧最远），新值在 target 右侧越来越远。一旦新值比队列头还远，说明后面所有值都更远。
- 最坏情况（target 很大，所有值都在左侧）仍是 O(n)，但平均情况会提前终止。

---

### 解法3: 双栈前驱/后继迭代器 — O(k + log n) / O(log n) 加分项

**Follow-up 最优解**: 不遍历整棵树。用两个栈分别定位到 target 的"前驱"和"后继"，然后像归并排序一样向两侧各取 k 步。

**核心思想**:
```
//  BST:              target = 3.7
//       4
//      / \           前驱栈 predStack: 从root出发，
//     2   5          遇到 val <= target 就入栈并往右走，
//    / \             遇到 val > target 就往左走。
//   1   3            最终栈顶 = 最大的 <= target 的值。
//
//  predStack 构建过程:          succStack 构建过程:
//  root=4, 4>3.7 → 往左         root=4, 4<=3.7? No → push(4), 往左
//  node=2, 2<=3.7 → push(2),    node=2, 2<=3.7? Yes → 往右
//    往右                        node=3, 3<=3.7? Yes → 往右
//  node=3, 3<=3.7 → push(3),    node=null → 停
//    往右                        succStack = [4]  (栈顶4是最小的>target)
//  node=null → 停
//  predStack = [2, 3] (栈顶3是最大的<=target)
//
//  合并过程 (取k=2个):
//  |3-3.7|=0.7  vs  |4-3.7|=0.3  → 选4, 推进后继栈
//  |3-3.7|=0.7  vs  |5-3.7|=1.3  → 选3, 推进前驱栈
//  结果: [4, 3]
```

```cpp
class Solution {
public:
    vector<int> closestKValues(TreeNode* root, double target, int k) {
        // 前驱栈：存储 <= target 的节点路径，栈顶是最大的 <= target
        stack<TreeNode*> predStk;
        // 后继栈：存储 > target 的节点路径，栈顶是最小的 > target
        stack<TreeNode*> succStk;
        
        // 初始化两个栈：从根节点出发定位到target附近
        initStacks(root, target, predStk, succStk);
        
        vector<int> result;
        for (int i = 0; i < k; i++) {
            if (predStk.empty() && succStk.empty()) break;
            
            if (succStk.empty() || 
                (!predStk.empty() && 
                 target - predStk.top()->val <= succStk.top()->val - target)) {
                // 前驱更近（或后继栈空）
                result.push_back(predStk.top()->val);
                movePred(predStk); // 移到下一个前驱
            } else {
                // 后继更近（或前驱栈空）
                result.push_back(succStk.top()->val);
                moveSucc(succStk); // 移到下一个后继
            }
        }
        return result;
    }
    
private:
    // 初始化前驱栈和后继栈
    void initStacks(TreeNode* root, double target,
                    stack<TreeNode*>& predStk, stack<TreeNode*>& succStk) {
        TreeNode* cur = root;
        while (cur) {
            if (cur->val <= target) {
                // 当前值 <= target，它是一个潜在的前驱
                predStk.push(cur);
                cur = cur->right; // 往右找更大的前驱
            } else {
                // 当前值 > target，它是一个潜在的后继
                succStk.push(cur);
                cur = cur->left; // 往左找更小的后继
            }
        }
        // 此时 predStk 栈顶 = 最大的 <= target 的节点
        // succStk 栈顶 = 最小的 > target 的节点
    }
    
    // 前驱栈：弹出栈顶后，移到"下一个更小的值"
    // 相当于中序遍历的反向（右→根→左的逆）
    void movePred(stack<TreeNode*>& stk) {
        TreeNode* node = stk.top()->left; // 往左子树走
        stk.pop();
        while (node) {
            stk.push(node);
            node = node->right; // 左子树中最右的 = 左子树最大值
        }
    }
    
    // 后继栈：弹出栈顶后，移到"下一个更大的值"
    // 相当于标准中序遍历的 next
    void moveSucc(stack<TreeNode*>& stk) {
        TreeNode* node = stk.top()->right; // 往右子树走
        stk.pop();
        while (node) {
            stk.push(node);
            node = node->left; // 右子树中最左的 = 右子树最小值
        }
    }
};
```

**为什么是 O(k + log n)?**
- 初始化两个栈：沿树高走一遍，O(log n)（平衡 BST）
- 每次 movePred/moveSucc：均摊 O(1)（类似迭代器 next 的均摊分析）
- 取 k 个值：O(k)
- 总计：O(k + log n)，当 k << n 时远优于 O(n)

---

## 解法对比

| | 解法1: 堆 | 解法2: 中序+窗口 | 解法3: 双栈 |
|---|---|---|---|
| 时间 | O(n log k) | O(n) 最坏 | O(k + log n) |
| 空间 | O(n + k) | O(k + log n) | O(log n) |
| 利用BST性质 | ❌ | ✅ 部分 | ✅ 完全 |
| 代码复杂度 | 简单 | 中等 | 较高 |
| 适用场景 | 通用二叉树 | BST | 平衡BST + k很小 |

**面试建议**：先说解法2展示你理解BST中序有序性，然后被追问Follow-up时给出解法3。

## 易错点

1. **滑动窗口方向搞反**：
   - ✗ 比较 `abs(cur->val - target) < abs(dq.back() - target)`（和尾部比）
   - ✓ 比较 `abs(cur->val - target) < abs(dq.front() - target)`（和头部比）
   - 原因：中序有序，队列头是最早加入的（离 target 最远的那个），新值应该替换最远的

2. **双栈初始化中 `<=` 和 `>` 的边界**：
   - ✗ `cur->val < target` 入前驱栈 → 等于 target 的值被放到后继栈
   - ✓ `cur->val <= target` 入前驱栈 → 确保等于 target 的值在前驱侧（更近）
   - 放哪边都能 work，但要和后续比较逻辑一致

3. **提前终止条件遗漏**：
   - ✗ 解法2中没有 `break`，导致遍历完所有节点
   - ✓ 当新值比窗口头更远时，立即 `break`，这是利用有序性的关键

4. **movePred/moveSucc 逻辑搞混**：
   - 前驱要找"下一个更小的" → 先走左子树，然后一路向右
   - 后继要找"下一个更大的" → 先走右子树，然后一路向左
   - 容易写反方向

## 面试追问

**Q1: 暴力解法是什么？时间复杂度？**
A: 遍历全部节点收集值，排序后取前 k 个。O(n log n) 时间，O(n) 空间。

**Q2: 如何利用 BST 性质优化到 O(n)？**
A: 中序遍历得到有序数组，用大小为 k 的滑动窗口。有序性保证最近的 k 个值是连续段，且可以提前终止。

**Q3 (Follow-up): 能否做到 O(k + log n)？**
A: 用双栈模拟前驱/后继迭代器。初始化 O(log n) 定位到 target 附近，然后双指针合并 k 步。关键是 movePred 和 moveSucc 操作均摊 O(1)。

**Q4: 如果 BST 不平衡（退化成链表），双栈解法的复杂度？**
A: 初始化变成 O(n)，栈空间也是 O(n)。所以双栈法的优势依赖于树的平衡性。对于不平衡的树，解法2更稳定。

## 相关题型

- **270. 最接近的二叉搜索树值** — 本题的简化版（k=1），只需找一个最近值。直接沿 BST 二分搜索即可 O(log n)。
- **94. 二叉树的中序遍历** — 解法2和解法3都依赖中序遍历的迭代实现，这道题是基础。
- **173. 二叉搜索树迭代器** — 解法3的 moveSucc 本质就是 BST 迭代器的 `next()` 操作，可以直接复用。
- **658. 找到 K 个最接近的元素** — 在有序数组上找最接近的 k 个元素，和本题中序后的滑动窗口思路完全一致，可以直接复用双指针/二分+扩展的模板。