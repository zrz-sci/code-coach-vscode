// 728. Self Dividing Numbers
// 难度: Easy
// 标签: Math

// ===================== 解法一：暴力枚举 + 取模逐位检查（推荐） =====================
// 时间 O((right - left + 1) * D)  空间 O(1)
// D 为最大位数，本题最多 5 位
// 思路：对范围内每个数，提取各位数字检查是否满足自除数条件
class Solution {
public:
    vector<int> selfDividingNumbers(int left, int right) {
        vector<int> result;
        for (int num = left; num <= right; num++) {
            if (isSelfDividing(num)) {
                result.push_back(num);
            }
        }
        return result;
    }

private:
    bool isSelfDividing(int num) {
        int tmp = num;
        while (tmp > 0) {
            int digit = tmp % 10;
            // 包含 0 或者不能被该位整除，则不是自除数
            if (digit == 0 || num % digit != 0) {
                return false;
            }
            tmp /= 10;
        }
        return true;
    }
};

// ===================== 解法二：字符串转换法 =====================
// 时间 O((right - left + 1) * D)  空间 O(D)
// 思路：将数字转为字符串，遍历每个字符进行检查
class Solution2 {
public:
    vector<int> selfDividingNumbers(int left, int right) {
        vector<int> result;
        for (int num = left; num <= right; num++) {
            if (isSelfDividing(num)) {
                result.push_back(num);
            }
        }
        return result;
    }

private:
    bool isSelfDividing(int num) {
        string s = to_string(num);
        for (char c : s) {
            int digit = c - '0';
            if (digit == 0 || num % digit != 0) {
                return false;
            }
        }
        return true;
    }
};

// ===================== 解法三：Lambda 风格简洁写法 =====================
// 时间 O((right - left + 1) * D)  空间 O(1)
// 思路：使用 lambda 和算法库实现更简洁的代码
class Solution3 {
public:
    vector<int> selfDividingNumbers(int left, int right) {
        vector<int> result;

        auto check = [](int num) -> bool {
            for (int tmp = num; tmp > 0; tmp /= 10) {
                int d = tmp % 10;
                if (d == 0 || num % d != 0) return false;
            }
            return true;
        };

        for (int num = left; num <= right; num++) {
            if (check(num)) {
                result.push_back(num);
            }
        }
        return result;
    }
};

// ===================== 解法四：跳过包含 0 的数优化 =====================
// 时间 O((right - left + 1) * D)  空间 O(1)
// 思路：如果个位为 0 可以直接跳过，减少一些检查
//       进一步可以在发现任意一位为 0 时立即跳过整个十位段
class Solution4 {
public:
    vector<int> selfDividingNumbers(int left, int right) {
        vector<int> result;
        for (int num = left; num <= right; num++) {
            // 快速跳过个位为 0 的数
            if (num % 10 == 0) continue;

            bool valid = true;
            int tmp = num;
            while (tmp > 0) {
                int digit = tmp % 10;
                if (digit == 0 || num % digit != 0) {
                    valid = false;
                    break;
                }
                tmp /= 10;
            }
            if (valid) {
                result.push_back(num);
            }
        }
        return result;
    }
};

// ===================== 解法五：位操作辅助检查 =====================
// 时间 O((right - left + 1) * D)  空间 O(1)
// 思路：用 bitmask 记录哪些数字出现过，然后只检查出现过的数字。
//       对于本题范围没有本质加速，但展示了位操作思路。
class Solution5 {
public:
    vector<int> selfDividingNumbers(int left, int right) {
        vector<int> result;
        for (int num = left; num <= right; num++) {
            int mask = 0;
            int tmp = num;
            bool hasZero = false;

            // 第一遍：收集所有出现的数字
            while (tmp > 0) {
                int d = tmp % 10;
                if (d == 0) { hasZero = true; break; }
                mask |= (1 << d);
                tmp /= 10;
            }
            if (hasZero) continue;

            // 第二遍：检查每个出现的数字是否能整除 num
            bool valid = true;
            for (int d = 1; d <= 9 && valid; d++) {
                if (mask & (1 << d)) {
                    if (num % d != 0) valid = false;
                }
            }
            if (valid) result.push_back(num);
        }
        return result;
    }
};

/*
 * 总结：
 * - 解法一（取模逐位检查）是面试中最推荐的写法：清晰、简洁、无额外空间
 * - 解法二（字符串法）代码更短但有转换开销
 * - 解法三（Lambda）展示现代 C++ 风格
 * - 解法四/五展示微优化思路，面试中提及即可
 *
 * 关键技巧：
 * 1. tmp % 10 取个位，tmp /= 10 去掉个位
 * 2. 先检查 digit == 0 防止除以零
 * 3. 用 num % digit 检查整除性（使用原始值而非 tmp）
 * 4. 封装 isSelfDividing 辅助函数保持主函数整洁
 */
