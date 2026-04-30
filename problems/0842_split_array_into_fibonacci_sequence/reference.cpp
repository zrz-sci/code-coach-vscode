// LeetCode 842: Split Array into Fibonacci Sequence
// 解法一：枚举前两个数 + 贪心验证（推荐）
// 时间 O(n^2 * n)，空间 O(n)
// 枚举 f[0] 和 f[1] 的所有可能，然后确定性地验证剩余部分

class Solution {
public:
    vector<int> splitIntoFibonacci(string num) {
        int n = num.size();
        // 枚举第一个数的长度 [1, n/2]
        for (int i = 1; i <= n / 2; ++i) {
            // 前导零检查：长度>1 且以 '0' 开头则跳过
            if (i > 1 && num[0] == '0') break;
            long long f0 = stoll(num.substr(0, i));
            if (f0 > INT_MAX) break;

            // 枚举第二个数的长度
            for (int j = 1; j <= n / 2; ++j) {
                if (j > 1 && num[i] == '0') break;
                long long f1 = stoll(num.substr(i, j));
                if (f1 > INT_MAX) break;

                // 贪心验证剩余部分
                vector<int> result;
                result.push_back((int)f0);
                result.push_back((int)f1);

                int pos = i + j; // 当前扫描位置
                long long prev1 = f0, prev2 = f1;
                bool valid = true;

                while (pos < n) {
                    long long sum = prev1 + prev2;
                    if (sum > INT_MAX) { valid = false; break; }
                    string sumStr = to_string(sum);
                    // 检查字符串从 pos 开始是否匹配 sumStr
                    if (pos + (int)sumStr.size() > n) {
                        valid = false;
                        break;
                    }
                    if (num.substr(pos, sumStr.size()) != sumStr) {
                        valid = false;
                        break;
                    }
                    result.push_back((int)sum);
                    pos += sumStr.size();
                    prev1 = prev2;
                    prev2 = sum;
                }

                if (valid && pos == n && result.size() >= 3) {
                    return result;
                }
            }
        }
        return {};
    }
};

// ============================================================
// 解法二：回溯 DFS
// 时间 O(n * 2^n)（带剪枝远快于此），空间 O(n)
// 使用标准回溯框架，每步选择下一个数的长度

class Solution2 {
public:
    vector<int> splitIntoFibonacci(string num) {
        vector<int> result;
        backtrack(num, 0, result);
        return result;
    }

private:
    bool backtrack(const string& num, int start, vector<int>& result) {
        // 终止条件：扫描完毕且序列长度 >= 3
        if (start == (int)num.size()) {
            return result.size() >= 3;
        }

        long long curr = 0;
        for (int i = start; i < (int)num.size(); ++i) {
            // 前导零：只允许单独的 "0"
            if (i > start && num[start] == '0') break;

            curr = curr * 10 + (num[i] - '0');
            // 溢出检查
            if (curr > INT_MAX) break;

            int sz = result.size();
            if (sz >= 2) {
                long long expected = (long long)result[sz - 2] + result[sz - 1];
                if (curr > expected) break;  // 已经超过，后续更大，剪枝
                if (curr < expected) continue; // 还不够大，继续添加数字
            }

            // curr == expected (或序列长度 < 2)
            result.push_back((int)curr);
            if (backtrack(num, i + 1, result)) return true;
            result.pop_back();
        }
        return false;
    }
};

// ============================================================
// 解法三：迭代确定性验证（更简洁的写法）
// 时间 O(n^2 * n)，空间 O(n)
// 与解法一思路相同，但使用 compare 避免 substr 开销

class Solution3 {
public:
    vector<int> splitIntoFibonacci(string num) {
        int n = num.size();
        for (int i = 1; i < n && i <= 10; ++i) {
            if (i > 1 && num[0] == '0') break;
            for (int j = 1; i + j < n && j <= 10; ++j) {
                if (j > 1 && num[i] == '0') break;

                vector<int> seq;
                long long a = stoll(num.substr(0, i));
                long long b = stoll(num.substr(i, j));
                if (a > INT_MAX || b > INT_MAX) break;

                seq.push_back((int)a);
                seq.push_back((int)b);
                int k = i + j;

                while (k < n) {
                    long long c = (long long)seq[seq.size() - 2] + seq[seq.size() - 1];
                    if (c > INT_MAX) break;
                    string cs = to_string(c);
                    int len = cs.size();
                    if (k + len > n) break;
                    // 使用 compare 避免创建子串
                    if (num.compare(k, len, cs) != 0) break;
                    seq.push_back((int)c);
                    k += len;
                }

                if (k == n && seq.size() >= 3) return seq;
            }
        }
        return {};
    }
};
