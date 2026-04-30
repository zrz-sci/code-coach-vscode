/*
 * LeetCode 869: Reordered Power of 2
 * 解法: 数字指纹比较（排序字符串 / 计数数组）
 *
 * 核心思路: 两个数互为重排列 ⟺ 它们的数字计数完全相同。
 *          预计算所有 2^0 ~ 2^30 的数字指纹，与 n 的指纹比较。
 *
 * 时间复杂度: O(31 * d) 其中 d 为数字位数，约 O(310)
 * 空间复杂度: O(1)
 */

// ===================== 解法一：排序字符串指纹 =====================

class Solution {
public:
    bool reorderedPowerOf2(int n) {
        // 计算 n 的排序指纹
        string target = getSortedDigits(n);

        // 枚举所有 int 范围内的 2 的幂
        for (int i = 0; i < 31; i++) {
            int powerOf2 = 1 << i;
            if (getSortedDigits(powerOf2) == target) {
                return true;
            }
        }
        return false;
    }

private:
    // 将数字转为排序后的字符串作为"指纹"
    // 例如 46 -> "46", 64 -> "46", 128 -> "128", 218 -> "128"
    string getSortedDigits(int x) {
        string s = to_string(x);
        sort(s.begin(), s.end());
        return s;
    }
};

// ===================== 解法二：计数数组指纹 =====================
// 用长度 10 的数字计数数组代替排序，理论上更快

class SolutionCounting {
public:
    bool reorderedPowerOf2(int n) {
        // 计算 n 的数字频率
        vector<int> countN = digitCount(n);

        // 枚举所有 2 的幂并比较计数
        for (int i = 0; i < 31; i++) {
            if (digitCount(1 << i) == countN) {
                return true;
            }
        }
        return false;
    }

private:
    // 统计每个数字(0~9)出现的次数
    vector<int> digitCount(int x) {
        vector<int> count(10, 0);
        while (x > 0) {
            count[x % 10]++;
            x /= 10;
        }
        return count;
    }
};

// ===================== 解法三：预计算所有 2 的幂指纹（set 查找） =====================
// 在构造函数中预处理，适合多次查询场景

class SolutionPrecompute {
public:
    // 预计算所有 2 的幂的指纹并存入 set
    set<string> powerFingerprints;

    SolutionPrecompute() {
        for (int i = 0; i < 31; i++) {
            string s = to_string(1 << i);
            sort(s.begin(), s.end());
            powerFingerprints.insert(s);
        }
    }

    bool reorderedPowerOf2(int n) {
        string s = to_string(n);
        sort(s.begin(), s.end());
        return powerFingerprints.count(s) > 0;
    }
};

// ===================== 解法四：全排列暴力（仅供对比，不推荐） =====================
// 枚举 n 的所有数字排列，检查是否为 2 的幂

class SolutionPermutation {
public:
    bool reorderedPowerOf2(int n) {
        string s = to_string(n);
        sort(s.begin(), s.end()); // 先排序以便 next_permutation 枚举所有排列
        do {
            // 跳过前导零
            if (s[0] == '0') continue;

            // 将排列转为数字
            long long num = stoll(s);

            // 检查是否为 2 的幂
            if (num > 0 && (num & (num - 1)) == 0) {
                return true;
            }
        } while (next_permutation(s.begin(), s.end()));

        return false;
    }
};

// ===================== 辅助说明 =====================
/*
 * 2 的幂在 int 范围内的完整列表 (2^0 ~ 2^30):
 * 1, 2, 4, 8, 16, 32, 64, 128, 256, 512,
 * 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288,
 * 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864,
 * 134217728, 268435456, 536870912, 1073741824
 *
 * 判断 x 是否为 2 的幂的位运算技巧:
 * x > 0 && (x & (x - 1)) == 0
 *
 * 为什么指纹方法正确:
 * "重排列后能否构成某个值" ⟺ "两者的数字多重集是否相同"
 * ⟺ "排序后的数字字符串是否相同"
 * ⟺ "每个数字 0~9 的出现次数是否都相同"
 */
