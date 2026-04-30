# 247. Strobogrammatic Number II (中心对称数 II)

## 核心思路

生成所有长度为n的中心对称数，本质上是**由内向外递归构建**。基础情况是 n=0 (空串) 和 n=1 (0/1/8)，每次递归在已有结果的两端添加对称数字对 (0,0), (1,1), (6,9), (8,8), (9,6)。注意最外层不能添加 (0,0) 以避免前导零。

## 思维链

1. **识别对称对**: 可以在两端同时添加的数字对为 (0,0), (1,1), (6,9), (8,8), (9,6)
2. **递归基**: n=0 返回 `[""]`, n=1 返回 `["0","1","8"]`
3. **递推关系**: n位中心对称数 = 在 (n-2)位中心对称数两端各加一对对称数字
4. **前导零处理**: 当当前层是最外层(n等于目标长度)时, 不能添加 (0,0)
5. **参数设计**: 递归函数需要两个参数 `n`(当前层) 和 `target`(目标长度)
6. **收集结果**: 递归返回所有有效组合

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 递归由内向外构建 | O(5^(n/2)) | O(5^(n/2)) | ⭐⭐⭐⭐⭐ |
| 迭代 BFS 式构建 | O(5^(n/2)) | O(5^(n/2)) | ⭐⭐⭐⭐ |

## 关键提示

- 递归方向是**从内向外**，不是从外向内
- `n` 和 `target` 两个参数缺一不可: `n` 控制递归层数，`target` 控制是否允许前导零
- n=0 和 n=1 是两个不同的 base case
- 递归每层 n 减少 2 (两端各加一个字符)
- 结果数量: 约 4 * 5^((n-2)/2) * 3(奇数) 或 4 * 5^((n-2)/2)(偶数)

## 解法详解

### 解法1: 递归由内向外构建 (推荐)

从最内层开始，每层在两端添加对称数字对，直到达到目标长度。

```cpp
class Solution {
public:
    vector<string> findStrobogrammatic(int n) {
        return helper(n, n);
    }
    
    vector<string> helper(int n, int target) {
        if (n == 0) return {""};
        if (n == 1) return {"0", "1", "8"};
        
        vector<string> middles = helper(n - 2, target);
        vector<string> result;
        
        for (const string& mid : middles) {
            // 最外层不能加前导零 (除非 target == 1, 即数字本身就是 "0")
            if (n != target) {
                result.push_back("0" + mid + "0");
            }
            result.push_back("1" + mid + "1");
            result.push_back("6" + mid + "9");
            result.push_back("8" + mid + "8");
            result.push_back("9" + mid + "6");
        }
        return result;
    }
};
```

**复杂度**: 时间 O(5^(n/2)), 空间 O(5^(n/2))

### 解法2: 迭代 BFS 式构建

从中心开始，像 BFS 一样逐层向外扩展。

```cpp
class Solution {
public:
    vector<string> findStrobogrammatic(int n) {
        // 根据 n 的奇偶性确定初始层
        vector<string> cur;
        if (n % 2 == 1) {
            cur = {"0", "1", "8"};
        } else {
            cur = {""};
        }
        
        vector<pair<char,char>> pairs = {{'0','0'}, {'1','1'}, {'6','9'}, {'8','8'}, {'9','6'}};
        
        // 从内向外, 每次长度+2, 直到达到n
        for (int len = (n % 2 == 1) ? 3 : 2; len <= n; len += 2) {
            vector<string> next;
            for (const string& s : cur) {
                for (auto& [a, b] : pairs) {
                    // 最外层 (len == n) 跳过前导零
                    if (len == n && a == '0') continue;
                    next.push_back(a + s + b);
                }
            }
            cur = next;
        }
        return cur;
    }
};
```

**复杂度**: 时间 O(5^(n/2)), 空间 O(5^(n/2))

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 只传 `n` 一个参数 | ✓ 传 `n` 和 `target` | 无法区分最外层和内层, 无法判断前导零 |
| ✗ n==0 返回空vector | ✓ n==0 返回 `{""}` | 空串是有效的递归基, 否则无法向外扩展 |
| ✗ 最外层也加 "0" + mid + "0" | ✓ 最外层跳过 (0,0) 对 | "069" 不是有效数字 (前导零) |
| ✗ 每层 n 减 1 | ✓ 每层 n 减 2 | 两端各加一个字符, 长度增加2 |

## 面试追问

**Q1: 如果 n 很大导致结果集爆炸, 如何优化?**
> 改为按需生成 (iterator/generator 模式), 不一次性存储所有结果。

**Q2: 如何在生成过程中直接过滤出 [low, high] 范围内的数? (引出 LC 248)**
> 生成后逐一比较，或在生成时剪枝: 如果当前前缀已经超出 high 或当前后缀已经小于 low，提前终止。

**Q3: 递归深度是多少? 会不会栈溢出?**
> 递归深度为 n/2, n 最大14, 深度最大7, 完全不会栈溢出。

## 相关题型

- [246. Strobogrammatic Number](../0246_strobogrammatic_number/) - 判断是否为中心对称数
- [248. Strobogrammatic Number III](../0248_strobogrammatic_number_iii/) - 统计范围内中心对称数个数
- [89. Gray Code](https://leetcode.com/problems/gray-code/) - 类似递归构建序列
- [22. Generate Parentheses](https://leetcode.com/problems/generate-parentheses/) - 递归生成有效组合
