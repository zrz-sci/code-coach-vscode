# 898. 子数组按位或操作

## 核心思路

本质上是一个**利用 OR 运算单调性进行状态压缩**的问题：对于以每个位置结尾的所有子数组的 OR 值，由于 OR 只会"加 bit 不减 bit"，所以不同的 OR 值最多只有 O(log(max_val)) 个。用一个 set 滚动维护"以当前位置结尾的所有可能 OR 值"，总复杂度远好于暴力。

## 思维链

1. **读完题第一反应** → 暴力：枚举所有子数组 O(n²)，对每个子数组计算 OR，把结果存入 set。但 n 最大 5×10⁴，O(n²) ≈ 2.5×10⁹，超时。
2. **暴力解的瓶颈在哪？** → 枚举所有子数组是 O(n²)，即使 OR 运算可以增量更新（在前一个结果上再 OR 当前元素），总操作量还是 O(n²)。
3. **怎么突破瓶颈？** → 关键观察：OR 运算的单调性。`a | b >= a`，因为 OR 只会把 0 变成 1，不会把 1 变成 0。这意味着以某个位置 i 结尾的所有子数组 OR 值，随着子数组长度增加，OR 值只增不减。
4. **进一步推导** → 既然 OR 值只增不减，而且每次增加至少加一个 bit，那么以位置 i 结尾的不同 OR 值最多只有 O(log(max_val)) 个（因为一个 32 位整数最多 30 个 bit）。
5. **算法设计** → 用一个 set `prev` 存"以前一个位置结尾的所有不同 OR 值"。遍历到 `arr[i]` 时，新的 set `curr` = {`v | arr[i]` for v in prev} ∪ {arr[i]}。由于上面的分析，`curr` 的大小最多 O(30)。把 curr 的所有值加入全局结果 set。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举所有子数组 | 双重循环 + 增量 OR | O(n²) | O(n²) | TLE，但要能说出 |
| 滚动 set + OR 单调性 | 利用 OR 只加 bit 的性质压缩状态 | O(n·30) | O(n·30) | ⭐ 面试首选 |
| 滚动 set（优化常数） | 用 unordered_set 替代 set | O(n·30) | O(n·30) | 实践更快 |

## 关键提示

1. **OR 的单调性是核心**：`a | b >= a`。这意味着以 i 结尾的子数组 OR 值随长度增加单调不减。每增加一个 bit 对应一个新的 OR 值，所以不同值最多 30 个。
2. **为什么不能用滑动窗口？** OR 运算没有"逆运算"——你可以 OR 上一个数，但不能"去掉"一个数的贡献。所以无法高效维护窗口内的 OR 值。这是 AND/OR/GCD 等运算与 SUM/XOR 的关键区别。
3. **滚动 set 的更新**：`curr = {v | arr[i] : v ∈ prev} ∪ {arr[i]}`。新增 `arr[i]` 是因为长度为 1 的子数组（只有 `arr[i]` 本身）。
4. **全局去重**：每步把 `curr` 中的所有值加入全局 `result` set，最终 `result.size()` 就是答案。
5. **复杂度分析**：外层 n 次迭代，每次 `prev` 最多 30 个元素，所以总操作 O(30n)。全局 result set 最多 O(30n) 个元素（实际远小于此）。

## 解法详解

### 解法1: 暴力枚举 — O(n²) / O(n²) (TLE)

**思考过程**: 枚举所有子数组起点 i 和终点 j，用增量 OR 计算每个子数组的 OR 值。

```
示例: arr = [1, 1, 2]
子数组:
  [1]     → OR = 1
  [1]     → OR = 1
  [2]     → OR = 2
  [1,1]   → OR = 1|1 = 1
  [1,2]   → OR = 1|2 = 3
  [1,1,2] → OR = 1|1|2 = 3
不同值: {1, 2, 3} → 答案 = 3
```

```cpp
class Solution {
public:
    int subarrayBitwiseORs(vector<int>& arr) {
        unordered_set<int> result;
        int n = arr.size();
        for (int i = 0; i < n; i++) {
            int orVal = 0;
            for (int j = i; j < n; j++) {
                orVal |= arr[j];
                result.insert(orVal);
            }
        }
        return result.size();
    }
};
```

**关键点**:
- 增量 OR：`orVal |= arr[j]` 避免每次重新计算整个子数组的 OR。
- n = 5×10⁴ 时 O(n²) ≈ 2.5×10⁹，超时。
- 价值：明确了暴力的瓶颈——虽然 OR 是增量的，但子数组数量还是 O(n²)。

---

### 解法2: 滚动 set + OR 单调性 — O(30n) / O(30n) ⭐ 面试首选

**从解法1优化**: 解法1的核心浪费在于：以位置 i 结尾的子数组 OR 值虽然有 O(n) 个子数组，但不同的 OR 值最多只有 30 个（因为每次 OR 最多多加一个 bit）。所以只需要维护不同的 OR 值。

```
示例: arr = [1, 2, 4]  (二进制: 001, 010, 100)

i=0: arr[0]=1(001)
  以 i=0 结尾的 OR 值: {1}
  全局: {1}

i=1: arr[1]=2(010)
  prev={1}, 对每个 v: v|2 → {1|2=3}
  加上 arr[1] 本身: {3, 2}
  全局: {1, 2, 3}

i=2: arr[2]=4(100)
  prev={3, 2}, 对每个 v: v|4 → {3|4=7, 2|4=6}
  加上 arr[2] 本身: {7, 6, 4}
  全局: {1, 2, 3, 4, 6, 7}

答案: 6 ✓
```

```cpp
class Solution {
public:
    int subarrayBitwiseORs(vector<int>& arr) {
        unordered_set<int> result;  // 全局去重
        unordered_set<int> prev;    // 以前一个位置结尾的所有不同 OR 值
        
        for (int x : arr) {
            unordered_set<int> curr;
            curr.insert(x);  // 长度为1的子数组
            
            // 对 prev 中每个值 OR 上当前元素
            for (int v : prev) {
                curr.insert(v | x);
            }
            
            // 把 curr 的所有值加入全局结果
            for (int v : curr) {
                result.insert(v);
            }
            
            prev = move(curr);  // 滚动
        }
        
        return result.size();
    }
};
```

**关键点**:
- **为什么 curr 最多 30 个元素？** 以位置 i 结尾的子数组 OR 值可以写成 `arr[i]`, `arr[i-1]|arr[i]`, `arr[i-2]|arr[i-1]|arr[i]`, ...。每一步 OR 一个新元素，要么不变（不增加 bit），要么增加至少 1 个 bit。32 位整数最多 30 个有效 bit，所以最多 30 个不同值。
- `move(curr)` 避免拷贝，把 curr 直接转移给 prev。
- 时间 O(30n)，空间 O(30n)（result 最多存 30n 个不同值），完全够用。

---

### 解法3: 用 set 替代 unordered_set（更稳定）

**变种**: 如果担心 unordered_set 的哈希冲突导致退化，可以用 set（红黑树），时间变为 O(30n·log(30n)) 但常数更稳定。

```cpp
class Solution {
public:
    int subarrayBitwiseORs(vector<int>& arr) {
        set<int> result;
        set<int> prev;
        
        for (int x : arr) {
            set<int> curr;
            curr.insert(x);
            for (int v : prev) {
                curr.insert(v | x);
            }
            result.insert(curr.begin(), curr.end());
            prev = move(curr);
        }
        
        return result.size();
    }
};
```

## 解法对比

|  | 暴力 O(n²) | 滚动 set (unordered) | 滚动 set (ordered) |
|--|-----------|---------------------|-------------------|
| 时间 | O(n²) TLE | **O(30n)** | O(30n·log) |
| 空间 | O(n²) | O(30n) | O(30n) |
| 适用 | 理解题意 | **首选** | 哈希不稳定时备选 |

**面试选择**: 直接写滚动 unordered_set 版本。关键是能讲清楚"OR 只加 bit"这个性质推导出的 O(30) 上界。

## 易错点

1. **忘记加长度为1的子数组**：
   - ✗ `curr` 只包含 `{v | x : v ∈ prev}`，漏掉了 `x` 本身
   - ✓ `curr.insert(x)` 在循环之前先加入

2. **prev 和 curr 混用**：
   - ✗ 直接在 prev 上修改 → 一边遍历 prev 一边修改它，未定义行为
   - ✓ 创建新的 curr，遍历完后 `prev = curr`

3. **没看到 OR 值上界的推导就写 O(n²) 暴力**：
   - 面试中如果直接写暴力且不知道优化方向，很难拿到好评
   - 关键是要能说出"OR 只加 bit，所以不同值最多 30 个"

4. **全局 result 忘记去重**：
   - ✗ 用 vector 存结果，最后忘记去重 → 答案偏大
   - ✓ 用 set/unordered_set 自动去重

## 面试追问

**Q1: 为什么不能用滑动窗口？**
> OR 运算没有逆运算。你可以 OR 上一个数（加入窗口），但不能"去掉"一个数的贡献（缩小窗口）。这和 SUM（可以减去）、XOR（可以再 XOR 一次抵消）不同。所以标准滑动窗口不适用。

**Q2: 如果把 OR 换成 AND，思路一样吗？**
> 完全对称。AND 也有单调性：`a & b <= a`，AND 只会"去 bit 不加 bit"。以位置 i 结尾的子数组 AND 值也最多 30 个不同值。算法框架一模一样，只是 OR 换成 AND。

**Q3: 如果把 OR 换成 GCD 呢？**
> GCD 也有类似性质：`gcd(a, b) <= min(a, b)`，随子数组变长，GCD 只会减小或不变。但 GCD 每次减小至少除以 2，所以不同 GCD 值最多 O(log(max_val)) 个。同样的滚动 set 框架适用。

**Q4: 全局 result set 最多有多少个元素？**
> 理论上界是 O(30n)，因为每步加入最多 30 个新值。但实际上由于不同位置的 OR 值有大量重叠，远小于此。最坏情况发生在每个元素只有一个 bit（如 1, 2, 4, 8, ...），此时每步都能产生新组合。

## 相关题型

- **1521. 找到最接近目标值的函数值** — 同样利用 AND 的单调性 + 滚动 set，在所有子数组 AND 值中找最接近 target 的。核心框架完全相同。
- **2411. 按位或最大的最小子数组长度** — OR 单调性的另一个应用：对每个位置找满足 OR >= target 的最短子数组。
- **2419. 按位与最大的最长子数组** — AND 版本的变体：找 AND 值最大的子数组长度。利用 AND 单调递减的性质。
- **209. 长度最小的子数组** — 对比学习：SUM 有逆运算所以可以用滑动窗口，而 OR/AND 没有逆运算所以用滚动 set。理解这个区别很重要。
