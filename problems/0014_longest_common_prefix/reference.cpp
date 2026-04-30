/*
 * LeetCode 14: 最长公共前缀 (Longest Common Prefix)
 *
 * 【题目本质】
 * 给定 n 个字符串，找到它们从头开始共享的最长公共部分。
 * 本质：逐列扫描，找到第一个"不齐"的位置。
 *
 * 【解法总览】
 * 解法1: 纵向扫描(逐列比较) — O(S) / O(1) — 面试首选 ⭐
 * 解法2: 横向扫描(逐对削短) — O(S) / O(m) — 另一种直觉
 * 解法3: 排序后比较首尾    — O(n·m·logn) / O(1) — 巧妙思路
 * 解法4: 分治法            — O(S) / O(m·logn) — 展示分治思维
 *
 * 其中 S = 所有字符总数, n = 字符串数量, m = 最短字符串长度
 */

// ============================================================
// 解法1: 纵向扫描 — 逐列比较所有字符串的同一位置
// 时间: O(S)  空间: O(1)
//
// 【思路】
// 把所有字符串"竖着对齐"，从第 0 列开始逐列检查。
// 每一列所有字符都相同就继续，否则停止。
//
// 可视化:
//   位置:  0  1  2  3  4  5
//         f  l  o  w  e  r    ← strs[0] = "flower"
//         f  l  o  w          ← strs[1] = "flow"
//         f  l  i  g  h  t   ← strs[2] = "flight"
//         ✓  ✓  ✗             ← col=2: 'o' ≠ 'i', 停止
//   结果: "fl"
//
// 为什么选这种方法？
// - 最符合直觉：公共前缀就是"所有字符串在同一位置字符都相同"
// - 最优时间：一旦发现不匹配立刻返回，不做任何多余工作
// - 最优空间：不需要额外数据结构
// ============================================================
class Solution1 {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return "";
        
        // 外层：遍历列（以 strs[0] 的长度为上界）
        for (int col = 0; col < (int)strs[0].size(); col++) {
            char c = strs[0][col]; // 当前列的"标准字符"
            
            // 内层：检查每个字符串在这一列是否匹配
            for (int row = 1; row < (int)strs.size(); row++) {
                // 为什么先检查长度？避免越界访问
                // 为什么 col >= size 就要停？说明 strs[row] 比 strs[0] 短，已经到头了
                if (col >= (int)strs[row].size() || strs[row][col] != c) {
                    return strs[0].substr(0, col);
                }
            }
        }
        // 走完了 strs[0] 的所有字符还没返回 → strs[0] 本身就是公共前缀
        // （strs[0] 是最短的，或者所有字符串完全相同）
        return strs[0];
    }
};

// ============================================================
// 解法2: 横向扫描 — 拿第一个字符串当候选，逐个削短
// 时间: O(S)  空间: O(m) — substr 会创建新字符串
//
// 【思路】
// 先让 prefix = strs[0]，然后依次和 strs[1], strs[2], ... 比较。
// 如果 prefix 不是当前字符串的前缀，就砍掉最后一个字符，直到匹配。
//
// 可视化:
//   prefix = "flower"
//   vs "flow":    "flower" → "flowe" → "flow"  ← 匹配!
//   prefix = "flow"
//   vs "flight":  "flow" → "flo" → "fl"         ← 匹配!
//   结果: "fl"
// ============================================================
class Solution2 {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return "";
        
        string prefix = strs[0];
        
        for (int i = 1; i < (int)strs.size(); i++) {
            // 不断削短 prefix，直到 strs[i] 以 prefix 开头
            // find() 返回 0 表示 prefix 出现在 strs[i] 的位置 0（即是前缀）
            while (strs[i].find(prefix) != 0) {
                prefix = prefix.substr(0, prefix.size() - 1);
                // 削到空字符串说明没有公共前缀
                if (prefix.empty()) return "";
            }
        }
        return prefix;
    }
};

// ============================================================
// 解法3: 排序后比较首尾 — 利用字典序的性质
// 时间: O(n·m·logn) 排序 + O(m) 比较  空间: O(1) 额外
//
// 【思路】
// 字典序排序后，差异最大的一对一定是第一个和最后一个字符串。
// 它们的公共前缀就是所有字符串的公共前缀。
//
// 为什么正确？
// 假设排序后 first 和 last 的公共前缀长度为 L。
// 对于任意 middle 字符串，first ≤ middle ≤ last（字典序）。
// 如果 middle 的第 k 个字符(k < L) 和 first 不同，
// 那么 middle 不可能在 first 和 last 之间 → 矛盾!
//
// 可视化:
//   排序前: ["flower", "flow", "flight"]
//   排序后: ["flight", "flow", "flower"]
//   只需比较 "flight" 和 "flower":
//     f == f  ✓
//     l == l  ✓
//     i != o  ✗ → 公共前缀 = "fl"
// ============================================================
class Solution3 {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return "";
        
        sort(strs.begin(), strs.end());
        
        const string& first = strs.front();
        const string& last = strs.back();
        
        int i = 0;
        while (i < (int)first.size() && i < (int)last.size() 
               && first[i] == last[i]) {
            i++;
        }
        return first.substr(0, i);
    }
};

// ============================================================
// 解法4: 分治法 — 递归拆分，合并公共前缀
// 时间: O(S)  空间: O(m·logn) — 递归栈 + 中间字符串
//
// 【思路】
// 分治三步:
// 1. 分解: 把字符串数组分成左右两半
// 2. 递归: 分别求左半和右半的公共前缀
// 3. 合并: 左半公共前缀 和 右半公共前缀 再求一次公共前缀
//
// 递归树可视化 (strs = ["flower", "flow", "flight"]):
//
//            LCP("flower","flow","flight")
//                  /                \
//     LCP("flower","flow")    LCP("flight")
//         /          \              |
//   "flower"       "flow"       "flight"
//         \          /              |
//         "flow"                "flight"
//              \                /
//              LCP("flow","flight") = "fl"
// ============================================================
class Solution4 {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return "";
        return divideAndConquer(strs, 0, strs.size() - 1);
    }
    
private:
    string divideAndConquer(vector<string>& strs, int left, int right) {
        // base case: 只有一个字符串，它本身就是"公共前缀"
        if (left == right) return strs[left];
        
        int mid = left + (right - left) / 2;
        string leftLCP = divideAndConquer(strs, left, mid);
        string rightLCP = divideAndConquer(strs, mid + 1, right);
        
        // 合并: 求两个字符串的公共前缀
        return commonPrefix(leftLCP, rightLCP);
    }
    
    string commonPrefix(const string& a, const string& b) {
        int len = min(a.size(), b.size());
        int i = 0;
        while (i < len && a[i] == b[i]) i++;
        return a.substr(0, i);
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间          | 空间        | 特点           |
// |-------------|---------------|-------------|---------------|
// | 纵向扫描     | O(S)          | O(1)        | 最优，面试首选  |
// | 横向扫描     | O(S)          | O(m)        | 直觉好理解      |
// | 排序比首尾   | O(n·m·logn)   | O(1)*       | 巧妙但多了排序  |
// | 分治         | O(S)          | O(m·logn)   | 展示分治思维    |
//
// 面试中优先写解法1（纵向扫描），然后口述解法3（排序）展示思路广度。
//
// 【易错点】
// 1. 纵向扫描中忘记检查 col >= strs[row].size():
//    ✗ if (strs[row][col] != c) → 当 strs[row] 比 strs[0] 短时越界
//    ✓ if (col >= (int)strs[row].size() || strs[row][col] != c)
//
// 2. 横向扫描中 find 的返回值判断:
//    ✗ while (strs[i].find(prefix)) → find 返回 npos (非零) 也进循环
//       但如果 prefix 在中间位置匹配也返回非零，逻辑混乱
//    ✓ while (strs[i].find(prefix) != 0) → 明确要求在位置0匹配
//
// 3. 忘记处理空字符串:
//    strs = ["", "abc", "ab"] → 应返回 ""
//    纵向扫描第一列就会触发 col >= strs[0].size() 即 0 >= 0，
//    但这里 strs[0] 是空串所以外层循环不进入，直接返回 strs[0] = "" ✓
//    如果空串在 strs[1] 而不是 strs[0]，内层 col >= strs[1].size() 触发 ✓
//
// 4. 类型转换问题:
//    ✗ col < strs[0].size() → size() 返回 size_t (无符号)，col 是 int
//       当 strs[0] 为空时 size()=0，int 和 size_t 比较可能有意外行为
//    ✓ col < (int)strs[0].size() → 统一为有符号比较
//
// 【面试追问】
// Q1(基础): 时间复杂度能否比 O(S) 更优？
//    → 不能。最坏情况所有字符串完全相同，必须检查每个字符。O(S)是下界。
//
// Q2(优化): 如果要频繁对同一组字符串的不同子集查询公共前缀，怎么优化？
//    → 预处理阶段把所有字符串插入 Trie。查询时从根向下走，
//      沿着只有单一孩子且非终止的路径走到底就是公共前缀。
//      预处理 O(S)，每次查询 O(m)。
//
// Q3(变体): 如果字符串极长(百万级)但数量极少(10个)，有什么特别的优化？
//    → 在 [0, minLen] 上二分前缀长度 mid。
//      对于给定 mid，检查所有字符串前 mid 个字符是否相同 O(n·mid)。
//      虽然渐近不一定更优，但如果公共前缀很短，二分能更快收敛。
// ============================================================
