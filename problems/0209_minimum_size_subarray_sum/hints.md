# 209. 长度最小的子数组

## 核心思路

本质上在问：在一个**正整数数组**中，找一段**连续子数组**，使其和 ≥ target，且长度尽可能短。关键词是"正整数"——这保证了加入更多元素总和只增不减，这是滑动窗口能用的根本前提。

## 思维链

1. **读完题第一反应**：枚举所有可能的子数组，计算每段的和，找满足条件的最短段。暴力枚举起点 i 和终点 j，O(n²) 种子数组，每段求和 O(n)，总共 O(n³)。
2. **暴力优化一步**：求和可以用前缀和把区间求和从 O(n) 优化到 O(1)，暴力降到 O(n²)。但 n=10⁵，还是太慢。
3. **瓶颈在哪？** 对于每个起点 i，我们在 `j = i, i+1, ..., n-1` 中找第一个使得 sum ≥ target 的 j。因为所有元素都是正整数，前缀和是**严格递增**的！递增序列上找目标值 → 二分查找！这就得到 O(n log n) 的解法。
4. **能否更快？** 观察：当我们把右边界 right 右移，窗口和增大；当窗口和已经 ≥ target 时，左边界 left 可以右移来缩短窗口。左右指针都只往右走，各走 n 步 → O(n)。这就是**滑动窗口**。
5. **为什么滑动窗口正确？** 因为所有元素为正，窗口扩大 → 和增大，窗口缩小 → 和减小，具有**单调性**。当 left 右移后和 < target，说明以当前 left 为起点的更短窗口不可能满足，不会漏解。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 双重循环枚举所有子数组 | O(n²) | O(1) | 能说出即可 |
| 前缀和 + 二分 | 前缀和递增，对每个起点二分找最短终点 | O(n log n) | O(n) | 加分项（Follow-up） |
| 滑动窗口 | 右扩左缩，维护窗口和 | O(n) | O(1) | ⭐ 必须写出 |

## 关键提示

1. **所有元素都是正整数**——这是核心条件。如果有负数，前缀和不单调，滑动窗口和二分都不能用。想想为什么？
2. **滑动窗口的收缩条件**：不是 `if` 而是 `while`。窗口和 ≥ target 后，可能连续收缩多次左边界都仍然满足条件。
3. **前缀和数组的长度**：prefix[0] = 0，prefix[i] = nums[0] + ... + nums[i-1]，长度为 n+1。区间 [i, j] 的和 = prefix[j+1] - prefix[i]。
4. **二分查找的目标**：对于起点 i，在 prefix[i+1..n] 中找第一个 ≥ prefix[i] + target 的位置，用 `lower_bound`。
5. 初始化 `ans = INT_MAX`（或 n+1），最后如果还是 INT_MAX 说明没有满足条件的子数组，返回 0。

```
滑动窗口过程示意 (target=7, nums=[2,3,1,2,4,3]):

Step1: [2] 3 1 2 4 3    sum=2  < 7, right++
Step2: [2 3] 1 2 4 3    sum=5  < 7, right++
Step3: [2 3 1] 2 4 3    sum=6  < 7, right++
Step4: [2 3 1 2] 4 3    sum=8  ≥ 7, len=4, 收缩left
Step5:  2[3 1 2] 4 3    sum=6  < 7, right++
Step6:  2[3 1 2 4] 3    sum=10 ≥ 7, len=4, 收缩left
Step7:  2 3[1 2 4] 3    sum=7  ≥ 7, len=3, 收缩left
Step8:  2 3 1[2 4] 3    sum=6  < 7, right++
Step9:  2 3 1[2 4 3]    sum=9  ≥ 7, len=3, 收缩left
Step10: 2 3 1 2[4 3]    sum=7  ≥ 7, len=2 ← 最优! 收缩left
Step11: 2 3 1 2 4[3]    sum=3  < 7, right++ → 越界结束

答案: 2
```

## 解法详解

### 解法1: 暴力枚举 — O(n²) / O(1)

**思考过程**：最直觉的做法——枚举每个起点 i，从 i 开始累加，一旦和 ≥ target 就记录长度并 break（因为继续往后只会更长）。

```cpp
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int n = nums.size();
        int ans = INT_MAX;
        for (int i = 0; i < n; i++) {
            int sum = 0;
            for (int j = i; j < n; j++) {
                sum += nums[j];
                if (sum >= target) {
                    ans = min(ans, j - i + 1);
                    break; // 已找到以i开头的最短，更长的没意义
                }
            }
        }
        return ans == INT_MAX ? 0 : ans;
    }
};
```

**关键点**：内层循环一旦满足就 break，因为再加只会更长。但最坏情况仍然 O(n²)（比如 target 很大，每次都要加到末尾）。

---

### 解法2: 滑动窗口 — O(n) / O(1) ⭐ 面试首选

**从解法1优化**：解法1中，当起点从 i 移到 i+1 时，我们丢掉了 nums[i] 的信息，又从头开始累加。但实际上，如果 [i, j] 的和 ≥ target，那 [i+1, j] 的和只是减了 nums[i]，不需要重新算！这就是滑动窗口的核心——**复用已计算的区间信息**。

```cpp
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int n = nums.size();
        int ans = INT_MAX;
        int left = 0, sum = 0;
        
        for (int right = 0; right < n; right++) {
            sum += nums[right];  // 扩大窗口
            
            // 窗口和满足条件时，尝试收缩左边界
            while (sum >= target) {
                ans = min(ans, right - left + 1);
                sum -= nums[left]; // 收缩窗口
                left++;
            }
        }
        
        return ans == INT_MAX ? 0 : ans;
    }
};
```

**关键点**：
- `while` 不是 `if`！满足条件后左边界可能连续收缩多次。
- left 和 right 都只向右移动，不回退。虽然有嵌套循环，但 left 总共最多移动 n 次，所以总时间 O(n)。

---

### 解法3: 前缀和 + 二分查找 — O(n log n) / O(n)（Follow-up）

**思路**：题目 Follow-up 要求 O(n log n) 解法。构建前缀和数组，由于元素全为正，前缀和严格递增。对每个起点 i，用二分查找在前缀和数组中找到最小的 j 使得 prefix[j] - prefix[i] ≥ target。

```cpp
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int n = nums.size();
        // prefix[i] = nums[0] + nums[1] + ... + nums[i-1]
        vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
        
        int ans = INT_MAX;
        for (int i = 0; i < n; i++) {
            // 找最小的 j 使得 prefix[j] >= prefix[i] + target
            int need = prefix[i] + target;
            // 在 prefix[i+1 .. n] 中二分查找
            auto it = lower_bound(prefix.begin() + i + 1, prefix.end(), need);
            if (it != prefix.end()) {
                int j = it - prefix.begin();
                ans = min(ans, j - i); // 子数组 [i, j-1]，长度 j-i
            }
        }
        
        return ans == INT_MAX ? 0 : ans;
    }
};
```

**关键点**：
- 前缀和数组长度 n+1，prefix[0] = 0。
- 二分找的是 `prefix[j] >= prefix[i] + target`，用 `lower_bound`。
- 子数组是 nums[i..j-1]，长度 j - i（不是 j - i + 1，因为前缀和的索引差就是元素个数）。

```
前缀和二分过程示意 (target=7, nums=[2,3,1,2,4,3]):
prefix = [0, 2, 5, 6, 8, 12, 15]

i=0: need=0+7=7, lower_bound在prefix[1..6]找≥7 → prefix[4]=8, len=4-0=4
i=1: need=2+7=9, lower_bound找≥9 → prefix[5]=12, len=5-1=4
i=2: need=5+7=12, lower_bound找≥12 → prefix[5]=12, len=5-2=3
i=3: need=6+7=13, lower_bound找≥13 → prefix[6]=15, len=6-3=3
i=4: need=8+7=15, lower_bound找≥15 → prefix[6]=15, len=6-4=2 ← 最优!
i=5: need=12+7=19, lower_bound找≥19 → 不存在

答案: 2
```

## 解法对比

| | 暴力 O(n²) | 滑动窗口 O(n) | 前缀和+二分 O(n log n) |
|---|---|---|---|
| 核心操作 | 双重循环 | 双指针同向移动 | 前缀和 + lower_bound |
| 空间 | O(1) | O(1) | O(n) |
| 适用条件 | 任何情况 | 元素全正（窗口和单调） | 元素全正（前缀和递增） |
| 面试场景 | 讲思路起点 | **首选写法** | Follow-up 追问时写 |
| 如果有负数？ | 仍可用 | ❌ 不能用 | ❌ 不能用（需要单调队列） |

**什么时候选哪个？**
- 面试默认写滑动窗口，O(n) 且代码简洁。
- 面试官追问 O(n log n) 时写前缀和+二分。
- 如果数组含负数，本题方法全部失效，需要用**单调双端队列**优化（LC 862）。

## 易错点

1. **收缩用 `if` 而不是 `while`**
   - ✗ `if (sum >= target) { ans = min(...); sum -= nums[left++]; }`
   - ✓ `while (sum >= target) { ans = min(...); sum -= nums[left++]; }`
   - 原因：满足条件后可能连续收缩多次仍满足，`if` 只收缩一次，会错过更短的答案。例如 target=4, nums=[1,1,1,1,4]，当 right 指向 4 时 sum=8，需要连续收缩 left 才能找到 len=1。

2. **前缀和索引错位**
   - ✗ `prefix[i] = nums[0] + ... + nums[i]`，然后 `lower_bound(prefix.begin() + i, ...)`
   - ✓ `prefix[i] = nums[0] + ... + nums[i-1]`（prefix[0]=0），然后 `lower_bound(prefix.begin() + i + 1, ...)`
   - 原因：前缀和的标准定义中 prefix[0]=0 代表空前缀，区间 [i, j] 的和 = prefix[j+1] - prefix[i]。

3. **忘记处理无解情况**
   - ✗ 直接返回 `ans`
   - ✓ `return ans == INT_MAX ? 0 : ans;`
   - 原因：如果所有元素之和 < target，不存在满足条件的子数组。

4. **二分查找中子数组长度算错**
   - ✗ `ans = min(ans, j - i + 1)` （前缀和版本）
   - ✓ `ans = min(ans, j - i)` （前缀和版本）
   - 原因：prefix[j] - prefix[i] 对应的子数组是 nums[i..j-1]，长度恰好是 j - i。

## 面试追问

**Q1: 暴力解的时间复杂度是多少？瓶颈在哪？**
> O(n²)。对每个起点 i 都要线性扫描找最短的满足条件的终点 j。瓶颈在于"对每个起点，重新从头累加"。

**Q2: 滑动窗口为什么是 O(n) 而不是 O(n²)？明明有嵌套循环。**
> 虽然形式上是 for 套 while，但 left 指针在整个过程中最多从 0 移动到 n，不会回退。所以 while 循环的总执行次数不超过 n 次。right 走 n 步 + left 走 n 步 = O(2n) = O(n)。

**Q3: 如果数组中有负数，滑动窗口还能用吗？怎么办？**
> 不能。因为加入负数后，窗口和不再单调——扩大窗口和可能减小，收缩窗口和可能增大，滑动窗口的正确性前提被破坏。解决方案是用**单调双端队列（deque）+ 前缀和**，维护一个前缀和递增的队列，时间仍然 O(n)。参考 LC 862。

**Q4: Follow-up 为什么要 O(n log n)？有什么场景比 O(n) 解法更合适？**
> 这个 Follow-up 主要考察你是否掌握前缀和+二分这个思路。实际性能上 O(n) 更优。但前缀和+二分的框架更通用——如果要处理离线查询（多个不同 target 查同一个数组），前缀和只需建一次，每次查询 O(n log n)。

## 相关题型

- **76. 最小覆盖子串** — 同样的滑动窗口框架，区别：窗口满足条件的判断从"和≥target"变成"包含所有目标字符"，用哈希表计数代替求和。
- **862. 和至少为 K 的最短子数组** — 本题的升级版，数组含负数，滑动窗口失效，需要用单调双端队列+前缀和，O(n)。学会本题后做 862 可以复用前缀和的思路，核心区别在于前缀和不单调时如何维护。
- **3. 无重复字符的最长子串** — 滑动窗口经典题，区别：本题找最短满足条件的窗口，3 找最长满足条件的窗口，收缩条件相反。
- **713. 乘积小于 K 的子数组** — 滑动窗口变体，把"和"改成"乘积"，单调性仍成立（正整数乘积单调递增）。