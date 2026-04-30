/*
 * LeetCode 1002: 查找共用字符 (Find Common Characters)
 * 
 * 【题目本质】
 * 求 26 个字母在所有字符串中出现频次的最小值，按最小值次数输出字符。
 *
 * 【解法总览】
 * 解法1: 频次取最小值 — O(n·m) / O(1) — 最优解，面试首选
 * 解法2: multiset 交集模拟 — O(n·m) / O(m) — 直觉性更强的做法
 * 
 * n = 字符串个数, m = 字符串平均长度
 */

// ============================================================
// 解法1: 频次取最小值 — 统计每个字符串的字母频次，逐个取 min
// 时间: O(n·m)  空间: O(1)（26长数组为常数）
// 
// 【思路】
// 一个字符能出现在答案中 k 次，当且仅当它在每个字符串中都至少出现 k 次。
// 所以答案中某字符的出现次数 = 该字符在所有字符串中频次的最小值。
// 
// 做法：
// 1. 用第一个字符串的频次初始化 minFreq
// 2. 遍历后续字符串，统计频次，对 26 个字母逐位取 min
// 3. 最后 minFreq[c] 就是字符 c 在答案中应出现的次数
//
// 示例: words = ["bella", "label", "roller"]
//
//          a  b  e  l  (其余省略，都是0)
// bella:   1  1  1  2
// label:   1  1  1  2
// roller:  0  0  1  2
//          ──────────
// min:     0  0  1  2   → 输出 ["e", "l", "l"]
// ============================================================
class Solution1 {
public:
    vector<string> commonChars(vector<string>& words) {
        // 用第一个字符串的频次初始化，避免用 INT_MAX
        vector<int> minFreq(26, 0);
        for (char c : words[0]) {
            minFreq[c - 'a']++;
        }
        
        // 从第二个字符串起，逐个统计频次并取 min
        for (int i = 1; i < (int)words.size(); i++) {
            vector<int> curFreq(26, 0);
            for (char c : words[i]) {
                curFreq[c - 'a']++;
            }
            // 为什么取 min？因为答案中某字符的次数受"最少"的那个字符串限制
            for (int j = 0; j < 26; j++) {
                minFreq[j] = min(minFreq[j], curFreq[j]);
            }
        }
        
        // 根据最终频次构造结果
        vector<string> result;
        for (int j = 0; j < 26; j++) {
            // minFreq[j] 是几就放几个该字母
            for (int k = 0; k < minFreq[j]; k++) {
                result.push_back(string(1, 'a' + j));
            }
        }
        return result;
    }
};

// ============================================================
// 解法2: multiset 交集模拟 — 逐字符串求"多重集合交集"
// 时间: O(n·m)  空间: O(m)
// 
// 【思路】
// 把每个字符串看成一个"字符包"（多重集合），问题就是求所有包的交集。
// 
// 模拟过程：
// 1. 把第一个字符串的所有字符放入"候选池" pool
// 2. 对后续每个字符串，统计其字符到 available map
// 3. 遍历 pool：如果字符在 available 中还有剩余，保留它并消耗一个
//    如果没有，丢弃这个字符
// 4. 最终 pool 中剩下的就是所有字符串的公共字符
//
// 为什么要"消耗"（available[c]--）？
// → 防止 pool 中 2 个 'l' 都匹配到当前字符串中只有 1 个 'l' 的情况
//
// 示例: words = ["bella", "label", "roller"]
// 
// 初始 pool = [b, e, l, l, a]  （来自 "bella"）
// 
// 处理 "label" (available: l=2, a=1, b=1, e=1):
//   b → available有 → 保留, available[b]=0
//   e → available有 → 保留, available[e]=0
//   l → available有 → 保留, available[l]=1
//   l → available有 → 保留, available[l]=0
//   a → available有 → 保留, available[a]=0
//   pool = [b, e, l, l, a]
//
// 处理 "roller" (available: r=2, o=1, l=2, e=1):
//   b → available无 → 丢弃
//   e → available有 → 保留, available[e]=0
//   l → available有 → 保留, available[l]=1
//   l → available有 → 保留, available[l]=0
//   a → available无 → 丢弃
//   pool = [e, l, l]  → 结果！
// ============================================================
class Solution2 {
public:
    vector<string> commonChars(vector<string>& words) {
        // 第一个字符串作为初始候选池
        vector<char> pool(words[0].begin(), words[0].end());
        
        for (int i = 1; i < (int)words.size(); i++) {
            // 统计当前字符串的可用字符
            unordered_map<char, int> available;
            for (char c : words[i]) {
                available[c]++;
            }
            
            // 筛选：pool 中只保留 available 里还有的字符
            vector<char> nextPool;
            for (char c : pool) {
                if (available[c] > 0) {
                    nextPool.push_back(c);
                    available[c]--;  // 消耗一个，确保不超额匹配
                }
            }
            pool = nextPool;
        }
        
        // 转换为 vector<string>
        vector<string> result;
        for (char c : pool) {
            result.push_back(string(1, c));
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
// 
// 解法1 (频次取 min):
//   - 更简洁，利用"只有26个字母"的特点
//   - 空间 O(1)，逻辑清晰
//   - 面试推荐首选
// 
// 解法2 (multiset 交集):
//   - 更贴近"集合交集"的直觉
//   - 如果字符集不固定（如 Unicode），这种方式更通用
//   - 空间 O(m)，稍大
//
// 两者时间复杂度相同 O(n·m)，面试优先写解法1。
//
// 【易错点】
// 1. ✗ 用 set 而非频次计数 → 丢失重复信息，"l" 只输出一次
//    ✓ 必须用频次数组或 multiset 来处理重复
//
// 2. ✗ minFreq 初始化为全 0 然后对所有字符串取 min → 结果全是 0
//    ✓ 用第一个字符串的频次初始化，或者用 INT_MAX 初始化
//
// 3. ✗ result.push_back('a' + j) → 类型错误，push 了 char 不是 string
//    ✓ result.push_back(string(1, 'a' + j))
//
// 4. ✗ 解法2中不做 available[c]-- → pool 中多个相同字符都能匹配
//    ✓ 每匹配一个必须消耗一个，否则结果会多出字符
//
// 【面试追问】
// Q1: 这道题的核心观察是什么？
//   → 答案中字符 c 的出现次数 = min(c 在每个字符串中的出现次数)
//     这是因为"共同出现"意味着受最少的那个字符串限制。
//
// Q2: 如果字符集是 Unicode 全集而非 26 个小写字母，怎么改？
//   → 把 vector<int>(26) 换成 unordered_map<int, int>，逻辑不变。
//     解法2天然支持任意字符集。
//
// Q3: 如果 words 很长（百万级），每个字符串很短，能否提前终止？
//   → 可以。每次取完 min 后检查 minFreq 是否全为 0，
//     如果全为 0 说明已无公共字符，后续字符串无需处理。
//
// 【相关题型】
// - 349. 两个数组的交集 — 不含重复的简化版，用 set 即可
// - 350. 两个数组的交集 II — 两个数组版本，直接复用"频次取 min"
//   区别：350 只有两个数组，本题是 n 个字符串
// - 383. 赎金信 — 也是频次计数，但判断 A 能否被 B 覆盖（单向），
//   本题是双向取 min（交集）
// ============================================================
