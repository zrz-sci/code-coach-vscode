// 解法一：贪心 + 位运算
// 时间 O(log n) 平均（每个CIDR块覆盖2的幂个IP），空间 O(1) 额外
// 核心：每次从当前IP出发，取 lowbit 和 n 约束下最大合法块
class Solution {
public:
    vector<string> ipToCIDR(string ip, int n) {
        vector<string> result;

        // 将IP字符串解析为32位无符号整数
        long start = ipToLong(ip);

        while (n > 0) {
            // 计算当前IP能对齐的最大块大小
            // lowbit = start & (-start) 取最低有效位
            // 特殊情况: start == 0 时 lowbit 为 0，此时无对齐限制
            long lowbit = start & (-start);
            if (start == 0) {
                // 没有对齐约束，取足够大的值
                lowbit = (long)1 << 32;
            }

            // 块大小还受 n 的限制，取不超过 n 的最大2的幂
            long blockSize = 1;
            while (blockSize * 2 <= n && blockSize * 2 <= lowbit) {
                blockSize *= 2;
            }

            // 计算前缀长度
            int prefix = 32;
            long temp = blockSize;
            while (temp > 1) {
                temp >>= 1;
                --prefix;
            }

            // 生成CIDR字符串
            result.push_back(longToIp(start) + "/" + to_string(prefix));

            // 推进
            start += blockSize;
            n -= (int)blockSize;
        }

        return result;
    }

private:
    // IP字符串 -> 32位长整数
    long ipToLong(const string& ip) {
        long result = 0;
        int cur = 0;
        for (char c : ip) {
            if (c == '.') {
                result = (result << 8) | cur;
                cur = 0;
            } else {
                cur = cur * 10 + (c - '0');
            }
        }
        result = (result << 8) | cur;
        return result;
    }

    // 32位长整数 -> IP字符串
    string longToIp(long val) {
        return to_string((val >> 24) & 255) + "." +
               to_string((val >> 16) & 255) + "." +
               to_string((val >> 8) & 255) + "." +
               to_string(val & 255);
    }
};

// 解法二：使用内置函数简化（更紧凑）
// 利用 __builtin_ctz 计算尾部零的个数来确定对齐
class Solution2 {
public:
    vector<string> ipToCIDR(string ip, int n) {
        vector<string> result;
        unsigned long start = parseIp(ip);

        while (n > 0) {
            // 当前IP能支持的最大块（对齐约束）
            int trailingZeros = (start == 0) ? 32 : __builtin_ctzll(start);
            long maxBlock = 1L << trailingZeros;

            // 不超过 n 的最大2的幂
            long maxByN = 1;
            while (maxByN * 2 <= n) maxByN *= 2;

            // 取较小者
            long blockSize = min(maxBlock, maxByN);
            int prefix = 32 - countTrailingZeros(blockSize);

            result.push_back(formatIp(start) + "/" + to_string(prefix));
            start += blockSize;
            n -= (int)blockSize;
        }

        return result;
    }

private:
    unsigned long parseIp(const string& ip) {
        unsigned long result = 0;
        int part = 0;
        for (char c : ip) {
            if (c == '.') {
                result = (result << 8) | part;
                part = 0;
            } else {
                part = part * 10 + (c - '0');
            }
        }
        return (result << 8) | part;
    }

    string formatIp(unsigned long val) {
        return to_string((val >> 24) & 0xFF) + "." +
               to_string((val >> 16) & 0xFF) + "." +
               to_string((val >> 8) & 0xFF) + "." +
               to_string(val & 0xFF);
    }

    int countTrailingZeros(long val) {
        int count = 0;
        while (val > 1) {
            val >>= 1;
            ++count;
        }
        return count;
    }
};
