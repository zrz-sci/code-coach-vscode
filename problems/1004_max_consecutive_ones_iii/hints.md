# 1004. 最大连续1的个数 III

## 核心思路

**转化问题**：不要想"翻转哪些0"，而是想"找一个最长的子数组，其中0的个数不超过k"。这样就把"翻转操作"转化成了经典的**滑动窗口**问题。

## 思维链

1. **读完题第一反应**：枚举所有子数组，检查每个子数组里0的个数是否≤k，取最长的那个。这是 O(n²) 甚至 O(n³) 的暴力。

2. **暴力解的瓶颈**：对每个起点 left，我们都要从 left 往右扫描统计0的个数。大量重复计算——left 移动一位时，窗口内0的个数只变化了一点点，却要重新扫。

3. **关键观察**：当 right 向右扩展时，0的个数只增不减；当 left 向右收缩时，0的个数只减不增。这种**单调性**正是滑动窗口能用的前提！

4. **滑动窗口怎么用**：维护 `[left, right]` 窗口，用变量 `zeroCount` 记录窗口内0的个数。right 不断右移扩展窗口；当 `zeroCount > k` 时，left 右移收缩窗口，直到 `zeroCount <= k`。每一步更新最大窗口长度。

5. **为什么不会漏解**：right 单调右移不回头，left 也单调右移不回头。对于每个 right，left 停在"使窗口合法的最左位置"，所以每个合法的最长子数组都会被考虑到。

6. **另一个视角（二分+前缀和）**：对于固定的窗口长度 len，可以用前缀和在 O(1) 时间内查询任意子数组中0的个数。len 越大越难满足，越小越容易满足——具有单调性，可以二分答案。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有子数组，统计0的个数 | O(n²) | O(1) | 能说出即可 |
| 前缀和 + 二分 | 二分答案长度，前缀和检验 | O(n log n) | O(n) | 加分项 |
| 滑动窗口 | 维护窗口内0个数≤k | O(n) | O(1) | ⭐ 必须写出 |

## 关键提示

1. **问题转化**：把"翻转最多k个0"重新理解为"找最长子数组，使得子数组内0的个数≤k"。这个转化是解题的第一步。

2. **滑动窗口适用性判断**：窗口合法条件是"0的个数≤k"。右扩展可能让窗口变不合法（多了一个0），左收缩可以让窗口重新合法（少了一个0）。满足"单调性"，可以用滑动窗口。

3. **不需要真的翻转**：我们只需要计算最大长度，不需要修改原数组。

4. **窗口收缩用 while 还是 if？** 当 `zeroCount > k` 时，只需要收缩到 `zeroCount == k` 就停。因为每次 right 移动最多让 zeroCount 增加1（从 k 变成 k+1），所以用 if 也行。但用 while 更通用、更安全。

5. **ASCII 示意图 — 滑动窗口过程**：
```
nums = [1,1,1,0,0,0,1,1,1,1,0], k=2

Step1: [1,1,1,0,0] zeros=2 ✓  len=5
        L       R

Step2: [1,1,1,0,0,0] zeros=3 ✗ → 收缩左边
        L         R

Step3:   [1,1,0,0,0] zeros=3 ✗ → 继续收缩
          L       R

Step4:     [1,0,0,0] zeros=3 ✗ → 继续收缩
            L     R

Step5:       [0,0,0] zeros=3 ✗ → 继续收缩
              L   R

Step6:         [0,0] zeros=2 ✓  len=2
                L R

Step7:         [0,0,1,1,1,1] zeros=2 ✓  len=6  ← 最优！
                L         R

Step8:         [0,0,1,1,1,1,0] zeros=3 ✗ → 收缩
                L           R

...最终答案 = 6
```

## 解法详解

### 解法1: 暴力枚举 — O(n²) / O(1)

**思考过程**：最直接的想法——枚举每个起点 left，从 left 往右扩展 right，一边扩展一边数0的个数。当0的个数超过k时停止，记录当前长度。

```cpp
class Solution {
public:
    int longestOnes(vector<int>& nums, int k) {
        int n = nums.size();
        int maxLen = 0;
        for (int left = 0; left < n; left++) {
            int zeros = 0;
            for (int right = left; right < n; right++) {
                if (nums[right] == 0) zeros++;
                if (zeros > k) break;  // 0太多了，不可能更长
                maxLen = max(maxLen, right - left + 1);
            }
        }
        return maxLen;
    }
};
```

**关键点**：这个解法能通过吗？n 最大 10⁵，O(n²) = 10¹⁰，**会超时**。但它帮我们理解了题目本质：找最长子数组使得0的个数≤k。

### 解法2: 前缀和 + 二分 — O(n log n) / O(n)

**从解法1优化**：暴力法对每个起点线性扫描。如果我们预处理一个"0的个数"的前缀和数组 `prefix`，就能 O(1) 查询任意区间内0的个数。然后对于每个 left，二分查找最远的 right 使得区间内0的个数≤k。

```cpp
class Solution {
public:
    int longestOnes(vector<int>& nums, int k) {
        int n = nums.size();
        // prefix[i] = nums[0..i-1] 中 0 的个数
        vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + (nums[i] == 0 ? 1 : 0);
        }
        
        int maxLen = 0;
        for (int left = 0; left < n; left++) {
            // 二分查找：最大的 right 使得 prefix[right+1] - prefix[left] <= k
            // 即 prefix[right+1] <= prefix[left] + k
            int target = prefix[left] + k;
            // 在 prefix[left+1 .. n] 中找最后一个 <= target 的位置
            int lo = left, hi = n; // right 的范围是 [left, n-1]，对应 prefix 的 [left+1, n]
            while (lo < hi) {
                int mid = lo + (hi - lo + 1) / 2;  // 上取整，避免死循环
                if (prefix[mid + 1] <= target) {    // 注意这里用 mid+1 因为 right=mid 对应 prefix[mid+1]
                    // 等等，这里的索引关系需要仔细理清
                    lo = mid;
                } else {
                    hi = mid - 1;
                }
            }
            // lo 就是最远的 right
            if (prefix[lo + 1] <= target) {  // 确认合法
                maxLen = max(maxLen, lo - left + 1);
            }
        }
        return maxLen;
    }
};
```

> **注意**：前缀和+二分的写法索引关系容易搞混。更推荐直接用下面的滑动窗口。

**另一种更优雅的二分思路——二分答案**：

```cpp
class Solution {
public:
    int longestOnes(vector<int>& nums, int k) {
        int n = nums.size();
        // prefix[i] = nums[0..i-1] 中 0 的个数
        vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + (nums[i] == 0 ? 1 : 0);
        }
        
        // 二分答案：长度为 len 的子数组中，是否存在某个窗口使得0个数 <= k？
        // len 越大越难满足 → 有单调性 → 可以二分
        int lo = 0, hi = n;
        while (lo < hi) {
            int mid = lo + (hi - lo + 1) / 2;  // 上取整
            // 检查是否存在长度为 mid 的窗口，其中0的个数 <= k
            bool found = false;
            for (int i = 0; i + mid <= n; i++) {
                int zeros = prefix[i + mid] - prefix[i];
                if (zeros <= k) {
                    found = true;
                    break;
                }
            }
            if (found) lo = mid;  // 能满足，尝试更长
            else hi = mid - 1;    // 不能满足，缩短
        }
        return lo;
    }
};
```

**关键点**：二分答案的判断函数是 O(n) 的（滑动窗口扫一遍），总复杂度 O(n log n)。

### 解法3: 滑动窗口 — O(n) / O(1) ⭐ 面试首选

**从解法1/2优化**：暴力法里，left 每次从头开始扫太浪费了。核心观察：当 left 右移时，窗口内的0只会变少或不变，不需要重新统计。所以 left 和 right **都是单调右移**的，总移动次数 O(n)。

```cpp
class Solution {
public:
    int longestOnes(vector<int>& nums, int k) {
        int left = 0, maxLen = 0, zeros = 0;
        
        for (int right = 0; right < (int)nums.size(); right++) {
            // 扩展右边界：如果新元素是0，zeros++
            if (nums[right] == 0) zeros++;
            
            // 收缩左边界：如果0太多，左边界右移直到合法
            while (zeros > k) {
                if (nums[left] == 0) zeros--;
                left++;
            }
            
            // 此时 [left, right] 是以 right 结尾的最长合法窗口
            maxLen = max(maxLen, right - left + 1);
        }
        
        return maxLen;
    }
};
```

**为什么正确**：
- right 从左到右遍历每个位置，保证所有可能的右端点都考虑到了
- 对于每个 right，left 停在"使窗口内0个数恰好≤k的最左位置"
- left 单调右移，不会回头 → 不会漏掉任何合法窗口

**关键点**：`while (zeros > k)` 不能写成 `if`——虽然在本题中 if 碰巧也对（因为每次 right 移动最多新增1个0），但 while 是滑动窗口的标准写法，更安全更通用。

## 解法对比

| | 暴力枚举 | 前缀和+二分 | 滑动窗口 |
|------|------|------|------|
| 时间 | O(n²) | O(n log n) | O(n) |
| 空间 | O(1) | O(n) | O(1) |
| 核心思想 | 穷举所有子数组 | 预处理+二分查找 | 双指针单调移动 |
| 面试推荐度 | 说思路即可 | 展示多种思路 | **首选** |
| 代码量 | 最少 | 最多（索引易错）| 适中 |

**什么时候选哪个**：面试 99% 的场景写滑动窗口。前缀和+二分适合你想展示"我会多种方法"，或者题目变体不适合滑动窗口的情况。

## 易错点

1. **窗口长度算错**：
   - ✗ `maxLen = max(maxLen, right - left)` — 少算了1
   - ✓ `maxLen = max(maxLen, right - left + 1)` — 闭区间 [left, right] 的长度

2. **忘记在收缩时更新 zeros**：
   - ✗ 收缩时直接 `left++` 而不检查 `nums[left]` 是否为0
   - ✓ `if (nums[left] == 0) zeros--; left++;`

3. **k=0 的边界**：k=0 意味着不能翻转任何0，结果应该是原数组中最长连续1的长度。滑动窗口代码天然处理了这个情况（遇到0就收缩到0后面），不需要特判。

4. **全0数组 + k=0**：结果应该是0。滑动窗口也天然正确。

5. **前缀和解法中索引关系混乱**：
   - ✗ `prefix[right] - prefix[left]` — 区间定义不对
   - ✓ 要搞清楚 prefix[i] 代表的是前 i 个元素中0的个数，区间 [left, right] 中0的个数 = `prefix[right+1] - prefix[left]`

## 面试追问

**Q1: 这道题的暴力解是什么？瓶颈在哪？**
→ 暴力枚举所有子数组 O(n²)，每次都要重新统计0的个数。瓶颈：大量重复计算——left 移动一位，窗口内容只变化一点点，却要从头统计。

**Q2: 滑动窗口为什么能保证不漏解？**
→ right 遍历了所有可能的右端点。对于每个 right，left 停在使窗口合法的最左位置（不能更左了，否则0会超过k）。所以对于每个 right，都找到了以它为右端点的最长合法窗口。全局最优一定在其中。

**Q3: 如果题目改成"恰好翻转k个0"，怎么改？**
→ 窗口内0的个数必须**恰好等于k**。此时不能简单用一个窗口，因为窗口内0可能少于k。一种做法：找0的个数恰好为k的最长窗口 — 需要同时维护"0个数≤k"和"0个数≤k-1"两个窗口，用差值得到恰好为k的结果。这就是"恰好型"滑动窗口的经典技巧：`atMost(k) - atMost(k-1)`。

**Q4: 如果要返回具体翻转了哪些位置（不只是长度），怎么做？**
→ 在滑动窗口中，记录取得最大长度时的 left 和 right。然后在 [left, right] 中找所有值为0的位置，就是被翻转的位置。

## 相关题型

- **485. 最大连续1的个数** — 本题的简化版（k=0），直接计数连续1。复用思路：特殊情况。
- **487. 最大连续1的个数 II**（Premium）— 本题的简化版（k=1），可以用滑动窗口的同一模板。
- **424. 替换后的最长重复字符** — 同样的滑动窗口框架，区别：窗口合法条件从"0个数≤k"变成"窗口长度-最大频次字符个数≤k"。
- **209. 长度最小的子数组** — 滑动窗口经典题，区别：本题求最长，209求最短。收缩条件和更新时机不同。
- **76. 最小覆盖子串** — 滑动窗口进阶，窗口合法条件更复杂（需要包含所有目标字符）。