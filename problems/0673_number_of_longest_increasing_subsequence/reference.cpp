/*
 * LeetCode 673: 最长递增子序列的个数 (Number of Longest Increasing Subsequence)
 * 难度: Medium | 标签: Array, Dynamic Programming, Segment Tree
 *
 * 题意: 给定整数数组 nums，返回最长严格递增子序列的个数。
 *
 * 核心思路:
 *   维护两个数组:
 *   - len[i]: 以 nums[i] 结尾的 LIS 长度
 *   - cnt[i]: 以 nums[i] 结尾的 LIS 个数
 *   对每个 j < i 且 nums[j] < nums[i]:
 *     - len[j]+1 > len[i]: 发现更长, 重置 cnt
 *     - len[j]+1 == len[i]: 发现等长, 累加 cnt
 *
 * DP 状态表 (nums = [1,3,5,4,7]):
 *   i:       0    1    2    3    4
 *   nums[i]: 1    3    5    4    7
 *   len[i]:  1    2    3    3    4
 *   cnt[i]:  1    1    1    1    2
 *   maxLen=4, ans=cnt[4]=2 → [1,3,5,7] 和 [1,3,4,7]
 *
 * DP 状态表 (nums = [2,2,2,2,2]):
 *   i:       0    1    2    3    4
 *   len[i]:  1    1    1    1    1   (无严格递增)
 *   cnt[i]:  1    1    1    1    1
 *   maxLen=1, ans=5
 */


// ============================================================
// 解法1: DP 双数组 ⭐ 推荐
// 时间: O(n^2)  空间: O(n)
//
// 核心: len[i] 记录 LIS 长度, cnt[i] 记录 LIS 个数
// 更新逻辑:
//   - 更长 → 重置 cnt (新的最优路径)
//   - 等长 → 累加 cnt (多条最优路径)
// ============================================================
class Solution1 {
public:
    int findNumberOfLIS(vector<int>& nums) {
        int n = nums.size();
        // 初始化: 每个元素自身是长度为1的子序列, 个数为1
        vector<int> len(n, 1), cnt(n, 1);
        int maxLen = 1;

        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (nums[j] < nums[i]) {            // 严格递增
                    if (len[j] + 1 > len[i]) {
                        // Case 1: 发现更长的 LIS
                        len[i] = len[j] + 1;
                        cnt[i] = cnt[j];             // 重置: 新路径
                    } else if (len[j] + 1 == len[i]) {
                        // Case 2: 发现等长的 LIS
                        cnt[i] += cnt[j];            // 累加: 多条路径
                    }
                    // Case 3: len[j]+1 < len[i] → 更短, 忽略
                }
            }
            maxLen = max(maxLen, len[i]);
        }

        // 统计: 累加所有达到最大长度位置的计数
        int ans = 0;
        for (int i = 0; i < n; i++) {
            if (len[i] == maxLen) {
                ans += cnt[i];
            }
        }
        return ans;
    }
};

// ============================================================
// 解法2: 线段树优化
// 时间: O(n log n)  空间: O(n)
//
// 思路: 离散化 + 线段树维护值域上的 (maxLen, totalCnt)
// 对每个元素 nums[i]:
//   1. 查询 [0, idx-1] 区间 → 所有比 nums[i] 小的值的最优 (len, cnt)
//   2. 更新 idx 位置 → (len+1, cnt)
// merge 操作: 较长的胜出; 等长则累加计数
// ============================================================
class Solution2 {
public:
    typedef pair<int,int> PII;
    vector<PII> tree;  // 线段树节点: (最大长度, 对应计数)

    // 合并两个节点: 取较长的; 等长累加计数
    PII merge(PII a, PII b) {
        if (a.first > b.first) return a;
        if (b.first > a.first) return b;
        return {a.first, a.second + b.second};
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
        if (r < lo || hi < l) return {0, 1}; // 空区间: 长度0, 计数1
        if (l <= lo && hi <= r) return tree[node];
        int mid = (lo + hi) / 2;
        return merge(query(2*node, lo, mid, l, r),
                     query(2*node+1, mid+1, hi, l, r));
    }

    int findNumberOfLIS(vector<int>& nums) {
        int n = nums.size();
        // 离散化: 将值域压缩到 [0, m-1]
        vector<int> sorted_nums = nums;
        sort(sorted_nums.begin(), sorted_nums.end());
        sorted_nums.erase(unique(sorted_nums.begin(), sorted_nums.end()),
                          sorted_nums.end());
        int m = sorted_nums.size();

        tree.assign(4 * m, {0, 1});

        int maxLen = 0, ans = 0;
        for (int x : nums) {
            int idx = lower_bound(sorted_nums.begin(), sorted_nums.end(), x)
                      - sorted_nums.begin();
            // 查询所有比 x 小的值的最优 (len, cnt)
            PII best = (idx > 0) ? query(1, 0, m-1, 0, idx-1)
                                 : PII{0, 1};
            int newLen = best.first + 1;
            int newCnt = max(best.second, 1);
            update(1, 0, m-1, idx, {newLen, newCnt});

            if (newLen > maxLen) {
                maxLen = newLen;
                ans = newCnt;
            } else if (newLen == maxLen) {
                ans += newCnt;
            }
        }
        return ans;
    }
};

/*
 * ============================================================
 * 解法对比
 * ============================================================
 * | 解法        | 时间      | 空间 | 实现难度 | 面试推荐 |
 * |-------------|-----------|------|---------|---------|
 * | DP双数组    | O(n^2)    | O(n) | 简单    | ⭐首选   |
 * | 线段树优化   | O(n logn) | O(n) | 较复杂  | 提及即可 |
 *
 * ============================================================
 * 易错点
 * ============================================================
 * 1. 发现更长时累加而非重置:
 *    错误: cnt[i] += cnt[j]  (更长时应重置)
 *    正确: cnt[i] = cnt[j]   (新路径, 旧路径已无效)
 *
 * 2. 最终答案只看最后一个位置:
 *    错误: return cnt[n-1]
 *    正确: 遍历所有 len[i]==maxLen 的位置, 累加 cnt[i]
 *    原因: LIS 可能以任意位置结尾
 *
 * 3. 初始化 cnt 为 0:
 *    错误: vector<int> cnt(n, 0)
 *    正确: vector<int> cnt(n, 1) — 每个元素自身是一条子序列
 *
 * 4. 全相同元素漏掉:
 *    [2,2,2,2,2] → 每个 len=1, cnt=1, 答案=5
 *    严格递增 (nums[j] < nums[i]), 相等不触发更新
 *
 * ============================================================
 * 面试追问
 * ============================================================
 * Q1: 优化到 O(n log n)?
 *     A: 线段树/树状数组维护值域上 (maxLen, cnt), 见解法2。
 *
 * Q2: 非严格递增(允许相等)?
 *     A: 条件改为 nums[j] <= nums[i], 注意去重。
 *
 * Q3: 输出所有最长递增子序列?
 *     A: 回溯: 从 maxLen 位置逆向找前驱, 构建路径。
 *
 * Q4: 和最短路径计数的关系?
 *     A: 对称逻辑 — 更短路径重置/等短累加 ↔ 更长子序列重置/等长累加。
 *
 * Q5: patience sort 能扩展吗?
 *     A: 可以但复杂, 需要在每个 pile 维护计数。推荐用线段树。
 */
