/*
 * LeetCode 880: Decoded String at Index
 *
 * 解法：逆向回推法（Reverse Working）
 *
 * 思路：
 * 1. 正向扫描编码字符串，计算解码后的总长度 size
 * 2. 从后向前逆向扫描，利用取模运算缩小 k 的范围
 * 3. 遇到数字 d：size /= d, k %= size
 * 4. 遇到字母且 k == 0 或 k == size：该字母即为答案
 *
 * 时间复杂度：O(n)，n 为编码字符串长度
 * 空间复杂度：O(1)
 */

class Solution {
public:
    string decodeAtIndex(string s, int k) {
        // 第一步：正向扫描计算解码后的总长度
        // 使用 long long 防止溢出（解码后长度可能极大）
        long long size = 0;
        int n = s.size();

        for (int i = 0; i < n; i++) {
            if (isdigit(s[i])) {
                // 数字 d 表示当前内容重复 d 次
                size *= (s[i] - '0');
            } else {
                // 字母使长度加 1
                size++;
            }
        }

        // 第二步：逆向回推，从后往前缩小 k 的范围
        // 核心思想：如果当前解码长度为 size，遇到数字 d，
        // 则重复 d 次前的长度为 size/d，k 在重复段内的等价位置为 k % (size/d)
        for (int i = n - 1; i >= 0; i--) {
            // 先对 k 取模，将 k 约束到当前 size 范围内
            // 这一步很关键：k %= size 将 k 映射到 [0, size-1]
            // 其中 0 代表当前段的最后一个字符
            k %= size;

            if (k == 0 && isalpha(s[i])) {
                // k == 0 说明目标是当前解码串的最后一个字符
                // 如果当前字符是字母，那它就是最后一个添加的字符
                return string(1, s[i]);
            }

            if (isdigit(s[i])) {
                // 遇到数字，收缩 size
                // 数字 d 之前的解码长度为 size / d
                size /= (s[i] - '0');
            } else {
                // 遇到字母，长度减 1
                size--;
            }
        }

        // 理论上不会执行到这里（题目保证有解）
        return "";
    }
};

/*
 * 解法思路详细说明：
 *
 * 示例：s = "leet2code3", k = 10
 *
 * 正向扫描：
 *   l -> size=1
 *   e -> size=2
 *   e -> size=3
 *   t -> size=4
 *   2 -> size=8  (leetleet)
 *   c -> size=9
 *   o -> size=10
 *   d -> size=11
 *   e -> size=12
 *   3 -> size=36
 *
 * 逆向回推（k=10）：
 *   i=9, s[9]='3': k = 10%36 = 10, size = 36/3 = 12
 *   i=8, s[8]='e': k = 10%12 = 10, k!=0, size = 11
 *   i=7, s[7]='d': k = 10%11 = 10, k!=0, size = 10
 *   i=6, s[6]='o': k = 10%10 = 0, k==0 且 isalpha('o') -> 返回 "o"
 *
 * 关键理解：
 *
 * 1. 为什么 k%size==0 时当前字母就是答案？
 *    k%size==0 意味着 k 恰好是 size 的倍数，即目标在当前解码串的末尾。
 *    如果当前字符是字母，那它就是末尾添加的那个字符。
 *
 * 2. 为什么逆向扫描是正确的？
 *    数字 d 代表重复 d 次。位置 k 在长度为 size 的串中，
 *    等价于位置 k%size 在长度为 size 的串中（循环性质）。
 *    收缩 size 后 k 的等价位置在更短的串中，最终定位到具体字母。
 *
 * 3. 边界情况：
 *    - k=1 时，找第一个字母即可（但逆向法统一处理）
 *    - 连续数字如 "a23"：a->1, 2->2, 3->6，逆向时依次收缩
 *    - 字符串以数字结尾：正向扫描后 size 可能很大，逆向第一步会大幅收缩
 */

/*
 * 变体解法：正向模拟（仅当 k 极小时可用，一般会超时）
 *
 * 思路是边解码边计数，达到第 k 个字符时返回。
 * 但由于解码后长度可能达 10^18，此方法不可行。
 * 仅作为对比理解。
 */
class Solution_BruteForce {
public:
    string decodeAtIndex(string s, int k) {
        // 此方法仅用于理解，实际会超时
        // 正确做法是上面的逆向回推法
        string decoded = "";
        for (char c : s) {
            if (isdigit(c)) {
                int d = c - '0';
                string temp = decoded;
                for (int j = 1; j < d; j++) {
                    decoded += temp;
                    if ((int)decoded.size() >= k) {
                        return string(1, decoded[k - 1]);
                    }
                }
            } else {
                decoded += c;
                if ((int)decoded.size() == k) {
                    return string(1, c);
                }
            }
        }
        return string(1, decoded[k - 1]);
    }
};
