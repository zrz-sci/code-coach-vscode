/*
 * LeetCode 106: 从中序与后序遍历序列构造二叉树
 *     (Construct Binary Tree from Inorder and Postorder Traversal)
 *
 * 【题目本质】
 * 利用后序遍历"最后一个 = 根"的性质确定根节点，
 * 再利用中序遍历"根的左边 = 左子树, 右边 = 右子树"的性质划分子问题，
 * 递归地重建整棵二叉树。
 *
 * 【解法总览】
 * 解法1: 暴力递归(切分数组)  — O(n²) / O(n²) — 最直觉但低效
 * 解法2: 哈希表+索引分治     — O(n)  / O(n)  — ⭐ 面试首选
 * 解法3: 迭代法(栈)          — O(n)  / O(n)  — 加分项
 */

// ============================================================
// 解法1: 暴力递归(切分数组) — 每次线性查找根 + 真正切割子数组
// 时间: O(n²) 最坏(链状树)  空间: O(n²) 每层拷贝子数组
//
// 【思路】
// 后序最后一个元素 = 根。在中序中线性查找根的位置，
// 得到左子树大小 leftSize，然后把中序和后序各切成左右子树的子数组。
// 
// 瓶颈: ① 每次线性查找 O(n)  ② 切数组拷贝开销大
//
// 【可视化 — 切分过程】
//
//  中序: [9, | 3, | 15, 20, 7]
//        左   根     右子树
//
//  后序: [9, | 15, 7, 20, | 3]
//        左     右子树     根
//
//  构建出:
//           3
//          / \
//         9   20
//            /  \
//          15    7
// ============================================================
class Solution1 {
public:
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        if (inorder.empty()) return nullptr;

        // 后序最后一个 = 根
        int rootVal = postorder.back();
        TreeNode* root = new TreeNode(rootVal);

        // 线性查找根在中序中的位置（瓶颈所在）
        int rootIdx = 0;
        for (int i = 0; i < (int)inorder.size(); i++) {
            if (inorder[i] == rootVal) {
                rootIdx = i;
                break;
            }
        }

        int leftSize = rootIdx;

        // 真正切割中序数组
        vector<int> inLeft(inorder.begin(), inorder.begin() + rootIdx);
        vector<int> inRight(inorder.begin() + rootIdx + 1, inorder.end());

        // 真正切割后序数组：前 leftSize 个是左子树，接下来到倒数第二个是右子树
        vector<int> postLeft(postorder.begin(), postorder.begin() + leftSize);
        vector<int> postRight(postorder.begin() + leftSize, postorder.end() - 1);

        root->left = buildTree(inLeft, postLeft);
        root->right = buildTree(inRight, postRight);

        return root;
    }
};

// ============================================================
// 解法2: 哈希表 + 索引分治 — ⭐ 面试首选
// 时间: O(n)  空间: O(n) (哈希表 + 递归栈)
//
// 【思路】
// 解法1有两个瓶颈:
//   ① 每次线性查找根位置 → 哈希表预存 值→索引，O(1) 查找
//   ② 每次切割子数组 → 改为传递索引范围 [inLeft, inRight], [postLeft, postRight]
//
// 【索引切分关系 — 核心公式】
//
//  中序: [ ... 左子树 ... | root | ... 右子树 ... ]
//         inLeft     rootIdx-1  rootIdx  rootIdx+1   inRight
//
//  后序: [ ... 左子树 ... | ... 右子树 ... | root ]
//         postLeft  postLeft+leftSize-1  ...  postRight-1  postRight
//
//  其中 leftSize = rootIdx - inLeft
//
// 【递归过程可视化】
//
//  inorder:   [9, 3, 15, 20, 7]    索引: 0  1  2   3  4
//  postorder: [9, 15, 7, 20, 3]    索引: 0  1  2   3  4
//
//  第1层: root = post[4] = 3
//         rootIdx = 1, leftSize = 1-0 = 1
//         左: in[0,0] post[0,0]   右: in[2,4] post[1,3]
//
//  第2层左: root = post[0] = 9
//           rootIdx = 0, leftSize = 0
//           左: in[0,-1] → null   右: in[1,0] → null
//           → 叶子节点 9
//
//  第2层右: root = post[3] = 20
//           rootIdx = 3, leftSize = 3-2 = 1
//           左: in[2,2] post[1,1]  右: in[4,4] post[2,2]
//
//  第3层: root=15 (叶子), root=7 (叶子)
//
//  最终:
//           3
//          / \
//         9   20
//            /  \
//          15    7
// ============================================================
class Solution2 {
public:
    unordered_map<int, int> inorderMap; // 值 → 中序索引

    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        // 预建哈希表：O(n) 一次性建好，后续每次查找 O(1)
        for (int i = 0; i < (int)inorder.size(); i++) {
            inorderMap[inorder[i]] = i;
        }
        return build(inorder, postorder,
                     0, (int)inorder.size() - 1,
                     0, (int)postorder.size() - 1);
    }

private:
    TreeNode* build(vector<int>& inorder, vector<int>& postorder,
                    int inLeft, int inRight,
                    int postLeft, int postRight) {
        // 区间为空 → 没有节点
        // 注意是 > 不是 >=，因为 inLeft == inRight 时还有一个节点
        if (inLeft > inRight || postLeft > postRight) return nullptr;

        // 后序最后一个 = 当前子树的根
        int rootVal = postorder[postRight];
        TreeNode* root = new TreeNode(rootVal);

        // O(1) 找到根在中序中的位置
        int rootIdx = inorderMap[rootVal];

        // 左子树节点个数 = 根位置 - 中序左边界
        // 这是正确切分后序数组的关键
        int leftSize = rootIdx - inLeft;

        // 递归构建左子树
        //   中序: [inLeft, rootIdx - 1]
        //   后序: [postLeft, postLeft + leftSize - 1]
        root->left = build(inorder, postorder,
                          inLeft, rootIdx - 1,
                          postLeft, postLeft + leftSize - 1);

        // 递归构建右子树
        //   中序: [rootIdx + 1, inRight]
        //   后序: [postLeft + leftSize, postRight - 1]  ← 注意 postRight 是根，要排除
        root->right = build(inorder, postorder,
                           rootIdx + 1, inRight,
                           postLeft + leftSize, postRight - 1);

        return root;
    }
};

// ============================================================
// 解法3: 迭代法(栈) — 不用递归
// 时间: O(n)  空间: O(n) (栈)
//
// 【思路】
// 后序逆序 = "根-右-左"（类似前序的镜像）。
// 从后序数组从右往左遍历，依次创建节点。
// 用栈维护"当前路径"，用中序数组从右往左的指针来判断
// "何时右子树结束、该转到左子树"。
//
// 核心逻辑：
//   - 如果栈顶 != inorder[inIdx]：新节点是栈顶的右孩子
//   - 如果栈顶 == inorder[inIdx]：回溯弹栈直到不匹配，
//     新节点是最后弹出节点的左孩子
//
// 【执行过程】
// postorder: [9, 15, 7, 20, 3],  inorder: [9, 3, 15, 20, 7]
//
// i=4: 创建 3(根), push, 栈=[3], inIdx=4
//      栈顶3 != inorder[4]=7 → 不回溯
// i=3: 创建 20, 3.right=20, push, 栈=[3,20], inIdx=4
//      栈顶20 != 7 → 不回溯
// i=2: 创建 7, 20.right=7, push, 栈=[3,20,7], inIdx=4
//      栈顶7 == inorder[4]=7 → 弹7, inIdx=3
//      栈顶20 == inorder[3]=20 → 弹20, inIdx=2
//      栈顶3 != inorder[2]=15 → 停止, parent=20
// i=1: 创建 15, 20.left=15, push, 栈=[3,15], inIdx=2
//      栈顶15 == inorder[2]=15 → 弹15, inIdx=1
//      栈顶3 == inorder[1]=3 → 弹3, inIdx=0
//      栈空 → 停止, parent=3
// i=0: 创建 9, 3.left=9, push, 栈=[9]
//
// 结果:
//           3
//          / \
//         9   20
//            /  \
//          15    7
// ============================================================
class Solution3 {
public:
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        if (postorder.empty()) return nullptr;

        int n = postorder.size();
        TreeNode* root = new TreeNode(postorder[n - 1]);
        stack<TreeNode*> stk;
        stk.push(root);

        int inIdx = n - 1; // 中序数组从右往左走

        // 后序从倒数第二个开始（最后一个已经是 root）
        for (int i = n - 2; i >= 0; i--) {
            TreeNode* node = new TreeNode(postorder[i]);

            if (stk.top()->val != inorder[inIdx]) {
                // 栈顶还没到中序边界 → 新节点是栈顶的右孩子
                // 因为后序逆序是"根-右-左"，先处理右
                stk.top()->right = node;
            } else {
                // 右子树结束了，需要回溯找到应该挂左孩子的那个祖先
                TreeNode* parent = nullptr;
                while (!stk.empty() && stk.top()->val == inorder[inIdx]) {
                    parent = stk.top();
                    stk.pop();
                    inIdx--;
                }
                // 新节点是回溯到的祖先的左孩子
                parent->left = node;
            }
            stk.push(node);
        }

        return root;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | 解法1(暴力切数组) | 解法2(哈希+索引) | 解法3(迭代栈)   |
// |-----------|------------------|-----------------|----------------|
// | 时间       | O(n²) 最坏       | O(n)            | O(n)           |
// | 空间       | O(n²) 拷贝       | O(n)            | O(n)           |
// | 思路清晰度  | 最直觉            | 清晰             | 需要理解逆后序  |
// | 面试推荐   | 不推荐            | ⭐ 首选          | 追问时展示      |
//
// 解法1 → 解法2: 两处优化
//   ① 线性查找 → 哈希表 O(1)
//   ② 切割数组 → 索引范围传参
//
// 解法2 → 解法3: 从递归到迭代
//   递归天然匹配分治思想，迭代需要手动用栈模拟
//
// ============================================================
// 【易错点】
//
// 1. leftSize 计算错误:
//    ✗ int leftSize = rootIdx;  // 没减 inLeft 偏移量
//    ✓ int leftSize = rootIdx - inLeft;
//    当 inLeft != 0 时（即处理非第一层递归时），直接用 rootIdx 会多算
//
// 2. 后序右子树区间右端点错误:
//    ✗ root->right = build(..., postLeft + leftSize, postRight);
//    ✓ root->right = build(..., postLeft + leftSize, postRight - 1);
//    postRight 是当前根，必须排除在右子树之外
//
// 3. 递归终止条件写错:
//    ✗ if (inLeft >= inRight) return nullptr;  // 等于时漏掉了单节点
//    ✓ if (inLeft > inRight) return nullptr;
//    当 inLeft == inRight 时正好有一个节点，应该创建并返回
//
// 4. 哈希表存错了:
//    ✗ inorderMap[i] = inorder[i];   // 索引→值，反了
//    ✓ inorderMap[inorder[i]] = i;   // 值→索引
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1(基础): 为什么后序最后一个是根？
//   → 后序遍历 = 左-右-根，最后处理的就是根节点。
//
// Q2(优化): 暴力解 O(n²)，怎么优化到 O(n)？
//   → 哈希表预存中序 值→索引（消除线性查找），
//     用索引范围传参（消除数组拷贝）。
//
// Q3(姊妹题): 如果给前序+中序呢？(LeetCode 105)
//   → 几乎一样的框架。区别：前序第一个元素是根（不是最后一个）。
//     代码改动：rootVal = preorder[preLeft]（而不是 postorder[postRight]），
//     左子树前序范围 = [preLeft+1, preLeft+leftSize]，
//     右子树前序范围 = [preLeft+leftSize+1, preRight]。
//
// Q4(深入): 能否用迭代实现？
//   → 解法3: 利用"后序逆序 = 根右左"的性质，用栈 + 中序指针来判断
//     右子树结束的时机。
//
// Q5(扩展): 如果值有重复怎么办？
//   → 仅中序+后序无法唯一确定（中序中根的位置可能有多个候选）。
//     需要额外信息（如树的结构约束）才能消除歧义。
// ============================================================
