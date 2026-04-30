# 412. Fizz Buzz

## 核心思路
根据数字是否能被 3 和/或 5 整除，输出对应字符串。关键在于**判断顺序**：先判15（同时整除3和5），再判3，再判5，最后输出数字本身。

## 思维链
1. 遍历 1 到 n
2. 对每个数检查整除条件
3. 优先级：15 > 3 > 5 > 其他
4. 也可用字符串拼接法避免硬编码优先级

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 直接判断 | O(n) | O(1) | 最直观，面试首选 |
| 字符串拼接 | O(n) | O(1) | 可扩展性强，适合 follow-up |

## 关键提示
1. 注意下标从 **1** 开始，不是 0
2. 字符串拼接法：先拼 "Fizz"，再拼 "Buzz"，空则转数字
3. Follow-up 常问：如果新增 7 -> "Jazz" 怎么办？字符串拼接法可以轻松扩展

## 解法详解

### 解法一：直接条件判断（推荐）
```cpp
class Solution {
public:
    vector<string> fizzBuzz(int n) {
        vector<string> res;
        for (int i = 1; i <= n; i++) {
            if (i % 15 == 0) res.push_back("FizzBuzz");
            else if (i % 3 == 0) res.push_back("Fizz");
            else if (i % 5 == 0) res.push_back("Buzz");
            else res.push_back(to_string(i));
        }
        return res;
    }
};
```

### 解法二：字符串拼接（可扩展）
```cpp
class Solution {
public:
    vector<string> fizzBuzz(int n) {
        vector<string> res;
        for (int i = 1; i <= n; i++) {
            string s;
            if (i % 3 == 0) s += "Fizz";
            if (i % 5 == 0) s += "Buzz";
            if (s.empty()) s = to_string(i);
            res.push_back(s);
        }
        return res;
    }
};
```

## 易错点
- ✗ 从 0 开始遍历 → ✓ 从 **1** 开始，题目是 1-indexed
- ✗ 先判断 3 和 5 再判断 15 → ✓ 先判断 15 或使用字符串拼接法
- ✗ `i % 3 == 0 && i % 5 == 0` 放在最后 → ✓ 必须放在最前面（if-else 结构）

## 面试追问
**Q1: 如果新增规则（如 7 → "Jazz"），你会怎么修改？**
A: 使用字符串拼接法 + 映射表 `vector<pair<int, string>>`，遍历映射表拼接字符串，扩展性 O(1) 开发成本。

**Q2: 能否不用取模运算？**
A: 可以用计数器，维护 fizz_count 和 buzz_count，每次递增，到 3/5 时重置。取模运算虽然常数开销略大，但在现代 CPU 上差异可忽略。

**Q3: 如何并行化处理大规模 n？**
A: 每个数的判断彼此独立，可以直接分段并行处理。预分配结果数组，每个线程填充自己的区间。

## 相关题型
- [202. Happy Number](https://leetcode.com/problems/happy-number/) - 数学模拟
- [263. Ugly Number](https://leetcode.com/problems/ugly-number/) - 整除判断
