/*
 * 【题目本质】
 * 统计[low, high]范围内中心对称数的个数。
 * 在LC247基础上: 枚举low.length到high.length每个长度,
 * 生成该长度所有中心对称数, 对边界长度做字符串大小比较过滤。
 *
 * 【解法总览】
 * 解法1 (推荐): 枚举长度 + 生成 + 过滤 — O(5^(n/2) * L) / O(5^(n/2))
 * 解法2: 分层计数优化 — 中间长度O(1), 边界O(5^(n/2))
 */

// ===================== 解法1: 枚举长度 + 生成 + 过滤 (推荐) =====================
// 思路: 对每个可能长度生成所有中心对称数, 用字符串比较过滤范围
class Solution1 {
public:
    int strobogrammaticInRange(string low, string high) {
        int count = 0;
        for (int len = low.size(); len <= (int)high.size(); len++) {
            vector<string> nums = generate(len, len);
            for (const string& num : nums) {
                // 关键: 同长度字符串字典序 == 数值序
                // 不同长度时用长度判断即可 (短的一定更小)
                if ((len > (int)low.size() || num >= low) &&
                    (len < (int)high.size() || num <= high)) {
                    count++;
                }
            }
        }
        return count;
    }

private:
    // 复用 LC247 的生成逻辑
    vector<string> generate(int n, int target) {
        if (n == 0) return {""};
        if (n == 1) return {"0", "1", "8"};

        vector<string> middles = generate(n - 2, target);
        vector<string> result;

        for (const string& mid : middles) {
            if (n != target) result.push_back("0" + mid + "0");
            result.push_back("1" + mid + "1");
            result.push_back("6" + mid + "9");
            result.push_back("8" + mid + "8");
            result.push_back("9" + mid + "6");
        }
        return result;
    }
};

// ===================== 解法2: 分层计数优化 =====================
// 思路: 中间长度用公式O(1)计算, 仅边界长度生成具体数字
class Solution2 {
public:
    int strobogrammaticInRange(string low, string high) {
        int count = 0;
        int lo = low.size(), hi = high.size();

        for (int len = lo; len <= hi; len++) {
            if (len > lo && len < hi) {
                // 中间长度: 公式计算 (无需生成)
                count += countForLength(len);
            } else {
                // 边界长度: 生成 + 过滤
                vector<string> nums = generate(len, len);
                for (const string& num : nums) {
                    if ((len > lo || num >= low) && (len < hi || num <= high)) {
                        count++;
                    }
                }
            }
        }
        return count;
    }

private:
    // n位中心对称数的个数 (不含前导零)
    int countForLength(int n) {
        if (n == 1) return 3;  // 0, 1, 8
        // 最外层: 4种 (1,6,8,9)
        // 中间每层: 5种 (0,1,6,8,9)
        // 中心(奇数): 3种 (0,1,8)
        int result = 4;
        for (int i = 0; i < n / 2 - 1; i++) result *= 5;
        if (n % 2 == 1) result *= 3;
        return result;
    }

    vector<string> generate(int n, int target) {
        if (n == 0) return {""};
        if (n == 1) return {"0", "1", "8"};
        vector<string> middles = generate(n - 2, target);
        vector<string> result;
        for (const string& mid : middles) {
            if (n != target) result.push_back("0" + mid + "0");
            result.push_back("1" + mid + "1");
            result.push_back("6" + mid + "9");
            result.push_back("8" + mid + "8");
            result.push_back("9" + mid + "6");
        }
        return result;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | 全量生成+过滤 | O(5^(n/2)*L) | O(5^(n/2)) | 简单直接 |
 * | 分层计数优化 | 边界O(5^(n/2)) | 边界O(5^(n/2)) | 中间长度O(1) |
 *
 * 【易错点】
 * 1. ✗ 用 stoi() 转数值 → ✓ 用字符串字典序比较
 *    数值可达10^15, 超出int甚至long范围
 * 2. ✗ 仅检查 num >= low → ✓ 需要结合长度条件
 *    "9" > "50" (字典序) 但 9 < 50 (数值), 不同长度不能直接比较
 * 3. ✗ 忘记 low.size() == high.size() → ✓ 条件表达式用 OR 覆盖
 *    此时一个数需同时满足 >= low 且 <= high
 * 4. ✗ int 与 size_t 混用 → ✓ 显式转 (int)low.size()
 *
 * 【面试追问】
 * Q1: 字符串比较局限? → 仅同长度无前导零时有效
 * Q2: low/high长度差距大如何优化? → 中间长度用公式O(1)计算
 * Q3: n位中心对称数公式? → 偶数: 4*5^(n/2-1), 奇数: 4*3*5^((n-3)/2)
 */
