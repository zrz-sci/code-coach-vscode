# 157. 用 Read4 读取 N 个字符

## 核心思路
本质是用一个每次只能读4字节的底层API来实现读取任意n字节的功能，关键在于处理"读多了"的情况(read4返回4但只需要部分字符)。

## 思维链
1. **理解接口**: read4 每次读4个字符到 buf4，返回实际读取数。我们要用它实现 read(buf, n)
2. **循环调用**: 反复调用 read4，每次拿到最多4个字符，拷贝到 buf 中
3. **终止条件**: 两种情况停止 — (a) 已读够n个字符 (b) read4返回 < 4(文件结束)
4. **最后一批**: 最后一次 read4 可能读到多余字符(比如只需要2个但读了4个)，取 min 截断

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 循环调用 read4 | 每次读4个，按需拷贝 | O(n) | O(1) | ⭐ 必知 |

## 关键提示
1. **buf4 是临时缓冲区**: 每次 read4 的结果先写入 buf4，再从 buf4 拷贝到 buf
2. **每次拷贝字符数 = min(read4返回值, 剩余需要的字符数)**
3. **read4 返回 0 或 < 4 表示文件已到末尾**
4. **本题 read 只会被调用一次**，不需要维护状态(对比158题)

```
示例: file = "abcde", n = 4

第1次 read4(buf4): 返回4, buf4 = "abcd"
  需要: min(4, 4-0) = 4 个字符
  拷贝 "abcd" → buf, total = 4

已读够 n=4 个字符，停止
返回 4

示例: file = "abc", n = 4

第1次 read4(buf4): 返回3, buf4 = "abc"
  需要: min(3, 4-0) = 3 个字符
  拷贝 "abc" → buf, total = 3

read4 返回 < 4，文件结束，停止
返回 3
```

## 解法详解

### 解法1: 循环调用 read4

**思路**: 用 while 循环反复调用 read4，每次从 buf4 拷贝所需字符到 buf，直到读够 n 个或文件结束。

```cpp
// 假设 read4 API 已定义
class Solution {
public:
    int read(char *buf, int n) {
        int total = 0;       // 已读字符总数
        char buf4[4];        // read4 的临时缓冲区
        bool eof = false;    // 文件是否结束
        
        while (total < n && !eof) {
            int count = read4(buf4);  // 调用底层API
            if (count < 4) eof = true; // 文件不足4字符，到末尾了
            
            // 实际需要的字符数: 不能超过 n - total
            int toRead = min(count, n - total);
            for (int i = 0; i < toRead; i++) {
                buf[total++] = buf4[i];
            }
        }
        return total;
    }
};
```

**关键点**:
- `min(count, n - total)` 防止读多了(比如n=5，第二次read4读了4个，但只需要1个)
- `count < 4` 检测文件结束
- buf4 是局部变量，每次 read4 都覆盖

### 解法1 的简化版本

```cpp
class Solution {
public:
    int read(char *buf, int n) {
        int total = 0;
        char buf4[4];
        
        while (total < n) {
            int count = read4(buf4);
            int toRead = min(count, n - total);
            for (int i = 0; i < toRead; i++) {
                buf[total++] = buf4[i];
            }
            if (count < 4) break; // 文件结束
        }
        return total;
    }
};
```

## 易错点

1. **忘记截断多余字符**
```
✗ for (int i = 0; i < count; i++)      // 可能超过n个字符
      buf[total++] = buf4[i];
✓ int toRead = min(count, n - total);   // 截断到恰好n个
  for (int i = 0; i < toRead; i++)
      buf[total++] = buf4[i];
```

2. **没有检查文件结束导致死循环**
```
✗ while (total < n) {                   // 文件只有3字符但n=100，死循环
      read4(buf4);
      ...
  }
✓ while (total < n) {
      int count = read4(buf4);
      ...
      if (count < 4) break;             // 文件结束，退出
  }
```

3. **buf4 定义在循环外但以为每次自动清空**
```
✗ // buf4 不会自动清零，read4 返回 count 后只有 buf4[0..count-1] 有效
  // 不要访问 buf4[count] 及之后的位置
✓ 只访问 buf4[0..count-1]，由 toRead = min(count, ...) 保证
```

## 面试追问

**Q1**: 如果 read 可能被多次调用怎么办？(LC 158)
→ 需要保存上次 read4 多读的字符。用成员变量 buf4[4]、bufIdx、bufCount 记录"剩余缓冲"，下次 read 时先消费缓冲再调 read4。

**Q2**: 如果底层API是 read_k(每次读k个字符)，你的代码需要改动吗？
→ 只需把 buf4[4] 改成 bufK[k]，阈值 4 改成 k，逻辑完全不变。

**Q3**: 如何测试这个函数？
→ 关键边界: n=0, 文件为空, n大于文件长度, n恰好是4的倍数, n不是4的倍数。

## 相关题型
- [158. 用 Read4 读取 N 个字符 II](../0158_read_n_characters_given_read4_ii_call_multiple_times/) — 多次调用版本，需维护缓冲区状态
