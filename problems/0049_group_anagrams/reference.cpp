/*
 * LeetCode 49: 字母异位词分组 (Group Anagrams)
 *
 * 【题目本质】
 * 为每个字符串找一个"标准化身份标识"，使得所有异位词共享同一个标识，
 * 然后用哈希表按标识分组。
 *
 * 【解法总览】
 * 解法0: 暴力两两比较   — O(n²·k) / O(n·k)  — 最直觉，但超时
 * 解法1: 排序作为 key   — O(n·k·logk) / O(n·k) — 面试首选 ⭐
 * 解法2: 字符计数作为 key — O(n·k) / O(n·k)   — 最优，追问时给出
 *
 * 其中 n = strs.length, k = 字符串最大长度
 */

// ============================================================
// 解法0: 暴力两两比较 — 最直觉的做法
// 时间: O(n²·k)  空间: O(n·k)
//
// 【思路】
// 对每个字符串，遍历已有的所有组，逐个比较是否是异位词。
// 瓶颈："查找属于哪一组"这个操作是 O(n) 的，总共 n 个字符串就是 O(n²)。
// 这就是后续用哈希表优化的动机。
// ============================================================
class Solution0 {
public:
    // 判断两个字符串是否为异位词 — O(k)
    bool isAnagram(const string& a, const string& b) {
        if (a.size() != b.size()) return false;
        int count[26] = {0};
        for (int i = 0; i < (int)a.size(); i++) {
            count[a[i] - 'a']++;
            count[b[i] - 'a']--;
        }
        for (int i = 0; i < 26; i++) {
            if (count[i] != 0) return false;
        }
        return true;
    }

    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> result;
        vector<bool> used(strs.size(), false);

        for (int i = 0; i < (int)strs.size(); i++) {
            if (used[i]) continue;
            vector<string> group = {strs[i]};
            for (int j = i + 1; j < (int)strs.size(); j++) {
                if (!used[j] && isAnagram(strs[i], strs[j])) {
                    group.push_back(strs[j]);
                    used[j] = true;
                }
            }
            result.push_back(group);
        }
        return result;
    }
};

// ============================================================
// 解法1: 排序作为 Key — 面试首选 ⭐
// 时间: O(n·k·logk)  空间: O(n·k)
//
// 【思路】
// 核心观察：异位词排序后的结果完全一样。
//   "eat" → sort → "aet"
//   "tea" → sort → "aet"   ← 同一个 key！
//   "tan" → sort → "ant"
//   "bat" → sort → "abt"
//
// 所以：排序后的字符串 = 异位词的"身份标识"。
// 用 unordered_map<排序后字符串, 原始字符串列表> 自动分组。
//
// 为什么比暴力快？
// 暴力：每个字符串要和所有已有组比较 → O(n) 查找
// 哈希表：每个字符串算 key 后直接 O(1) 查找 → 总体省掉一个 n
// ============================================================
class Solution1 {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> groups;

        for (const string& s : strs) {
            // 对副本排序，不能修改原字符串！
            string key = s;
            sort(key.begin(), key.end());
            // 相同 key 的字符串归为一组
            groups[key].push_back(s);
        }

        // 将哈希表中的所有组收集到结果数组
        vector<vector<string>> result;
        result.reserve(groups.size());
        for (auto& [key, group] : groups) {
            result.push_back(move(group));  // move 避免拷贝
        }
        return result;
    }
};

// ============================================================
// 解法2: 字符计数作为 Key — 最优解
// 时间: O(n·k)  空间: O(n·k)
//
// 【思路】
// 解法1的瓶颈：每个字符串排序需要 O(k·logk)。
// 优化：字母只有26种，统计每个字母出现次数就能唯一标识一个异位词组。
//
// key 编码示例:
//   "eat" → a:1, e:1, t:1 → "1#0#0#0#1#0#...#0#1#0#..."
//   "tea" → a:1, e:1, t:1 → "1#0#0#0#1#0#...#0#1#0#..."  ← 同 key！
//
// 为什么要用 '#' 分隔？
//   没分隔: a出现1次+b出现12次 = "112..."
//           a出现11次+b出现2次 = "112..."  ← 冲突！
//   有分隔: "1#12#..." vs "11#2#..."  ← 不冲突
//
// 生成 key: O(k) 遍历字符串 + O(26) 拼接 = O(k)
// 比排序法的 O(k·logk) 更快，尤其当 k 很大时。
// ============================================================
class Solution2 {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> groups;

        for (const string& s : strs) {
            // O(k): 统计字符频次
            int count[26] = {0};
            for (char c : s) {
                count[c - 'a']++;
            }

            // O(26) = O(1): 将频次编码为字符串 key
            string key;
            for (int i = 0; i < 26; i++) {
                key += to_string(count[i]);
                key += '#';  // 分隔符防歧义
            }

            groups[key].push_back(s);
        }

        vector<vector<string>> result;
        result.reserve(groups.size());
        for (auto& [key, group] : groups) {
            result.push_back(move(group));
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间           | 空间    | key 生成方式 | 代码复杂度 |
// |-----------|---------------|--------|-------------|-----------|
// | 暴力       | O(n²·k)       | O(n·k) | 无 key      | 中等       |
// | 排序法 ⭐  | O(n·k·logk)   | O(n·k) | 排序字符串   | 最简       |
// | 计数法     | O(n·k)         | O(n·k) | 编码计数数组 | 稍多       |
//
// 面试策略:
// - 默认写排序法（代码最短、不易出错）
// - 被问"能否更快？"时给出计数法
// - 暴力法用于分析阶段，展示你理解了问题和瓶颈
//
// 【易错点】
// 1. 排序法中直接对原字符串排序:
//    ✗ sort(s.begin(), s.end()); groups[s].push_back(s);
//      → 原字符串被改了，存入 group 的也是排序后的版本！
//    ✓ string key = s; sort(key.begin(), key.end()); groups[key].push_back(s);
//
// 2. 计数法中漏掉分隔符:
//    ✗ key += to_string(count[i]);
//      → "1"+"12" 和 "11"+"2" 都拼成 "112"，不同组误合并
//    ✓ key += to_string(count[i]) + '#';
//
// 3. 用 map 而非 unordered_map:
//    不会错，但 map 查找 O(logn)，unordered_map 均摊 O(1)。
//    当 n 较大时性能差距明显。
//
// 【面试追问 — 递进链】
//
// Q1: "暴力法能过吗？瓶颈在哪？"
// → O(n²·k)，n=10⁴ 时约 10⁸，会 TLE。
//   瓶颈在于"查找属于哪一组"是 O(n) 的线性扫描。
//   用哈希表把查找降到 O(1) 就能解决。
//
// Q2: "排序法的时间能否进一步优化？"
// → 排序每个字符串 O(k·logk) 是瓶颈。
//   字符集只有26个字母，用计数法可以 O(k) 生成 key。
//   总时间从 O(n·k·logk) 降到 O(n·k)。
//
// Q3: "如果字符集是 Unicode 全集（而非26个字母）呢？"
// → 计数法的固定26位数组不适用了。
//   可改用 map<char,int> 计数后序列化，但 key 长度不固定。
//   排序法对任何字符集都天然适用，此时反而是更好的选择。
//
// Q4: "如果是流式输入（字符串逐个到来），怎么在线分组？"
// → 维护全局 unordered_map<key, group>，每来一个字符串:
//   O(k·logk) 或 O(k) 算 key → O(1) 插入对应组。
//   这就是在线算法，无需缓存所有数据。
//
// 【相关题型】
// - 242. 有效的字母异位词: 本题的子问题（判断两个串是否异位词），
//   复用计数数组比较法。
// - 438. 找到字符串中所有字母异位词: 滑动窗口 + 字符计数，
//   复用计数法思路，但增量更新窗口内的计数而非每次重算。
// - 249. 移位字符串分组: 同样的"设计key + 哈希表分组"框架，
//   区别在于 key 改为相邻字符差值序列。
// ============================================================
