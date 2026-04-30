// LeetCode 848: Shifting Letters
// 解法一：后缀和从后往前（推荐）
// 时间 O(n)，空间 O(1)
// 从后往前累加移位量，原地修改字符串

class Solution {
public:
    string shiftingLetters(string s, vector<int>& shifts) {
        int n = s.size();
        long long total = 0;

        // 从后往前遍历
        for (int i = n - 1; i >= 0; --i) {
            total += shifts[i];
            // 对 26 取模避免不必要的大数运算
            int shift = total % 26;
            // 应用移位
            s[i] = 'a' + (s[i] - 'a' + shift) % 26;
        }

        return s;
    }
};

// ============================================================
// 解法二：先计算总和再从前往后减
// 时间 O(n)，空间 O(1)
// 先求所有 shifts 之和，然后从前往后，每处理一个字符后减去 shifts[i]

class Solution2 {
public:
    string shiftingLetters(string s, vector<int>& shifts) {
        int n = s.size();

        // 计算总移位量（对 26 取模避免溢出问题）
        long long total = 0;
        for (int i = 0; i < n; ++i) {
            total += shifts[i];
        }

        // 从前往后遍历
        for (int i = 0; i < n; ++i) {
            int shift = (int)(total % 26);
            s[i] = 'a' + (s[i] - 'a' + shift) % 26;
            total -= shifts[i];
        }

        return s;
    }
};

// ============================================================
// 解法三：构建后缀和数组（显式）
// 时间 O(n)，空间 O(n)
// 先将 shifts 转为后缀和数组，再逐个字符应用

class Solution3 {
public:
    string shiftingLetters(string s, vector<int>& shifts) {
        int n = s.size();

        // 构建后缀和数组（原地修改 shifts 或用新数组）
        vector<long long> suffixSum(n);
        suffixSum[n - 1] = shifts[n - 1];
        for (int i = n - 2; i >= 0; --i) {
            suffixSum[i] = suffixSum[i + 1] + shifts[i];
        }

        // 应用移位
        for (int i = 0; i < n; ++i) {
            int shift = (int)(suffixSum[i] % 26);
            s[i] = 'a' + (s[i] - 'a' + shift) % 26;
        }

        return s;
    }
};

// ============================================================
// 解法四：原地修改 shifts 数组为后缀和
// 时间 O(n)，空间 O(1)
// 直接在 shifts 数组上做后缀和，节省空间

class Solution4 {
public:
    string shiftingLetters(string s, vector<int>& shifts) {
        int n = s.size();

        // 从倒数第二个开始累加，将 shifts 转为后缀和
        // 注意：原数组元素可能很大，累加可能溢出 int
        // 但我们只关心 mod 26 的结果
        // 为安全起见，边累加边取模
        for (int i = n - 2; i >= 0; --i) {
            shifts[i] = (int)(((long long)shifts[i] + shifts[i + 1]) % 26);
        }

        // 应用移位
        for (int i = 0; i < n; ++i) {
            s[i] = 'a' + (s[i] - 'a' + shifts[i] % 26) % 26;
        }

        return s;
    }
};

// ============================================================
// 暴力解法（仅供理解题意，会超时）
// 时间 O(n^2)，空间 O(1)

class SolutionBrute {
public:
    string shiftingLetters(string s, vector<int>& shifts) {
        int n = s.size();

        for (int i = 0; i < n; ++i) {
            int x = shifts[i];
            // 将 s[0..i] 各右移 x 位
            for (int j = 0; j <= i; ++j) {
                s[j] = 'a' + (s[j] - 'a' + x) % 26;
            }
        }

        return s;
    }
};
