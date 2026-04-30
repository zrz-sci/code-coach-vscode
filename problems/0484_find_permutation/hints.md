# 484. 寻找排列 (Find Permutation)

## 核心思路

根据 'I'/'D' 序列构造字典序最小的排列。贪心策略：遇到 'I' 时尽量放最小的数，遇到连续 'D' 段时将该段反转。
核心观察：先按升序排列 [1,2,...,n]，再将每个连续 'D' 段对应的子数组反转。

## 思维链

1. **初始化升序排列**：先构造 [1, 2, 3, ..., n]，这是字典序最小的基础
2. **定位连续 D 段**：找到每个连续 'D' 的起始和结束位置
3. **反转 D 段**：对每个连续 D 段，将对应的子数组 [i, j+1] 反转
4. **正确性验证**：反转后，D 段变为递减，I 处保持递增，满足约束
5. **字典序最优性**：因为初始是升序，每次只反转必须的最小范围，保证结果字典序最小

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 升序+反转D段 | O(n) | O(1) 额外 | ⭐ 最优解，直观 |
| 栈模拟 | O(n) | O(n) | 通用思路 |
| 双指针原地 | O(n) | O(1) 额外 | 一次遍历 |

## 关键提示

1. 排列长度 = s.length() + 1（n 个数，n-1 个关系）
2. 反转区间是 [D段起始, D段结束+1]，因为 D 影响的是位置 i 和 i+1
3. 栈解法：遇到 'I' 或到末尾时弹出栈中所有元素
4. 两种解法时间都是 O(n)，反转法空间更优

## 解法详解

### 解法1: 升序初始化 + 反转 D 段

```cpp
class Solution1 {
public:
    vector<int> findPermutation(string s) {
        int n = s.size() + 1;
        vector<int> res(n);
        // 初始化为升序 [1, 2, ..., n]
        for (int i = 0; i < n; i++) res[i] = i + 1;
        
        // 找到每段连续 D，反转对应区间
        int i = 0;
        while (i < (int)s.size()) {
            if (s[i] == 'D') {
                int j = i;
                while (j < (int)s.size() && s[j] == 'D') j++;
                // 反转 res[i..j] (闭区间)
                reverse(res.begin() + i, res.begin() + j + 1);
                i = j;
            } else {
                i++;
            }
        }
        return res;
    }
};
```

### 解法2: 栈模拟

```cpp
class Solution2 {
public:
    vector<int> findPermutation(string s) {
        int n = s.size() + 1;
        vector<int> res;
        stack<int> stk;
        
        for (int i = 1; i <= n; i++) {
            stk.push(i);
            // 遇到 'I' 或到最后一个数时，弹出栈中所有元素
            if (i == n || s[i - 1] == 'I') {
                while (!stk.empty()) {
                    res.push_back(stk.top());
                    stk.pop();
                }
            }
        }
        return res;
    }
};
```

### 解法3: 双指针原地构造

```cpp
class Solution3 {
public:
    vector<int> findPermutation(string s) {
        int n = s.size() + 1;
        vector<int> res(n);
        int num = 1;
        int i = 0;
        while (i < n) {
            if (i == (int)s.size() || s[i] == 'I') {
                res[i] = num++;
                i++;
            } else {
                int j = i;
                while (j < (int)s.size() && s[j] == 'D') j++;
                for (int k = j; k >= i; k--) {
                    res[k] = num++;
                }
                i = j + 1;
            }
        }
        return res;
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 原因 |
|-----------|-----------|------|
| `n = s.size()` | `n = s.size() + 1` | 排列有 n 个数，字符串有 n-1 个字符 |
| 反转区间 `[i, j)` | 反转区间 `[i, j+1)` | D 段 s[i..j-1] 影响 res[i..j] 共 j-i+1 个位置 |
| 只反转 D 不考虑边界 | 结尾也要处理栈中剩余元素 | 字符串末尾如果是 D，需要把剩余数弹出 |
| 从 0 开始推数 | 数的范围是 [1, n] | 题目要求排列 [1..n] |

## 面试追问

**Q1: 为什么升序初始化再反转 D 段就是字典序最小的？**
A: 升序 [1,2,...,n] 本身就是字典序最小的排列。我们只在必须递减的位置做最小范围的反转，不会引入不必要的大数。反转一段连续 D 会把最小的数尽量放前面。

**Q2: 栈解法的直觉是什么？**
A: 按顺序把 1,2,3,... 压入栈。遇到 'D' 就继续压（积攒递减序列），遇到 'I' 或结束时弹出所有（后进先出产生递减序列）。这样数在 D 段内反序输出，在 I 处正序推进。

**Q3: 如果要求字典序最大的排列呢？**
A: 改为初始化降序 [n, n-1, ..., 1]，然后反转每个连续 'I' 段。或者用栈从 n 到 1 压入，遇到 'D' 或结束时弹出。

## 相关题型

- [31. 下一个排列](../0031_next_permutation/) - 排列操作
- [46. 全排列](../0046_permutations/) - 排列枚举
- [942. 增减字符串匹配](../0942_di_string_match/) - 类似 DI 模式
- [60. 排列序列](../0060_permutation_sequence/) - 第 k 个排列
