/*
 * LeetCode 99: 恢复二叉搜索树 (Recover Binary Search Tree)
 *
 * 【题目本质】
 * BST 中恰好两个节点的值被交换，利用中序遍历的有序性找到
 * 这两个"逆序"节点并交换回来。
 *
 * 【解法总览】
 * 解法1: 中序存数组+排序对比  — O(n log n) / O(n)  — 最直觉
 * 解法2: 递归中序+逆序检测    — O(n) / O(h)        — 面试首选
 * 解法3: 迭代中序+逆序检测    — O(n) / O(h)        — 迭代写法
 * 解法4: Morris中序遍历       — O(n) / O(1)        — Follow-up 最优
 */

// ============================================================
// 解法1: 中序存数组 — 遍历→排序→找差异
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// BST 的中序遍历应该是有序的。将中序结果存入数组，
// 排序后与原数组对比，值不同的位置就是被交换的节点。
// 这是最朴素的想法，帮助理解题意。
//
//  示例2:
//       3          中序遍历: [1, 3, 2, 4]
//      / \         排序后:   [1, 2, 3, 4]
//     1   4                      ↑  ↑
//        /         位置1和位置2不同 → 交换对应节点的值
//       2
// ============================================================
class Solution1 {
public:
    void recoverTree(TreeNode* root) {
        vector<TreeNode*> nodes;
        vector<int> vals;
        inorder(root, nodes, vals);
        
        // 排序得到正确的值序列
        sort(vals.begin(), vals.end());
        
        // 把正确的值写回对应节点
        for (int i = 0; i < (int)nodes.size(); i++) {
            if (nodes[i]->val != vals[i]) {
                nodes[i]->val = vals[i];
            }
        }
    }

private:
    void inorder(TreeNode* root, vector<TreeNode*>& nodes, vector<int>& vals) {
        if (!root) return;
        inorder(root->left, nodes, vals);
        nodes.push_back(root);
        vals.push_back(root->val);
        inorder(root->right, nodes, vals);
    }
};

// ============================================================
// 解法2: 递归中序 + 逆序对检测 — 面试首选 ⭐
// 时间: O(n)  空间: O(h)  (h为树高, 最坏O(n))
//
// 【思路】
// 不需要存数组，中序遍历时维护 prev 指针，
// 一旦发现 prev->val > curr->val 就是逆序。
//
// 关键: 有序数组交换两元素后逆序对的规律：
//
//  情况A: 交换不相邻元素
//  正确: [1, 2, 3, 4, 5, 6, 7]
//  交换: [1, 6, 3, 4, 5, 2, 7]
//            ↑↓         ↑↓
//     逆序1(6,3)   逆序2(5,2)
//  first = 6 (第一个逆序对的前者)
//  second = 2 (最后一个逆序对的后者)
//
//  情况B: 交换相邻元素
//  正确: [1, 2, 3, 4, 5, 6, 7]
//  交换: [1, 2, 4, 3, 5, 6, 7]
//               ↑↓
//        只有逆序(4,3)
//  first = 4, second = 3
//
//  统一写法:
//    第一次逆序: first = prev, second = curr
//    后续逆序:   只更新 second = curr
// ============================================================
class Solution2 {
public:
    void recoverTree(TreeNode* root) {
        TreeNode* first = nullptr;
        TreeNode* second = nullptr;
        TreeNode* prev = nullptr;
        
        inorder(root, prev, first, second);
        
        // 交换回正确的值
        swap(first->val, second->val);
    }

private:
    // 注意: prev, first, second 必须用引用传递!
    // 否则递归中的修改不会传回上层调用
    void inorder(TreeNode* curr, TreeNode*& prev,
                 TreeNode*& first, TreeNode*& second) {
        if (!curr) return;
        
        // 左子树
        inorder(curr->left, prev, first, second);
        
        // 访问当前节点: 检测是否逆序
        if (prev && prev->val > curr->val) {
            if (!first) {
                // 第一次发现逆序: first 记大值(前者)
                first = prev;
            }
            // 每次逆序都更新 second 为小值(后者)
            // 这样无论1个还是2个逆序对都正确
            second = curr;
        }
        prev = curr;
        
        // 右子树
        inorder(curr->right, prev, first, second);
    }
};

// ============================================================
// 解法3: 迭代中序 + 逆序对检测 — 栈模拟递归
// 时间: O(n)  空间: O(h)
//
// 【思路】
// 用显式栈模拟递归中序遍历。检测逆序的逻辑与解法2完全一致。
// 面试中如果被要求写迭代版，这就是标准答案。
//
// 迭代中序模板:
//   1. curr 从 root 开始
//   2. 一路向左全部入栈
//   3. 弹出栈顶 → 访问（检测逆序）
//   4. 转向右子树
//   5. 重复直到 curr==null 且 栈空
//
// 示例1: [1,3,null,null,2]
//     1
//    /
//   3
//    \
//     2
//
// 栈操作:              访问顺序    prev比较
// push(1), left→3
// push(3), left→null
// pop 3, 访问3         [3]        prev=null, 无比较
// right→2
// push(2), left→null
// pop 2, 访问2         [3,2]      prev=3, 3>2 → first=3, second=2
// right→null
// pop 1, 访问1         [3,2,1]    prev=2, 2>1 → second更新为1
// right→null, 栈空
// 结果: first=3, second=1, 交换它们
// ============================================================
class Solution3 {
public:
    void recoverTree(TreeNode* root) {
        stack<TreeNode*> stk;
        TreeNode* curr = root;
        TreeNode* prev = nullptr;
        TreeNode* first = nullptr;
        TreeNode* second = nullptr;
        
        while (curr || !stk.empty()) {
            // 一路向左入栈
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }
            
            // 弹出 = 当前中序节点
            curr = stk.top();
            stk.pop();
            
            // 检测逆序 (和递归版完全一样)
            if (prev && prev->val > curr->val) {
                if (!first) first = prev;
                second = curr;
            }
            prev = curr;
            
            // 转向右子树
            curr = curr->right;
        }
        
        swap(first->val, second->val);
    }
};

// ============================================================
// 解法4: Morris 中序遍历 — O(1) 空间 (Follow-up 最优)
// 时间: O(n)  空间: O(1)
//
// 【思路】
// Follow-up 要求 O(1) 空间。递归和迭代都需要 O(h) 的栈空间。
// Morris 遍历利用叶节点的空右指针建立临时"线索"指向中序后继，
// 无需额外空间即可完成中序遍历。
//
// Morris 中序遍历算法:
// while (curr != null):
//   if curr 没有左子树:
//     访问 curr, curr = curr->right
//   else:
//     找 curr 左子树的最右节点 (predecessor)
//     if predecessor->right == null:
//       建线索: predecessor->right = curr
//       curr = curr->left  (进入左子树)
//     else (predecessor->right == curr):
//       断线索: predecessor->right = null
//       访问 curr, curr = curr->right  (左子树已处理完)
//
// 图解 (示例2):
//
// 初始:          建线索后:           断线索+访问:
//      3              3                  3
//     / \           /   \              /   \
//    1   4         1     4            1     4
//       /           \   /                  /
//      2          →→ 3 2              null 2
//            (1的右指向3)         (恢复1的右为null)
//
// 遍历顺序: 访问1 → 访问3 → 访问2(发现3>2,逆序!) → 访问4
// ============================================================
class Solution4 {
public:
    void recoverTree(TreeNode* root) {
        TreeNode* first = nullptr;
        TreeNode* second = nullptr;
        TreeNode* prev = nullptr;
        TreeNode* curr = root;
        
        while (curr) {
            if (!curr->left) {
                // 没有左子树: 直接访问当前节点, 向右走
                detect(prev, curr, first, second);
                prev = curr;
                curr = curr->right;
            } else {
                // 找左子树的最右节点 (中序前驱 predecessor)
                TreeNode* predecessor = curr->left;
                while (predecessor->right && predecessor->right != curr) {
                    predecessor = predecessor->right;
                }
                
                if (!predecessor->right) {
                    // predecessor 的右指针为空:
                    // 建立线索, 让 predecessor->right 指向 curr (后继)
                    // 然后进入左子树
                    predecessor->right = curr;
                    curr = curr->left;
                } else {
                    // predecessor 的右指针已指向 curr:
                    // 说明左子树已遍历完毕
                    // 1. 断开线索 (恢复树结构)
                    // 2. 访问当前节点
                    // 3. 向右走
                    predecessor->right = nullptr;
                    detect(prev, curr, first, second);
                    prev = curr;
                    curr = curr->right;
                }
            }
        }
        
        // 遍历结束, 交换两个错误节点的值
        swap(first->val, second->val);
    }

private:
    // 逆序检测 — 和前面解法完全相同的逻辑
    void detect(TreeNode* prev, TreeNode* curr,
                TreeNode*& first, TreeNode*& second) {
        if (prev && prev->val > curr->val) {
            if (!first) first = prev;
            second = curr;
        }
    }
};

/*
 * ============================================================
 * 【解法对比】
 * 
 * | 解法 | 时间       | 空间 | 特点                    |
 * |------|-----------|------|------------------------|
 * | 1    | O(nlogn)  | O(n) | 最直觉,面试说思路即可    |
 * | 2    | O(n)      | O(h) | ⭐面试首选,递归简洁      |
 * | 3    | O(n)      | O(h) | 迭代版,栈模拟递归        |
 * | 4    | O(n)      | O(1) | Follow-up,Morris遍历   |
 *
 * 解法2 vs 3: 逻辑完全相同,只是遍历框架不同(递归 vs 栈)
 * 解法3 vs 4: 都是迭代,4用Morris替代栈实现O(1)空间
 *
 * ============================================================
 * 【易错点】
 *
 * 1. second 赋值位置:
 *    ✗ if (!first) { first=prev; second=curr; }
 *      else if (first) { second=curr; }
 *      → 正确但多余的条件判断
 *    ✓ if (!first) first=prev;
 *      second = curr;  // 放在if外面,每次逆序都更新
 *      → 简洁且覆盖1个/2个逆序对两种情况
 *
 * 2. 递归版忘记用引用:
 *    ✗ void inorder(TreeNode* curr, TreeNode* prev, ...)
 *      → prev/first/second 的修改不传回上层
 *    ✓ void inorder(TreeNode* curr, TreeNode*& prev, ...)
 *      → 必须用 TreeNode*&
 *    (或者用类成员变量代替参数传递)
 *
 * 3. Morris遍历忘记断线索:
 *    ✗ 只建立 predecessor->right = curr, 遍历完不恢复
 *      → 树结构被永久修改,后续操作出错
 *    ✓ 第二次遇到时 predecessor->right = nullptr 恢复
 *
 * 4. 找到第一个逆序就停止:
 *    ✗ 发现一个逆序对就 return/break
 *    ✓ 必须遍历完,可能有第二个逆序对需要更新 second
 *
 * ============================================================
 * 【面试追问】
 *
 * Q1: 为什么交换两个值后中序序列一定有逆序？
 * → BST中序遍历严格递增,交换任意两个值必然破坏某处的递增性
 *
 * Q2: 逆序对可能是1个也可能是2个,怎么统一处理？
 * → first始终=第一个逆序对的大值(prev), second始终更新为
 *   最新逆序对的小值(curr)。1个逆序对时second就是第一次的curr,
 *   2个逆序对时second被更新为第二次的curr,两种情况都正确。
 *
 * Q3: 能否O(1)空间？
 * → Morris遍历:利用空右指针建临时线索,不需要栈。
 *   核心:找到当前节点左子树的最右节点(中序前驱),
 *   用它的空右指针记录"回来的路"。
 *
 * Q4: 如果不是恰好交换2个,而是K个节点被打乱怎么办？
 * → 中序遍历得到实际序列,排序得到期望序列,对比找出所有
 *   不匹配位置,逐一恢复。需要O(n)空间存序列,O(nlogn)排序。
 *
 * ============================================================
 */
