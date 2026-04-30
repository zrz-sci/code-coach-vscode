# 76. 最小覆盖子串

## 核心思路

本质上是在字符串 `s` 中找一个**最短的连续子串**，使得这个子串"覆盖"了 `t` 中所有字符（含重复）。这是经典的**滑动窗口**问题：右指针扩展窗口直到满足条件，左指针收缩窗口寻找最短解。

## 思维链

1. **读完题第一反应** → 枚举 `s` 的所有子串，检查每个子串是否包含 `t` 的所有字符。子串数量 O(m²)，每次检查 O(m+n)，暴力解 O(m²·(m+n))，太慢。

2. **暴力解瓶颈在哪？** → 每个子串都从头开始统计字符频率，大量重复计算。比如子串 `s[0..5]` 和 `s[0..6]` 只差一个字符，却要全部重新统计。

3. **怎么突破瓶颈？** → 用**滑动窗口**！窗口右边界扩一个字符只需 O(1) 更新频率，左边界缩一个字符也只需 O(1)。这样整体只需 O(m)。

4. **窗口何时扩展、何时收缩？** → 窗口未覆盖 `t` 时，右指针扩展；窗口已覆盖 `t` 时，左指针收缩（试图找更短的）。

5. **怎么高效判断"覆盖"？** → 维护一个计数器 `formed`：当窗口中某字符的个数 **恰好达到** `t` 中该字符的需求量时，`formed++`。当 `formed == 所需的不同字符数` 时，窗口满足条件。

6. **进一步优化** → 如果 `s` 很长但 `t` 中字符很少，大量字符是"无关字符"。可以预处理 `s`，只保留 `t` 中出现过的字符及其索引，形成 `filtered_s`，在这个更小的集合上滑动。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有子串，逐个检查 | O(m²·n) | O(m+n) | 能说出即可 |
| 滑动窗口 | 双指针 + 哈希表计数 | O(m+n) | O(m+n) | ⭐ 必须写出 |
| 滑动窗口(优化) | 过滤无关字符后滑动 | O(m+n) | O(m+n) | 加分项 |

## 关键提示

1. **"覆盖"的判断不能每次遍历哈希表** — 你需要一个额外变量跟踪"已满足的字符种类数"，这样判断覆盖只需 O(1)。

2. **窗口收缩时机** — 不是找到第一个覆盖窗口就停，要**持续收缩左边界**直到不满足，每次满足时都更新最小值。

3. **字符频率用数组比 unordered_map 快** — 英文字母只有 128 个 ASCII 值，用 `int[128]` 比哈希表常数更小。

4. **注意 t 中的重复字符** — `t = "AAB"` 要求窗口中至少有 2 个 A 和 1 个 B，不是"出现过就行"。

5. **滑动窗口的过程可视化**：
```
s = "ADOBECODEBANC", t = "ABC"

步骤1: 右扩展 →
  A D O B E C O D E B A N C
  l           r              窗口 "ADOBEC" 覆盖ABC ✓, 长度6
  
步骤2: 左收缩 →
  A D O B E C O D E B A N C
    l         r              窗口 "DOBEC" 不覆盖(缺A) ✗, 恢复右扩展

步骤3: 继续右扩展 →
  A D O B E C O D E B A N C
    l                 r      窗口 "DOBECODEBA" 覆盖 ✓, 长度10

步骤4: 左收缩 →
  ...不断收缩直到不满足...

最终找到: "BANC" (长度4)
```

## 解法详解

### 解法1: 暴力枚举 — O(m²·n) / O(m+n)

**思考过程**: 最直觉的做法——枚举 `s` 的每一个起点 `i`，对每个起点从左向右扩展终点 `j`，维护窗口内字符频率，直到覆盖 `t`。记录最短的覆盖窗口。

```cpp
class Solution {
public:
    string minWindow(string s, string t) {
        int m = s.size(), n = t.size();
        if (m < n) return "";
        
        // 统计 t 中每个字符的需求量
        unordered_map<char, int> need;
        for (char c : t) need[c]++;
        
        int minLen = INT_MAX, minStart = 0;
        
        for (int i = 0; i < m; i++) {
            unordered_map<char, int> window;
            int formed = 0;
            int required = need.size(); // t 中不同字符的种类数
            
            for (int j = i; j < m; j++) {
                char c = s[j];
                window[c]++;
                // 某字符刚好达到需求量时，formed+1
                if (need.count(c) && window[c] == need[c]) {
                    formed++;
                }
                // 窗口覆盖了 t 的所有字符
                if (formed == required) {
                    int len = j - i + 1;
                    if (len < minLen) {
                        minLen = len;
                        minStart = i;
                    }
                    break; // 对于当前 i，找到最短的 j 了
                }
            }
        }
        
        return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
    }
};
```

**关键点**: 
- 对每个起点 `i`，一旦找到覆盖窗口就 `break`（因为继续扩展只会更长）。
- 时间复杂度仍然是 O(m²)（最坏情况每个起点都要扫到末尾），无法通过大数据。

---

### 解法2: 滑动窗口 — O(m+n) / O(m+n) ⭐ 面试首选

**从解法1优化**: 解法1对每个起点 `i` 都重新统计窗口，浪费了大量信息。关键观察：**当右指针找到一个覆盖窗口后，左指针右移缩小窗口，缩到不满足后右指针继续扩**。左右指针各自最多走 m 步，总共 O(m)。

```
// 滑动窗口核心逻辑:
//
// 1. 右指针 right 不断右移，每次将 s[right] 加入窗口
// 2. 当窗口满足覆盖条件时:
//    a. 更新最小窗口
//    b. 左指针 left 右移，尝试缩小窗口
//    c. 重复直到窗口不再满足条件
// 3. 右指针继续右移...
//
// s = "ADOBECODEBANC", t = "ABC"
//
// right=0: [A]DOBECODEBANC         formed=1 (A满足)
// right=5: [ADOBEC]ODEBANC         formed=3 ✓ → 收缩
//   left=1: A[DOBEC]ODEBANC        formed=2 ✗ → 停止收缩
// right=10: ADOB[ECODEBA]NC        formed=3 ✓ → 收缩
//   left=5: ADOBE[CODEBA]NC        formed=3 ✓ → 继续收缩
//   left=6: ADOBEC[ODEBA]NC        formed=3 ✓ → 继续
//   ...
//   left=9: ADOBECODE[BA]NC        formed=2 ✗ → 停止
// right=12: ADOBECODE[BANC]        formed=3 ✓ → 收缩
//   left=10: ADOBECODEB[ANC]       formed=3 ✓ 长度3... 不对
//   等等，让我仔细走一遍...
//   最终 "BANC" 长度4 是最短的
```

```cpp
class Solution {
public:
    string minWindow(string s, string t) {
        int m = s.size(), n = t.size();
        if (m < n) return "";
        
        // need[c]: t 中字符 c 的需求量
        // window[c]: 当前窗口中字符 c 的出现次数
        int need[128] = {}, window[128] = {};
        
        // 统计 t 的字符需求
        int required = 0; // t 中不同字符的种类数
        for (char c : t) {
            if (need[c] == 0) required++; // 新字符
            need[c]++;
        }
        
        int formed = 0;  // 当前窗口中已满足需求的字符种类数
        int minLen = INT_MAX, minStart = 0;
        
        // left 和 right 就是窗口的左右边界
        for (int left = 0, right = 0; right < m; right++) {
            // 1. 右指针扩展：将 s[right] 加入窗口
            char c = s[right];
            window[c]++;
            
            // 如果 c 是 t 中的字符，且恰好达到需求量 → formed++
            // 为什么是 == 而不是 >=？因为只在"恰好达到"时计数一次
            if (need[c] > 0 && window[c] == need[c]) {
                formed++;
            }
            
            // 2. 左指针收缩：当窗口已覆盖 t 时，尝试缩小
            while (formed == required) {
                // 更新最小窗口
                int len = right - left + 1;
                if (len < minLen) {
                    minLen = len;
                    minStart = left;
                }
                
                // 将 s[left] 移出窗口
                char d = s[left];
                window[d]--;
                // 如果 d 是 t 中的字符，且移出后不再满足需求 → formed--
                if (need[d] > 0 && window[d] < need[d]) {
                    formed--;
                }
                left++;
            }
        }
        
        return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
    }
};
```

**关键点**:
- `formed` 变量是精髓——避免每次都遍历哈希表判断是否覆盖。
- `window[c] == need[c]` 的判断只在"恰好达到"时触发，多出来的不重复计数。
- 左右指针各只走一遍 `s`，所以是 O(m)。加上建表 O(n)，总共 O(m+n)。

---

### 解法3: 滑动窗口 + 过滤无关字符 — O(m+n) / O(m+n)

**从解法2优化**: 当 `s` 非常长但 `t` 很短时，`s` 中大量字符和 `t` 无关。我们可以预处理一个 `filtered` 列表，只保留 `s` 中出现在 `t` 里的字符及其原始索引，然后在 `filtered` 上滑动窗口。

**什么时候有优势？** 当 `|s| >> |t|` 时，`filtered` 的长度远小于 `s`，窗口操作的次数显著减少。

```cpp
class Solution {
public:
    string minWindow(string s, string t) {
        int m = s.size(), n = t.size();
        if (m < n) return "";
        
        int need[128] = {};
        int required = 0;
        for (char c : t) {
            if (need[c] == 0) required++;
            need[c]++;
        }
        
        // 只保留 s 中出现在 t 里的字符 (字符, 原始索引)
        // 例如 s = "ADOBECODEBANC", t = "ABC"
        // filtered = [(A,0),(B,3),(C,5),(B,9),(A,10),(C,12)]
        vector<pair<int, char>> filtered;
        for (int i = 0; i < m; i++) {
            if (need[s[i]] > 0) {
                filtered.push_back({i, s[i]});
            }
        }
        
        int window[128] = {};
        int formed = 0;
        int minLen = INT_MAX, minStart = 0;
        
        // 在 filtered 上滑动窗口
        for (int left = 0, right = 0; right < (int)filtered.size(); right++) {
            char c = filtered[right].second;
            window[c]++;
            if (window[c] == need[c]) formed++;
            
            while (formed == required) {
                // 窗口在原始 s 上的范围
                int start = filtered[left].first;
                int end = filtered[right].first;
                int len = end - start + 1;
                if (len < minLen) {
                    minLen = len;
                    minStart = start;
                }
                
                char d = filtered[left].second;
                window[d]--;
                if (window[d] < need[d]) formed--;
                left++;
            }
        }
        
        return minLen == INT_MAX ? "" : s.substr(minStart, minLen);
    }
};
```

**关键点**:
- 渐进复杂度相同，但实际运行中当 `s` 很长且 `t` 的字符集很小时，过滤后的列表短很多。
- 窗口的长度要用**原始索引**计算（`filtered[right].first - filtered[left].first + 1`），不是 filtered 的下标差。

---

## 解法对比

| | 暴力枚举 | 滑动窗口 | 滑动窗口(过滤) |
|---|---|---|---|
| 时间 | O(m²·n) | O(m+n) | O(m+n) |
| 空间 | O(m+n) | O(1)* | O(m) |
| 适用 | 理解题意 | **面试首选** | s很长t很短 |
| 代码复杂度 | 简单 | 中等 | 稍复杂 |

*用 `int[128]` 数组是 O(1) 空间（常数大小）

**关键区别**：暴力解对每个起点独立扫描；滑动窗口左右指针各走一遍，信息不丢弃。过滤版在此基础上跳过无关字符。

## 易错点

1. **✗ 用 `window[c] >= need[c]` 来增加 formed**
   ```cpp
   // 错误：每次 window[c] >= need[c] 都 formed++
   if (need[c] > 0 && window[c] >= need[c]) formed++;
   ```
   **→ ✓ 只在 `window[c] == need[c]` 时加**
   ```cpp
   // 正确：只在恰好满足时计数一次
   if (need[c] > 0 && window[c] == need[c]) formed++;
   ```
   **原因**：`>=` 会导致同一字符被多次计入 formed，formed 的值会超过 required。

2. **✗ 收缩时忘记在 `window[d]--` 之后判断**
   ```cpp
   // 错误顺序：先判断再减
   if (window[d] == need[d]) formed--;  // 判断时还没减
   window[d]--;
   ```
   **→ ✓ 先减再用 `<` 判断**
   ```cpp
   window[d]--;
   if (need[d] > 0 && window[d] < need[d]) formed--;
   ```
   **原因**：要判断的是"减完之后是否不满足"，所以先减后判断。

3. **✗ 忘记处理 `s` 比 `t` 短的情况** → 直接返回 `""`。

4. **✗ `required` 算成 `t.size()` 而不是 `t` 中不同字符的种类数**
   ```cpp
   // 错误：t = "AAB" 时 required = 3
   int required = t.size();
   ```
   **→ ✓ required 是 t 中不同字符的数量**
   ```cpp
   // 正确：t = "AAB" 时 required = 2 (A和B两种)
   int required = need.size(); // 或用计数器
   ```

## 面试追问

**Q1: 暴力解的复杂度是多少？瓶颈在哪？**
→ O(m²·n)。瓶颈在于每个子串起点都独立统计，大量重复。滑动窗口通过增量更新（加一个字符 / 减一个字符都是 O(1)）消除重复计算。

**Q2: 怎么在 O(1) 时间判断窗口是否覆盖了 t？**
→ 维护 `formed` 计数器，记录"当前窗口中已满足需求量的字符种类数"。当 `formed == required` 时窗口覆盖 t。关键是只在 `window[c] == need[c]` 时增加 formed（恰好达标），避免重复计数。

**Q3: 如果 s 非常长（10^8），t 很短（3个字符），怎么优化？**
→ 用过滤法：预处理 `s`，只保留 `t` 中出现的字符和它们的索引，在这个压缩列表上做滑动窗口。这样窗口操作的次数从 O(|s|) 降到 O(|s 中与 t 相关的字符数|)。

**Q4: 如果需要返回所有最短覆盖子串（可能有多个同样长度的），怎么改？**
→ 维护一个 `vector<string>` 存结果。当 `len < minLen` 时清空结果重新加入；当 `len == minLen` 时追加。

## 相关题型

- **209. 长度最小的子数组** — 同样的滑动窗口框架，但条件从"覆盖字符"变成"和 ≥ target"，收缩逻辑更简单
- **3. 无重复字符的最长子串** — 滑动窗口求**最长**而非最短，窗口条件从"满足时收缩"变成"不满足时收缩"
- **438. 找到字符串中所有字母异位词** — 固定窗口大小的覆盖问题，可视为本题的特例（窗口大小 = t.size()）
- **567. 字符串的排列** — 和 438 几乎相同，判断 s2 是否包含 s1 的排列，也是固定窗口滑动