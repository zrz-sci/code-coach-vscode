[file://solution.cpp](./solution.cpp)

# 925. 长按键入

## 核心思路

用**双指针**分别遍历 `name` 和 `typed`。对于 `name` 中的每个字符，检查 `typed` 中是否有匹配的字符。匹配后，`typed` 中可能还有多余的相同字符（长按产生的），跳过它们即可。如果任何时刻字符不匹配，返回 false。

## 思维链

1. **读完题第一反应**：`typed` 是 `name` 的"扩展版"，每个字符可能被重复多次（长按）。核心是验证 `typed` 能否由 `name` 通过"重复某些字符"得到。

2. **双指针自然吻合**：指针 `i` 扫 `name`，指针 `j` 扫 `typed`。两者同时向前走，每次比较当前字符：
   - 如果 `name[i] == typed[j]`：两个指针都前进
   - 如果不等：说明 `typed[j]` 是长按的额外字符，但它必须和前一个字符相同（否则错误）

3. **长按字符的判定**：当 `name[i] != typed[j]` 时，`typed[j]` 必须等于 `typed[j-1]`（是前一个字符的长按重复）。如果不是，说明 `typed` 中出现了一个与 `name` 不对应的字符，返回 false。

4. **结束条件**：遍历完后，`i` 必须恰好等于 `name.size()`（name 全部匹配完）。如果 `i < name.size()`，说明 name 还有字符没匹配到，返回 false。

5. **边界情况**：typed 比 name 短 — 直接在循环中 j 先到末尾，i 没走完，返回 false。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 双指针逐字符 | i 扫 name，j 扫 typed，匹配或跳过长按 | O(m+n) | O(1) | ⭐ 面试首选 |
| 分组比较 | 按连续相同字符分组，逐组比较长度 | O(m+n) | O(m+n) | 思路更清晰 |

> 两种解法时间复杂度相同，双指针更省空间且代码更短。

## 关键提示

1. **typed 中的每个字符只有两种身份** — 要么匹配 name 中的当前字符（正常按键），要么是前一个字符的长按重复。如果两种身份都不满足，就是非法输入。

2. **j 指针永远不回退** — 这是单调双指针。j 每一步要么匹配 name[i]（i 和 j 同时前进），要么是长按跳过（只有 j 前进）。

3. **最后检查 i 是否到达 name 末尾** — 这是容易遗漏的终止条件。j 可以还没到末尾（typed 尾部多余的长按），但 i 必须到末尾。

4. 双指针过程示意：
```
name  = "alex"     typed = "aaleex"
         i=0                j=0

i=0,j=0: 'a'=='a' ✓ → i=1,j=1
i=1,j=1: 'l'!='a' → typed[1]=='a'==typed[0]? ✓ 长按跳过 → j=2
i=1,j=2: 'l'=='l' ✓ → i=2,j=3
i=2,j=3: 'e'=='e' ✓ → i=3,j=4
i=3,j=4: 'x'!='e' → typed[4]=='e'==typed[3]? ✓ 长按跳过 → j=5
i=3,j=5: 'x'=='x' ✓ → i=4,j=6
i==name.size() ✓ → return true
```

## 解法详解

### 解法1: 双指针逐字符 — O(m+n) / O(1) ⭐ 面试首选

**思考过程**：

用 `j` 遍历 `typed` 的每个字符，决定它是"匹配 name[i]"还是"长按重复"：

```cpp
class Solution {
public:
    bool isLongPressedName(string name, string typed) {
        int i = 0, j = 0;
        int m = name.size(), n = typed.size();
        
        while (j < n) {
            if (i < m && name[i] == typed[j]) {
                // 正常匹配
                i++;
                j++;
            } else if (j > 0 && typed[j] == typed[j - 1]) {
                // 长按重复：跳过
                j++;
            } else {
                // 既不匹配 name，也不是长按 → 非法
                return false;
            }
        }
        
        // name 必须全部匹配完
        return i == m;
    }
};
```

**关键点**：
- `j > 0` 的检查防止 j=0 时访问 typed[-1]。
- 三个分支覆盖了所有情况：匹配、长按、非法。
- 循环结束后检查 `i == m` 确保 name 全部消耗。

### 解法2: 分组比较 — O(m+n) / O(m+n)

**思考过程**：

把 `name` 和 `typed` 分别按连续相同字符分组，然后逐组比较：
- 组内的字符必须相同
- typed 组的长度必须 >= name 组的长度（长按只会增加，不会减少）

```cpp
class Solution {
public:
    bool isLongPressedName(string name, string typed) {
        // 将字符串按连续相同字符分组
        auto getGroups = [](const string& s) {
            vector<pair<char, int>> groups;
            int i = 0;
            while (i < s.size()) {
                char c = s[i];
                int count = 0;
                while (i < s.size() && s[i] == c) {
                    i++;
                    count++;
                }
                groups.push_back({c, count});
            }
            return groups;
        };
        
        auto nameGroups = getGroups(name);
        auto typedGroups = getGroups(typed);
        
        // 组数必须相同
        if (nameGroups.size() != typedGroups.size()) return false;
        
        for (int i = 0; i < nameGroups.size(); i++) {
            // 字符必须相同
            if (nameGroups[i].first != typedGroups[i].first) return false;
            // typed 组长度 >= name 组长度（可以长按，不能短）
            if (typedGroups[i].second < nameGroups[i].second) return false;
        }
        
        return true;
    }
};
```

**关键点**：
- 分组后比较更直观：字符相同、长度不短。
- 需要额外空间存储分组结果。

## 解法对比

| 维度 | 双指针逐字符 | 分组比较 |
|------|-------------|---------|
| 时间 | O(m+n) | O(m+n) |
| 空间 | O(1) | O(m+n) 存分组 |
| 代码量 | 更短 | 稍长但更清晰 |
| 边界处理 | j>0 检查 | 自然处理 |
| 面试推荐 | 首选 | 当思路需要更清晰时 |

## 易错点

1. **忘记最终检查 i == m**：
   - ✗ 循环结束直接返回 true → 如果 typed 提前结束但 name 还没匹配完
   - ✓ `return i == m;` 确保 name 每个字符都匹配到了

2. **长按判断时忘记 j > 0**：
   - ✗ `typed[j] == typed[j-1]` 当 j=0 时越界访问
   - ✓ `j > 0 && typed[j] == typed[j-1]`

3. **三个分支的优先级搞错**：
   - ✗ 先检查长按，再检查匹配 → 可能把匹配字符误判为长按
   - ✓ 先检查匹配（优先消耗 name），再检查长按（只是额外的重复）

4. **name 比 typed 长的情况**：
   - ✗ 没有处理 → 循环正常结束但 i < m
   - ✓ 最终 `i == m` 检查会捕获这个情况

5. **分组比较中只比较字符不比较长度**：
   - ✗ 只检查字符相同 → 长按不够的情况被放过
   - ✓ 必须同时检查 typedGroups[i].second >= nameGroups[i].second

## 面试追问

**Q1: 如果 typed 末尾有多余的长按字符怎么办？比如 name="abc", typed="abcccc"**

> 双指针法中，`i` 到达 name 末尾后，`j` 继续扫描 typed 剩余字符。这些字符必须都是长按（等于前一个字符），否则返回 false。代码中 `j > 0 && typed[j] == typed[j-1]` 这个条件自然处理了。

**Q2: 时间复杂度是 O(m+n) 还是 O(n)？**

> 更精确地说是 O(max(m, n))。`j` 最多走 n 步（typed 的长度），而 `i` 最多走 m 步。两者独立推进，总操作次数不超过 m + n。由于 typed.length >= name.length（否则直接 false），可以简写为 O(n)。

**Q3: 这道题能用正则表达式解吗？**

> 理论上可以：从 name 生成正则模式，比如 "alex" → `a+l+e+x+`，然后匹配 typed。但这效率不如双指针，且正则引擎有额外开销。面试中提一下即可，不推荐实际使用。

## 相关题型

- **LeetCode 844. 比较含退格的字符串** — 同样是双指针比较两个"处理过的"字符串。844 的"退格"对应本题的"长按"，都需要在遍历中处理特殊字符。
- **LeetCode 392. 判断子序列** — 判断 s 是否是 t 的子序列，双指针框架几乎相同。本题多了"长按字符必须和前一个相同"的约束。
- **LeetCode 1143. 最长公共子序列** — 如果本题不要求严格匹配而是求最长匹配，就变成 LCS 问题，需要 DP。
