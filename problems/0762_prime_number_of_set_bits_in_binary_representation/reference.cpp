// 解法一：位掩码 + __builtin_popcount（最优解）
// 时间 O(right - left)，空间 O(1)
// 核心思路：popcount 最大 20，将 20 以内的质数编码为位掩码实现 O(1) 判定
class Solution {
public:
    int countPrimeSetBits(int left, int right) {
        // 20 以内的质数: 2, 3, 5, 7, 11, 13, 17, 19
        // 将它们编码为位掩码：第 k 位为 1 表示 k 是质数
        // 二进制: 10_1000_1010_0010_1011_00 = 665772
        int primeMask = (1 << 2) | (1 << 3) | (1 << 5) | (1 << 7)
                      | (1 << 11) | (1 << 13) | (1 << 17) | (1 << 19);

        int count = 0;
        for (int i = left; i <= right; ++i) {
            int bits = __builtin_popcount(i);
            // 位掩码判断 bits 是否为质数
            if ((primeMask >> bits) & 1) {
                ++count;
            }
        }
        return count;
    }
};

// 解法二：HashSet 判质数
// 时间 O(right - left)，空间 O(1)
// 核心思路：用 unordered_set 存储质数，语义更清晰
class Solution2 {
public:
    int countPrimeSetBits(int left, int right) {
        // 直接列举 20 以内所有质数
        unordered_set<int> primes = {2, 3, 5, 7, 11, 13, 17, 19};

        int count = 0;
        for (int i = left; i <= right; ++i) {
            int bits = __builtin_popcount(i);
            if (primes.count(bits)) {
                ++count;
            }
        }
        return count;
    }
};

// 解法三：手动 popcount (Brian Kernighan) + 手动质数判断
// 时间 O((right - left) * log(right))，空间 O(1)
// 核心思路：不依赖内置函数，手动实现所有步骤
class Solution3 {
public:
    int countPrimeSetBits(int left, int right) {
        int count = 0;
        for (int i = left; i <= right; ++i) {
            int bits = popcount(i);
            if (isPrime(bits)) {
                ++count;
            }
        }
        return count;
    }

private:
    // Brian Kernighan 算法：每次消除最低位的 1
    // n & (n-1) 会将 n 的最低位 1 变为 0
    int popcount(int n) {
        int cnt = 0;
        while (n) {
            n &= (n - 1);
            ++cnt;
        }
        return cnt;
    }

    // 试除法判断质数
    // 对于本题 n <= 20，实际上就检查几个小数
    bool isPrime(int n) {
        if (n < 2) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        for (int i = 3; i * i <= n; i += 2) {
            if (n % i == 0) return false;
        }
        return true;
    }
};

// 解法四：查表法 popcount + 位掩码
// 时间 O(right - left)，空间 O(1)
// 核心思路：对于不能用 __builtin_popcount 的场景，使用字节查表法
class Solution4 {
public:
    int countPrimeSetBits(int left, int right) {
        int primeMask = (1 << 2) | (1 << 3) | (1 << 5) | (1 << 7)
                      | (1 << 11) | (1 << 13) | (1 << 17) | (1 << 19);

        // 预计算 0~255 每个字节的 popcount
        // table[i] 表示数值 i 的二进制中 1 的个数
        int table[256];
        table[0] = 0;
        for (int i = 1; i < 256; ++i) {
            // i 的 popcount = i 右移 1 位的 popcount + 最低位
            table[i] = table[i >> 1] + (i & 1);
        }

        int count = 0;
        for (int i = left; i <= right; ++i) {
            // 将 32 位拆成 4 个字节分别查表求和
            int bits = table[i & 0xFF]
                     + table[(i >> 8) & 0xFF]
                     + table[(i >> 16) & 0xFF]
                     + table[(i >> 24) & 0xFF];
            if ((primeMask >> bits) & 1) {
                ++count;
            }
        }
        return count;
    }
};
