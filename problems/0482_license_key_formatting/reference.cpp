// LeetCode 482: License Key Formatting
// 核心：去掉破折号 + 转大写 + 从后向前每 k 个字符分组
//
// 规则：
// 1. 去掉所有 '-'，小写转大写
// 2. 从右往左每 k 个字符一组
// 3. 第一组可以不足 k 个字符但至少 1 个
// 4. 组间用 '-' 分隔
//
// 时间 O(n)，空间 O(n)

// ==================== 解法1: 反向遍历 + 翻转（推荐）====================
// 从末尾开始遍历，每 k 个字符插入一个 '-'，最后翻转
// 这种方法自动处理了第一组长度不足 k 的情况
class Solution {
public:
    string licenseKeyFormatting(string s, int k) {
        string result;
        int count = 0;  // 当前组已有的字符数

        // 从后往前遍历原始字符串
        for (int i = (int)s.size() - 1; i >= 0; --i) {
            if (s[i] == '-') continue;  // 跳过原有的破折号

            // 每满 k 个字符，插入一个分隔符
            if (count > 0 && count % k == 0) {
                result.push_back('-');
            }

            // 转大写并追加
            result.push_back(toupper(s[i]));
            ++count;
        }

        // 翻转得到正确顺序
        reverse(result.begin(), result.end());
        return result;
    }
};

// ==================== 解法2: 正向构建（先清洗再分组）====================
// 先去掉所有破折号并转大写，再按规则分组
class Solution_Forward {
public:
    string licenseKeyFormatting(string s, int k) {
        // Step 1: 清洗 — 去掉 '-' 并转大写
        string clean;
        for (char c : s) {
            if (c != '-') {
                clean.push_back(toupper(c));
            }
        }

        if (clean.empty()) return "";

        // Step 2: 计算第一组长度
        int len = clean.size();
        int firstGroupLen = len % k;
        if (firstGroupLen == 0) firstGroupLen = k;  // 整除时第一组也是 k 个

        // Step 3: 构建结果
        string result;
        int idx = 0;

        // 添加第一组
        for (int i = 0; i < firstGroupLen; ++i) {
            result.push_back(clean[idx++]);
        }

        // 添加后续各组，每组前加 '-'
        while (idx < len) {
            result.push_back('-');
            for (int i = 0; i < k && idx < len; ++i) {
                result.push_back(clean[idx++]);
            }
        }

        return result;
    }
};

// ==================== 解法3: 使用 count 变量的反向法变体 ====================
// 另一种写法：使用独立的 count 重置逻辑
class Solution_Variant {
public:
    string licenseKeyFormatting(string s, int k) {
        string result;
        int count = 0;

        for (int i = (int)s.size() - 1; i >= 0; --i) {
            if (s[i] == '-') continue;

            // 当积累了 k 个字符时，插入分隔符并重置
            if (count == k) {
                result.push_back('-');
                count = 0;
            }

            result.push_back(toupper(s[i]));
            ++count;
        }

        reverse(result.begin(), result.end());
        return result;
    }
};

// ==================== 示例演练 ====================
// 输入: s = "5F3Z-2e-9-w", k = 4
//
// 反向遍历: w -> 9 -> e -> 2 -> Z -> 3 -> F -> 5
//   count=1: result="W"
//   count=2: result="W9"
//   count=3: result="W9E"
//   count=4: result="W9E2"
//   count=5: 先加 '-' → result="W9E2-", 然后 count=1, result="W9E2-Z"
//   count=2: result="W9E2-Z3"
//   count=3: result="W9E2-Z3F"
//   count=4: result="W9E2-Z3F5"
//
// 翻转: "5F3Z-2E9W" ✓

// ==================== 复杂度分析 ====================
// 时间：O(n) — 遍历一次 + reverse 一次
// 空间：O(n) — 结果字符串
//
// 边界情况：
// - 全是 '-' → 返回 ""
// - 没有 '-' → 正常分组
// - 只有 1 个字符 → 返回该字符（大写）
