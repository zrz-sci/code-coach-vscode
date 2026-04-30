# 900. RLE 迭代器

## 核心思路

本质上是一个**指针跳跃 + 计数消耗**的设计题：维护一个指针 `idx` 指向当前 RLE 编码的位置，每次 `next(n)` 时从当前位置开始"消耗" n 个元素，如果当前段不够就跳到下一段。关键是不要真的展开 RLE（因为值可能高达 10^9），而是直接在编码数组上操作计数。

## 思维链

1. **读完题第一反应** → RLE 编码就是"计数+值"的对。比如 `[3,8,2,5]` 表示 `[8,8,8,5,5]`。`next(n)` 要消耗 n 个元素并返回最后一个。
2. **暴力做法？** → 先把 RLE 展开成完整数组，然后维护一个全局指针，每次 `next(n)` 就往前跳 n 步。但 encoding[i] 可以到 10^9，展开后数组可能巨大，内存爆炸。
3. **优化方向** → 不展开！直接在编码数组上操作。维护指针 `idx` 指向当前段（encoding 中的偶数索引），以及当前段的剩余计数。
4. **next(n) 的逻辑** → 循环：如果当前段剩余计数 >= n，减掉 n 并返回当前段的值；如果不够，n 减去当前段的剩余计数，idx 跳到下一段。如果 idx 越界了还没消耗完，返回 -1。
5. **状态管理** → 只需要两个状态：`idx`（当前段的位置）和 `remaining`（当前段的剩余计数）。也可以直接修改 `encoding[idx]` 来记录剩余计数。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 展开 RLE 数组 | 先展开再用指针 | O(total) 初始化 | O(total) | 会 MLE，但要能说出 |
| 指针跳跃（直接修改 encoding） | 在编码数组上操作，修改原数组记录剩余 | O(n) per call amortized | O(1) 额外 | ⭐ 面试首选 |
| 指针跳跃（额外变量） | 用 idx + remaining 不修改原数组 | O(n) per call amortized | O(1) 额外 | 等价写法 |

## 关键提示

1. **不要展开 RLE**：encoding[i] 可达 10^9，展开后的数组可能有数十亿个元素，内存不够。核心思想是在编码层面直接操作计数。
2. **next(n) 的 n 也可以很大**：n 最大 10^9，所以 next 内部不能用循环一个一个消耗，必须一次减掉一整段。
3. **均摊时间分析**：每次 `next` 调用可能跳过多段，但每段最多被跳过一次。总共 encoding.length/2 段，最多 1000 次 next 调用，所以所有 next 调用的总时间是 O(encoding.length + 1000)。
4. **-1 的条件**：当所有段都消耗完了（idx 越界），但还需要消耗更多元素时，返回 -1。
5. **encoding 的结构**：偶数索引是计数，奇数索引是值。`encoding[0]` 个 `encoding[1]`，`encoding[2]` 个 `encoding[3]`，以此类推。

## 解法详解

### 解法1: 展开 RLE（会 MLE，用于理解）

**思考过程**: 先展开理解题意，再优化。

```
encoding = [3, 8, 0, 9, 2, 5]
展开: [8, 8, 8, 5, 5]  (3个8, 0个9, 2个5)

next(2): 消耗 [8, 8]，返回 8。剩余 [8, 5, 5]
next(1): 消耗 [8]，返回 8。剩余 [5, 5]
next(1): 消耗 [5]，返回 5。剩余 [5]
next(2): 要消耗2个，但只剩1个，返回 -1
```

```cpp
class RLEIterator {
public:
    vector<int> expanded;
    int ptr;
    
    RLEIterator(vector<int>& encoding) {
        ptr = 0;
        for (int i = 0; i < encoding.size(); i += 2) {
            for (int j = 0; j < encoding[i]; j++) {
                expanded.push_back(encoding[i + 1]);
            }
        }
    }
    
    int next(int n) {
        ptr += n;
        if (ptr > (int)expanded.size()) return -1;
        return expanded[ptr - 1];
    }
};
```

**关键点**:
- encoding[i] 可达 10^9，展开后数组太大，MLE。
- 但这个思路帮助理解 next 的语义：消耗 n 个元素，返回第 n 个。

---

### 解法2: 指针跳跃（直接修改 encoding） — O(均摊) / O(1) ⭐ 面试首选

**从解法1优化**: 不展开，直接在 encoding 数组上操作。用 `idx` 指向当前段，直接修改 `encoding[idx]` 记录剩余计数。

```
encoding = [3, 8, 0, 9, 2, 5]
初始: idx = 0

next(2): 
  encoding[0]=3 >= 2? 是! encoding[0] -= 2 → encoding[0]=1
  返回 encoding[1] = 8 ✓

next(1):
  encoding[0]=1 >= 1? 是! encoding[0] -= 1 → encoding[0]=0
  返回 encoding[1] = 8 ✓

next(1):
  encoding[0]=0 >= 1? 否! n -= 0, idx += 2 → idx=2
  encoding[2]=0 >= 1? 否! n -= 0, idx += 2 → idx=4
  encoding[4]=2 >= 1? 是! encoding[4] -= 1 → encoding[4]=1
  返回 encoding[5] = 5 ✓

next(2):
  encoding[4]=1 >= 2? 否! n -= 1, idx += 2 → idx=6
  idx=6 >= encoding.size()=6 → 越界，返回 -1 ✓
```

```cpp
class RLEIterator {
public:
    vector<int> enc;
    int idx;
    
    RLEIterator(vector<int>& encoding) {
        enc = encoding;
        idx = 0;
    }
    
    int next(int n) {
        while (idx < (int)enc.size()) {
            if (enc[idx] >= n) {
                // 当前段够消耗
                enc[idx] -= n;
                return enc[idx + 1];
            } else {
                // 当前段不够，消耗完跳到下一段
                n -= enc[idx];
                idx += 2;
            }
        }
        // 所有段都消耗完了，还需要更多
        return -1;
    }
};
```

**关键点**:
- 直接修改 `enc[idx]` 记录剩余计数，不需要额外变量。
- 每段最多被跳过一次（一旦跳过就 idx 前进了），所以所有 next 调用的总跳段次数 <= encoding.length/2。
- `enc[idx] >= n` 用 `>=` 而不是 `>`：等于的时候意味着当前段恰好消耗完，下次 next 时会发现 `enc[idx]=0` 然后跳到下一段。

---

### 解法3: 指针跳跃（不修改原数组） — O(均摊) / O(1) 额外

**变种**: 用 `remaining` 变量记录当前段的剩余计数，不修改原数组。

```cpp
class RLEIterator {
public:
    vector<int> enc;
    int idx;
    long long remaining;  // 当前段的剩余计数
    
    RLEIterator(vector<int>& encoding) {
        enc = encoding;
        idx = 0;
        remaining = idx < (int)enc.size() ? enc[0] : 0;
    }
    
    int next(int n) {
        long long need = n;  // 用 long long 防止溢出
        
        while (idx < (int)enc.size()) {
            if (remaining >= need) {
                // 当前段够消耗
                remaining -= need;
                return enc[idx + 1];
            } else {
                // 当前段不够
                need -= remaining;
                idx += 2;
                remaining = idx < (int)enc.size() ? enc[idx] : 0;
            }
        }
        
        return -1;
    }
};
```

**关键点**:
- 用 `remaining` 而不是修改原数组，更"纯净"。
- 注意 `need` 用 `long long`：如果 n 和 encoding[i] 都接近 10^9，减法过程中中间值可能溢出 int。
- 初始化时要处理空编码的边界情况。

## 解法对比

|  | 展开 RLE | 修改原数组 | 额外变量 |
|--|---------|-----------|---------|
| 时间 | O(total) 初始化 | **O(1) 初始化** | **O(1) 初始化** |
| 空间 | O(total) MLE风险 | **O(1) 额外** | **O(1) 额外** |
| next均摊 | O(1) per call | O(段数/调用数) | O(段数/调用数) |
| 代码简洁 | 简单 | **最简洁** | 稍复杂 |

**面试选择**: 写解法2（修改原数组），代码最简洁，逻辑最清晰。

## 易错点

1. **试图展开 RLE 数组**：
   - ✗ 把 encoding 展开成完整数组 → encoding[i] 可达 10^9，内存爆炸
   - ✓ 直接在编码数组上操作计数

2. **idx 步进忘记 +2**：
   - ✗ `idx++` → encoding 是 [count, value, count, value, ...]，每段占两个位置
   - ✓ `idx += 2`

3. **整数溢出**：
   - ✗ `n -= enc[idx]` 用 int，但连续多段的总计数可能超过 int 范围
   - ✓ 实际上由于 n 本身是 int（最大 10^9），减法不会溢出。但如果用累加判断则要注意。

4. **等于边界处理**：
   - ✗ `if (enc[idx] > n)` → 漏掉了恰好等于的情况
   - ✓ `if (enc[idx] >= n)` → 等于时消耗完当前段，下次调用再跳

5. **空段的处理**：
   - encoding 中 count 可以是 0（如 `[3, 8, 0, 9, 2, 5]` 中的 `0, 9`）
   - 代码自然处理：`enc[idx]=0 < n`，直接跳过该段

## 面试追问

**Q1: 所有 next 调用的总时间复杂度是多少？**
> O(encoding.length/2 + Q)，其中 Q 是 next 调用次数。因为每段最多被跳过一次（跳过后 idx 永远不会回退），总跳段次数不超过 encoding.length/2。每次 next 调用至少做一次比较（O(1)）。所以总时间是 O(段数 + 调用数)。

**Q2: 如果要支持 prev(n)（回退 n 个元素），怎么改？**
> 需要支持 idx 回退。可以把每段的原始计数保存下来（不修改原数组），用额外变量 remaining 记录当前段剩余。回退时从当前段往前找，恢复之前段的计数。或者用解法3 的框架，idx 和 remaining 都可以回退。

**Q3: 如果 next 的 n 可以是 0，需要特殊处理吗？**
> 根据约束 n >= 1，不需要。但如果 n=0，应该返回"上一个消耗的元素"。这需要额外记录上一次返回值。

**Q4: 如果 encoding 非常长（百万级），但大部分 count 是 0，怎么优化？**
> 预处理：跳过所有 count=0 的段，或者用一个"非零段索引数组"。也可以在 next 的 while 循环中自然跳过（count=0 的段会立即被跳过，不影响正确性，只是多了几次循环迭代）。

## 相关题型

- **284. 窥探迭代器 (Peeking Iterator)** — 同为迭代器设计题。需要实现 peek() 在不消耗的情况下查看下一个元素。核心技巧：用一个缓存变量存"预读"的值。
- **341. 扁平化嵌套列表迭代器** — 更复杂的迭代器设计。需要递归/栈来处理嵌套结构。本题的 RLE 是"一维压缩"，341 是"多维嵌套"。
- **604. 迭代压缩字符串** — 和本题非常相似！也是压缩编码的迭代器，需要 next() 和 hasNext()。区别是编码格式不同（字母+数字 vs 数字对）。
- **271. 编码和解码字符串** — 涉及编码/解码的设计题。和 RLE 编码的思想相关，但侧重字符串序列化。
