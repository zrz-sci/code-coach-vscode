# 673. 最长递增子序列的个数 (Number of Longest Increasing Subsequence)

## 难度: Medium | 标签: Array, Dynamic Programming, Binary Indexed Tree, Segment Tree

---

## 核心思路

经典 LIS（最长递增子序列）的扩展：不仅求最长长度，还要**统计个数**。核心方法是维护两个数组 `len[i]` 和 `cnt[i]`：
- `len[i]`：以 `nums[i]` **结尾**的 LIS 长度
- `cnt[i]`：以 `nums[i]` **结尾**的 LIS 个数

遍历所有 `j < i` 且 `nums[j] < nums[i]`，分两种情况更新：发现更长的 / 发现等长的。

---

## 思维链

1. **读完题第一反应**：经典 LIS 用 `dp[i] = max(dp[j]+1)` 求长度，复杂度 O(n^2)。但这里还要计数——需要知道"有多少条路径到达这个最长长度"。

2. **类比思考**：就像"最短路径计数"问题：Dijkstra 求最短距离时，遇到等距路径要累加计数，遇到更短路径要重置计数。这里是"最长子序列计数"，逻辑完全对称。

3. **关键洞察**：对于每个位置 `i`，遍历所有 `j < i` 且 `nums[j] < nums[i]`：
   - 若 `len[j]+1 > len[i]`：**发现更长的** LIS，更新 `len[i] = len[j]+1`，**重置** `cnt[i] = cnt[j]`
   - 若 `len[j]+1 == len[i]`：**发现等长的** LIS，**累加** `cnt[i] += cnt[j]`
   - 若 `len[j]+1 < len[i]`：比当前短，忽略

4. **最终统计**：找到全局最长长度 `maxLen`，答案 = 所有 `len[i] == maxLen` 的 `cnt[i]` 之和。

5. **进阶思考**：O(n^2) 的 DP 是否能优化？可以用线段树或树状数组优化到 O(n log n)，但面试一般不要求。

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| ⭐ 解法1: DP 双数组 | len[] + cnt[] 双重更新 | O(n^2) | O(n) | 首选 |
| 解法2: 线段树优化 | 维护区间最长长度和对应计数 | O(n log n) | O(n) | 进阶 |

---

## 关键提示

- **提示1 (入门)**：先回忆经典 LIS 的 DP 写法。在此基础上，多维护一个"计数"数组。

- **提示2 (核心)**：区分"发现更长"和"发现等长"两种情况——前者**重置**计数，后者**累加**计数。

- **提示3 (初始化)**：每个元素自身就是长度为 1 的子序列，所以 `len[i] = 1, cnt[i] = 1`。

- **提示4 (全相同元素)**：`[2,2,2,2,2]` → 每个元素的 LIS 长度都是 1，不存在严格递增关系，答案就是 n=5。

- **提示5 (最终统计)**：不是只看最后一个位置，而是遍历所有位置，累加 `len[i] == maxLen` 的 `cnt[i]`。

---

## 解法详解

### DP 状态表（nums = [1,3,5,4,7]）

```
i:       0    1    2    3    4
nums[i]: 1    3    5    4    7
len[i]:  1    2    3    2    4     ← 以 nums[i] 结尾的 LIS 长度
cnt[i]:  1    1    1    1    2     ← 以 nums[i] 结尾的 LIS 个数

详细推导:
i=0: nums[0]=1, 无 j<0, len[0]=1, cnt[0]=1
i=1: nums[1]=3
     j=0: nums[0]=1<3, len[0]+1=2>len[1]=1 → len[1]=2, cnt[1]=cnt[0]=1
i=2: nums[2]=5
     j=0: nums[0]=1<5, len[0]+1=2>len[2]=1 → len[2]=2, cnt[2]=1
     j=1: nums[1]=3<5, len[1]+1=3>len[2]=2 → len[2]=3, cnt[2]=cnt[1]=1
i=3: nums[3]=4
     j=0: nums[0]=1<4, len[0]+1=2>len[3]=1 → len[3]=2, cnt[3]=1
     j=1: nums[1]=3<4, len[1]+1=3>len[3]=2 → len[3]=3, cnt[3]=cnt[1]=1
     j=2: nums[2]=5≥4, 跳过
i=4: nums[4]=7
     j=0: 1<7, len[0]+1=2>1 → len[4]=2, cnt[4]=1
     j=1: 3<7, len[1]+1=3>2 → len[4]=3, cnt[4]=1
     j=2: 5<7, len[2]+1=4>3 → len[4]=4, cnt[4]=cnt[2]=1
     j=3: 4<7, len[3]+1=4==4 → cnt[4]+=cnt[3]=1 → cnt[4]=2  ← 关键!

maxLen=4, 只有 len[4]==4, 答案=cnt[4]=2
两条 LIS: [1,3,5,7] 和 [1,3,4,7]
```

### DP 状态表（nums = [2,2,2,2,2]）

```
i:       0    1    2    3    4
nums[i]: 2    2    2    2    2
len[i]:  1    1    1    1    1     ← 全是1（无严格递增）
cnt[i]:  1    1    1    1    1

maxLen=1, 所有 len[i]==1
答案 = 1+1+1+1+1 = 5
```

---

### 解法1: DP 双数组 (推荐)

**思考过程**：
> "在经典 LIS 的双层循环基础上，增加一个 cnt 数组。更新 len 的同时同步更新 cnt。最后统计所有达到最大长度位置的 cnt 之和。"

```cpp
// 解法1: DP 双数组 — O(n^2) / O(n)
// len[i]: 以 nums[i] 结尾的 LIS 长度
// cnt[i]: 以 nums[i] 结尾的 LIS 个数
class Solution {
public:
    int findNumberOfLIS(vector<int>& nums) {
        int n = nums.size();
        // 初始化: 每个元素自身是长度为1的子序列, 个数为1
        vector<int> len(n, 1), cnt(n, 1);
        int maxLen = 1;

        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (nums[j] < nums[i]) {         // 严格递增
                    if (len[j] + 1 > len[i]) {
                        // 发现更长的 LIS → 重置
                        len[i] = len[j] + 1;
                        cnt[i] = cnt[j];          // 重置: 新路径
                    } else if (len[j] + 1 == len[i]) {
                        // 发现等长的 LIS → 累加
                        cnt[i] += cnt[j];          // 累加: 多条路径
                    }
                    // len[j]+1 < len[i]: 比当前短, 忽略
                }
            }
            maxLen = max(maxLen, len[i]);
        }

        // 统计所有达到最大长度的位置的计数之和
        int ans = 0;
        for (int i = 0; i < n; i++) {
            if (len[i] == maxLen) {
                ans += cnt[i];
            }
        }
        return ans;
    }
};
```

**关键点**：
- 初始化 `len[i] = 1, cnt[i] = 1`：每个元素自身是一条 LIS
- "更长"时**重置** cnt，"等长"时**累加** cnt——这是本题最核心的区分
- 最终答案不是 `cnt[n-1]`，而是所有 `len[i] == maxLen` 位置的 cnt 之和

---

### 解法2: 线段树优化 (进阶)

**思考过程**：
> "O(n^2) 的瓶颈在内层循环：对每个 i，需要查询所有 j<i 且 nums[j]<nums[i] 中的最大 len 和对应 cnt 总和。可以用线段树维护值域上的 (maxLen, totalCnt)，实现 O(log n) 查询和更新。"

```cpp
// 解法2: 线段树优化 — O(n log n) / O(n)
// 线段树节点维护 (maxLen, totalCnt)
// 查询: 在 [min_val, nums[i]-1] 范围内找最大 len 及其 cnt 总和
// 更新: 在 nums[i] 位置更新 (len[i], cnt[i])
class Solution {
public:
    // 线段树节点: (最大长度, 对应计数)
    typedef pair<int,int> PII;
    vector<PII> tree;

    // 合并两个节点
    PII merge(PII a, PII b) {
        if (a.first > b.first) return a;
        if (b.first > a.first) return b;
        return {a.first, a.second + b.second}; // 等长: 累加计数
    }

    void update(int node, int lo, int hi, int pos, PII val) {
        if (lo == hi) {
            tree[node] = merge(tree[node], val);
            return;
        }
        int mid = (lo + hi) / 2;
        if (pos <= mid) update(2*node, lo, mid, pos, val);
        else update(2*node+1, mid+1, hi, pos, val);
        tree[node] = merge(tree[2*node], tree[2*node+1]);
    }

    PII query(int node, int lo, int hi, int l, int r) {
        if (r < lo || hi < l) return {0, 1};
        if (l <= lo && hi <= r) return tree[node];
        int mid = (lo + hi) / 2;
        return merge(query(2*node, lo, mid, l, r),
                     query(2*node+1, mid+1, hi, l, r));
    }

    int findNumberOfLIS(vector<int>& nums) {
        // 离散化
        vector<int> sorted_nums = nums;
        sort(sorted_nums.begin(), sorted_nums.end());
        sorted_nums.erase(unique(sorted_nums.begin(), sorted_nums.end()), sorted_nums.end());
        int m = sorted_nums.size();

        tree.assign(4 * m, {0, 1});

        int maxLen = 0, ans = 0;
        for (int x : nums) {
            int idx = lower_bound(sorted_nums.begin(), sorted_nums.end(), x) - sorted_nums.begin();
            PII best = (idx > 0) ? query(1, 0, m-1, 0, idx-1) : PII{0, 1};
            int newLen = best.first + 1;
            int newCnt = max(best.second, 1);
            update(1, 0, m-1, idx, {newLen, newCnt});

            if (newLen > maxLen) { maxLen = newLen; ans = newCnt; }
            else if (newLen == maxLen) { ans += newCnt; }
        }
        return ans;
    }
};
```

**关键点**：
- 离散化将值域压缩到 [0, m-1]
- 线段树节点维护 (最大长度, 对应计数)
- merge 操作：较长的胜出；等长则累加计数
- 对每个元素查询 `[0, idx-1]` 区间（所有比它小的值）

---

## 解法对比

| 维度 | 解法1 DP双数组 | 解法2 线段树 |
|------|---------------|-------------|
| 时间复杂度 | O(n^2) | O(n log n) |
| 空间复杂度 | O(n) | O(n) |
| 实现难度 | 简单 | 较复杂 |
| 面试推荐 | 首选 | 提及即可 |
| 适用场景 | n <= 2000 | n 很大时 |

---

## 易错点

1. **cnt[i] 在发现更长序列时累加而非重置**
   ```cpp
   // 错误: 累加了旧的计数
   if (len[j] + 1 > len[i]) {
       len[i] = len[j] + 1;
       cnt[i] += cnt[j];  // 错! 应该是 cnt[i] = cnt[j]
   }
   // 正确: 重置为新路径的计数
   cnt[i] = cnt[j];  // 重置, 因为之前更短的路径已经无效
   ```

2. **最终答案只看最后一个位置**
   ```cpp
   // 错误: LIS 不一定以最后一个元素结尾
   return cnt[n-1];
   // 正确: 遍历所有位置, 累加 len[i]==maxLen 的 cnt[i]
   for (int i = 0; i < n; i++)
       if (len[i] == maxLen) ans += cnt[i];
   ```

3. **忘记处理全相同元素**
   ```cpp
   // nums = [2,2,2,2,2]
   // 每个元素 len[i]=1, cnt[i]=1
   // maxLen=1, 答案=5 (不是1)
   // 严格递增 => nums[j] < nums[i], 相等不算
   ```

4. **初始化 cnt[i] = 0 而非 1**
   ```cpp
   // 错误: cnt 初始化为 0
   vector<int> cnt(n, 0);  // 错! 每个元素自身是1条子序列
   // 正确:
   vector<int> cnt(n, 1);
   ```

---

## 面试追问

1. **"能否优化到 O(n log n)？"**
   - 可以。用线段树或树状数组，在值域上维护 (最大长度, 计数) 对。
   - 对每个元素查询"所有比它小的值中，最大的 len 和对应 cnt"。
   - 面试中说出思路即可，不一定需要手写。

2. **"如果改为非严格递增（允许相等）？"**
   - 把 `nums[j] < nums[i]` 改为 `nums[j] <= nums[i]`。
   - 需要注意去重：相同值的不同位置是不同的子序列。

3. **"如果要输出所有最长递增子序列？"**
   - 用回溯：从 maxLen 位置开始，逆向找所有合法的前驱，构建路径。
   - 时间复杂度与答案数量相关。

4. **"这个问题和最短路径计数有什么关系？"**
   - 完全类似：Dijkstra 的"更短路径重置计数 / 等长路径累加计数"逻辑，
     和这里的"更长子序列重置 / 等长子序列累加"逻辑是对称的。

5. **"经典 LIS 的 O(n log n) patience sort 能直接扩展吗？"**
   - 可以但复杂。需要在每个 pile（桶）中维护计数信息。
   - 推荐用线段树方案，更直观。

---

## 相关题型

| 题目 | 关联 | 复用技巧 |
|------|------|----------|
| [300. 最长递增子序列](../0300_longest_increasing_subsequence/) | LIS 基础版 | 本题是其扩展，加了计数 |
| [354. 俄罗斯套娃信封](../0354_russian_doll_envelopes/) | 二维 LIS | 排序后转化为一维 LIS |
| [334. 递增的三元子序列](../0334_increasing_triplet_subsequence/) | LIS 特例 | 只需要长度 >= 3 |
| [646. 最长数对链](../0646_maximum_length_of_pair_chain/) | 类 LIS | 贪心 + 排序 |
| [1964. 找出到每个位置为止最长的有效障碍赛跑路线](../1964_find_the_longest_valid_obstacle_course_at_each_position/) | LIS 变体 | 非严格递增 LIS |
