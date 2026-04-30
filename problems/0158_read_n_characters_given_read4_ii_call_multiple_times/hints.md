# 158. 用 Read4 读取 N 个字符 II - 多次调用

## 核心思路
本质是在 read 可能被多次调用的情况下，用 read4 实现 read(n)，关键在于维护一个内部缓冲区保存上次 read4 多读但未消费的字符。

## 思维链
1. **回顾157题**: 单次调用只需循环 read4 + 截断。但多次调用时，上次多读的字符会丢失
2. **问题**: 第一次 read(buf,1) 调用 read4 读了 "abcd"，只用了 "a"，"bcd" 怎么办？
3. **关键设计**: 用成员变量保存缓冲区 + 当前读取位置 + 缓冲区有效字符数
4. **每次 read**: 先消费缓冲区剩余字符，不够再调 read4 补充
5. **状态管理**: bufIdx(下次从buf4的哪个位置开始读), bufCount(buf4中有多少有效字符)

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 内部缓冲区 + 状态管理 | 成员变量保存 buf4 剩余 | O(n) | O(1) | ⭐ 必知 |

## 关键提示
1. **三个成员变量**: `char buf4[4]`(缓冲区), `int bufIdx`(下次读取位置), `int bufCount`(有效字符数)
2. **消费优先**: 每次 read 先从 buf4[bufIdx] 消费剩余字符，再决定是否调 read4
3. **buf4 用完的标志**: bufIdx >= bufCount，此时需要调 read4 重新填充
4. **文件结束标志**: read4 返回 0，此时不再调用 read4

```
示例: file = "abcde", 调用序列: read(buf,2), read(buf,3)

初始状态: buf4 = "", bufIdx = 0, bufCount = 0

--- read(buf, 2) ---
  buf4 空，调 read4 → buf4 = "abcd", bufCount = 4, bufIdx = 0
  从 buf4 取2个: buf = "ab", bufIdx = 2
  返回 2

状态: buf4 = "abcd", bufIdx = 2, bufCount = 4
       (buf4[2]='c', buf4[3]='d' 还没用)

--- read(buf, 3) ---
  buf4 还有 4-2=2 个字符: 取 'c','d' → buf = "cd", bufIdx = 4
  还差1个，调 read4 → buf4 = "e___", bufCount = 1, bufIdx = 0
  从 buf4 取1个: buf = "cde", bufIdx = 1
  返回 3
```

## 解法详解

### 解法1: 内部缓冲区 + 状态管理

**思路**: 用成员变量持久化 read4 的缓冲区状态，每次 read 先消费缓冲区，不够再补充。

```cpp
class Solution {
private:
    char buf4[4];      // read4 的内部缓冲区
    int bufIdx = 0;    // buf4 中下一个待读位置
    int bufCount = 0;  // buf4 中有效字符数
    
public:
    int read(char *buf, int n) {
        int total = 0;
        
        while (total < n) {
            // 如果缓冲区已经用完，调 read4 补充
            if (bufIdx >= bufCount) {
                bufCount = read4(buf4);
                bufIdx = 0;
                if (bufCount == 0) break; // 文件结束
            }
            
            // 从缓冲区取字符
            while (total < n && bufIdx < bufCount) {
                buf[total++] = buf4[bufIdx++];
            }
        }
        
        return total;
    }
};
```

**关键点**:
- buf4, bufIdx, bufCount 是成员变量，跨 read 调用保持状态
- 先检查缓冲区是否有剩余，有就先消费
- 只有缓冲区空了才调 read4，减少系统调用
- bufCount == 0 表示文件已读完

### 解法1 的另一种写法 (更紧凑)

```cpp
class Solution {
private:
    char buf4[4];
    int bufIdx = 0;
    int bufCount = 0;
    
public:
    int read(char *buf, int n) {
        int total = 0;
        
        while (total < n) {
            // 缓冲区为空时补充
            if (bufIdx == bufCount) {
                bufCount = read4(buf4);
                bufIdx = 0;
            }
            // 无数据可读，文件结束
            if (bufCount == 0) break;
            
            // 逐字符拷贝
            buf[total++] = buf4[bufIdx++];
        }
        
        return total;
    }
};
```

## 易错点

1. **成员变量没有初始化**
```
✗ int bufIdx;     // 未初始化，值不确定
  int bufCount;
✓ int bufIdx = 0;  // 初始化为0
  int bufCount = 0;
```

2. **每次 read 重置了成员变量**
```
✗ int read(char *buf, int n) {
      bufIdx = 0; bufCount = 0;  // 错! 清掉了上次的剩余数据
      ...
  }
✓ int read(char *buf, int n) {
      // 不重置! 接着上次继续
      ...
  }
```

3. **调用 read4 后忘记重置 bufIdx**
```
✗ bufCount = read4(buf4);   // bufIdx 还是旧值
✓ bufCount = read4(buf4);
  bufIdx = 0;               // 新数据从 buf4[0] 开始
```

4. **157和158混淆 -- 把buf4声明为局部变量**
```
✗ int read(char *buf, int n) {
      char buf4[4]; // 局部变量，下次调用时数据丢失!
      ...
  }
✓ // buf4 是成员变量，跨调用保持
  char buf4[4]; // 声明在类里
```

## 面试追问

**Q1**: 这题和157题的核心区别是什么？
→ 157题 read 只调一次，buf4 可以是局部变量。158题 read 多次调用，必须用成员变量保存 buf4 中"已读但未消费"的字符。

**Q2**: 如果要设计成线程安全的，需要注意什么？
→ buf4, bufIdx, bufCount 是共享可变状态。多线程下需要加锁保护(如 mutex)，或者使用线程局部存储(thread_local)。

**Q3**: 如果 read4 的网络延迟很高，如何优化？
→ 可以预读: 每次 read4 时多调几次缓存更多数据(增大内部缓冲区)。类似 BufferedReader 的设计，用更大的缓冲区减少系统调用次数。

## 相关题型
- [157. 用 Read4 读取 N 个字符](../0157_read_n_characters_given_read4/) — 单次调用版本，不需要状态管理
- [系统设计] BufferedReader/BufferedInputStream — 同样的"内部缓冲"设计模式
