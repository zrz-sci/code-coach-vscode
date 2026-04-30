/*
 * LeetCode 341: 扁平化嵌套列表迭代器 (Flatten Nested List Iterator)
 * 
 * 【题目本质】
 * 嵌套列表可以看作一棵多叉树，整数是叶子节点，列表是内部节点。
 * "扁平化迭代器" = 对这棵树进行先序遍历，逐个返回叶子节点值。
 * 
 * 【NestedInteger 接口说明 — 题目已提供，不需要我们实现】
 * class NestedInteger {
 *   public:
 *     bool isInteger() const;
 *     int getInteger() const;
 *     const vector<NestedInteger> &getList() const;
 * };
 *
 * 【解法总览】
 * 解法1: 预处理展平 — 构造O(n), next/hasNext O(1) — 最直觉
 * 解法2: 栈迭代器(惰性求值) — 均摊O(1) / O(d) — 面试首选
 *
 * 【嵌套列表的树形结构可视化】
 *
 *   示例1: [[1,1],2,[1,1]]
 *
 *              root
 *            /   |   \
 *         [1,1]  2  [1,1]
 *         / \        / \
 *        1   1      1   1
 *
 *   先序遍历叶子节点: 1, 1, 2, 1, 1
 *
 *   示例2: [1,[4,[6]]]
 *
 *              root
 *             /    \
 *            1    [4,[6]]
 *                 /    \
 *                4     [6]
 *                       |
 *                       6
 *
 *   先序遍历叶子节点: 1, 4, 6
 */

// ============================================================
// 解法1: 预处理展平 — 构造时递归 DFS 把所有整数放入数组
// 时间: 构造 O(n), next O(1), hasNext O(1)
// 空间: O(n)，n 为所有整数的总个数
//
// 【思路】
// 最直觉的做法：嵌套结构天然适合递归处理。
// 在构造函数里一次性递归遍历整个嵌套列表，
// 把遇到的每个整数按顺序放入 vector。
// 之后 next() 和 hasNext() 退化为数组下标操作。
//
// 优点：实现简单，正确性显而易见。
// 缺点：即使调用者只需要前几个元素，也要展开全部。
// ============================================================
class NestedIterator1 {
    vector<int> vals;
    int idx;
    
    // 递归展平：遇到整数就收集，遇到列表就递归进去
    void flatten(const vector<NestedInteger>& list) {
        for (const auto& ni : list) {
            if (ni.isInteger()) {
                vals.push_back(ni.getInteger());
            } else {
                // 递归展开子列表 — 就像 DFS 进入子树
                flatten(ni.getList());
            }
        }
    }
    
public:
    NestedIterator1(vector<NestedInteger>& nestedList) : idx(0) {
        flatten(nestedList);
    }
    
    int next() {
        // hasNext() 保证 idx 有效
        return vals[idx++];
    }
    
    bool hasNext() {
        return idx < (int)vals.size();
    }
};

// ============================================================
// 解法2: 栈模拟迭代器（惰性求值）— 面试首选 ⭐
// 时间: 均摊 O(1)（每个元素最多压栈/弹栈各一次）
// 空间: O(d)，d 为嵌套的最大深度
//
// 【思路】
// 解法1的问题：构造时就展开全部，不够"懒"。
// 面试官期望的是一个真正的迭代器 — 按需展开。
//
// 关键观察：递归 DFS 用的是系统调用栈来记住"我在哪一层、
// 遍历到第几个元素了"。我们可以用一个显式栈来模拟：
//   栈中每一层存 (cur迭代器, end迭代器)
//   表示"某个列表中，从 cur 到 end 还没处理"
//
// hasNext() 的逻辑（核心！）：
//   while 栈非空:
//     如果栈顶的 cur == end → 这一层遍历完了，弹栈
//     如果栈顶的 cur 是整数 → 找到了！返回 true
//     如果栈顶的 cur 是列表 → cur++（跳过），压入子列表的迭代器
//   栈空 → 返回 false
//
// next()：hasNext() 已保证栈顶 cur 指向整数，取值并 cur++
//
// 【栈的展开过程可视化 — 以 [1,[4,[6]]] 为例】
//
//  初始状态:
//    stack: [(cur→elem0, end→过elem2)]
//    其中 elem0=1, elem1=[4,[6]]
//
//  调用 hasNext():
//    栈顶 cur→1 (整数) → 返回 true
//  调用 next():
//    取出 1, cur 前进到 elem1
//
//  调用 hasNext():
//    栈顶 cur→[4,[6]] (列表!) 
//    → cur++ (跳过这个列表元素)
//    → 压入 [4,[6]] 的子迭代器: (cur→4, end→过[6])
//    → 新栈顶 cur→4 (整数) → 返回 true
//  调用 next():
//    取出 4, cur 前进到 [6]
//
//  调用 hasNext():
//    栈顶 cur→[6] (列表!)
//    → cur++ → 压入 [6] 的子迭代器: (cur→6, end→过6)
//    → 新栈顶 cur→6 (整数) → 返回 true
//  调用 next():
//    取出 6, cur 前进到 end
//
//  调用 hasNext():
//    栈顶 cur==end → 弹栈
//    下一层 cur==end → 弹栈  
//    最外层 cur==end → 弹栈
//    栈空 → 返回 false
// ============================================================
class NestedIterator2 {
    using Iter = vector<NestedInteger>::iterator;
    // 栈中每一帧: (当前位置, 结束位置)
    // 类比递归中的局部变量: 正在遍历哪个列表、遍历到第几个
    stack<pair<Iter, Iter>> stk;
    
public:
    NestedIterator2(vector<NestedInteger>& nestedList) {
        // 把最外层列表的遍历范围压栈
        stk.push({nestedList.begin(), nestedList.end()});
    }
    
    int next() {
        // 前置条件: hasNext() 已保证栈顶 cur 指向一个整数
        int val = stk.top().first->getInteger();
        // 取完值后前进，为下一次 hasNext() 做准备
        stk.top().first++;
        return val;
    }
    
    bool hasNext() {
        // 核心循环: 不断调整栈，直到栈顶 cur 指向整数 或 栈空
        while (!stk.empty()) {
            auto& [cur, end] = stk.top();
            
            if (cur == end) {
                // 当前层遍历完毕，返回上一层（类比递归返回）
                stk.pop();
                continue;
            }
            
            if (cur->isInteger()) {
                // 找到一个整数，可以被 next() 取走
                return true;
            }
            
            // cur 指向一个子列表，需要"递归"进入
            // 关键: 先保存子列表引用，再 cur++ 跳过当前元素
            // 如果顺序反了（先压栈再cur++），引用可能出问题
            auto& subList = cur->getList();
            cur++;  // ← 必须在压栈前！回到这层时要跳过已展开的子列表
            stk.push({subList.begin(), subList.end()});
            // 继续循环: 检查新压入的子列表的第一个元素
        }
        return false;  // 栈空，所有元素都遍历完了
    }
};

// ============================================================
// 【解法对比】
//
// | 维度           | 解法1 (预处理展平)  | 解法2 (栈迭代器)    |
// |----------------|---------------------|---------------------|
// | 展开时机       | 构造时全部展开      | 按需惰性展开        |
// | 空间           | O(n) 存全部整数     | O(d) 只存栈帧       |
// | 实现复杂度     | 简单                | 中等                |
// | 提前终止       | 浪费                | 高效                |
// | 面试推荐       | 保底方案            | ⭐ 首选方案          |
//
// 面试策略: 先口述解法1展示理解，然后主动说"但这不够lazy"，
// 引出解法2，展示设计迭代器的能力。
//
// ============================================================
// 【易错点】
//
// 1. cur++ 的时机:
//    ✗ stk.push({subList.begin(), subList.end()}); cur++;
//      → 先压栈后 cur++，如果子列表为空，弹栈后 cur 未前进，
//        再次看到同一个子列表 → 死循环
//    ✓ cur++; stk.push({subList.begin(), subList.end()});
//      → 先跳过，再压入子列表
//
// 2. hasNext() 里只 if 不 while:
//    ✗ if (cur == end) { stk.pop(); } if (cur->isInteger()) ...
//      → 遇到连续空列表 [[], [], 1] 只弹一层就返回错误结果
//    ✓ 用 while 循环持续处理，直到找到整数或栈空
//
// 3. next() 忘记前进:
//    ✗ return stk.top().first->getInteger();  // 没有 cur++
//      → 每次 next() 返回同一个值，hasNext() 永远 true
//    ✓ 取值后 stk.top().first++;
//
// 4. 引用悬垂:
//    ✗ auto& [cur, end] = stk.top(); stk.pop(); 
//       // pop 后 cur 和 end 是悬垂引用
//    ✓ pop 后立即 continue，不再访问 cur/end
//
// ============================================================
// 【面试追问】
//
// Q1 (基础): "解法1有什么缺点？什么时候不理想？"
//    → 构造时展开全部，如果列表很大但只需要前几个就浪费了。
//
// Q2 (优化): "解法2的 hasNext() 有 while 循环，时间复杂度真的是 O(1) 吗？"
//    → 单次可能 O(d)，但均摊 O(1)：每个 NestedInteger 最多被
//      压栈一次弹栈一次，遍历 n 个整数的总操作为 O(n)。
//
// Q3 (变体): "如果 getList() 返回拷贝而不是引用怎么办？"
//    → 迭代器会指向临时对象，失效。此时要么用解法1，
//      要么栈中存 NestedInteger 对象（或智能指针），
//      在 hasNext 中手动展开。
//
// Q4 (进阶): "能否让这个迭代器支持 peek() — 查看下一个但不消费？"
//    → 在 hasNext() 成功后，缓存 stk.top().first->getInteger()
//      到一个成员变量。peek() 返回这个缓存值，next() 也从缓存取
//      并清除缓存标志。
// ============================================================
