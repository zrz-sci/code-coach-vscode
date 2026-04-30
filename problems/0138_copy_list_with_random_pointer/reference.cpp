/*
 * LeetCode 138: 随机链表的复制 (Copy List with Random Pointer)
 *
 * 【题目本质】
 * 深拷贝一个带 random 指针的链表。难点在于 random 指向的节点可能还未创建，
 * 核心问题是"如何建立原节点到新节点的映射"。
 *
 * 【解法总览】
 * 解法1: 暴力(数位置)    — O(n²) / O(1)   — 最直觉但慢
 * 解法2: 哈希表映射       — O(n)  / O(n)   — 面试首选 ⭐
 * 解法3: 节点编织(交错)   — O(n)  / O(1)   — 空间最优，面试加分
 * 解法4: 递归+哈希表      — O(n)  / O(n)   — DFS思维，适合图的拓展
 */

/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* next;
    Node* random;
    
    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};
*/

// ============================================================
// 解法1: 暴力 — 通过"数位置"建立映射
// 时间: O(n²)  空间: O(1)（不算输出）
//
// 【思路】
// 最朴素的想法：先按 next 复制一遍链表（不管 random），
// 然后对每个节点，在原链表中数出 random 指向的是第几个节点，
// 再到新链表中找到对应位置的节点。
//
// 瓶颈：对每个节点找 random 位置需要 O(n)，总共 O(n²)。
//
// 链表示意:
//   原: [7] → [13] → [11] → [10] → [1] → null
//         |      |      |      |      |
//   rand: null    ↓      ↓      ↓      ↓
//               [7]    [1]   [11]   [7]
//
//   复制 next 后:
//   新: [7'] → [13'] → [11'] → [10'] → [1'] → null
//   
//   对 13': 原链表中 13.random = 7，是第0个节点
//           → 新链表中第0个 = 7' → 13'.random = 7'
// ============================================================
class Solution1 {
public:
    Node* copyRandomList(Node* head) {
        if (!head) return nullptr;
        
        // 第一步：复制所有节点，只连 next
        Node* newHead = new Node(head->val);
        Node* oldCur = head->next;
        Node* newCur = newHead;
        while (oldCur) {
            newCur->next = new Node(oldCur->val);
            newCur = newCur->next;
            oldCur = oldCur->next;
        }
        
        // 第二步：对每个节点，找 random 指向的位置索引，再到新链表中定位
        oldCur = head;
        newCur = newHead;
        while (oldCur) {
            if (oldCur->random) {
                // 在原链表中找到 oldCur->random 的索引
                int idx = 0;
                Node* tmp = head;
                while (tmp != oldCur->random) {
                    tmp = tmp->next;
                    idx++;
                }
                // 在新链表中走到第 idx 个位置
                Node* target = newHead;
                for (int i = 0; i < idx; i++) {
                    target = target->next;
                }
                newCur->random = target;
            }
            // random 为 null 时新节点的 random 默认就是 nullptr，不用处理
            oldCur = oldCur->next;
            newCur = newCur->next;
        }
        return newHead;
    }
};

// ============================================================
// 解法2: 哈希表映射 — 面试首选 ⭐
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 解法1的瓶颈在于"给定原节点，无法快速找到对应的新节点"。
// 用哈希表 map<Node*, Node*> 存储「原节点 → 新节点」的映射，
// 查找从 O(n) 降到 O(1)。
//
// 分两遍遍历：
//   第一遍：创建所有新节点，建立映射（不管 next/random）
//   第二遍：利用映射设置 next 和 random
//
// 为什么分两遍？因为设置 random 时，目标新节点必须已经存在。
// 如果合并成一遍，random 指向的节点可能还没创建。
//
// 映射示意:
//   map = {
//     原[7]  → 新[7'],
//     原[13] → 新[13'],
//     原[11] → 新[11'],
//     原[10] → 新[10'],
//     原[1]  → 新[1']
//   }
//   
//   设置 random 时: 
//     13'.random = map[13.random] = map[原[7]] = 新[7'] ✓
// ============================================================
class Solution2 {
public:
    Node* copyRandomList(Node* head) {
        if (!head) return nullptr;
        
        unordered_map<Node*, Node*> oldToNew;
        
        // 第一遍：创建所有新节点，建立映射
        Node* cur = head;
        while (cur) {
            oldToNew[cur] = new Node(cur->val);
            cur = cur->next;
        }
        
        // 第二遍：设置 next 和 random
        cur = head;
        while (cur) {
            // oldToNew[nullptr] 在 unordered_map 中返回默认值 nullptr
            // 所以当 cur->next 或 cur->random 为 null 时自动处理正确
            oldToNew[cur]->next = oldToNew[cur->next];
            oldToNew[cur]->random = oldToNew[cur->random];
            cur = cur->next;
        }
        
        return oldToNew[head];
    }
};

// ============================================================
// 解法3: 节点编织（交错插入）— 空间最优
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 不想用哈希表？需要另一种方式快速找到"原节点的副本"。
// 巧妙做法：把新节点直接插到原节点后面！
// 这样 old.next 就是 old 的副本，天然完成了 O(1) 映射。
//
// 三步走：
//
// Step 1: 编织 — 在每个原节点后插入副本
//   原: A → B → C → null
//   后: A → A' → B → B' → C → C' → null
//
// Step 2: 设置 random
//   关键公式: A'.random = A.random.next（因为任何原节点X的副本都是X.next）
//
//   例: A.random = C
//       A'.random = C.next = C'  ✓
//
// Step 3: 拆分 — 恢复原链表 + 提取新链表
//   A → A' → B → B' → C → C' → null
//   拆成:
//   A → B → C → null        (原链表恢复)
//   A' → B' → C' → null     (新链表)
//
// 完整过程示意（以示例1为例）:
//
// 原始:
//   [7] → [13] → [11] → [10] → [1] → null
//    |       |       |       |      |
//   null    [7]     [1]    [11]   [7]    (random 指向)
//
// Step1 编织后:
//   [7]→[7']→[13]→[13']→[11]→[11']→[10]→[10']→[1]→[1']→null
//
// Step2 设置 random:
//   7'.random  = 7.random 为 null → null
//   13'.random = 13.random.next = [7].next = [7']  ✓
//   11'.random = 11.random.next = [1].next = [1']  ✓
//   10'.random = 10.random.next = [11].next = [11'] ✓
//   1'.random  = 1.random.next  = [7].next = [7']  ✓
//
// Step3 拆分:
//   原: [7]→[13]→[11]→[10]→[1]→null    (恢复)
//   新: [7']→[13']→[11']→[10']→[1']→null (结果)
// ============================================================
class Solution3 {
public:
    Node* copyRandomList(Node* head) {
        if (!head) return nullptr;
        
        // ===== Step 1: 在每个原节点后面插入副本 =====
        Node* cur = head;
        while (cur) {
            Node* copy = new Node(cur->val);
            copy->next = cur->next;     // copy 接上原来的下一个节点
            cur->next = copy;            // 原节点指向 copy
            cur = copy->next;            // 跳到下一个原节点
        }
        
        // ===== Step 2: 设置副本节点的 random =====
        cur = head;
        while (cur) {
            Node* copy = cur->next;      // cur 的副本紧跟其后
            // 核心: 如果 cur.random = X，那么 X 的副本是 X.next
            copy->random = cur->random ? cur->random->next : nullptr;
            cur = copy->next;            // 跳到下一个原节点
        }
        
        // ===== Step 3: 拆分为原链表和新链表 =====
        Node* newHead = head->next;      // 新链表的头
        cur = head;
        while (cur) {
            Node* copy = cur->next;
            cur->next = copy->next;      // 恢复原链表: 跳过 copy
            // 新链表: copy->next 应指向下一个副本节点
            // 下一个副本是 copy->next（原节点）的 next
            copy->next = copy->next ? copy->next->next : nullptr;
            cur = cur->next;             // 移到下一个原节点
        }
        
        return newHead;
    }
};

// ============================================================
// 解法4: 递归 + 哈希表 — DFS 思维
// 时间: O(n)  空间: O(n)（哈希表 + 递归栈）
//
// 【思路】
// 用递归的方式思考：copy(node) 返回 node 的深拷贝。
// 如果 node 已经被拷贝过（在 map 中），直接返回副本。
// 否则创建新节点，放入 map，递归设置 next 和 random。
//
// 本质是 DFS + 记忆化，和"克隆图"(LeetCode 133) 思路一致。
// 好处是代码简洁；坏处是递归栈空间 O(n)。
//
// 递归展开过程:
//   copy(7) → 创建 7', 递归 copy(13) 和 copy(null)
//     copy(13) → 创建 13', 递归 copy(11) 和 copy(7)
//       copy(11) → 创建 11', 递归 copy(10) 和 copy(1)
//         copy(10) → 创建 10', 递归 copy(1) 和 copy(11)
//           copy(1) → 创建 1', 递归 copy(null) 和 copy(7)
//             copy(null) → return null
//             copy(7) → 已在 map 中，直接返回 7'
//           copy(11) → 已在 map 中，直接返回 11'
//         ...全部回溯完成
// ============================================================
class Solution4 {
public:
    unordered_map<Node*, Node*> visited;
    
    Node* copyRandomList(Node* head) {
        if (!head) return nullptr;
        
        // 如果已经拷贝过，直接返回（避免无限递归）
        if (visited.count(head)) {
            return visited[head];
        }
        
        // 创建新节点并立即放入 map（在递归之前！防止环形引用）
        Node* copy = new Node(head->val);
        visited[head] = copy;
        
        // 递归设置 next 和 random
        copy->next = copyRandomList(head->next);
        copy->random = copyRandomList(head->random);
        
        return copy;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法            | 时间   | 空间   | 特点                          |
// |-----------------|--------|--------|-------------------------------|
// | 暴力(数位置)     | O(n²)  | O(1)   | 简单但慢                      |
// | 哈希表(迭代)     | O(n)   | O(n)   | 简洁易懂，面试首选 ⭐          |
// | 节点编织         | O(n)   | O(1)   | 最优空间，追问时给出           |
// | 递归+哈希表      | O(n)   | O(n)   | DFS思维，可推广到图的深拷贝    |
//
// 面试建议：
// - 先写解法2（哈希表），正确率高，代码简洁
// - 面试官问"能否O(1)空间"→ 给解法3（编织法）
// - 面试官问"如果是图怎么办"→ 用解法4的递归思路推广
//
// ============================================================
// 【易错点】
//
// 1. 编织法 Step3 拆分时访问 null->next:
//    ✗ copy->next = copy->next->next;
//    ✓ copy->next = copy->next ? copy->next->next : nullptr;
//    原因：最后一个副本节点的 copy->next 可能为 null
//
// 2. 哈希表解法利用 map[nullptr] 返回 nullptr 的特性:
//    这在 C++ unordered_map 中可行（默认插入 {nullptr, nullptr}），
//    但在其他语言中不一定安全。显式写法更稳妥：
//    ✗ oldToNew[cur]->random = oldToNew[cur->random];  // 依赖默认行为
//    ✓ oldToNew[cur]->random = cur->random ? oldToNew[cur->random] : nullptr;
//
// 3. 递归解法中必须在递归之前就把新节点放入 map:
//    ✗ 先递归 next/random，再放入 map → random 可能回指自己，无限递归
//    ✓ 创建节点后立即 visited[head] = copy，再递归
//
// 4. 编织法 Step2 中忘记判空 cur->random:
//    ✗ copy->random = cur->random->next;  // cur->random 为 null 时崩溃
//    ✓ copy->random = cur->random ? cur->random->next : nullptr;
//
// 5. 编织法结束后忘记恢复原链表:
//    面试中修改输入而不恢复，会被认为是不好的编程习惯
//
// ============================================================
// 【面试追问】
//
// Q1: 最直接的方式是什么？
//     → 哈希表，两遍遍历。第一遍建映射，第二遍连指针。O(n)/O(n)。
//
// Q2: 能否 O(1) 额外空间？
//     → 编织法：新节点插原节点后面，利用位置关系设 random，最后拆分。
//        必须恢复原链表。三步走，每步都是 O(n)。
//
// Q3: 如果是任意图的深拷贝（不是链表）？
//     → 用哈希表 + BFS/DFS。核心相同：map<原→新>。
//        编织法不适用于图（没有线性 next 结构）。
//        参考 LeetCode 133 克隆图。
//
// Q4: 递归解法如果链表很长会怎样？
//     → 栈溢出。链表长度可达 1000，一般没问题。
//        但对于极长链表，迭代解法更安全。
//
// 【相关题型】
// - 133. 克隆图 — 复用"哈希表映射 + BFS/DFS"，区别：图的邻居是列表，需要遍历
// - 141/142. 环形链表 — 链表指针操作基础训练
// - 382/398. 蓄水池抽样 — 同为链表上的随机相关问题，但算法完全不同
// ============================================================
