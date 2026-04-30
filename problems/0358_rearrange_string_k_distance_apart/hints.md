# 358. Rearrange String k Distance Apart - K距离间隔重排字符串

## 核心思路
**贪心 + 最大堆 + 冷却队列**：每次贪心选取剩余频率最高的字符放置。放置后该字符需要"冷却" k 步才能再次使用 → 用队列维护冷却中的字符。如果堆为空但队列非空且还有字符要放，说明无法完成重排。

## 思维链
1. 统计每个字符的频率 → `freq[26]`
2. 将所有字符按频率放入最大堆 → 每次取频率最高的字符
3. 取出字符后频率 -1，放入冷却队列（长度为 k）
4. 当冷却队列满 k 个时，队头字符可以重新入堆
5. 如果堆空了但结果长度不够 → 返回 ""

## 解法概览表⭐

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 最大堆 + 冷却队列 | O(N log 26) | O(26) | 标准贪心解法 |
| 贪心 + 计数数组 | O(N * 26) | O(26) | 无需堆，直接找最大频率 |

## 关键提示
1. **k=0 直接返回 s**：距离 0 表示无约束
2. **贪心正确性**：优先放频率最高的字符可以最大化后续放置空间
3. **冷却队列**：用 queue 存放 (char, remainFreq)，当 queue.size() >= k 时弹出队头重新入堆
4. **不可能的判断**：如果某字符频率 > ceil(n/k)（n 为字符串长度），则无法重排

## 解法详解

### 解法一：最大堆 + 冷却队列（推荐）

**思路**：最大堆按频率排序，每步取最高频字符放置。已放置字符进入冷却队列，等待 k 步后回到堆中。

```cpp
class Solution {
public:
    string rearrangeString(string s, int k) {
        if (k <= 1) return s;
        
        unordered_map<char, int> freq;
        for (char c : s) freq[c]++;
        
        // 最大堆：(频率, 字符)
        priority_queue<pair<int,char>> pq;
        for (auto& [c, f] : freq) {
            pq.push({f, c});
        }
        
        string result;
        queue<pair<int,char>> cooldown;  // 冷却队列
        
        while (!pq.empty()) {
            auto [f, c] = pq.top(); pq.pop();
            result += c;
            cooldown.push({f - 1, c});
            
            // 冷却期满，字符可以重新使用
            if ((int)cooldown.size() >= k) {
                auto [rf, rc] = cooldown.front();
                cooldown.pop();
                if (rf > 0) pq.push({rf, rc});
            }
        }
        
        return result.size() == s.size() ? result : "";
    }
};
```

**复杂度**：时间 O(N log 26) = O(N)，空间 O(26) = O(1)

### 解法二：贪心 + 直接填充

**思路**：每轮填充 k 个不同字符（频率从高到低），直到所有字符用完。

```cpp
class Solution {
public:
    string rearrangeString(string s, int k) {
        if (k <= 1) return s;
        
        int n = s.size();
        vector<int> freq(26, 0);
        for (char c : s) freq[c - 'a']++;
        
        string result;
        
        while ((int)result.size() < n) {
            // 每轮找频率最高的字符放置，最多放k个
            // 先找出当前可用字符按频率排序
            vector<pair<int,int>> candidates; // (freq, charIdx)
            for (int i = 0; i < 26; i++) {
                if (freq[i] > 0) candidates.push_back({freq[i], i});
            }
            sort(candidates.rbegin(), candidates.rend());
            
            int fill = min(k, n - (int)result.size());
            if ((int)candidates.size() < fill) return ""; // 不够k种字符
            
            for (int i = 0; i < fill; i++) {
                result += (char)('a' + candidates[i].second);
                freq[candidates[i].second]--;
            }
        }
        return result;
    }
};
```

**复杂度**：时间 O(N * 26)，空间 O(26)

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| k=0 时不特判 | k <= 1 时直接返回 s | k=0 或 k=1 无约束 |
| 冷却队列到 k 才弹出 | `cooldown.size() >= k` 时弹出 | 等号不能漏 |
| 堆空就结束 | 还需检查 result 长度是否等于 s 长度 | 堆空但结果不完整说明失败 |
| 不判断最后一轮 fill < k | 最后一轮 fill = min(k, remaining) | 最后几个字符可能不足 k 种 |

## 面试追问

**Q1: 如何判断是否一定无法重排？**

A: 设最大频率为 maxF，字符串长度为 n。无法重排的充要条件是 `maxF > (n-1)/(k-1) + 1`，等价于 `(maxF-1) * k + countOfMaxF > n`（其中 countOfMaxF 是频率等于 maxF 的字符数）。可以在 O(N) 内预判。

**Q2: 这题和 767. Reorganize String 有什么关系？**

A: 767 是本题 k=2 的特例。767 要求相邻字符不同 = 相同字符距离至少 2。解法完全一致，只是 k 固定为 2。

**Q3: 如果要求恰好距离 k（不是至少距离 k），如何修改？**

A: 这变成了更难的约束满足问题，不再是简单贪心。需要用回溯或 constraint propagation。对于特殊情况（如 k 整除 n），可以将 n 个位置分成 n/k 组，每组 k 个位置，用匹配算法分配字符。

## 相关题型
- [767. Reorganize String](../0767_reorganize_string/) - 本题 k=2 特例
- [621. Task Scheduler](../0621_task_scheduler/) - 冷却时间调度
- [1054. Distant Barcodes](../1054_distant_barcodes/) - 相邻不同排列
