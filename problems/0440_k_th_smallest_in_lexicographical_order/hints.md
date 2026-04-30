# 440. 字典序的第K小数字 (K-th Smallest in Lexicographical Order)

## 核心思路
将 1~n 的数字按字典序排列等价于一棵 **十叉前缀树 (Trie)**。从根节点 1 出发，每次决定是"跳过当前子树（向右兄弟移动）"还是"进入子树（向下移动）"，关键在于高效计算每棵子树包含的节点数。

## 思维链
1. 字典序 = 十叉 Trie 的前序遍历
2. 要找第 k 个节点，不需要遍历整棵树
3. 计算以 prefix 为根的子树在 [1, n] 中有多少个数
4. 若子树节点数 <= 剩余 k，跳过此子树，prefix++
5. 否则进入子树，prefix *= 10，k--
6. 重复直到 k == 0

## 解法概览表

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| ⭐ 解法1: 十叉Trie计数 | 逐层计算子树大小，跳/进 | O(log²n) | O(1) | 首选 |

## 关键提示
1. **countSteps(n, prefix)**: 计算以 prefix 为前缀的数在 [1,n] 中有多少个
2. 用 `first = prefix, last = prefix` 逐层扩展：`first *= 10, last = last*10 + 9`
3. 每层贡献 `min(n+1, last+1) - first` 个节点
4. 注意用 `long` 防溢出（n 最大 10^9）

## 解法详解

### 解法1: 十叉Trie计数 ⭐
```cpp
class Solution {
public:
    int findKthNumber(int n, int k) {
        int curr = 1;
        k--; // curr=1 已经是第1个
        while (k > 0) {
            long steps = countSteps(n, curr);
            if (steps <= k) {
                // 跳过当前子树，移到右兄弟
                k -= steps;
                curr++;
            } else {
                // 进入子树
                k--;
                curr *= 10;
            }
        }
        return curr;
    }
    
    long countSteps(int n, long prefix) {
        long steps = 0;
        long first = prefix, last = prefix;
        while (first <= n) {
            steps += min((long)n + 1, last + 1) - first;
            first *= 10;
            last = last * 10 + 9;
        }
        return steps;
    }
};
```

## 易错点
| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `int first, last` | `long first, last` | prefix*10 可能溢出 int |
| `min(n, last+1) - first` | `min((long)n+1, last+1) - first` | n+1 才是开区间上界 |
| 忘记 `k--` 初始化 | `k--` 在起点 curr=1 时 | curr=1 本身占一个位置 |
| 进入子树后忘记 `k--` | 进入子树时 `k--; curr*=10` | 进入子树消耗当前节点 |

## 面试追问

**Q1: 为什么用 long 而不是 int？**
> prefix 和 last 在逐层乘 10 时可能超过 int 范围（n 最大 10^9，乘 10 后为 10^10），必须用 long。

**Q2: 时间复杂度为什么是 O(log²n)？**
> 外层最多移动 O(logn) 次（每次 prefix++ 或 prefix*=10），每次 countSteps 内部循环 O(logn) 层，总计 O(log²n)。

**Q3: 能否用这个方法解决 "字典序第 K 大" 的问题？**
> 可以。字典序第 K 大等价于第 (n-K+1) 小，或者从 prefix=9 反向遍历，但逻辑更复杂。正向转换更简单。

## 相关题型
- [386. 字典序排数](https://leetcode.com/problems/lexicographical-numbers/) - Trie前序遍历
- [1415. 长度为 n 的开心字符串中字典序第 k 小的字符串](https://leetcode.com/problems/the-k-th-lexicographical-string-of-all-happy-strings-of-length-n/)
