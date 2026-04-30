// 【题目本质】在 [s1,n1] 中最多能匹配多少个 [s2,n2] 的子序列 -> 循环节优化
// 【解法总览】解法1: 循环节检测 O(len1*len2)/O(len2) | 解法2: 预处理跳转 O(len1*len2+n1*len2)/O(len2*26)

// ===================== Solution 1: 循环节检测 =====================
// 思路：每遍历完一个 s1，记录 s2 指针位置 j；j 重复时找到循环节
//       前缀 + 循环体*次数 + 尾部 = 总匹配 s2 数，最后除以 n2
// 时间 O(min(n1, len2) * len1)，空间 O(len2)
class Solution1 {
public:
    int getMaxRepetitions(string s1, int n1, string s2, int n2) {
        if (n1 == 0) return 0;
        int len1 = s1.size(), len2 = s2.size();

        vector<int> s2cnt(n1, 0), s2idx(n1, 0);
        int cnt = 0, j = 0;

        for (int i = 0; i < n1; i++) {
            for (int k = 0; k < len1; k++) {
                if (s1[k] == s2[j]) {
                    j++;
                    if (j == len2) {
                        j = 0;
                        cnt++;
                    }
                }
            }
            s2cnt[i] = cnt;
            s2idx[i] = j;

            // 查找循环
            for (int prev = 0; prev < i; prev++) {
                if (s2idx[prev] == j) {
                    int prefixS1 = prev + 1;
                    int cycleS1 = i - prev;
                    int prefixS2 = s2cnt[prev];
                    int cycleS2 = cnt - s2cnt[prev];

                    int remainS1 = n1 - prefixS1;
                    int totalS2 = prefixS2
                                  + (remainS1 / cycleS1) * cycleS2
                                  + s2cnt[prev + remainS1 % cycleS1] - prefixS2;
                    return totalS2 / n2;
                }
            }
        }
        return s2cnt[n1 - 1] / n2;
    }
};

// ===================== Solution 2: 预处理 + 模拟 =====================
// 思路：预处理 next[j][c] = 从 s2 位置 j 开始在 s1 中匹配字符 c 的下一个位置
//       然后模拟 n1 个 s1，利用预处理跳过逐字符扫描
// 时间 O(len1*26 + n1*len2)，空间 O(len2)
class Solution2 {
public:
    int getMaxRepetitions(string s1, int n1, string s2, int n2) {
        if (n1 == 0) return 0;
        int len1 = s1.size(), len2 = s2.size();

        // 对每个 s2 的起始位置 j，遍历一遍 s1 后 j 变成什么、匹配了几个 s2
        vector<int> nextIdx(len2), countPerRound(len2);

        for (int j = 0; j < len2; j++) {
            int pos = j, count = 0;
            for (int k = 0; k < len1; k++) {
                if (s1[k] == s2[pos]) {
                    pos++;
                    if (pos == len2) {
                        pos = 0;
                        count++;
                    }
                }
            }
            nextIdx[j] = pos;
            countPerRound[j] = count;
        }

        // 模拟 n1 轮，检测循环
        int totalS2 = 0, j = 0;
        unordered_map<int, pair<int,int>> seen; // j -> (round, totalS2)

        for (int i = 0; i < n1; i++) {
            if (seen.count(j)) {
                auto [prevRound, prevCount] = seen[j];
                int cycleLen = i - prevRound;
                int cycleCount = totalS2 - prevCount;
                int remaining = n1 - i;
                totalS2 += (remaining / cycleLen) * cycleCount;
                for (int r = 0; r < remaining % cycleLen; r++) {
                    totalS2 += countPerRound[j];
                    j = nextIdx[j];
                }
                return totalS2 / n2;
            }
            seen[j] = {i, totalS2};
            totalS2 += countPerRound[j];
            j = nextIdx[j];
        }
        return totalS2 / n2;
    }
};

// 【解法对比】
// Solution1: 经典循环节检测，在遍历过程中同步检测，代码紧凑
// Solution2: 预处理每个起始位置的跳转结果，循环检测分离，更模块化

// 【易错点】
// 1. 忘记最后除以 n2 -> 答案是 str2=[s2,n2] 的个数，不是 s2 的个数
// 2. 循环检测比较 cnt 而非 j -> 循环由 s2 指针位置决定
// 3. 尾部计算索引偏移错误 -> 需要 s2cnt[prev + tail] - s2cnt[prev]
// 4. s2 含有 s1 中不存在的字符时未特判 -> 会死循环或返回错误结果

// 【面试追问】
// Q1: 为何一定能找到循环？-> 鸽巢原理，j 只有 len2 种值
// Q2: 最坏需要多少轮 s1 才能检测到循环？-> len2 + 1 轮
// Q3: 如何验证 s2 是否能从 s1 中匹配？-> 预检查 s2 的每个字符都在 s1 中出现
