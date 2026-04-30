/*
 * LeetCode 451: 根据字符出现频率排序 (Sort Characters By Frequency)
 * 
 * 【题目本质】
 * 统计字符频率，按频率降序拼接字符串。核心是"排序"这一步用什么方式。
 *
 * 【解法总览】
 * 解法1: 哈希表 + 排序     — O(n + k log k) / O(n) — 面试首选，代码最简洁
 * 解法2: 哈希表 + 最大堆   — O(n + k log k) / O(n) — 动态取最大的思路
 * 解法3: 哈希表 + 桶排序   — O(n) / O(n)           — 理论最优，避免比较排序
 *
 * 其中 k 是不同字符种类数，本题 k ≤ 62 (a-z, A-Z, 0-9)
 */

// ============================================================
// 解法1: 哈希表 + 排序 — 面试首选
// 时间: O(n + k log k)  空间: O(n)
//
// 【思路】
// 最直觉的做法：
//   1. 遍历字符串，用哈希表统计每个字符出现次数
//   2. 把 (字符, 频率) 对收集起来，按频率降序排序
//   3. 按排序结果逐个拼接字符串
//
// 为什么高效？排序的对象是"字符种类"（最多62个），不是原字符串。
// 所以排序代价 O(k log k) ≈ O(1)，总体瓶颈在遍历字符串的 O(n)。
//
// 示例: s = "tree"
// Step1: freq = {t:1, r:1, e:2}
// Step2: 排序 → [(e,2), (t,1), (r,1)]
// Step3: 拼接 → "ee" + "t" + "r" = "eetr"
// ============================================================
class Solution1 {
public:
    string frequencySort(string s) {
        // 统计每个字符的频率
        unordered_map<char, int> freq;
        for (char c : s) {
            freq[c]++;
        }
        
        // 收集所有 (字符, 频率) 对，按频率降序排序
        vector<pair<char, int>> chars(freq.begin(), freq.end());
        sort(chars.begin(), chars.end(), [](const pair<char, int>& a, const pair<char, int>& b) {
            return a.second > b.second; // 为什么用 > ？因为要降序（频率高的排前面）
        });
        
        // 按排序结果拼接
        string result;
        for (auto& [ch, cnt] : chars) {
            result += string(cnt, ch); // 一次性追加 cnt 个字符 ch
        }
        return result;
    }
};

// ============================================================
// 解法2: 哈希表 + 最大堆（优先队列）
// 时间: O(n + k log k)  空间: O(n)
//
// 【思路】
// 和解法1的区别：排序这一步换成用最大堆。
// 堆天然支持"每次取最大"，概念上更贴合"按频率从高到低输出"。
//
// 什么时候堆比排序更有优势？
// → 如果只需要前 K 个最高频（如 347 题），堆可以 O(k log k) 提前终止，
//   而排序必须对所有元素排。本题需要全部输出，两者没差别。
//
// 技巧: pair<int, char> 放入 priority_queue，把频率放 first，
//        默认大顶堆会按频率降序排列。
// ============================================================
class Solution2 {
public:
    string frequencySort(string s) {
        // 统计频率
        unordered_map<char, int> freq;
        for (char c : s) {
            freq[c]++;
        }
        
        // 放入最大堆: pair<频率, 字符>，频率放第一个是因为 pair 默认按 first 比较
        priority_queue<pair<int, char>> maxHeap;
        for (auto& [ch, cnt] : freq) {
            maxHeap.push({cnt, ch});
        }
        
        // 逐个弹出堆顶（频率最高的字符），拼接结果
        string result;
        while (!maxHeap.empty()) {
            auto [cnt, ch] = maxHeap.top();
            maxHeap.pop();
            result += string(cnt, ch);
        }
        return result;
    }
};

// ============================================================
// 解法3: 哈希表 + 桶排序 — 理论最优
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 解法1/2 都用了比较排序 O(k log k)。虽然 k 很小，但理论上可以更好。
// 
// 关键观察：频率值的范围是 [1, n]，可以用桶排序！
//   - 创建 n+1 个桶，桶 i 存放所有出现了恰好 i 次的字符
//   - 从桶 n 到桶 1 遍历，遇到非空桶就拼接
//
// 桶排序示意:
//   s = "tree" (n=4)
//   freq = {t:1, r:1, e:2}
//
//   bucket[0]: []        (没有出现0次的字符)
//   bucket[1]: [t, r]    ← 出现1次
//   bucket[2]: [e]       ← 出现2次
//   bucket[3]: []
//   bucket[4]: []
//
//   从后往前遍历: bucket[4]→...→bucket[2]→bucket[1]
//   结果: "" + "ee" + "t" + "r" = "eetr"
//
// 为什么总时间 O(n)？
//   - 统计频率: O(n)
//   - 建桶: O(k)，k 种字符各放一次
//   - 遍历桶 + 拼接: 桶的总大小 n+1，但实际拼接的字符总数恰好是 n
//   - 总计 O(n)
// ============================================================
class Solution3 {
public:
    string frequencySort(string s) {
        int n = s.size();
        
        // Step1: 统计频率
        unordered_map<char, int> freq;
        for (char c : s) {
            freq[c]++;
        }
        
        // Step2: 桶排序 — bucket[i] 存所有出现恰好 i 次的字符
        // 为什么桶大小是 n+1？因为一个字符最多出现 n 次，索引范围 [0, n]
        vector<vector<char>> buckets(n + 1);
        for (auto& [ch, cnt] : freq) {
            buckets[cnt].push_back(ch);
        }
        
        // Step3: 从高频到低频遍历桶，拼接结果
        string result;
        for (int i = n; i >= 1; i--) {       // 频率从 n 到 1
            for (char ch : buckets[i]) {       // 同频率的字符（顺序任意）
                result += string(i, ch);       // 字符 ch 出现了 i 次
            }
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 特性         | 解法1(排序) | 解法2(堆)  | 解法3(桶排序) |
// |-------------|------------|-----------|-------------|
// | 时间         | O(n+klogk) | O(n+klogk)| O(n)        |
// | 空间         | O(n)       | O(n)      | O(n)        |
// | 代码简洁度    | ⭐⭐⭐      | ⭐⭐       | ⭐⭐         |
// | 面试推荐     | ⭐ 首选     | 能说出     | 加分项       |
//
// 由于 k ≤ 62，三种方法实际运行时间几乎相同。
// 面试中写解法1最快最清晰，提到解法3说明你了解桶排序是加分项。
//
// 解法1 vs 解法2:
//   - 排序更简洁直接；堆在"只需前K个"的变体中有优势
// 解法1 vs 解法3:
//   - 桶排序理论更优但代码稍多；本题 k 极小，差异可忽略
//
// ============================================================
// 【易错点】
//
// 1. 排序方向写反:
//    ✗ return a.second < b.second;  → 升序（低频在前）
//    ✓ return a.second > b.second;  → 降序（高频在前）
//
// 2. 桶排序桶大小写成固定值:
//    ✗ vector<vector<char>> buckets(128);
//      → 如果 s = "aaa...a"(500000个a)，频率=500000 > 128，越界！
//    ✓ vector<vector<char>> buckets(n + 1);
//      → 桶大小随字符串长度，频率范围 [1, n] 完全覆盖
//
// 3. 忘记大小写敏感:
//    ✗ 把 freq['A'] 和 freq['a'] 合并统计
//    ✓ 'A' (ASCII 65) 和 'a' (ASCII 97) 是不同字符，直接用 map 不会混淆
//
// 4. 拼接时频率和字符搞反:
//    ✗ result += string(ch, cnt);  → string(char, count) 参数顺序错误
//    ✓ result += string(cnt, ch);  → string(count, char) 是正确的构造函数
//
// ============================================================
// 【面试追问】
//
// Q1: "你说排序是 O(k log k)，k 是什么？能更精确地分析吗？"
//   → k 是不同字符种类数。本题字符集是 a-z, A-Z, 0-9，
//     所以 k ≤ 62。O(62 log 62) 是常数，总体 O(n)。
//
// Q2: "如果字符集扩展到整个 Unicode（几十万种字符），哪种方法最好？"
//   → 桶排序。因为桶大小取决于 n（字符串长度），与字符种类数无关。
//     排序和堆都是 O(k log k)，k 很大时不可忽略。
//
// Q3: "如果频率相同的字符要求按字典序排列？"
//   → 解法1: sort 的 lambda 加条件:
//     if (a.second == b.second) return a.first < b.first;
//     解法3: 桶内字符排序 sort(buckets[i].begin(), buckets[i].end())
//
// Q4: "347 题（前K个高频元素）你会怎么做？和本题的区别？"
//   → 框架完全相同，区别是 347 只取前 K 个。
//     此时堆更有优势：维护大小为 k 的小顶堆，O(n log k)。
//     桶排序也行：从高频桶往低频桶遍历，收够 K 个就停。
// ============================================================
