# 1929. 数组串联

## 核心思路（1句话）
创建一个 2n 长度的数组，前半部分和后半部分都是 nums 的拷贝；本题虽然简单，但面试中可以展示对 STL 容器操作和内存管理的理解。

## 思维链（4-6步）

1. **第一反应**：直接创建长度 2n 的新数组，用两次循环（或一次循环同时写两个位置）填充即可。
2. **STL 简化**：C++ 的 `vector` 有 `insert` 方法，可以直接把 nums 追加到自身末尾。一行代码搞定。
3. **性能考虑**：预分配空间 `reserve(2n)` 可以避免 `insert` 时的内存重新分配。虽然本题数据量很小 (n <= 1000) 无所谓，但展示了对 vector 扩容机制的理解。
4. **原地 vs 新数组**：题目要求返回新数组 `ans`，但也可以直接在 `nums` 上追加（修改原数组）。面试时应先确认是否允许修改输入。
5. **取模技巧**：`ans[i] = nums[i % n]`，用一个循环遍历 0 到 2n-1，下标取模映射回原数组。这是循环数组的通用技巧。
6. **最终决策**：面试时给出 `insert` 一行解法展示 STL 熟练度，再解释底层的手动循环方式。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 解法1: 手动双写循环 ⭐面试首选 | O(n) | O(n) 输出 | 清晰展示逻辑 |
| 解法2: STL insert 一行解 | O(n) | O(n) 输出 | 最简洁 |
| 解法3: 取模映射 | O(n) | O(n) 输出 | 循环数组通用技巧 |
| 解法4: memcpy/copy | O(n) | O(n) 输出 | 底层操作，展示系统编程能力 |

## 关键提示

1. **vector::insert 的底层行为**：`nums.insert(nums.end(), nums.begin(), nums.end())` 看似简单，但有一个隐含陷阱——如果 insert 触发了 vector 扩容（reallocation），原来的 `begin()` 和 `end()` 迭代器会失效！好在标准库实现会在 insert 开始前就复制好源数据范围，所以实际使用是安全的。但面试中提到这个细节能加分。

2. **reserve vs resize 的区别**：
   - `reserve(2n)`：只分配内存，不改变 size，适合后续 `push_back`
   - `resize(2n)`：分配内存并初始化元素，改变 size，适合直接用下标访问
   - 本题用 `resize(2n)` 配合下标赋值，或 `reserve(2n)` 配合 `push_back`

3. **取模映射的通用性**：`ans[i] = nums[i % n]` 这个技巧在很多"循环数组"题目中都会用到，比如 LC 503 下一个更大元素 II（循环遍历数组）。

4. **C++ 中数组拷贝的几种方式**：
   ```cpp
   // 方式1: 循环赋值
   for (int i = 0; i < n; i++) ans[i+n] = nums[i];
   
   // 方式2: STL copy
   copy(nums.begin(), nums.end(), ans.begin() + n);
   
   // 方式3: memcpy (连续内存)
   memcpy(&ans[n], &nums[0], n * sizeof(int));
   
   // 方式4: insert
   ans.insert(ans.end(), nums.begin(), nums.end());
   ```

5. **面试延伸**：虽然题目本身很简单，但面试官可能借此考察：
   - vector 的内存模型（连续内存、capacity vs size）
   - 迭代器失效问题
   - 时间复杂度分析（为什么是 O(n) 而非 O(1)）
   - 如果不允许额外空间怎么办（本题不可能，因为结果就是 2n）

## 解法详解

### 解法1: 手动双写循环 — O(n)/O(n) ⭐面试首选

**思考过程**: 最直观的方式：创建 2n 数组，一次循环中同时写前半和后半。

```cpp
class Solution {
public:
    vector<int> getConcatenation(vector<int>& nums) {
        int n = nums.size();
        vector<int> ans(2 * n);  // 预分配 2n 空间
        for (int i = 0; i < n; i++) {
            ans[i] = nums[i];        // 前半部分
            ans[i + n] = nums[i];    // 后半部分
        }
        return ans;
    }
};
```

**关键点**: 
- `vector<int> ans(2 * n)` 直接构造 2n 长度的 vector，所有元素初始化为 0
- 一次循环写两个位置，减少循环次数（虽然渐进复杂度一样）
- 代码逻辑清晰，面试中最容易解释

### 解法2: STL insert 一行解 — O(n)/O(n)

**从解法1简化**: 利用 vector 的 `insert` 方法，把 nums 的所有元素追加到自身末尾。

```cpp
class Solution {
public:
    vector<int> getConcatenation(vector<int>& nums) {
        // 把 nums 的 [begin, end) 追加到 nums 末尾
        // 注意：虽然源和目标是同一个 vector，标准库实现是安全的
        nums.insert(nums.end(), nums.begin(), nums.end());
        return nums;
    }
};
```

**关键点**: 
- 一行核心代码，最简洁
- 直接修改了输入 `nums`，如果题目不允许修改输入，需要先拷贝
- `insert` 可能触发扩容（reallocation），时间 O(n) 是摊销的

### 解法3: 取模映射 — O(n)/O(n)

**从解法1优化**: 用取模运算统一处理前半和后半，代码更优雅且展示"循环数组"思维。

```cpp
class Solution {
public:
    vector<int> getConcatenation(vector<int>& nums) {
        int n = nums.size();
        vector<int> ans(2 * n);
        for (int i = 0; i < 2 * n; i++) {
            // i % n: 0,1,...,n-1, 0,1,...,n-1
            ans[i] = nums[i % n];
        }
        return ans;
    }
};
```

**关键点**: 
- `i % n` 将下标映射回 [0, n-1]，自然实现了"重复"效果
- 这个技巧在循环数组相关题目中非常常用（如 LC 503、LC 918）
- 取模运算有微小的额外开销，但在本题规模下可忽略

### 解法4: STL copy + 预分配 — O(n)/O(n)

**从解法1优化**: 展示 C++ STL 算法和底层内存操作的熟练度。

```cpp
class Solution {
public:
    vector<int> getConcatenation(vector<int>& nums) {
        int n = nums.size();
        vector<int> ans;
        ans.reserve(2 * n);  // 预分配，避免扩容

        // 第一份：直接拷贝
        ans.assign(nums.begin(), nums.end());

        // 第二份：追加
        copy(nums.begin(), nums.end(), back_inserter(ans));

        return ans;
    }
};
```

**关键点**: 
- `reserve(2 * n)` 预分配内存，避免后续 push_back/copy 时的多次扩容
- `assign` + `copy` 分两步完成，语义清晰
- `back_inserter` 创建一个特殊迭代器，每次赋值等效于 `push_back`

## 解法对比

| 维度 | 解法1: 双写循环 | 解法2: insert | 解法3: 取模 | 解法4: copy |
|------|---------------|--------------|------------|------------|
| 时间 | O(n) | O(n) | O(n) | O(n) |
| 空间 | O(n) 新数组 | O(n) 原地扩展 | O(n) 新数组 | O(n) 新数组 |
| 代码量 | 5行 | 1行 | 4行 | 5行 |
| 可读性 | 最高 | 高 | 高 | 中 |
| 面试展示 | 基础循环能力 | STL 熟练度 | 循环数组思维 | 底层操作 |
| 修改输入 | 否 | 是 | 否 | 否 |

## 易错点

1. **数组越界**
   ```cpp
   // ✗ 错误：ans 只分配了 n 个空间，写 ans[i+n] 越界！
   vector<int> ans(n);
   for (int i = 0; i < n; i++) {
       ans[i] = nums[i];
       ans[i + n] = nums[i];  // 越界！未定义行为
   }

   // ✓ 正确：分配 2n 空间
   vector<int> ans(2 * n);
   ```
   **原因**：`vector<int> ans(n)` 只有 n 个元素，访问 `ans[n]` 到 `ans[2n-1]` 是越界。

2. **insert 后迭代器失效**
   ```cpp
   // ✗ 潜在问题（实际标准库处理了，但面试需要理解原因）
   auto it_begin = nums.begin();
   auto it_end = nums.end();
   nums.insert(nums.end(), it_begin, it_end);
   // 如果 insert 触发 reallocation，it_begin 和 it_end 已失效
   // 标准库实现会在 insert 前拷贝范围，所以直接传 begin/end 是安全的

   // ✓ 更安全的写法（拷贝到新 vector）
   vector<int> ans(nums);  // 拷贝
   ans.insert(ans.end(), nums.begin(), nums.end());
   ```
   **原因**：vector 扩容时会分配新内存，旧内存释放，所有指向旧内存的迭代器失效。

3. **返回引用而非新对象**
   ```cpp
   // ✗ 错误：直接返回 nums（如果在 nums 上修改了），但函数签名返回 vector<int>
   // 如果面试要求不能修改输入，insert 方式就不行
   vector<int> getConcatenation(vector<int>& nums) {
       nums.insert(nums.end(), nums.begin(), nums.end());
       return nums;  // 返回修改后的 nums，原来的 nums 也被改了
   }

   // ✓ 如果不能修改输入，创建新数组
   vector<int> getConcatenation(vector<int>& nums) {
       vector<int> ans(nums);
       ans.insert(ans.end(), nums.begin(), nums.end());
       return ans;
   }
   ```

## 面试追问

### Q1: 为什么不能 O(1) 空间完成？
**回答**：
因为输出本身就是长度 2n 的数组，至少需要 O(n) 的空间来存储结果。如果不算输出空间，题目本身确实只需要 O(1) 额外空间（直接在结果数组上写即可）。所以更准确的说法是：额外空间 O(1)，输出空间 O(n)。

### Q2: vector 的 insert 触发扩容时，时间复杂度是多少？
**回答**：
单次 insert(end, begin, end) 最坏情况下：
1. 发现 capacity 不够，重新分配 2n 的内存 -> O(1) 分配
2. 把原来的 n 个元素移动到新内存 -> O(n)
3. 把要插入的 n 个新元素拷贝到末尾 -> O(n)
4. 释放旧内存 -> O(1)

总计 O(n)。如果提前 `reserve(2n)`，可以避免第 2 步的移动，只需要第 3 步的拷贝 O(n)。

vector 的扩容策略通常是当前 capacity 的 2 倍（MSVC）或 1.5 倍（GCC），所以 n 次 push_back 的摊销时间复杂度是 O(n)。

### Q3: `i % n` 的取模操作有性能开销吗？有没有更快的方式？
**回答**：
取模 (%) 在现代 CPU 上是一个除法操作，比加法慢约 3-5 倍。对于本题 n <= 1000 完全可以忽略。但在高性能场景中：

1. **位运算替代**：如果 n 是 2 的幂，`i % n` 等价于 `i & (n-1)`，位运算比除法快得多。
2. **条件分支替代**：
   ```cpp
   int idx = i;
   if (idx >= n) idx -= n;  // 比 % 快，但只适用于循环一次的场景
   ```
3. **两次循环替代**（本题最优）：
   ```cpp
   for (int i = 0; i < n; i++) ans[i] = nums[i];
   for (int i = 0; i < n; i++) ans[i+n] = nums[i];
   ```
   完全避免了取模运算。

### Q4: 这题的实际应用场景是什么？
**回答**：
数组串联在很多场景中有用：
1. **循环数组模拟**：将数组复制一份拼接，可以把"循环遍历"变成"线性遍历"。比如 LC 503（下一个更大元素 II）就可以这样处理。
2. **字符串旋转判断**：判断 s2 是否是 s1 的旋转，只需检查 s2 是否是 s1+s1 的子串。
3. **音频/信号处理**：循环缓冲区（ring buffer）的展开处理。
4. **图形学**：纹理重复（texture tiling）的实现。

## 相关题型

| 题目 | 关系 | 复用技巧 |
|------|------|---------|
| [503. 下一个更大元素 II](https://leetcode.com/problems/next-greater-element-ii/) | 循环数组 | 拼接数组或用 `i % n` 模拟循环遍历 |
| [918. 环形子数组的最大和](https://leetcode.com/problems/maximum-sum-circular-subarray/) | 循环数组 | 数组串联思想处理环形情况 |
| [796. 旋转字符串](https://leetcode.com/problems/rotate-string/) | 串联判旋转 | s1+s1 包含所有旋转结果 |
| [1470. 重新排列数组](https://leetcode.com/problems/shuffle-the-array/) | 数组重排 | 类似的数组构造题 |
| [88. 合并两个有序数组](https://leetcode.com/problems/merge-sorted-array/) | 数组操作 | 双数组合并到一个的技巧 |
