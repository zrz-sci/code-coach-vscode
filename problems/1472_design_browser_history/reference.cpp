/*
 * LeetCode 1472: 设计浏览器历史记录 (Design Browser History)
 *
 * 【题目本质】
 * 设计一个线性历史记录，支持：在当前位置插入（并截断后续）、前移、后移。
 * 核心在于 visit 时"清除前进历史"的高效实现。
 *
 * 【解法总览】
 * 解法1: 数组 + 双指针   — visit O(1), back O(1), forward O(1) / O(n) — 最优，面试首选
 * 解法2: 双栈            — visit O(1), back O(steps), forward O(steps) / O(n) — 经典栈思维
 * 解法3: 双向链表        — visit O(1), back O(steps), forward O(steps) / O(n) — 展示链表功底
 */

// ============================================================
// 解法1: 数组 + 双指针 — 所有操作 O(1)
// 时间: visit O(1), back O(1), forward O(1)
// 空间: O(n)，n 为总访问页面数
//
// 【思路】
// 浏览历史本质是一个线性列表 + 当前位置指针。
// 用数组存所有页面，cur 记录当前位置，end_ 记录有效历史的末尾。
//
// visit 时：cur 后移一位，写入新 url，end_ = cur（逻辑截断前进历史）
// back 时：cur 左移 steps 步，但不低于 0
// forward 时：cur 右移 steps 步，但不超过 end_
//
// 为什么 end_ = cur 就能"清除前进历史"？
// 因为 forward 受 end_ 限制，cur 永远不会超过 end_，
// 所以 end_ 后面的数据即使还在数组里，也永远不会被访问到。
//
// 状态变化示意:
//
// 初始:      [leetcode]                  cur=0, end=0
// visit(g):  [leetcode, google]          cur=1, end=1
// visit(f):  [leetcode, google, fb]      cur=2, end=2
// visit(y):  [leetcode, google, fb, yt]  cur=3, end=3
// back(1):   cur=2 → "fb"
// back(1):   cur=1 → "google"
// forward(1): cur=2 → "fb"
// visit(ln): [leetcode, google, fb, ln]  cur=3, end=3
//            ↑ yt 被覆盖了
// forward(2): cur = min(3, 3+2) = 3 → "ln" (无法前进)
// back(2):   cur = max(0, 3-2) = 1 → "google"
// back(7):   cur = max(0, 1-7) = 0 → "leetcode"
// ============================================================
class Solution1 {
public:
    class BrowserHistory {
        vector<string> history;
        int cur;   // 当前页面在数组中的下标
        int end_;  // 有效历史的末尾下标
    public:
        BrowserHistory(string homepage) {
            history.push_back(homepage);
            cur = 0;
            end_ = 0;
        }
        
        void visit(string url) {
            cur++;
            // 为什么要判断？因为如果之前 back 过再 visit，
            // cur 可能还在数组范围内，直接覆盖即可
            if (cur < (int)history.size()) {
                history[cur] = url;
            } else {
                history.push_back(url);
            }
            // 关键：逻辑截断前进历史
            // 不需要真的删除后面的元素，因为 forward 受 end_ 限制
            end_ = cur;
        }
        
        string back(int steps) {
            // 最多退到下标 0（homepage）
            cur = max(0, cur - steps);
            return history[cur];
        }
        
        string forward(int steps) {
            // 最多进到 end_（有效历史末尾）
            cur = min(end_, cur + steps);
            return history[cur];
        }
    };
};

// ============================================================
// 解法2: 双栈 — 后退栈 + 前进栈
// 时间: visit O(1)均摊, back O(steps), forward O(steps)
// 空间: O(n)
//
// 【思路】
// 从解法1换个角度：不用数组 + 指针，而是用两个栈模拟"后退"和"前进"。
//
// backStack: 存放当前页面之前的所有页面（栈顶是最近的）
// forwardStack: 存放当前页面之后的所有页面（栈顶是最近的）
// current: 当前正在浏览的页面
//
// visit: 把 current 压入 backStack，current = 新url，清空 forwardStack
// back:  把 current 压入 forwardStack，从 backStack 弹出作为新 current
// forward: 把 current 压入 backStack，从 forwardStack 弹出作为新 current
//
// 与解法1对比：
// - 优点：概念更直观，"两个方向各一个栈"
// - 缺点：back/forward 每步都要做 push/pop，不是 O(1)
//
// 状态示意:
// 
// backStack      current      forwardStack
// []             "leetcode"   []
// 
// visit("google"):
// [leetcode]     "google"     []    ← forwardStack 被清空
//
// visit("facebook"):
// [leet, google] "facebook"   []
//
// back(1):
// [leetcode]     "google"     [facebook]
//                ↑弹出google   ↑facebook压入
// ============================================================
class Solution2 {
public:
    class BrowserHistory {
        stack<string> backStack;
        stack<string> forwardStack;
        string current;
    public:
        BrowserHistory(string homepage) {
            current = homepage;
        }
        
        void visit(string url) {
            // 当前页面成为"后退历史"的一部分
            backStack.push(current);
            current = url;
            // 题目要求：visit 清除所有前进历史
            // 为什么用 while 而不是 forwardStack = stack<string>()？
            // 两种写法都可以，赋值新栈更简洁但可能有额外开销
            while (!forwardStack.empty()) forwardStack.pop();
        }
        
        string back(int steps) {
            // 每后退一步：current → forwardStack，backStack.top() → current
            while (steps > 0 && !backStack.empty()) {
                forwardStack.push(current);
                current = backStack.top();
                backStack.pop();
                steps--;
            }
            return current;
        }
        
        string forward(int steps) {
            // 每前进一步：current → backStack，forwardStack.top() → current
            while (steps > 0 && !forwardStack.empty()) {
                backStack.push(current);
                current = forwardStack.top();
                forwardStack.pop();
                steps--;
            }
            return current;
        }
    };
};

// ============================================================
// 解法3: 双向链表 — 指针移动
// 时间: visit O(1), back O(steps), forward O(steps)
// 空间: O(n)
//
// 【思路】
// 链表天然支持"在某个位置后面插入并截断后续"，
// 这正好对应 visit 的语义：在当前页面后插入新页面，丢弃前进历史。
//
// 链表结构示意:
//
// visit 操作前:
//   leetcode ↔ google ↔ facebook ↔ youtube
//                         ↑ current
//
// visit("linkedin") 后:
//   leetcode ↔ google ↔ facebook ↔ linkedin
//                                    ↑ current
//   (youtube 被截断，next 指针指向 linkedin 而非 youtube)
//
// back(2):
//   leetcode ↔ google ↔ facebook ↔ linkedin
//     ↑ current (从 linkedin 左移2步到 google)
//
// 与解法1对比：
// - visit 的截断更自然（修改指针），但 back/forward 仍需逐步移动
// - 有内存泄漏问题（被截断的节点没有释放）
// - 代码复杂度最高，面试中一般不作为首选
// ============================================================
class Solution3 {
public:
    class BrowserHistory {
        struct Node {
            string url;
            Node* prev;
            Node* next;
            Node(string u) : url(u), prev(nullptr), next(nullptr) {}
        };
        Node* current;
        
    public:
        BrowserHistory(string homepage) {
            current = new Node(homepage);
        }
        
        void visit(string url) {
            Node* newNode = new Node(url);
            // 截断前进历史：current 的 next 直接指向新节点
            // 注意：这里旧的 next 链没有被 delete，存在内存泄漏
            // 生产代码应该遍历旧 next 链逐个释放
            current->next = newNode;
            newNode->prev = current;
            current = newNode;
        }
        
        string back(int steps) {
            // 沿 prev 指针移动，直到走了 steps 步或到达链表头
            while (steps > 0 && current->prev) {
                current = current->prev;
                steps--;
            }
            return current->url;
        }
        
        string forward(int steps) {
            // 沿 next 指针移动，直到走了 steps 步或到达链表尾
            while (steps > 0 && current->next) {
                current = current->next;
                steps--;
            }
            return current->url;
        }
        
        // 析构函数：应该从头节点开始逐个释放所有节点
        // 面试中可以口头说明，不必完整实现
        ~BrowserHistory() {
            // 先找到头节点
            Node* head = current;
            while (head->prev) head = head->prev;
            // 逐个释放
            while (head) {
                Node* next = head->next;
                delete head;
                head = next;
            }
        }
    };
};

// ============================================================
// 【解法对比】
//
// | 操作    | 数组+双指针 | 双栈         | 双向链表      |
// |---------|-------------|-------------|---------------|
// | visit   | O(1)        | O(1) 均摊   | O(1)          |
// | back    | O(1) ⭐     | O(steps)    | O(steps)      |
// | forward | O(1) ⭐     | O(steps)    | O(steps)      |
// | 代码量  | 最少        | 中等         | 最多          |
// | 内存管理| 无问题      | 无问题       | 需要手动释放  |
//
// 面试首选解法1（数组+双指针），所有操作 O(1)，代码简洁。
// 解法2展示对栈的理解，解法3展示链表功底，作为备选方案。
//
// ============================================================
// 【易错点】
//
// 1. 解法1中 visit 忘记更新 end_:
//    ✗ history[++cur] = url;  // 没有 end_ = cur
//    ✓ history[++cur] = url; end_ = cur;
//    后果：forward 可能走到已被覆盖/废弃的旧页面
//
// 2. 解法1中 visit 时 cur 超出数组范围却直接赋值:
//    ✗ cur++; history[cur] = url;  // 如果 cur == history.size()，越界！
//    ✓ cur++; if (cur < history.size()) history[cur] = url;
//             else history.push_back(url);
//
// 3. back 时 cur 减成负数:
//    ✗ cur -= steps; return history[cur];  // 负下标，未定义行为
//    ✓ cur = max(0, cur - steps);
//
// 4. 解法2中 visit 后忘记清空 forwardStack:
//    ✗ backStack.push(current); current = url;  // forwardStack 还有旧数据！
//    ✓ 必须加 while (!forwardStack.empty()) forwardStack.pop();
//
// 5. 解法3中 visit 截断后没考虑内存:
//    面试中主动提到"这里有内存泄漏，生产代码需要释放旧链"是加分项
//
// ============================================================
// 【面试追问】
//
// Q1(基础理解): 为什么数组方案的 back/forward 能做到 O(1)?
//    → 因为只需要移动一个整数指针(cur)，不需要移动数据。
//      而双栈方案需要逐个 pop/push，所以是 O(steps)。
//
// Q2(优化): 如果 visit 时需要真的释放前进历史的内存呢?
//    → 数组方案可以 resize(cur+1) 或 erase 后面的元素，但这变成 O(n)。
//      通常不需要，因为逻辑删除(end_=cur)已经够了。
//      如果内存敏感，可以用链表方案并在截断时遍历释放。
//
// Q3(变体): 如果历史记录有最大长度限制 N 怎么办?
//    → 用环形数组(circular buffer)。start 和 end 用取模运算。
//      当历史超过 N 条时，start 后移，覆盖最老的记录。
//      这类似 LRU 缓存的思想。
//
// Q4(设计): 如果要支持"查看第 k 条历史记录"这样的随机访问?
//    → 数组方案天然支持 O(1) 随机访问 history[k]。
//      双栈和链表需要额外的索引结构。
// ============================================================
