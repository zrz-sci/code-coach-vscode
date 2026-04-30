# 393. UTF-8 Validation - UTF-8 编码验证

## 核心思路
按照UTF-8编码规则逐字节验证：先通过首字节的高位前缀确定字符字节数，再验证后续字节是否都以`10`开头。

## 思维链
1. 读首字节 -> 数高位连续1的个数确定n（1-4字节字符）
2. n=0: 单字节ASCII（0xxxxxxx），合法
3. n=1: 非法（10xxxxxx是续字节，不能作为首字节）
4. n>=5: 非法（UTF-8最多4字节）
5. 验证接下来的n-1个字节是否都以10开头
6. 循环直到处理完所有字节

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 位运算逐字节验证 | O(n) | O(1) | 直接按规则检查 |
| 位掩码简化版 | O(n) | O(1) | 用掩码代替循环数1 |

## 关键提示
1. **只用低8位**：`data[i] & 0xFF` 或直接用，题目保证 0 <= data[i] <= 255
2. **首字节判断**：用掩码 `(byte >> 5) == 0b110` 判断2字节，`(byte >> 4) == 0b1110` 判断3字节等
3. **续字节验证**：`(byte >> 6) == 0b10` 即 `(byte & 0xC0) == 0x80`
4. 处理完一个字符后继续处理下一个，直到所有字节消耗完

## 解法详解

### 解法一：位运算逐字节验证（推荐）
```cpp
class Solution {
public:
    bool validUtf8(vector<int>& data) {
        int remaining = 0; // 剩余需要的续字节数
        for (int byte : data) {
            if (remaining > 0) {
                // 必须是续字节 10xxxxxx
                if ((byte >> 6) != 0b10) return false;
                remaining--;
            } else {
                // 首字节，判断字符长度
                if ((byte >> 7) == 0)       remaining = 0; // 1字节
                else if ((byte >> 5) == 0b110)  remaining = 1; // 2字节
                else if ((byte >> 4) == 0b1110) remaining = 2; // 3字节
                else if ((byte >> 3) == 0b11110) remaining = 3; // 4字节
                else return false; // 非法首字节
            }
        }
        return remaining == 0; // 所有字符完整
    }
};
```

### 解法二：计数前导1
```cpp
class Solution {
public:
    bool validUtf8(vector<int>& data) {
        int i = 0, n = data.size();
        while (i < n) {
            int ones = countLeadingOnes(data[i]);
            if (ones == 0) { i++; continue; }
            if (ones == 1 || ones > 4) return false;
            // 检查后续 ones-1 个续字节
            for (int j = 1; j < ones; j++) {
                if (i + j >= n) return false;
                if ((data[i + j] >> 6) != 0b10) return false;
            }
            i += ones;
        }
        return true;
    }
private:
    int countLeadingOnes(int byte) {
        int count = 0;
        for (int mask = 0x80; mask > 0 && (byte & mask); mask >>= 1)
            count++;
        return count;
    }
};
```

## 易错点
- ✗ 忘记处理数组末尾不完整的多字节字符 -> 必须检查 `remaining == 0`
- ✓ 循环结束后验证没有未完成的续字节

- ✗ 认为首字节5个以上连续1也合法
- ✓ UTF-8最多4字节，首字节最多4个连续1

- ✗ 没有取低8位就直接做位运算
- ✓ 题目保证0~255，但习惯上可 `byte & 0xFF`

## 面试追问

**Q1: 为什么UTF-8最多4字节而不是更多？**
> UTF-8最初设计支持6字节，但Unicode码点最大U+10FFFF只需要21位，4字节的11110xxx 10xxxxxx 10xxxxxx 10xxxxxx恰好能编码21位。RFC 3629限制了最大4字节。

**Q2: 如何区分UTF-8和ASCII？**
> ASCII是UTF-8的子集。所有ASCII字符（0-127）在UTF-8中用单字节0xxxxxxx表示，完全兼容。反过来，合法的UTF-8不一定是纯ASCII。

**Q3: 如果输入是字节流（逐字节到达），如何做在线验证？**
> 维护一个状态机，状态为"还需要多少续字节"。每到达一个字节，要么开始新字符（首字节），要么消耗一个续字节。和解法一的remaining变量本质相同。

## 相关题型
- [68. Text Justification](https://leetcode.com/problems/text-justification/) - 字符串格式化
- [271. Encode and Decode Strings](https://leetcode.com/problems/encode-and-decode-strings/) - 编码解码
