# 109. 有序链表转换二叉搜索树

## 核心思路
将有序链表转为高度平衡BST，本质是每次找到链表的中间节点作为根，递归处理左右两半。

## 思维链
1. **联想数组版本**：LC 108 是有序数组转 BST，直接取中间索引作为根；但链表无法随机访问
2. **朴素转换**：先将链表转为数组，然后用 LC 108 的方法。O(n) 时间 + O(n) 空间
3. **快慢指针找中点**：用快慢指针直接在链表上找中间节点作为根，递归左右子链表
4. **断链递归**：找到中点后需要断开链表，分成左半部分和右半部分
5. **中序遍历模拟**：利用 BST 中序遍历就是有序序列的性质，按中序顺序构建节点
6. **最优方案**：中序模拟法 O(n) 时间 + O(log n) 栈空间，但快慢指针法面试更常见

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 快慢指针 + 递归 | 找中点作根，递归左右 | O(n log n) | O(log n) | ⭐ 推荐 |
| 中序遍历模拟 | 按中序顺序构建BST | O(n) | O(log n) | ⭐ 进阶 |
| 转数组 + 二分 | 链表转数组后用108题方法 | O(n) | O(n) | 了解即可 |

## 关键提示
1. **快慢指针找中点**：slow 每次走1步，fast 每次走2步，fast 到末尾时 slow 在中间
2. **需要保存 prev**：找到中点后要断开左半部分，所以需要记住 slow 的前一个节点
3. **递归终止条件**：当 head 为空或 head == tail 时返回 nullptr

```
有序链表: -10 -> -3 -> 0 -> 5 -> 9

找到中点 0 作为根：
         0
        / \
      -3    9
      /    /
    -10   5

快慢指针过程：
  -10 -> -3 -> 0 -> 5 -> 9
   s      s    s
   f            f         f
  prev = -3, slow = 0 (中点)
```

## 解法详解

### 解法1：快慢指针 + 递归

**思考过程**：
- 链表中点就是 BST 的根（保证左右子树节点数差不超过 1）
- 找到中点后，断开链表，左半递归建左子树，右半递归建右子树
- 每层递归需要 O(n) 找中点，共 O(log n) 层，总时间 O(n log n)

```cpp
class Solution {
public:
    TreeNode* sortedListToBST(ListNode* head) {
        if (!head) return nullptr;
        if (!head->next) return new TreeNode(head->val);
        
        // 快慢指针找中点，prev 记录 slow 前一个节点
        ListNode* prev = nullptr;
        ListNode* slow = head;
        ListNode* fast = head;
        
        while (fast && fast->next) {
            prev = slow;
            slow = slow->next;
            fast = fast->next->next;
        }
        
        // 断开左半部分
        if (prev) prev->next = nullptr;
        
        // slow 就是中点，作为当前根
        TreeNode* root = new TreeNode(slow->val);
        root->left = sortedListToBST(head);        // 左半链表
        root->right = sortedListToBST(slow->next);  // 右半链表
        
        return root;
    }
};
```

**关键点**：
- 必须用 prev 断开左半部分，否则左子树递归时仍包含中点
- 奇数个节点时 slow 恰好在中间，偶数个节点时 slow 在中间偏右

### 解法2：中序遍历模拟

**思考过程**：
- BST 的中序遍历就是有序序列，所以我们可以按中序遍历的顺序来构建
- 先算链表长度，然后递归：先建左子树（消耗链表前半部分），再建根，再建右子树
- 用一个全局指针 current 跟踪当前链表位置

```cpp
class Solution {
public:
    TreeNode* sortedListToBST(ListNode* head) {
        int size = 0;
        ListNode* curr = head;
        while (curr) {
            size++;
            curr = curr->next;
        }
        
        current = head;
        return buildBST(0, size - 1);
    }
    
private:
    ListNode* current;
    
    TreeNode* buildBST(int left, int right) {
        if (left > right) return nullptr;
        
        int mid = left + (right - left) / 2;
        
        // 中序遍历：先递归构建左子树
        TreeNode* leftChild = buildBST(left, mid - 1);
        
        // 当前链表节点就是根（中序遍历到此）
        TreeNode* root = new TreeNode(current->val);
        current = current->next;
        
        root->left = leftChild;
        
        // 再递归构建右子树
        root->right = buildBST(mid + 1, right);
        
        return root;
    }
};
```

**关键点**：
- 利用中序遍历的性质，链表指针自然按顺序推进
- 先递归左子树，再处理当前节点，再递归右子树
- 时间 O(n)，每个节点只访问一次

## 易错点

1. **忘记断链导致无限递归**
```cpp
// ✗ 错误：没有断开左半部分，head 仍然连到 slow
TreeNode* root = new TreeNode(slow->val);
root->left = sortedListToBST(head);  // head 链表仍包含 slow！

// ✓ 正确：必须断开
prev->next = nullptr;
TreeNode* root = new TreeNode(slow->val);
root->left = sortedListToBST(head);
```

2. **prev 初始化为 nullptr 时的边界**
```cpp
// ✗ 错误：当链表只有2个节点时，prev 可能导致问题
// 需要额外判断 head == slow 的情况

// ✓ 正确：已在开头处理了只有1个节点的情况
if (!head->next) return new TreeNode(head->val);
```

3. **中序模拟法忘记推进指针**
```cpp
// ✗ 错误：忘记 current = current->next
TreeNode* root = new TreeNode(current->val);
// current 没有前进！

// ✓ 正确：
TreeNode* root = new TreeNode(current->val);
current = current->next;  // 必须推进
```

## 面试追问

**Q1: 快慢指针法的时间复杂度为什么是 O(n log n)？**
→ 每层递归需要 O(n) 遍历找中点，递归深度为 O(log n)。第一层 n，第二层 n/2 + n/2 = n，每层都是 O(n)，共 O(n log n)。

**Q2: 如何优化到 O(n)？**
→ 使用中序遍历模拟法（解法2），利用 BST 中序遍历等于有序序列的性质，维护一个全局链表指针，每个节点只访问一次。

**Q3: 如果输入是有序双向链表呢？**
→ 双向链表可以同时向前向后遍历，快慢指针法不变。但如果要求原地转换（不使用额外 TreeNode），则需要将链表节点本身作为树节点重排指针。

## 相关题型
- [108. Convert Sorted Array to Binary Search Tree](https://leetcode.com/problems/convert-sorted-array-to-binary-search-tree/) - 数组版本更简单
- [148. Sort List](https://leetcode.com/problems/sort-list/) - 同样使用快慢指针找中点
- [876. Middle of the Linked List](https://leetcode.com/problems/middle-of-the-linked-list/) - 快慢指针基础
