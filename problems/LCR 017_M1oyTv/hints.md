# LCR 017. 最小覆盖子串

## 核心思路

**滑动窗口**经典题。维护一个窗口 `[left, right]`，用哈希表记录窗口内各字符的计数。当窗口包含了 t 的所有字符时，尝试收缩左边界以找到最短的合法窗口。用 `formed` 计数器跟踪"有多少种字符已经满足需求"，避免每次都遍历整个哈希表检查。

> 本题同主站 **LeetCode 76. Minimum Window Substring**。

## 思维链

1. **读完题第一反应**：在 s 中找一个最短的子串，使得这个子串包含 t 中所有字符（含重复）。暴力做法：枚举所有子串 O(n^2)，每个子串检查是否包含 t 中所有字符 O(n)，总共 O(n^3)。

2. **暴力的瓶颈在哪？** 当右端点右移一步时，我们只新增了一个字符，不需要重新检查整个子串。这和"最长无重复子串"一样，是**增量维护**的思路 → 滑动窗口。

3. **滑动窗口的框架**：
   - **右指针扩张**：把 `s[right]` 加入窗口，更新窗口内的字符计数。
   - **检查合法性**：当窗口包含 t 的所有字符时，窗口合法。
   - **左指针收缩**：合法时尝试左移 left，每次检查去掉 `s[left]` 后窗口是否仍合法。
   - **更新答案**：每次合法时，记录当前窗口长度，取最小值。

4. **如何高效判断"窗口包含 t 的所有字符"？** 如果每次都遍历哈希表检查所有字符是否满足，是 O(字符集大小) 的。优化：用 `formed` 计数器 —— 当某字符在窗口中的数量**恰好达到** t 中需要的数量时 `formed++`，当 `formed == required`（t 中不同字符的种类数）时窗口合法。

5. **收缩时的更新**：左指针移动时，如果被移除的字符 `s[left]` 是 t 中需要的，且移除后该字符数量**恰好降到不足**，则 `formed--`。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有子串，检查是否覆盖 | O(n^2 * m) | O(m) | 理解题意 |
| 滑动窗口 + HashMap | 右扩左缩，formed 跟踪合法性 | O(n + m) | O(128) | 面试必写 ⭐ |
| 滑动窗口 + 数组 | 用 int[128] 替代 HashMap | O(n + m) | O(1) | 极致优化 |

> n = |s|, m = |t|

## 关键提示

1. **need 和 window 两个计数器**：
   - `need[c]`：字符 c 在 t 中出现的次数（目标需求）
   - `window[c]`：字符 c 在当前窗口中的出现次数（实际供给）
   - 当 `window[c] == need[c]` 时，字符 c 的需求刚好被满足。

2. **formed 变量的作用**：
   - `required`：t 中有多少种不同字符（比如 t = "AABC"，required = 3 种: A, B, C）
   - `formed`：当前窗口中有多少种字符已经满足需求
   - 当 `formed == required` 时，窗口覆盖了 t 的所有字符

3. **formed 的增减时机**：
   - **增加**：`window[c]++` 后，如果 `need[c] > 0 && window[c] == need[c]`，则 `formed++`
   - **减少**：`window[c]--` 前，如果 `need[c] > 0 && window[c] == need[c]`，则 `formed--`
   - 注意：只在"恰好"达到/降到临界值时变化，不是每次都变

4. **收缩的目的**：在窗口合法的前提下，尽可能缩短窗口长度，找到最小覆盖子串。

5. **窗口滑动过程示意**（s = "ADOBECODEBANC", t = "ABC"）：
   ```
   need: A=1, B=1, C=1 (required=3种)

   A D O B E C O D E B A  N  C
   0 1 2 3 4 5 6 7 8 9 10 11 12

   right=0: 加入'A', window[A]=1, formed=1 (A满足)
   ┌─────┐
   │  A  │ D O B E C O D E B A N C
   └─────┘
   formed=1/3, 不合法

   right=3: 加入'B', window[B]=1, formed=2 (B满足)
   ┌──────────────┐
   │  A  D  O  B  │ E C O D E B A N C
   └──────────────┘
   formed=2/3, 不合法

   right=5: 加入'C', window[C]=1, formed=3 (C满足) → 合法!
   ┌─────────────────────┐
   │  A  D  O  B  E  C   │ O D E B A N C
   └─────────────────────┘
   formed=3/3, 合法! 记录长度=6, 开始收缩左边界

   收缩 left=0→1: 移除'A', window[A]=0, formed=2 → 不合法了
       ┌──────────────────┐
    A  │  D  O  B  E  C   │ O D E B A N C
       └──────────────────┘

   right 继续扩张...

   right=10: 加入'A', window[A]=1, formed=3 → 合法!
              ┌─────────────────────────────────┐
    A  D  O  │  B  E  C  O  D  E  B  A         │ N C
              └─────────────────────────────────┘
   长度=8, 不如之前的6

   收缩 left=3→4: 移除'B', formed仍=3 (window[B]还有1个)
   收缩 left=4→5: 移除'E'
   收缩 left=5→6: 移除'C', window[C]=0, formed=2 → 不合法

   ... 继续扩张+收缩 ...

   最终在 right=12 时找到 "BANC" (长度4) 为最小覆盖子串
              ┌──────────────┐
              │  B  A  N  C  │
              └──────────────┘
   ```

## 解法详解

### 解法1: 滑动窗口 + 数组 — O(|s| + |t|) / O(128) ⭐ 面试首选

**数据结构选择**：字符为英文字母，ASCII 值范围有限，用 `int[128]` 数组比 `unordered_map` 快且无哈希开销。

```cpp
class Solution {
public:
    string minWindow(string s, string t) {
        // Step 1: 统计 t 中每个字符的需求量
        vector<int> need(128, 0), window(128, 0);
        int required = 0; // t 中不同字符的种类数
        for (char c : t) {
            if (need[c] == 0) required++;
            need[c]++;
        }

        // Step 2: 滑动窗口
        int formed = 0;  // 已满足需求的字符种类数
        int left = 0;
        int minLen = INT_MAX, minStart = 0;

        for (int right = 0; right < (int)s.size(); right++) {
            // 扩张: 加入 s[right]
            char c = s[right];
            window[c]++;
            // 如果 c 是 t 中需要的字符，且恰好满足需求
            if (need[c] > 0 && window[c] == need[c]) {
                formed++;
            }

            // 收缩: 当窗口合法时，尝试缩小
            while (formed == required) {
                // 更新答案
                if (right - left + 1 < minLen) {
                    minLen = right - left + 1;
                    minStart = left;
                }
                // 移除 s[left]
                char d = s[left];
                if (need[d] > 0 && window[d] == need[d]) {
                    formed--;
                }
                window[d]--;
                left++;
            }
        }

        return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
    }
};
```

### 解法2: 滑动窗口 + HashMap — O(|s| + |t|) / O(|字符集|)

**与解法1 的区别**：用 `unordered_map` 替代数组。逻辑完全一致，但代码更通用（支持任意字符集，如 Unicode）。

```cpp
class Solution {
public:
    string minWindow(string s, string t) {
        unordered_map<char, int> need, window;
        for (char c : t) need[c]++;
        int required = need.size(); // 不同字符种类数

        int formed = 0, left = 0;
        int minLen = INT_MAX, minStart = 0;

        for (int right = 0; right < (int)s.size(); right++) {
            char c = s[right];
            window[c]++;
            if (need.count(c) && window[c] == need[c]) {
                formed++;
            }

            while (formed == required) {
                if (right - left + 1 < minLen) {
                    minLen = right - left + 1;
                    minStart = left;
                }
                char d = s[left++];
                if (need.count(d) && window[d] == need[d]) {
                    formed--;
                }
                window[d]--;
            }
        }
        return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
    }
};
```

## 解法对比

| | 暴力枚举 | 滑窗 + 数组 | 滑窗 + HashMap |
|---|---|---|---|
| 时间 | O(n^2 * m) | O(n + m) | O(n + m) |
| 空间 | O(m) | O(128) = O(1) | O(字符集) |
| 常数因子 | 大 | 小 | 中 |
| 字符集限制 | 无 | ASCII | 无 |
| 推荐 | 理解 | 面试首选 | 通用 |

## 易错点

1. **formed 的增减时机搞错**：
   ```
   ✗ 每次 window[c]++ 都 formed++
   ✓ 只在 window[c] 恰好等于 need[c] 时 formed++
   ```
   如果 window[c] 从 1 增到 2，而 need[c] = 1，formed 不应该再增加。

2. **formed 减少的判断在 window-- 之前还是之后**：
   ```
   ✗ window[d]--;  if (window[d] < need[d]) formed--;  // 可能减多次
   ✓ if (window[d] == need[d]) formed--;  window[d]--;   // 恰好不满足时减一次
   ```

3. **忘记处理"找不到"的情况**：
   ```
   ✗ return s.substr(minStart, minLen);  // minLen 还是 INT_MAX 时会出错
   ✓ return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
   ```

4. **没有考虑 t 中有重复字符**：
   ```
   输入: s = "a", t = "aa"
   need[a] = 2, 但 s 中只有 1 个 'a', window[a] 永远到不了 2
   → formed 永远不等于 required → 返回 ""
   ```

5. **窗口长度计算**：
   ```
   ✗ right - left (差1)
   ✓ right - left + 1
   ```

## 面试追问

**Q1: formed 变量的设计有什么好处？为什么不直接每次遍历 need 检查？**
如果每次检查窗口合法性都遍历整个 need 表，每一步都是 O(字符集大小)，总时间变成 O(n * 字符集)。而 formed 是 O(1) 的增量维护：每次加入/移除字符只更新一个计数器。这把"检查合法性"从 O(字符集) 优化到了 O(1)。

**Q2: 为什么左指针只往右走不回头？这样不会漏掉答案吗？**
不会。假设最优答案的窗口是 `[L*, R*]`。当 right = R* 时，窗口必然合法（因为包含了最优窗口的所有字符）。此时 left 会收缩到 L* 或更右的位置。left 不需要回退，因为对于任何 right' > R*，如果存在更短的覆盖子串，其左端点一定 >= L*。

**Q3: 能不能用"计数器"代替 need 和 window 两个哈希表？**
可以。有一种简洁写法：只用一个 `count[128]` 数组初始化为 t 中字符频次，扩张时 `count[s[right]]--`，如果减完 >= 0 说明消耗了一个有效字符（总需求 -1）。收缩时 `count[s[left]]++`，如果加完 > 0 说明释放了一个有效字符（总需求 +1）。这种写法更短但更难理解。

**Q4: 如果有多个最短覆盖子串，要返回所有呢？**
在更新答案时，如果当前长度等于 minLen，把 `s.substr(left, minLen)` 加入结果数组。如果小于 minLen，清空结果数组重新收集。

## 相关题型

- **3. 无重复字符的最长子串** — 滑动窗口入门题，本题是它的"升级版"。区别：第 3 题是"最长"且判断"无重复"，本题是"最短"且判断"覆盖"。

- **438. 找到字符串中所有字母异位词** — 固定窗口大小的滑动窗口，判断窗口内字符频次是否匹配。

- **567. 字符串的排列** — 与 438 几乎相同，判断 s2 中是否有 s1 的排列。

- **209. 长度最小的子数组** — 滑动窗口求"最短"子数组，但判断条件是"和 >= target"，比本题简单。
