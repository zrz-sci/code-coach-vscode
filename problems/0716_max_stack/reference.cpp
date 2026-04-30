/*
 * LeetCode 716: Max Stack (最大栈)
 * 
 * 【题目本质】
 * 设计一个栈，除了常规 push/pop/top，还要支持 peekMax（查看最大值）
 * 和 popMax（删除最大值，多个相同最大值时删最靠近栈顶的）。
 * 难点：popMax 需要从栈的"中间"删除元素。
 *
 * 【解法总览】
 * 解法1: 双栈法          — push/pop/top/peekMax O(1), popMax O(n) — 最直觉
 * 解法2: 双向链表+有序映射 — top O(1), 其他 O(log n)            — 最优(满足Follow-up)
 */

// ============================================================
// 解法1: 双栈法 — 类比 155. Min Stack，额外处理 popMax
// 时间: push/pop/top/peekMax O(1), popMax O(n)
// 空间: O(n)
//
// 【思路】
// 从 155. Min Stack 出发：用一个辅助栈 maxStk，其中 maxStk[i] 记录
// 前 i 个元素中的最大值。这样 peekMax = maxStk.top()，O(1)。
//
// 但 popMax 怎么办？辅助栈只能告诉我们最大值是多少，不能直接删。
// 只能暴力：逐个弹出栈顶元素到临时栈，直到找到最大值，弹出它，
// 然后把临时栈的元素放回去（放回时要复用 push 重新维护 maxStk）。
//
// 示意图:
//   stk:    [5, 1, 5]   ← 栈顶是 5
//   maxStk: [5, 5, 5]   ← peekMax = 5
//
//   popMax 过程:
//   Step1: stk.top()=5 == maxVal=5 → 找到了！弹出
//          stk:    [5, 1]
//          maxStk: [5, 5]
//   结果: 返回 5，栈变为 [5, 1]
//
//   如果最大值在中间:
//   stk:    [5, 3, 1]   maxStk: [5, 5, 5]   maxVal=5
//   Step1: top=1 != 5 → 弹到 tmp，stk=[5,3], maxStk=[5,5]
//   Step2: top=3 != 5 → 弹到 tmp，stk=[5],   maxStk=[5]
//   Step3: top=5 == 5 → 弹出！    stk=[],     maxStk=[]
//   Step4: 放回 tmp: push(3) → stk=[3], maxStk=[3]
//          push(1) → stk=[3,1], maxStk=[3,3]
// ============================================================
class MaxStack1 {
    stack<int> stk;
    stack<int> maxStk;
public:
    MaxStack1() {}
    
    void push(int x) {
        stk.push(x);
        // 每次 push 都同步更新 maxStk
        // 用 >= 保证重复最大值也被记录
        int curMax = maxStk.empty() ? x : max(maxStk.top(), x);
        maxStk.push(curMax);
    }
    
    int pop() {
        int val = stk.top();
        stk.pop();
        maxStk.pop(); // 同步弹出
        return val;
    }
    
    int top() {
        return stk.top();
    }
    
    int peekMax() {
        return maxStk.top();
    }
    
    int popMax() {
        int maxVal = maxStk.top();
        stack<int> tmp;
        // 逐个弹出，直到找到最大值
        while (stk.top() != maxVal) {
            tmp.push(stk.top());
            stk.pop();
            maxStk.pop();
        }
        // 弹出最大值本身
        stk.pop();
        maxStk.pop();
        // 把临时栈的元素放回（必须复用 push 来维护 maxStk）
        while (!tmp.empty()) {
            push(tmp.top());
            tmp.pop();
        }
        return maxVal;
    }
};

// ============================================================
// 解法2: 双向链表 + 有序映射 — 满足 Follow-up 的 O(log n) 要求
// 时间: top O(1), push/pop/peekMax/popMax O(log n)
// 空间: O(n)
//
// 【思路】
// 解法1的瓶颈：popMax 要线性扫描找最大值并重建栈。
// 需要两个能力来突破：
//   1. 快速找到最大值 → std::map 有序，rbegin() 是最大 key，O(log n)
//   2. 快速从栈中间删除元素 → 双向链表，已知迭代器可 O(1) 删除
//
// 数据结构设计:
//   - list<int> dll: 双向链表，back() = 栈顶
//   - map<int, vector<list<int>::iterator>> mp:
//     key = 元素值
//     value = 该值在链表中所有出现位置的迭代器列表（按插入顺序）
//             最后一个 = 最靠近栈顶的
//
// 示意图:
//   push(5), push(1), push(5) 后:
//
//   dll (双向链表，从左到右是栈底→栈顶):
//     [5] ↔ [1] ↔ [5]
//      ↑              ↑
//      it_a           it_b    (两个迭代器)
//
//   mp (有序映射):
//     1 → [ it_1 ]           (指向链表中的 [1])
//     5 → [ it_a, it_b ]    (it_a 先插入 = 更靠近栈底,
//                              it_b 后插入 = 更靠近栈顶)
//
//   popMax:
//     mp.rbegin() → key=5
//     取 mp[5].back() = it_b (最靠近栈顶的 5)
//     dll.erase(it_b) → 链表变为 [5] ↔ [1]
//     mp[5].pop_back() → mp[5] = [it_a]
//     返回 5
// ============================================================
class MaxStack {
    // 双向链表模拟栈，back() 是栈顶
    list<int> dll;
    // 有序映射：值 → 链表中该值所有节点的迭代器列表
    // vector 中越靠后的迭代器，对应越靠近栈顶的节点
    map<int, vector<list<int>::iterator>> mp;
    
public:
    MaxStack() {}
    
    void push(int x) {
        dll.push_back(x);
        // prev(dll.end()) 是刚插入节点的迭代器
        // 不能用 dll.end()，那是 past-the-end，不是有效节点！
        mp[x].push_back(prev(dll.end()));
    }
    
    int pop() {
        // 栈顶 = 链表尾部
        int val = dll.back();
        dll.pop_back();
        
        // 从 map 中移除：该值最后一个迭代器就是刚弹出的节点
        mp[val].pop_back();
        // 如果该值不再有节点，从 map 中彻底删除
        // 否则 rbegin() 可能指向一个空 vector 的 key
        if (mp[val].empty()) mp.erase(val);
        
        return val;
    }
    
    int top() {
        // O(1)，链表尾部
        return dll.back();
    }
    
    int peekMax() {
        // map 有序，rbegin() 的 key 就是最大值
        return mp.rbegin()->first;
    }
    
    int popMax() {
        // 找到最大值
        int maxVal = mp.rbegin()->first;
        
        // 取最靠近栈顶的那个节点（vector 最后一个 = 最后插入 = 最靠近栈顶）
        auto it = mp[maxVal].back();
        mp[maxVal].pop_back();
        if (mp[maxVal].empty()) mp.erase(maxVal);
        
        // 从双向链表中 O(1) 删除
        // 这就是选择链表而非数组的核心原因
        dll.erase(it);
        
        return maxVal;
    }
};

// ============================================================
// 【解法对比】
//
// | 操作     | 解法1 (双栈)  | 解法2 (链表+map) |
// |----------|---------------|------------------|
// | push     | O(1)          | O(log n)         |
// | pop      | O(1)          | O(log n)         |
// | top      | O(1)          | O(1)             |
// | peekMax  | O(1)          | O(log n)         |
// | popMax   | O(n) ← 瓶颈  | O(log n) ✓       |
//
// 解法1 在 push/pop/top/peekMax 上更快（O(1)），但 popMax 是 O(n)。
// 解法2 牺牲了一点常数（都是 O(log n)），但 popMax 从 O(n) 降到 O(log n)。
//
// 面试中：先说解法1展示理解，然后分析瓶颈引出解法2。
//
// 【易错点】
// 1. 解法1: popMax 放回元素时必须复用 push() 而不是直接 stk.push() + maxStk.push()
//    ✗ stk.push(tmp.top()); maxStk.push(tmp.top()); — maxStk 没有正确维护历史最大值
//    ✓ push(tmp.top()); — 复用 push 自动计算 max(maxStk.top(), x)
//
// 2. 解法2: 获取新节点迭代器
//    ✗ mp[x].push_back(dll.end());        — end() 是哨兵，不是有效节点
//    ✓ mp[x].push_back(prev(dll.end()));  — prev(end()) 才是最后一个有效节点
//
// 3. 解法2: 忘记清理 map 中的空 vector
//    ✗ mp[val].pop_back(); // 没有检查是否为空
//    ✓ mp[val].pop_back(); if (mp[val].empty()) mp.erase(val);
//    不清理的后果: rbegin() 指向一个没有节点的 key，peekMax/popMax 返回错误值
//
// 4. 解法2: popMax 取 vector 的 front() 而非 back()
//    ✗ auto it = mp[maxVal].front(); — 这是最先插入的(最靠近栈底)，不是题意
//    ✓ auto it = mp[maxVal].back();  — 最后插入的 = 最靠近栈顶 = 题意要求
//
// 【面试追问】
// Q1: 只需要 peekMax 不需要 popMax，怎么简化？
//     → 退化为 155. Min Stack 的镜像。辅助栈记录历史最大值即可，所有操作 O(1)。
//
// Q2: 为什么选双向链表而不是 vector/deque？
//     → vector 中间删除是 O(n)（要移动后续元素），双向链表中间删除是 O(1)
//        （只改前后节点的指针）。popMax 的核心需求就是"知道位置后 O(1) 删除"。
//
// Q3: map 中 value 用 vector 会不会有问题？pop_back 是 O(1)，但如果要删
//     中间的迭代器呢？
//     → 本题中只会删 vector 的最后一个元素（最靠近栈顶），所以 pop_back 够用。
//        如果需要删中间元素（比如支持 removeByIndex），可以改用 list<iterator>，
//        但本题不需要。
//
// Q4: 能否用 multimap 替代 map<int, vector<iterator>>？
//     → multimap 可以存重复 key，但 popMax 需要删"最后插入的"同值元素，
//        multimap 的 equal_range 返回的范围中元素顺序不反映插入时间。
//        需要额外存时间戳来区分，不如当前方案直观。
// ============================================================
