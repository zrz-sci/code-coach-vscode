/*
 * LeetCode 501: 二叉搜索树中的众数 (Find Mode in Binary Search Tree)
 *
 * 【题目本质】
 * BST 中序遍历是有序序列，在有序序列中找出现频率最高的元素。
 * 相同值一定连续出现，所以只需要一个计数器和前驱指针。
 *
 * 【解法总览】
 * 解法1: 哈希表统计      — O(n) / O(n) — 暴力，不利用 BST 性质
 * 解法2: 中序遍历(递归)   — O(n) / O(h) — 利用 BST 有序性 ⭐ 面试首选
 * 解法3: 中序遍历(迭代)   — O(n) / O(h) — 栈模拟递归
 * 解法4: Morris 中序遍历  — O(n) / O(1) — Follow-up: 不用额外空间
 */


// ============================================================
// 解法1: 哈希表统计 — 遍历全树，统计每个值出现次数
// 时间: O(n)  空间: O(n) — 哈希表存所有不同的值
//
// 【思路】
// 最直觉的做法：不管是不是 BST，遍历所有节点用 map 统计频次，
// 然后找最大频次对应的值。
// 缺点：完全没利用 BST 的有序性质，空间浪费在哈希表上。
//
// 示例树:
//     1
//      \
//       2       中序: [1, 2, 2]
//      /
//     2
//
// 哈希表: {1:1, 2:2} → maxFreq=2 → result=[2]
// ============================================================
class Solution1 {
public:
    vector<int> findMode(TreeNode* root) {
        unordered_map<int, int> freq;

        // 任意遍历方式，这里用前序 DFS
        function<void(TreeNode*)> dfs = [&](TreeNode* node) {
            if (!node) return;
            freq[node->val]++;
            dfs(node->left);
            dfs(node->right);
        };
        dfs(root);

        // 找最大频率
        int maxFreq = 0;
        for (auto& [val, cnt] : freq)
            maxFreq = max(maxFreq, cnt);

        // 收集所有达到最大频率的值
        vector<int> result;
        for (auto& [val, cnt] : freq)
            if (cnt == maxFreq) result.push_back(val);

        return result;
    }
};

// ============================================================
// 解法2: 中序遍历(递归) — 利用 BST 中序有序，边遍历边计数
// 时间: O(n)  空间: O(h) — 递归栈，h 是树高
//
// 【思路】
// BST 的中序遍历产生有序序列，相同值一定连续出现。
// 维护 prev 指向上一个访问的节点：
// - 当前值 == prev 值: count++
// - 当前值 != prev 值: count = 1 (新值)
// - count > maxCount: 发现更高频率，清空 result 重填
// - count == maxCount: 并列最高频率，追加到 result
//
// 遍历过程 (示例 [0, 2, 2, 4, 6, 7, 8, 9]):
//
//       6
//      / \
//     2   8
//    / \ / \
//   0  4 7  9
//    \
//     2
//
// 中序: [0, 2, 2, 4, 6, 7, 8, 9]
//
// Step | val | prev | count | maxCount | result
// -----|-----|------|-------|----------|--------
//  1   |  0  | NULL |   1   |    1     | [0]
//  2   |  2  |  0   |   1   |    1     | [0, 2]
//  3   |  2  |  2   |   2   |    2     | [2]      ← clear + push
//  4   |  4  |  2   |   1   |    2     | [2]
//  5   |  6  |  4   |   1   |    2     | [2]
//  6   |  7  |  6   |   1   |    2     | [2]
//  7   |  8  |  7   |   1   |    2     | [2]
//  8   |  9  |  8   |   1   |    2     | [2]
// 最终: result = [2]
// ============================================================
class Solution2 {
public:
    vector<int> findMode(TreeNode* root) {
        vector<int> result;
        TreeNode* prev = nullptr;
        int count = 0, maxCount = 0;

        function<void(TreeNode*)> inorder = [&](TreeNode* node) {
            if (!node) return;
            inorder(node->left);

            // --- 中序位置：处理当前节点 ---
            // 为什么先判 prev：第一个节点时 prev 为空，不能解引用
            if (prev && node->val == prev->val) {
                count++;          // 和前一个相同，计数+1
            } else {
                count = 1;        // 新值出现，重置为1
            }

            // 为什么用 > 而不是 >=：
            // > 时清空重填，保证 result 里只有当前最高频率的值
            // == 时追加，收集所有并列最高频率的值
            if (count > maxCount) {
                maxCount = count;
                result.clear();
                result.push_back(node->val);
            } else if (count == maxCount) {
                result.push_back(node->val);
            }

            prev = node;  // 更新前驱指针
            // --- 处理结束 ---

            inorder(node->right);
        };

        inorder(root);
        return result;
    }
};

// ============================================================
// 解法3: 中序遍历(迭代) — 栈模拟递归
// 时间: O(n)  空间: O(h) — 显式栈
//
// 【思路】
// 和解法2逻辑完全一样，只是用栈模拟中序遍历：
// 1. 一路向左，沿途节点全部入栈
// 2. 弹出栈顶（当前最小未处理节点），处理它
// 3. 转向右子树，回到步骤1
//
// 为什么要会迭代版？
// - 递归在树很深时可能栈溢出（h=n 时递归深度 O(n)）
// - 面试中能写出迭代版展示对遍历的深入理解
//
// 栈模拟过程（以简单树 [1,null,2,2] 为例）:
//     1
//      \
//       2
//      /
//     2
//
// Step1: curr=1, 无左子树, push(1)
//        实际上: curr=1 → 向左到 NULL → 弹出 1 → 处理 1 → curr=1->right=2
// Step2: curr=2, 有左子树, push(2) → curr=2(左孩子)
//        push(2) → curr=NULL → 弹出 2 → 处理 2 → curr=NULL
//        弹出 2 → 处理 2 → curr=NULL
// ============================================================
class Solution3 {
public:
    vector<int> findMode(TreeNode* root) {
        vector<int> result;
        stack<TreeNode*> stk;
        TreeNode* curr = root;
        TreeNode* prev = nullptr;
        int count = 0, maxCount = 0;

        while (curr || !stk.empty()) {
            // 一路向左入栈
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }

            // 弹出栈顶，这是当前最小的未处理节点
            curr = stk.top(); stk.pop();

            // --- 处理逻辑和递归版完全一致 ---
            if (prev && curr->val == prev->val) {
                count++;
            } else {
                count = 1;
            }

            if (count > maxCount) {
                maxCount = count;
                result.clear();
                result.push_back(curr->val);
            } else if (count == maxCount) {
                result.push_back(curr->val);
            }

            prev = curr;
            // --- 处理结束 ---

            // 转向右子树
            curr = curr->right;
        }

        return result;
    }
};

// ============================================================
// 解法4: Morris 中序遍历 — O(1) 额外空间 (Follow-up)
// 时间: O(n)  空间: O(1) — 不需要栈/递归
//
// 【思路】
// Morris 遍历的核心：利用叶子节点空闲的右指针建立临时线索，
// 使得遍历到左子树的最右节点后能"回到"父节点，无需栈。
//
// 对于当前节点 curr：
// Case1: curr 没有左子树
//   → 直接访问 curr，走 curr = curr->right
//
// Case2: curr 有左子树
//   → 找到 curr 左子树的最右节点 (predecessor)
//   Case2a: predecessor->right == NULL (第一次到达)
//     → 建线索: predecessor->right = curr，向左走
//   Case2b: predecessor->right == curr (第二次到达，左子树已遍历完)
//     → 删线索: predecessor->right = NULL，访问 curr，向右走
//
// 每个节点最多被访问两次（一次建线索，一次删线索），总时间 O(n)。
// 遍历结束后树结构完全恢复。
//
// Morris 示例 (树 [4, 2, 6, 1, 3, 5, 7]):
//
//       4
//      / \
//     2   6
//    / \ / \
//   1  3 5  7
//
// curr=4: 有左子树，predecessor=3(2的右子树最右)
//   3->right==NULL → 建线索 3->right=4, curr=2
// curr=2: 有左子树，predecessor=1
//   1->right==NULL → 建线索 1->right=2, curr=1
// curr=1: 无左子树 → 访问1, curr=1->right=2(线索)
// curr=2: 有左子树，predecessor=1
//   1->right==2(==curr) → 删线索, 访问2, curr=3
// curr=3: 无左子树 → 访问3, curr=3->right=4(线索)
// curr=4: 有左子树，predecessor=3
//   3->right==4(==curr) → 删线索, 访问4, curr=6
// curr=6: 有左子树，predecessor=5
//   5->right==NULL → 建线索 5->right=6, curr=5
// curr=5: 无左子树 → 访问5, curr=5->right=6(线索)
// curr=6: 有左子树，predecessor=5
//   5->right==6(==curr) → 删线索, 访问6, curr=7
// curr=7: 无左子树 → 访问7, curr=NULL → 结束
// 访问顺序: [1, 2, 3, 4, 5, 6, 7] ✓ 中序!
// ============================================================
class Solution4 {
public:
    vector<int> findMode(TreeNode* root) {
        vector<int> result;
        TreeNode* curr = root;
        TreeNode* prev = nullptr;
        int count = 0, maxCount = 0;

        while (curr) {
            if (!curr->left) {
                // Case1: 无左子树 → 直接访问当前节点
                process(curr, prev, count, maxCount, result);
                prev = curr;
                curr = curr->right;
            } else {
                // Case2: 有左子树 → 找 predecessor
                TreeNode* predecessor = curr->left;
                while (predecessor->right && predecessor->right != curr)
                    predecessor = predecessor->right;

                if (!predecessor->right) {
                    // Case2a: 第一次到达，建立线索
                    predecessor->right = curr;
                    curr = curr->left;  // 向左走，注意这里不访问 curr
                } else {
                    // Case2b: 第二次到达，左子树已遍历完
                    predecessor->right = nullptr;  // 删除线索，恢复树结构
                    process(curr, prev, count, maxCount, result);
                    prev = curr;
                    curr = curr->right;
                }
            }
        }

        return result;
    }

private:
    // 抽取处理逻辑，避免重复代码
    void process(TreeNode* curr, TreeNode* prev, int& count, int& maxCount, vector<int>& result) {
        if (prev && curr->val == prev->val) {
            count++;
        } else {
            count = 1;
        }
        if (count > maxCount) {
            maxCount = count;
            result.clear();
            result.push_back(curr->val);
        } else if (count == maxCount) {
            result.push_back(curr->val);
        }
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间 | 空间 | 利用BST | 适用场景               |
// |-------------|------|------|---------|----------------------|
// | 哈希表       | O(n) | O(n) |   ✗    | 任意二叉树都能用         |
// | 中序递归     | O(n) | O(h) |   ✓    | 面试首选，代码简洁       |
// | 中序迭代     | O(n) | O(h) |   ✓    | 树很深时避免栈溢出       |
// | Morris      | O(n) | O(1) |   ✓    | Follow-up 要求 O(1) 空间 |
//
// 核心区别：哈希表是通用解法但浪费空间；
//          利用 BST 中序有序后只需 O(1) 变量就能统计频次。
//
// 【易错点】
// 1. prev 判空遗漏:
//    ✗ if (node->val == prev->val) → 第一个节点 prev=nullptr，崩溃
//    ✓ if (prev && node->val == prev->val) → 先判空
//
// 2. count > maxCount 时忘记 clear():
//    ✗ result.push_back(node->val);  // 不清空，旧的还在
//    ✓ result.clear(); result.push_back(node->val);
//
// 3. Morris 中在建线索时就访问节点:
//    ✗ Case2a 中 process(curr,...); predecessor->right = curr; curr = curr->left;
//       → 节点被访问两次！
//    ✓ Case2a 只建线索不访问，Case2b(删线索时)才访问
//
// 4. Morris 的 predecessor 查找死循环:
//    ✗ while (predecessor->right) predecessor = predecessor->right;
//       → 线索建立后 predecessor->right == curr，会绕回来死循环
//    ✓ while (predecessor->right && predecessor->right != curr)
//       → 遇到线索时停止
//
// 【面试追问】
// Q1: 如果不是 BST 而是普通二叉树，怎么找众数？
// → 只能哈希表统计，O(n) 空间。BST 的价值就是中序有序。
//
// Q2: 你的一次遍历怎么保证收集到了所有众数？
// → count > maxCount 时清空重填，保证最终 result 里只有最高频率的值。
//    如果后面出现更高频率，之前的会被清掉。
//
// Q3: Follow-up O(1) 空间怎么实现？
// → Morris 中序遍历。利用叶子空闲右指针建临时线索代替栈，
//    遍历后删除线索恢复树结构。
//
// Q4: 树退化成链表（h=n）时哪种解法最安全？
// → Morris 或迭代版。递归版递归深度 O(n) 可能栈溢出。
//
// 【相关题目】
// - 98.  验证二叉搜索树 — 复用中序遍历 + prev 指针技巧，
//        区别：检查 prev->val < curr->val（严格递增）
// - 230. BST第K小元素 — 复用中序遍历框架，计数到 K 停止
// - 94.  二叉树中序遍历 — 本题的基础，递归/迭代/Morris 三种实现
// - 169. 多数元素(数组) — 无序数组找众数，Boyer-Moore 投票法 O(1) 空间
// ============================================================
