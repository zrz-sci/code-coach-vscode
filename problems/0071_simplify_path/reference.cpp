/*
 * LeetCode 71: Simplify Path
 * 
 * 【题目本质】
 * 模拟 Unix 路径解析：按 '/' 分割路径为 token，
 * 用栈处理目录的进入(push)和回退(pop)，最后拼接结果。
 *
 * 【解法总览】
 * 解法1: 栈 + stringstream 分割  — O(n) / O(n) — 面试首选，代码最简洁
 * 解法2: 栈 + 手动双指针分割    — O(n) / O(n) — 不依赖库函数
 */

// ============================================================
// 解法1: 栈 + stringstream 分割 — 面试首选
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 路径天然是按 '/' 分层的结构。
// 核心观察：".." 要回退到"最近进入的目录" → 后进先出 → 栈。
//
// 流程：
// 1. 用 getline(ss, token, '/') 按 '/' 分割
// 2. 对每个 token：
//    - "" 或 "."  → 跳过（连续斜杠或当前目录）
//    - ".."       → 栈非空则 pop（回退上一级）
//    - 其他       → push（进入目录，包括 "..." 等合法名）
// 3. 把栈从底到顶用 "/" 拼接
//
// 处理流程可视化:
// 输入: "/home/user/Documents/../Pictures"
// 
// 分割: ["home", "user", "Documents", "..", "Pictures"]
// 
//   token         操作        栈状态
//   "home"        push        [home]
//   "user"        push        [home, user]
//   "Documents"   push        [home, user, Documents]
//   ".."          pop         [home, user]
//   "Pictures"    push        [home, user, Pictures]
// 
// 拼接: "/home/user/Pictures"
//
// 边界 case: "/../" → 分割得 ["", "..", ""]
//   ".." 时栈空，跳过 → 栈为空 → 返回 "/"
// ============================================================
class Solution1 {
public:
    string simplifyPath(string path) {
        vector<string> stk;  // 用 vector 当栈，方便正序遍历拼接
        stringstream ss(path);
        string token;
        
        while (getline(ss, token, '/')) {
            if (token.empty() || token == ".") {
                // 空串（连续斜杠产生）或当前目录 → 无操作
                continue;
            } else if (token == "..") {
                // 回退上一级：必须检查栈非空，否则已在根目录
                if (!stk.empty()) {
                    stk.pop_back();
                }
            } else {
                // 合法目录名（"...", "a.b", 普通名字等）→ 入栈
                stk.push_back(token);
            }
        }
        
        // 从底到顶拼接，每个目录前加 "/"
        string result;
        for (const string& dir : stk) {
            result += "/" + dir;
        }
        // 栈空表示最终在根目录
        return result.empty() ? "/" : result;
    }
};

// ============================================================
// 解法2: 栈 + 手动双指针分割 — 不依赖 stringstream
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 面试官可能不让用 stringstream，需要手动提取 token。
// 核心逻辑和解法1完全一样，只是分割方式不同：
//   - 指针 i 从左到右扫描
//   - 先跳过所有连续 '/'
//   - 记录 start 位置，扫到下一个 '/' 或末尾，提取 substr
//
// 手动分割过程（以 "/home//foo/" 为例）:
//   i=0: '/' → 跳过
//   i=1: start=1, 扫到 i=5 ('/')  → token="home"
//   i=5: '/' → 跳过
//   i=6: '/' → 跳过（连续斜杠）
//   i=7: start=7, 扫到 i=10 ('/') → token="foo"
//   i=10: '/' → 跳过
//   i=11: 越界结束
// ============================================================
class Solution2 {
public:
    string simplifyPath(string path) {
        vector<string> stk;
        int n = path.size();
        int i = 0;
        
        while (i < n) {
            // 跳过所有连续 '/'
            while (i < n && path[i] == '/') i++;
            
            // 提取 token: 从 start 到下一个 '/' 前
            int start = i;
            while (i < n && path[i] != '/') i++;
            string token = path.substr(start, i - start);
            
            // 和解法1 完全相同的判断逻辑
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

// ============================================================
// 【解法对比】
// 
// 解法1 vs 解法2:
//   - 核心逻辑完全一致：分割 → 栈操作 → 拼接
//   - 唯一区别：分割方式
//     解法1 用 getline(ss, token, '/') — 代码更短
//     解法2 用双指针手动扫描 — 不依赖 <sstream>
//   - 面试首选解法1（除非面试官明确要求不用库函数）
//
// 【易错点】
// 1. 把 "..." 当特殊符号:
//    ✗ if (token[0] == '.') skip;  ← "..." 是合法目录名
//    ✓ 只有 token == "." 或 token == ".." 才是特殊的
//
// 2. 栈空时 pop:
//    ✗ stk.pop_back();  ← 运行时崩溃
//    ✓ if (!stk.empty()) stk.pop_back();
//
// 3. 用 stack<string> 导致拼接困难:
//    ✗ stack 只能访问栈顶，需要 pop 到临时数组再反转
//    ✓ 用 vector 的 push_back/pop_back 模拟栈，正序遍历即可拼接
//
// 4. 空栈时返回空串:
//    ✗ return result;  ← 当 stk 为空时 result 是 ""
//    ✓ return result.empty() ? "/" : result;
//
// 5. getline 产生的空 token 没跳过:
//    ✗ 连续斜杠 "//" 产生空串被当作目录名 push
//    ✓ if (token.empty() || token == ".") continue;
//
// 【面试追问】
// Q1: 为什么用栈？
//   → ".." 回退最近进入的目录 = 后进先出 = 栈的 pop 操作
//
// Q2: 不用 stringstream 怎么分割？
//   → 双指针：跳过 '/'，记录 start，扫到下一个 '/'，取 substr（解法2）
//
// Q3: 如果要处理相对路径 + 当前工作目录 cwd 怎么办？
//   → 先把 cwd 解析入栈，再处理相对路径的 token。
//     ".." 的回退逻辑不变，但栈空时不能再回退（已在根目录）。
//
// Q4: 如果路径非常长（几 GB），内存放不下怎么办？
//   → 流式处理：逐字符读取并维护栈，不需要一次性加载整个字符串。
//     解法2 的手动扫描方式天然适合改造为流式。
// ============================================================
