# 284. Peeking Iterator (窥视迭代器)

**难度**: Medium | **标签**: Design, Array, Iterator

## 核心思路

本质是**缓存预读**设计模式。在现有迭代器上封装一层，用一个变量缓存下一个值来实现 `peek()`——即"偷看"下一个元素但不移动指针。这是装饰器模式的经典应用。

## 思维链

1. `peek()` 需要返回下一个元素但不推进迭代器
2. 但底层 `Iterator::next()` 一旦调用就会推进
3. 解决方案：提前调用底层 `next()` 把值缓存起来
4. 用一个 `bool hasPeeked` 标志 + `int peekedVal` 缓存值
5. `peek()`: 如果未缓存，调用底层 next() 缓存；返回缓存值
6. `next()`: 如果有缓存，返回缓存值并清除标志；否则调用底层 next()
7. `hasNext()`: 有缓存则 true，否则看底层 hasNext()

```
状态转换图:

初始: hasPeeked=false, Iterator->[1,2,3]

peek() 调用:
  hasPeeked=false -> 调用 Iterator::next() -> 得到 1
  -> hasPeeked=true, peekedVal=1
  -> 返回 1

next() 调用:
  hasPeeked=true -> 返回 peekedVal=1
  -> hasPeeked=false
  -> 返回 1

next() 调用:
  hasPeeked=false -> 调用 Iterator::next()
  -> 返回 2

hasNext() 调用:
  hasPeeked=false -> 返回 Iterator::hasNext() -> true
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 解法1: 缓存标志法 | bool标志 + 缓存值 | O(1) per call | O(1) | ⭐⭐⭐ |
| 解法2: 构造时预读 | 始终保持一个预读值 | O(1) per call | O(1) | ⭐⭐ |

## 关键提示

```
缓存预读状态机:

                 +--------+
     peek()      |        |  next() [返回缓存]
   +------------>| CACHED |----------+
   |  [缓存值]   |        |          |
   |             +--------+          |
   |                                 v
+--------+                      +--------+
| NORMAL |<---------------------| NORMAL |
+--------+   清除缓存            +--------+
   |                                 |
   | next() [直接调底层]              | peek() [调底层缓存]
   +---> 返回 Iterator::next()       +---> 缓存 -> CACHED
```

## 解法详解

### 解法1: 缓存标志法 (推荐)

```cpp
class PeekingIterator : public Iterator {
    bool hasPeeked;
    int peekedVal;
public:
    PeekingIterator(const vector<int>& nums) : Iterator(nums) {
        hasPeeked = false;
    }

    int peek() {
        if (!hasPeeked) {
            peekedVal = Iterator::next();
            hasPeeked = true;
        }
        return peekedVal;
    }

    int next() {
        if (hasPeeked) {
            hasPeeked = false;
            return peekedVal;
        }
        return Iterator::next();
    }

    bool hasNext() const {
        return hasPeeked || Iterator::hasNext();
    }
};
```

### 解法2: 构造时预读

```cpp
class PeekingIterator : public Iterator {
    int nextVal;
    bool hasNextVal;
public:
    PeekingIterator(const vector<int>& nums) : Iterator(nums) {
        // 构造时就预读第一个值
        hasNextVal = Iterator::hasNext();
        if (hasNextVal) nextVal = Iterator::next();
    }

    int peek() {
        return nextVal;
    }

    int next() {
        int ret = nextVal;
        // 预读下一个
        hasNextVal = Iterator::hasNext();
        if (hasNextVal) nextVal = Iterator::next();
        return ret;
    }

    bool hasNext() const {
        return hasNextVal;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|----------|----------|------|
| ✗ `peek()` 每次都调底层 next() | ✓ 只在未缓存时调用一次 | 多次 peek 应返回相同值 |
| ✗ `hasNext()` 只看底层 | ✓ `hasPeeked \|\| Iterator::hasNext()` | 缓存中可能还有元素 |
| ✗ `next()` 不检查缓存 | ✓ 先检查 hasPeeked | 否则会跳过已 peek 的元素 |
| ✗ 直接拷贝 nums 操作 | ✓ 只用 Iterator 接口 | 题目要求不能直接操作 nums |

## 面试追问

**Q1: 如何扩展为泛型 PeekingIterator<T>?**
-> C++ 中用模板类 `template<typename T> class PeekingIterator`，缓存值类型改为 `T`，用 `optional<T>` 代替 bool+val。

**Q2: 这个设计模式的名称是什么？**
-> 装饰器模式(Decorator Pattern)。PeekingIterator 装饰了 Iterator，在不修改原接口的基础上增加了 peek 功能。

**Q3: 如果底层迭代器的 next() 有副作用(如网络请求)，这个设计有什么隐患？**
-> peek() 会提前触发 next() 的副作用。可以考虑延迟执行(lazy evaluation)或记录副作用状态。

## 相关题型

- [281. Zigzag Iterator](https://leetcode.com/problems/zigzag-iterator/) - 锯齿迭代器
- [251. Flatten 2D Vector](https://leetcode.com/problems/flatten-2d-vector/) - 展平二维向量
- [341. Flatten Nested List Iterator](https://leetcode.com/problems/flatten-nested-list-iterator/) - 嵌套列表迭代器
- [173. Binary Search Tree Iterator](https://leetcode.com/problems/binary-search-tree-iterator/) - BST 迭代器
