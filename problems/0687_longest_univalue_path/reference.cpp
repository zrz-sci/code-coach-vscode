// LeetCode 687: Longest Univalue Path
// 解法一: DFS 后序遍历 + 全局变量
// 解法二: DFS 后序遍历 + 引用参数（无全局变量）
// 时间: O(n), 空间: O(h)

// ============================================================
// 核心模式: 树上路径问题
// - 递归返回: 从当前节点向下的最长单侧同值边数
// - 全局更新: 在当前节点处用 leftArm + rightArm 更新答案
// - 路径不能分叉，所以返回给父节点的只能是 max(left, right)
// ============================================================

// 解法一: DFS + 全局变量
class Solution {
public:
    int ans = 0;

    int longestUnivaluePath(TreeNode* root) {
        ans = 0; // 重要: 重置，因为 LeetCode 可能复用 Solution 对象
        dfs(root);
        return ans;
    }

private:
    // 返回: 以 node 为起点向下延伸的最长同值边数（单侧）
    int dfs(TreeNode* node) {
        if (!node) return 0;

        // 后序遍历: 先处理左右子树
        int leftLen = dfs(node->left);
        int rightLen = dfs(node->right);

        int leftArm = 0, rightArm = 0;

        // 检查左子节点是否与当前节点同值
        if (node->left && node->left->val == node->val) {
            leftArm = leftLen + 1; // 延伸一条边到左子
        }
        // 不同值时 leftArm 保持 0, 路径在此中断

        // 检查右子节点是否与当前节点同值
        if (node->right && node->right->val == node->val) {
            rightArm = rightLen + 1; // 延伸一条边到右子
        }

        // 经过当前节点的完整路径 = 左臂 + 右臂
        // 这条路径在 node 处"拐弯", 不能再向上延伸
        ans = max(ans, leftArm + rightArm);

        // 返回较长的一侧给父节点（路径不能分叉）
        return max(leftArm, rightArm);
    }
};

// ============================================================
// 解法二: DFS + 引用参数（避免全局变量，更函数式）
// 时间: O(n), 空间: O(h)
//
// 通过引用参数传递全局最优值，避免成员变量的副作用
// ============================================================

class Solution2 {
public:
    int longestUnivaluePath(TreeNode* root) {
        int result = 0;
        dfs(root, result);
        return result;
    }

private:
    int dfs(TreeNode* node, int& globalMax) {
        if (!node) return 0;

        int leftLen = dfs(node->left, globalMax);
        int rightLen = dfs(node->right, globalMax);

        int leftArm = 0, rightArm = 0;

        if (node->left && node->left->val == node->val) {
            leftArm = leftLen + 1;
        }
        if (node->right && node->right->val == node->val) {
            rightArm = rightLen + 1;
        }

        // 更新经过 node 拐弯的路径长度
        globalMax = max(globalMax, leftArm + rightArm);

        return max(leftArm, rightArm);
    }
};

// ============================================================
// 解法三: N 叉树推广（面试追问）
// 时间: O(n), 空间: O(h)
//
// 对于 N 叉树, 需要找所有子节点中最大的两个臂长之和
// 返回最大的一个臂长给父节点
// ============================================================

/*
// 假设 N 叉树节点定义:
// struct NTreeNode {
//     int val;
//     vector<NTreeNode*> children;
// };

class Solution3 {
public:
    int longestUnivaluePath(NTreeNode* root) {
        int result = 0;
        dfs(root, result);
        return result;
    }

private:
    int dfs(NTreeNode* node, int& globalMax) {
        if (!node) return 0;

        // 收集所有同值子节点的臂长
        int maxArm = 0;      // 最长臂
        int secMaxArm = 0;   // 次长臂

        for (auto* child : node->children) {
            int childLen = dfs(child, globalMax);
            int arm = 0;
            if (child->val == node->val) {
                arm = childLen + 1;
            }
            // 维护最大和次大
            if (arm >= maxArm) {
                secMaxArm = maxArm;
                maxArm = arm;
            } else if (arm > secMaxArm) {
                secMaxArm = arm;
            }
        }

        // 经过 node 的最优路径: 最长臂 + 次长臂
        globalMax = max(globalMax, maxArm + secMaxArm);

        // 返回最长单侧臂给父节点
        return maxArm;
    }
};
*/

// ============================================================
// 示例推演:
//       1
//      / \
//     4   5
//    / \   \
//   4   4    5
//
// dfs(左下4): 无子 → leftArm=0, rightArm=0, ans=0, return 0
// dfs(右下4): 无子 → leftArm=0, rightArm=0, ans=0, return 0
// dfs(左上4): left=0,right=0
//   leftArm = 0+1 = 1 (左子值4==自身4)
//   rightArm = 0+1 = 1 (右子值4==自身4)
//   ans = max(0, 1+1) = 2
//   return max(1,1) = 1
// dfs(右下5): 无子 → return 0
// dfs(右上5): right=0
//   rightArm = 0+1 = 1 (右子值5==自身5)
//   ans = max(2, 0+1) = 2 (不更新)
//   return 1
// dfs(根1): left=1, right=1
//   leftArm = 0 (左子值4≠自身1, 中断!)
//   rightArm = 0 (右子值5≠自身1, 中断!)
//   ans = max(2, 0) = 2
//   return 0
//
// 最终答案: 2 (路径: 4-4-4)
// ============================================================
