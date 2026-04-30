# 423. Reconstruct Original Digits from English - 从英文中重建数字

## 核心思路
利用每个数字英文单词中的"独特字母"来确定各数字的出现次数。某些字母只在特定数字中出现，可以直接确定该数字的个数。

## 思维链
1. 统计字符串中每个字母的频率
2. 找出独特字母对应的数字（第一轮直接确定）：
   - `z` → 0 (zero)，`w` → 2 (two)，`u` → 4 (four)，`x` → 6 (six)，`g` → 8 (eight)
3. 扣除已确定数字的字母后，找第二轮独特字母：
   - `o` → 1 (one)，`h` → 3 (three)，`f` → 5 (five)，`s` → 7 (seven)
4. 最后剩余的 `i` → 9 (nine)
5. 按升序拼接结果

## 解法概览表

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 解法1 | 独特字母推断 ⭐ | O(n) | O(1) | 首选 |

## 关键提示
1. 关键洞察：`z` 只出现在 zero，`w` 只出现在 two，`u` 只出现在 four，`x` 只出现在 six，`g` 只出现在 eight
2. 确定顺序很重要：先确定有独特字母的数字，扣除后再确定剩余数字
3. 结果需要按数字升序排列

## 解法详解

### 解法1: 独特字母推断
```cpp
class Solution {
public:
    string originalDigits(string s) {
        vector<int> freq(26, 0);
        for (char c : s) freq[c - 'a']++;
        
        vector<int> count(10, 0);
        // 第一轮：独特字母直接确定
        count[0] = freq['z' - 'a'];  // zero
        count[2] = freq['w' - 'a'];  // two
        count[4] = freq['u' - 'a'];  // four
        count[6] = freq['x' - 'a'];  // six
        count[8] = freq['g' - 'a'];  // eight
        
        // 第二轮：扣除后的独特字母
        count[1] = freq['o' - 'a'] - count[0] - count[2] - count[4]; // one
        count[3] = freq['h' - 'a'] - count[8];  // three
        count[5] = freq['f' - 'a'] - count[4];  // five
        count[7] = freq['s' - 'a'] - count[6];  // seven
        
        // 第三轮：最后剩余
        count[9] = freq['i' - 'a'] - count[5] - count[6] - count[8]; // nine
        
        string result;
        for (int i = 0; i <= 9; i++)
            result += string(count[i], '0' + i);
        return result;
    }
};
```

## 易错点
| 错误写法 | 正确写法 |
|---------|---------|
| ✗ 用 `o` 直接确定 1（o也出现在0,2,4中） | ✓ 先确定0,2,4再用 o 的剩余量确定 1 |
| ✗ 用 `i` 直接确定 9（i也出现在5,6,8中） | ✓ 9 放到最后一轮，扣除5,6,8后的 i |
| ✗ 不按顺序处理导致扣除关系错误 | ✓ 严格按 0,2,4,6,8 → 1,3,5,7 → 9 的顺序 |

## 面试追问
**Q1: 为什么不直接用贪心逐个匹配完整单词？**
> 逐个匹配需要考虑匹配顺序问题（某些单词的字母完全包含在其他单词中），而独特字母法直接用数学关系一步到位。

**Q2: 如何验证输入一定合法？**
> 题目保证合法，但如果要验证，最终 freq 数组中所有元素应该都被消耗为 0。

**Q3: 如果数字范围扩大到 0-99，思路如何变化？**
> 可以先识别 0-9 的个位数字，再组合成两位数。但两位数的英文表示更复杂（如 twenty-one），需要重新分析独特字母。

## 相关题型
- [383. Ransom Note](https://leetcode.com/problems/ransom-note/) - 字母频率匹配
- [451. Sort Characters By Frequency](https://leetcode.com/problems/sort-characters-by-frequency/) - 频率统计
