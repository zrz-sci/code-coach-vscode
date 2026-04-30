/*
 * LeetCode 833: Find And Replace in String
 *
 * 【题目本质】
 * 在原字符串的若干指定位置同时做"匹配-替换"操作。
 * 核心难点：所有替换基于原始索引，替换改变字符串长度后索引会偏移。
 *
 * 【解法总览】
 * 解法1: 从右到左排序替换    — O(n + k·m) / O(k)   — 最直觉
 * 解法2: 哈希表+逐字符构建   — O(n + k·m) / O(n+k) — 面试首选，一遍扫描
 */

// ============================================================
// 解法1: 从右到左排序替换
// 时间: O(n + k·m + k·log(k))  空间: O(k)
//
// 【思路】
// 直接在字符串上替换会导致索引偏移。但如果从右到左处理，
// 右边的替换不会改变左边字符的位置，所以按 indices 降序排序
// 再逐个替换即可。
//
// 示例: s = "abcd", indices=[0,2], sources=["a","cd"], targets=["eee","ffff"]
//
// 排序后顺序: 先处理 idx=2, 再处理 idx=0
//
// Step1: idx=2, s[2..3]="cd"=="cd" ✓ → s = "ab" + "ffff" = "abffff"
//        注意此时 idx=0 处的 "a" 位置没变！
// Step2: idx=0, s[0..0]="a"=="a" ✓ → s = "" + "eee" + "bffff" = "eeebffff"
//
// 结果: "eeebffff"
// ============================================================
class Solution1 {
public:
    string findReplaceString(string s, vector<int>& indices, vector<string>& sources, vector<string>& targets) {
        int k = indices.size();

        // 创建排序索引，按 indices 降序
        // 为什么降序？从右到左替换，右边替换不影响左边索引
        vector<int> order(k);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) {
            return indices[a] > indices[b];
        });

        for (int i : order) {
            int idx = indices[i];
            int len = sources[i].size();
            // 在原始（或仅被右边替换过的）字符串上检查匹配
            if (s.substr(idx, len) == sources[i]) {
                // 拼接：idx 之前 + target + idx+len 之后
                s = s.substr(0, idx) + targets[i] + s.substr(idx + len);
            }
        }
        return s;
    }
};


// ============================================================
// 解法2: 哈希表 + 逐字符构建（面试首选）
// 时间: O(n + k·m)  空间: O(n + k)
//
// 【思路】
// 解法1每次替换都拼接字符串，有额外拷贝开销。
// 换个思路：不修改原字符串，从左到右扫描，逐字符决定
// "拷贝原字符" 还是 "替换成 target"。
//
// 用哈希表 lookup[idx] = 操作编号，遍历时 O(1) 查询。
//
// 示例: s = "abcd", indices=[0,2], sources=["a","cd"], targets=["eee","ffff"]
//
// 哈希表: {0 → 0, 2 → 1}
//
// i=0: lookup 找到操作0, s[0..0]="a"=="a" ✓ → result+="eee", i跳到1
// i=1: lookup 没找到 → result+="b", i=2
// i=2: lookup 找到操作1, s[2..3]="cd"=="cd" ✓ → result+="ffff", i跳到4
// i=4: 超出，结束
// result = "eeebffff"
// ============================================================
class Solution2 {
public:
    string findReplaceString(string s, vector<int>& indices, vector<string>& sources, vector<string>& targets) {
        int n = s.size(), k = indices.size();

        // 建立 原始索引 → 操作编号 的映射
        // 为什么用哈希表？遍历 s 时需要快速判断当前位置是否有替换操作
        unordered_map<int, int> lookup;
        for (int i = 0; i < k; i++) {
            lookup[indices[i]] = i;
        }

        string result;
        int i = 0;
        while (i < n) {
            auto it = lookup.find(i);
            if (it != lookup.end()) {
                int opIdx = it->second;
                int len = sources[opIdx].size();
                // 在原字符串上检查匹配
                if (s.substr(i, len) == sources[opIdx]) {
                    result += targets[opIdx]; // 匹配成功，追加 target
                    i += len;                 // 跳过 source 的长度（不是 target！）
                    continue;
                }
            }
            // 无替换操作 或 匹配失败 → 保留原字符
            result += s[i];
            i++;
        }
        return result;
    }
};


// ============================================================
// 【解法对比】
//
// 解法1 (从右到左替换):
//   - 优点: 思路直觉，容易想到
//   - 缺点: 每次 substr 拼接有额外拷贝，需要排序
//   - 适用: 快速说出思路，展示理解
//
// 解法2 (哈希表+逐字符构建):
//   - 优点: 不需要排序，一遍扫描，无中间字符串拷贝
//   - 缺点: 需要额外哈希表空间
//   - 适用: 面试写代码首选
//
// 两种解法时间复杂度相同(排序的 O(k·log(k)) 在实际中可忽略)，
// 但解法2更优雅且更容易写对。
//
// ============================================================
// 【易错点】
//
// 1. 跳过长度用错:
//    ✗ i += targets[opIdx].size()  — 用了 target 长度
//    ✓ i += sources[opIdx].size()  — 指针在原字符串上，应跳过 source 长度
//
// 2. 匹配失败时漏掉原字符:
//    ✗ 在 lookup 找到但不匹配后直接 continue，i 不前进，死循环
//    ✓ 不匹配时走 else 分支: result += s[i]; i++;
//
// 3. 哈希表键值建反:
//    ✗ lookup[i] = indices[i]
//    ✓ lookup[indices[i]] = i  — 键是位置，值是操作编号
//
// 4. 解法1不排序就替换:
//    ✗ 按给定顺序从左到右替换 → 前面替换改变了字符串长度，后面索引对不上
//    ✓ 必须按 indices 降序排序
//
// ============================================================
// 【面试追问】
//
// Q1(基础理解): 为什么不能从左到右依次替换？
// A1: 替换可能改变字符串长度(source 和 target 长度不同)，导致后续操作
//     的 indices 在修改后的字符串上不再准确。题目要求所有操作基于原始索引。
//
// Q2(优化): 解法1和解法2的核心区别是什么？什么时候解法2更好？
// A2: 解法1需要排序且每次替换都重建字符串(O(n)拷贝)，最坏 O(k·n)。
//     解法2只遍历原串一次，用哈希表 O(1) 查询，总体更高效。
//     当 k 较大且 s 较长时，解法2优势明显。
//
// Q3(变体): 如果替换操作可能重叠（如 indices=[0,1], sources=["ab","bc"]），
//     如何处理？
// A3: 需要定义优先级规则。一种策略：按索引排序，贪心匹配——匹配成功后
//     标记覆盖区间 [idx, idx+len)，后续操作如果起点落在已覆盖区间内则跳过。
//     如果优先级是"长匹配优先"，则按 source 长度降序排序后贪心。
// ============================================================
