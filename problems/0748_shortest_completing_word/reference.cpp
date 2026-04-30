// 748. Shortest Completing Word
// 难度: Easy
// 标签: Array, Hash Table, String

// ============================================================
// 解法一：频次数组匹配 - 推荐
// 时间: O(P + n * L)  空间: O(1)
// P = licensePlate长度, n = words数量, L = 平均单词长度
// ============================================================
// 1. 解析 licensePlate 得到字母频次数组 need[26]
// 2. 对每个 word 检查是否满足频次要求
// 3. 取最短的满足条件的单词

class Solution {
public:
    string shortestCompletingWord(string licensePlate, vector<string>& words) {
        // 步骤1：解析 licensePlate，统计字母频次（忽略大小写）
        int need[26] = {};
        for (char c : licensePlate) {
            if (isalpha(c)) {
                need[tolower(c) - 'a']++;
            }
        }

        string result;

        // 步骤2：遍历每个候选单词
        for (const string& word : words) {
            if (isCompleting(word, need)) {
                // 步骤3：选最短的；相同长度取先出现的
                if (result.empty() || word.size() < result.size()) {
                    result = word;
                }
            }
        }

        return result;
    }

private:
    // 检查 word 的字母频次是否满足 need 的要求
    bool isCompleting(const string& word, int need[]) {
        int have[26] = {};
        for (char c : word) {
            have[c - 'a']++;
        }
        // 检查每个字母的频次是否 >= need
        for (int i = 0; i < 26; ++i) {
            if (have[i] < need[i]) {
                return false;
            }
        }
        return true;
    }
};

// ============================================================
// 解法二：使用 unordered_map 的频次匹配
// 时间: O(P + n * L)  空间: O(26) = O(1)
// ============================================================
// 用 map 代替数组，逻辑更直观但常数略大

class Solution {
public:
    string shortestCompletingWord(string licensePlate, vector<string>& words) {
        // 解析 licensePlate
        unordered_map<char, int> need;
        for (char c : licensePlate) {
            if (isalpha(c)) {
                need[tolower(c)]++;
            }
        }

        string result;
        for (const string& word : words) {
            if (matches(word, need)) {
                if (result.empty() || word.size() < result.size()) {
                    result = word;
                }
            }
        }
        return result;
    }

private:
    bool matches(const string& word, const unordered_map<char, int>& need) {
        unordered_map<char, int> have;
        for (char c : word) {
            have[c]++;
        }
        for (auto& [ch, cnt] : need) {
            if (have[ch] < cnt) return false;
        }
        return true;
    }
};

// ============================================================
// 解法三：排序优先 + 提前终止
// 时间: O(n log n + n * L)  空间: O(n)
// ============================================================
// 按长度排序后，第一个满足条件的就是答案
// 注意：必须用稳定排序保持同长度单词的原始顺序

class Solution {
public:
    string shortestCompletingWord(string licensePlate, vector<string>& words) {
        // 解析 licensePlate
        int need[26] = {};
        for (char c : licensePlate) {
            if (isalpha(c)) {
                need[tolower(c) - 'a']++;
            }
        }

        // 创建 (word, 原始下标) 对，按长度稳定排序
        vector<int> indices(words.size());
        iota(indices.begin(), indices.end(), 0);
        stable_sort(indices.begin(), indices.end(), [&](int a, int b) {
            return words[a].size() < words[b].size();
        });

        // 从最短开始找第一个满足条件的
        for (int idx : indices) {
            if (isCompleting(words[idx], need)) {
                return words[idx];
            }
        }

        return "";  // 题目保证有解，不会到这里
    }

private:
    bool isCompleting(const string& word, int need[]) {
        int have[26] = {};
        for (char c : word) {
            have[c - 'a']++;
        }
        for (int i = 0; i < 26; ++i) {
            if (have[i] < need[i]) return false;
        }
        return true;
    }
};

// ============================================================
// 解法四：位掩码加速（适合只判断存在性的简化版本）
// 时间: O(P + n * L)  空间: O(1)
// ============================================================
// 注意：这种方法只能处理频次为1的情况
// 对于频次 > 1 的场景需要扩展，这里展示思路

class Solution {
public:
    string shortestCompletingWord(string licensePlate, vector<string>& words) {
        // 这个解法仍使用频次数组（完整版），但展示一种紧凑的写法
        int need[26] = {};
        int numRequired = 0;  // 需要匹配的字母种类数
        for (char c : licensePlate) {
            if (isalpha(c)) {
                int idx = tolower(c) - 'a';
                if (need[idx] == 0) numRequired++;
                need[idx]++;
            }
        }

        string result;
        for (const string& word : words) {
            // 快速过滤：如果单词长度比当前最佳还长，跳过
            if (!result.empty() && word.size() >= result.size()) continue;

            int have[26] = {};
            int matched = 0;
            bool valid = true;
            for (char c : word) {
                int idx = c - 'a';
                have[idx]++;
                if (have[idx] == need[idx] && need[idx] > 0) {
                    matched++;
                }
            }
            if (matched == numRequired) {
                result = word;
            }
        }
        return result;
    }
};

// ============================================================
// 测试用例分析
// ============================================================
// 输入: licensePlate = "1s3 PSt", words = ["step","steps","stripe","stepple"]
// need: s=2, p=1, t=1
// "step":   s=1,t=1,e=1,p=1 => s不够(1<2) => 不满足
// "steps":  s=2,t=1,e=1,p=1 => 全部满足 => 候选(len=5)
// "stripe": s=1,t=1,r=1,i=1,p=1,e=1 => s不够 => 不满足
// "stepple":s=1,t=1,e=2,p=2,l=1 => s不够 => 不满足
// 答案: "steps"  正确
//
// 输入: licensePlate = "1s3 456", words = ["looks","pest","stew","show"]
// need: s=1
// "looks": s=1 => 满足(len=5)
// "pest":  s=1 => 满足(len=4) => 更短，更新
// "stew":  s=1 => 满足(len=4) => 同长度但不是更短，不更新
// "show":  s=1 => 满足(len=4) => 同长度，不更新
// 答案: "pest"  正确
