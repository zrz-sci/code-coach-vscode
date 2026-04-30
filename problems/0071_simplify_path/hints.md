# 71. 简化路径

## 核心思路

本质上就是模拟 Unix 的 `cd` 命令：把路径按 `/` 分割成一个个目录名，遇到 `..` 就回退一层，遇到 `.` 就不动，最后把剩余的目录拼起来。**栈**天然适合这种"进入/回退"的操作。

## 思维链

1. **读完题第一反应**：路径是用 `/` 分隔的，每段可能是目录名、`.`、`..` 或空串（连续斜杠导致）。我需要逐段处理。
2. **怎么分割？** 用 `/` 作为分隔符，把路径拆成若干 token。连续的 `/` 会产生空串，直接跳过即可。
3. **每段怎么处理？** 
   - 空串或 `"."`：什么都不做（当前目录）
   - `".."`：回到上一级 → 删除最近进入的那个目录
   - 其他：这是个合法目录名，记录下来
4. **"删除最近进入的"→ 后进先出 → 栈！** 用栈存储当前路径中的各级目录名，遇到 `..` 就 pop，其他有效名就 push。
5. **最后拼接**：把栈从底到顶用 `/` 连接，前面加一个 `/`。如果栈空就返回 `"/"`。
6. **关键细节**：`...`、`....` 等是合法目录名，不要误当特殊符号处理。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 栈 + split | 按 `/` 分割后用栈模拟进入/回退 | O(n) | O(n) | ⭐ 必须写出 |
| 栈 + 手动解析 | 不用 stringstream，手动遍历字符提取 token | O(n) | O(n) | 能说出即可 |
| 原地双指针 | 用 deque 代替栈方便正序输出 | O(n) | O(n) | 加分项（代码更简洁） |

## 关键提示

1. **分割是第一步**：先想怎么把 `"/home//foo/"` 变成 `["home", "foo"]`，再想怎么处理每个 token。
2. **为什么用栈？** 因为 `..` 需要回退"最近进入的目录"——这就是后进先出（LIFO）。
3. **`.` 和空串效果一样**：都是"什么都不做"，统一处理可以简化代码。
4. **`..` 在栈空时怎么办？** 已经在根目录了，不能再回退，直接忽略（示例4：`"/../"` → `"/"`）。
5. **`"..."` 是合法目录名！** 只有恰好一个点 `"."` 和恰好两个点 `".."` 是特殊的。

```
// 处理流程可视化:
// 输入: "/home/user/Documents/../Pictures"
// 
// 分割: ["home", "user", "Documents", "..", "Pictures"]
// 
// 栈操作:
// "home"      → push → 栈: [home]
// "user"      → push → 栈: [home, user]
// "Documents" → push → 栈: [home, user, Documents]
// ".."        → pop  → 栈: [home, user]
// "Pictures"  → push → 栈: [home, user, Pictures]
// 
// 拼接: "/" + "home" + "/" + "user" + "/" + "Pictures"
// 输出: "/home/user/Pictures"
```

## 解法详解

### 解法1: 栈 + stringstream 分割 — O(n) / O(n) ⭐ 面试首选

**思考过程**：路径天然是按 `/` 分层的结构，`..` 回退到上一层 = 栈的 pop 操作。所以核心逻辑就是：split → 逐个 token 决定 push/pop/skip → 拼接结果。

C++ 中用 `getline(ss, token, '/')` 可以方便地按 `/` 分割字符串。

```cpp
class Solution {
public:
    string simplifyPath(string path) {
        vector<string> stk;  // 用 vector 当栈，方便最后正序遍历拼接
        stringstream ss(path);
        string token;
        
        while (getline(ss, token, '/')) {
            if (token == "" || token == ".") {
                // 空串(连续斜杠)或当前目录，跳过
                continue;
            } else if (token == "..") {
                // 回退上一级：栈非空才能 pop
                if (!stk.empty()) {
                    stk.pop_back();
                }
                // 栈空说明已在根目录，忽略即可
            } else {
                // 合法目录名（包括 "..."、"a.b" 等），入栈
                stk.push_back(token);
            }
        }
        
        // 拼接结果
        string result;
        for (const string& dir : stk) {
            result += "/" + dir;
        }
        return result.empty() ? "/" : result;
    }
};
```

**关键点**：
- `getline(ss, token, '/')` 遇到连续 `/` 会产生空串，必须跳过。
- `vector` 比 `stack` 好用：stack 只能访问栈顶，拼接时需要从底到顶遍历，用 vector 直接 for 循环即可。

### 解法2: 手动解析（不依赖 stringstream）— O(n) / O(n)

**从解法1变体**：面试中有时面试官不允许用 stringstream，需要手动用双指针提取两个 `/` 之间的子串。

```cpp
class Solution {
public:
    string simplifyPath(string path) {
        vector<string> stk;
        int n = path.size();
        int i = 0;
        
        while (i < n) {
            // 跳过所有连续的 '/'
            while (i < n && path[i] == '/') i++;
            
            // 提取下一个 token（两个 '/' 之间的内容）
            int start = i;
            while (i < n && path[i] != '/') i++;
            string token = path.substr(start, i - start);
            
            if (token.empty() || token == ".") {
                continue;
            } else if (token == "..") {
                if (!stk.empty()) stk.pop_back();
            } else {
                stk.push_back(token);
            }
        }
        
        string result;
        for (const string& dir : stk) {
            result += "/" + dir;
        }
        return result.empty() ? "/" : result;
    }
};
```

**关键点**：双指针 `start` 和 `i` 配合，`start` 标记 token 开头，`i` 向前扫到下一个 `/` 或末尾。

### 解法3: 使用 deque（双端队列）— O(n) / O(n)

**思路**：本质和解法1完全一样，只是用 `deque` 替代 `vector`。实际上这里 `vector` 就够了，但如果题目变成处理相对路径（需要在前面插入），deque 更灵活。

```cpp
class Solution {
public:
    string simplifyPath(string path) {
        deque<string> dq;
        stringstream ss(path);
        string token;
        
        while (getline(ss, token, '/')) {
            if (token.empty() || token == ".") continue;
            if (token == "..") {
                if (!dq.empty()) dq.pop_back();
            } else {
                dq.push_back(token);
            }
        }
        
        string result;
        for (const string& dir : dq) {
            result += "/" + dir;
        }
        return result.empty() ? "/" : result;
    }
};
```

## 解法对比

| | 解法1 (stringstream) | 解法2 (手动解析) | 解法3 (deque) |
|---|---|---|---|
| 时间 | O(n) | O(n) | O(n) |
| 空间 | O(n) | O(n) | O(n) |
| 代码量 | 最少 | 稍多 | 和1差不多 |
| 面试推荐 | ✅ 首选 | 不让用库函数时 | 功能上和1等价 |
| 核心区别 | 依赖 stringstream 分割 | 手写分割逻辑 | 容器不同而已 |

三种解法的核心逻辑完全一致：分割 → 栈操作 → 拼接。区别仅在分割方式和容器选择上。

## 易错点

1. **把 `"..."` 当成特殊符号处理**
   - ✗ `if (token.find('.') != npos) skip;` — 这会把 `"..."` 也跳过
   - ✓ 只有 **恰好等于** `"."` 或 `".."` 才特殊处理，其他含点的字符串都是合法目录名

2. **忘记处理空 token**
   - ✗ 分割后不检查 `token.empty()`，导致把空串当作目录名 push 入栈
   - ✓ `if (token.empty() || token == ".") continue;`

3. **栈空时遇到 `..` 没有保护**
   - ✗ `stk.pop_back();` 不检查 empty → 运行时崩溃
   - ✓ `if (!stk.empty()) stk.pop_back();`

4. **用 `stack<string>` 导致拼接困难**
   - ✗ `stack` 只能访问栈顶，要从底到顶拼接得先全部 pop 到临时数组再反转
   - ✓ 直接用 `vector<string>` 当栈，pop_back / push_back 模拟栈操作，正序遍历即可拼接

5. **最终结果忘记处理空栈**
   - ✗ 栈空时返回空字符串 `""`
   - ✓ 栈空时返回根目录 `"/"`

## 面试追问

**Q1（基础理解）：为什么选择栈而不是其他数据结构？**
> `..` 的语义是"回到最近进入的目录"，这是典型的后进先出(LIFO)模式，栈完美匹配。用队列或其他结构都需要额外处理。

**Q2（实现细节）：如果不允许用 stringstream，怎么分割字符串？**
> 用双指针手动扫描：跳过连续 `/`，记录 token 开始位置，扫到下一个 `/` 或末尾取 `substr`。就是解法2的做法。

**Q3（变体）：如果输入是相对路径（不以 `/` 开头），需要一个当前工作目录 cwd，怎么处理？**
> 先把 cwd 按同样方式解析入栈，再处理相对路径的 token。`..` 的回退逻辑不变，但要注意 cwd 本身的层级可能不够回退（此时应该报错或留在根目录）。

**Q4（扩展）：如果路径中包含符号链接(symlink)，`..` 的行为应该回到物理父目录还是逻辑父目录？**
> 这道题按逻辑路径处理（纯字符串操作）。如果要处理物理路径，需要在每次遇到目录名时查文件系统判断是否是 symlink，解析其指向后再决定。这超出了本题范围，但在实际 shell 的 `realpath` 命令中会遇到。

## 相关题型

- **[150. 逆波兰表达式求值](https://leetcode.cn/problems/evaluate-reverse-polish-notation/)** — 复用相同的栈模式：遇到操作数 push，遇到运算符 pop 两个操作数计算后 push 结果。本题是"遇到目录名 push，遇到 `..` pop"。
- **[20. 有效的括号](https://leetcode.cn/problems/valid-parentheses/)** — 栈的经典应用，"匹配/回退"的模式和本题的 `..` 回退一脉相承。
- **[394. 字符串解码](https://leetcode.cn/problems/decode-string/)** — 也是用栈处理嵌套结构，`[` push 进栈，`]` pop 出来处理，和本题 push 目录 / `..` pop 目录异曲同工。