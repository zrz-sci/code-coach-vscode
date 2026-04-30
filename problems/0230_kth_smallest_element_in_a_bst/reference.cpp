/*
 * 【题目本质】
 * BST 中序遍历产生升序序列，第 K 小 = 中序遍历第 K 个节点
 * 关键：提前终止，不需要遍历完所有节点
 *
 * 【解法总览】
 * 解法1: 迭代中序遍历（栈） - O(H+k) 时间, O(H) 空间 ⭐推荐
 * 解法2: 递归中序遍历      - O(H+k) 时间, O(H) 空间
 */

// ==================== 解法1: 迭代中序遍历 ====================
// 思路: 栈模拟中序遍历，弹出第 k 个节点即为答案
// 优势: 真正的提前终止，面试首选
class Solution1 {
public:
    int kthSmallest(TreeNode* root, int k) {
        stack<TreeNode*> stk;
        TreeNode* curr = root;

        while (curr || !stk.empty()) {
            // 一路向左 -> 压入所有左节点
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }

            // 弹出 = 当前最小未访问节点
            curr = stk.top();
            stk.pop();

            // 到达第 k 个，直接返回
            if (--k == 0) return curr->val;

            // 转向右子树
            curr = curr->right;
        }

        return -1; // 题目保证 k 合法，不会到达这里
    }
};

// ==================== 解法2: 递归中序遍历 ====================
// 思路: 递归中序遍历，引用传递计数器和结果
// 注意: 找到后需要传播"已找到"信号来跳过后续递归
class Solution2 {
public:
    int kthSmallest(TreeNode* root, int k) {
        int result = 0;
        int count = 0;
        inorder(root, k, count, result);
        return result;
    }

private:
    void inorder(TreeNode* node, int k, int& count, int& result) {
        if (!node || count >= k) return; // 剪枝：已找到就不继续

        inorder(node->left, k, count, result);

        if (++count == k) {
            result = node->val;
            return;
        }

        inorder(node->right, k, count, result);
    }
};

/*
 * 【解法对比】
 * ┌──────────┬───────────┬──────────┬────────────────────────┐
 * │ 解法     │ 时间      │ 空间     │ 特点                   │
 * ├──────────┼───────────┼──────────┼────────────────────────┤
 * │ 迭代栈   │ O(H + k)  │ O(H)    │ 真正提前终止，面试推荐 │
 * │ 递归     │ O(H + k)  │ O(H)    │ 代码简洁，需剪枝       │
 * └──────────┴───────────┴──────────┴────────────────────────┘
 *
 * 【易错点】
 * 1. 忘记提前终止 -> 退化为 O(n) 遍历整棵树
 * 2. k 是 1-indexed，--k == 0 或 ++count == k 两种写法都可以
 * 3. 迭代法弹出后要 curr = curr->right，否则右子树丢失
 * 4. 递归法剪枝条件 count >= k 不能忘，否则无法提前退出
 *
 * 【面试追问】
 * Q: BST 频繁修改 + 频繁查询第 K 小怎么优化？
 * A: 每个节点存 leftCount（左子树大小），查询 O(H)：
 *    - k <= leftCount: 往左走
 *    - k == leftCount + 1: 当前节点
 *    - k > leftCount + 1: 往右走，k -= leftCount + 1
 *    插入/删除时沿路径更新 leftCount，也是 O(H)
 *
 * Q: 能否转化为迭代器？
 * A: 可以，即 LC 173 BST Iterator，栈保存状态，hasNext/next 均摊 O(1)
 */
