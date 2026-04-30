// 776. Split BST
// 难度: Medium
// 解法: 递归拆分
// 时间复杂度: O(h), h 为树高 (最坏 O(n))
// 空间复杂度: O(h) 递归栈

// ============================================================
// 核心思想:
// 利用 BST 性质, 根据 root->val 与 target 的关系:
// - val <= target: root归入小树, 递归拆分右子树
// - val > target:  root归入大树, 递归拆分左子树
// 递归返回 [small, large] 两棵子树的根
// ============================================================

// 解法一: 递归拆分 (标准解法, 面试首选)
class Solution {
public:
    // 返回 {small, large}
    // small: 所有值 <= target 的节点构成的 BST
    // large: 所有值 > target 的节点构成的 BST
    vector<TreeNode*> splitBST(TreeNode* root, int target) {
        if (!root) {
            return {nullptr, nullptr};
        }

        if (root->val <= target) {
            // Case 1: root <= target
            // root 和左子树全部 <= target, 归入 small
            // 右子树需要递归拆分: 可能有部分 <= target
            auto result = splitBST(root->right, target);
            // result[0] = 右子树中 <= target 的部分
            // result[1] = 右子树中 > target 的部分

            // 右子树中 <= target 的部分仍跟着 root
            root->right = result[0];

            // 返回: root 为 small 的根, result[1] 为 large 的根
            return {root, result[1]};
        } else {
            // Case 2: root > target
            // root 和右子树全部 > target, 归入 large
            // 左子树需要递归拆分: 可能有部分 > target
            auto result = splitBST(root->left, target);
            // result[0] = 左子树中 <= target 的部分
            // result[1] = 左子树中 > target 的部分

            // 左子树中 > target 的部分仍跟着 root
            root->left = result[1];

            // 返回: result[0] 为 small 的根, root 为 large 的根
            return {result[0], root};
        }
    }
};

// ============================================================
// 解法二: 迭代实现 (避免递归栈)
// ============================================================
// 使用两个虚拟头节点追踪 small 和 large 树的构建过程
// 沿 BST 路径迭代, 每步将当前节点接入对应的子树

class Solution2 {
public:
    vector<TreeNode*> splitBST(TreeNode* root, int target) {
        // 虚拟头节点, 简化边界处理
        TreeNode smallDummy(0), largeDummy(0);

        // 接口指针: 指向 small/large 树中"等待接入新节点"的位置
        // smallTail: small 树中最右的节点 (下一个节点接到其 right)
        // largeTail: large 树中最左的节点 (下一个节点接到其 left)
        TreeNode* smallTail = &smallDummy;
        TreeNode* largeTail = &largeDummy;

        while (root) {
            if (root->val <= target) {
                // root 归入 small 树
                smallTail->right = root;
                smallTail = root;

                // 保存右子树引用后断开连接
                TreeNode* nextRoot = root->right;
                root->right = nullptr;  // 断开, 防止跨树引用
                root = nextRoot;
            } else {
                // root 归入 large 树
                largeTail->left = root;
                largeTail = root;

                // 保存左子树引用后断开连接
                TreeNode* nextRoot = root->left;
                root->left = nullptr;  // 断开, 防止跨树引用
                root = nextRoot;
            }
        }

        return {smallDummy.right, largeDummy.left};
    }
};

// ============================================================
// 递归过程图解 (以 [4,2,6,1,3,5,7], target=2 为例):
//
// 初始树:        4
//               / \
//              2   6
//             / \ / \
//            1  3 5  7
//
// splitBST(4, 2):
//   4 > 2, 所以 4 归入 large, 递归拆分左子树
//   splitBST(2, 2):
//     2 <= 2, 所以 2 归入 small, 递归拆分右子树
//     splitBST(3, 2):
//       3 > 2, 所以 3 归入 large, 递归拆分左子树
//       splitBST(nullptr, 2): 返回 [null, null]
//       3->left = null
//       返回 [null, 3]
//     2->right = null (rightSmall)
//     返回 [2, 3] (2带着左子树1)
//   4->left = 3 (leftLarge)
//   返回 [2(带1), 4(带3,6(带5,7))]
//
// 结果:
//   small:  2      large:    4
//          /                / \
//         1                3   6
//                             / \
//                            5   7
// ============================================================

// ============================================================
// 要点总结:
// 1. 递归思路: 根据 val 与 target 关系决定归属, 只递归一侧
// 2. 嫁接操作: 递归返回的子树接到当前节点的对应位置
// 3. 时间 O(h): 每次递归沿路径走一步
// 4. 不创建新节点: 只修改指针(left/right)
// 5. 迭代解法用虚拟头节点避免边界判断
// ============================================================
