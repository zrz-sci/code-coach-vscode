// LeetCode 686: Repeated String Match
// 解法一: 暴力重复 + 子串查找
// 解法二: KMP 在虚拟无限重复串上匹配
// 解法三: Rabin-Karp 滚动哈希

// ============================================================
// 解法一: 暴力重复 + find
// 时间: O((n+m) * m) 最坏, 空间: O(n * ceil(m/n))
//
// 思路: 把 a 重复到长度 >= len(b)，检查 b 是否为子串
// 不是则再多加一个 a，仍不是则返回 -1
// ============================================================

class Solution {
public:
    int repeatedStringMatch(string a, string b) {
        // 字符集预检查: b 中有 a 中不存在的字符则直接返回 -1
        vector<bool> inA(26, false);
        for (char c : a) inA[c - 'a'] = true;
        for (char c : b) {
            if (!inA[c - 'a']) return -1;
        }

        int n = a.size(), m = b.size();
        int minRepeats = (m + n - 1) / n; // ceil(m / n)

        // 构造重复字符串
        string repeated;
        repeated.reserve((long long)n * (minRepeats + 1));
        for (int i = 0; i < minRepeats; i++) {
            repeated += a;
        }

        // 检查 b 是否是 repeated 的子串
        if (repeated.find(b) != string::npos) {
            return minRepeats;
        }

        // 多拼一个 a 处理边界跨越情况
        repeated += a;
        if (repeated.find(b) != string::npos) {
            return minRepeats + 1;
        }

        return -1;
    }
};

// ============================================================
// 解法二: KMP 在虚拟无限重复串上匹配
// 时间: O(n + m), 空间: O(m)
//
// 思路: 不真正构造重复串，用 i % n 访问虚拟文本的字符
// 在虚拟文本上运行 KMP，匹配成功则根据位置算重复次数
// ============================================================

class Solution2 {
public:
    int repeatedStringMatch(string a, string b) {
        int n = a.size(), m = b.size();

        // 字符集预检查
        vector<bool> inA(26, false);
        for (char c : a) inA[c - 'a'] = true;
        for (char c : b) {
            if (!inA[c - 'a']) return -1;
        }

        // === 构建 KMP 的 failure (next) 数组 ===
        // fail[i] = b[0..i] 的最长真前后缀长度
        vector<int> fail(m, 0);
        for (int i = 1; i < m; i++) {
            int j = fail[i - 1];
            while (j > 0 && b[i] != b[j]) {
                j = fail[j - 1];
            }
            if (b[i] == b[j]) j++;
            fail[i] = j;
        }

        // === 在虚拟无限重复串上做 KMP 匹配 ===
        // 虚拟文本长度上限: n * (ceil(m/n) + 1)
        int textLen = n * ((m + n - 1) / n + 1);
        int j = 0; // b 中的匹配位置

        for (int i = 0; i < textLen; i++) {
            char textChar = a[i % n]; // 虚拟文本的第 i 个字符

            while (j > 0 && textChar != b[j]) {
                j = fail[j - 1];
            }
            if (textChar == b[j]) j++;

            if (j == m) {
                // 匹配成功! b 出现在虚拟文本的位置 [i-m+1, i]
                // 需要的重复次数 = 覆盖到位置 i 需要多少个完整的 a
                int repeats = i / n + 1;
                return repeats;
            }
        }

        return -1;
    }
};

// ============================================================
// 解法三: Rabin-Karp 滚动哈希
// 时间: O(n + m) 期望, 空间: O(1) 额外
//
// 思路: 在虚拟重复串上维护长度为 m 的滑动窗口哈希
// 窗口哈希与 b 的哈希匹配时逐字符验证
// ============================================================

class Solution3 {
public:
    int repeatedStringMatch(string a, string b) {
        int n = a.size(), m = b.size();

        // 字符集预检查
        vector<bool> inA(26, false);
        for (char c : a) inA[c - 'a'] = true;
        for (char c : b) {
            if (!inA[c - 'a']) return -1;
        }

        long long MOD = 1e9 + 7;
        long long BASE = 31;

        // 计算 b 的哈希值
        long long hashB = 0;
        long long power = 1;
        for (int i = m - 1; i >= 0; i--) {
            hashB = (hashB + (long long)(b[i] - 'a' + 1) * power) % MOD;
            if (i > 0) power = power * BASE % MOD;
        }
        // power 现在等于 BASE^(m-1)

        // 在虚拟重复串上滑动窗口
        int textLen = n * ((m + n - 1) / n + 1);
        long long hashWin = 0;

        for (int i = 0; i < textLen; i++) {
            char c = a[i % n];

            if (i < m) {
                // 初始化阶段: 逐步构建窗口哈希
                hashWin = (hashWin * BASE + (c - 'a' + 1)) % MOD;
            } else {
                // 滑动阶段: 移除最高位，加入新最低位
                char old_c = a[(i - m) % n];
                hashWin = (hashWin - (long long)(old_c - 'a' + 1) * power % MOD + MOD) % MOD;
                hashWin = (hashWin * BASE + (c - 'a' + 1)) % MOD;
            }

            // 窗口长度达到 m 时检查
            if (i >= m - 1 && hashWin == hashB) {
                // 逐字符验证避免哈希碰撞
                bool match = true;
                int start = i - m + 1;
                for (int j = 0; j < m; j++) {
                    if (a[(start + j) % n] != b[j]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    return i / n + 1; // 重复次数
                }
            }
        }

        return -1;
    }
};
