# 187. 重复的DNA序列

## 核心思路

DNA 序列只由 `A, C, G, T` 四种字符组成。题目要求找出所有在字符串 `s` 中出现**超过一次**的长度为 10 的子串。

本质上这是一个**滑动窗口 + 哈希**的问题：用长度为 10 的窗口在字符串上滑动，记录每个子串出现的次数，次数 > 1 的就是答案。

## 思维链

1. **读完题第一反应** → 暴力做法：枚举所有长度为 10 的子串，统计出现次数。子串总共有 `n - 9` 个，每次提取子串需要 O(10) 的时间，总时间 O(10n)。
2. **暴力能不能过？** → `s.length` 最大 10^5，O(10n) = 10^6，完全可以。但存子串用 `string` 作为哈希 key，每个 key 长度 10，哈希计算和比较也是 O(10)。总体 O(10n) 没问题。
3. **有没有更优的做法？** → 既然只有4种字符，可以把每个字符编码为 2 bit（A=00, C=01, G=10, T=11），一个长度10的子串只需要 20 bit，用一个 `int` 就能表示。这样哈希 key 变成整数，比较和哈希都是 O(1)。
4. **滑动窗口怎么更新？** → 窗口右移一位时，左移掉最高位的 2 bit，右端加上新字符的 2 bit。类似"滚动哈希"的思想。
5. **去重怎么处理？** → 用 `unordered_map<key, int>` 记录出现次数，只在 count 从 1 变到 2 时加入结果，避免重复添加。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 哈希表 + 子串 | 直接用 string 子串做 key | O(10n) | O(10n) | 能快速写出 |
| 滑动窗口 + 位编码 | 4字符编码为2bit，整数做key | O(n) | O(n) | 进阶优化 |
| Rabin-Karp 滚动哈希 | 用数字哈希代替字符串比较 | O(n) | O(n) | 理解思想即可 |

## 关键提示

1. **字符集极小是突破口**：只有 `A, C, G, T` 四种字符 = 2 bit，10 个字符 = 20 bit，一个 `int` 就装得下。这是位运算优化的基础。
2. **去重的正确姿势**：不是用 `set` 存结果（那样还得从 map 转到 set），而是在 map 中判断 `count == 2` 时才加入结果 -- 刚好是第二次出现的那一刻。或者用两个 set：`seen` + `repeated`。
3. **位编码映射**：A=0, C=1, G=2, T=3（或任意不冲突的 2bit 编码）。关键是保证 4 种字符映射到不同的值。
4. **窗口滑动的位运算**：新窗口 = `((旧窗口 << 2) | 新字符编码) & mask`，其中 `mask = (1 << 20) - 1` 用来保留低 20 位（即窗口长度 10 个字符 * 2 bit）。
5. **边界情况**：如果 `s.length() <= 10`，直接返回空数组（不可能有重复的长度 10 子串）。

## 解法详解

### 解法1: 哈希表 + 子串 -- O(10n) / O(10n)

**思考过程**: 最直观的做法 -- 枚举所有长度为 10 的子串，用哈希表统计出现次数。

```
示例: s = "AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT"

所有长度10的子串:
  s[0..9]   = "AAAAACCCCC"   → count: 1
  s[1..10]  = "AAAACCCCCA"   → count: 1
  s[2..11]  = "AAACCCCCAA"   → count: 1
  ...
  s[5..14]  = "CCCCCAAAAA"   → count: 1
  ...
  s[10..19] = "AAAAACCCCC"   → count: 2  ← 第二次出现! 加入结果
  ...
  s[13..22] = "AACCCCCCAA"   → count: 1
  ...
  s[15..24] = "CCCCCCAAAA"   → count: 1
  s[16..25] = "CCCCCAAAAA"   → count: 2  ← 第二次出现! 加入结果
  ...

最终结果: ["AAAAACCCCC", "CCCCCAAAAA"]
```

```cpp
class Solution {
public:
    vector<string> findRepeatedDnaSequences(string s) {
        if (s.size() <= 10) return {};
        
        unordered_map<string, int> seen;
        vector<string> result;
        
        for (int i = 0; i + 10 <= (int)s.size(); i++) {
            string sub = s.substr(i, 10);
            seen[sub]++;
            if (seen[sub] == 2) {  // 恰好第二次出现时加入
                result.push_back(sub);
            }
        }
        return result;
    }
};
```

**关键点**:
- `seen[sub] == 2` 而不是 `> 1`，确保每个重复子串只加入结果一次。
- `substr` 创建新字符串有 O(10) 的开销，但 10 是常数，总体 O(n)。
- 这个解法在面试中完全够用，代码简洁易写。

**替代写法**: 用两个 `unordered_set` 代替 `map`：

```cpp
unordered_set<string> seen, repeated;
for (int i = 0; i + 10 <= (int)s.size(); i++) {
    string sub = s.substr(i, 10);
    if (!seen.insert(sub).second) {  // insert 返回 pair，second=false 说明已存在
        repeated.insert(sub);
    }
}
return vector<string>(repeated.begin(), repeated.end());
```

---

### 解法2: 滑动窗口 + 位编码 -- O(n) / O(n)

**从解法1优化**: 解法1中每个子串是长度 10 的 `string`，作为哈希 key 比较慢。能不能把子串编码成一个整数？

**核心观察**: DNA 只有 4 种碱基，每种可以用 2 bit 编码。10 个碱基 = 20 bit，一个 `int`（32bit）就够了。

```
位编码:  A = 00 (0)
         C = 01 (1)
         G = 10 (2)
         T = 11 (3)

子串 "ACGT" 的编码过程:
  初始 hash = 0
  加入 A(0):  hash = (0  << 2) | 0 = 0       = 0b00
  加入 C(1):  hash = (0  << 2) | 1 = 1       = 0b0001
  加入 G(2):  hash = (1  << 2) | 2 = 6       = 0b000110
  加入 T(3):  hash = (6  << 2) | 3 = 27      = 0b00011011

滑动窗口更新:
  旧窗口: |A C G T A C G T A C|G T ...
  新窗口:  A|C G T A C G T A C G|T ...
  
  操作: hash = ((hash << 2) | encode(新字符)) & mask

  其中 mask = (1 << 20) - 1 = 0xFFFFF (保留低20位)
  << 2 同时完成了"腾出右边2位"和"最高位的字符自动被mask掉"两件事
```

```cpp
class Solution {
public:
    vector<string> findRepeatedDnaSequences(string s) {
        if (s.size() <= 10) return {};
        
        // 字符 -> 2bit 编码
        int encode[128] = {};
        encode['A'] = 0;  // 00
        encode['C'] = 1;  // 01
        encode['G'] = 2;  // 10
        encode['T'] = 3;  // 11
        
        int mask = (1 << 20) - 1;  // 低20位全1
        int hash = 0;
        
        unordered_map<int, int> seen;
        vector<string> result;
        
        // 先计算前9个字符的哈希
        for (int i = 0; i < 9; i++) {
            hash = (hash << 2) | encode[(int)s[i]];
        }
        
        // 从第10个字符开始，每次滑动一位
        for (int i = 9; i < (int)s.size(); i++) {
            hash = ((hash << 2) | encode[(int)s[i]]) & mask;
            seen[hash]++;
            if (seen[hash] == 2) {
                result.push_back(s.substr(i - 9, 10));
            }
        }
        return result;
    }
};
```

**关键点**:
- `mask = (1 << 20) - 1` 确保哈希值始终只保留低 20 位（10个字符 * 2bit = 20bit）。
- 先处理前 9 个字符构建初始窗口，从第 10 个字符开始正式滑动。
- 整数比较和哈希比字符串快很多，常数因子更小。
- 整数 key 占用的空间也比 string key 少得多。

---

### 解法3: Rabin-Karp 风格滚动哈希 -- O(n) / O(n)

**另一种视角**: 把 DNA 看成一个 4 进制数，用 Rabin-Karp 风格的滚动哈希来计算。

```
将 DNA 看作 4 进制数:
  A=0, C=1, G=2, T=3

  "ACGT" = 0*4^3 + 1*4^2 + 2*4^1 + 3*4^0 = 0 + 16 + 8 + 3 = 27

滑动时:
  旧窗口 "ACGTACGTAC", 新加入字符 G
  去掉最高位: hash -= encode(A) * 4^9     // 减去 A 的贡献
  腾出位置:   hash *= 4                    // 所有字符左移一位
  加上新字符: hash += encode(G)            // 新字符进入窗口
```

这本质上和解法2的位运算是等价的（`<< 2` 就是 `* 4`），只是换了一种理解方式。对于更一般的字符集（比如 26 个字母），Rabin-Karp 风格更通用。

```cpp
class Solution {
public:
    vector<string> findRepeatedDnaSequences(string s) {
        if (s.size() <= 10) return {};
        
        unordered_map<char, int> encode = {{'A',0},{'C',1},{'G',2},{'T',3}};
        int base = 4;
        long long power = 1;  // base^9，用于去掉最高位
        for (int i = 0; i < 9; i++) power *= base;
        
        long long hash = 0;
        unordered_map<long long, int> seen;
        vector<string> result;
        
        // 计算第一个窗口
        for (int i = 0; i < 10; i++) {
            hash = hash * base + encode[s[i]];
        }
        seen[hash] = 1;
        
        // 滑动窗口
        for (int i = 10; i < (int)s.size(); i++) {
            hash = (hash - encode[s[i - 10]] * power) * base + encode[s[i]];
            seen[hash]++;
            if (seen[hash] == 2) {
                result.push_back(s.substr(i - 9, 10));
            }
        }
        return result;
    }
};
```

**关键点**:
- `power = 4^9` 是最高位的权重，用于"减去"滑出窗口的字符。
- 由于窗口长度固定为 10，4^9 = 262144，不会溢出。
- Rabin-Karp 在更一般的"字符串匹配"和"重复子串"问题中非常重要（如第 1044 题）。

## 解法对比

|  | 哈希表+子串 | 位编码 | Rabin-Karp |
|--|-----------|--------|------------|
| 时间 | O(10n) ~ O(n) | **O(n)** | O(n) |
| 空间 | O(10n) | **O(n)** | O(n) |
| 常数因子 | 较大（string哈希） | **最小** | 中等 |
| 代码复杂度 | **最简单** | 中等 | 中等 |
| 通用性 | 任意字符集 | 仅小字符集 | 任意字符集 |

**面试选择**: 先写解法1（快速正确），然后讲解法2的优化思路（展示位运算功底）。

## 易错点

1. **去重写错，结果中出现重复子串**：
   - 错: `if (seen[sub] > 1) result.push_back(sub);` → 出现3次的子串会被加入2次
   - 对: `if (seen[sub] == 2) result.push_back(sub);` → 只在第2次出现时加入

2. **位编码的 mask 计算错误**：
   - 错: `mask = (1 << 10) - 1` → 只保留10位，但我们需要20位（10字符 * 2bit）
   - 对: `mask = (1 << 20) - 1`

3. **滑动窗口的起始位置搞错**：
   - 错: 从 `i = 0` 开始就统计 → 前9个字符不够10个，得到的不是有效子串
   - 对: 先处理前9个字符，从 `i = 9`（第10个字符）开始统计

4. **Rabin-Karp 的 power 计算错误**：
   - 错: `power = 4^10` → 这是11个字符的窗口的最高位权重
   - 对: `power = 4^9` → 10个字符的窗口，最高位是第0位乘以 base^9

5. **忘记处理 `s.length() <= 10` 的边界**：
   - 不做检查的话循环只执行0~1次（无害），但提前返回更清晰。

## 面试追问

**Q1: 为什么不用 set 去重？**
> 可以用 `unordered_set<string> seen` + `unordered_set<string> repeated`，seen 记录是否见过，repeated 自动去重。但这需要两个集合。用 `map<string, int>` 一个就搞定，判断 `count == 2` 更优雅。两种写法都可以，面试官不会纠结。

**Q2: 如果窗口长度不是 10 而是 k（k 很大），解法1还行吗？**
> 不行。每次 `substr` 是 O(k)，总时间 O(kn)。此时位编码也不行（k 大了 2k bit 超过 int/long long 范围）。需要用 Rabin-Karp 滚动哈希 + 取模，时间 O(n)。这就是第 1044 题（最长重复子串）的核心思路。

**Q3: 如果要找出现恰好 k 次的子串呢？**
> 统计所有子串的出现次数（用哈希表），然后筛选 count == k 的。框架完全一样，只改过滤条件。

**Q4: 位编码方案的字符映射怎么选？有讲究吗？**
> 任意 4 个不同的 2bit 值都行（00, 01, 10, 11 的排列）。只要保证：(1) 不同字符映射不同值；(2) 每个值在 [0, 3] 范围内。一个巧妙的映射方式是利用 ASCII：`(s[i] >> 1) & 3` 恰好能区分 A(0x41), C(0x43), G(0x47), T(0x54)，但面试中显式数组映射最清晰。

## 相关题型

- **28. 找出字符串中第一个匹配项的下标** -- Rabin-Karp 是本题滚动哈希的"原型"算法，用于字符串匹配。
- **1044. 最长重复子串** -- 本题的"进阶版"：窗口长度不固定，需要二分搜索 + Rabin-Karp 找最长重复子串。核心的"判断是否存在长度为 k 的重复子串"与本题思路一致。
- **49. 字母异位词分组** -- 同样是"字符串编码 + 哈希分组"的思路，但编码方式不同（排序或计数数组）。
- **438. 找到字符串中所有字母异位词** -- 固定窗口滑动 + 哈希，和本题的滑动窗口框架类似，但匹配条件不同（字符频率而非完全匹配）。
