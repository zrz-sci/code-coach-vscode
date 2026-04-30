# 281. Zigzag Iterator (锯齿迭代器)

**难度**: Medium | **标签**: Design, Queue, Array, Iterator

## 核心思路

本质是**多路归并的轮询调度**问题。将多个向量的迭代器放入队列中，每次从队头取一个迭代器读取元素，若该迭代器未耗尽则重新放回队尾，实现交替遍历。

## 思维链

1. 最直观的想法：用一个标志位在两个向量间切换 -> 但扩展到 k 个向量时不优雅
2. 更通用的做法：用队列管理所有"未耗尽"的迭代器
3. 每次 `next()` 从队头弹出一个迭代器，读取元素，如果还有剩余就放回队尾
4. `hasNext()` 只需检查队列是否为空

```
迭代器调度过程 (v1=[1,2], v2=[3,4,5,6]):

Queue: [it1(v1), it2(v2)]

next() -> pop it1 -> 读 1 -> it1还有 -> push it1
Queue: [it2(v2), it1(v1)]

next() -> pop it2 -> 读 3 -> it2还有 -> push it2
Queue: [it1(v1), it2(v2)]

next() -> pop it1 -> 读 2 -> it1耗尽 -> 不push
Queue: [it2(v2)]

next() -> pop it2 -> 读 4 -> it2还有 -> push it2
next() -> pop it2 -> 读 5 -> it2还有 -> push it2
next() -> pop it2 -> 读 6 -> it2耗尽 -> 不push
Queue: []  -> hasNext() = false
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 解法1: 队列+迭代器对 | queue存(vector指针, 索引)对 | O(1) per call | O(k) | ⭐⭐⭐ |
| 解法2: 双指针交替 | 两个索引交替推进 | O(1) per call | O(1) | ⭐⭐ |

## 关键提示

```
Queue-based Iterator Scheduling (k=3 扩展):

v1: [1, 2, 3]       -> it1
v2: [4, 5, 6, 7]    -> it2
v3: [8, 9]           -> it3

Round 1: Queue=[it1,it2,it3] -> 输出 1,4,8
Round 2: Queue=[it1,it2,it3] -> 输出 2,5,9
Round 3: Queue=[it1,it2]     -> 输出 3,6  (it3耗尽)
Round 4: Queue=[it2]         -> 输出 7    (it1耗尽)

最终: [1,4,8,2,5,9,3,6,7]
```

## 解法详解

### 解法1: 队列 + 迭代器对 (推荐)

```cpp
class ZigzagIterator {
    queue<pair<vector<int>::iterator, vector<int>::iterator>> q;
public:
    ZigzagIterator(vector<int>& v1, vector<int>& v2) {
        if (!v1.empty()) q.push({v1.begin(), v1.end()});
        if (!v2.empty()) q.push({v2.begin(), v2.end()});
    }

    int next() {
        auto [cur, end] = q.front();
        q.pop();
        int val = *cur;
        ++cur;
        if (cur != end) q.push({cur, end});
        return val;
    }

    bool hasNext() {
        return !q.empty();
    }
};
```

### 解法2: 双指针交替

```cpp
class ZigzagIterator {
    vector<int> v1_, v2_;
    int i1 = 0, i2 = 0;
    bool pickFirst = true;
public:
    ZigzagIterator(vector<int>& v1, vector<int>& v2) : v1_(v1), v2_(v2) {}

    int next() {
        int val;
        if (i1 < v1_.size() && (pickFirst || i2 >= v2_.size())) {
            val = v1_[i1++];
            pickFirst = false;
        } else {
            val = v2_[i2++];
            pickFirst = true;
        }
        return val;
    }

    bool hasNext() {
        return i1 < v1_.size() || i2 < v2_.size();
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|----------|----------|------|
| ✗ 不检查空向量直接push | ✓ `if (!v1.empty()) q.push(...)` | 空向量不应加入队列 |
| ✗ 交替时不检查边界 | ✓ 一方耗尽后只从另一方读取 | 两向量长度可能不等 |
| ✗ 存vector的拷贝到queue | ✓ 存迭代器对(begin,end) | 避免不必要的内存开销 |

## 面试追问

**Q1: 如果扩展到 k 个向量怎么办？**
-> 队列解法天然支持 k 个向量，构造时将所有非空向量的迭代器对都加入队列即可。

**Q2: 如何实现线程安全版本？**
-> 在 `next()` 和 `hasNext()` 中加互斥锁，或使用并发队列。注意 `hasNext()` + `next()` 的原子性问题。

**Q3: 如果向量非常大，如何优化内存？**
-> 使用懒加载/流式读取，每个"迭代器"只需保存当前位置和数据源引用，不需要预加载全部数据。

## 相关题型

- [251. Flatten 2D Vector](https://leetcode.com/problems/flatten-2d-vector/) - 二维向量展平迭代器
- [284. Peeking Iterator](https://leetcode.com/problems/peeking-iterator/) - 支持 peek 的迭代器
- [341. Flatten Nested List Iterator](https://leetcode.com/problems/flatten-nested-list-iterator/) - 嵌套列表迭代器
- [173. Binary Search Tree Iterator](https://leetcode.com/problems/binary-search-tree-iterator/) - BST 迭代器
