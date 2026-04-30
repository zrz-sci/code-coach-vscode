# 466. Count The Repetitions - 统计重复个数

## 核心思路
模拟遍历 s1 匹配 s2，利用鸽巢原理发现循环节。每遍历一遍 s1，记录此时 s2 匹配到的位置，若位置重复则找到了循环。

## 思维链
1. str1 = [s1, n1]，str2 = [s2, n2]，求最大 m 使 [str2, m] 是 str1 的子序列
2. 暴力：逐字符匹配 s1 重复 n1 次中能匹配多少个完整的 s2 -> n1 可达 10^6，需优化
3. 关键观察：每轮 s1 结束时，s2 的指针位置 j 只有 len(s2) 种可能 -> 必出现循环
4. 找到循环后：前缀 + 循环体 * 重复次数 + 后缀 = 总共匹配的 s2 个数
5. 最终答案 = 匹配的 s2 总数 / n2

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 循环节检测 | O(len1 * len2) | O(len2) | 标准最优 |
| 暴力模拟 | O(n1 * len1) | O(1) | 超时 |

## 关键提示
1. **核心变量**：每遍历完一个 s1，记录 (s2指针位置j, 已匹配完的s2个数)
2. **循环检测**：s2 的指针 j 只有 len(s2) 种取值，最多 len(s2)+1 轮 s1 后必出现重复
3. **计算公式**：
   - 循环前的 s1 轮数和匹配的 s2 数
   - 每个循环消耗的 s1 轮数和匹配的 s2 数
   - 剩余 s1 轮数能额外匹配的 s2 数
4. 最终 `answer = totalS2Count / n2`

## 解法详解

### 解法一：循环节检测
```cpp
class Solution {
public:
    int getMaxRepetitions(string s1, int n1, string s2, int n2) {
        if (n1 == 0) return 0;
        int len1 = s1.size(), len2 = s2.size();
        
        // s2cnt[i] = 遍历了 i+1 个 s1 后，匹配了多少个完整 s2
        // s2idx[i] = 遍历了 i+1 个 s1 后，s2 的指针位置
        vector<int> s2cnt(n1, 0), s2idx(n1, 0);
        
        int cnt = 0, j = 0; // cnt = 已匹配完的 s2 个数, j = s2 当前指针
        for (int i = 0; i < n1; i++) {
            for (int k = 0; k < len1; k++) {
                if (s1[k] == s2[j]) {
                    j++;
                    if (j == len2) {
                        j = 0;
                        cnt++;
                    }
                }
            }
            s2cnt[i] = cnt;
            s2idx[i] = j;
            
            // 检查是否出现过相同的 s2 指针位置（循环节）
            for (int prev = 0; prev < i; prev++) {
                if (s2idx[prev] == j) {
                    // 找到循环：prev+1 到 i 是一个循环
                    int prefixS1 = prev + 1;         // 前缀用了多少个 s1
                    int cycleS1 = i - prev;           // 一个循环用多少个 s1
                    int prefixS2 = s2cnt[prev];       // 前缀匹配了多少个 s2
                    int cycleS2 = cnt - s2cnt[prev];  // 一个循环匹配多少个 s2
                    
                    int remainS1 = n1 - prefixS1;
                    int cycles = remainS1 / cycleS1;
                    int tailS1 = remainS1 % cycleS1;
                    
                    int totalS2 = prefixS2 + cycles * cycleS2 
                                  + s2cnt[prev + tailS1] - s2cnt[prev];
                    return totalS2 / n2;
                }
            }
        }
        // 没找到循环，直接用最终结果
        return s2cnt[n1 - 1] / n2;
    }
};
```

## 易错点
- ✗ 暴力拼接 str1 再匹配 -> ✓ n1 可达 10^6，内存和时间都超
- ✗ 循环检测时比较 cnt 而非 j -> ✓ 循环由 s2 指针位置 j 决定，不是匹配数
- ✗ 计算尾部时用 s2cnt[tailS1] -> ✓ 应该用 `s2cnt[prev + tailS1] - s2cnt[prev]`
- ✗ 忘记最后除以 n2 -> ✓ 题目要的是 [str2, m]，str2 = [s2, n2]

## 面试追问

**Q1: 为什么一定能找到循环？**
> 鸽巢原理：s2 指针位置 j 只有 len(s2) 种值（0 到 len2-1），最多 len2+1 轮 s1 后必有两轮结束时 j 相同。

**Q2: 时间复杂度怎么分析？**
> 最多遍历 min(n1, len2+1) 个 s1（找到循环就停），每个 s1 长度 len1。所以是 O(min(n1, len2) * len1)。循环检测部分 O(len2^2) 可优化为 O(len2) 用 map。

**Q3: 如果 s2 中有 s1 不包含的字符怎么办？**
> 直接返回 0。可以预处理检查。

## 相关题型
- [392. Is Subsequence](https://leetcode.com/problems/is-subsequence/) - 基础子序列判断
- [792. Number of Matching Subsequences](https://leetcode.com/problems/number-of-matching-subsequences/) - 多模式子序列
- [418. Sentence Screen Fitting](https://leetcode.com/problems/sentence-screen-fitting/) - 类似循环节优化
