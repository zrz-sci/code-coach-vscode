# 424. 替换后的最长重复字符

## 核心思路

本题本质上是：**找一个最长的窗口（子串），使得窗口内"不是出现次数最多的字符"的个数 ≤ k**。换句话说，窗口长度 - 窗口内最大频次 ≤ k 时，这个窗口就是合法的。

## 思维链

1. **读完题第一反应**：枚举所有子串，检查每个子串最多替换 k 次后能否变成全部相同字符。对于每个子串，统计字符频次，用 `子串长度 - 最大频次 ≤ k` 来判断。暴力枚举所有子串 O(n²)，每个子串统计频次 O(n)，总共 O(n³)。

2. **暴力解的瓶颈**：O(n³) 太慢（n 可达 10⁵）。瓶颈在于：每个子串都重新统计频次，且枚举了所有子串。

3. **优化观察 — 滑动窗口**：这是一个"最长合法子串"问题。合法条件是 `窗口长度 - 窗口内最大字符频次 ≤ k`。当右指针右移加入新字符，窗口可能合法也可能不合法；当不合法时，左指针右移缩小窗口。这正是**滑动窗口**的经典场景。

4. **窗口内维护什么**：用一个长度为 26 的频次数组，记录窗口内每个字符的出现次数。关键量是 `maxFreq`（窗口内出现次数最多的字符的频次）。

5. **maxFreq 的巧妙处理**：右指针扩展时更新 `maxFreq`。左指针收缩时，`maxFreq` 理论上可能减小，但**我们不需要精确维护它的减小**！因为答案只会在 `maxFreq` 增大时更新。`maxFreq` 只增不减，窗口大小也只增不减（或不变），这是本题最精妙的地方。

6. **最终**：一次遍历 O(n)，频次数组 O(26)，完美。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有子串，逐个检查 | O(n²) | O(26) | 能说出即可 |
| 滑动窗口（标准） | 不合法时收缩左边界 | O(n) | O(26) | ⭐ 必须写出 |
| 滑动窗口（maxFreq 不回退） | maxFreq 只增不减，窗口只增不缩 | O(n) | O(26) | 加分项 |

## 关键提示

1. **怎么判断一个子串能否通过 ≤k 次替换变成全相同？** 统计子串中出现最多的字符频次 `maxFreq`，需要替换的字符数 = `子串长度 - maxFreq`，只要这个值 ≤ k 就行。

2. **什么时候能用滑动窗口？** 当右指针右移使条件"变差"（需要替换更多），左指针右移使条件"变好"（需要替换更少），存在单调性时就能用。

3. **maxFreq 需要在左指针右移时精确减小吗？** 想一想：答案只可能在 maxFreq 增大时变大。如果 maxFreq 没有增大，窗口不会比之前的答案更大。

4. 滑动窗口的变化过程（以 `s = "AABABBA", k = 1` 为例）：
```
索引:  0 1 2 3 4 5 6
字符:  A A B A B B A

窗口扩展与收缩:
right=0: [A]ABABBA         freq={A:1}       maxFreq=1  len-maxFreq=0 ≤ 1 ✓
right=1: [AA]BABBA         freq={A:2}       maxFreq=2  len-maxFreq=0 ≤ 1 ✓
right=2: [AAB]ABBA         freq={A:2,B:1}   maxFreq=2  len-maxFreq=1 ≤ 1 ✓
right=3: [AABA]BBA         freq={A:3,B:1}   maxFreq=3  len-maxFreq=1 ≤ 1 ✓  ← 窗口大小4
right=4: [AABAB]BA         freq={A:3,B:2}   maxFreq=3  len-maxFreq=2 > 1 ✗
         → 左指针右移: A[ABAB]BA  freq={A:2,B:2}  maxFreq 仍为3?  len-maxFreq=1 ≤ 1 ✓
right=5: A[ABABB]A         freq={A:2,B:3}   maxFreq=3  len-maxFreq=2 > 1 ✗
         → 左指针右移: AA[BABB]A  freq={A:1,B:3}  len-maxFreq=1 ≤ 1 ✓
right=6: AA[BABBA]         freq={A:2,B:3}   maxFreq=3  len-maxFreq=2 > 1 ✗
         → 左指针右移: AAB[ABBA]  freq={A:2,B:2}  len-maxFreq=2 > 1 ✗  (maxFreq=3不回退)
         → 再次右移? 不!用不回退版本窗口保持大小4
答案: 4
```

5. **26 个字母的约束很重要**：频次数组大小固定为 26，遍历它找 maxFreq 的代价是 O(1)（常数）。

## 解法详解

### 解法1: 暴力枚举 — O(n²) / O(26)

**思考过程**：最直接的想法——枚举所有可能的子串，对每个子串检查"最少需要替换多少个字符才能全部相同"。如果需要替换的个数 ≤ k，就是合法子串，更新答案。

为了避免 O(n³)，我们可以固定左端点 `i`，让右端点 `j` 从 `i` 向右扩展，**增量维护**频次数组，这样每个子串的检查是 O(1)。

```cpp
class Solution {
public:
    int characterReplacement(string s, int k) {
        int n = s.size(), ans = 0;
        for (int i = 0; i < n; i++) {
            int freq[26] = {};
            int maxFreq = 0;
            for (int j = i; j < n; j++) {
                freq[s[j] - 'A']++;
                maxFreq = max(maxFreq, freq[s[j] - 'A']);
                int windowLen = j - i + 1;
                // 需要替换的字符数 = 窗口长度 - 出现最多的字符次数
                if (windowLen - maxFreq <= k) {
                    ans = max(ans, windowLen);
                }
            }
        }
        return ans;
    }
};
```

**关键点**：增量维护频次，每次右端点右移只需更新一个字符的频次。n = 10⁵ 时 O(n²) = 10¹⁰，会超时。

---

### 解法2: 滑动窗口（标准写法） — O(n) / O(26) ⭐ 面试首选

**从解法1优化**：解法1 的瓶颈是对每个左端点都重新扫描右端点。观察到：当窗口不合法（`窗口长度 - maxFreq > k`）时，继续扩大右端点只会更不合法，所以应该收缩左端点。这就是滑动窗口。

```cpp
class Solution {
public:
    int characterReplacement(string s, int k) {
        int n = s.size(), ans = 0;
        int freq[26] = {};
        int maxFreq = 0;  // 窗口内出现次数最多的字符的频次
        int left = 0;
        
        for (int right = 0; right < n; right++) {
            // 扩展窗口：加入 s[right]
            freq[s[right] - 'A']++;
            maxFreq = max(maxFreq, freq[s[right] - 'A']);
            
            // 窗口不合法：需要替换的字符数 > k
            // 收缩左边界直到合法
            while (right - left + 1 - maxFreq > k) {
                freq[s[left] - 'A']--;
                left++;
                // 注意：这里没有更新 maxFreq 的减小
                // 如果要精确维护，需要遍历 freq 数组找新的最大值
                // 但由于答案只在 maxFreq 增大时更新，不精确也不影响正确性
            }
            
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};
```

**等等，maxFreq 不精确维护真的对吗？** 这里需要仔细分析：
- 当 `maxFreq` 不回退时，`while` 条件可能在 `maxFreq` 实际已经减小时仍判断为不合法，导致多收缩了一步。
- 但这只会让窗口"偶尔小一点"，不会让窗口"错误地大"。
- 答案的更新依赖 `maxFreq` 真的增大了，所以最终答案是正确的。

如果你觉得不放心，可以在 while 里精确更新 maxFreq（遍历26个字母），仍然是 O(n)：

```cpp
class Solution {
public:
    int characterReplacement(string s, int k) {
        int n = s.size(), ans = 0;
        int freq[26] = {};
        int left = 0;
        
        for (int right = 0; right < n; right++) {
            freq[s[right] - 'A']++;
            
            // 精确计算窗口内最大频次
            int maxFreq = *max_element(freq, freq + 26);
            
            while (right - left + 1 - maxFreq > k) {
                freq[s[left] - 'A']--;
                left++;
                maxFreq = *max_element(freq, freq + 26);  // O(26) = O(1)
            }
            
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};
```

**关键点**：遍历 26 个字母找 max 是 O(26) = O(1)，不影响整体复杂度。

---

### 解法3: 滑动窗口（maxFreq 不回退，窗口只增不缩） — O(n) / O(26)

**进一步优化思路**：核心洞察——**我们要找的是最长的合法窗口。一旦窗口达到某个长度 L，之后我们只关心能否找到更长的窗口。所以窗口大小永远不需要缩小，只需要"平移"。**

具体来说：
- `maxFreq` 只在变大时更新，永远不回退
- 当窗口不合法时，左右指针**同时右移一步**（窗口大小不变），而不是 while 循环收缩
- 这样窗口大小单调不减，最终窗口大小就是答案

```cpp
class Solution {
public:
    int characterReplacement(string s, int k) {
        int n = s.size();
        int freq[26] = {};
        int maxFreq = 0;
        int left = 0;
        
        for (int right = 0; right < n; right++) {
            freq[s[right] - 'A']++;
            // maxFreq 只增不减
            maxFreq = max(maxFreq, freq[s[right] - 'A']);
            
            // 如果窗口不合法，左指针右移一步（窗口平移，大小不变）
            if (right - left + 1 - maxFreq > k) {
                freq[s[left] - 'A']--;
                left++;
                // 注意：不更新 maxFreq 的减小
            }
        }
        
        // 最终窗口大小就是答案（因为窗口只增不减）
        return n - left;
    }
};
```

**为什么正确？**
- 假设当前最优窗口长度为 L，那么 `maxFreq ≥ L - k`。
- 之后如果 maxFreq 没有增大，窗口长度不可能超过 L（因为需要 `maxFreq > L - k` 才能让 `L+1 - maxFreq ≤ k`）。
- 所以当 maxFreq 不增大时，窗口保持大小 L 平移即可，不会错过更优解。
- 当 maxFreq 增大时，窗口有可能变大，此时 `if` 条件不成立，右指针右移而左指针不动，窗口增大。

## 解法对比

| | 解法1: 暴力 | 解法2: 滑动窗口(标准) | 解法3: 窗口不回缩 |
|---|---|---|---|
| 时间 | O(n²) | O(n) | O(n) |
| 空间 | O(26) | O(26) | O(26) |
| maxFreq | 每个子串独立 | 可精确也可不精确 | 只增不减 |
| 窗口行为 | 无窗口 | 可增可缩 | 只增或平移 |
| 理解难度 | 低 | 中 | 高（需理解不回退的正确性） |
| 面试推荐 | 说思路即可 | ⭐ 首选 | 被追问时说 |

## 易错点

1. **maxFreq 更新时机错误**
   - ✗ 在 `while` 循环收缩后才更新 `maxFreq = max(maxFreq, freq[s[right]-'A'])`
   - ✓ 在右指针扩展后**立即**更新 `maxFreq`，因为新字符可能创造新的最大频次

2. **窗口长度算错**
   - ✗ `right - left`（少了1）
   - ✓ `right - left + 1`（闭区间 [left, right] 的长度）

3. **解法3 中用 while 代替 if**
   - ✗ `while (right - left + 1 - maxFreq > k) { ... left++; }` — 在解法3中不该用 while
   - ✓ `if (right - left + 1 - maxFreq > k) { ... left++; }` — 每次只移一步，保证窗口不缩小
   - 解法2 用 while 是正确的（标准滑动窗口），解法3 用 if（窗口不回缩版本）

4. **频次数组没初始化为 0**
   - ✗ `int freq[26];`（C++ 中局部数组不自动初始化）
   - ✓ `int freq[26] = {};` 或 `int freq[26] = {0};`

## 面试追问

**Q1: 暴力解的时间复杂度是多少？能不能用在面试中？**
> O(n²)（增量维护频次后）。n = 10⁵ 时约 10¹⁰ 运算量，会超时。面试中可以作为起点说清思路，但必须优化到 O(n)。

**Q2: 滑动窗口中 maxFreq 不精确维护为什么不影响答案？**
> 答案只会在 maxFreq 增大时变大。如果 maxFreq 没增大，当前窗口不可能比历史最优更长。maxFreq 偏大只会让窗口"看起来合法区间更大"，但由于窗口大小不会超过之前的最优，不影响最终答案。

**Q3: 如果字符集不是26个大写字母，而是任意 Unicode 字符怎么办？**
> 频次数组改为 `unordered_map<char, int>`。如果要精确维护 maxFreq，遍历 map 的代价变为 O(字符集大小)。解法3 的优势更明显——不需要精确维护 maxFreq，避免了这个开销。

**Q4: 这道题和"最多包含K个不同字符的最长子串"有什么区别？**
> 那道题（LeetCode 340）关心的是不同字符的**种类数** ≤ k；本题关心的是需要**替换的字符个数** ≤ k。窗口合法条件不同，但都是滑动窗口框架。

## 相关题型

- **3. 无重复字符的最长子串** — 同样是滑动窗口求最长子串，区别：合法条件是"无重复"而不是"替换次数 ≤ k"。复用同样的左右指针框架。
- **340. 至多包含 K 个不同字符的最长子串** — 滑动窗口 + 哈希表计数，合法条件是"不同字符种类 ≤ k"。复用本题的滑动窗口模板，只需修改 while 条件。
- **1004. 最大连续1的个数 III** — 本题的特例！把字符集缩小到 {0, 1}，"出现最多的字符"就是 1（或 0），`窗口长度 - count(1) ≤ k`。完全复用本题的解法。
- **76. 最小覆盖子串** — 滑动窗口求**最短**合法子串，框架类似但方向相反：先扩展找到合法，再收缩求最短。