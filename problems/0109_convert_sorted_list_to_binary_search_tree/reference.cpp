/*
 * 【题目本质】
 * 将有序链表转换为高度平衡的BST，核心是每次找中点作根，递归建左右子树。
 *
 * 【解法总览】
 * 1. 快慢指针法 - 找中点+断链+递归    O(n log n) / O(log n)
 * 2. 中序遍历模拟 - 按中序顺序构建    O(n) / O(log n)
 * 3. 转数组法 - 链表转数组后二分构建   O(n) / O(n)
 *
 * 链表与BST的对应关系：
 *
 *   链表: -10 -> -3 -> 0 -> 5 -> 9
 *
 *   BST:       0
 *            /   \
 *          -3     9
 *          /     /
 *        -10    5
 *
 *   中序遍历BST = 链表顺序: -10, -3, 0, 5, 9
 */

// ===================== 解法1：快慢指针 + 递归 =====================
// 思路：链表不支持随机访问，用快慢指针代替数组的二分取中。
// 为什么用快慢指针？因为 fast 走两步 slow 走一步，fast 到底时 slow 恰好在中间。
// 找到中点后需要"断链"，否则递归左半时会越过中点导致无限循环。
class Solution1 {
public:
    TreeNode* sortedListToBST(ListNode* head) {
        if (!head) return nullptr;
        if (!head->next) return new TreeNode(head->val);

        // 快慢指针找中点
        ListNode* prev = nullptr;
        ListNode* slow = head;
        ListNode* fast = head;

        while (fast && fast->next) {
            prev = slow;
            slow = slow->next;
            fast = fast->next->next;
        }

        // 断开左半部分链表
        // 为什么要断？不断的话 sortedListToBST(head) 仍然包含 slow 及之后的节点
        if (prev) prev->next = nullptr;

        TreeNode* root = new TreeNode(slow->val);
        root->left = sortedListToBST(head);         // 左半链表 [head, prev]
        root->right = sortedListToBST(slow->next);  // 右半链表 [slow->next, end]

        return root;
    }
};

// ===================== 解法2：中序遍历模拟 =====================
// 思路：BST 中序遍历 = 有序序列。所以反过来，按中序顺序构建节点，
// 链表指针自然按顺序消耗。这样每个节点只访问一次，总时间 O(n)。
// 为什么比解法1快？因为不需要每次递归都 O(n) 找中点。
class Solution2 {
public:
    TreeNode* sortedListToBST(ListNode* head) {
        // 先计算链表长度
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
    ListNode* current;  // 全局链表指针，按中序顺序推进

    TreeNode* buildBST(int left, int right) {
        if (left > right) return nullptr;

        int mid = left + (right - left) / 2;

        // 关键：先递归构建左子树（消耗链表前半部分）
        TreeNode* leftChild = buildBST(left, mid - 1);

        // 此时 current 恰好指向第 mid 个节点
        TreeNode* root = new TreeNode(current->val);
        current = current->next;  // 推进到下一个节点

        root->left = leftChild;
        root->right = buildBST(mid + 1, right);

        return root;
    }
};

// ===================== 解法3：转数组 + 二分 =====================
// 思路：最直接的方法，先把链表值存入数组，然后等价于 LC 108。
// 缺点是需要 O(n) 额外空间存数组，但代码最简单。
class Solution3 {
public:
    TreeNode* sortedListToBST(ListNode* head) {
        vector<int> nums;
        while (head) {
            nums.push_back(head->val);
            head = head->next;
        }
        return buildBST(nums, 0, (int)nums.size() - 1);
    }

private:
    TreeNode* buildBST(vector<int>& nums, int left, int right) {
        if (left > right) return nullptr;

        int mid = left + (right - left) / 2;
        TreeNode* root = new TreeNode(nums[mid]);
        root->left = buildBST(nums, left, mid - 1);
        root->right = buildBST(nums, mid + 1, right);

        return root;
    }
};

/*
 * 【解法对比】
 * | 解法          | 时间       | 空间      | 适用场景              |
 * |--------------|-----------|----------|---------------------|
 * | 快慢指针+递归  | O(n log n)| O(log n) | 面试首选，思路直观      |
 * | 中序遍历模拟   | O(n)      | O(log n) | 追求最优时间复杂度      |
 * | 转数组+二分    | O(n)      | O(n)     | 代码最简，允许额外空间   |
 *
 * 【易错点】
 * 1. 快慢指针法必须断链（prev->next = nullptr），否则无限递归
 * 2. 中序模拟法中 current 必须在创建根节点后立即推进
 * 3. 递归终止条件：不仅 head==nullptr，也要处理单节点情况
 * 4. 偶数节点时中点取左中还是右中都可以，产生不同但合法的BST
 *
 * 【面试追问】
 * Q1: 时间复杂度分析？
 *     → 快慢指针法：T(n) = 2T(n/2) + O(n)，解得 O(n log n)
 *     → 中序模拟法：每节点访问一次，O(n)
 * Q2: 为什么中序遍历模拟能保证平衡？
 *     → 因为每次用 mid 分割，左右子树节点数差不超过 1
 * Q3: 如果要求空间严格 O(1)（不算递归栈），能做到吗？
 *     → 可以用迭代方式+Morris遍历思想，但实现极其复杂，面试不要求
 */
