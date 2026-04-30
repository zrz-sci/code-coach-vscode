/*
 * LeetCode 2296: 设计一个文本编辑器 (Design a Text Editor)
 *
 * 【题目本质】
 * 光标把文本分成左右两部分，所有操作（增/删/移动）都发生在这个分界处。
 * 选择什么数据结构来高效操作这个"分界处"是关键。
 *
 * 【解法总览】
 * 解法1: 字符串模拟   — O(n) per op / O(n) — 最直觉，暴力
 * 解法2: 双栈(对顶栈) — O(k) per op / O(n) — 面试首选 ⭐
 * 解法3: 双向链表     — O(k) per op / O(n) — 追问链表时用
 */

// ============================================================
// 解法1: 字符串模拟 — 一个 string + 一个 int 光标位置
// 时间: O(n) 每次操作（insert/erase 在中间需要移动后续字符）
// 空间: O(n) 存储文本
//
// 【思路】
// 最直觉的做法：维护完整字符串和光标位置（整数）。
// addText 用 string::insert 在光标处插入。
// deleteText 用 string::erase 删除光标左边的字符。
// cursorLeft/Right 直接加减位置。
// 
// 瓶颈：insert 和 erase 在中间位置是 O(n) 的，因为要移动后面的字符。
// 本题数据范围小（总调用 2×10^4，每次 k ≤ 40）所以能过。
// ============================================================
class TextEditor1 {
    string text;
    int cursor;
public:
    TextEditor1() : cursor(0) {}

    void addText(string t) {
        // 在光标位置插入 — O(n) 因为后面的字符要整体右移
        text.insert(cursor, t);
        cursor += t.size();
    }

    int deleteText(int k) {
        // 最多删到开头
        int del = min(k, cursor);
        text.erase(cursor - del, del);  // O(n) 同理
        cursor -= del;
        return del;
    }

    string cursorLeft(int k) {
        cursor = max(0, cursor - k);
        int len = min(10, cursor);
        return text.substr(cursor - len, len);
    }

    string cursorRight(int k) {
        cursor = min((int)text.size(), cursor + k);
        int len = min(10, cursor);
        return text.substr(cursor - len, len);
    }
};


// ============================================================
// 解法2: 双栈 (对顶栈) — 用两个 string 当栈 ⭐ 面试首选
// 时间: O(k) 每次操作（addText 是 O(|text|)）
// 空间: O(n) 存储文本
//
// 【思路】
// 核心观察：光标把文本分成左右两部分。
// 如果我们用两个"栈"分别维护左半部分和右半部分，
// 那么所有操作都发生在栈顶，天然 O(1) 单字符操作。
//
//   左栈 (bottom→top)        右栈 (bottom→top)
//   [l, e, e, t]             [e, c, i, t, c, a, r, p]
//         ↑ 栈顶 = 光标左边第1个字符    ↑ 栈顶 = 光标右边第1个字符
//
//   完整文本: "leet|practice"  (| 是光标)
//
// addText("xy"):    左栈 push 'x', push 'y'
// deleteText(2):    左栈 pop 2次
// cursorLeft(1):    左栈 pop → 右栈 push  (光标左移)
// cursorRight(1):   右栈 pop → 左栈 push  (光标右移)
//
// C++ 的 string 天然支持 push_back/pop_back (均摊 O(1))，
// 所以直接用 string 当栈，还能方便地用 substr 取末尾10个字符。
// ============================================================
class TextEditor {
    string left, right;  // left 的末尾 = 栈顶; right 的末尾 = 栈顶

    // 返回光标左边 min(10, len) 个字符
    // left 的末尾是最靠近光标的，substr 从左到右刚好是正确顺序
    string getLeft() {
        int len = min(10, (int)left.size());
        return left.substr(left.size() - len, len);
    }

public:
    TextEditor() {}

    void addText(string text) {
        // 追加到左栈末尾 = 在光标位置插入文字
        // string 的 += 是均摊 O(|text|)
        left += text;
    }

    int deleteText(int k) {
        // 从左栈末尾弹出 min(k, left.size()) 个字符
        int del = min(k, (int)left.size());
        // 从末尾删除不需要移动其他字符，O(del)
        left.erase(left.size() - del, del);
        return del;
    }

    string cursorLeft(int k) {
        // 光标左移 = 左栈顶字符搬到右栈
        int moves = min(k, (int)left.size());
        for (int i = 0; i < moves; i++) {
            right.push_back(left.back());  // 搬到右栈
            left.pop_back();               // 从左栈移除
        }
        return getLeft();
    }

    string cursorRight(int k) {
        // 光标右移 = 右栈顶字符搬到左栈
        int moves = min(k, (int)right.size());
        for (int i = 0; i < moves; i++) {
            left.push_back(right.back());  // 搬到左栈
            right.pop_back();              // 从右栈移除
        }
        return getLeft();
    }
};

// ============================================================
// 示例验证 (双栈方案):
//
// 初始:     left=""          right=""        text: "|"
//
// addText("leetcode"):
//           left="leetcode"  right=""        text: "leetcode|"
//
// deleteText(4):  删4个
//           left="leet"      right=""        text: "leet|"
//           返回 4
//
// addText("practice"):
//           left="leetpractice"  right=""    text: "leetpractice|"
//
// cursorRight(3):  右栈为空，无法右移
//           left="leetpractice"  right=""    text: "leetpractice|"
//           getLeft() = "etpractice" (末尾10个字符)
//
// cursorLeft(8):  左栈顶8个字符搬到右栈
//           搬: e→右, c→右, i→右, t→右, c→右, a→右, r→右, p→右
//           left="leet"  right="practice"(反序存储为"ecitcarp")
//           text: "leet|practice"
//           getLeft() = "leet"
//
// deleteText(10):  左栈只有4个字符，删4个
//           left=""  right="ecitcarp"       text: "|practice"
//           返回 4
//
// cursorLeft(2):  左栈为空，无法左移
//           getLeft() = ""
//
// cursorRight(6):  右栈顶6个字符搬到左栈
//           搬: p→左, r→左, a→左, c→左, t→左, i→左
//           left="practi"  right="ec"       text: "practi|ce"
//           getLeft() = "practi"
// ============================================================


// ============================================================
// 解法3: 双向链表 — 光标是链表中的一个指针
// 时间: O(k) 每次操作（addText 是 O(|text|)）
// 空间: O(n) 每个字符一个节点
//
// 【思路】
// 另一种经典做法：每个字符是双向链表的一个节点。
// 光标用一个指针 cursor 表示，指向光标左边的节点。
// 用 dummy 哨兵节点处理"光标在最左边"的边界情况。
//
//   dummy ↔ 'l' ↔ 'e' ↔ 'e' ↔ 't' ↔ 'p' ↔ ...
//                              ↑
//                           cursor
//   表示: "leet|practice..."
//
// addText: 在 cursor 后面逐个插入节点，cursor 随之右移
// deleteText: 逐个删除 cursor 指向的节点，cursor 回退到前驱
// cursorLeft: cursor = cursor->prev （不能越过 dummy）
// cursorRight: cursor = cursor->next （不能越过末尾）
//
// 优点：插入/删除/移动都是纯指针操作 O(1)
// 缺点：每个字符一个 new Node，内存开销大且缓存不友好
// ============================================================
class TextEditor3 {
    struct Node {
        char ch;
        Node *prev, *next;
        Node(char c = 0) : ch(c), prev(nullptr), next(nullptr) {}
    };

    Node *dummy, *cursor;

    // 在 node 后插入新节点，返回新节点
    Node* insertAfter(Node* node, char c) {
        Node* nn = new Node(c);
        nn->next = node->next;
        nn->prev = node;
        if (node->next) node->next->prev = nn;
        node->next = nn;
        return nn;
    }

    // 删除 node（保证不是 dummy），返回 node->prev
    Node* removeNode(Node* node) {
        Node* p = node->prev;
        Node* n = node->next;
        p->next = n;
        if (n) n->prev = p;
        delete node;
        return p;
    }

    // 获取光标左边 min(10, len) 个字符
    string getLeft() {
        string res;
        Node* p = cursor;
        for (int i = 0; i < 10 && p != dummy; i++) {
            res += p->ch;
            p = p->prev;
        }
        // p 是从 cursor 往左遍历的，所以 res 是反序的
        reverse(res.begin(), res.end());
        return res;
    }

public:
    TextEditor3() {
        dummy = new Node();  // 哨兵：不存储字符，是链表头
        cursor = dummy;      // 初始光标在最左边
    }

    void addText(string text) {
        for (char c : text) {
            // 在 cursor 后插入，cursor 移到新节点
            cursor = insertAfter(cursor, c);
        }
    }

    int deleteText(int k) {
        int del = 0;
        while (del < k && cursor != dummy) {
            // 删除 cursor 指向的节点，cursor 回退到前驱
            cursor = removeNode(cursor);
            del++;
        }
        return del;
    }

    string cursorLeft(int k) {
        for (int i = 0; i < k && cursor != dummy; i++) {
            cursor = cursor->prev;
        }
        return getLeft();
    }

    string cursorRight(int k) {
        for (int i = 0; i < k && cursor->next != nullptr; i++) {
            // 注意：是 cursor->next != nullptr，不是 cursor != nullptr
            // 因为 cursor 指向光标左边的节点，右移是看右边还有没有节点
            cursor = cursor->next;
        }
        return getLeft();
    }

    ~TextEditor3() {
        Node* p = dummy;
        while (p) {
            Node* nxt = p->next;
            delete p;
            p = nxt;
        }
    }
};


// ============================================================
// 【解法对比】
//
// | 方案       | 单次操作 | 实现难度 | 实际性能   | 适用场景       |
// |------------|----------|----------|------------|----------------|
// | 字符串模拟 | O(n)     | 最简单   | 数据小能过 | 快速验证思路   |
// | 双栈 ⭐    | O(k)     | 简单     | 最佳       | 面试首选       |
// | 双向链表   | O(k)     | 中等     | 内存开销大 | 追问链表时     |
//
// 双栈方案是最佳选择：代码短、思路清晰、性能好、满足 O(k) 进阶要求。
//
// ============================================================
// 【易错点】
//
// 1. ✗ cursorLeft 时 left.erase(left.size()-k, k) 没有 clamp k
//    ✓ 应该 int moves = min(k, (int)left.size())，否则 size()-k 下溢变成巨大数
//
// 2. ✗ getLeft() 中手动 pop 出字符后拼接，忘了 reverse
//    ✓ 如果是 pop 出来拼的（栈顶先出 = 最靠近光标的先出），结果是反序的
//       但用 substr 从 left 末尾取，顺序天然正确，不需要 reverse
//
// 3. ✗ 双向链表 cursorRight 判断 cursor != nullptr
//    ✓ 应该判断 cursor->next != nullptr，因为 cursor 指的是光标左边节点
//
// 4. ✗ 链表 removeNode 后忘记更新 cursor
//    ✓ removeNode 返回被删节点的 prev，必须赋值给 cursor
//
// 5. ✗ 右栈存储的顺序搞混：以为右栈 bottom→top 是左到右
//    ✓ 右栈 top 是光标右边第1个字符，所以 "practice" 在右栈中
//       bottom→top 是 "ecitcarp"（反序）。cursorRight pop 出 'p' 压入左栈，正确。
//
// ============================================================
// 【面试追问】
//
// Q1: 暴力字符串模拟的最坏复杂度？
//   → 文本长度 n 时，insert/erase 中间是 O(n)。
//     极端情况总文本可达 ~8×10^5 字符，2×10^4 次操作 → ~10^10 不可接受。
//     本题因为 k≤40 且总调用 2×10^4 所以实际上能过。
//
// Q2: 双栈方案为什么满足 O(k)？
//   → addText: left += text 均摊 O(|text|)
//     deleteText: left.erase 从末尾删 O(k)
//     cursorLeft/Right: 循环 k 次 push_back/pop_back 各 O(1)，总 O(k)
//     getLeft: 取末尾 min(10,len) 个字符 O(1)（substr 是 O(10)=O(1)）
//
// Q3: 如果要支持 undo/redo？
//   → 命令模式：每个操作记录反操作到 undo 栈。
//     addText("abc") 的反操作是 deleteText(3)。
//     cursorLeft(k) 的反操作是 cursorRight(k)。
//     deleteText(k) 需要记录被删的字符串，反操作是 addText + cursorLeft。
//
// Q4: 超大文本（GB 级别）怎么办？
//   → 双栈本质是 Gap Buffer（间隙缓冲区），适合光标移动范围小的场景。
//     如果光标频繁跨大距离跳转，考虑 Rope 数据结构（平衡树，每个节点存一段子串）。
//     或者 Piece Table（VS Code 使用的方案）：不修改原始缓冲区，用表记录增删片段。
// ============================================================
