/*
 * LeetCode 424: 替换后的最长重复字符
 * (Longest Repeating Character Replacement)
 *
 * 【题目本质】
 * 找最长子串，使得"子串长度 - 子串内最大字符频次 ≤ k"。
 * 即：窗口内最多 k 个字符不是出现次数最多的那个字符。
 *
 * 【解法总览】
 * 解法1: 暴力枚举         — O(n²) / O(26)  — 最直觉，用于理解问题
 * 解法2: 滑动窗口(标准)   — O(n)  / O(26)  — 面试首选 ⭐
 * 解法3: 滑动窗口(不回缩) — O(n)  / O(26)  — 最优/加分项
 */

// ============================================================
// 解法1: 暴力枚举 — O(n²) / O(26)
//
// 【思路】
// 枚举每个左端点 i，右端点 j 从 i 向右扩展。
// 增量维护频次数组和 maxFreq，避免每个子串重新统计。
// 合法条件：windowLen - maxFreq <= k
//
// 瓶颈：两重循环 O(n²)，n=10^5 时会超时。
// 但它帮我们看清核心判断条件，是滑动窗口优化的基础。
// ============================================================
class Solution1 {
public:
    int characterReplacement(string s, int k) {
        int n = s.size(), ans = 0;
        for (int i = 0; i < n; i++) {
            int freq[26] = {};
            int maxFreq = 0;
            for (int j = i; j < n; j++) {
                freq[s[j] - 'A']++;
                // 只有新加入的字符可能成为新的最大频次
                maxFreq = max(maxFreq, freq[s[j] - 'A']);
                int windowLen = j - i + 1;
                // 核心判断：需要替换的字符数 = windowLen - maxFreq
                if (windowLen - maxFreq <= k) {
                    ans = max(ans, windowLen);
                }
            }
        }
        return ans;
    }
};

// ============================================================
// 解法2: 滑动窗口（标准写法） — O(n) / O(26)  ⭐ 面试首选
//
// 【思路】
// 解法1的瓶颈：对每个左端点重新扫描。
// 观察：右端点扩展使"需要替换数"增加，左端点收缩使其减少。
// → 满足滑动窗口的单调性条件。
//
// 窗口合法条件：windowLen - maxFreq <= k
// 不合法时，左指针右移直到合法。
//
// maxFreq 在左指针右移时可能减小，但不精确维护也不影响答案：
// - 答案只在 maxFreq 增大时可能更新
// - maxFreq 偏大 → 窗口看起来更合法 → 但窗口不会比真实最优更大
//
// 如果面试中不确定，可以精确维护（遍历 freq 找 max，O(26)=O(1)）
//
// 滑动窗口过程示意（s = "AABABBA", k = 1）:
//
// right=0: [A]ABABBA         maxFreq=1  len=1  replace=0 ≤ 1 ✓
// right=1: [AA]BABBA         maxFreq=2  len=2  replace=0 ≤ 1 ✓
// right=2: [AAB]ABBA         maxFreq=2  len=3  replace=1 ≤ 1 ✓
// right=3: [AABA]BBA         maxFreq=3  len=4  replace=1 ≤ 1 ✓  ans=4
// right=4: [AABAB]BA         maxFreq=3  len=5  replace=2 > 1  ✗
//           → 收缩: A[ABAB]BA  maxFreq=3  len=4  replace=1 ≤ 1 ✓
// right=5: A[ABABB]A         maxFreq=3  len=5  replace=2 > 1  ✗
//           → 收缩: AA[BABB]A  maxFreq=3  len=4  replace=1 ≤ 1 ✓
// right=6: AA[BABBA]         maxFreq=3  len=5  replace=2 > 1  ✗
//           → 收缩: AAB[ABBA]  len=4  ✓
// 最终 ans = 4
// ============================================================
class Solution2 {
public:
    int characterReplacement(string s, int k) {
        int n = s.size(), ans = 0;
        int freq[26] = {};
        int maxFreq = 0;
        int left = 0;

        for (int right = 0; right < n; right++) {
            // 1. 扩展窗口：加入 s[right]
            freq[s[right] - 'A']++;
            // 新字符可能刷新 maxFreq
            maxFreq = max(maxFreq, freq[s[right] - 'A']);

            // 2. 收缩窗口：当需要替换的字符数 > k 时不合法
            while (right - left + 1 - maxFreq > k) {
                freq[s[left] - 'A']--;
                left++;
                // maxFreq 可能已经不精确了，但不影响最终答案
                // 如果要精确维护，取消下面这行的注释:
                // maxFreq = *max_element(freq, freq + 26);
            }

            // 3. 更新答案
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};

// ============================================================
// 解法2b: 滑动窗口（精确维护 maxFreq 版本） — O(n) / O(26)
//
// 【思路】
// 和解法2 完全一样，唯一区别是在收缩时精确计算 maxFreq。
// 遍历 26 个字母的频次数组找最大值，O(26) = O(1)。
// 这个版本更容易理解正确性，适合面试中不确定时使用。
// ============================================================
class Solution2b {
public:
    int characterReplacement(string s, int k) {
        int n = s.size(), ans = 0;
        int freq[26] = {};
        int left = 0;

        for (int right = 0; right < n; right++) {
            freq[s[right] - 'A']++;

            // 精确计算窗口内最大频次
            int maxFreq = *max_element(freq, freq + 26);

            // 收缩直到合法
            while (right - left + 1 - maxFreq > k) {
                freq[s[left] - 'A']--;
                left++;
                maxFreq = *max_element(freq, freq + 26);
            }

            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};

// ============================================================
// 解法3: 滑动窗口（maxFreq 不回退，窗口只增不缩） — O(n) / O(26)
//
// 【思路】
// 核心洞察：我们只关心"能否找到更长的窗口"。
//
// 假设当前最优窗口长度为 L，则 maxFreq >= L - k。
// 要找到长度 L+1 的窗口，需要 maxFreq >= L+1-k，
// 即 maxFreq 必须增大。如果 maxFreq 没增大，
// 不可能找到更长的合法窗口。
//
// 所以：
// - maxFreq 只在变大时更新（只增不减）
// - 窗口不合法时，左右指针同时右移一步（窗口平移，大小不变）
// - 窗口合法时，只右指针右移（窗口增大）
// - 最终窗口大小 = 历史最大合法窗口大小 = 答案
//
// 关键：用 if 而不是 while！每次最多收缩一步。
//
// 窗口行为示意:
// 合法 → 右指针右移, 窗口增大:   |-------|   →  |--------|
// 不合法 → 左右同时右移, 窗口平移:  |-------|  →   |-------|
// 窗口大小 = max(历史所有合法窗口大小)
// ============================================================
class Solution3 {
public:
    int characterReplacement(string s, int k) {
        int n = s.size();
        int freq[26] = {};
        int maxFreq = 0;
        int left = 0;

        for (int right = 0; right < n; right++) {
            freq[s[right] - 'A']++;
            // maxFreq 只增不减：只在新字符频次超过历史最大时更新
            maxFreq = max(maxFreq, freq[s[right] - 'A']);

            // 用 if 而不是 while：最多收缩一步，窗口大小不减
            if (right - left + 1 - maxFreq > k) {
                freq[s[left] - 'A']--;
                left++;
                // 不更新 maxFreq 的减小 — 这是核心
            }
        }

        // 最终窗口 [left, n-1] 的大小就是答案
        // 因为窗口只增不减，最终窗口大小 = 历史最大合法窗口大小
        return n - left;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 vs 解法2:
//   解法1 每个左端点重新扫描，O(n²)
//   解法2 用滑动窗口，左右指针各走一遍，O(n)
//   关键优化：左指针不回退（窗口收缩而非重新开始）
//
// 解法2 vs 解法3:
//   解法2 窗口可增可缩（while 循环），maxFreq 可选精确维护
//   解法3 窗口只增或平移（if 条件），maxFreq 只增不减
//   解法3 利用了"答案只在 maxFreq 增大时可能更优"的性质
//   解法3 代码更简洁，但正确性的理解更难
//
// 面试建议：
//   - 先说解法1 展示理解
//   - 写解法2（标准滑动窗口）
//   - 被追问"maxFreq 为什么不用回退"时，讲解法3
//
// ============================================================
// 【易错点】
//
// 1. maxFreq 更新时机：
//    ✗ 在 while 收缩之后才更新 maxFreq
//    ✓ 在右指针扩展后立即更新，因为新字符可能创造新的最大频次
//
// 2. 窗口长度公式：
//    ✗ right - left    （少了1，左闭右闭区间长度应该 +1）
//    ✓ right - left + 1
//
// 3. 解法3 用 while 代替 if：
//    ✗ while (right - left + 1 - maxFreq > k) { left++; }
//       → 窗口可能缩小，破坏了"窗口只增不减"的不变量
//    ✓ if (right - left + 1 - maxFreq > k) { left++; }
//       → 每轮最多收缩一步，窗口大小 ≥ 上一轮
//
// 4. freq 数组未初始化：
//    ✗ int freq[26];     // C++ 局部数组内容未定义
//    ✓ int freq[26] = {}; // 全部初始化为 0
//
// 5. 解法3 的返回值：
//    ✗ return ans;  // 解法3没有维护 ans 变量
//    ✓ return n - left;  // 最终窗口 [left, n-1] 的大小
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: "暴力法怎么做？瓶颈在哪？"
//     → 枚举所有子串 O(n²)，增量维护频次可以避免 O(n³)。
//       瓶颈在于每个左端点都重新扫描右端点。
//
// Q2: "滑动窗口中 maxFreq 不精确维护真的对吗？证明一下。"
//     → 答案 = max(合法窗口长度) = max(maxFreq + k)（受窗口长度限制）。
//       maxFreq 单调不减时，窗口大小单调不减。
//       只有 maxFreq 增大时窗口才可能增大。
//       maxFreq 偏大不会导致窗口比真实最优更大，
//       因为 if 条件仍会阻止不合法的窗口增长。
//
// Q3: "如果字符集是任意 Unicode 字符（如中文），怎么改？"
//     → freq 数组改为 unordered_map<char, int>。
//       精确维护 maxFreq 需要遍历 map，代价 O(字符集大小)。
//       解法3 的优势更大：maxFreq 不回退，避免了遍历 map 的开销。
//
// Q4: "这题和 LeetCode 1004（最大连续1的个数III）有什么关系？"
//     → 1004 是本题的特例：字符集 = {0,1}，目标字符 = 1。
//       条件简化为：窗口内 0 的个数 ≤ k，等价于 windowLen - count1 ≤ k。
//       完全复用本题的滑动窗口框架。
// ============================================================
