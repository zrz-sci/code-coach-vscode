/*
 * LeetCode 692: 前K个高频单词
 * 难度: Medium | 标签: Hash Table, String, Sorting, Heap (Priority Queue), Trie, Bucket Sort
 *
 * 题目: 给定单词列表 words 和整数 k，返回前 k 个出现次数最多的单词。
 *       按频率降序排列；频率相同则按字典序升序排列。
 *
 * Follow-up: 能否用 O(n log k) 时间和 O(n) 空间解决？
 *
 * 核心模式: TopK + 自定义排序规则
 */

// ============================================================
// 解法1: 哈希表 + 自定义排序 — O(n log n) / O(n) ⭐ 面试首选
// ============================================================
// 思路: 统计频率 → 全排序 → 取前 k 个
// 排序规则: 频率不同 → 频率高排前面
//           频率相同 → 字典序小排前面
//
// 示例: words=["i","love","leetcode","i","love","coding"], k=2
//   freq: {"i":2, "love":2, "leetcode":1, "coding":1}
//   排序后: ["i"(2), "love"(2), "coding"(1), "leetcode"(1)]
//   取前2: ["i", "love"]

class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
        // Step 1: 统计频率
        unordered_map<string, int> freq;
        for (const string& w : words) {
            freq[w]++;
        }

        // Step 2: 提取不同单词
        vector<string> candidates;
        for (auto& [word, _] : freq) {
            candidates.push_back(word);
        }

        // Step 3: 自定义排序
        // 频率降序; 同频字典序升序
        sort(candidates.begin(), candidates.end(),
            [&](const string& a, const string& b) {
                if (freq[a] != freq[b]) return freq[a] > freq[b];
                return a < b;
            });

        // Step 4: 取前 k 个
        return vector<string>(candidates.begin(), candidates.begin() + k);
    }
};


// ============================================================
// 解法2: 最小堆 (大小 k) — O(n log k) / O(n) ⭐ 满足 Follow-up
// ============================================================
// 思路: 维护大小为 k 的最小堆，堆顶是"最不重要的"（最先被淘汰的）。
//       遍历所有单词，堆满后如果新元素比堆顶更重要就替换。
//
// 关键: 堆的比较器方向和最终排序方向 **相反**!
//
// 最终排序要求:
//   - 频率高 → 更重要 → 排前面
//   - 同频字典序小 → 更重要 → 排前面
//
// 堆比较器 (堆顶 = 最不重要的):
//   - 频率高 → 更重要 → 沉底 (cmp 返回 true)
//   - 同频字典序小 → 更重要 → 沉底 (cmp 返回 true)
//
// priority_queue 比较器含义:
//   cmp(a, b) = true → a 优先级低于 b → a 排在 b 后面 → b 更靠近堆顶
//
// 所以: cmp(a, b):
//   频率不同: freq[a] > freq[b] 返回 true → a 频率高但优先级低(沉底)
//   频率相同: a < b 返回 true → a 字典序小但优先级低(沉底)

/*
class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
        // 统计频率
        unordered_map<string, int> freq;
        for (const string& w : words) freq[w]++;

        // 最小堆比较器: 最不重要的在堆顶
        // "更重要" = 频率更高 或 同频字典序更小 → 返回 true (沉底)
        auto cmp = [&](const string& a, const string& b) {
            if (freq[a] != freq[b]) return freq[a] > freq[b];
            return a < b;
        };
        priority_queue<string, vector<string>, decltype(cmp)> minHeap(cmp);

        // 遍历所有单词, 维护大小为 k 的堆
        for (auto& [word, _] : freq) {
            minHeap.push(word);
            if ((int)minHeap.size() > k) {
                minHeap.pop();  // 弹出堆顶(最不重要的)
            }
        }

        // 弹出并反转 (堆弹出顺序: 不重要→重要, 需要反转)
        vector<string> result(k);
        for (int i = k - 1; i >= 0; i--) {
            result[i] = minHeap.top();
            minHeap.pop();
        }
        return result;
    }
};
*/

/*
 * 堆操作详解 (words=["i","love","leetcode","i","love","coding"], k=2):
 *
 * freq: {"i":2, "love":2, "leetcode":1, "coding":1}
 *
 * 处理 "i"(2):
 *   push "i" → 堆: ["i"(2)]   size=1 <= k=2, 不弹出
 *
 * 处理 "love"(2):
 *   push "love" → 堆: ["love"(2), "i"(2)]   size=2 <= k=2, 不弹出
 *   堆顶: "love" (频率2, 字典序大 → 最不重要)
 *
 * 处理 "leetcode"(1):
 *   push "leetcode" → 堆大小=3 > k=2
 *   pop 堆顶 → 弹出 "leetcode"(1) (频率最低, 最不重要)
 *   堆: ["love"(2), "i"(2)]
 *
 * 处理 "coding"(1):
 *   push "coding" → 堆大小=3 > k=2
 *   pop 堆顶 → 弹出 "coding"(1) (频率最低, 最不重要)
 *   堆: ["love"(2), "i"(2)]
 *
 * 弹出: "love", "i"  (从不重要到重要)
 * 反转: ["i", "love"] ✓
 */


// ============================================================
// 解法3: 桶排序 — O(n) 平均 / O(n)
// ============================================================
// 思路: 按频率分桶，频率为 f 的单词放入 bucket[f]。
//       从最高频率桶开始遍历，桶内按字典序排序，取够 k 个即停。

/*
class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
        unordered_map<string, int> freq;
        for (const string& w : words) freq[w]++;

        // 找最大频率
        int maxFreq = 0;
        for (auto& [_, f] : freq) maxFreq = max(maxFreq, f);

        // 建桶: bucket[i] = 出现 i 次的单词
        vector<vector<string>> buckets(maxFreq + 1);
        for (auto& [word, f] : freq) {
            buckets[f].push_back(word);
        }

        // 桶内按字典序排序
        for (auto& bucket : buckets) {
            sort(bucket.begin(), bucket.end());
        }

        // 从高频桶向低频桶取, 直到取够 k 个
        vector<string> result;
        for (int f = maxFreq; f >= 0 && (int)result.size() < k; f--) {
            for (const string& w : buckets[f]) {
                result.push_back(w);
                if ((int)result.size() == k) break;
            }
        }
        return result;
    }
};
*/


// ============================================================
// 比较器对比表 (理解方向差异的核心!)
// ============================================================
/*
 * ┌─────────────┬──────────────────────────┬──────────────────────────┐
 * │             │  排序 (sort 的 cmp)      │  最小堆 (pq 的 cmp)     │
 * ├─────────────┼──────────────────────────┼──────────────────────────┤
 * │ 频率不同时  │ freq[a]>freq[b] → a排前  │ freq[a]>freq[b] → a沉底 │
 * │             │ (高频在前)               │ (高频是重要的, 沉底保留)  │
 * ├─────────────┼──────────────────────────┼──────────────────────────┤
 * │ 频率相同时  │ a<b → a排前              │ a<b → a沉底              │
 * │             │ (字典序小在前)            │ (字典序小更重要, 沉底保留) │
 * ├─────────────┼──────────────────────────┼──────────────────────────┤
 * │ 返回true    │ a 排在 b 前面            │ a 优先级低于 b           │
 * │ 的含义      │                          │ (b 更靠近堆顶)           │
 * └─────────────┴──────────────────────────┴──────────────────────────┘
 *
 * 注意: 两个比较器的 lambda 代码完全相同!
 *   [&](a, b) { freq[a]>freq[b] ? true : a<b; }
 * 但含义不同:
 *   sort: "a 应该排在 b 前面" (a 更优先)
 *   pq:   "a 优先级低于 b"   (a 沉底, b 上浮到堆顶)
 * 效果: sort 让频率高的在前; pq 让频率低的在堆顶 → 刚好互补!
 */


// ============================================================
// 常见错误示范
// ============================================================

// 错误1: 堆的比较器方向写反 (变成最大堆)
// auto cmp = [&](const string& a, const string& b) {
//     if (freq[a] != freq[b]) return freq[a] < freq[b]; // ✗ 频率低沉底
//     return a > b;  // ✗ 字典序大沉底
// };
// → 堆顶是最重要的, 每次弹出的是最重要的 → 结果完全反了

// 错误2: 同频字典序方向搞反
// sort(... [&](a, b) { return freq[a]!=freq[b] ? freq[a]>freq[b] : a>b; });
// → "love" 排在 "i" 前面 (应该是 "i" 在前)

// 错误3: 堆弹出后忘记反转
// while (!minHeap.empty()) { result.push_back(minHeap.top()); minHeap.pop(); }
// → 结果是 ["love", "i"], 应该是 ["i", "love"]

// 错误4: 堆大小判断用 >= k 而非 > k
// if (minHeap.size() >= k) minHeap.pop();
// → push 后立刻 pop, 堆中只保留 k-1 个元素
// ✓ 应该 push 后检查是否 > k 再 pop, 保证堆中恰好 k 个


// ============================================================
// 【解法对比】
// ============================================================
// 哈希+排序   O(n log n)/O(n) — 代码最简洁，面试首选
// 最小堆(k)   O(n log k)/O(n) — 满足 follow-up 的 O(n log k)，n 很大时更优
// 桶排序      O(n) 平均 /O(n) — 理论最快，但桶内排序有额外开销，实际优势不明显
//
// 选用场景:
//   n 不大（<=10^4）→ 哈希+排序，代码短
//   n 很大，k 很小 → 最小堆，严格 O(n log k)
//   面试被追问 O(n) 方案 → 桶排序（需解释桶内排序仍为 O(m log m)）

// ============================================================
// 【易错点】
// ============================================================
// ✗ auto cmp = [&](a,b){ return freq[a] < freq[b]; };  // 最大堆，堆顶是最重要的
// ✓ auto cmp = [&](a,b){ return freq[a] > freq[b]; };  // 最小堆，堆顶是最不重要的
//   TopK 问题用最小堆！让不重要的在堆顶被淘汰。
//
// ✗ 同频时字典序方向搞反: return a > b;  // "love" 排在 "i" 前面
// ✓ sort 里: return a < b;   // 字典序小的排前
//   pq  里: return a < b;   // 字典序小的沉底保留（代码相同含义不同！）
//
// ✗ 堆弹出后直接 push_back 不反转
// ✓ 堆弹出顺序是"最不重要→最重要"，必须反转或从后往前填充 result[k-1..0]

// ============================================================
// 【面试追问 Interview Follow-ups】
// ============================================================
// Q1: 如果只需要前 k 个但不需要排序，有更快的方案吗？
// A1: 可以用 nth_element (快速选择) O(n) 找到第 k 大频率分界，
//     但本题要求输出有序（频率降序+字典序），所以最终仍需排序前 k 个。
//
// Q2: 如果数据是流式输入（单词不断到来），如何实时维护 Top K？
// A2: 用哈希表维护频率 + 大小为 k 的最小堆。每来一个单词更新频率后，
//     检查是否需要替换堆顶。但频率变化时堆无法高效更新，
//     实际可用 TreeMap/有序集合(如 C++ set<pair<int,string>>) 做到
//     O(log n) 的动态 Top K。
//
// Q3: 如果要返回 Top K 中出现频率相同的所有单词（不截断），怎么改？
// A3: 先找到第 k 个单词的频率 threshold，然后返回所有频率 >= threshold 的单词，
//     按(频率降序, 字典序升序)排列。结果可能超过 k 个。
