/*
 * LeetCode 249: 移位字符串分组 (Group Shifted Strings)
 *
 * 【题目本质】
 * 分组问题：找到每个移位序列的"不变量"作为哈希 key，相同 key 的字符串归为一组。
 * 
 * 【解法总览】
 * 解法1: 暴力两两比较       — O(n²·L) / O(n·L) — 最直觉
 * 解法2: 差值序列编码+哈希表 — O(n·L) / O(n·L) — 面试首选
 * 解法3: 归一化编码+哈希表   — O(n·L) / O(n·L) — 代码最简洁
 */

// ============================================================
// 解法1: 暴力两两比较
// 时间: O(n²·L)  空间: O(n·L)
//
// 【思路】
// 最朴素的想法：遍历每对字符串，判断是否属于同一移位序列。
// 判断方法：长度必须相同，且每个位置的偏移量（b[i]-a[i]）一致。
// 
// 瓶颈：n² 次配对，每次比较 O(L)。
// 当 n 大时效率低。
//
// 可视化判断过程:
//   "abc" vs "bcd":
//   偏移: b-a=1, c-b=1, d-c=1 → 全部相同 → 同组 ✓
//
//   "abc" vs "acef":
//   长度不同(3 vs 4) → 直接跳过 ✗
//
//   "az" vs "ba":
//   偏移: b-a=1, a-z=(1-25+26)%26=... 
//   实际: 'b'-'a'=1, 'a'-'z'=-25 → (-25+26)%26=1 → 相同 → 同组 ✓
// ============================================================
class Solution1 {
public:
    bool isSameGroup(const string& a, const string& b) {
        if (a.size() != b.size()) return false;
        if (a.empty()) return true;
        // 计算统一偏移量
        int shift = ((b[0] - a[0]) % 26 + 26) % 26;
        for (int i = 1; i < (int)a.size(); i++) {
            // 每个位置的偏移量都必须和第一个位置一样
            if (((b[i] - a[i]) % 26 + 26) % 26 != shift) return false;
        }
        return true;
    }
    
    vector<vector<string>> groupStrings(vector<string>& strings) {
        int n = strings.size();
        vector<bool> used(n, false);
        vector<vector<string>> result;
        
        for (int i = 0; i < n; i++) {
            if (used[i]) continue;
            vector<string> group = {strings[i]};
            for (int j = i + 1; j < n; j++) {
                if (!used[j] && isSameGroup(strings[i], strings[j])) {
                    group.push_back(strings[j]);
                    used[j] = true;
                }
            }
            result.push_back(group);
        }
        return result;
    }
};

// ============================================================
// 解法2: 差值序列编码 + 哈希表 ⭐ 面试首选
// 时间: O(n·L)  空间: O(n·L)
//
// 【思路】
// 解法1的瓶颈是 O(n²) 配对。
// 
// 关键洞察：同一移位序列中，相邻字符的差值(mod 26)是不变量。
// 比如 "abc" → diff=[1,1], "bcd" → diff=[1,1], "xyz" → diff=[1,1]
// 
// 把差值序列编码成字符串作为哈希表的 key，
// 一次遍历就能把所有字符串分组。
//
// 可视化:
//   字符串    →  差值序列(mod 26)  →  key
//   "abc"    →  [1, 1]           →  "1,1,"
//   "bcd"    →  [1, 1]           →  "1,1,"     ← 同 key，同组！
//   "xyz"    →  [1, 1]           →  "1,1,"     ← 同 key，同组！
//   "acef"   →  [2, 2, 1]        →  "2,2,1,"
//   "az"     →  [25]             →  "25,"
//   "ba"     →  [25]             →  "25,"      ← 和 "az" 同组！
//   "a"      →  []               →  ""
//   "z"      →  []               →  ""         ← 和 "a" 同组！
// ============================================================
class Solution2 {
public:
    vector<vector<string>> groupStrings(vector<string>& strings) {
        unordered_map<string, vector<string>> groups;
        
        for (const string& s : strings) {
            string key = "";
            for (int i = 1; i < (int)s.size(); i++) {
                // 为什么 +26 再 %26：C++ 中负数 % 26 仍为负数
                // 例如 'a'-'z' = -25, (-25%26) = -25, (-25+26)%26 = 1 ✓
                int diff = ((s[i] - s[i - 1]) % 26 + 26) % 26;
                // 为什么要加逗号：防止 [1,12] 和 [11,2] 都变成 "112"
                key += to_string(diff) + ",";
            }
            groups[key].push_back(s);
        }
        
        vector<vector<string>> result;
        for (auto& [key, group] : groups) {
            result.push_back(move(group));
        }
        return result;
    }
};

// ============================================================
// 解法3: 归一化编码 + 哈希表
// 时间: O(n·L)  空间: O(n·L)
//
// 【思路】
// 换一种编码方式：把每个字符串的首字母"对齐"到 'a'。
// 同组字符串归一化后完全相同。
//
// 例如：
//   "abc" → 首字母 'a'，偏移 0 → 归一化 "abc"
//   "bcd" → 首字母 'b'，偏移 1 → 每个字母减1 → "abc"  ← 相同！
//   "xyz" → 首字母 'x'，偏移 23 → 每个字母减23 → "abc" ← 相同！
//   "az"  → 首字母 'a'，偏移 0 → 归一化 "az"
//   "ba"  → 首字母 'b'，偏移 1 → 'b'-1='a', 'a'-1='z' → "az" ← 和 "az" 相同！
//
// 这种方法的 key 是可读的字符串，debug 更方便。
// ============================================================
class Solution3 {
public:
    vector<vector<string>> groupStrings(vector<string>& strings) {
        unordered_map<string, vector<string>> groups;
        
        for (const string& s : strings) {
            string key = s;
            int shift = s[0] - 'a';  // 首字母到 'a' 的距离
            for (char& c : key) {
                // 为什么不能直接 c - shift：当 c < 'a' + shift 时会变成负数
                // 例如 "ba" 中 'a' - 1 = -1，需要回绕到 'z'
                c = (c - 'a' - shift + 26) % 26 + 'a';
            }
            groups[key].push_back(s);
        }
        
        vector<vector<string>> result;
        for (auto& [key, group] : groups) {
            result.push_back(move(group));
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 vs 解法2/3: 
//   暴力 O(n²) 配对 → 哈希 O(n) 分组，核心突破是找到"移位不变量"。
//
// 解法2 vs 解法3:
//   本质相同，都是找不变量编码为 key。
//   解法2 (差值编码)：更直接体现数学关系，适合讲思路。
//   解法3 (归一化)：代码更简洁，key 可读性更好（就是个正常字符串）。
//   面试中两种都可以，推荐讲解法2的思路，然后写解法3的代码。
//
// 【易错点】
// 1. 负数取模：
//    ✗ int diff = (s[i] - s[i-1]) % 26;
//      → 'a'-'z' = -25, -25%26 = -25 (C++中)
//    ✓ int diff = ((s[i] - s[i-1]) % 26 + 26) % 26;
//      → (-25+26)%26 = 1 ✓
//
// 2. 差值编码缺少分隔符：
//    ✗ key += to_string(diff);  
//      → [1,12] 和 [11,2] 都编码成 "112"，错误合并！
//    ✓ key += to_string(diff) + ",";
//      → "1,12," 和 "11,2,"，正确区分
//
// 3. 归一化时修改了原始字符串：
//    ✗ string& key = s; ... // 修改了 s 本身，后面 push_back 的不是原串
//    ✓ string key = s;      // 拷贝后再操作
//
// 4. 忘记单字符串的 case：
//    长度为 1 的字符串差值序列为空 → key 都是空串 → 它们确实应该同组
//    （"a" 和 "z" 确实可以互相移位得到）这不是 bug 但容易让人怀疑
//
// 【面试追问】
// Q1: 这道题和 49. Group Anagrams 有什么异同？
//   → 框架完全一样：设计 key + 哈希分组。区别在于 key 的设计：
//     49 的 key 是排序后的字符串（或字符频次），
//     本题的 key 是差值序列或归一化字符串。
//
// Q2: 如果字符串数量很大（百万级），怎么优化？
//   → 先按长度分桶（不同长度的一定不同组），再对每个桶内做哈希分组。
//     这减少了哈希碰撞，也便于并行处理。
//
// Q3: 如果字符集不是小写字母而是任意 Unicode，思路变吗？
//   → 差值编码的思路不变，取模的基数从 26 改成字符集大小。
//     但需要确认字符集是循环的（有回绕规则）。
//     归一化方式需要定义"首字符归一化到什么"。
// ============================================================
