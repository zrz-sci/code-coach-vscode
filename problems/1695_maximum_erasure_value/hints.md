# 1695. 删除子数组的最大得分

## 核心思路

找**元素不重复的连续子数组的最大元素和**。经典**滑动窗口 + 哈希集合**问题——维护一个 [left, right] 窗口，保证窗口内无重复元素，同时追踪窗口元素和的最大值。与 LC 3（最长无重复子串）几乎相同，只是目标从"最大长度"变成"最大和"。

## 滑动窗口过程图解

```
示例: nums = [4, 2, 4, 5, 6]，找元素不重复的子数组的最大和

Step 1: right=0, nums[0]=4
  窗口: [4]          seen={4}  sum=4   ans=4
        ^L ^R

Step 2: right=1, nums[1]=2, 2不在seen中
  窗口: [4, 2]       seen={4,2}  sum=6   ans=6
        ^L    ^R

Step 3: right=2, nums[2]=4, 4在seen中! → 收缩左边界
  收缩: 移除nums[0]=4, left=1
  窗口: [2, 4]       seen={2,4}  sum=6   ans=6
           ^L  ^R
  (此时4不在seen了，可以加入)
  窗口: [2, 4]       seen={2,4}  sum=6   ans=6

  更详细的过程:
    right=2, nums[2]=4
    while seen.count(4):         ← 4在seen中
      seen.erase(nums[0]=4)      ← 移除4
      sum -= 4  → sum=2
      left++ → left=1
    seen.insert(4), sum+=4 → sum=6, ans=max(6,6)=6

Step 4: right=3, nums[3]=5, 5不在seen中
  窗口: [2, 4, 5]    seen={2,4,5}  sum=11  ans=11
           ^L     ^R

Step 5: right=4, nums[4]=6, 6不在seen中
  窗口: [2, 4, 5, 6] seen={2,4,5,6}  sum=17  ans=17
           ^L        ^R

最终 ans = 17，对应子数组 [2,4,5,6]
```

```
示例: nums = [5, 2, 1, 2, 5, 2, 1, 2, 5]

Step 1-3: right=0,1,2
  [5, 2, 1]  sum=8  ans=8
   ^L     ^R

Step 4: right=3, nums[3]=2, 2在seen中
  收缩: 移除5(left=0→1), 2还在? → 移除2(left=1→2)
  等等, while循环:
    seen.count(2)=true → erase(nums[0]=5), sum=3, left=1
    seen.count(2)=true → erase(nums[1]=2), sum=1, left=2
    seen.count(2)=false → 退出while
  加入2: seen={1,2}, sum=3, ans=max(8,3)=8
  窗口: [1, 2]
            ^L  ^R

Step 5: right=4, nums[4]=5, 5不在seen中
  [1, 2, 5]  sum=8  ans=8
      ^L     ^R

Step 6: right=5, nums[5]=2, 2在seen中
  收缩: erase(1), sum=7, left=3; 2还在 → erase(2), sum=5, left=4
  加入2: [5, 2]  sum=7  ans=8
              ^L  ^R

Step 7: right=6, nums[6]=1, 不重复
  [5, 2, 1]  sum=8  ans=8
      ^L     ^R

Step 8: right=7, nums[7]=2, 2在seen中
  收缩: erase(5), sum=3, left=5; 2还在 → erase(2), sum=1, left=6
  加入2: [1, 2]  sum=3  ans=8

Step 9: right=8, nums[8]=5, 不重复
  [1, 2, 5]  sum=8  ans=8

最终 ans = 8，对应子数组 [5,2,1] 或 [1,2,5]
```

## 思维链

1. **读完题第一反应** → "删除一个子数组使得所有元素唯一，得分为子数组之和"。等价于找**元素不重复的连续子数组**的最大和。
2. **暴力做法** → 枚举所有子数组 O(n²)，对每个子数组检查是否无重复 O(n)。总 O(n³)。太慢。
3. **优化方向** → 想到 LC 3（最长无重复子串）用滑动窗口 O(n) 解决。本题只是把"最大长度"换成"最大和"，窗口维护逻辑完全相同。
4. **滑动窗口** → 维护 [left, right] 窗口，`unordered_set<int> seen` 记录窗口内元素，`sum` 记录窗口内元素和。
5. **扩展右边界** → 每步 right 右移一位，尝试加入 `nums[right]`。
6. **收缩左边界** → 如果 `nums[right]` 已在 seen 中，不断移除 `nums[left]` 并 left++，直到窗口内不再包含 `nums[right]`。
7. **更新答案** → 每次扩展后，`ans = max(ans, sum)`。
8. **时间分析** → left 和 right 各最多移动 n 次，总操作 O(2n) = O(n)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试推荐 |
|------|------|------|------|----------|
| 滑窗 + unordered_set | set 判重 | O(n) | O(n) | 推荐 |
| 滑窗 + 数组计数 | cnt[val]++ 判重 | O(n) | O(max_val) | 常数更快 |
| 滑窗 + 前缀和 | 前缀和替代累加 | O(n) | O(n) | 写法变体 |

## 关键提示

1. **提示1 — 类比 LC 3**: 本题和 LC 3（最长无重复子串）几乎一模一样。LC 3 求最大**长度**，本题求最大**和**。窗口收缩/扩展逻辑完全相同，只是 `ans = max(ans, right-left+1)` 变成 `ans = max(ans, sum)`。
2. **提示2 — 窗口不变式**: 任何时候，窗口 [left, right] 内的元素**互不相同**。这是通过 while 循环收缩来保证的。
3. **提示3 — 用数组代替 set**: 元素值范围 [1, 10^4]，可以用 `int cnt[10001]` 代替 `unordered_set`，每次 `cnt[nums[right]]++`，判重条件变为 `cnt[nums[right]] > 1`。常数因子更小。
4. **提示4 — 前缀和写法**: 可以用前缀和数组代替累加变量 `sum`。窗口和 = `prefix[right+1] - prefix[left]`。但累加 `sum` 更直接。
5. **提示5 — 为什么是 O(n)**: left 和 right 各从 0 走到 n-1，left 永远不回退。虽然有嵌套的 while 循环，但 left 总移动次数 <= n，所以总操作 O(2n) = O(n)。

## 解法详解

### 解法1: 滑动窗口 + unordered_set -- O(n) / O(n) -- 面试推荐

```cpp
class Solution {
public:
    int maximumUniqueSubarray(vector<int>& nums) {
        unordered_set<int> seen;
        int left = 0, sum = 0, ans = 0;

        for (int right = 0; right < (int)nums.size(); right++) {
            // 收缩: 如果 nums[right] 已在窗口中, 从左边不断移除
            while (seen.count(nums[right])) {
                seen.erase(nums[left]);
                sum -= nums[left];
                left++;
            }
            // 扩展: 加入 nums[right]
            seen.insert(nums[right]);
            sum += nums[right];
            // 更新最大和
            ans = max(ans, sum);
        }
        return ans;
    }
};
```

**关键点**:
- `while` 而不是 `if`：可能需要连续收缩多步才能移除重复元素。例如窗口 `[5,2,1]`，right 指向 2，需要移除 5 和 2 才行。
- `seen.count()` 的时间是均摊 O(1)（哈希表查找）。
- `sum` 随窗口实时维护，无需每次重新计算。

---

### 解法2: 滑动窗口 + 数组计数 -- O(n) / O(max_val) -- 常数更快

```cpp
class Solution {
public:
    int maximumUniqueSubarray(vector<int>& nums) {
        int cnt[10001] = {};  // 元素值 1~10^4
        int left = 0, sum = 0, ans = 0;

        for (int right = 0; right < (int)nums.size(); right++) {
            cnt[nums[right]]++;
            sum += nums[right];

            // 收缩: 如果 nums[right] 出现了不止一次
            while (cnt[nums[right]] > 1) {
                cnt[nums[left]]--;
                sum -= nums[left];
                left++;
            }
            ans = max(ans, sum);
        }
        return ans;
    }
};
```

**关键点**:
- 元素值范围 [1, 10^4]，用数组 `cnt[10001]` 比 `unordered_set` 更快（避免哈希计算和内存分配）。
- 先 `cnt[nums[right]]++`，再检查是否 > 1，最后在 while 中收缩。
- 注意收缩条件是 `cnt[nums[right]] > 1`，不是 `cnt[nums[left]] > 1`——我们要移除的是 right 端元素的重复。

---

### 解法3: 滑动窗口 + 前缀和 -- O(n) / O(n)

```cpp
class Solution {
public:
    int maximumUniqueSubarray(vector<int>& nums) {
        int n = nums.size();
        vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++)
            prefix[i+1] = prefix[i] + nums[i];

        unordered_set<int> seen;
        int left = 0, ans = 0;

        for (int right = 0; right < n; right++) {
            while (seen.count(nums[right])) {
                seen.erase(nums[left]);
                left++;
            }
            seen.insert(nums[right]);
            // 窗口和 = prefix[right+1] - prefix[left]
            ans = max(ans, prefix[right+1] - prefix[left]);
        }
        return ans;
    }
};
```

## 易错点

1. **用 `if` 代替 `while` 收缩**:
   - 错误: `if (seen.count(nums[right]))` 只移除一个元素 → 可能没有完全移除重复
   - 正确: `while (seen.count(nums[right]))` 持续收缩直到无重复

2. **解法2中收缩条件搞反**:
   - 错误: `while (cnt[nums[left]] > 1)` → 可能 left 的元素只出现一次但 right 的元素重复，不收缩
   - 正确: `while (cnt[nums[right]] > 1)` → 对准 right 端的重复元素

3. **忘记在收缩时更新 sum**:
   - 错误: `left++` 但不减 `sum -= nums[left-1]` → sum 不正确
   - 正确: 先 `sum -= nums[left]`，再 `left++`

4. **数组大小不够**:
   - 错误: `int cnt[10000]` → 元素值可达 10^4，`cnt[10000]` 越界
   - 正确: `int cnt[10001]` 或 `int cnt[10005]`

5. **混淆"最大长度"和"最大和"**:
   - 错误: 照搬 LC 3 的 `ans = max(ans, right-left+1)` → 求的是长度不是和
   - 正确: `ans = max(ans, sum)`

## 面试追问

**Q1: 为什么 while 循环不会导致时间退化到 O(n^2)？**
> left 指针只向右移动，永不回退。在整个算法执行过程中，left 总共从 0 移到最多 n-1，while 中每次 left++ 的总次数 <= n。所以 right 移动 n 次 + left 总移动 n 次 = O(2n) = O(n)。

**Q2: 和 LC 3 (Longest Substring Without Repeating Characters) 有什么区别？**
> 窗口维护逻辑完全相同（seen + while 收缩）。区别仅在目标：LC 3 求 max(right-left+1)，本题求 max(sum)。如果面试中先做过 LC 3，本题可以直接说"复用 LC 3 的框架，把 max 长度改成 max 和"。

**Q3: 如果元素可以为负数呢？**
> 滑动窗口失效！因为加入负数元素可能让和变小，但后续可能通过加入更多元素让和变大。窗口收缩的单调性前提（移除元素让和变小）不成立。需要用其他方法（如 DP 或更复杂的数据结构）。

**Q4: 如果要返回子数组本身（而不只是和）呢？**
> 在更新 `ans` 时同时记录 `best_left = left, best_right = right`。最后返回 `nums[best_left..best_right]`。

**Q5: unordered_set 的最坏时间是 O(n)（哈希冲突），怎么保证效率？**
> 实际中几乎不会发生极端冲突。如果追求严格最坏 O(n)，用数组计数法（解法2），`cnt[val]` 的增删是严格 O(1)。

## 相关题型

- **3. 无重复字符的最长子串** — 本题的"模板题"，窗口逻辑完全相同，只是目标从最大长度变最大和。
- **209. 长度最小的子数组** — 另一个滑动窗口经典题，但目标是最小长度使和 >= target（求最小 vs 求最大）。
- **992. K 个不同整数的子数组** — 滑动窗口进阶，需要"恰好 K 个不同 = 最多 K 个 - 最多 K-1 个"的技巧。
- **904. 水果成篮** — 最多 2 种元素的最长子数组，也是滑动窗口 + 计数。
- **2461. 长度为 K 子数组中的最大和** — 固定窗口大小 K，元素不重复的最大和。本题是可变窗口大小的推广。
