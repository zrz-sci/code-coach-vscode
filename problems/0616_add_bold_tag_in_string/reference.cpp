/*
 * LeetCode 616: Add Bold Tag in String
 * 
 * 【题目本质】
 * 区间标记 + 区间合并：找出 s 中所有被 words 覆盖的字符位置，
 * 合并重叠/相邻区间，在边界处插入 <b> 和 </b> 标签。
 *
 * 【解法总览】
 * 解法1: 暴力匹配 + 布尔标记  — O(n·m·L) / O(n)     — 面试首选，清晰简洁
 * 解法2: Trie + 布尔标记      — O(n·L+Σ|w|) / O(Σ|w|) — words多时优化
 * 解法3: 区间收集 + 排序合并  — O(n·m·L+k·logk) / O(k) — 通用区间合并思路
 */

// ============================================================
// 解法1: 暴力匹配 + 布尔标记 — 面试首选
// 时间: O(n × m × L)  空间: O(n)
// n=|s|, m=|words|, L=最长word长度
//
// 【思路】
// 把问题拆成两步：
// Step1: 对每个位置 i，检查每个 word 是否从 i 开始匹配。
//        如果匹配了长度为 len 的 word，把 bold[i..i+len-1] 标 true。
//        布尔数组的妙处：重叠/相邻区间自动合并（都是 true）。
// Step2: 线性扫描 bold 数组，在 true 段起点插 <b>，终点后插 </b>。
//
// 示例: s = "aaabbb", words = ["aa", "b"]
//
//   位置:  0  1  2  3  4  5
//   字符:  a  a  a  b  b  b
//
//   "aa" 匹配位置 0: bold[0,1] = T
//   "aa" 匹配位置 1: bold[1,2] = T
//   "b"  匹配位置 3: bold[3] = T
//   "b"  匹配位置 4: bold[4] = T
//   "b"  匹配位置 5: bold[5] = T
//
//   bold:  T  T  T  T  T  T
//          ↑                 ← 起点: 插入 <b>
//                         ↑  ← 终点: 插入 </b>
//   结果: "<b>aaabbb</b>"
// ============================================================
class Solution1 {
public:
    string addBoldTag(string s, vector<string>& words) {
        int n = s.size();
        vector<bool> bold(n, false);
        
        // Step1: 标记每个字符是否需要加粗
        for (int i = 0; i < n; i++) {
            for (const string& word : words) {
                int len = word.size();
                // 先检查长度防止越界，再用 compare 匹配（不创建临时字符串）
                if (i + len <= n && s.compare(i, len, word) == 0) {
                    for (int j = i; j < i + len; j++) {
                        bold[j] = true;
                    }
                }
            }
        }
        
        // Step2: 根据 bold 数组构建带标签的结果
        string result;
        for (int i = 0; i < n; i++) {
            // bold 段起点：当前为 true 且前一个为 false 或不存在
            if (bold[i] && (i == 0 || !bold[i - 1])) {
                result += "<b>";
            }
            result += s[i];
            // bold 段终点：当前为 true 且下一个为 false 或不存在
            if (bold[i] && (i == n - 1 || !bold[i + 1])) {
                result += "</b>";
            }
        }
        
        return result;
    }
};

// ============================================================
// 解法1.5: 暴力匹配 + end 指针优化
// 时间: O(n × m × L)  空间: O(n)
//
// 【思路】
// 解法1中内层标记循环 for(j=i;j<i+len;j++) 看似冗余。
// 观察：如果维护一个 "当前加粗覆盖的最远右端点" end，
// 匹配到 word 时更新 end = max(end, i+len)。
// 然后判断 i < end 即可知道位置 i 是否加粗。
// 这避免了重复标记已标记过的位置。
// ============================================================
class Solution1_5 {
public:
    string addBoldTag(string s, vector<string>& words) {
        int n = s.size();
        vector<bool> bold(n, false);
        
        int end = 0;  // 当前加粗区间的最远右端点（不包含）
        for (int i = 0; i < n; i++) {
            for (const string& word : words) {
                int len = word.size();
                if (i + len <= n && s.compare(i, len, word) == 0) {
                    // 扩展右端点而不是逐个标记
                    end = max(end, i + (int)len);
                }
            }
            // 位置 i 在覆盖范围内
            if (i < end) {
                bold[i] = true;
            }
        }
        
        // 构建结果（同解法1）
        string result;
        for (int i = 0; i < n; i++) {
            if (bold[i] && (i == 0 || !bold[i - 1])) result += "<b>";
            result += s[i];
            if (bold[i] && (i == n - 1 || !bold[i + 1])) result += "</b>";
        }
        return result;
    }
};

// ============================================================
// 解法2: Trie + 布尔标记
// 时间: O(n × L + Σ|word|)  空间: O(Σ|word| + n)
// L = 最长 word 长度
//
// 【思路】
// 解法1的瓶颈：对每个位置 i，遍历所有 words 匹配。
// 如果 words 有公共前缀（如 "abc" 和 "ab"），重复比较了 "a"→"b"。
//
// Trie 把所有 words 的公共前缀合并成一棵树：
//
//   words = ["abc", "ab", "abd"]
//
//        root
//         |
//         a
//         |
//         b  ← isEnd=true（匹配 "ab"）
//        / \
//       c   d
//    (end) (end)
//
// 对位置 i，从 root 开始沿 s[i], s[i+1], ... 往下走：
//   - 走到 isEnd 节点 → 发现一个匹配，标记 [i, j]
//   - 走不下去 → 停止（比遍历所有 words 快）
//
// 优势：和 words 数量 m 无关，只和匹配深度 L 有关。
// ============================================================
class Solution2 {
public:
    struct TrieNode {
        TrieNode* children[128] = {};  // 英文字母 + 数字，用 ASCII 范围
        bool isEnd = false;
    };
    
    string addBoldTag(string s, vector<string>& words) {
        // 构建 Trie
        TrieNode* root = new TrieNode();
        for (const string& word : words) {
            TrieNode* node = root;
            for (char c : word) {
                if (!node->children[c]) {
                    node->children[c] = new TrieNode();
                }
                node = node->children[c];
            }
            node->isEnd = true;
        }
        
        int n = s.size();
        vector<bool> bold(n, false);
        
        // 对每个起始位置 i，沿 Trie 匹配
        int end = 0;  // 复用 end 指针优化
        for (int i = 0; i < n; i++) {
            TrieNode* node = root;
            for (int j = i; j < n; j++) {
                // 当前字符在 Trie 中没有对应的子节点 → 无法继续匹配
                if (!node->children[s[j]]) break;
                node = node->children[s[j]];
                // 走到了某个 word 的结尾 → 找到匹配
                if (node->isEnd) {
                    end = max(end, j + 1);
                }
            }
            // 位置 i 在覆盖范围内
            if (i < end) {
                bold[i] = true;
            }
        }
        
        // 构建结果
        string result;
        for (int i = 0; i < n; i++) {
            if (bold[i] && (i == 0 || !bold[i - 1])) result += "<b>";
            result += s[i];
            if (bold[i] && (i == n - 1 || !bold[i + 1])) result += "</b>";
        }
        return result;
    }
};

// ============================================================
// 解法3: 区间收集 + 排序合并
// 时间: O(n × m × L + k·log k)  空间: O(k)
// k = 匹配产生的区间总数
//
// 【思路】
// 不用布尔数组，换一种经典思路：
// 1. 收集所有匹配产生的区间 [start, end)
// 2. 按 start 排序
// 3. 合并重叠和相邻的区间（经典区间合并，同 LeetCode 56）
// 4. 根据合并后的区间列表，在 s 中插入标签
//
// 区间合并过程示例:
//   s = "aaabbb", words = ["aa", "b"]
//   匹配区间: [0,2), [1,3), [3,4), [4,5), [5,6)
//   排序后:   [0,2), [1,3), [3,4), [4,5), [5,6)
//   合并:
//     [0,2) + [1,3) → [0,3)  (1 <= 2, 重叠)
//     [0,3) + [3,4) → [0,4)  (3 <= 3, 相邻)
//     [0,4) + [4,5) → [0,5)  (4 <= 4, 相邻)
//     [0,5) + [5,6) → [0,6)  (5 <= 5, 相邻)
//   结果: [0,6) → "<b>aaabbb</b>"
// ============================================================
class Solution3 {
public:
    string addBoldTag(string s, vector<string>& words) {
        int n = s.size();
        vector<pair<int, int>> intervals;
        
        // Step1: 收集所有匹配区间 [start, end)
        for (int i = 0; i < n; i++) {
            for (const string& word : words) {
                int len = word.size();
                if (i + len <= n && s.compare(i, len, word) == 0) {
                    intervals.push_back({i, i + len});
                }
            }
        }
        
        if (intervals.empty()) return s;
        
        // Step2: 按起点排序
        sort(intervals.begin(), intervals.end());
        
        // Step3: 合并重叠/相邻区间（注意 <= 处理相邻）
        vector<pair<int, int>> merged;
        merged.push_back(intervals[0]);
        for (int i = 1; i < (int)intervals.size(); i++) {
            auto& last = merged.back();
            // <= 而不是 <：因为左闭右开区间 [a, b) 和 [b, c) 是相邻的，需要合并
            if (intervals[i].first <= last.second) {
                last.second = max(last.second, intervals[i].second);
            } else {
                merged.push_back(intervals[i]);
            }
        }
        
        // Step4: 根据合并后的区间插入标签
        string result;
        int mi = 0;
        for (int i = 0; i < n; i++) {
            // 进入一个合并区间的起点
            if (mi < (int)merged.size() && i == merged[mi].first) {
                result += "<b>";
            }
            result += s[i];
            // 到达一个合并区间的终点（end 是开区间，所以 i+1 == end）
            if (mi < (int)merged.size() && i + 1 == merged[mi].second) {
                result += "</b>";
                mi++;
            }
        }
        
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1（暴力+布尔）vs 解法2（Trie+布尔）:
//   - 核心区别：匹配方式不同。解法1逐word匹配 O(m×L)，解法2用Trie O(L)
//   - 当 words 少（m < 20）时，解法1更简单；words 多且有共享前缀时，解法2更快
//
// 解法1 vs 解法3（区间合并）:
//   - 核心区别：标记方式不同。布尔数组天然合并，区间需要显式排序合并
//   - 布尔数组更简洁；区间合并是更通用的模板，可复用于其他问题
//
// 面试推荐：解法1（清晰简洁，数据范围够用）
//          追问优化时讲解法2（Trie）
//          追问其他思路时讲解法3（区间合并）
//
// ============================================================
// 【易错点】
//
// 1. 匹配越界：
//    ✗ s.compare(i, len, word) 但没检查 i+len <= n
//    ✓ 先判断 i + len <= n 再 compare
//    （s.compare 本身不会崩溃，但 len 超过剩余长度时行为不符合预期）
//
// 2. 标签起点判断写反：
//    ✗ if (bold[i] && bold[i-1])  → 这是段中间
//    ✓ if (bold[i] && (i==0 || !bold[i-1]))  → 前一个不加粗才是起点
//
// 3. 区间合并时 <= vs < 搞混：
//    ✗ intervals[i].first < last.second  → 只处理重叠，漏掉相邻
//    ✓ intervals[i].first <= last.second → 同时处理重叠和相邻
//    因为区间是左闭右开 [start, end)，所以 [0,3) 和 [3,5) 应该合并
//
// 4. words 为空导致区间合并崩溃：
//    ✗ 直接 merged.push_back(intervals[0]) 但 intervals 为空
//    ✓ 先检查 if (intervals.empty()) return s;
//
// ============================================================
// 【面试追问】
//
// Q1: 布尔数组方案的时间复杂度？能否接受？
//     → O(n × m × L)，本题 n,L ≤ 1000, m ≤ 100，约 10^8，
//       但 compare 常数很小，实际通常能过。如果担心超时可用 end 指针优化。
//
// Q2: 如果 words 有上万个且共享大量前缀（如字典），怎么优化？
//     → Trie：将所有 words 构建前缀树，对每个位置 i 沿 Trie 走一遍。
//       复杂度从 O(n×m×L) 降到 O(n×L + Σ|word|)，与 words 数量无关。
//
// Q3: 如果不用布尔数组，还有什么方法处理区间重叠？
//     → 区间合并：收集所有 [start, end) 区间，排序后扫描合并，
//       同 LeetCode 56 合并区间的经典模板。
//
// Q4: 这道题和 758 是什么关系？和 LeetCode 56 呢？
//     → 758 完全相同（换了编号），代码直接复用。
//       56 是本题解法3的子问题（区间排序合并），学会56的模板可在此直接使用。
// ============================================================
