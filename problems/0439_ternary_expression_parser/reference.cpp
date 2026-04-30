/*
 * 【题目本质】
 * 解析右结合的嵌套三元表达式 cond?expr1:expr2
 * 核心是处理嵌套结构的归约或递归解析
 *
 * 【解法总览】
 * 解法1: 栈（从右往左）⭐ — O(n) / O(n) — 面试首选
 * 解法2: 递归解析 — O(n) / O(n) — 自顶向下，逻辑清晰
 */

// ============================================================
// 解法1: 栈（从右往左）⭐
// 思路：从右往左扫描，遇到条件字符时归约栈顶的 ?val1:val2
// 时间 O(n)，空间 O(n)
// ============================================================
class Solution1 {
public:
    string parseTernary(string expression) {
        stack<char> st;
        for (int i = (int)expression.size() - 1; i >= 0; i--) {
            char c = expression[i];
            if (!st.empty() && st.top() == '?') {
                st.pop();  // pop '?'
                char trueVal = st.top(); st.pop();
                st.pop();  // pop ':'
                char falseVal = st.top(); st.pop();
                st.push(c == 'T' ? trueVal : falseVal);
            } else {
                st.push(c);
            }
        }
        return string(1, st.top());
    }
};

// ============================================================
// 解法2: 递归解析
// 思路：从左往右递归，pos 标记当前位置
//       遇到 cond? 时递归解析 true/false 分支
// 时间 O(n)，空间 O(n)（递归栈）
// ============================================================
class Solution2 {
public:
    string parseTernary(string expression) {
        int pos = 0;
        return string(1, parse(expression, pos));
    }

private:
    char parse(const string& s, int& pos) {
        char c = s[pos++];
        // 如果后面跟 '?'，说明 c 是条件
        if (pos < (int)s.size() && s[pos] == '?') {
            pos++; // skip '?'
            char trueVal = parse(s, pos);
            pos++; // skip ':'
            char falseVal = parse(s, pos);
            return c == 'T' ? trueVal : falseVal;
        }
        // 否则 c 就是叶子值（数字/T/F）
        return c;
    }
};

/*
 * 【解法对比】
 * 栈方法：从右往左天然处理右结合，迭代无递归栈溢出风险
 * 递归方法：代码更短，逻辑更直观，但递归深度可达 O(n/4)
 *
 * 【易错点】
 * 1. 从左往右用栈会导致结合性错误 → 必须从右往左
 * 2. 栈归约时 trueVal 和 falseVal 取反 → 从右往左时 first 是 true 分支
 * 3. 递归解析忘记跳 ':' → pos++ 不能漏
 * 4. 返回值是 string 而非 char → string(1, ch) 转换
 *
 * 【面试追问】
 * Q: 多位数怎么办？ A: 栈存 string，解析数字时连续读取
 * Q: 左结合怎么处理？ A: 从左往右用栈或迭代匹配最外层 ?:
 * Q: 时间下界？ A: O(n)，必须读取每个字符
 */
