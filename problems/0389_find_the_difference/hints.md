# 389. 找不同 (Find the Difference)

## 难度: Easy
## 标签: Bit Manipulation, Hash Table, String, Sorting

---

## 核心思路

给定两个字符串 s 和 t，t 是 s 随机重排后在随机位置添加了一个字母。找出这个被添加的字母。

**关键洞察**：这道题本质上是「找出多余的那一个元素」，与 Single Number (LC 136) 思路一致。可以用三种方法：
1. **异或 (XOR)**：相同字符异或为 0，最终剩下的就是多余的字符
2. **求和差值**：t 的 ASCII 总和 - s 的 ASCII 总和 = 多出字符的 ASCII
3. **哈希计数**：统计字符频率差异

其中异或法和求和法都是 O(1) 空间，是最优解。

---

## 思维链（5步推导）

### Step 1: 暴力思路（排序后比较）
- 将 s 和 t 分别排序
- 逐字符比较，第一个不同位置就是答案
- 如果所有字符都相同，答案是 t 的最后一个字符
- 时间 O(n log n)，空间 O(1)（原地排序）

### Step 2: 哈希计数
- 统计 s 中每个字符出现的次数
- 遍历 t，对每个字符减少计数
- 当某个字符的计数变为负数时，它就是答案
- 时间 O(n)，空间 O(26) = O(1)

### Step 3: 求和差值
- 分别计算 s 和 t 所有字符的 ASCII 值之和
- 差值就是多出字符的 ASCII 值
- 注意：用 int 类型存和，避免 char 溢出
- 时间 O(n)，空间 O(1)

### Step 4: 异或法（最优雅）
- 性质：a ^ a = 0，a ^ 0 = a
- 将 s 和 t 的所有字符异或在一起
- s 和 t 中成对出现的字符全部抵消为 0
- 剩下的就是只出现一次的多余字符
- 时间 O(n)，空间 O(1)

### Step 5: 验证
```
s = "abcd", t = "abcde"
异或: 'a'^'b'^'c'^'d' ^ 'a'^'b'^'c'^'d'^'e'
    = ('a'^'a') ^ ('b'^'b') ^ ('c'^'c') ^ ('d'^'d') ^ 'e'
    = 0 ^ 0 ^ 0 ^ 0 ^ 'e'
    = 'e'
```

---

## 解法概览

| 解法 | 时间 | 空间 | 特点 | 推荐度 |
|------|------|------|------|--------|
| Solution1: 异或 (XOR) | O(n) | O(1) | **最优雅，无溢出** | ⭐⭐⭐⭐⭐ |
| Solution2: 求和差值 | O(n) | O(1) | 直观简单 | ⭐⭐⭐⭐ |
| Solution3: 哈希计数 | O(n) | O(1) | 通用性最强 | ⭐⭐⭐⭐ |
| Solution4: 排序比较 | O(n log n) | O(1) | 最朴素 | ⭐⭐ |

---

## 关键提示

1. **异或性质**：a ^ a = 0, a ^ 0 = a，交换律+结合律使顺序无关
2. **求和注意溢出**：用 int 存和而不是 char，char 最大 127 可能不够
3. **哈希计数用数组**：只有 26 个小写字母，int count[26] 比 map 更快
4. **这是 Single Number 的变体**：寻找"出现奇数次的唯一元素"

---

## 解法详解

### Solution1: 异或 (XOR)（最优雅 ⭐）

**思考过程**：
- XOR 的核心性质：相同元素异或为 0，任何元素与 0 异或为自身
- 把 s 和 t 的所有字符全部异或，成对出现的字符抵消为 0
- 最终结果就是只出现一次的那个多余字符

```cpp
class Solution1 {
public:
    char findTheDifference(string s, string t) {
        char res = 0;
        for (char c : s) res ^= c;  // 异或 s 的所有字符
        for (char c : t) res ^= c;  // 异或 t 的所有字符
        return res;                  // 成对抵消，剩余就是答案
    }
};
```

**关键点**：
- 不需要关心字符的顺序，异或满足交换律和结合律
- 不存在溢出问题（char XOR char 还是 char）
- 一行循环写法：`for (char c : s + t) res ^= c;`（但会创建新字符串）

---

### Solution2: 求和差值

**思考过程**：
- t 比 s 多一个字符
- sum(t) - sum(s) = 多出字符的 ASCII 值
- 可以合并为一个循环：遍历 t 加，遍历 s 减

```cpp
class Solution2 {
public:
    char findTheDifference(string s, string t) {
        int sum = 0;
        // t 比 s 多一个字符, 直接对 t 求和
        for (char c : t) sum += c;
        // 减去 s 的所有字符
        for (char c : s) sum -= c;
        return (char)sum;
    }
};
```

**关键点**：
- 使用 int 类型的 sum，避免 char 加法溢出（26 * 122 = 3172 > 127）
- 最终 sum 的值在 [0, 122] 范围内，安全转为 char

---

### Solution3: 哈希计数（通用性最强）

**思考过程**：
- 统计 s 中每个字符的出现次数
- 遍历 t，逐个递减计数
- 当某字符计数变为负数，说明它就是多余的

```cpp
class Solution3 {
public:
    char findTheDifference(string s, string t) {
        int count[26] = {};
        // 统计 s 的字符频率
        for (char c : s) count[c - 'a']++;
        // 遍历 t, 逐个减少
        for (char c : t) {
            count[c - 'a']--;
            if (count[c - 'a'] < 0) return c;  // 发现多余字符
        }
        return ' '; // 不会到达这里
    }
};
```

**关键点**：
- 数组大小固定为 26，空间 O(1)
- 可以提前返回（一旦发现多余字符立即停止）
- 这种方法最通用，适用于"差异不止一个"的变体

---

### Solution4: 排序后比较

**思考过程**：
- 排序后，s 和 t 的字符在相同位置应该一致
- 第一个不匹配的位置就是答案
- 如果全部匹配，答案是 t 的最后一个字符

```cpp
class Solution4 {
public:
    char findTheDifference(string s, string t) {
        sort(s.begin(), s.end());
        sort(t.begin(), t.end());
        for (int i = 0; i < (int)s.size(); i++) {
            if (s[i] != t[i]) return t[i];
        }
        return t.back();  // 多出的字符在最后
    }
};
```

**关键点**：
- 时间 O(n log n) 因为排序
- 修改了输入字符串（或需要拷贝）
- 面试中不推荐，但可作为思维起点

---

## 解法对比

| 维度 | Solution1 异或 | Solution2 求和 | Solution3 计数 | Solution4 排序 |
|------|---------------|---------------|---------------|---------------|
| 时间复杂度 | O(n) | O(n) | O(n) | O(n log n) |
| 空间复杂度 | O(1) | O(1) | O(1) | O(1)/O(n) |
| 溢出风险 | 无 | int可处理 | 无 | 无 |
| 通用性 | 仅限一个差异 | 仅限一个差异 | 可扩展多个差异 | 可扩展 |
| 优雅程度 | 最高 | 高 | 中 | 低 |

**面试策略**：先说异或法展示位运算功底，再提及求和法和计数法展示多种思路。

---

## 易错点（具体 Bug 示例）

### Bug 1: 异或时 res 初始化为字符而非 0
```cpp
char res = 'a';  // 错! 应该初始化为 0
// 0 是异或的幺元, 不影响结果
// 初始化为 'a' 会多一个 'a' 参与异或
```

### Bug 2: 求和时用 char 类型存和
```cpp
char sum = 0;
for (char c : t) sum += c;  // 可能溢出! char 最大 127
// s = "zzzzzzzz", t 的和 > 127 -> 溢出为负数
// 修复: 用 int sum = 0;
```

### Bug 3: 计数法中遍历顺序搞反
```cpp
// 错误: 先遍历 t 再遍历 s
for (char c : t) count[c - 'a']++;
for (char c : s) {
    count[c - 'a']--;
    if (count[c - 'a'] < 0) return c;  // 可能把 s 中的字符误判
}
// 正确: 先统计 s, 再遍历 t 减少
```

### Bug 4: 忘记处理 s 为空的情况
```cpp
// s = "", t = "y"
// 异或法: res = 0 ^ 'y' = 'y' (正确处理)
// 求和法: sum = 'y' - 0 = 'y' (正确处理)
// 排序法: s 为空, 循环不执行, return t.back() (正确)
// 大多数解法都能自然处理，但要注意测试这个边界
```

---

## 面试追问递进链

### Q1: "如果不止多一个字符，而是 t 比 s 多了 k 个相同字符呢？"
**答**：
- 异或法不适用（多个相同字符异或为 0 或原值，取决于奇偶）
- 求和法依然有效：sum(t) - sum(s) = k * ASCII(该字符)，如果知道 k 就能求出
- 计数法最通用：比较频率差异

### Q2: "如果 s 和 t 可能有多个不同字符呢？"
**答**：只能用哈希计数法，统计频率差异，返回所有不同的字符。异或和求和法在多差异时不适用。

### Q3: "异或法能用在哪些其他场景？"
**答**：
- LC 136: Single Number（找唯一出现一次的数）
- LC 268: Missing Number（找缺失的数）
- LC 260: Single Number III（两个只出现一次的数）
- 通用模式：「找出出现奇数次的元素」

### Q4: "求和法有溢出风险吗？"
**答**：用 int 存和时，最坏情况 1001 * 122 = 122122，远小于 INT_MAX。安全。但如果字符串长度很大或 ASCII 值很大（如 Unicode），需要用 long long。

---

## 相关题型

| 题目 | 关系 | 复用说明 |
|------|------|----------|
| [136. Single Number](../0136_single_number/) | 异或找唯一 | 完全相同的异或思路 |
| [268. Missing Number](../0268_missing_number/) | 找缺失数 | 异或法或求和法 |
| [260. Single Number III](../0260_single_number_iii/) | 两个唯一数 | 异或 + 分组 |
| [242. Valid Anagram](../0242_valid_anagram/) | 字母异位词 | 哈希计数比较 |
| [771. Jewels and Stones](../0771_jewels_and_stones/) | 集合查找 | 哈希表 + 字符串遍历 |
