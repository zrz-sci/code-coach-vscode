# 161. 相隔为 1 的编辑距离

## 核心思路
本质是分类讨论：根据两个字符串的长度差（0 或 1），在第一个不同字符处判断剩余部分是否完全相同。

## 思维链
1. **暴力思考**：计算完整的编辑距离（DP），然后判断是否等于 1。但编辑距离是 O(mn)，杀鸡用牛刀。
2. **观察约束**：编辑距离恰好为 1，只有三种操作：插入、删除、替换。插入和删除本质是对称的。
3. **长度约束**：如果 |len(s) - len(t)| > 1，一定不是 one edit distance。如果 |len(s) - len(t)| == 0，只能替换。如果 == 1，只能插入/删除。
4. **利用前缀**：从左到右扫描，找到第一个不同的位置 i，然后根据长度关系判断剩余部分。
5. **边界处理**：两个空字符串编辑距离为 0，不是 1。两个相同字符串也是 0。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 分类讨论 + 一次遍历 | 根据长度差分三种情况 | O(n) | O(1) | ⭐ 必知 |
| substr 比较法 | 找到分歧点后用 substr 比较 | O(n) | O(n) | 简洁 |

## 关键提示
1. **先保证 s 较短**：`if (s.size() > t.size()) swap(s, t);` 简化后续逻辑
2. **长度差 > 1 直接返回 false**：不可能一步到位
3. **等长时**：找到第一个不同字符，之后的部分必须完全相同（恰好替换这一个字符）
4. **差1时**：找到第一个不同字符，s 的剩余部分必须等于 t 从下一个位置开始的部分（在 s 中插入一个字符）
5. **全部相同的陷阱**：如果扫描完没有不同字符，则两个字符串要么相等（返回 false），要么长度差1且 t 多了最后一个字符（返回 true）

```
场景1: 等长替换
s = "abc"    t = "adc"
     ^ 位置1不同
检查: s[2..] == t[2..] → "c" == "c" ✓  → true

场景2: 差1插入
s = "ab"     t = "acb"
     ^ 位置1不同
检查: s[1..] == t[2..] → "b" == "b" ✓  → true

场景3: 差1且前缀完全相同
s = "ab"     t = "abc"
扫描完 s 没有不同 → len(t) - len(s) == 1 → true
```

## 解法详解

### 解法1: 分类讨论 + 一次遍历

**思路过程**：
- 确保 s 是较短的字符串，减少分支
- 长度差 > 1 直接 false
- 找第一个不同位置 i
- 等长：检查 s[i+1:] == t[i+1:]
- 差1：检查 s[i:] == t[i+1:]（相当于 t 在位置 i 多了一个字符）

```cpp
class Solution1 {
public:
    bool isOneEditDistance(string s, string t) {
        int ns = s.size(), nt = t.size();
        // 保证 s 是较短的
        if (ns > nt) return isOneEditDistance(t, s);
        
        // 长度差 > 1 不可能
        if (nt - ns > 1) return false;
        
        for (int i = 0; i < ns; ++i) {
            if (s[i] != t[i]) {
                if (ns == nt) {
                    // 等长：替换场景，剩余部分必须相同
                    return s.substr(i + 1) == t.substr(i + 1);
                } else {
                    // 差1：插入场景，s从i开始 == t从i+1开始
                    return s.substr(i) == t.substr(i + 1);
                }
            }
        }
        
        // 遍历完 s 都没有不同字符
        // 只有 t 比 s 恰好多一个字符才返回 true
        return nt - ns == 1;
    }
};
```

**复杂度分析**：
- 时间 O(n)：最坏情况遍历整个字符串
- 空间 O(n)：substr 会创建新字符串。可用逐字符比较优化到 O(1)

### 解法2: 纯指针法 (空间 O(1))

**思路过程**：
- 不用 substr，直接用双指针从分歧点开始逐字符比较
- 避免额外空间开销

```cpp
class Solution2 {
public:
    bool isOneEditDistance(string s, string t) {
        int ns = s.size(), nt = t.size();
        if (ns > nt) return isOneEditDistance(t, s);
        if (nt - ns > 1) return false;
        
        bool foundDiff = false;
        int i = 0, j = 0;
        
        while (i < ns && j < nt) {
            if (s[i] != t[j]) {
                if (foundDiff) return false;  // 第二次不同，超过1步
                foundDiff = true;
                if (ns == nt) {
                    // 替换：两个指针都前进
                    i++; j++;
                } else {
                    // 插入：只有 t 的指针前进（跳过 t 中多出的字符）
                    j++;
                }
            } else {
                i++; j++;
            }
        }
        
        // 如果到这里没发现不同，那 t 必须恰好多一个尾部字符
        return foundDiff || (nt - ns == 1);
    }
};
```

**复杂度分析**：
- 时间 O(n)
- 空间 O(1)：只用了几个变量

## 易错点

1. **忘记两个相同字符串应返回 false**
```
✗ 错误：s == t 时返回 true
  // 编辑距离为 0，不是 1

✓ 正确：s == t 时返回 false（最后 return nt - ns == 1 自动处理）
```

2. **忘记两个空字符串应返回 false**
```
✗ 错误：没有处理 s="" t="" 的情况
✓ 正确：nt - ns == 0，for 循环不执行，return 0 == 1 → false
```

3. **等长场景忘记检查"剩余部分"**
```
✗ 错误：发现第一个不同就返回 true
  // s="abc" t="adc" 没问题，但 s="abc" t="ade" 应该 false

✓ 正确：必须检查分歧点之后的所有字符都相同
```

## 面试追问

**Q1: 这题和经典编辑距离（LC 72）有什么区别？**
A: LC 72 求最少编辑次数（DP, O(mn)），本题只判断是否恰好为 1。利用"恰好为 1"这个约束可以 O(n) 解决。

**Q2: 如果要求判断编辑距离 <= k 呢？**
A: k 较小时可以用"对角线 DP"在 O(nk) 时间解决。k=1 就退化成本题的 O(n)。

**Q3: 如何处理 Unicode 字符？**
A: 逻辑不变，但要注意 C++ 中 std::string 按字节处理，多字节字符需要用 wstring 或按码点遍历。

## 相关题型
- [72. 编辑距离](../0072_edit_distance/) - 经典 DP 求最小编辑距离
- [583. 两个字符串的删除操作](../0583_delete_operation_for_two_strings/) - 删除操作版的编辑距离
- [712. 两个字符串的最小ASCII删除和](../0712_minimum_ascii_delete_sum_for_two_strings/) - 带权重的编辑距离变体
