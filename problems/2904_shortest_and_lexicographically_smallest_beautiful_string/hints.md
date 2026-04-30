# 2904. 最短且字典序最小的美丽子字符串 (Shortest and Lexicographically Smallest Beautiful String)

## 难度: Medium
## 标签: String, Sliding Window

---

## 题目理解

给定一个二进制字符串 s 和正整数 k。如果子字符串中 '1' 的个数恰好为 k，则称该子串为**美丽子串**。

要求：
1. 先找到所有美丽子串中**最短的长度** len
2. 在所有长度为 len 的美丽子串中，找**字典序最小**的那个
3. 如果不存在美丽子串(即 s 中 '1' 的个数少于 k)，返回空字符串

关键观察：
- 最短的美丽子串一定以 '1' 开头且以 '1' 结尾（否则去掉头尾的 0 可以更短）
- s = "100011001", k = 3: 最短长度为 5，字典序最小的是 "11001"
- s = "1011", k = 2: 最短长度为 2，字典序最小的是 "11"
- s = "000", k = 1: 没有美丽子串，返回 ""

---

## 核心思路

主流有两种解法：

1. **滑动窗口法**: 用变长滑动窗口找恰好包含 k 个 '1' 的子串，同时尽可能收缩窗口使之最短，在所有最短候选中选字典序最小的。

2. **枚举第 1 个和第 k 个 '1' 的位置法**: 预处理所有 '1' 的位置，然后枚举每对 (第 i 个 '1', 第 i+k-1 个 '1')，子串从第 i 个 '1' 的位置开始，到第 i+k-1 个 '1' 的位置结束。

两者都需要在等长候选中比较字典序。

---

## 思维链 (从观察到解法)

### 第一步: 最短美丽子串的特征

一个包含恰好 k 个 '1' 的最短子串，一定：
- 以 '1' 开头（否则去掉前导 0 更短且仍然有 k 个 '1'）
- 以 '1' 结尾（否则去掉尾部 0 更短且仍然有 k 个 '1'）

所以最短子串 = 从某个 '1' 开始，到之后第 k 个 '1' 结束。

### 第二步: 预处理 '1' 的位置

```
s = "100011001", k = 3
'1' 的位置: [0, 4, 5, 8]

枚举起止:
  位置 0 到位置 5 -> "100011" (长度 6)
  位置 4 到位置 8 -> "11001"  (长度 5) <- 更短!
```

### 第三步: 找最短长度

最短长度 = min(pos[i+k-1] - pos[i] + 1)，其中 i 遍历所有可能。

### 第四步: 在等长候选中选字典序最小

如果有多个最短子串，比较它们的字典序。

**字典序比较的直觉**: 对于二进制串，字典序小意味着前面的 '1' 尽可能多（因为 '1' > '0'）。
等等，实际上 '0' < '1'，所以字典序小意味着前面的 '0' 尽可能多？
不对！这些子串都以 '1' 开头（因为是最短的），所以第一个字符都是 '1'。
区别在于第二个字符及之后：中间 '0' 越少字典序越大（如 "110" > "101"）。

实际上直接用字符串比较即可。

---

## 解法表
| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| Solution1: 滑动窗口 | O(n^2) | O(n) | 经典窗口写法 |
| Solution2: 预处理 '1' 位置 | O(n^2) | O(n) | 直接枚举候选 |
| Solution3: 暴力枚举 | O(n^3) | O(n) | 最简单但最慢 |

(注: 由于 n <= 100，三种方法都能通过)

---

## 解法详解

### Solution1: 滑动窗口

**算法**:
1. 右指针 right 向右扩展，计数 '1' 的个数
2. 当 '1' 个数 == k 时，收缩左端：去掉前导 '0' 使窗口最短
3. 收缩后的窗口就是一个候选美丽子串
4. 比较所有候选，取最短且字典序最小的

**收缩策略的细节**:
- 去掉左端的 '0' 不会减少 '1' 的数量，可以放心去掉
- 但不能去掉 '1'，否则 '1' 数量变成 k-1
- 如果 ones > k，则需要去掉左端的 '1' 直到 ones == k

```
s = "100011001", k = 3
right=0: '1', ones=1
right=1: '0', ones=1
...
right=5: '1', ones=3 -> 窗口 [0,5]="100011"
  收缩: left=0 是 '1'，不能去(ones会变2)
  候选: "100011" (长度6)
right=6: '0', ones=3 -> 窗口 [0,6]="1000110"
  收缩: left=0 是 '1'，不能去
  候选: "1000110" (长度7，比之前长，跳过)
...
right=8: '1', ones=4 -> ones > k!
  收缩: 去掉 left=0 的 '1', ones=3, left=1
  继续收缩: left=1是'0', 去掉, left=2
  继续: left=2是'0', 去掉, left=3
  继续: left=3是'0', 去掉, left=4
  left=4是'1', 不能去(ones会变2)
  候选: "11001" (长度5) <- 更短!
```

```cpp
class Solution1 {
public:
    string shortestBeautifulSubstring(string s, int k) {
        int n = s.size();
        int ones = 0;
        string result = "";

        for (int left = 0, right = 0; right < n; right++) {
            if (s[right] == '1') ones++;

            // 收缩左端: 去掉前导 0，或 ones > k 时去掉 '1'
            while (ones > k || (left <= right && s[left] == '0')) {
                if (s[left] == '1') {
                    if (ones == k) break; // 不能再去掉 '1' 了
                    ones--;
                }
                left++;
            }

            if (ones == k) {
                string candidate = s.substr(left, right - left + 1);
                if (result.empty() ||
                    candidate.size() < result.size() ||
                    (candidate.size() == result.size() && candidate < result)) {
                    result = candidate;
                }
            }
        }
        return result;
    }
};
```

---

### Solution2: 预处理 '1' 位置

**算法**:
1. 收集所有 '1' 的位置到数组 pos
2. 如果 pos.size() < k，返回 ""
3. 枚举每对 (pos[i], pos[i+k-1])，子串长度 = pos[i+k-1] - pos[i] + 1
4. 找最短长度，在最短中选字典序最小

```cpp
class Solution2 {
public:
    string shortestBeautifulSubstring(string s, int k) {
        // 收集 '1' 的位置
        vector<int> pos;
        for (int i = 0; i < (int)s.size(); i++) {
            if (s[i] == '1') pos.push_back(i);
        }

        // 不够 k 个 '1'
        if ((int)pos.size() < k) return "";

        string result = "";
        int minLen = INT_MAX;

        // 枚举: 第 i 个 '1' 作为起点，第 i+k-1 个 '1' 作为终点
        for (int i = 0; i + k - 1 < (int)pos.size(); i++) {
            int start = pos[i];
            int end = pos[i + k - 1];
            int len = end - start + 1;

            if (len < minLen) {
                minLen = len;
                result = s.substr(start, len);
            } else if (len == minLen) {
                string candidate = s.substr(start, len);
                if (candidate < result) {
                    result = candidate;
                }
            }
        }
        return result;
    }
};
```

---

### Solution3: 暴力枚举所有子串

**算法**: 枚举所有 (i, j) 对，检查 s[i..j] 中 '1' 的数量是否为 k。

```cpp
class Solution3 {
public:
    string shortestBeautifulSubstring(string s, int k) {
        int n = s.size();
        string result = "";

        for (int i = 0; i < n; i++) {
            int ones = 0;
            for (int j = i; j < n; j++) {
                if (s[j] == '1') ones++;
                if (ones == k) {
                    string candidate = s.substr(i, j - i + 1);
                    if (result.empty() ||
                        candidate.size() < result.size() ||
                        (candidate.size() == result.size() && candidate < result)) {
                        result = candidate;
                    }
                    break; // 对于固定 i，找到第一个恰好 k 个 1 就是最短的
                }
            }
        }
        return result;
    }
};
```

---

## 图解示例

```
s = "100011001", k = 3

'1' 的位置: [0, 4, 5, 8]

枚举 (Solution2):
  i=0: pos[0]=0, pos[2]=5 -> s[0..5] = "100011" (长度 6)
  i=1: pos[1]=4, pos[3]=8 -> s[4..8] = "11001"  (长度 5) <- 最短!

最短长度 = 5
唯一候选: "11001"
答案: "11001"
```

```
s = "1011", k = 2

'1' 的位置: [0, 2, 3]

枚举 (Solution2):
  i=0: pos[0]=0, pos[1]=2 -> s[0..2] = "101" (长度 3)
  i=1: pos[1]=2, pos[2]=3 -> s[2..3] = "11"  (长度 2) <- 最短!

最短长度 = 2
唯一候选: "11"
答案: "11"
```

---

## 滑动窗口的收缩过程详解

```
s = "100011001", k = 3

right=0: ones=1, 窗口[0,0]="1"
right=1: ones=1, 窗口[0,1]="10"
right=2: ones=1, 窗口[0,2]="100"
right=3: ones=1, 窗口[0,3]="1000"
right=4: ones=2, 窗口[0,4]="10001"
right=5: ones=3, 窗口[0,5]="100011"
  收缩: s[0]='1', ones==k, 不能去 -> 停止
  候选 "100011", result="100011"

right=6: ones=3, 窗口[0,6]="1000110"
  收缩: s[0]='1', ones==k, 不能去
  候选 "1000110" (len=7 > 6, 跳过)

right=7: ones=3, 窗口[0,7]="10001100"
  收缩: s[0]='1', ones==k, 不能去
  候选 "10001100" (len=8 > 6, 跳过)

right=8: ones=4 > k!
  收缩: s[0]='1', ones>k, 去掉! ones=3, left=1
  继续: s[1]='0', 去掉, left=2
  继续: s[2]='0', 去掉, left=3
  继续: s[3]='0', 去掉, left=4
  s[4]='1', ones==k, 不能去 -> 停止
  候选 "11001" (len=5 < 6), result="11001"

最终答案: "11001"
```

---

## 易错点

1. **'1' 的总数不足 k**: 需要提前检查，返回 ""
2. **收缩窗口时的条件**: ones > k 时才能去掉 '1'，ones == k 时不能再去
3. **字典序比较**: 只在长度相同时比较字典序，不是所有候选都比
4. **暴力法的优化**: 对固定起点 i，找到第 k 个 '1' 后可以 break (不需要继续扩展)
5. **最短子串以 '1' 开头和结尾**: 这是一个重要的不变量，可用于剪枝

---

## 面试追问递进链

1. **"如何证明最短子串一定以 '1' 开头和结尾？"**
   -> 如果以 '0' 开头/结尾，去掉它不改变 '1' 的数量但长度更短，矛盾

2. **"能否做到 O(n) 时间？"**
   -> 预处理 '1' 位置后，找最短间距是 O(m) 的(m 为 '1' 的数量)
   -> 但字典序比较最坏 O(n)，所以总体 O(n * m) 或 O(n^2)

3. **"如果要求恰好 k 个 '0' 呢？"**
   -> 类似思路，用滑动窗口维护 '0' 的计数

4. **"如果字符串是非二进制的(如小写字母)，要求恰好 k 个特定字符？"**
   -> 完全相同的滑动窗口框架，只改计数条件

5. **"如果需要返回所有最短美丽子串？"**
   -> 收集所有长度等于 minLen 的美丽子串到列表中

---

## 相关题型

- [76. Minimum Window Substring](../0076_minimum_window_substring/) - 滑动窗口经典题
- [209. Minimum Size Subarray Sum](../0209_minimum_size_subarray_sum/) - 最短子数组+滑动窗口
- [3. Longest Substring Without Repeating Characters](../0003_longest_substring_without_repeating_characters/) - 滑动窗口
- [1297. Maximum Number of Occurrences of a Substring](../1297_maximum_number_of_occurrences_of_a_substring/) - 子串计数
