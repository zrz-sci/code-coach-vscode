# 251. Flatten 2D Vector (展开二维向量)

## 核心思路

设计一个迭代器将二维向量展开为一维序列。核心是维护**两个指针**: 外层指针 `outer` 指向当前行，内层指针 `inner` 指向当前行中的元素。关键在于 `advance()` 辅助函数，负责跳过空行，确保指针始终指向有效元素。

## 思维链

1. **状态设计**: 需要两个索引 `outer` (行) 和 `inner` (列) 追踪当前位置
2. **跳过空行**: 二维向量中可能有空的子数组 `[]`，必须跳过
3. **advance 辅助**: 在 `hasNext()` 中调用，将指针推进到下一个有效元素
4. **next() 逻辑**: 先确保指针有效 (调用 hasNext), 返回当前元素，推进 inner
5. **hasNext() 逻辑**: 跳过空行后检查是否还有有效元素
6. **边界处理**: 全空输入 `[[], [], []]`、单元素 `[[1]]`、混合 `[[],[1],[]]`

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 双指针 (outer/inner) | next/hasNext O(1)均摊 | O(1) | ⭐⭐⭐⭐⭐ |
| 预展开为一维数组 | 构造 O(N), next O(1) | O(N) | ⭐⭐⭐ |

## 关键提示

- `advance()` 是核心: 当 `inner` 到达当前行末尾时，`outer++` 并重置 `inner=0`
- `hasNext()` 中调用 `advance()` 确保指针有效
- `next()` 中先调用 `hasNext()` (隐式 advance), 再返回元素
- 存储引用而非拷贝: 构造函数参数是 `vector<vector<int>>&`
- 面试中常考 follow-up: 用 C++ iterator 实现

## 解法详解

### 解法1: 双指针 (推荐)

维护外层和内层两个索引，advance 辅助函数跳过空行。

```cpp
class Vector2D {
public:
    Vector2D(vector<vector<int>>& vec) : data(vec), outer(0), inner(0) {}
    
    int next() {
        hasNext();  // 确保指针指向有效位置
        return data[outer][inner++];
    }
    
    bool hasNext() {
        advance();
        return outer < data.size();
    }
    
private:
    vector<vector<int>>& data;
    int outer, inner;
    
    // 跳过空行, 将指针推进到下一个有效元素
    void advance() {
        while (outer < data.size() && inner >= data[outer].size()) {
            outer++;
            inner = 0;
        }
    }
};
```

**复杂度**: 构造 O(1), next() 均摊 O(1), hasNext() 均摊 O(1), 空间 O(1)

### 解法2: 预展开为一维数组

构造时将所有元素展开到一维数组，用单个指针遍历。

```cpp
class Vector2D {
public:
    Vector2D(vector<vector<int>>& vec) : idx(0) {
        for (const auto& row : vec) {
            for (int val : row) {
                flat.push_back(val);
            }
        }
    }
    
    int next() {
        return flat[idx++];
    }
    
    bool hasNext() {
        return idx < flat.size();
    }
    
private:
    vector<int> flat;
    int idx;
};
```

**复杂度**: 构造 O(N), next() O(1), hasNext() O(1), 空间 O(N)

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 不处理空行 `[[],[3]]` | ✓ advance() 跳过空行 | 空行会导致 inner 越界访问 |
| ✗ 在 next() 中不调用 hasNext() | ✓ next() 先调用 hasNext() | 如果连续调用 next(), 指针可能停在空行 |
| ✗ 拷贝整个 vec | ✓ 存储引用 `vector<vector<int>>& data` | 拷贝浪费 O(N) 空间, 且题目传的是引用 |
| ✗ advance 用 `inner == data[outer].size()` | ✓ 用 `inner >= data[outer].size()` | 防御性编程, 处理空行时 size()=0 |

## 面试追问

**Q1: 如何用 C++ STL iterator 实现? (Follow-up)**
> 用 `vector<vector<int>>::iterator` 作为外层迭代器，`vector<int>::iterator` 作为内层迭代器。advance 中比较 inner 与 `outer->end()`。

**Q2: 如果要支持 remove() 操作呢?**
> 需要在 next() 返回后标记当前元素可删除，remove() 时执行实际删除并调整指针。Java Iterator 的标准模式。

**Q3: next() 和 hasNext() 的调用顺序有要求吗?**
> 设计上应该允许多次调用 hasNext() 不移动指针（幂等），next() 前隐式调用 hasNext() 保证安全。

## 相关题型

- [341. Flatten Nested List Iterator](https://leetcode.com/problems/flatten-nested-list-iterator/) - 嵌套列表展开迭代器（更复杂）
- [281. Zigzag Iterator](https://leetcode.com/problems/zigzag-iterator/) - 交替遍历多个列表
- [173. Binary Search Tree Iterator](https://leetcode.com/problems/binary-search-tree-iterator/) - BST 迭代器设计
- [284. Peeking Iterator](https://leetcode.com/problems/peeking-iterator/) - 支持 peek 的迭代器
