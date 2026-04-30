# 443. 压缩字符串

## 核心思路

本质上是一道**原地双指针**题：用一个读指针扫描连续重复字符并计数，用一个写指针把压缩结果覆盖回原数组。关键在于"压缩后的长度 ≤ 原始长度"，所以写指针永远不会越过读指针，原地修改是安全的。

## 思维链

1. **读完题第一反应**：遍历数组，统计每组连续相同字符的个数，然后把字符+个数写回去。最朴素的做法是用一个额外的字符串/数组存结果，最后拷贝回 `chars`。
2. **空间瓶颈**：题目要求 O(1) 额外空间，不能用额外数组。那能不能直接在原数组上写？
3. **关键观察——写指针不会超过读指针**：
   - 一个字符出现 1 次 → 只写 1 个字符（长度不变）
   - 一个字符出现 2 次 → 写 2 个位置（"a2"），原来占 2 个位置（"aa"），不变
   - 一个字符出现 12 次 → 写 3 个位置（"b12"），原来占 12 个位置，缩短了
   - **压缩后长度 ≤ 原始长度**，所以写指针 `write` 永远 ≤ 读指针 `read`，不会覆盖还没读过的数据
4. **双指针设计**：
   - `read` 指针：遍历原数组，找到每组连续字符的起止位置
   - `write` 指针：在原数组上写入压缩结果
   - 对每组：先写字符，再写计数（如果 > 1），计数要拆成单个字符逐位写入
5. **数字写入细节**：计数如 12 需要写成 '1' 和 '2' 两个字符。可以先转成字符串再逐字符写入，或者用除法/取模手动拆分。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 额外空间 | 新建字符串存压缩结果，拷贝回原数组 | O(n) | O(n) | 能说出即可 |
| 双指针原地 | read 扫描计数，write 原地写入 | O(n) | O(1) | ⭐ 必须写出 |

## 关键提示

1. **安全性保证**：想清楚为什么 `write ≤ read` 始终成立——这是原地修改不出错的核心原因。
2. **计数 = 1 时不写数字**：题目规定组长度为 1 时只写字符，不追加 "1"。
3. **计数 ≥ 10 的处理**：12 要拆成 '1' 和 '2'。用 `to_string` 最简单，手动做的话注意位序。
4. **统计一组的长度**：内层循环统计时，条件是 `read < n && chars[read] == chars[read的起点]`，注意别越界。
5. **返回值是 write 指针的最终位置**，不是 `chars.size()`。

```
// 压缩过程可视化:
// 输入: ['a','a','b','b','c','c','c']
//
// 第1组: 'a' x2
//   write=0: chars[0]='a'  → ['a','a','b','b','c','c','c']
//   write=1: chars[1]='2'  → ['a','2','b','b','c','c','c']
//                                  ↑write    ↑read
// 第2组: 'b' x2
//   write=2: chars[2]='b'  → ['a','2','b','b','c','c','c']
//   write=3: chars[3]='2'  → ['a','2','b','2','c','c','c']
//                                      ↑write    ↑read
// 第3组: 'c' x3
//   write=4: chars[4]='c'  → ['a','2','b','2','c','c','c']
//   write=5: chars[5]='3'  → ['a','2','b','2','c','3','c']
//                                          ↑write        ↑read=7(结束)
// 返回 write=6
```

## 解法详解

### 解法1: 额外空间 — O(n) / O(n)

**思考过程**: 最直觉的做法——用一个临时字符串收集压缩结果，最后写回原数组。不满足 O(1) 空间要求，但有助于理解压缩逻辑本身。

```cpp
class Solution {
public:
    int compress(vector<char>& chars) {
        int n = chars.size();
        string result;
        
        int i = 0;
        while (i < n) {
            char ch = chars[i];
            int count = 0;
            // 统计连续相同字符的个数
            while (i < n && chars[i] == ch) {
                i++;
                count++;
            }
            // 写入字符
            result += ch;
            // 只有 count > 1 才写数字
            if (count > 1) {
                result += to_string(count);
            }
        }
        
        // 把结果拷贝回原数组
        for (int j = 0; j < (int)result.size(); j++) {
            chars[j] = result[j];
        }
        return result.size();
    }
};
```

**关键点**: 这里先把逻辑理清——外层 while 遍历每组，内层 while 计数。count=1 时不写数字。这个逻辑是所有解法的基础。

### 解法2: 双指针原地修改 — O(n) / O(1) ⭐ 面试首选

**从解法1优化**: 解法1 的瓶颈是用了 O(n) 额外空间。核心观察是压缩后长度 ≤ 原始长度，所以可以直接用 `write` 指针在原数组上写，不会覆盖未读数据。

```cpp
class Solution {
public:
    int compress(vector<char>& chars) {
        int n = chars.size();
        int write = 0;  // 写指针：下一个要写入的位置
        int read = 0;   // 读指针：当前正在扫描的位置
        
        while (read < n) {
            char ch = chars[read];
            int count = 0;
            
            // 统计当前字符的连续出现次数
            while (read < n && chars[read] == ch) {
                read++;
                count++;
            }
            
            // 写入字符本身
            chars[write++] = ch;
            
            // 只有 count > 1 才写入数字
            if (count > 1) {
                // 把数字转成字符串，逐字符写入
                string countStr = to_string(count);
                for (char c : countStr) {
                    chars[write++] = c;
                }
            }
        }
        
        return write;
    }
};
```

**关键点**:
- `write` 始终 ≤ `read`，因为压缩后不可能比原来长
- `to_string(count)` 自动处理了多位数的拆分问题
- 返回 `write` 而不是 `chars.size()`

### 解法2b: 双指针原地（不用 to_string） — O(n) / O(1)

**变体**: 有些面试官不允许用 `to_string`，手动处理数字到字符的转换。

```cpp
class Solution {
public:
    int compress(vector<char>& chars) {
        int n = chars.size();
        int write = 0, read = 0;
        
        while (read < n) {
            char ch = chars[read];
            int count = 0;
            while (read < n && chars[read] == ch) {
                read++;
                count++;
            }
            
            chars[write++] = ch;
            
            if (count > 1) {
                // 手动将 count 的每一位写入
                // 先记录 write 的起始位置
                int start = write;
                while (count > 0) {
                    chars[write++] = '0' + (count % 10);
                    count /= 10;
                }
                // 上面写入的是逆序（个位先写），需要反转
                reverse(chars.begin() + start, chars.begin() + write);
            }
        }
        
        return write;
    }
};
```

**关键点**: 用取模写入数字时是逆序的（如 12 会先写 '2' 再写 '1'），必须反转。这是手动拆分数字的经典易错点。

## 解法对比

| | 解法1(额外空间) | 解法2(双指针+to_string) | 解法2b(双指针手动) |
|---|---|---|---|
| 时间 | O(n) | O(n) | O(n) |
| 空间 | O(n) | O(1)* | O(1) |
| 代码复杂度 | 简单 | 简单 | 稍复杂 |
| 满足题目要求 | ✗ | ✓ | ✓ |

*注：`to_string` 内部会创建临时字符串，严格来说是 O(log count) 空间。面试中一般认为是 O(1)，因为 count ≤ 2000，最多 4 个字符。

**选择建议**: 面试首选解法2，简洁清晰。如果被追问不用库函数，切换到解法2b。

## 易错点

1. **count=1 时写了 "1"**：
   - ✗ `chars = ['a','1']` (对单个 'a')
   - ✓ `chars = ['a']` (count=1 不写数字)
   - 题目明确说"组长度为 1 则只追加字符"

2. **多位数字忘记拆分**：
   - ✗ count=12 写成单个字符 `chars[write++] = 12` → 完全错误，12 不是合法 char
   - ✓ count=12 拆成 '1' 和 '2' 分别写入

3. **手动拆数字忘记反转**：
   - ✗ 12 → 先写 '2' 再写 '1' → "b21"
   - ✓ 12 → 先写 '2' 再写 '1' → 反转得 "b12"

4. **内层循环越界**：
   - ✗ `while (chars[read] == ch)` — read 可能已经 = n
   - ✓ `while (read < n && chars[read] == ch)` — 先检查边界

5. **返回值错误**：
   - ✗ `return chars.size()` — 原数组大小没变
   - ✓ `return write` — 写指针位置才是压缩后长度

## 面试追问

**Q1: 为什么原地修改是安全的？write 不会超过 read 吗？**

> 关键证明：对每组连续字符，原始占 `count` 个位置，压缩后占 `1 + len(str(count))` 个位置（count=1 时占 1 个）。当 count=1 时 1≤1；当 count≥2 时 `1+digits(count) ≤ count`（因为 2→2位, 10→3位, 100→4位...都不超过原数）。所以 write 永远追不上 read。

**Q2: 如果要求压缩后的字符串可能比原数组长怎么办？（比如每个字符都不重复且要写 "a1b1c1"）**

> 这种情况 write 可能超过 read，原地修改就不安全了。需要：
> - 方案A：先算出压缩后总长度，从后往前写（类似数组原地扩展的经典技巧）
> - 方案B：使用额外空间

**Q3: 如果输入是字符串流（streaming），一次只能读一个字符，怎么压缩？**

> 维护当前字符和计数器。每读一个新字符，如果和当前相同则计数+1；如果不同则输出当前字符的压缩结果，重置计数器。最后别忘了输出最后一组。

## 相关题型

- **38. 外观数列 (Count and Say)** — 复用相同的"连续相同字符分组+计数"逻辑，区别在于本题原地修改，38 题生成新字符串
- **271. 字符串的编码与解码 (Encode and Decode Strings)** — 同属字符串编码类，但编码策略不同（长度前缀 vs 游程编码）
- **394. 字符串解码 (Decode String)** — 本题的逆操作方向（压缩 vs 解压），用栈处理嵌套