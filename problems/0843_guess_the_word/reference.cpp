/*
 * LeetCode 843: Guess the Word
 *
 * 【题目本质】
 * 交互式信息博弈：每次猜一个词获得位置匹配数反馈，需在有限次数内
 * 猜中秘密单词。核心是如何选择每次猜测的词，使反馈信息最大化地缩小候选集。
 *
 * 【解法总览】
 * 解法1: 随机猜测 + 过滤       — O(n²) / O(n) — 最直觉，理解题意
 * 解法2: Minimax 最优猜测      — O(n²) / O(n) — 面试首选 ⭐
 * 解法3: 最小化零匹配分组      — O(n²) / O(n) — 简化启发式，实践效果好
 */

// ============================================================
// 辅助函数：计算两个长度6的字符串有几个位置完全匹配
// 这就是 Bulls and Cows 中的"公牛数"
// ============================================================
// match("acckzz", "ccbazz")
//   a≠c, c==c, c≠b, k≠a, z==z, z==z → 返回 3

// ============================================================
// 解法1: 随机猜测 + 过滤
// 时间: O(n²) 总计（每轮 O(n) 过滤，最多 10 轮）
// 空间: O(n)
//
// 【思路】
// 最朴素的想法：随便从候选集中选一个词猜，系统返回匹配数 m。
// 然后利用 m 过滤候选集：只保留和我猜的词匹配数恰好为 m 的词。
// 因为 secret 和我猜的词匹配数就是 m，所以 secret 一定在过滤后的集合中。
// 
// 为什么可能不够好：如果选的词恰好和大部分候选词匹配数都是 0，
// 返回 0 时几乎排除不了任何词，候选集缩小很慢。
// ============================================================
class Solution1 {
public:
    int match(const string& a, const string& b) {
        int cnt = 0;
        for (int i = 0; i < 6; i++)
            if (a[i] == b[i]) cnt++;
        return cnt;
    }
    
    void findSecretWord(vector<string>& words, Master& master) {
        vector<string> candidates = words;
        
        for (int t = 0; t < 10 && !candidates.empty(); t++) {
            // 随机选一个候选词来猜
            string guess_word = candidates[rand() % candidates.size()];
            int m = master.guess(guess_word);
            
            // 猜中了直接返回
            if (m == 6) return;
            
            // 核心过滤逻辑：只保留和 guess_word 匹配数恰好为 m 的词
            // 为什么这样过滤是对的？
            //   因为 match(secret, guess_word) == m
            //   所以 secret 在过滤后的集合中（不会被误删）
            //   而 match(w, guess_word) != m 的 w 一定不是 secret（可以安全删除）
            vector<string> next;
            for (const string& w : candidates) {
                if (match(w, guess_word) == m) {
                    next.push_back(w);
                }
            }
            candidates = next;
        }
    }
};

// ============================================================
// 解法2: Minimax 最优猜测 — 面试首选 ⭐
// 时间: O(n²) 每轮选词 + O(n) 过滤，总计 O(n² × 轮数)
// 空间: O(n)
//
// 【思路】
// 解法1的瓶颈：随机选词可能选到"信息量低"的词。
// 
// 优化思路：对每个候选词 g，预判"如果选 g 来猜，最坏情况下
// 剩余候选集有多大"。
// 
// 具体做法：
//   对每个 g，统计它和所有候选词的匹配数分布
//   groups[i] = 和 g 匹配数为 i 的候选词数量
//   最坏情况 = max(groups[0], groups[1], ..., groups[6])
//   选使最坏情况最小的 g
//
// 为什么这样更好？
//   类比猜数字游戏，你总想"二分"搜索空间。这里我们不是二分，
//   而是按匹配数分成最多 7 组，让最大的组尽可能小。
//
// 决策过程示例 (5个候选词):
//
//   如果选 wordA 来猜:           如果选 wordB 来猜:
//   match=0: {C, D}    → 2个     match=0: {A, C, D, E} → 4个
//   match=1: {B, E}    → 2个     match=1: {}            → 0个
//   match=3: {}         → 0个     match=2: {}            → 0个
//   最大分组 = 2                  最大分组 = 4
//
//   选 wordA! 因为 2 < 4 → A 的最坏情况更好
// ============================================================
class Solution2 {
public:
    int match(const string& a, const string& b) {
        int cnt = 0;
        for (int i = 0; i < 6; i++)
            if (a[i] == b[i]) cnt++;
        return cnt;
    }
    
    void findSecretWord(vector<string>& words, Master& master) {
        vector<string> candidates = words;
        
        for (int t = 0; t < 10 && !candidates.empty(); t++) {
            // ========== 选最优猜测词：Minimax ==========
            string best_guess = candidates[0];
            int best_worst = (int)candidates.size(); // 初始化为最大（越小越好）
            
            for (const string& g : candidates) {
                // 统计如果猜 g，各匹配数的分组大小
                // groups[i] = 候选词中和 g 恰好匹配 i 个位置的词数
                int groups[7] = {};
                for (const string& w : candidates) {
                    groups[match(g, w)]++;
                }
                
                // 最坏情况 = 最大分组大小
                // （无论系统返回什么值，我们都要面对对应分组大小的候选集）
                int worst = 0;
                for (int i = 0; i < 7; i++) {
                    worst = max(worst, groups[i]);
                }
                
                // 贪心：选使最坏情况最小的词
                if (worst < best_worst) {
                    best_worst = worst;
                    best_guess = g;
                }
            }
            
            // 用选出的最优词来猜
            int m = master.guess(best_guess);
            if (m == 6) return;
            
            // 根据反馈过滤候选集
            vector<string> next;
            for (const string& w : candidates) {
                if (match(w, best_guess) == m) {
                    next.push_back(w);
                }
            }
            candidates = next;
        }
    }
};

// ============================================================
// 解法3: 最小化零匹配分组 — 简化启发式
// 时间: O(n²) / O(n)
//
// 【思路】
// 观察：在 Minimax 中，最大分组几乎总是 groups[0]（匹配数为 0 的分组）。
// 
// 为什么？两个随机的 6 字母字符串，每个位置匹配的概率约 1/26。
// 6 个位置全不匹配的概率 ≈ (25/26)^6 ≈ 79%。
// 所以大部分词对之间匹配数为 0，groups[0] 通常是最大的分组。
//
// 简化策略：只关注 groups[0]，选与其他候选词匹配 0 次数最少的词。
// 这等价于 Minimax 只看 i=0 这一组，代码更简洁，效果接近。
// ============================================================
class Solution3 {
public:
    int match(const string& a, const string& b) {
        int cnt = 0;
        for (int i = 0; i < 6; i++)
            if (a[i] == b[i]) cnt++;
        return cnt;
    }
    
    void findSecretWord(vector<string>& words, Master& master) {
        vector<string> candidates = words;
        
        for (int t = 0; t < 10 && !candidates.empty(); t++) {
            // 选与其他词 match==0 次数最少的词
            // 直觉：这个词和其他词"重叠度"更高，信息量更大
            string best_guess = candidates[0];
            int min_zero = (int)candidates.size();
            
            for (const string& g : candidates) {
                int zero_count = 0;
                for (const string& w : candidates) {
                    if (match(g, w) == 0) zero_count++;
                }
                if (zero_count < min_zero) {
                    min_zero = zero_count;
                    best_guess = g;
                }
            }
            
            int m = master.guess(best_guess);
            if (m == 6) return;
            
            vector<string> next;
            for (const string& w : candidates) {
                if (match(w, best_guess) == m) {
                    next.push_back(w);
                }
            }
            candidates = next;
        }
    }
};

// ============================================================
// 【解法对比】
//
// 三种解法的过滤逻辑完全相同（match(w, guess) == m 才保留），
// 区别仅在"选哪个词来猜":
//
//   解法1 随机选      → 简单但不稳定，最坏情况可能超时
//   解法2 Minimax     → 最大分组最小化，理论上最优贪心 ⭐
//   解法3 最小化零匹配 → Minimax 的简化版，利用 groups[0] 通常最大的经验
//
// 面试推荐路径：
//   1) 先说随机+过滤的思路，展示你理解了题意和反馈信息的价值
//   2) 指出随机策略的缺陷（最坏情况排除不了多少）
//   3) 引出 Minimax：对每个候选词评估最坏情况，选最好的
//   4) 如果时间紧张，写解法3（只看 zero_count）也完全可以
//
// 【易错点】
//
// 1. match 函数必须是位置匹配（a[i]==b[i]），不是字符集合的交集
//    ✗ 用 unordered_map 统计字符频率求交集（那是 Wordle 的黄色匹配）
//    ✓ for (int i = 0; i < 6; i++) if (a[i] == b[i]) cnt++;
//
// 2. groups 数组大小应为 7（匹配数 0~6），不是 6
//    ✗ int groups[6] = {};  // 索引越界！match 返回 6 时写 groups[6]
//    ✓ int groups[7] = {};
//
// 3. 猜中时必须立即 return，否则继续循环可能导致候选集为空后数组越界
//    ✗ 忘记 if (m == 6) return; 直接过滤
//    ✓ 先检查 m == 6，再过滤
//
// 4. 过滤后候选集不包含 guess_word 本身（因为 match(g,g)==6 != m）
//    这是自然的，不需要特别处理，但要理解为什么
//
// 【面试追问】
//
// Q1: 为什么随机猜不够好？→ 最坏情况下候选集缩小太慢，10次猜不完
//
// Q2: Minimax 是全局最优吗？→ 不是，它是贪心（只看一步）。
//     全局最优需要博弈树搜索，复杂度指数级。但 n≤100 下贪心足够。
//
// Q3: 如果 n 很大（比如 10^5）怎么办？→ Minimax 选词是 O(n²)，
//     可以改用基于字母频率的启发式（O(n) 选词）：统计每个位置
//     每个字母的频率，选各位置字母频率之和最大的词。
//
// Q4: 这道题和 Mastermind 游戏的关系？→ 本题是 Mastermind 的简化版：
//     只有 Bulls（位置+值匹配），没有 Cows（值匹配位置不同）。
//     Minimax 是 Mastermind 的经典策略（Knuth 1977）。
// ============================================================
