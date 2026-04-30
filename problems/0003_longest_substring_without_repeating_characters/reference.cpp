/*
 * LeetCode 3: Longest Substring Without Repeating Characters
 * 
 * 【题目本质】
 * 在字符串中找最长的连续窗口（子串），使窗口内无重复字符。
 * "连续" + "满足某条件的最长" → 经典滑动窗口问题。
 *
 * 【解法总览】
 * 解法1: 暴力枚举          — O(n³) / O(n)        — 理解题意
 * 解法2: 滑动窗口 + HashSet — O(n)  / O(min(n,m)) — 面试首选 ⭐
 * 解法3: 滑动窗口 + HashMap — O(n)  / O(min(n,m)) — 跳转优化
 * 解法4: 滑动窗口 + 数组    — O(n)  / O(1)        — 极致优化
 */

// ============================================================
// 解法1: 暴力枚举 — 枚举所有子串，检查每个子串是否无重复
// 时间: O(n³)  空间: O(n)
// 
// 【思路】
// 最直觉的做法：三层循环。
//   外两层枚举子串的起点 i 和终点 j，
//   内层用 set 检查 s[i..j] 是否有重复字符。
// 
// 瓶颈：n = 50000 时约 1.25×10¹⁴ 次操作，完全不可行。
// 但这个暴力帮我们看清优化方向：当 j 右移一步时，
// 我们其实只新增了一个字符 s[j]，不需要重新检查整个子串。
// 这就是滑动窗口的出发点。
// ============================================================
class Solution1 {
public:
    int lengthOfLongestSubstring(string s) {
        int n = s.size(), ans = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {
                // 检查 s[i..j] 是否有重复字符
                unordered_set<char> seen;
                bool valid = true;
                for (int k = i; k <= j; k++) {
                    if (seen.count(s[k])) {
                        valid = false;
                        break;
                    }
                    seen.insert(s[k]);
                }
                if (valid) ans = max(ans, j - i + 1);
            }
        }
        return ans;
    }
};

// ============================================================
// 解法2: 滑动窗口 + HashSet — 右扩左缩，set 维护窗口内字符
// 时间: O(n)  空间: O(min(n, m))  (m = 字符集大小)
// 
// 【思路】
// 暴力解的瓶颈：每次右端点移动都重新检查整个子串。
// 优化：维护一个窗口 [left, right]，用 set 记录窗口内字符。
// - right 每次右移一步（扩张）
// - 如果 s[right] 已在 set 中（重复了），就从左边逐个移除，直到无重复
// - 两个指针都只往右走不回头，总移动次数 ≤ 2n，所以 O(n)
//
// 滑动窗口过程 (s = "abcabcbb"):
//
//   a b c a b c b b
//   L R               window={a,b}, len=2
//   L   R             window={a,b,c}, len=3 ← max
//   L     R           'a' 在 set 中! 移除 s[L]='a', L++
//     L   R           window={b,c,a}, len=3
//     L     R         'b' 在 set 中! 移除 s[L]='b', L++
//       L   R         window={c,a,b}, len=3
//       ...           持续滑动，最终答案 = 3
// ============================================================
class Solution2 {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_set<char> window; // 维护当前窗口内的字符集合
        int left = 0, ans = 0;
        
        for (int right = 0; right < (int)s.size(); right++) {
            // 只要 s[right] 在窗口中，就从左边收缩
            // 为什么用 while 不用 if：可能左边有多个字符要移除
            // 才能消除和 s[right] 的重复（比如 left 指向的不是重复的那个字符）
            while (window.count(s[right])) {
                window.erase(s[left]);
                left++;
            }
            // 窗口内无重复了，放入新字符
            window.insert(s[right]);
            // 更新最长长度（right - left + 1 就是当前窗口大小）
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};

// ============================================================
// 解法3: 滑动窗口 + HashMap — 记录字符最后位置，左指针直接跳转
// 时间: O(n)  空间: O(min(n, m))
// 
// 【思路】
// 解法2 中左指针可能需要一步步移动多次才能消除重复。
// 优化：用 map 记录每个字符最后出现的索引。
// 当 s[right] 已出现过，且上次位置在窗口内时，
// 直接把 left 跳到 "上次位置 + 1"。
//
// 关键：left 只能往右跳，不能往左退！
// 所以必须取 max(left, lastPos[s[right]] + 1)
//
// 跳转示意 (s = "abba"):
//   索引:    0  1  2  3
//   字符:    a  b  b  a
//
//   right=0: 'a'首次, map={a:0}, left=0, len=1
//   right=1: 'b'首次, map={a:0,b:1}, left=0, len=2
//   right=2: 'b'上次在1, left=max(0, 1+1)=2, map={a:0,b:2}, len=1
//   right=3: 'a'上次在0, 但0 < left(=2), 不跳!
//            left=max(2, 0+1)=2, map={a:3,b:2}, len=2
//                                 ↑ 如果不取 max，left 会倒退到 1，错误！
// ============================================================
class Solution3 {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_map<char, int> lastPos; // char → 最后出现的索引
        int left = 0, ans = 0;
        
        for (int right = 0; right < (int)s.size(); right++) {
            // 如果 s[right] 出现过，且在当前窗口 [left, right) 内
            if (lastPos.count(s[right]) && lastPos[s[right]] >= left) {
                // 直接跳到重复字符的下一个位置
                // 等价于 left = max(left, lastPos[s[right]] + 1)
                left = lastPos[s[right]] + 1;
            }
            // 无论如何更新该字符的最后出现位置
            lastPos[s[right]] = right;
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};

// ============================================================
// 解法4: 滑动窗口 + 数组 — 用 int[128] 替代 HashMap，O(1) 空间
// 时间: O(n)  空间: O(1)（128 个 int 是常数空间）
// 
// 【思路】
// 题目说字符由英文字母、数字、符号和空格组成，即 ASCII 128 种字符。
// 用固定大小的数组替代 HashMap：
// - 数组下标 = 字符的 ASCII 值
// - 数组值 = 该字符最后出现的索引
// - 初始化为 -1 表示"从未出现"
//
// 逻辑和解法3 完全一致，只是数据结构从 hash map 换成数组。
// 优点：数组访问比哈希表快（无哈希计算、无冲突处理）。
// ============================================================
class Solution4 {
public:
    int lengthOfLongestSubstring(string s) {
        // 初始化为 -1：表示所有字符都未出现过
        // 不能初始化为 0，因为索引 0 是合法位置，会和"未出现"混淆
        int lastPos[128];
        memset(lastPos, -1, sizeof(lastPos));
        
        int left = 0, ans = 0;
        for (int right = 0; right < (int)s.size(); right++) {
            // lastPos[c] >= left 说明该字符在当前窗口内出现过
            if (lastPos[(int)s[right]] >= left) {
                left = lastPos[(int)s[right]] + 1;
            }
            lastPos[(int)s[right]] = right;
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};

// ============================================================
// 【解法对比】
// 
// | 解法 | 数据结构 | 左指针移动方式 | 时间 | 空间    |
// |------|----------|----------------|------|---------|
// | 暴力 | set      | —              | O(n³)| O(n)    |
// | Set  | HashSet  | 逐步收缩       | O(n) | O(min n,m)|
// | Map  | HashMap  | 直接跳转       | O(n) | O(min n,m)|
// | 数组 | int[128] | 直接跳转       | O(n) | O(1)    |
//
// Set 版和 Map 版的总时间复杂度相同（都是 O(n)），但 Map 版在
// 实际运行中更快，因为左指针移动次数更少。
// 
// 面试推荐：先写 Set 版（不易出错），再口述 Map 版优化。
//
// ============================================================
// 【易错点】
//
// 1. 左指针倒退 bug（解法3/4 最常见错误）：
//    ✗ left = lastPos[s[right]] + 1;  // 不判断是否在窗口内
//    ✓ if (lastPos[s[right]] >= left) left = lastPos[s[right]] + 1;
//    反例: "abba" → 处理最后的'a'时 map['a']=0, left=2
//          不加判断会让 left 从 2 退回到 1
//
// 2. 数组初始化为 0 导致索引 0 的字符被误判：
//    ✗ int lastPos[128] = {0}; // 0 是合法索引！
//    ✓ memset(lastPos, -1, sizeof(lastPos)); // -1 表示未出现
//
// 3. 窗口长度少 +1：
//    ✗ ans = max(ans, right - left);
//    ✓ ans = max(ans, right - left + 1);
//    [left, right] 闭区间包含 right - left + 1 个字符
//
// 4. Set 版中用 if 代替 while：
//    ✗ if (window.count(s[right])) { window.erase(s[left]); left++; }
//    ✓ while (window.count(s[right])) { window.erase(s[left]); left++; }
//    反例: "dvdf" → right=3('f'不重复,没问题), 但考虑 "abcdc"
//          right=4('c'), 移除 left='a' 后 'c' 仍在 set 中，需要继续移除
//
// ============================================================
// 【面试追问】(递进链)
//
// Q1: "暴力 O(n³) 能不能优化到 O(n²)？"
//     可以。固定左端点 i，右端点 j 向右移动时增量式检查：
//     如果 s[j] 在 set 中就 break，否则 set.insert(s[j])。
//     每个起点 i 重置 set，总时间 O(n²)。
//     但这还不够好 → 引出滑动窗口 O(n)。
//
// Q2: "Set 版本左指针是逐步移动的，能不能更快？"
//     用 HashMap 记录字符最后出现位置，直接跳转。
//     但要注意 left 不能倒退（取 max）。
//
// Q3: "如果要返回最长子串本身而不是长度呢？"
//     在更新 ans 时同时记录 bestLeft = left;
//     最后返回 s.substr(bestLeft, ans);
//
// Q4: "如果字符集是 Unicode（百万级），空间怎么办？"
//     只能用 HashMap（不能开百万大小的数组）。
//     空间 O(min(n, m))，m 是实际出现的字符种类数。
//
// ============================================================
