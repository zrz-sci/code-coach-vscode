/*
 * LeetCode 555: Split Concatenated Strings (分割连接字符串)
 * 难度: Medium
 * 标签: Greedy, Array, String
 *
 * =============================================
 * 解法一: 贪心 + 枚举切割点 (推荐)
 * =============================================
 *
 * 核心思路:
 *   1. 非切割字符串: 贪心取 max(s, reverse(s)) — 中间段越大越好
 *   2. 切割字符串: 同时尝试正序和反转的每个切割位置
 *   3. 构造候选: 右半部分 + 其他串(环形顺序) + 左半部分
 *
 * 关键洞察:
 *   - 切割点一定在某个字符串内部
 *   - 不含切割点的串, 选字典序更大的方向一定不会变差
 *   - 含切割点的串, 必须两个方向都试 (切割位置不同, 最优方向不同)
 *
 * 时间: O(L^2), L = 总字符数 (<= 1000)
 * 空间: O(L)
 */

class Solution {
public:
    string splitLoopedString(vector<string>& strs) {
        int n = strs.size();

        // ---- Step 1: 预处理 ----
        // 对每个字符串, 取正序/反转中字典序更大的
        // 这是对"不含切割点的字符串"的贪心最优选择
        for (auto& s : strs) {
            string rev(s.rbegin(), s.rend());
            if (rev > s) s = rev;
        }

        string result = "";

        // ---- Step 2: 枚举切割字符串 ----
        for (int i = 0; i < n; i++) {
            // 构造 "其他部分": 按环形顺序拼接 i+1,...,n-1,0,...,i-1
            // 这些字符串已经在 Step 1 中被设为最优方向
            string other;
            for (int k = i + 1; k < n; k++) other += strs[k];
            for (int k = 0; k < i; k++) other += strs[k];

            // ---- Step 3: 尝试正序和反转 ----
            // 切割字符串不能直接用预处理的方向, 必须两个都试
            string rev(strs[i].rbegin(), strs[i].rend());

            for (const string& cur : {strs[i], rev}) {
                int len = cur.size();

                // ---- Step 4: 枚举切割位置 ----
                // 在位置 j 切割: 右半 cur[j..len-1] 成为开头, 左半 cur[0..j-1] 成为结尾
                for (int j = 0; j < len; j++) {
                    // 切割后: cur[j..end] + other + cur[0..j-1]
                    string candidate = cur.substr(j) + other + cur.substr(0, j);
                    if (candidate > result) {
                        result = candidate;
                    }
                }
            }
        }

        return result;
    }
};

/*
 * trace: strs = ["abc", "xyz"]
 *
 * Step 1 预处理:
 *   "abc" vs "cba" → "cba" (字典序更大)
 *   "xyz" vs "zyx" → "zyx" (字典序更大)
 *   strs = ["cba", "zyx"]
 *
 * Step 2 枚举 i=0 ("cba"):
 *   other = "zyx"
 *   正序 "cba":
 *     j=0: "cba" + "zyx" = "cbazyx"
 *     j=1: "ba" + "zyx" + "c" = "bazyxc"
 *     j=2: "a" + "zyx" + "cb" = "azyxcb"
 *   反转 "abc":
 *     j=0: "abc" + "zyx" = "abczyx"
 *     j=1: "bc" + "zyx" + "a" = "bczyxa"
 *     j=2: "c" + "zyx" + "ab" = "czyxab"
 *   当前最大: "cbazyx"
 *
 * Step 2 枚举 i=1 ("zyx"):
 *   other = "cba"
 *   正序 "zyx":
 *     j=0: "zyx" + "cba" = "zyxcba"  ← 新最大!
 *     j=1: "yx" + "cba" + "z" = "yxcbaz"
 *     j=2: "x" + "cba" + "zy" = "xcbazy"
 *   反转 "xyz":
 *     j=0: "xyz" + "cba" = "xyzcba"
 *     j=1: "yz" + "cba" + "x" = "yzcbax"
 *     j=2: "z" + "cba" + "xy" = "zcbaxy"
 *
 * 最终答案: "zyxcba" ✓
 */


/*
 * =============================================
 * 解法二: 带剪枝的优化版
 * =============================================
 *
 * 优化: 字典序最大的字符串一定以最大字符开头
 * 对于切割字符串, 只考虑以最大字符开头的切割位置
 * 跳过不可能成为最优的位置
 *
 * 最坏情况仍然 O(L^2) (所有字符相同), 但实际快很多
 */

class Solution2 {
public:
    string splitLoopedString(vector<string>& strs) {
        int n = strs.size();

        // 预处理: 贪心选最大方向
        for (auto& s : strs) {
            string rev(s.rbegin(), s.rend());
            if (rev > s) s = rev;
        }

        string result = "";

        for (int i = 0; i < n; i++) {
            // 构造其他部分 (环形顺序)
            string other;
            for (int k = i + 1; k < n; k++) other += strs[k];
            for (int k = 0; k < i; k++) other += strs[k];

            string rev(strs[i].rbegin(), strs[i].rend());

            for (const string& cur : {strs[i], rev}) {
                // 找到当前字符串中最大的字符
                char maxChar = *max_element(cur.begin(), cur.end());

                for (int j = 0; j < (int)cur.size(); j++) {
                    // 剪枝: 只考虑以最大字符开头的位置
                    // 以较小字符开头的候选不可能比以最大字符开头的更大
                    if (cur[j] < maxChar) continue;

                    string candidate = cur.substr(j) + other + cur.substr(0, j);
                    if (candidate > result) {
                        result = candidate;
                    }
                }
            }
        }

        return result;
    }
};


/*
 * =============================================
 * trace: strs = ["abc"]
 * =============================================
 *
 * 预处理: "abc" vs "cba" → "cba"
 * strs = ["cba"]
 *
 * i=0: other = "" (只有一个字符串)
 *   正序 "cba":
 *     j=0: "cba" + "" = "cba"
 *     j=1: "ba" + "c" = "bac"
 *     j=2: "a" + "cb" = "acb"
 *   反转 "abc":
 *     j=0: "abc" = "abc"
 *     j=1: "bc" + "a" = "bca"
 *     j=2: "c" + "ab" = "cab"
 *
 * 最大: "cba" ✓
 *
 *
 * =============================================
 * 解法对比
 * =============================================
 *
 * | 解法       | 时间       | 空间  | 特点               |
 * |-----------|-----------|-------|-------------------|
 * | 贪心+枚举  | O(L^2)    | O(L)  | 清晰, 面试推荐      |
 * | 带剪枝     | O(L^2)最坏 | O(L)  | 实际更快, 跳过无效位置 |
 *
 *
 * =============================================
 * 易错点
 * =============================================
 *
 * 1. 切割字符串也用预处理的固定方向:
 *    ✗ 直接用 max(s, rev(s)) 作为切割串
 *    ✓ 切割串需要同时尝试正序和反转
 *    → 因为切割位置不同, 正序和反转可能产生不同最优结果
 *
 * 2. 只枚举字符串边界切割:
 *    ✗ 只在字符串之间切割 (j=0)
 *    ✓ 必须枚举字符串内部每个字符位置 (j=0..len-1)
 *    → 最优切割点可能在字符串中间
 *
 * 3. 环形拼接顺序错误:
 *    ✗ 简单拼接 strs[0] + strs[1] + ... + strs[n-1]
 *    ✓ 切割 i 后: strs[i+1], ..., strs[n-1], strs[0], ..., strs[i-1]
 *    → 保持环形顺序, 切割点右边的在前
 *
 * 4. result 初始值:
 *    ✗ 初始化为某个候选值 (可能是非最优的)
 *    ✓ result = "" (空串字典序比任何非空串小, 安全)
 *
 * 5. 预处理修改 strs 后忘记对切割串重新计算 rev:
 *    ✗ 用预处理前的原始串
 *    ✓ 用 strs[i] (已被预处理) 的反转
 *
 *
 * =============================================
 * 面试追问
 * =============================================
 *
 * Q1: 时间复杂度为什么可接受?
 *     → L <= 1000, O(L^2) = O(10^6), 完全可以
 *     → 每次构造候选 O(L), 枚举 L 个切割点, 总计 O(L^2)
 *
 * Q2: 为什么非切割串可以贪心选最大方向?
 *     → 非切割串出现在结果的"中间段"
 *     → 更大的中间段在字典序比较中永远不会更差
 *     → 严格证明: prefix 相同时比较取决于中间段; prefix 不同时中间段不影响
 *
 * Q3: 如果字符串可以重新排序, 怎么做?
 *     → 全排列 * 方向 * 切割点, 搜索空间 O(n! * 2^n * L)
 *     → n=1000 时完全不可行, 需要更高级的贪心或 NP-hard 证明
 *
 * 关联题目:
 *   796. Rotate String — 环形字符串旋转
 *   899. Orderly Queue — 字典序最小 + 旋转
 *   1163. Last Substring in Lexicographic Order — 字典序最大
 *   151. Reverse Words in a String — 字符串翻转
 *   344. Reverse String — 基础字符串反转
 */
