/*
 * LeetCode 272: Closest Binary Search Tree Value II
 * 
 * 【题目本质】
 * 在 BST 中找距离 target 最近的 k 个值。
 * BST 中序遍历 = 有序数组 → 问题转化为在有序数组上找最近 k 个值。
 *
 * 【解法总览】
 * 解法1: 中序 + 最大堆       — O(n log k) / O(n)       — 最直觉，不依赖BST有序性
 * 解法2: 中序 + 滑动窗口     — O(n) / O(k)             — 利用有序性，面试首选
 * 解法3: 双栈前驱后继迭代器  — O(k + log n) / O(log n)  — Follow-up最优解
 */


// ============================================================
// 解法1: 中序遍历 + 最大堆 — 遍历所有节点，堆维护最近k个
// 时间: O(n log k)  空间: O(n) (栈) + O(k) (堆)
//
// 【思路】
// 不管 BST 有序性，把它当普通二叉树处理。
// 遍历每个节点，用大小为 k 的最大堆（按距离排序）维护最近的 k 个值。
// 当堆满且新值比堆顶更近时，替换堆顶。
//
//  示例: root = [4,2,5,1,3], target = 3.714286, k = 2
//       4
//      / \
//     2   5
//    / \
//   1   3
//
//  遍历过程 (中序: 1,2,3,4,5):
//  node=1: heap=[(2.71,1)]             -- 未满
//  node=2: heap=[(2.71,1),(1.71,2)]    -- 未满(size=k=2)
//  node=3: 0.71 < 2.71 → pop(1), push(3) → heap=[(1.71,2),(0.71,3)]
//  node=4: 0.29 < 1.71 → pop(2), push(4) → heap=[(0.71,3),(0.29,4)]
//  node=5: 1.29 > 0.71 → 不替换
//  结果: [3, 4]
// ============================================================
class Solution1 {
public:
    vector<int> closestKValues(TreeNode* root, double target, int k) {
        // 最大堆: {距离, 值}，堆顶是距离最大的
        priority_queue<pair<double, int>> maxHeap;
        
        // 中序遍历(迭代)
        stack<TreeNode*> stk;
        TreeNode* cur = root;
        while (cur || !stk.empty()) {
            while (cur) {
                stk.push(cur);
                cur = cur->left;
            }
            cur = stk.top(); stk.pop();
            
            double diff = abs(cur->val - target);
            if ((int)maxHeap.size() < k) {
                maxHeap.push({diff, cur->val});
            } else if (diff < maxHeap.top().first) {
                // 新值比堆中最远的更近 → 替换
                maxHeap.pop();
                maxHeap.push({diff, cur->val});
            }
            // 注意: 这里不能提前终止，因为堆不保证有序关系
            // （但如果用中序+有序性，可以提前终止 — 见解法2）
            
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

// ============================================================
// 解法2: 中序遍历 + 滑动窗口(双端队列) — 利用BST有序性
// 时间: O(n) 最坏, 可提前终止  空间: O(k) (队列) + O(log n) (栈)
//
// 【思路】
// BST 中序遍历 = 有序数组。最近的 k 个值在有序数组中一定是连续的一段。
// 用双端队列维护大小为 k 的窗口:
//   - 窗口未满: 直接加入尾部
//   - 窗口已满: 新值(有序,从小到大) vs 队列头(最老/最远的值)
//     - 新值更近 → 踢掉头部，加入尾部
//     - 新值更远 → 后续只会更远，提前终止!
//
//  中序: [1, 2, 3, 4, 5]    target=3.714286, k=2
//
//  Step1: deque=[1]         未满
//  Step2: deque=[1,2]       满了(size=k=2)
//  Step3: |3-3.71|=0.71 < |1-3.71|=2.71 → pop_front, push_back
//         deque=[2,3]
//  Step4: |4-3.71|=0.29 < |2-3.71|=1.71 → pop_front, push_back
//         deque=[3,4]
//  Step5: |5-3.71|=1.29 > |3-3.71|=0.71 → break!
//  结果: [3, 4]
// ============================================================
class Solution2 {
public:
    vector<int> closestKValues(TreeNode* root, double target, int k) {
        deque<int> dq;
        
        stack<TreeNode*> stk;
        TreeNode* cur = root;
        while (cur || !stk.empty()) {
            while (cur) {
                stk.push(cur);
                cur = cur->left;
            }
            cur = stk.top(); stk.pop();
            
            if ((int)dq.size() < k) {
                dq.push_back(cur->val);
            } else {
                // 窗口已满，利用有序性判断
                // 新值(cur->val)一定 >= dq中所有值(中序递增)
                // dq.front() 是窗口中最小的，也是离target最远的（在target左侧最远端）
                if (abs(cur->val - target) < abs(dq.front() - target)) {
                    dq.pop_front();  // 踢掉最远的
                    dq.push_back(cur->val);
                } else {
                    // 新值已经比窗口头更远了
                    // 有序性保证后续值只会更远 → 提前终止
                    break;
                }
            }
            
            cur = cur->right;
        }
        
        return vector<int>(dq.begin(), dq.end());
    }
};

// ============================================================
// 解法3: 双栈前驱/后继迭代器 — Follow-up O(k + log n)
// 时间: O(k + log n) (平衡BST)  空间: O(log n)
//
// 【思路】
// 核心想法: 不遍历整棵树，只定位到target附近，然后向两侧扩展k步。
//
// 用两个栈模拟"前驱迭代器"和"后继迭代器":
//   - predStk: 栈顶 = 最大的 <= target 的节点
//   - succStk: 栈顶 = 最小的 > target 的节点
//
// 初始化: 从根出发沿BST搜索路径分流到两个栈
//   val <= target → 入predStk，往右找更大前驱
//   val >  target → 入succStk，往左找更小后继
//
// 取k个值: 比较两个栈顶谁更近target，选中的那个栈"推进"到下一个值
//   - movePred: 弹出栈顶，走左子树，然后一路向右（找下一个更小的）
//   - moveSucc: 弹出栈顶，走右子树，然后一路向左（找下一个更大的）
//
//  示例:  target = 3.714286, k = 2
//       4
//      / \
//     2   5
//    / \
//   1   3
//
//  初始化过程:
//  cur=4: 4 > 3.71 → succStk=[4], go left
//  cur=2: 2 <= 3.71 → predStk=[2], go right
//  cur=3: 3 <= 3.71 → predStk=[2,3], go right
//  cur=null → 停
//  
//  predStk = [2, 3]  (栈顶3 = 最大的<=3.71)
//  succStk = [4]      (栈顶4 = 最小的>3.71)
//
//  取值过程:
//  i=0: |3-3.71|=0.71 vs |4-3.71|=0.29 → 选4
//       moveSucc: 4弹出, 右子树=5, push(5)
//       succStk = [5]
//  i=1: |3-3.71|=0.71 vs |5-3.71|=1.29 → 选3
//       movePred: 3弹出, 左子树=null, 不push
//       predStk = [2]
//  结果: [4, 3] ✓
// ============================================================
class Solution3 {
public:
    vector<int> closestKValues(TreeNode* root, double target, int k) {
        stack<TreeNode*> predStk; // <= target 的前驱路径
        stack<TreeNode*> succStk; // >  target 的后继路径
        
        // 初始化: 从根沿搜索路径分流
        initStacks(root, target, predStk, succStk);
        
        vector<int> result;
        for (int i = 0; i < k; i++) {
            if (predStk.empty() && succStk.empty()) break;
            
            if (succStk.empty() ||
                (!predStk.empty() &&
                 target - predStk.top()->val <= succStk.top()->val - target)) {
                // 前驱更近（或后继栈空）
                result.push_back(predStk.top()->val);
                movePred(predStk);
            } else {
                // 后继更近（或前驱栈空）
                result.push_back(succStk.top()->val);
                moveSucc(succStk);
            }
        }
        return result;
    }
    
private:
    void initStacks(TreeNode* root, double target,
                    stack<TreeNode*>& predStk, stack<TreeNode*>& succStk) {
        TreeNode* cur = root;
        while (cur) {
            if (cur->val <= target) {
                predStk.push(cur);  // 潜在前驱
                cur = cur->right;   // 往右找更大(更接近target)的前驱
            } else {
                succStk.push(cur);  // 潜在后继
                cur = cur->left;    // 往左找更小(更接近target)的后继
            }
        }
    }
    
    // 前驱栈推进: 找"下一个更小的值"
    // 相当于中序遍历的逆向(反向中序迭代器的next)
    // 弹出当前栈顶后，进入它的左子树，然后一路向右到底
    void movePred(stack<TreeNode*>& stk) {
        TreeNode* node = stk.top()->left;
        stk.pop();
        // 左子树中最右的节点 = 左子树中最大值 = 下一个前驱
        while (node) {
            stk.push(node);
            node = node->right;
        }
    }
    
    // 后继栈推进: 找"下一个更大的值"
    // 相当于标准中序迭代器的next (和LeetCode 173完全一致)
    // 弹出当前栈顶后，进入它的右子树，然后一路向左到底
    void moveSucc(stack<TreeNode*>& stk) {
        TreeNode* node = stk.top()->right;
        stk.pop();
        // 右子树中最左的节点 = 右子树中最小值 = 下一个后继
        while (node) {
            stk.push(node);
            node = node->left;
        }
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 核心思想 | 适用场景 |
// |------|------|------|----------|----------|
// | 解法1(堆) | O(n log k) | O(n+k) | 堆维护topK | 通用，不依赖BST |
// | 解法2(窗口) | O(n)最坏 | O(k) | 中序有序+滑动窗口 | BST, 面试首选 |
// | 解法3(双栈) | O(k+logn) | O(logn) | 前驱后继迭代器 | 平衡BST, k<<n |
//
// 解法1→解法2: 利用中序有序性，用O(1)的deque操作替代O(logk)的堆操作
// 解法2→解法3: 不遍历全部n个节点，只定位+扩展k步
//
// 【易错点】
// 1. 解法2中比较对象搞错:
//    ✗ abs(cur->val-target) < abs(dq.back()-target)  ← 和尾部比
//    ✓ abs(cur->val-target) < abs(dq.front()-target) ← 和头部比
//    原因: 中序递增，队列头是最小值（离target左侧最远），应该被替换
//
// 2. 解法3 initStacks 中 <= 和 > 的边界:
//    ✗ val < target 入predStk → 等于target的节点去了succStk
//    ✓ val <= target 入predStk → 相等时归入前驱侧，和比较时的<=一致
//
// 3. 解法3 movePred/moveSucc 方向写反:
//    movePred: 弹出后走 left 子树，然后一路 right（找左子树最大）
//    moveSucc: 弹出后走 right 子树，然后一路 left（找右子树最小）
//    很容易在while循环中把left/right写反
//
// 4. 解法2 提前终止忘写break:
//    ✗ 省略else中的break → 遍历完所有节点，退化为O(n)且无优势
//    ✓ else { break; } → 利用有序性提前终止，这是比解法1快的关键
//
// 5. deque.size() 返回 size_t (无符号), 和 int k 比较时:
//    ✗ dq.size() < k 当 k 为负数时有 bug（虽然题目保证k>=1）
//    ✓ (int)dq.size() < k 显式转换更安全
//
// 【面试追问】
// Q1: 暴力解时间复杂度？瓶颈在哪？
//     → O(n log n) 排序。瓶颈: 没利用BST有序性，把结构信息丢了。
//
// Q2: 中序+滑动窗口为什么能提前终止？
//     → 中序有序递增，窗口头是最远的。新值更远说明后续都更远。
//     → 类比: 有序数组 [1,2,3,4,5] 找离3.7最近2个，
//       从左扫到5时发现5比窗口头3更远，后面6,7...只会更远。
//
// Q3: Follow-up: 如何做到 O(k + log n)?
//     → 双栈前驱后继迭代器。初始化O(logn)定位，每步O(1)均摊扩展。
//     → 本质: BST版的"在有序数组上从target位置向两侧双指针扩展"。
//
// Q4: 如果树不平衡，双栈法还有效吗？
//     → 初始化退化为O(n)（树高=n），优势消失。
//     → 此时解法2更实用，因为常数因子小且实现简单。
//
// Q5: 如果需要多次查询不同的target（同一棵树），如何优化？
//     → 预处理: 中序遍历一次存为有序数组O(n)，每次查询用二分+双指针O(logn+k)。
//     → 或者: 对平衡BST，每次用双栈法O(k+logn)。
// ============================================================
