[file://solution.cpp](./solution.cpp)

# 916. 单词子集

## 核心思路

给定 words1 和 words2，要找 words1 中的"通用"字符串——即对 words2 中的每个字符串 b，b 都是 a 的子集（每个字母在 a 中出现的次数 >= 在 b 中出现的次数）。关键突破口：不需要逐个检查 words2 中的每个词，可以把 words2 中所有词的字符需求**合并成一个"最大需求"数组**，然后对每个 words1 中的词只需检查一次。

## 思维链

1. **读完题第一反应**：暴力做法——对 words1 中的每个词 a，遍历 words2 中的每个词 b，检查 b 是否是 a 的子集。如果所有 b 都是 a 的子集，则 a 是通用的。时间 O(|words1| * |words2| * 10)。
2. **暴力解的瓶颈**：对每个 a 都要遍历所有的 b。words2 有多达 10^4 个词，每个 a 都要检查 10^4 次，总体太慢。
3. **关键观察——合并 words2 的需求**：words2 中所有词的字符需求可以合并。比如 words2 = ["ec", "oc"]，那么字母 e 至少需要 1 个，o 至少需要 1 个，c 至少需要 1 个。合并方式：对每个字母取所有 words2 中该字母出现次数的最大值。
4. **优化后的算法**：先扫一遍 words2 构建"最大需求"数组 maxFreq[26]，然后对 words1 中的每个词统计字频，检查是否每个字母都满足 maxFreq 的需求。时间 O(|words1| * 10 + |words2| * 10) = O(n * L)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力检查 | 对每个 a 逐个检查所有 b | O(m * n * L) | O(26) | 能说出即可 |
| 合并需求 | 把 words2 合并成一个需求数组，逐个检查 words1 | O((m + n) * L) | O(26) | 面试首选 |

## 关键提示

1. **"子集"的定义是字频层面的**：字符串 b 是 a 的子集，意味着 b 中每个字母的出现次数 <= 该字母在 a 中的出现次数。这就是一个字频数组的逐位比较问题。
2. **合并 words2 用 max 而非 sum**：如果 words2 = ["ee", "eo"]，合并后 e 需要 2 个（来自 "ee"），o 需要 1 个（来自 "eo"）。注意是取 max，不是求和！因为 a 只需要同时满足所有 b，而非把所有 b 的需求叠加。
3. **为什么 max 是正确的？** 考虑字母 c。words2 中某个词需要 c 出现 3 次，另一个需要 c 出现 2 次。a 只要有 3 个 c 就同时满足两个需求。所以取 max(3, 2) = 3。
4. **字符串长度很短**：每个词最多 10 个字符，所以统计字频是常数时间操作。这使得合并和检查都很快。

```
// 合并 words2 的需求示意:
//
// words2 = ["ec", "oc", "ccc"]
//
// "ec" 的字频: e=1, c=1
// "oc" 的字频: o=1, c=1
// "ccc" 的字频: c=3
//
// 合并（对每个字母取 max）:
// maxFreq: c=3, e=1, o=1  （其他字母=0）
//
// 检查 words1 中的词:
// "amazon" → a=2,m=1,z=1,o=1,n=1 → c=0 < 3 ✗
// "leetcode" → l=1,e=3,t=1,c=1,o=1,d=1 → c=1 < 3 ✗
// "ccceo" → c=3,e=1,o=1 → c=3>=3, e=1>=1, o=1>=1 ✓
```

## 解法详解

### 解法1: 暴力检查 — O(m * n * L)

**思考过程**：最直觉——对 words1 中的每个词 a，检查 words2 中的每个词 b 是否是 a 的子集。

```cpp
class Solution {
public:
    vector<string> wordSubsets(vector<string>& words1, vector<string>& words2) {
        vector<string> result;
        
        for (auto& a : words1) {
            // 统计 a 的字频
            vector<int> freqA(26, 0);
            for (char c : a) freqA[c - 'a']++;
            
            bool universal = true;
            for (auto& b : words2) {
                // 统计 b 的字频
                vector<int> freqB(26, 0);
                for (char c : b) freqB[c - 'a']++;
                
                // 检查 b 是否是 a 的子集
                for (int i = 0; i < 26; i++) {
                    if (freqB[i] > freqA[i]) {
                        universal = false;
                        break;
                    }
                }
                if (!universal) break;
            }
            
            if (universal) result.push_back(a);
        }
        
        return result;
    }
};
```

**瓶颈**：对每个 a 都要遍历所有 b 并重复统计字频。words1 和 words2 各有 10^4 个词时，总共要做 10^8 次字频统计，太慢。

---

### 解法2: 合并需求数组 — O((m + n) * L) 面试首选

**从暴力优化**：既然 words2 中所有 b 的需求最终是"对每个字母取 max"，那可以预处理一次把 words2 合并成一个需求数组，之后对每个 a 只检查一次。

```cpp
class Solution {
public:
    vector<string> wordSubsets(vector<string>& words1, vector<string>& words2) {
        // 第一步：合并 words2 中所有词的字频需求
        // 对每个字母，取所有 words2 中该字母出现次数的 max
        vector<int> maxFreq(26, 0);
        for (auto& b : words2) {
            vector<int> freqB(26, 0);
            for (char c : b) freqB[c - 'a']++;
            for (int i = 0; i < 26; i++) {
                maxFreq[i] = max(maxFreq[i], freqB[i]);
            }
        }
        
        // 第二步：检查 words1 中每个词是否满足合并后的需求
        vector<string> result;
        for (auto& a : words1) {
            vector<int> freqA(26, 0);
            for (char c : a) freqA[c - 'a']++;
            
            bool universal = true;
            for (int i = 0; i < 26; i++) {
                if (freqA[i] < maxFreq[i]) {
                    universal = false;
                    break;
                }
            }
            
            if (universal) result.push_back(a);
        }
        
        return result;
    }
};
```

**关键点**：
- 合并 words2 只需要一次遍历，之后 maxFreq 就固定了。
- 对每个 a 的检查从遍历所有 b 变成了一次 26 位比较。
- 总时间复杂度从 O(m * n * L) 降到 O((m + n) * L)。

## 解法对比

| | 暴力检查 | 合并需求 |
|---|---|---|
| 时间 | O(m * n * L) | O((m + n) * L) |
| 空间 | O(26) | O(26) |
| 核心思想 | 逐个检查每个 b | 预处理合并 words2 的需求 |
| 面试推荐 | 开场思路 | **首选** |

**什么时候选哪个？**
- 面试先说暴力思路，指出瓶颈（重复遍历 words2），然后说"可以把 words2 的需求合并"，直接写解法2。

## 易错点

1. **合并 words2 时用 max 而非 sum**
   - 如果 words2 = ["aa", "ab"]，合并后 a 需要 2（来自 "aa"），b 需要 1（来自 "ab"）。
   - 如果错误地 sum，会得到 a 需要 3——这太严格了。

2. **忘记给每个 b 单独统计字频**
   - 每个 b 需要独立统计字频，然后和 maxFreq 做 max 合并。不能把所有 b 的字符混在一起计数。

3. **字频数组大小写问题**
   - 题目说"只包含小写字母"，所以大小为 26，用 `c - 'a'` 索引。如果题目允许大写，需要调整。

4. **结果的顺序**
   - 题目说"可以任意顺序返回"，所以直接按遍历顺序 push_back 即可。

## 面试追问

**Q1: 暴力解的时间复杂度是多少？瓶颈在哪？**
> O(m * n * L)，其中 m = |words1|, n = |words2|, L = 最大词长度(10)。瓶颈是对每个 a 都要遍历所有 b。

**Q2: 为什么合并时取 max 而不是 sum？**
> 因为 a 需要同时满足所有 b 的需求，而不是累加需求。比如 b1 需要 2 个 'a'，b2 需要 3 个 'a'，a 只需要 3 个 'a' 就同时满足两者。

**Q3: 合并后检查的时间复杂度为什么是 O(1)？**
> 检查 26 个字母的频次比较是常数时间（字母表大小固定为 26）。所以每个 a 的检查是 O(L + 26) = O(L)。

**Q4: 如果 words2 中有重复的词，影响结果吗？**
> 不影响。合并时取 max，重复的词不会改变 maxFreq 数组。

## 相关题型

- **49. Group Anagrams** — 同样需要字频统计和比较，不同的是这道题是分组而非子集检查。字频统计的基础技巧完全相同。
- **383. Ransom Note** — 检查一个字符串的字频是否被另一个字符串覆盖，是本题"子集"检查的简化版（只有一对而非多对）。
- **350. Intersection of Two Arrays II** — 两个数组的交集按频次计算，核心也是字频/元素频次的 min 操作，与本题的 max 操作互为对偶。
