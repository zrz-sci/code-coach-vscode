# 421. 数组中两个数的最大异或值 (Maximum XOR of Two Numbers in an Array)

## 难度: Medium | 标签: Bit Manipulation, Trie, Array, Hash Table

---

## 核心思路

给定整数数组 `nums`，求 `nums[i] XOR nums[j]` 的最大值。暴力 O(n^2) 枚举所有对，但可以用**字典树 (Trie)** 在 O(n * 32) 时间内解决：将所有数的二进制表示插入 Trie，然后对每个数在 Trie 中贪心查找，尽量走相反方向使每一位异或结果为 1。

## 字典树结构图

```
将 nums = [3, 10, 5, 25, 2, 8] 的低5位插入Trie（实际是32位，这里简化展示）:

  3  = 00011
  10 = 01010
  5  = 00101
  25 = 11001
  2  = 00010
  8  = 01000

                 root
                /    \
              [0]    [1]
             /   \      \
           [0]   [1]    [1]
           / \    |       \
         [0] [1] [0]     [0]
         / \   \   \       \
       [1] [0] [0] [1]    [0]
        |   |   |   |      |
       [0] [1] [1] [0]    [1]
        |   |   |   |      |
       (2) (5) (8) (10)   (25)

查询 5 (00101) 在 Trie 中找最大 XOR:
  bit4=0: 想走1(有!) → 走[1], curXor |= (1<<4) = 10000
  bit3=0: 想走1(有!) → 走[1], curXor |= (1<<3) = 11000
  bit2=1: 想走0(有!) → 走[0], curXor |= (1<<2) = 11100
  bit1=0: 想走1(无)  → 走[0], curXor 不变 = 11100
  bit0=1: 想走0(有!) → 走[0], curXor |= (1<<0) = 11101

  curXor = 11101 = 28, 对应 5 XOR 25 = 28 ✓
```

## 思维链

1. **读完题第一反应** → 暴力枚举所有 (i,j) 对，O(n^2)，n 可达 2*10^5，会超时。
2. **XOR 的性质** → 要最大化 XOR，应该从**最高位**开始，尽量让每一位都是 1。这提示了贪心策略。
3. **Trie 思路** → 把每个数字看作 32 位二进制串，从高位到低位插入 Trie（每个节点只有 0 和 1 两个子节点）。查询时，对于当前数的每一位，尽量走**相反方向**的分支（这样异或结果为 1）。
4. **为什么从高位开始？** → 高位为 1 的贡献远大于低位。比如 `10000` (16) > `01111` (15)。所以贪心从最高位开始是正确的。
5. **替代方案：哈希表** → 从最高位到最低位，逐位确定答案。假设当前答案前缀为 `prefix`，想在当前位取 1，即 `prefix | (1 << bit)`。利用 XOR 的性质 `a ^ b = c` ⟺ `a ^ c = b`，检查是否存在两个数的前缀 XOR 能产生这个 `prefix`。

## 字典树详解

```
Trie 的结构：

  struct TrieNode {
      children[0]  →  指向 "当前位为 0" 的子节点
      children[1]  →  指向 "当前位为 1" 的子节点
  }

  深度: 32 层（int 有 32 位）
  从 bit 31（最高位）到 bit 0（最低位）

  插入数字 25 (11001):
    root → children[1] → children[1] → children[0] → children[0] → children[1]
    bit31  bit30  ...  bit4=1       bit3=1       bit2=0       bit1=0       bit0=1

  查询时对数字 5 (00101):
    bit4=0: 想让 XOR 为 1, 走 children[1] (相反方向)
    bit3=0: 想让 XOR 为 1, 走 children[1] (相反方向)
    bit2=1: 想让 XOR 为 1, 走 children[0] (相反方向)
    bit1=0: 想让 XOR 为 1, 走 children[1]... 如果不存在, 只能走 children[0]
    bit0=1: 想让 XOR 为 1, 走 children[0] (相反方向)
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| Trie（字典树） | 二进制 Trie + 贪心查找 | O(32n) | O(32n) | ⭐ 面试首选 |
| 哈希表（逐位确定） | 前缀集合 + XOR 验证 | O(32n) | O(n) | 替代方案 |
| 暴力枚举 | 枚举所有对 | O(n^2) | O(1) | 仅用于对拍 |

## 关键提示

1. **Trie 的深度是 31（0~30）或 32（0~31）**：对于 32 位 int，从 bit 31 开始到 bit 0。但由于 `nums[i] <= 2^31 - 1`，最高位（bit 31，符号位）总是 0，可以从 bit 30 开始。不过从 bit 31 开始也不影响正确性。
2. **贪心方向**：查询时，对当前数的第 `i` 位 `bit`，想走 `1 - bit`（相反方向），这样异或结果为 1，贡献 `(1 << i)` 到答案中。如果相反方向不存在，只能走 `bit` 方向，该位异或为 0。
3. **哈希表方法的核心**：利用 `a ^ b = c ⟹ a ^ c = b`。用一个集合存所有数的前缀，然后检查是否存在两个前缀 XOR 能产生期望的最大前缀值。
4. **内存管理**：Trie 节点用 new 分配，面试中可以不关心释放。如果要优化，可以用数组模拟 Trie（预分配 `32 * n` 个节点）。

## 解法详解

### 解法1: Trie（字典树） — O(32n) / O(32n) ⭐

**完整过程**:
```
nums = [3, 10, 5, 25, 2, 8]

Step 1: 所有数插入 Trie（32位二进制，从高位到低位）
Step 2: 对每个数在 Trie 中贪心查找最大 XOR

  查询 num=5 (二进制 ...00101):
    bit 31~5: 全是 0, 想走 1 方向。如果有就走（找到25的路径），没有就走 0
    bit 4: 5 的 bit4 = 0, 想走 1 → 走到 25 的路径
    bit 3: 5 的 bit3 = 0, 想走 1 → 走到 25 的路径
    bit 2: 5 的 bit2 = 1, 想走 0 → 走到 25 的路径
    bit 1: 5 的 bit1 = 0, 想走 1 → 25 的 bit1=0, 走 0
    bit 0: 5 的 bit0 = 1, 想走 0 → 25 的 bit0=1, 走 1 (但XOR=0)
    
  最终 curXor = 28 = 5 XOR 25

  遍历所有数, maxXor = max(所有 curXor) = 28
```

```cpp
class Solution {
    struct TrieNode {
        TrieNode* children[2] = {nullptr, nullptr};
    };
public:
    int findMaximumXOR(vector<int>& nums) {
        TrieNode* root = new TrieNode();
        for (int num : nums) {
            TrieNode* node = root;
            for (int i = 31; i >= 0; i--) {
                int bit = (num >> i) & 1;
                if (!node->children[bit])
                    node->children[bit] = new TrieNode();
                node = node->children[bit];
            }
        }
        int maxXor = 0;
        for (int num : nums) {
            TrieNode* node = root;
            int curXor = 0;
            for (int i = 31; i >= 0; i--) {
                int bit = (num >> i) & 1;
                int want = 1 - bit;
                if (node->children[want]) {
                    curXor |= (1 << i);
                    node = node->children[want];
                } else {
                    node = node->children[bit];
                }
            }
            maxXor = max(maxXor, curXor);
        }
        return maxXor;
    }
};
```

---

### 解法2: 哈希表（逐位确定答案） — O(32n) / O(n)

**核心思想**：从最高位到最低位，逐位确定答案。在每一位上，假设答案能取 1，然后验证。

```
nums = [3(011), 10(1010), 5(101), 25(11001), 2(10), 8(1000)]

从 bit 4 开始（最高有效位）:
  prefixes = {0, 0, 0, 1, 0, 0} (取 bit4)
  尝试 answer = 10000: 对每个 p, 查 p ^ 10000 是否在集合中
    1 ^ 10000 = 10001? 不在 → 但 0 ^ 10000 = 10000? 不在
    ... 只有 prefix 1 存在，但 1^10000=10001 不在
    → answer 保持 0

  bit 3: prefixes = {00, 01, 00, 11, 00, 01}
  尝试 answer = 01000: 查集合...
    01 ^ 01000 = 01001? 不在; 11 ^ 01000 = 10011? 不在
    → answer 保持 0

  ... 最终 answer = 28
```

---

## 易错点

1. **Trie 从高位到低位构建**：必须从 bit 31（或 30）开始，不是从 bit 0。从低位开始会导致贪心方向错误。
2. **查询时 node 可能为空**：如果 `children[want]` 不存在就只能走 `children[bit]`，但 `children[bit]` 也可能不存在吗？不会——因为至少有当前数自己在 Trie 中，所以 `children[bit]` 一定存在。
3. **溢出问题**：`nums[i]` 可达 2^31-1，用 int 没问题。但如果用 `1 << 31` 会在 C++ 中造成未定义行为，用 `1 << 30` 或从 bit 30 开始更安全。

## 面试追问递进链

1. **"Trie 的空间优化？"** → 数组模拟 Trie：预分配 `int trie[32*n][2]`，用索引代替指针，避免 new/delete。
2. **"如果数组有重复元素？"** → 不影响。Trie 中相同路径会合并，查询结果不变。两个相同数 XOR=0。
3. **"如果要找最大 XOR 子数组？"** → [1707. Maximum XOR With an Element From Array](../1707_maximum_xor_with_an_element_from_array/)，在 Trie 基础上增加约束条件。
4. **"哈希表方法的正确性证明？"** → 利用 XOR 的自反性：`a^b=c ⟹ a=b^c`。如果能从集合中找到某个 `a` 使得 `a ^ candidate` 也在集合中，则 `a ^ (a ^ candidate) = candidate` 可行。

## 复杂度分析

| 解法 | 时间 | 空间 | 说明 |
|------|------|------|------|
| Trie | O(32n) = O(n) | O(32n) = O(n) | 每个数 32 位，插入+查询各一遍 |
| 哈希表 | O(32n) = O(n) | O(n) | 32 轮，每轮遍历一次 |
| 暴力 | O(n^2) | O(1) | 枚举所有对 |

## 相关题型

- [1707. Maximum XOR With an Element From Array](../1707_maximum_xor_with_an_element_from_array/) — Trie + 离线排序
- [1938. Maximum Genetic Difference Query](../1938_maximum_genetic_difference_query/) — Trie + DFS
- [208. Implement Trie](../0208_implement_trie_prefix_tree/) — Trie 基础实现
- [1268. Search Suggestions System](../1268_search_suggestions_system/) — Trie 实际应用
