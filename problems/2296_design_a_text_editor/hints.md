# 2296. 设计一个文本编辑器

## 核心思路

这道题的本质是：**光标把文本分成左右两部分，所有操作都发生在光标位置（即左右两部分的交界处）**。选择什么数据结构来表示这"左半部分"和"右半部分"，决定了每个操作的效率。

## 思维链

1. **读完题第一反应**：用一个字符串 + 一个整数表示光标位置，每次操作直接在字符串上 `insert`/`erase`/移动位置。这是最直觉的模拟做法。

2. **暴力解的瓶颈在哪？**：字符串的 `insert` 和 `erase` 在中间位置操作时需要 O(n) 移动后续所有字符。如果文本很长且频繁在中间操作，每次调用都是 O(n)。

3. **关键观察**：光标把文本天然分成了**左半部分**和**右半部分**。所有操作都在这个分界点发生：
   - `addText`：往左半部分的末尾追加
   - `deleteText`：从左半部分的末尾删除
   - `cursorLeft`：把左半部分的末尾字符搬到右半部分的开头
   - `cursorRight`：把右半部分的开头字符搬到左半部分的末尾

4. **什么数据结构末尾操作是 O(1)？** —— **栈**！左半部分用一个栈（尾部操作），右半部分也用一个栈（头部操作 = 栈顶操作）。这就是经典的**双栈**模型。

5. **另一种思路**：用**双向链表**，光标就是一个指针指向链表中某个节点。插入/删除/移动都是 O(1) 的指针操作。但实现更复杂。

6. **进阶要求 O(k)**：双栈方案天然满足——`addText` O(|text|)，`deleteText` O(k)，`cursorLeft/Right` O(k)，取左边 10 个字符也是 O(10)=O(1)。

## 解法概览

| 解法 | 思路 | 时间（每次调用） | 空间 | 面试 |
|------|------|------|------|------|
| 字符串模拟 | string + cursor 位置 | O(n)（insert/erase） | O(n) | 能说出即可 |
| 双栈 | 左栈 + 右栈 | O(k) 或 O(\|text\|) | O(n) | ⭐ 必须写出 |
| 双向链表 | 链表 + 光标指针 | O(k) 或 O(\|text\|) | O(n) | 加分项 |

## 关键提示

1. **光标是分界线**：不要把它想成"一个位置"，而是想成"把文本劈成两半"——左边是已打出的内容，右边是光标后面的内容。

2. **两个栈的栈顶就是光标两侧**：左栈的栈顶 = 光标左边第一个字符，右栈的栈顶 = 光标右边第一个字符。所有操作都只涉及栈顶。

3. **返回光标左边 min(10, len) 个字符**：这是从左栈顶往下取 10 个，注意顺序——栈顶是最靠近光标的，输出时要反转。

4. **用 `string` 当栈**：C++ 中 `string` 的 `push_back` 和 `pop_back` 是 O(1) 的，天然可以当栈用，而且取末尾 10 个字符用 `substr` 非常方便。

5. **双向链表方案**：如果面试官追问"能否用链表实现"，核心是维护一个 `cursor` 指针指向光标左边的节点（或用 dummy 节点处理边界），插入/删除/移动都是指针操作。

```
// 双栈模型的可视化:
//
// 文本: "leet|practice"  (| 表示光标)
//
//   左栈 (bottom→top)     右栈 (bottom→top)
//   [l, e, e, t]          [e, c, i, t, c, a, r, p]
//         ↑ 栈顶                              ↑ 栈顶
//    光标左边第1个字符          光标右边第1个字符
//
// cursorLeft(1):
//   左栈弹出 't' → 压入右栈
//   左栈: [l, e, e]       右栈: [e, c, i, t, c, a, r, p, t]
//   文本变成: "lee|tpractice"
```

## 解法详解

### 解法1: 字符串模拟 — O(n) / O(n)

**思考过程**: 最直接的做法——用一个 `string` 保存整个文本，用一个 `int` 记录光标位置。每个操作直接用字符串的 `insert`、`erase` 方法。

```cpp
class TextEditor {
    string text;
    int cursor;
public:
    TextEditor() : cursor(0) {}
    
    void addText(string t) {
        text.insert(cursor, t);
        cursor += t.size();
    }
    
    int deleteText(int k) {
        int del = min(k, cursor);         // 最多删到开头
        text.erase(cursor - del, del);
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
```

**关键点**: `insert` 和 `erase` 在中间位置操作时需要移动后面所有字符，时间 O(n)。本题数据范围小（总调用 2×10⁴，每次 text/k ≤ 40），所以暴力能过，但不满足进阶的 O(k) 要求。

---

### 解法2: 双栈 — O(k) / O(n) ⭐ 面试首选

**从解法1优化**: 解法1的瓶颈是字符串中间的插入/删除是 O(n)。核心观察是**光标把文本分成左右两部分，所有操作只发生在分界处**。用两个栈分别维护左右两部分，所有操作就变成了栈顶操作——O(1) 单字符，O(k) 移动 k 步。

```
// 操作映射:
//
// addText("xy"):  左栈 push 'x', push 'y'
//                 (文字追加到光标左边)
//
// deleteText(2):  左栈 pop 2次
//                 (删除光标左边的字符)
//
// cursorLeft(1):  左栈 pop → 右栈 push
//                 (光标左移 = 把左边的字符搬到右边)
//
// cursorRight(1): 右栈 pop → 左栈 push
//                 (光标右移 = 把右边的字符搬到左边)
```

```cpp
class TextEditor {
    // 用 string 当栈：push_back / pop_back 都是 O(1)
    string left, right;
    
    // 辅助：返回光标左边 min(10, len) 个字符
    string getLeft() {
        int len = min(10, (int)left.size());
        return left.substr(left.size() - len, len);
    }
    
public:
    TextEditor() {}
    
    void addText(string text) {
        // 直接追加到左栈末尾（相当于在光标位置插入）
        left += text;  // O(|text|)
    }
    
    int deleteText(int k) {
        int del = min(k, (int)left.size());
        // 从左栈末尾弹出 del 个字符
        left.erase(left.size() - del, del);  // O(del)
        return del;
    }
    
    string cursorLeft(int k) {
        // 光标左移 k 步：左栈顶字符搬到右栈
        int moves = min(k, (int)left.size());
        for (int i = 0; i < moves; i++) {
            right.push_back(left.back());
            left.pop_back();
        }
        return getLeft();
    }
    
    string cursorRight(int k) {
        // 光标右移 k 步：右栈顶字符搬到左栈
        int moves = min(k, (int)right.size());
        for (int i = 0; i < moves; i++) {
            left.push_back(right.back());
            right.pop_back();
        }
        return getLeft();
    }
};
```

**关键点**:
- 右栈是**反序**存储的。比如光标右边是 "practice"，右栈从底到顶是 `e,c,i,t,c,a,r,p`。这样右栈的栈顶 `p` 就是光标右边第一个字符。
- `addText` 直接字符串拼接到 `left` 末尾——因为 `string::operator+=` 追加到末尾是均摊 O(|text|) 的。
- `deleteText` 用 `erase` 从末尾删是 O(del) 的，不需要移动其他字符。
- **每次操作严格 O(k)**，满足进阶要求。

---

### 解法3: 双向链表 — O(k) / O(n)

**另一个角度**: 如果面试官要求用链表实现（题目标签里有 Doubly-Linked List），核心思路是：每个字符是一个链表节点，光标是一个指针指向光标左边的节点。

```
// 链表模型:
//
// dummy ↔ 'l' ↔ 'e' ↔ 'e' ↔ 't' ↔ 'p' ↔ 'r' ↔ 'a' ↔ 'c' ↔ 'e'
//                              ↑
//                           cursor (指向光标左边的节点 't')
//
// 此时文本表示: "leet|practice"
//
// cursorLeft(1): cursor = cursor->prev  →  指向第2个'e'
//                文本变成: "lee|tpractice"
//
// addText("x"):  在 cursor 后面插入 'x'，cursor 移到 'x'
//                文本变成: "leex|tpractice"
```

```cpp
class TextEditor {
    struct Node {
        char ch;
        Node *prev, *next;
        Node(char c = 0) : ch(c), prev(nullptr), next(nullptr) {}
    };
    
    Node *dummy, *cursor;  // cursor 指向光标左边的节点
    
    // 在 node 后面插入新节点
    Node* insertAfter(Node* node, char c) {
        Node* newNode = new Node(c);
        newNode->next = node->next;
        newNode->prev = node;
        if (node->next) node->next->prev = newNode;
        node->next = newNode;
        return newNode;
    }
    
    // 删除 node（不是 dummy），返回 node->prev
    Node* remove(Node* node) {
        Node* prev = node->prev;
        Node* next = node->next;
        prev->next = next;
        if (next) next->prev = prev;
        delete node;
        return prev;
    }
    
    string getLeft() {
        string res;
        Node* p = cursor;
        for (int i = 0; i < 10 && p != dummy; i++) {
            res += p->ch;
            p = p->prev;
        }
        reverse(res.begin(), res.end());
        return res;
    }
    
public:
    TextEditor() {
        dummy = new Node();    // 哨兵节点，不存储字符
        cursor = dummy;        // 初始光标在最左边
    }
    
    void addText(string text) {
        for (char c : text) {
            cursor = insertAfter(cursor, c);
        }
    }
    
    int deleteText(int k) {
        int del = 0;
        while (del < k && cursor != dummy) {
            cursor = remove(cursor);
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
            cursor = cursor->next;
        }
        return getLeft();
    }
    
    ~TextEditor() {
        Node* p = dummy;
        while (p) {
            Node* next = p->next;
            delete p;
            p = next;
        }
    }
};
```

**关键点**:
- `dummy` 哨兵节点避免了处理"光标在最左边"的边界条件——`cursor == dummy` 就意味着光标左边没有字符。
- 链表方案在 `addText` 时逐字符插入是 O(|text|)，`deleteText` 是 O(k)，移动是 O(k)。
- 缺点：内存分配开销大（每个字符一个节点），且缓存不友好。实际性能通常不如双栈。

## 解法对比

| | 字符串模拟 | 双栈 ⭐ | 双向链表 |
|---|---|---|---|
| **核心思想** | 暴力模拟 | 光标分左右两半 | 链表指针即光标 |
| **单次操作** | O(n) 最坏 | O(k) | O(k) |
| **实现难度** | 最简单 | 简单 | 中等 |
| **实际性能** | 数据小能过 | 最佳（缓存友好） | 内存开销大 |
| **代码量** | 最少 | 适中 | 最多 |
| **适用场景** | 快速 AC | 面试首选 | 面试官追问链表时 |

**面试中推荐双栈**：代码简洁、思路清晰、完美满足 O(k) 要求。

## 易错点

1. **✗ `cursorLeft` 时直接 `cursor -= k`，没有 clamp 到 0**
   → ✓ `moves = min(k, (int)left.size())`，确保不会移到文本之前

2. **✗ `getLeft()` 返回时顺序搞反**
   → ✓ 左栈末尾是最靠近光标的字符，`substr(size-10, 10)` 的顺序刚好是从左到右，不需要反转。但如果你是逐个 pop 出来拼接的，需要 reverse。

3. **✗ 双向链表中 `cursorRight` 判断条件写成 `cursor != nullptr`**
   → ✓ 应该是 `cursor->next != nullptr`，因为 cursor 指向的是光标左边的节点，右移是看右边还有没有节点。

4. **✗ `deleteText` 忘记更新 cursor**
   → ✓ 双栈方案中 `erase` 从末尾删是安全的；链表方案中删除后 cursor 必须回到前驱节点。

5. **✗ 双栈方案 `addText` 时用 `left.insert(left.size(), text)` 以为比 `+=` 快**
   → ✓ 直接 `left += text` 最简洁高效，均摊 O(|text|)。

## 面试追问

**Q1: 暴力的字符串模拟方案最坏情况是什么？**
> 如果文本长度为 n，在中间 insert/erase 需要 O(n) 来移动后续字符。2×10⁴ 次操作，每次文本可能增长到 ~8×10⁵，总共可能 O(n × 操作数) ≈ 10¹⁰，理论上会超时，但本题数据范围较小所以能过。

**Q2: 双栈方案如何保证每次操作是 O(k)？**
> 所有操作都只涉及两个栈的栈顶区域。`addText` 追加到左栈末尾 O(|text|)；`deleteText` 从左栈末尾删除 O(k)；`cursorLeft/Right` 在两栈间搬运 O(k) 个字符。取左边 10 个字符是 O(10) = O(1)。没有任何操作需要遍历整个文本。

**Q3: 如果要支持 `undo/redo` 功能，怎么扩展？**
> 可以用**命令模式**：每个操作记录为一个命令对象（包含操作类型和参数），维护一个 undo 栈和 redo 栈。执行操作时压入 undo 栈；undo 时弹出并执行反操作，压入 redo 栈。对于双栈方案：addText 的反操作是 deleteText（相同长度），cursorLeft 的反操作是 cursorRight（相同步数）。

**Q4: 如果文本非常大（比如几 GB），双栈方案有什么问题？**
> `string` 的 `+=` 可能触发重新分配和拷贝。可以改用 **rope 数据结构**（一种平衡树，每个节点存一段子串），支持 O(log n) 的拼接和分割。或者用 **gap buffer**（本题的双栈其实就是 gap buffer 的简化版）：一个大数组，光标位置是一个"间隙"，两侧分别是左右文本。

## 相关题型

- **1472. 设计浏览器历史记录** — 同样是双栈模型：back 栈和 forward 栈，visit 时清空 forward 栈。复用"两个栈维护当前位置左右两侧"的思想。
- **155. 最小栈** — 设计题 + 栈，体会"辅助栈"思想。
- **146. LRU 缓存** — 设计题，用双向链表 + 哈希表。复用本题中双向链表的增删操作，区别在于 LRU 需要 O(1) 随机访问所以加了哈希表。
- **707. 设计链表** — 如果你选链表方案，这道题是基础练习，熟悉双向链表的增删操作。