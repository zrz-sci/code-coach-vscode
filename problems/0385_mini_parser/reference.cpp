/*
 * 385. Mini Parser
 * 难度: Medium
 * 标签: Stack, Depth-First Search, String
 *
 * 【题目本质】
 * 将嵌套列表的字符串序列化形式反序列化为 NestedInteger 对象。
 * 本质是解析嵌套结构，用栈或递归处理层级关系。
 *
 * 【解法总览】
 * 解法1: 栈模拟 - 显式栈处理嵌套层级
 * 解法2: 递归下降解析器 - 利用函数调用栈
 */

// ===================== Solution 1: 栈模拟 =====================
// 思路：遇 [ 压栈新建列表，遇 ] 弹栈合并，遇数字添加到栈顶
// 时间：O(n)
// 空间：O(d)，d 为最大嵌套深度
class Solution1 {
public:
    NestedInteger deserialize(string s) {
        // 特判：纯数字
        if (s[0] != '[') return NestedInteger(stoi(s));

        stack<NestedInteger> stk;
        int i = 0, n = s.size();

        while (i < n) {
            if (s[i] == '[') {
                stk.push(NestedInteger());  // 新建空列表压栈
                i++;
            } else if (s[i] == ']') {
                if (stk.size() > 1) {
                    NestedInteger top = stk.top();
                    stk.pop();
                    stk.top().add(top);  // 弹出并合并到上层
                }
                i++;
            } else if (s[i] == ',') {
                i++;  // 跳过分隔符
            } else {
                // 解析数字（含负数）
                int j = i;
                while (j < n && s[j] != ',' && s[j] != ']') j++;
                int num = stoi(s.substr(i, j - i));
                stk.top().add(NestedInteger(num));
                i = j;
            }
        }
        return stk.top();
    }
};

// ===================== Solution 2: 递归下降解析器 =====================
// 思路：用全局指针 i 扫描，遇 [ 递归解析子列表，遇数字直接返回
// 时间：O(n)
// 空间：O(d) 递归栈
class Solution2 {
    int i = 0;

    NestedInteger parse(const string& s) {
        if (s[i] == '[') {
            NestedInteger ni;
            i++;  // 跳过 '['
            while (s[i] != ']') {
                ni.add(parse(s));
                if (s[i] == ',') i++;
            }
            i++;  // 跳过 ']'
            return ni;
        } else {
            int j = i;
            while (i < (int)s.size() && s[i] != ',' && s[i] != ']') i++;
            return NestedInteger(stoi(s.substr(j, i - j)));
        }
    }

public:
    NestedInteger deserialize(string s) {
        i = 0;
        return parse(s);
    }
};

/*
 * 【解法对比】
 * | 解法 | 代码量 | 可读性 | 栈溢出风险 |
 * |------|--------|--------|-----------|
 * | 栈模拟 | 较多 | 中等 | 无 |
 * | 递归下降 | 较少 | 高 | 深层嵌套时有风险 |
 *
 * 【易错点】
 * 1. 纯数字输入 "324" 必须特判（不含括号）
 * 2. 负数处理："-123" 中 '-' 是数字的一部分
 * 3. 空列表 "[]" 要正确处理
 * 4. 最外层 ']' 弹栈时不需要再合并（栈中只剩一个元素）
 *
 * 【面试追问】
 * Q1: 非法输入检测 → 检查括号匹配和栈状态
 * Q2: 与表达式求值的关系 → 同为栈处理嵌套，但本题无运算符
 * Q3: 序列化的逆操作 → 用递归/BFS遍历 NestedInteger 构建字符串
 */
