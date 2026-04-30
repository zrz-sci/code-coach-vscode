# 3. 无重复字符的最长子串

## 核心思路

本质上是在一个字符串中，找到一个**最长的连续窗口**，使得窗口内没有重复字符。关键词是"连续"（子串）和"无重复"，这天然指向**滑动窗口**技巧。

## 思维链

1. **读完题第一反应**：枚举所有可能的子串，检查每个子串是否有重复字符，取最长的。这就是暴力解 — 枚举所有 (i, j) 对，O(n²) 个子串，每个子串检查重复需要 O(n)，总共 O(n³)。

2. **暴力解的瓶颈在哪？** 两层循环枚举所有子串起止点，然后第三层循环判断是否有重复。能不能减少枚举量？

3. **关键观察**：如果 `s[left..right]` 已经包含重复字符了，那 `s[left..right+1]`, `s[left..right+2]`... 全都不用看了，因为只会更长更不可能无重复。同理，如果 `s[left..right]` 无重复，就不需要收缩右端。这种**单调性**正是滑动窗口的适用条件。

4. **怎么维护窗口内"无重复"？** 需要快速判断一个字符是否已经在窗口内 → 用**哈希集合 (set)** 记录窗口内的字符，查找 O(1)。

5. **进一步优化**：当发现重复字符时，set 版本需要一个一个地从左边移除，直到重复消除。如果用**哈希表 (map)** 记录每个字符最后出现的位置，可以直接把左指针**跳到重复位置的右边**，省去逐步收缩的过程。

6. **极致优化**：字符集有限（ASCII 128 个），用数组代替哈希表，常数时间更快。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有子串，逐个检查重复 | O(n³) | O(n) | 能说出即可 |
| 滑动窗口 + HashSet | 右指针扩张，遇重复左指针逐步收缩 | O(n) | O(min(n, m)) | ⭐ 必须写出 |
| 滑动窗口 + HashMap | 记录字符最后位置，左指针直接跳转 | O(n) | O(min(n, m)) | 加分项 |
| 滑动窗口 + 数组 | 用 int[128] 替代 HashMap | O(n) | O(1) | 加分项 |

> m 为字符集大小（ASCII 为 128）

## 关键提示

1. **子串 vs 子序列**：子串必须是连续的字符，子序列可以跳着选。本题要求子串，所以用滑动窗口而非 DP。

2. **滑动窗口的核心**：右指针每次右移一步（扩张），左指针只在必要时右移（收缩），两个指针都**只往右走不回头**，所以总操作次数是 O(n) 而非 O(n²)。

3. **想一想**：当右指针遇到窗口内已存在的字符 `c` 时，左指针需要移动到哪里？移动到 `c` 上次出现位置的**右边一个位置**。

4. **特别注意**：用 HashMap 跳转时，需要取 `max(left, map[c] + 1)`，因为 `map[c]` 记录的位置可能在当前窗口的左边（已经被淘汰了），不能让左指针倒退。

5. **窗口示意**（以 "abcabcbb" 为例）：
```
a b c a b c b b
^     ^             left=0, right=3, 发现'a'重复
  ^   ^             left跳到1, 窗口"bca"
    ^   ^           left跳到2, 窗口"cab"
      ^   ^         left跳到3, 窗口"abc"
        ^   ^       left跳到4, 窗口"bcb"→重复
          ^ ^       left跳到5, 窗口"cb"
            ^^      left跳到6, 窗口"b"
最长窗口长度 = 3
```

## 解法详解

### 解法1: 暴力枚举 — O(n³) / O(n)

**思考过程**：最直觉的做法 — 枚举所有子串的起止点 (i, j)，对每个子串用 set 检查是否有重复字符。

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int n = s.size(), ans = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {
                // 检查 s[i..j] 是否有重复
                unordered_set<char> seen;
                bool valid = true;
                for (int k = i; k <= j; k++) {
                    if (seen.count(s[k])) {
                        valid = false;
                        break;
                    }
                    seen.insert(s[k]);
                }
                if (valid) ans = max(ans, j - i + 1);
            }
        }
        return ans;
    }
};
```

**关键点**：三层循环，n = 50000 时完全不能接受（~1.25×10¹⁴ 次操作）。但理解这个暴力解有助于看清优化方向：内层的 "检查重复" 和 "枚举右端点" 可以合并。

---

### 解法2: 滑动窗口 + HashSet — O(n) / O(min(n, m)) ⭐ 面试首选

**从解法1优化**：暴力解中，当固定左端点 i、右端点 j 向右移动时，我们**重复检查**了很多已经看过的字符。实际上只需要**增量式地**检查新加入的字符是否重复。

当发现 `s[right]` 在窗口中已存在时，就**从左边逐个移除**，直到窗口内不再有 `s[right]`。

```
// 滑动窗口过程 (s = "abcabcbb"):
//
//  a b c a b c b b
//  L
//  R               → 加入'a', set={a}, len=1
//  L
//    R             → 加入'b', set={a,b}, len=2
//  L
//      R           → 加入'c', set={a,b,c}, len=3 ← max
//  L
//        R         → 'a'在set中! 移除s[L]='a', L++
//    L
//        R         → 现在'a'不在set中, 加入'a', set={b,c,a}, len=3
//    L
//          R       → 'b'在set中! 移除s[L]='b', L++
//      L
//          R       → 现在'b'不在set中, 加入'b', set={c,a,b}, len=3
//      ...以此类推
```

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_set<char> window; // 记录窗口内有哪些字符
        int left = 0, ans = 0;
        
        for (int right = 0; right < (int)s.size(); right++) {
            // 如果 s[right] 已在窗口中，从左边逐个移除直到消除重复
            while (window.count(s[right])) {
                window.erase(s[left]);
                left++;
            }
            // 此时窗口内无重复，加入 s[right]
            window.insert(s[right]);
            // 更新答案：当前窗口长度 = right - left + 1
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};
```

**为什么是 O(n)**：虽然有 while 循环嵌套在 for 循环里，但 left 指针在整个过程中**最多移动 n 次**（只往右不回头），right 指针也移动 n 次，总操作 ≤ 2n。

**关键点**：while 循环不是从头找重复字符，而是从 left 开始逐个移除。因为窗口是连续的，只需要把左边的字符移出去即可。

---

### 解法3: 滑动窗口 + HashMap（直接跳转） — O(n) / O(min(n, m))

**从解法2优化**：解法2 中左指针有时需要一步步移动多次才能消除重复。如果我们用 HashMap 记录每个字符**最后一次出现的索引**，就可以让左指针**一步跳到位**。

```
// 跳转示意 (s = "abcabcbb"):
//
//  索引: 0 1 2 3 4 5 6 7
//  字符: a b c a b c b b
//
//  right=0: 'a'首次出现, map={a:0}, left=0, len=1
//  right=1: 'b'首次出现, map={a:0,b:1}, left=0, len=2
//  right=2: 'c'首次出现, map={a:0,b:1,c:2}, left=0, len=3
//  right=3: 'a'上次在0, left = max(0, 0+1) = 1, map={a:3,b:1,c:2}, len=3
//  right=4: 'b'上次在1, left = max(1, 1+1) = 2, map={a:3,b:4,c:2}, len=3
//  right=5: 'c'上次在2, left = max(2, 2+1) = 3, map={a:3,b:4,c:5}, len=3
//  right=6: 'b'上次在4, left = max(3, 4+1) = 5, map={a:3,b:6,c:5}, len=2
//  right=7: 'b'上次在6, left = max(5, 6+1) = 7, map={a:3,b:7,c:5}, len=1
//
//  答案 = 3
```

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_map<char, int> lastPos; // 记录每个字符最后出现的索引
        int left = 0, ans = 0;
        
        for (int right = 0; right < (int)s.size(); right++) {
            // 如果 s[right] 之前出现过，且在当前窗口内
            if (lastPos.count(s[right]) && lastPos[s[right]] >= left) {
                // 直接跳到重复字符的下一个位置
                left = lastPos[s[right]] + 1;
            }
            lastPos[s[right]] = right; // 更新最后出现位置
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};
```

**为什么要 `max(left, lastPos[s[right]] + 1)` / `lastPos[s[right]] >= left`**：因为 map 中可能存着窗口左边的过期信息。例如 "abba"：处理第二个 'a' 时，map 中 'a' 的位置是 0，但此时 left 已经是 2 了（因为处理 'b' 时跳过了），如果不做判断，left 会倒退到 1。

---

### 解法4: 滑动窗口 + 数组优化 — O(n) / O(1)

**从解法3优化**：题目说字符由英文字母、数字、符号和空格组成，即 ASCII 字符，用大小 128 的数组替代 HashMap，避免哈希冲突的开销。

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        // 用 -1 初始化表示"从未出现过"
        int lastPos[128];
        memset(lastPos, -1, sizeof(lastPos));
        
        int left = 0, ans = 0;
        for (int right = 0; right < (int)s.size(); right++) {
            // 如果该字符上次出现的位置在窗口内，跳转左指针
            if (lastPos[(int)s[right]] >= left) {
                left = lastPos[(int)s[right]] + 1;
            }
            lastPos[(int)s[right]] = right;
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};
```

**对比解法3**：逻辑完全相同，只是把 `unordered_map<char, int>` 换成 `int[128]`。空间严格 O(1)（固定 128 个 int），且数组访问比哈希表快很多。

## 解法对比

| | 暴力枚举 | 滑窗 + Set | 滑窗 + Map | 滑窗 + 数组 |
|---|---|---|---|---|
| 时间 | O(n³) | O(n) | O(n) | O(n) |
| 空间 | O(n) | O(min(n,m)) | O(min(n,m)) | O(1) |
| 左指针移动 | — | 逐步收缩 | 直接跳转 | 直接跳转 |
| 常数因子 | 大 | 中 | 中 | 小 |
| 推荐场景 | 理解题意 | 面试首选 | 进阶理解 | 竞赛/极致优化 |

**面试推荐**：先写解法2（Set版），再口述或改写解法3（Map跳转版）。解法2思路更清晰不易出错，解法3展示优化能力。

## 易错点

1. **左指针倒退 bug**：
   - ✗ `left = lastPos[s[right]] + 1;`（不加判断直接跳转）
   - ✓ `left = max(left, lastPos[s[right]] + 1);` 或先检查 `lastPos[s[right]] >= left`
   - 原因：对 "abba"，处理最后的 'a' 时，map['a']=0 但 left 已经是 2，不加 max 会让 left 从 2 退回到 1。

2. **数组初始化为 0 而非 -1**：
   - ✗ `int lastPos[128] = {0};` 然后用 `lastPos[c] != 0` 判断是否出现过
   - ✓ `memset(lastPos, -1, sizeof(lastPos));` 然后用 `lastPos[c] >= left` 判断
   - 原因：索引 0 是合法位置，初始化为 0 会让第一个字符的位置和"未出现"混淆。

3. **空字符串未处理**：
   - 如果代码正确使用 `for (int right = 0; right < n; right++)`，空字符串自然返回 0，无需特判。但如果用 while 循环写法，要确保不会越界。

4. **窗口长度计算**：
   - ✗ `ans = max(ans, right - left);`（少了 +1）
   - ✓ `ans = max(ans, right - left + 1);`
   - 原因：窗口 [left, right] 包含 right - left + 1 个字符。

## 面试追问

**Q1: 暴力解能过吗？瓶颈在哪？**
暴力 O(n³) 对 n=50000 不能过。瓶颈在于：对每个起点 i，右端点 j 每次都从头检查整个子串是否有重复，没有利用"j-1 的检查结果"。滑动窗口的本质就是增量式地维护窗口信息。

**Q2: Set 版和 Map 版有什么本质区别？实际面试写哪个？**
Set 版在遇到重复时需要**逐步收缩**左指针（最坏单次 O(n)，但摊还 O(1)）；Map 版能**直接跳转**。两者总时间复杂度都是 O(n)。面试中 Set 版更不容易写错，推荐先写 Set 版，然后口头说明 Map 版的优化思路。

**Q3: 如果字符集非常大（比如 Unicode 全集），空间怎么优化？**
此时不能用固定大小数组了，只能用 HashMap。空间复杂度是 O(min(n, m))，其中 m 是字符集大小。对于 Unicode，m 可能到百万级，但实际窗口内的字符数不超过 n，所以空间仍然受限于 min(n, m)。

**Q4: 如果要返回最长无重复子串本身（而非长度），怎么改？**
记录答案更新时的 left 和 right，最后用 `s.substr(bestLeft, bestLen)` 返回。只需多存两个变量，不影响复杂度。

## 相关题型

- **76. 最小覆盖子串 (Minimum Window Substring)** — 同样的滑动窗口框架，区别是：本题窗口**扩张时检查是否无重复**（约束在窗口内），76 题窗口**收缩时检查是否覆盖目标**。掌握本题后做 76 题可以复用"右扩左缩"的模板。

- **159. 至多包含两个不同字符的最长子串** — 本题的变体：把"无重复字符"改成"字符种类 ≤ 2"。复用同样的滑动窗口，只是判断条件从 `set.count(c)` 变成 `map.size() > 2`。

- **340. 至多包含 K 个不同字符的最长子串** — 159 的泛化版本。窗口收缩条件变为 `map.size() > k`。

- **424. 替换后的最长重复字符** — 滑动窗口 + 频率统计。同样的框架，区别在于窗口合法条件不同。

- **438. 找到字符串中所有字母异位词** — 固定窗口大小的滑动窗口变体。