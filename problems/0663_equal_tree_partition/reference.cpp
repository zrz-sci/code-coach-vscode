// 663. Equal Tree Partition
// 难度: Medium
// 标签: Tree, Depth-First Search, Binary Tree

// =====================================================
// 解法一：一遍 DFS + 数组收集（推荐）
// 时间 O(n)  空间 O(n)
// =====================================================
// 后序遍历收集所有子树和到数组，最后一个元素是根的和
// 检查除最后一个元素外是否有 == totalSum/2

class Solution {
public:
    bool checkEqualTree(TreeNode* root) {
        vector<long> subtreeSums;
        dfs(root, subtreeSums);

        long totalSum = subtreeSums.back();

        // 总和为奇数，不可能等分
        if (totalSum % 2 != 0) return false;

        long target = totalSum / 2;

        // 检查除最后一个（根的和）以外的所有子树和
        // 最后一个是根节点的和，切根节点没有意义（另一侧为空）
        for (int i = 0; i < (int)subtreeSums.size() - 1; i++) {
            if (subtreeSums[i] == target) {
                return true;
            }
        }

        return false;
    }

private:
    long dfs(TreeNode* node, vector<long>& sums) {
        if (!node) return 0;
        long sum = node->val + dfs(node->left, sums) + dfs(node->right, sums);
        sums.push_back(sum);
        return sum;
    }
};

// =====================================================
// 解法二：两遍 DFS
// 时间 O(n)  空间 O(h) h为树高
// =====================================================
// 第一遍计算总和，第二遍检查是否存在非根子树和 == totalSum/2

class Solution2 {
public:
    bool checkEqualTree(TreeNode* root) {
        // 第一遍：计算整棵树的总和
        long totalSum = getSum(root);

        // 总和为奇数，不可能等分
        if (totalSum % 2 != 0) return false;

        long target = totalSum / 2;

        // 第二遍：检查左子树或右子树中是否有子树和等于 target
        // 注意：不检查根本身（因为根的子树和 = totalSum）
        bool found = false;
        checkSubtree(root->left, target, found);
        if (!found) checkSubtree(root->right, target, found);
        return found;
    }

private:
    // 计算以 node 为根的子树和
    long getSum(TreeNode* node) {
        if (!node) return 0;
        return node->val + getSum(node->left) + getSum(node->right);
    }

    // DFS 检查是否存在子树和 == target，同时返回子树和
    long checkSubtree(TreeNode* node, long target, bool& found) {
        if (!node) return 0;
        long sum = node->val
                 + checkSubtree(node->left, target, found)
                 + checkSubtree(node->right, target, found);
        if (sum == target) {
            found = true;
        }
        return sum;
    }
};

// =====================================================
// 解法三：一遍 DFS + multiset
// 时间 O(n)  空间 O(n)
// =====================================================
// 收集所有子树和到 multiset，然后检查是否包含 totalSum/2
// 使用 multiset 处理 totalSum=0 的边界情况

class Solution3 {
public:
    bool checkEqualTree(TreeNode* root) {
        multiset<long> sums;
        long totalSum = collectSums(root, sums);

        // 从 multiset 中移除根节点对应的和（即 totalSum）
        // 这样剩下的都是非根子树的和
        sums.erase(sums.find(totalSum));

        // 检查是否有非根子树的和 == totalSum / 2
        return totalSum % 2 == 0 && sums.count(totalSum / 2);
    }

private:
    long collectSums(TreeNode* node, multiset<long>& sums) {
        if (!node) return 0;
        long sum = node->val
                 + collectSums(node->left, sums)
                 + collectSums(node->right, sums);
        sums.insert(sum);
        return sum;
    }
};

// =====================================================
// 示例模拟
// =====================================================
//
// 示例 1: [5, 10, 10, null, null, 2, 3]
//         5
//        / \
//      10   10
//          /  \
//         2    3
//
// totalSum = 5 + 10 + 10 + 2 + 3 = 30
// target = 15
//
// 子树和（后序收集）:
//   叶子 10(左): sum = 10
//   叶子 2:     sum = 2
//   叶子 3:     sum = 3
//   节点 10(右): sum = 10+2+3 = 15  <- 等于 target!
//   根 5:       sum = 30 (跳过，是最后一个)
//
// 找到子树和 15 == target，返回 true
// 移除 5->10(右) 的边: 左部分{5,10}和=15，右部分{10,2,3}和=15
//
// =====================================================
// 示例 2: [1, 2, 10, null, null, 2, 20]
//         1
//        / \
//       2   10
//          /  \
//         2   20
//
// totalSum = 1 + 2 + 10 + 2 + 20 = 35
// 35 是奇数 -> 直接返回 false
//
// =====================================================
// 边界: totalSum = 0 的情况
//
// 树: [0, -1, 1]    ->   0
//                       / \
//                     -1   1
// totalSum = 0, target = 0
// 子树和: -1(左), 1(右), 0(根-跳过)
// 没有非根子树和 == 0 -> 返回 false
//
// 树: [0, 0, 0]     ->   0
//                       / \
//                      0   0
// totalSum = 0, target = 0
// 子树和: 0(左) == target -> 返回 true
