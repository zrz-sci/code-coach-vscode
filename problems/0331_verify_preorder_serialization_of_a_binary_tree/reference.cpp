/*
 * 【题目本质】
 * 验证一个逗号分隔的字符串是否为合法的二叉树前序序列化。
 * 不能建树，需要用数学性质（入度/出度平衡）或栈模拟判断。
 *
 * 【解法总览】
 * 解法1: 入度出度差 (diff/slots) — O(n)/O(1) ⭐推荐
 * 解法2: 栈模拟消除              — O(n)/O(n)
 */

// ===================== 解法1: 入度出度差 =====================
// 思路: 维护 diff = 可用槽位数
//   初始 diff = 1（根节点需要一个槽位）
//   非空节点: diff += 1 (消耗1个槽 + 产生2个新槽 = 净+1)
//   空节点 #: diff -= 1 (消耗1个槽，不产生新槽)
//   合法条件: 中间 diff 始终 > 0，结束时 diff == 0
// 时间: O(n)  空间: O(1)
class Solution1 {
public:
    bool isValidSerialization(string preorder) {
        int diff = 1; // 初始 1 个可用槽位
        int i = 0, n = preorder.size();

        while (i < n) {
            if (diff == 0) return false; // 中途槽位用完但还有节点

            if (preorder[i] == ',') {
                i++; // 跳过分隔符
            } else if (preorder[i] == '#') {
                diff--;  // 空节点消耗一个槽位
                i++;
            } else {
                // 数字节点（可能多位: 0-100）
                while (i < n && preorder[i] != ',') i++;
                diff++; // 净效果: -1 + 2 = +1
            }
        }

        return diff == 0;
    }
};

// ===================== 解法2: 栈模拟消除 =====================
// 思路: 模拟前序遍历的"消除"过程
//   将 token 依次压栈
//   当栈顶形成 [数字, #, #] 时，表示一个叶子节点的完整子树
//   将这三个元素替换为一个 # (叶子子树 → 空位)
//   最终栈中应只剩一个 #
// 时间: O(n)  空间: O(n)
class Solution2 {
public:
    bool isValidSerialization(string preorder) {
        vector<string> stk; // 用 vector 模拟栈，方便访问倒数第三个
        stringstream ss(preorder);
        string token;

        while (getline(ss, token, ',')) {
            stk.push_back(token);

            // 尝试反复消除栈顶的 [数字, #, #] 模式
            while (stk.size() >= 3 &&
                   stk.back() == "#" &&
                   stk[stk.size() - 2] == "#" &&
                   stk[stk.size() - 3] != "#") {
                stk.pop_back(); // 弹出 #
                stk.pop_back(); // 弹出 #
                stk.pop_back(); // 弹出数字
                stk.push_back("#"); // 替换为空节点
            }
        }

        return stk.size() == 1 && stk[0] == "#";
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 | 劣势 |
 * |------|------|------|------|------|
 * | 入出度差 | O(n) | O(1) | 空间最优，代码简洁 | 需理解数学原理 |
 * | 栈消除 | O(n) | O(n) | 直观模拟树结构 | 需额外空间 |
 *
 * 【易错点】
 * 1. diff 初始值设为 0 而非 1 → 第一个节点就会导致 diff 异常
 * 2. 只在结尾检查 diff==0，不在中间检查 diff>0
 *    → "9,#,#,1" 中 diff 在位置 3 已归零，但后面还有节点
 * 3. 节点值可能多位(如 "100")，按单字符处理会错
 * 4. 栈消除法中，三个 # 相邻时错误消除（必须是 [数字,#,#]）
 *
 * 【面试追问】
 * Q1: 数学原理? → n 个节点的二叉树有 n+1 个空位，总 2n+1 个元素
 * Q2: 后序序列化验证? → 反转后变镜像前序，或从右往左 diff 检查
 * Q3: 递归解析? → parse()消耗一个token：若 # 返回；否则递归左+右
 */
