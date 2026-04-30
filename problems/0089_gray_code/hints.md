# 89. 格雷编码

## 核心思路
本质是构造一个满足相邻元素只差一位的排列，利用格雷编码的数学公式 `i ^ (i >> 1)` 或镜像反射法递归构造。

## 思维链
1. **理解约束**：2^n 个数，相邻（含首尾）二进制恰好差一位
2. **暴力回溯**：用 visited 集合 + 回溯搜索所有合法排列，但 O(2^n!) 太慢
3. **发现规律**：n=1 为 [0,1]，n=2 可以在 n=1 的基础上"镜像"并加高位 → [0,1,3,2]
4. **镜像法**：将前一轮序列倒序遍历，每个数加上 `1 << (n-1)`，追加到结果末尾
5. **公式法**：格雷编码第 i 个数就是 `i ^ (i >> 1)`，直接 O(2^n) 生成
6. **最终方案**：公式法最简洁，镜像法更直观且面试中推导过程加分

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 镜像反射法 | 递推：每轮镜像翻转+加高位 | O(2^n) | O(2^n) | ⭐ 推荐 |
| 位运算公式 | i ^ (i >> 1) | O(2^n) | O(2^n) | ⭐ 必知 |
| 回溯法 | DFS搜索合法排列 | O(2^n * 2^n) | O(2^n) | 了解即可 |

## 关键提示
1. **镜像反射核心原理**：n 位格雷码 = n-1 位格雷码前面加 0 + n-1 位格雷码逆序前面加 1
2. **公式推导**：格雷编码 G(i) = i XOR (i >> 1)，因为相邻 i 和 i+1 的二进制恰好在最低变化位处不同
3. **验证方法**：检查相邻两数 XOR 结果是否为 2 的幂（只有一个 bit 为 1）

```
镜像反射法示意（n=3）：

n=1:  0, 1
      ↓ 镜像翻转 + 加高位 100
n=2:  00, 01, 11, 10
      ↓ 镜像翻转 + 加高位 100
n=3:  000, 001, 011, 010, 110, 111, 101, 100
```

## 解法详解

### 解法1：镜像反射法

**思考过程**：
- n=1 的格雷码是 [0, 1]
- 构造 n 位格雷码时，先把 n-1 位的序列保持不动（高位补 0），再将 n-1 位序列倒序遍历，每个数加上 `1 << (n-1)`（高位补 1）
- 这保证了镜像接缝处只差最高位

```cpp
class Solution {
public:
    vector<int> grayCode(int n) {
        vector<int> result;
        result.push_back(0);
        
        // 逐位构建：第 i 轮处理第 i 位
        for (int i = 0; i < n; i++) {
            int size = result.size();
            // 镜像：从末尾向前遍历，加上新的高位
            for (int j = size - 1; j >= 0; j--) {
                result.push_back(result[j] | (1 << i));
            }
        }
        
        return result;
    }
};
```

**关键点**：
- 每轮结束后序列大小翻倍
- 镜像保证接缝处（原序列最后一个和翻转后第一个相同）只在新高位不同

### 解法2：位运算公式法

**思考过程**：
- 格雷编码有数学公式：第 i 个格雷码 = i XOR (i >> 1)
- 这个公式保证相邻两个编码恰好差一位
- 直接循环 0 到 2^n - 1 即可

```cpp
class Solution {
public:
    vector<int> grayCode(int n) {
        vector<int> result;
        int total = 1 << n;  // 2^n
        
        for (int i = 0; i < total; i++) {
            // 格雷编码公式：G(i) = i ^ (i >> 1)
            result.push_back(i ^ (i >> 1));
        }
        
        return result;
    }
};
```

**关键点**：
- 为什么公式成立？因为 i 和 i+1 的二进制在最低变化位 k 处：i 的第 k 位为 0，i+1 的第 k 位为 1，右移一位后 XOR 的结果恰好只在第 k 位不同
- 时间和空间都是 O(2^n)，代码极简

### 解法3：回溯法

```cpp
class Solution {
public:
    vector<int> grayCode(int n) {
        vector<int> result;
        int total = 1 << n;
        vector<bool> visited(total, false);
        
        result.push_back(0);
        visited[0] = true;
        backtrack(result, visited, n, total);
        
        return result;
    }
    
private:
    bool backtrack(vector<int>& result, vector<bool>& visited, int n, int total) {
        if ((int)result.size() == total) {
            // 检查首尾是否只差一位
            int diff = result.front() ^ result.back();
            return (diff & (diff - 1)) == 0;
        }
        
        int last = result.back();
        for (int i = 0; i < n; i++) {
            int next = last ^ (1 << i);  // 翻转第 i 位
            if (!visited[next]) {
                visited[next] = true;
                result.push_back(next);
                if (backtrack(result, visited, n, total)) return true;
                result.pop_back();
                visited[next] = false;
            }
        }
        return false;
    }
};
```

## 易错点

1. **镜像法遍历方向错误**
```cpp
// ✗ 错误：正序遍历不是镜像
for (int j = 0; j < size; j++) {
    result.push_back(result[j] | (1 << i));
}

// ✓ 正确：必须逆序遍历才能保证接缝处只差一位
for (int j = size - 1; j >= 0; j--) {
    result.push_back(result[j] | (1 << i));
}
```

2. **公式法的移位优先级**
```cpp
// ✗ 错误：运算符优先级问题
result.push_back(i ^ i >> 1);  // 等价于 i ^ (i >> 1)? 实际上是，但可读性差

// ✓ 正确：显式加括号
result.push_back(i ^ (i >> 1));
```

3. **total 计算溢出**
```cpp
// ✗ 潜在问题：n=16 时 1 << 16 = 65536，int 可以容纳但要注意
int total = 1 << n;

// ✓ 安全写法
int total = 1 << n;  // n <= 16，int 足够
```

## 面试追问

**Q1: 请解释镜像法为什么能保证首尾也只差一位？**
→ 因为每轮构造后，最后一个元素是 `result[0] | (1 << i)`，而第一个元素是 `result[0]` = 0。最终首元素为 0，尾元素为 `1 << (n-1)`，它们只在最高位不同。

**Q2: 如果要求输出格雷码的逆映射（给定格雷码求原始二进制），怎么做？**
→ 逆格雷码：`b[i] = g[n-1] ^ g[n-2] ^ ... ^ g[i]`，可以用循环 XOR 实现：`num = 0; while(gray) { num ^= gray; gray >>= 1; }`

**Q3: 如何验证一个序列是否是合法的格雷码序列？**
→ 检查三个条件：(1) 长度为 2^n (2) 所有元素互不相同且在 [0, 2^n-1] 内 (3) 相邻元素（含首尾）XOR 结果是 2 的幂

## 相关题型
- [1238. Circular Permutation in Binary Representation](https://leetcode.com/problems/circular-permutation-in-binary-representation/) - 格雷码变体，指定起始值
- [191. Number of 1 Bits](https://leetcode.com/problems/number-of-1-bits/) - 位运算基础
- [136. Single Number](https://leetcode.com/problems/single-number/) - XOR 技巧复用
