// 791. Custom Sort String
// 难度: Medium
// 标签: Hash Table, String, Sorting

// ============================================================
// 解法一：计数排列法 (桶排序思想, 推荐面试首选)
// 时间 O(N + M), 空间 O(26)
// ============================================================
class Solution {
public:
    string customSortString(string order, string s) {
        // 第一步: 统计 s 中每个字符的出现次数
        int count[26] = {};
        for (char c : s) {
            count[c - 'a']++;
        }

        string result;
        result.reserve(s.size());

        // 第二步: 按 order 中的顺序输出
        for (char c : order) {
            // 将字符 c 重复 count[c] 次追加到结果
            result.append(count[c - 'a'], c);
            count[c - 'a'] = 0;  // 清零, 防止第三步重复输出
        }

        // 第三步: 输出不在 order 中的字符
        for (int i = 0; i < 26; ++i) {
            if (count[i] > 0) {
                result.append(count[i], 'a' + i);
            }
        }

        return result;
    }
};

// ============================================================
// 解法二：自定义比较函数排序
// 时间 O(N log N), 空间 O(26)
// ============================================================
class Solution2 {
public:
    string customSortString(string order, string s) {
        // 构建优先级映射: order 中的字符按位置赋值
        // 不在 order 中的字符给默认值 26 (放在最后)
        int priority[26];
        fill(priority, priority + 26, 26);

        for (int i = 0; i < (int)order.size(); ++i) {
            priority[order[i] - 'a'] = i;
        }

        // 按优先级排序 s
        sort(s.begin(), s.end(), [&](char a, char b) {
            return priority[a - 'a'] < priority[b - 'a'];
        });

        return s;
    }
};

// ============================================================
// 解法三：原地构建 (双指针交换思想)
// 时间 O(N * M), 空间 O(1)
// ============================================================
// 遍历 order 中的每个字符 c, 将 s 中所有 c 移到当前位置
// 类似选择排序的思想, 不需要额外空间
class Solution3 {
public:
    string customSortString(string order, string s) {
        int writePos = 0;

        for (char c : order) {
            // 找到 s 中所有等于 c 的字符, 交换到 writePos
            for (int i = writePos; i < (int)s.size(); ++i) {
                if (s[i] == c) {
                    swap(s[writePos], s[i]);
                    writePos++;
                }
            }
        }

        return s;
    }
};

// ============================================================
// 解法四：使用 unordered_map (面试中更通用的写法)
// 时间 O(N + M), 空间 O(N)
// ============================================================
class Solution4 {
public:
    string customSortString(string order, string s) {
        // 统计 s 中每个字符的频次
        unordered_map<char, int> freq;
        for (char c : s) {
            freq[c]++;
        }

        string result;
        result.reserve(s.size());

        // 按 order 的顺序输出
        for (char c : order) {
            if (freq.count(c)) {
                result.append(freq[c], c);
                freq.erase(c);  // 移除已处理的字符
            }
        }

        // 输出剩余字符 (不在 order 中的)
        for (auto& [ch, cnt] : freq) {
            result.append(cnt, ch);
        }

        return result;
    }
};
