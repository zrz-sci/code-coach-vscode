/*
 * LeetCode 76: Minimum Window Substring (最小覆盖子串)
 *
 * 【题目本质】
 * 在字符串 s 中找最短的连续子串，使其包含 t 的所有字符（含重复）。
 * 经典滑动窗口问题：右扩左缩，维护覆盖状态。
 *
 * 【解法总览】
 * 解法1: 暴力枚举       — O(m²·n) / O(m+n) — 最直觉，理解题意
 * 解法2: 滑动窗口        — O(m+n) / O(1)   — 面试首选 ⭐
 * 解法3: 滑动窗口+过滤   — O(m+n) / O(m)   — 进阶优化（s很长t很短时实际更快）
 */

// ============================================================
// 解法1: 暴力枚举 — 枚举所有子串，逐个检查覆盖
// 时间: O(m²·n)  空间: O(m+n)
//
// 【思路】
// 最直觉：枚举 s 的每个起点 i，向右扩展 j 直到覆盖 t。
// 对于每个 i，只需找到最小的 j 使得 s[i..j] 覆盖 t。
// 
// 瓶颈：每个起点都独立统计字符频率，O(m) 个起点各扫 O(m)。
// 这是滑动窗口的前身——理解它有助于理解为什么窗口能优化。
// ============================================================
class Solution1 {
public:
    string minWindow(string s, string t) {
        int m = s.size(), n = t.size();
        if (m < n) return "";
        
        // 统计 t 中每个字符的需求量
        int need[128] = {};
        int required = 0;
        for (char c : t) {
            if (need[c] == 0) required++;
            need[c]++;
        }
        
        int minLen = INT_MAX, minStart = 0;
        
        for (int i = 0; i < m; i++) {
            int window[128] = {};
            int formed = 0;
            
            for (int j = i; j < m; j++) {
                char c = s[j];
                window[c]++;
                // 某字符恰好达到需求量时 formed 加 1
                if (need[c] > 0 && window[c] == need[c]) {
                    formed++;
                }
                if (formed == required) {
                    int len = j - i + 1;
                    if (len < minLen) {
                        minLen = len;
                        minStart = i;
                    }
                    break; // 当前起点已找到最短覆盖，更长的没意义
                }
            }
        }
        
        return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
    }
};

// ============================================================
// 解法2: 滑动窗口 — 双指针 + 哈希表计数 ⭐ 面试首选
// 时间: O(m+n)  空间: O(1) (128大小的数组是常数)
//
// 【思路】
// 解法1 的瓶颈：每个起点 i 都重新统计，信息被丢弃了。
// 
// 滑动窗口的核心洞察：
// - 当窗口 [left, right] 还没覆盖 t 时，right 右移扩展
// - 当窗口已覆盖 t 时，left 右移收缩（尝试找更短的）
// - left 和 right 各最多走 m 步 → 总共 O(m)
//
// 关键优化：用 formed 计数器 O(1) 判断"是否覆盖"
// - formed 记录"已满足需求量的字符种类数"
// - formed == required 时窗口覆盖 t
//
// 过程可视化 (s = "ADOBECODEBANC", t = "ABC"):
//
//  A D O B E C O D E B A  N  C
//  0 1 2 3 4 5 6 7 8 9 10 11 12
//
//  right=0:  [A]                    window: A=1         formed=1
//  right=3:  [A D O B]             window: A=1,B=1     formed=2
//  right=5:  [A D O B E C]         window: A=1,B=1,C=1 formed=3 ✓
//    → 记录长度6，收缩: left=1 → A移出 → formed=2 ✗
//  right=9:  D[O B E C O D E B]    formed=2，继续扩...
//            得到 B → formed还是2(缺A)
//  right=10: D O B[E C O D E B A]  formed=3 ✓
//    → 收缩...直到不满足
//  right=12: ...最终在 [B A N C] 找到长度4的最短窗口
//
// ============================================================
class Solution2 {
public:
    string minWindow(string s, string t) {
        int m = s.size(), n = t.size();
        if (m < n) return "";
        
        // need[c]: t 中字符 c 的需求量
        // window[c]: 当前窗口中字符 c 的出现次数
        int need[128] = {}, window[128] = {};
        
        int required = 0; // t 中不同字符的种类数（不是 t.size()!）
        for (char c : t) {
            if (need[c] == 0) required++;
            need[c]++;
        }
        
        int formed = 0;   // 当前窗口已满足需求的字符种类数
        int minLen = INT_MAX, minStart = 0;
        
        for (int left = 0, right = 0; right < m; right++) {
            // ---- 扩展：将 s[right] 加入窗口 ----
            char c = s[right];
            window[c]++;
            
            // 为什么用 == 而不是 >=：
            // 只在"恰好达标"那一刻 formed++，避免重复计数
            if (need[c] > 0 && window[c] == need[c]) {
                formed++;
            }
            
            // ---- 收缩：当窗口已覆盖 t 时，左指针右移 ----
            while (formed == required) {
                // 更新答案
                int len = right - left + 1;
                if (len < minLen) {
                    minLen = len;
                    minStart = left;
                }
                
                // 将 s[left] 移出窗口
                char d = s[left];
                window[d]--;
                // 为什么先减后判断 <：
                // 要判断"减完之后是否不满足需求"
                if (need[d] > 0 && window[d] < need[d]) {
                    formed--;
                }
                left++;
            }
        }
        
        return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
    }
};

// ============================================================
// 解法3: 滑动窗口 + 过滤无关字符 — 进阶优化
// 时间: O(m+n)  空间: O(m)
//
// 【思路】
// 解法2 的潜在低效：当 s 非常长但 t 只有几个字符时，
// 右指针逐个字符扫 s，大量字符和 t 完全无关，白白遍历。
//
// 优化：预处理 s，只保留出现在 t 中的字符及其原始索引。
// 例如 s = "XAXBXCX", t = "ABC"
// filtered = [(A,1), (B,3), (C,5)]
// 在这个长度为3的列表上滑动窗口，而不是长度为7的 s 上。
//
// 渐进复杂度不变，但当 s 中大量字符不在 t 中时实际更快。
// ============================================================
class Solution3 {
public:
    string minWindow(string s, string t) {
        int m = s.size(), n = t.size();
        if (m < n) return "";
        
        int need[128] = {};
        int required = 0;
        for (char c : t) {
            if (need[c] == 0) required++;
            need[c]++;
        }
        
        // 过滤：只保留 s 中在 t 里出现的字符
        // pair: (原始索引, 字符)
        vector<pair<int, char>> filtered;
        for (int i = 0; i < m; i++) {
            if (need[(unsigned char)s[i]] > 0) {
                filtered.push_back({i, s[i]});
            }
        }
        
        int window[128] = {};
        int formed = 0;
        int minLen = INT_MAX, minStart = 0;
        
        for (int left = 0, right = 0; right < (int)filtered.size(); right++) {
            char c = filtered[right].second;
            window[c]++;
            if (window[c] == need[c]) formed++;
            
            while (formed == required) {
                // 注意：窗口长度用原始索引算，不是 filtered 下标
                int start = filtered[left].first;
                int end = filtered[right].first;
                int len = end - start + 1;
                
                if (len < minLen) {
                    minLen = len;
                    minStart = start;
                }
                
                char d = filtered[left].second;
                window[d]--;
                if (window[d] < need[d]) formed--;
                left++;
            }
        }
        
        return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 暴力: O(m²·n) — 每个起点独立扫描，大量重复
//   ↓ 优化思路: 左右指针各走一遍，增量更新
// 解法2 滑动窗口: O(m+n) — 面试标准答案
//   ↓ 进一步优化: 跳过与 t 无关的字符
// 解法3 过滤+滑动窗口: O(m+n) — 实际常数更小
//
// 面试中写出解法2即可。如果面试官追问"s很长t很短怎么优化"
// 则说解法3的思路。
//
// 【易错点】
//
// 1. formed 的增减时机:
//    ✗ if (window[c] >= need[c]) formed++;  // 多次重复计数
//    ✓ if (window[c] == need[c]) formed++;  // 只在恰好满足时+1
//
// 2. 收缩时先减后判:
//    ✗ if (window[d] == need[d]) formed--;  // 还没减呢就判断
//       window[d]--;
//    ✓ window[d]--;                          // 先减
//       if (window[d] < need[d]) formed--;   // 再判断是否不足
//
// 3. required 的计算:
//    ✗ int required = t.size();          // t="AAB"时=3，错误
//    ✓ 用 need 数组中非零元素的个数     // t="AAB"时=2 (A和B两种)
//
// 4. 解法3中窗口长度的计算:
//    ✗ int len = right - left + 1;           // 这是 filtered 的下标差
//    ✓ int len = filtered[right].first - filtered[left].first + 1;  // 原始索引
//
// 【面试追问】
//
// Q1: 暴力解复杂度多少？瓶颈在哪？
// → O(m²·n)。每个起点独立统计字符频率。滑动窗口通过增量更新(加/减一个字符O(1))
//   消除重复计算，左右指针各走 m 步，总 O(m)。
//
// Q2: 怎么 O(1) 判断窗口是否覆盖 t？
// → 维护 formed 计数器 = "已满足需求的字符种类数"。
//   formed == required 时覆盖。关键: 只在 window[c]==need[c] 时加 1。
//
// Q3: s 非常长(10^8)、t 很短(3个字符)，怎么进一步优化？
// → 预处理 filtered 列表，只保留 s 中出现在 t 里的字符及其索引。
//   在 filtered 上做滑动窗口，跳过大量无关字符。
//
// Q4: 如果需要返回所有等长最短覆盖子串怎么改？
// → 用 vector<string> 存结果。len < minLen 时清空重装；len == minLen 时追加。
//
// Q5: 本题和 438/567 的关系？
// → 438(找所有字母异位词)/567(判断排列) 是固定窗口大小=t.size()的特例。
//   本题窗口大小不固定，需要动态收缩找最小。
// ============================================================
