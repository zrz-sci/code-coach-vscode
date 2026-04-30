/*
 * 【题目本质】
 * 生成所有长度为n的中心对称数。
 * 核心是递归由内向外构建: base case为n=0("")和n=1("0","1","8"),
 * 每层在两端添加对称数字对, 最外层不能有前导零。
 *
 * 【解法总览】
 * 解法1 (推荐): 递归由内向外构建 — O(5^(n/2)) / O(5^(n/2))
 * 解法2: 迭代 BFS 式构建 — O(5^(n/2)) / O(5^(n/2))
 */

// ===================== 解法1: 递归由内向外构建 (推荐) =====================
// 思路: 从最内层开始, 每层在两端添加一对对称数字
// 关键: 需要 n 和 target 两个参数来判断是否为最外层(控制前导零)
class Solution1 {
public:
    vector<string> findStrobogrammatic(int n) {
        return helper(n, n);
    }

private:
    vector<string> helper(int n, int target) {
        // base case: 长度为0返回空串, 长度为1返回自对称数字
        if (n == 0) return {""};
        if (n == 1) return {"0", "1", "8"};

        // 递归获取内层 (长度为 n-2) 的所有中心对称数
        vector<string> middles = helper(n - 2, target);
        vector<string> result;

        for (const string& mid : middles) {
            // 最外层不能添加前导零
            if (n != target) {
                result.push_back("0" + mid + "0");
            }
            result.push_back("1" + mid + "1");
            result.push_back("6" + mid + "9");
            result.push_back("8" + mid + "8");
            result.push_back("9" + mid + "6");
        }
        return result;
    }
};

// ===================== 解法2: 迭代 BFS 式构建 =====================
// 思路: 从中心层开始, 逐层向外扩展, 避免递归调用栈
class Solution2 {
public:
    vector<string> findStrobogrammatic(int n) {
        // 根据奇偶性确定初始层
        vector<string> cur;
        if (n % 2 == 1) {
            cur = {"0", "1", "8"};  // 奇数: 中心是单个自对称数字
        } else {
            cur = {""};             // 偶数: 中心是空串
        }

        vector<pair<char,char>> pairs = {
            {'0','0'}, {'1','1'}, {'6','9'}, {'8','8'}, {'9','6'}
        };

        // 从内向外扩展, 每次长度+2
        int startLen = (n % 2 == 1) ? 3 : 2;
        for (int len = startLen; len <= n; len += 2) {
            vector<string> next;
            for (const string& s : cur) {
                for (auto& [a, b] : pairs) {
                    // 最外层 (len == n) 不允许前导零
                    if (len == n && a == '0') continue;
                    next.push_back(a + s + b);
                }
            }
            cur = next;
        }
        return cur;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | 递归构建 | O(5^(n/2)) | O(5^(n/2)) | 代码简洁, 逻辑清晰 |
 * | 迭代BFS | O(5^(n/2)) | O(5^(n/2)) | 避免递归栈, 适合大n |
 *
 * 【易错点】
 * 1. ✗ n==0 返回空vector → ✓ 返回 {""}
 *    空串是递归基, 否则外层无法在两端添加字符
 * 2. ✗ 忘记区分最外层 → ✓ n != target 时才加 "0"+mid+"0"
 *    否则生成 "069" 等前导零数字
 * 3. ✗ 每层 n 减 1 → ✓ 每层 n 减 2
 *    两端各加一个字符, 总长度增加2
 * 4. ✗ 只传 n 一个参数 → ✓ 需要 target 参数标记目标长度
 *    内层允许 "0...0", 最外层不允许
 *
 * 【面试追问】
 * Q1: 递归深度和栈安全? → 深度 n/2, n<=14 完全安全
 * Q2: 如何不生成全部结果就统计个数? → 数学公式: 4*5^((n-2)/2) (偶数)
 * Q3: 如何扩展到统计[low,high]范围内的个数? → 枚举各长度 + 边界比较 (LC 248)
 */
