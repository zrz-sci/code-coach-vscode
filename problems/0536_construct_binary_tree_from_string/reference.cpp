/*
 * 【题目本质】
 * 从嵌套括号字符串 "val(left)(right)" 中递归解析并构建二叉树。
 * 核心难点：正确解析数字（含负号）和管理括号嵌套层级。
 *
 * 【解法总览】
 * Solution1: 递归解析（引用下标）— 最直观，代码简洁（推荐）
 * Solution2: 栈模拟 — 迭代方式，避免递归深度问题
 */

// 解法一：递归解析（推荐）
// 思路：用引用下标 i 扫描字符串，遇到数字建节点，遇到 ( 递归进子树
// 时间 O(n)，空间 O(h) 递归栈
class Solution1 {
public:
    TreeNode* str2tree(string s) {
        int i = 0;
        return helper(s, i);
    }

    TreeNode* helper(string& s, int& i) {
        if (i >= s.size()) return nullptr;

        // 解析数字（含负号）
        int sign = 1;
        if (s[i] == '-') { sign = -1; i++; }
        int num = 0;
        while (i < s.size() && isdigit(s[i])) {
            num = num * 10 + (s[i] - '0');
            i++;
        }
        TreeNode* node = new TreeNode(sign * num);

        // 左子树
        if (i < s.size() && s[i] == '(') {
            i++; // 跳过 '('
            node->left = helper(s, i);
            i++; // 跳过 ')'
        }
        // 右子树
        if (i < s.size() && s[i] == '(') {
            i++; // 跳过 '('
            node->right = helper(s, i);
            i++; // 跳过 ')'
        }
        return node;
    }
};

// 解法二：栈模拟
// 思路：遍历字符串，遇到数字创建节点压栈并链接父节点，遇到 ) 弹栈
// 时间 O(n)，空间 O(h)
class Solution2 {
public:
    TreeNode* str2tree(string s) {
        if (s.empty()) return nullptr;
        stack<TreeNode*> stk;
        int i = 0;

        while (i < s.size()) {
            if (s[i] == ')') {
                stk.pop();
                i++;
            } else if (s[i] == '(') {
                i++;
            } else {
                // 解析数字
                int sign = 1;
                if (s[i] == '-') { sign = -1; i++; }
                int num = 0;
                while (i < s.size() && isdigit(s[i])) {
                    num = num * 10 + (s[i] - '0');
                    i++;
                }
                TreeNode* node = new TreeNode(sign * num);
                if (!stk.empty()) {
                    TreeNode* parent = stk.top();
                    if (!parent->left) parent->left = node;
                    else parent->right = node;
                }
                stk.push(node);
            }
        }
        return stk.empty() ? nullptr : stk.top();
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | 递归 | O(n) | O(h) | 代码简洁直观 |
 * | 栈 | O(n) | O(h) | 避免递归深度限制 |
 *
 * 【易错点】
 * 1. 下标 i 必须按引用传递，否则递归返回后不同步
 * 2. 负号处理：先检查 '-'，再解析数字部分
 * 3. 栈解法中遇到 ')' 必须弹栈，表示子树构建完成
 * 4. 空字符串需要特判返回 nullptr
 *
 * 【面试追问】
 * Q1: 反向序列化？→ 前序遍历输出 val(left)(right)
 * Q2: 只有右子树？→ val()(right) 空括号表示空左子树
 * Q3: 递归深度过大？→ 用栈模拟迭代
 */
