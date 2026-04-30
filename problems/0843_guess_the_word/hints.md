# 843. 猜猜这个单词

## 核心思路

这道题本质是一个**交互式信息博弈**：你每次猜一个单词，系统告诉你匹配了几个位置，你需要利用这个反馈信息**尽快缩小候选集**，在有限次数内猜中秘密单词。核心问题是：**每次该猜哪个单词，才能让反馈信息最大化地缩小候选范围？**

## 思维链

1. **读完题第一反应**：这是个交互题，我每次猜一个词，得到匹配数。最朴素的做法——随机猜，期望每次能排除一些候选词。但随机猜的效率不稳定，最坏情况下可能排除不了多少。

2. **理解反馈信息的含义**：当我猜了单词 `g`，系统返回匹配数 `m`。这意味着 **secret 和 g 恰好有 m 个位置相同**。所以所有和 `g` 匹配数不等于 `m` 的候选词都可以排除！这就是"信息"的价值。

3. **瓶颈在哪？** 如果我选的猜测词很"偏"，比如它和大部分候选词匹配数都是 0，那得到 0 的反馈几乎排除不了什么（因为大部分词匹配数本来就是 0）。我们需要选一个词，使得**无论返回什么匹配数，剩余候选集都尽可能小**。

4. **启发式策略——最小化最大分组(Minimax)**：对于每个候选词 `w`，假设选它来猜，统计它和所有其他候选词的匹配数分布（匹配 0 个的有多少、匹配 1 个的有多少...匹配 6 个的有多少）。最坏情况下，剩余候选集大小 = 最大分组的大小。我们选择使**最大分组最小**的那个词来猜。

5. **实现流程**：维护一个候选列表 → 选最优猜测词 → 调用 guess → 根据返回值过滤候选列表 → 重复直到猜中。

6. **为什么 10 次够用？** 每个词 6 个字母，匹配数 0~6 共 7 种。如果分布均匀，每次能把候选集缩小到约 1/7。100 个词最多需要 log₇(100) ≈ 2.4 次，实际由于分布不均匀需要更多次，但 10 次绑绑有余。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 随机猜测 + 过滤 | 随机选词猜，按反馈过滤 | O(n²) per guess | O(n) | 能说出即可 |
| Minimax 最优猜测 | 选使最大分组最小的词 | O(n²) per guess | O(n) | ⭐ 必须写出 |
| 最大化零匹配排除 | 选与最多词匹配 0 的词反向思考 | O(n²) per guess | O(n) | 加分项 |

## 关键提示

1. **匹配函数是核心工具**：先写一个 `match(a, b)` 函数，返回两个长度 6 的字符串有几个位置完全相同。所有策略都建立在这个函数之上。

2. **反馈信息 = 过滤条件**：猜了 `g` 得到 `m`，则新的候选集 = `{w ∈ candidates : match(w, g) == m}`。这一步是确定性的，不管用什么策略都要做。

3. **为什么随机猜有风险？** 考虑极端情况：所有候选词互相之间匹配数都是 0（比如每个位置的字母都不同）。随机猜一个得到 0，几乎无法排除任何词。

4. **Minimax 的直觉**：想象你在玩"猜数字"游戏，你总想选一个能把范围切成最均匀两半的数。这里类似，但"切法"不是二分，而是按匹配数分成 7 组。

5. **匹配 0 的分组通常最大**：6 个位置全不同的概率远大于全相同的概率（25/26 vs 1/26 每位）。所以优化的重点是**减少匹配数为 0 的分组大小**。

## 解法详解

### 解法1: 随机猜测 + 过滤 — O(n²) / O(n)

**思考过程**: 最直觉的做法——随便选一个候选词猜，根据反馈过滤掉不可能的词。虽然不是最优选择策略，但过滤逻辑是正确的，只要运气不太差就能通过。

```cpp
/**
 * // This is the Master's API interface.
 * // You should not implement it, or speculate about its implementation
 * class Master {
 *   public:
 *     int guess(string word);
 * };
 */
class Solution {
public:
    // 计算两个长度6的字符串有几个位置完全匹配
    int match(const string& a, const string& b) {
        int cnt = 0;
        for (int i = 0; i < 6; i++)
            if (a[i] == b[i]) cnt++;
        return cnt;
    }
    
    void findSecretWord(vector<string>& words, Master& master) {
        vector<string> candidates = words;
        
        for (int t = 0; t < 10 && !candidates.empty(); t++) {
            // 随机选一个候选词（这里简单选第一个，或用 rand）
            string guess_word = candidates[rand() % candidates.size()];
            int m = master.guess(guess_word);
            
            if (m == 6) return; // 猜中了
            
            // 根据反馈过滤：只保留与 guess_word 匹配数恰好为 m 的词
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
```

**关键点**: 过滤逻辑是这道题的基础——`match(w, guess_word) == m` 才保留。这一步永远正确，因为 secret 和 guess_word 的匹配数确实是 m，所以 secret 一定在过滤后的集合中。随机策略的风险是：某些测试用例可能恰好运气不好，无法在 10 次内收敛。

### 解法2: Minimax 最优猜测 — O(n²) / O(n) ⭐ 面试首选

**从解法1优化**: 解法1随机选词，可能选到一个"信息量低"的词。我们改为：对每个候选词，预判如果选它来猜，最坏情况下剩余候选集有多大。选择最坏情况最小的词。

```
// Minimax 决策过程示例:
// 候选集: [A, B, C, D, E]
// 
// 如果选 A 来猜:
//   match=0: {C, D}     ← 2个
//   match=1: {B, E}     ← 2个
//   match=2: {}          ← 0个
//   最大分组 = 2
//
// 如果选 B 来猜:
//   match=0: {C, D, E}  ← 3个
//   match=1: {A}         ← 1个
//   最大分组 = 3
//
// 选 A！因为 max(2,2,0) < max(3,1)
//         即 2 < 3，A 的最坏情况更好
```

```cpp
class Solution {
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
            // === 选最优猜测词：Minimax ===
            string best_guess = candidates[0];
            int best_worst = candidates.size(); // 越小越好
            
            for (const string& g : candidates) {
                // 统计：如果选 g 来猜，各匹配数的分组大小
                int groups[7] = {};
                for (const string& w : candidates) {
                    groups[match(g, w)]++;
                }
                // 最坏情况 = 最大分组（排除 match==6，即猜中的情况）
                int worst = 0;
                for (int i = 0; i < 7; i++) {
                    worst = max(worst, groups[i]);
                }
                // 选使最坏情况最小的词
                if (worst < best_worst) {
                    best_worst = worst;
                    best_guess = g;
                }
            }
            
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
```

**关键点**: 
- Minimax 不保证每次都选到全局最优（那需要博弈树搜索），但作为贪心策略已经足够好。
- `groups[match(g, w)]++` 这行是核心——它在统计"如果猜 g，各种反馈值对应多少候选词"。
- 注意 `groups[6]` 代表和 g 完全匹配的词数（通常只有 g 自己），猜中了就结束，所以它也可以纳入 worst 计算中（不影响正确性）。

### 解法3: 最小化零匹配分组 — O(n²) / O(n)

**观察**: 在实践中，匹配数为 0 的分组几乎总是最大的（两个随机 6 字母串完全不匹配的概率很高）。所以一个更简单的启发式：**选与其他候选词匹配 0 次数最少的词**。这等价于"让最可能出现的最坏情况（返回 0）尽可能小"。

```cpp
class Solution {
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
            string best_guess = candidates[0];
            int min_zero = candidates.size();
            
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
```

**关键点**: 这是 Minimax 的简化版。实际效果非常接近完整 Minimax，因为匹配 0 的分组确实几乎总是瓶颈。代码更简洁，面试时如果时间紧可以先写这个。

## 解法对比

| | 随机猜测 | Minimax | 最小化零匹配 |
|---|---|---|---|
| 策略 | 随机选 | 选最坏情况最小的 | 选零匹配最少的 |
| 最坏情况 | 不保证收敛 | 理论最优贪心 | 接近最优 |
| 代码复杂度 | 最简单 | 中等 | 简单 |
| 面试推荐 | 作为思路起点 | ⭐ 首选 | 简化替代 |

三种解法的**过滤逻辑完全相同**，区别只在"选哪个词来猜"。面试时，先说随机思路，再解释为什么要优化选择策略，最后写 Minimax 或零匹配优化。

## 易错点

1. **✗ 过滤时忘记排除猜测词本身**
   ```cpp
   // 错误：没有排除 guess_word 自己（实际上不需要特别排除，
   // 因为 match(guess_word, guess_word) == 6 != m (当 m != 6 时) 自然被排除）
   // 但如果 m == 6 说明猜中了，此时应该直接 return
   ```
   ✓ 正确做法：先检查 `if (m == 6) return;`，然后过滤自然会排除猜测词。

2. **✗ match 函数只比较值不比较位置**
   ```cpp
   // 错误：统计字符出现次数的交集（这是 Wordle 的黄色匹配）
   int match(string a, string b) {
       sort(a.begin(), a.end()); // 错！题目要求位置也匹配
       ...
   }
   ```
   ✓ 正确做法：逐位比较 `a[i] == b[i]`。

3. **✗ Minimax 中遗漏匹配数 0 的情况**
   ```cpp
   // 错误：groups 数组只开了 [1..6]，漏掉 0
   int groups[6] = {}; // 应该是 groups[7]，索引 0~6
   ```

4. **✗ 候选集为空时继续猜**
   ```cpp
   // 如果所有候选都被排除（不应该发生，但防御性编程），继续猜会数组越界
   ```
   ✓ 循环条件加 `!candidates.empty()`。

## 面试追问

**Q1: 为什么不能暴力尝试所有词？**
→ 因为 allowedGuesses 最小是 10，而 words 最多 100 个。你只有 10 次机会猜中，必须利用每次 guess 的反馈来缩小范围。暴力需要最多 100 次。

**Q2: 随机策略平均需要几次？最坏情况呢？**
→ 平均情况还不错（因为每次过滤通常能排除不少词），但最坏情况可能需要很多次。比如所有词两两之间匹配数都是 0，那返回 0 的反馈毫无信息量，候选集几乎不缩小。这就是为什么需要 Minimax。

**Q3: Minimax 是全局最优吗？有没有更好的策略？**
→ Minimax 是贪心的——它只看当前一步的最坏情况，不考虑后续猜测。全局最优需要构建完整博弈树（类似 Mastermind 的求解），时间复杂度指数级。但对于这道题的规模（n≤100, 长度 6），Minimax 贪心已经绑绑够用，且是面试官期望的解法。

**Q4: 如果单词长度不是 6 而是很大怎么办？**
→ match 函数本身是 O(L) 的，总体时间变为 O(n² × L)。选词策略不变。如果 n 也很大，可以考虑用位运算加速 match，或者对词进行聚类预处理。

**Q5: 能否利用字母频率进一步优化选词？**
→ 可以。统计每个位置上每个字母在候选词中的出现频率，选择每个位置上字母频率都较高的词来猜。这样的词"信息量"更大，因为它更可能和其他词产生非零匹配。这是另一种启发式，实践中也很有效。

## 相关题型

- **299. 公牛和母牛 (Bulls and Cows)** — 本题的 match 函数就是计算"公牛数"（位置和值都匹配）。299 题还需要计算"母牛数"（值匹配但位置不同），可以复用逐位比较的框架。
- **Mastermind 游戏** — 经典博弈论问题，本题是 Mastermind 的简化版（只有"公牛"反馈，没有"母牛"反馈），且字母表是 26 而不是 6 种颜色。Minimax 策略在 Mastermind 中也是标准解法。
- **375. 猜数字大小 II** — 也是 Minimax 思想的应用：最小化最坏情况下的代价。区别在于 375 是数值上的二分结构，而 843 是字符串匹配。