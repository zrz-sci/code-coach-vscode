# 248. Strobogrammatic Number III (中心对称数 III)

## 核心思路

统计 [low, high] 范围内中心对称数的个数。本质是在 LC 247 的基础上加上**范围过滤**: 枚举 low.length 到 high.length 之间所有可能长度，对每个长度生成所有中心对称数，对边界长度进行额外的大小比较过滤。

## 思维链

1. **复用 LC 247**: 用递归/迭代方法生成指定长度的所有中心对称数
2. **按长度枚举**: 从 `low.length()` 到 `high.length()` 逐一枚举
3. **中间长度无需边界检查**: 如果长度严格介于 low 和 high 长度之间，所有该长度的中心对称数都在范围内
4. **边界长度需要比较**: 长度等于 low.length 或 high.length 时，需要字符串大小比较
5. **字符串比较技巧**: 同长度字符串可以直接用 `<=` 比较（字典序等价于数值序）
6. **累加计数返回结果**

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 枚举长度 + 生成 + 过滤 | O(5^(n/2)) | O(5^(n/2)) | ⭐⭐⭐⭐⭐ |
| 枚举长度 + 数学计数 + 边界生成 | O(5^(n/2)) 边界 | O(5^(n/2)) 边界 | ⭐⭐⭐⭐ |

## 关键提示

- 同长度的字符串可以直接用 `>=` 和 `<=` 做字典序比较，等价于数值比较
- 中间长度 (len > low.size() && len < high.size()) 的所有中心对称数直接全部计入
- 注意 low 和 high 长度相同的特殊情况：同时满足 `>= low` 和 `<= high`
- 生成时注意前导零: 长度 > 1 时不能有前导零

## 解法详解

### 解法1: 枚举长度 + 生成 + 过滤 (推荐)

对每个可能的长度生成所有中心对称数，对边界长度进行范围过滤。

```cpp
class Solution {
public:
    int strobogrammaticInRange(string low, string high) {
        int count = 0;
        for (int len = low.size(); len <= (int)high.size(); len++) {
            vector<string> nums = generate(len, len);
            for (const string& num : nums) {
                // 同长度字符串字典序比较等价于数值比较
                if ((len > (int)low.size() || num >= low) &&
                    (len < (int)high.size() || num <= high)) {
                    count++;
                }
            }
        }
        return count;
    }
    
private:
    vector<string> generate(int n, int target) {
        if (n == 0) return {""};
        if (n == 1) return {"0", "1", "8"};
        
        vector<string> middles = generate(n - 2, target);
        vector<string> result;
        
        for (const string& mid : middles) {
            if (n != target) result.push_back("0" + mid + "0");
            result.push_back("1" + mid + "1");
            result.push_back("6" + mid + "9");
            result.push_back("8" + mid + "8");
            result.push_back("9" + mid + "6");
        }
        return result;
    }
};
```

**复杂度**: 时间 O(5^(n/2) * L), 空间 O(5^(n/2)), 其中 L 为最大长度

### 解法2: 分层计数优化

对中间长度用数学公式直接计算个数，只对边界长度生成具体数字。

```cpp
class Solution {
public:
    int strobogrammaticInRange(string low, string high) {
        int count = 0;
        int lo = low.size(), hi = high.size();
        
        for (int len = lo; len <= hi; len++) {
            if (len > lo && len < hi) {
                // 中间长度: 用公式直接计算
                // n位中心对称数个数: 偶数长度 4*5^(n/2-1), 奇数长度 4*3*5^((n-3)/2)
                // 但 n=1 特殊处理为 3
                count += countStrobo(len);
            } else {
                // 边界长度: 生成后过滤
                vector<string> nums = generate(len, len);
                for (const string& num : nums) {
                    if ((len > lo || num >= low) && (len < hi || num <= high)) {
                        count++;
                    }
                }
            }
        }
        return count;
    }
    
private:
    int countStrobo(int n) {
        if (n == 1) return 3;
        int base = 4; // 最外层 4 种选择 (1,6,8,9)
        int layers = n / 2 - 1; // 中间层数
        int mid = (n % 2 == 1) ? 3 : 1; // 奇数中心有 3 种选择
        int result = base * mid;
        for (int i = 0; i < layers; i++) result *= 5;
        return result;
    }
    
    vector<string> generate(int n, int target) {
        if (n == 0) return {""};
        if (n == 1) return {"0", "1", "8"};
        vector<string> middles = generate(n - 2, target);
        vector<string> result;
        for (const string& mid : middles) {
            if (n != target) result.push_back("0" + mid + "0");
            result.push_back("1" + mid + "1");
            result.push_back("6" + mid + "9");
            result.push_back("8" + mid + "8");
            result.push_back("9" + mid + "6");
        }
        return result;
    }
};
```

**复杂度**: 中间长度 O(1) 计算, 边界长度 O(5^(n/2))

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 用 `stoi/stol` 转数值比较 | ✓ 用字符串字典序比较 | 数值可达 10^15, 超出 int/long 范围 |
| ✗ 只检查 `num >= low` | ✓ 需要同时检查长度条件 | 不同长度的字符串字典序不等价于数值序 |
| ✗ 忘记 low.size() == high.size() 的情况 | ✓ 条件用 OR 组合覆盖所有case | 此时同一个数需同时满足两个边界 |
| ✗ `int len` 与 `size_t` 比较 | ✓ 转为 `(int)low.size()` | 有符号/无符号比较可能出错 |

## 面试追问

**Q1: 字符串比较为什么等价于数值比较?**
> 仅在同长度且无前导零时成立。同长度数字字符串的字典序与数值序一致。

**Q2: 当 low 和 high 长度差距很大时如何优化?**
> 对中间长度用公式 O(1) 计算个数，不生成具体数字。仅对边界两个长度生成并过滤。

**Q3: 如果要返回具体的数字列表而不是个数呢?**
> 在过滤阶段将满足条件的数字存入结果列表即可。

## 相关题型

- [246. Strobogrammatic Number](../0246_strobogrammatic_number/) - 判断是否为中心对称数
- [247. Strobogrammatic Number II](../0247_strobogrammatic_number_ii/) - 生成所有n位中心对称数
- [233. Number of Digit One](https://leetcode.com/problems/number-of-digit-one/) - 范围内数字计数
- [357. Count Numbers with Unique Digits](https://leetcode.com/problems/count-numbers-with-unique-digits/) - 数位计数问题
