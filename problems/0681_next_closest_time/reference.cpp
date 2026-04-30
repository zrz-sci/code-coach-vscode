// LeetCode 681: Next Closest Time - Reference Solution
// 难度: Medium
// 标签: Hash Table, String, Backtracking, Enumeration

// ============================================================
// 解法一: 模拟法 -- 逐分钟推进（推荐面试解法）
// ============================================================
// 核心思路:
//   - 提取当前时间中出现的所有数字（去重）
//   - 从当前时间 +1 分钟开始，每次 +1（模 1440）
//   - 检查新时间的 4 位数字是否都在可用集合中
//   - 第一个满足条件的就是答案，最多循环 1440 次
//
// 时间复杂度: O(1) -- 最多 1440 次迭代，常数
// 空间复杂度: O(1)

class Solution {
public:
    string nextClosestTime(string time) {
        // 步骤1: 提取可用数字
        unordered_set<int> digits;
        for (char c : time) {
            if (c != ':') {
                digits.insert(c - '0');
            }
        }

        // 步骤2: 当前时间转分钟
        int hours = stoi(time.substr(0, 2));
        int mins  = stoi(time.substr(3, 2));
        int cur   = hours * 60 + mins;

        // 步骤3: 逐分钟推进，找下一个合法时间
        for (int delta = 1; delta <= 1440; delta++) {
            int next = (cur + delta) % 1440;
            int h = next / 60;
            int m = next % 60;

            // 检查四位数字是否都在可用集合中
            if (digits.count(h / 10) && digits.count(h % 10) &&
                digits.count(m / 10) && digits.count(m % 10)) {
                // 格式化输出（含前导零）
                char buf[6];
                sprintf(buf, "%02d:%02d", h, m);
                return string(buf);
            }
        }

        // 理论上不会到这里（至少原时间本身满足条件）
        return time;
    }
};

// ============================================================
// 解法二: 枚举法 -- 四重循环枚举所有候选时间
// ============================================================
// 核心思路:
//   - 从时间中提取去重后的数字集合
//   - 四重循环枚举所有 d1 d2 : d3 d4 的组合
//   - 过滤出合法时间（h < 24, m < 60）
//   - 计算每个候选与当前时间的差值（模 1440）
//   - 取差值最小且 > 0 的候选
//
// 时间复杂度: O(1) -- 最多 4^4 = 256 种组合
// 空间复杂度: O(1)

class Solution2 {
public:
    string nextClosestTime(string time) {
        // 提取去重后的数字
        vector<int> digits;
        unordered_set<int> seen;
        for (char c : time) {
            if (c != ':') {
                int d = c - '0';
                if (!seen.count(d)) {
                    seen.insert(d);
                    digits.push_back(d);
                }
            }
        }

        // 当前时间转分钟
        int hours = stoi(time.substr(0, 2));
        int mins  = stoi(time.substr(3, 2));
        int cur   = hours * 60 + mins;

        int bestDiff = 1441;  // 初始化为超过一天
        string bestTime = time;

        // 四重循环枚举
        for (int d1 : digits) {
            for (int d2 : digits) {
                int h = d1 * 10 + d2;
                if (h >= 24) continue;  // 小时不合法
                for (int d3 : digits) {
                    for (int d4 : digits) {
                        int m = d3 * 10 + d4;
                        if (m >= 60) continue;  // 分钟不合法

                        int candidate = h * 60 + m;
                        // 计算正向差值（跨天取模）
                        int diff = (candidate - cur + 1440) % 1440;

                        // diff > 0 排除自身
                        // diff == 0 时表示 24 小时后，差值为 1440
                        if (diff == 0) diff = 1440;

                        if (diff < bestDiff) {
                            bestDiff = diff;
                            char buf[6];
                            sprintf(buf, "%02d:%02d", h, m);
                            bestTime = string(buf);
                        }
                    }
                }
            }
        }

        return bestTime;
    }
};

// ============================================================
// 解法三: 贪心逐位构造（面试加分解法）
// ============================================================
// 核心思路:
//   - 从最低位（分钟个位）开始，尝试用集合中下一个更大的数字替换
//   - 如果能替换，高位保持不变，低位全部填集合中最小的数字
//   - 如果四位都无法替换，说明需要跨天，所有位都填最小数字
//   - 需要对每一位考虑该位的上界约束
//
// 时间复杂度: O(1)
// 空间复杂度: O(1)

class Solution3 {
public:
    string nextClosestTime(string time) {
        // 提取并排序数字
        vector<int> sorted_digits;
        unordered_set<int> seen;
        for (char c : time) {
            if (c != ':') {
                int d = c - '0';
                if (!seen.count(d)) {
                    seen.insert(d);
                    sorted_digits.push_back(d);
                }
            }
        }
        sort(sorted_digits.begin(), sorted_digits.end());
        int minDigit = sorted_digits[0];

        // 当前四位数字
        int d[4] = {time[0]-'0', time[1]-'0', time[3]-'0', time[4]-'0'};

        // 每一位的上界（exclusive）
        // d[0]: 小时十位，最大 2
        // d[1]: 小时个位，d[0]==2 时最大 3，否则最大 9
        // d[2]: 分钟十位，最大 5
        // d[3]: 分钟个位，最大 9
        auto getUpperBound = [&](int pos) -> int {
            if (pos == 0) return 3;  // 十位最多到 2
            if (pos == 1) return (d[0] == 2) ? 4 : 10;
            if (pos == 2) return 6;  // 分钟十位最多到 5
            return 10;  // 分钟个位无限制
        };

        // 从低位到高位，尝试增加
        for (int pos = 3; pos >= 0; pos--) {
            int ub = getUpperBound(pos);
            // 在 sorted_digits 中找比 d[pos] 大且 < ub 的最小值
            int nextDigit = -1;
            for (int x : sorted_digits) {
                if (x > d[pos] && x < ub) {
                    nextDigit = x;
                    break;
                }
            }

            if (nextDigit != -1) {
                d[pos] = nextDigit;
                // 低位全部填最小数字
                for (int j = pos + 1; j < 4; j++) {
                    d[j] = minDigit;
                }
                char buf[6];
                sprintf(buf, "%d%d:%d%d", d[0], d[1], d[2], d[3]);
                return string(buf);
            }
        }

        // 所有位都无法增加，跨天到全最小
        char buf[6];
        sprintf(buf, "%d%d:%d%d", minDigit, minDigit, minDigit, minDigit);
        return string(buf);
    }
};
