/*
 * LeetCode 1268: Search Suggestions System (搜索推荐系统)
 * 
 * 【题目本质】
 * 对 searchWord 的每个前缀，从排序后的 products 中找出字典序最小的至多 3 个匹配产品。
 * 核心观察：排序后，匹配同一前缀的产品是连续的一段。
 *
 * 【解法总览】
 * 解法1: 暴力过滤     — O(L×N×M) / O(sort)     — 最直觉
 * 解法2: 排序+二分查找 — O(NlogN + L×logN) / O(1) — 面试首选
 * 解法3: 排序+双指针   — O(NlogN + N) / O(1)      — 思路优雅
 * 解法4: Trie 前缀树   — O(总字符数) / O(总字符数) — 多查询场景
 */

// ============================================================
// 解法1: 暴力过滤 — 排序后每次遍历所有产品检查前缀
// 时间: O(L × N × M)  空间: O(sort)
//
// 【思路】
// 最直觉的想法：先排序保证字典序，然后对 searchWord 的每个前缀，
// 遍历所有产品检查是否以该前缀开头。
// 因为已排序，找到前 3 个匹配的就可以 break。
// 瓶颈：每次都从头遍历 N 个产品，定位匹配位置太慢。
//
// 示例: products = ["mobile","mouse","moneypot","monitor","mousepad"]
// 排序后: ["mobile","moneypot","monitor","mouse","mousepad"]
//
// 前缀 "m":     遍历全部 → mobile, moneypot, monitor (前3个匹配)
// 前缀 "mo":    遍历全部 → mobile, moneypot, monitor
// 前缀 "mou":   遍历全部 → 跳过 mobile,moneypot,monitor → mouse, mousepad
// 前缀 "mous":  遍历全部 → mouse, mousepad
// 前缀 "mouse": 遍历全部 → mouse, mousepad
// ============================================================
class Solution1 {
public:
    vector<vector<string>> suggestedProducts(vector<string>& products, string searchWord) {
        sort(products.begin(), products.end());
        vector<vector<string>> result;
        string prefix;
        
        for (char c : searchWord) {
            prefix += c;
            vector<string> suggestions;
            for (const string& p : products) {
                // 检查 p 是否以 prefix 开头
                if (p.size() >= prefix.size() && 
                    p.compare(0, prefix.size(), prefix) == 0) {
                    suggestions.push_back(p);
                    // 排序后前 3 个匹配的就是字典序最小的
                    if (suggestions.size() == 3) break;
                }
            }
            result.push_back(suggestions);
        }
        return result;
    }
};

// ============================================================
// 解法2: 排序 + 二分查找 — lower_bound 快速定位匹配起点
// 时间: O(N log N + L × (log N + M))  空间: O(sort额外空间)
//
// 【思路】
// 解法1的瓶颈：每次都遍历全部产品来找第一个匹配位置。
// 关键观察：排序后，匹配同一前缀的产品是连续的一段。
// 用 lower_bound 找"第一个 >= prefix 的位置"，然后往后看最多 3 个。
//
// 二分搜索空间收缩过程（示例）:
// 排序后: ["mobile","moneypot","monitor","mouse","mousepad"]
//           0        1          2        3       4
//
// 前缀 "m":
//   lower_bound("m") → index 0 ("mobile")
//   检查 [0,1,2]: mobile✓ moneypot✓ monitor✓ → 取3个
//
// 前缀 "mou":
//   lower_bound("mou") → index 3 ("mouse")
//   检查 [3,4]: mouse✓ mousepad✓ → 取2个
//
// 为什么 lower_bound 后还要验证前缀？
// 因为 lower_bound 只保证 >= prefix，不保证前缀匹配。
// 例如 prefix="abc", lower_bound 可能找到 "abd"（>= "abc" 但不以 "abc" 开头）
// ============================================================
class Solution2 {
public:
    vector<vector<string>> suggestedProducts(vector<string>& products, string searchWord) {
        sort(products.begin(), products.end());
        vector<vector<string>> result;
        string prefix;
        
        for (char c : searchWord) {
            prefix += c;
            // O(log N): 找第一个 >= prefix 的位置
            auto it = lower_bound(products.begin(), products.end(), prefix);
            
            vector<string> suggestions;
            for (int i = 0; i < 3 && it + i != products.end(); i++) {
                const string& candidate = *(it + i);
                // 必须验证 candidate 确实以 prefix 开头
                if (candidate.size() >= prefix.size() && 
                    candidate.compare(0, prefix.size(), prefix) == 0) {
                    suggestions.push_back(candidate);
                } else {
                    // 排序保证：一旦不匹配，后续也不匹配
                    break;
                }
            }
            result.push_back(suggestions);
        }
        return result;
    }
};

// ============================================================
// 解法3: 排序 + 双指针 — 利用前缀越长匹配范围越小的单调性
// 时间: O(N log N + N + L×M)  空间: O(sort额外空间)
//
// 【思路】
// 另一个角度：维护候选区间 [left, right]，初始覆盖所有产品。
// 每输入一个新字符，从两端收缩：
//   - 左端产品如果太短或第 i 个字符不匹配 → left++
//   - 右端产品如果太短或第 i 个字符不匹配 → right--
// 因为前缀只会变长，候选范围只会缩小（单调性），所以双指针不会往回走。
// 总收缩次数 ≤ N，效率很高。
//
// 过程演示:
// 排序后: ["mobile","moneypot","monitor","mouse","mousepad"]
//           0        1          2        3       4
//
// i=0, c='m': left=0, right=4 → 全部第0位都是'm' → 不收缩
//             取 [0,1,2]: mobile, moneypot, monitor
//
// i=1, c='o': left=0, right=4 → 全部第1位都是'o' → 不收缩
//             取 [0,1,2]: mobile, moneypot, monitor
//
// i=2, c='u': left=0, right=4
//   products[0]="mobile"  → 'b' != 'u' → left=1
//   products[1]="moneypot"→ 'n' != 'u' → left=2
//   products[2]="monitor" → 'n' != 'u' → left=3
//   products[4]="mousepad"→ 'u' == 'u' → 不收缩right
//   取 [3,4]: mouse, mousepad
// ============================================================
class Solution3 {
public:
    vector<vector<string>> suggestedProducts(vector<string>& products, string searchWord) {
        sort(products.begin(), products.end());
        vector<vector<string>> result;
        int left = 0, right = (int)products.size() - 1;
        
        for (int i = 0; i < (int)searchWord.size(); i++) {
            char c = searchWord[i];
            
            // 左端收缩：产品长度不够 或 第i个字符不匹配
            while (left <= right && 
                   ((int)products[left].size() <= i || products[left][i] != c)) {
                left++;
            }
            // 右端收缩：产品长度不够 或 第i个字符不匹配
            while (left <= right && 
                   ((int)products[right].size() <= i || products[right][i] != c)) {
                right--;
            }
            
            vector<string> suggestions;
            int count = min(3, right - left + 1);
            for (int j = 0; j < count; j++) {
                suggestions.push_back(products[left + j]);
            }
            result.push_back(suggestions);
        }
        return result;
    }
};

// ============================================================
// 解法4: Trie 前缀树 — 每个节点预存 top-3 建议
// 时间: O(总字符数 + L)  空间: O(总字符数)
//
// 【思路】
// 如果有多个 searchWord 要查询，每次都排序+二分不划算。
// Trie 的优势：预处理一次，之后每次查询 O(searchWord 长度)。
//
// 做法：
// 1. 先排序 products（保证插入顺序就是字典序）
// 2. 把每个产品插入 Trie，在经过的每个节点上记录该产品（最多记 3 个）
// 3. 查询时沿 searchWord 逐字符走 Trie，每个节点上的列表就是答案
//
// Trie 结构示意（示例1，部分展示）:
//
//  root
//   └── m
//       └── o
//           ├── b → i → l → e              ("mobile")
//           ├── n
//           │   ├── e → y → p → o → t      ("moneypot")
//           │   └── i → t → o → r          ("monitor")
//           └── u
//               └── s → e                   ("mouse")
//                       └── p → a → d       ("mousepad")
//
// 节点 'o'(m→o) 的 suggestions: ["mobile","moneypot","monitor"]
// 节点 'u'(m→o→u) 的 suggestions: ["mouse","mousepad"]
// ============================================================
class Solution4 {
public:
    struct TrieNode {
        TrieNode* children[26] = {};
        vector<string> suggestions; // 经过此节点的字典序最小的 ≤3 个产品
    };
    
    vector<vector<string>> suggestedProducts(vector<string>& products, string searchWord) {
        // 排序保证插入顺序即字典序 → 先插入的产品字典序更小
        sort(products.begin(), products.end());
        
        // 建 Trie
        TrieNode* root = new TrieNode();
        for (const string& product : products) {
            TrieNode* node = root;
            for (char c : product) {
                int idx = c - 'a';
                if (!node->children[idx]) {
                    node->children[idx] = new TrieNode();
                }
                node = node->children[idx];
                // 因为按字典序插入，前 3 个到达此节点的产品就是答案
                if (node->suggestions.size() < 3) {
                    node->suggestions.push_back(product);
                }
            }
        }
        
        // 查询：沿 searchWord 逐字符走 Trie
        vector<vector<string>> result;
        TrieNode* node = root;
        bool found = true; // 一旦断开，后续全为空
        
        for (char c : searchWord) {
            if (found && node->children[c - 'a']) {
                node = node->children[c - 'a'];
                result.push_back(node->suggestions);
            } else {
                found = false;
                result.push_back({});
            }
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间               | 空间           | 适用场景        |
// |------------|-------------------|----------------|----------------|
// | 暴力       | O(L×N×M)          | O(sort)        | 数据极小        |
// | 排序+二分  | O(NlogN + L×logN) | O(sort)        | 通用，面试首选  |
// | 排序+双指针| O(NlogN + N)      | O(sort)        | 通用，思路优雅  |
// | Trie       | O(总字符数)        | O(总字符数)     | 多次查询       |
//
// 面试推荐顺序：
// 1. 先说排序+二分（最直观的优化，面试官必满意）
// 2. 被追问"还能更好吗"→ 说双指针（利用单调性避免重复搜索）
// 3. 被追问"大量查询怎么办"→ 说 Trie（预处理 + O(L) 查询）
//
// 【易错点】
// 1. 越界访问: products[left][i] 前必须检查 products[left].size() > i
//    ✗ while (left <= right && products[left][i] != c)  // 长度不够会越界
//    ✓ while (left <= right && ((int)products[left].size() <= i || products[left][i] != c))
//
// 2. lower_bound 后忘记验证前缀:
//    ✗ auto it = lower_bound(...); result.push_back({*it, *(it+1), *(it+2)});
//    ✓ 必须检查每个 candidate 确实以 prefix 开头，lower_bound 只保证 >= prefix
//
// 3. Trie 断开后继续访问:
//    ✗ node = node->children[c-'a']; // node 可能为 nullptr，下一轮会段错误
//    ✓ 用 found 标记，一旦某个字符不存在，后续全部返回空列表
//
// 4. 双指针中 right-left+1 可能为负:
//    ✗ int count = min(3, right - left + 1); // 如果 left > right，此值为负数
//    ✓ 这里 min(3, right-left+1) 在 left>right 时返回负数，for 循环不执行，结果正确
//       但如果用 unsigned 比较会出 bug，建议先判断 left <= right
//
// 【面试追问】
// Q1: "这道题的暴力解是什么？瓶颈在哪？"
//   → 每个前缀遍历所有产品检查匹配，O(L×N×M)。瓶颈在"定位第一个匹配"。
//
// Q2: "排序+二分 和 排序+双指针，你更推荐哪个？"
//   → 二分更通用更安全（每次独立查找，不依赖前一次结果）。
//     双指针利用了"前缀越长范围越小"的单调性，总移动次数 ≤ N，理论更优。
//     面试中二分更容易写对，推荐先写二分。
//
// Q3: "如果有成千上万个不同的 searchWord 要查询？"
//   → Trie 预处理一次 O(总字符数)，之后每个查询 O(searchWord长度)。
//     排序+二分每次查询 O(L×logN)，大量查询时 Trie 更优。
//
// Q4: "如果 products 会动态增删呢？"
//   → 排序+二分不适合动态场景（每次增删要重新排序）。
//     Trie 支持动态插入，但删除复杂（节点上的 top-3 列表需要维护）。
//     可以考虑用 Trie + 每个节点存 priority_queue 来维护动态 top-3。
// ============================================================
