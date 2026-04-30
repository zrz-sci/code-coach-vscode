/*
 * 【题目本质】
 * 统计所有区间和落在 [lower, upper] 的子数组个数。
 * 转化为前缀和之差: lower <= prefix[j] - prefix[i] <= upper (i < j)。
 * 本质是"逆序对变体" — 在排序过程中统计满足范围条件的有序对。
 *
 * 【解法总览】
 * 解法1: 归并排序          — O(n log n)/O(n) ⭐推荐
 * 解法2: BIT + 离散化       — O(n log n)/O(n)
 */

// ===================== 解法1: 归并排序 =====================
// 思路: 将前缀和数组归并排序，在合并阶段用双指针统计跨越中点的满足条件的对
//       对于右半段每个 prefix[j]，在已排序的左半段中找
//       满足 prefix[j]-upper <= prefix[i] <= prefix[j]-lower 的 i 的个数
// 时间: O(n log n)  空间: O(n)
class Solution1 {
public:
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        int n = nums.size();
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
        return mergeSort(prefix, 0, n + 1, lower, upper);
    }

private:
    int mergeSort(vector<long long>& arr, int left, int right, int lower, int upper) {
        if (right - left <= 1) return 0;

        int mid = left + (right - left) / 2;
        int count = mergeSort(arr, left, mid, lower, upper)
                  + mergeSort(arr, mid, right, lower, upper);

        // 统计阶段: 双指针在左半段寻找范围
        // lo: 第一个 >= arr[j] - upper 的位置
        // hi: 第一个 >  arr[j] - lower 的位置
        // count += hi - lo
        int lo = left, hi = left;
        for (int j = mid; j < right; j++) {
            while (lo < mid && arr[lo] < arr[j] - upper) lo++;
            while (hi < mid && arr[hi] <= arr[j] - lower) hi++;
            count += hi - lo;
        }

        // 归并排序阶段
        vector<long long> temp(right - left);
        int p1 = left, p2 = mid, idx = 0;
        while (p1 < mid && p2 < right) {
            if (arr[p1] <= arr[p2]) temp[idx++] = arr[p1++];
            else temp[idx++] = arr[p2++];
        }
        while (p1 < mid) temp[idx++] = arr[p1++];
        while (p2 < right) temp[idx++] = arr[p2++];
        copy(temp.begin(), temp.end(), arr.begin() + left);

        return count;
    }
};

// ===================== 解法2: BIT + 离散化 =====================
// 思路: 从左到右遍历前缀和，每次查询 BIT 中 [prefix[i]-upper, prefix[i]-lower] 的计数
//       然后将 prefix[i] 插入 BIT。值域大需离散化。
// 时间: O(n log n)  空间: O(n)
class Solution2 {
public:
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        int n = nums.size();
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++)
            prefix[i + 1] = prefix[i] + nums[i];

        // 离散化: 收集所有可能查询的值
        set<long long> allVals(prefix.begin(), prefix.end());
        for (auto& p : prefix) {
            allVals.insert(p - lower);
            allVals.insert(p - upper);
        }
        unordered_map<long long, int> ranks;
        int rank = 1;
        for (long long v : allVals) ranks[v] = rank++;

        vector<int> bit(rank + 1, 0);
        int count = 0;

        for (int i = 0; i <= n; i++) {
            int lo = ranks[prefix[i] - upper];
            int hi = ranks[prefix[i] - lower];
            count += query(bit, hi) - query(bit, lo - 1);
            update(bit, ranks[prefix[i]], 1);
        }
        return count;
    }

private:
    void update(vector<int>& bit, int i, int val) {
        for (; i < (int)bit.size(); i += i & (-i)) bit[i] += val;
    }
    int query(vector<int>& bit, int i) {
        int s = 0;
        for (; i > 0; i -= i & (-i)) s += bit[i];
        return s;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 | 劣势 |
 * |------|------|------|------|------|
 * | 归并排序 | O(n log n) | O(n) | 代码自包含，无需额外数据结构 | 递归栈深 |
 * | BIT+离散化 | O(n log n) | O(n) | 在线添加查询，思路直观 | 离散化代码量 |
 *
 * 【易错点】
 * 1. prefix 用 int → 严重溢出 (nums[i] 可达 2^31-1)
 * 2. 归并时先排序再统计 → 排序破坏了左右半段的原始对应关系
 * 3. prefix 长度 n 而非 n+1 → 丢失 prefix[0]=0，漏统计从头开始的区间
 * 4. 双指针 lo/hi 的条件 < vs <= 弄反 → 计数多算或少算
 *
 * 【面试追问】
 * Q1: 为什么双指针不回退？→ j 递增时 arr[j]-upper 和 arr[j]-lower 递增
 * Q2: 能否 O(n) 解？→ 不能，本质是二维偏序统计，下界 O(n log n)
 * Q3: 和 LC 315 的关系？→ 同属"归并排序统计有序对"，315 是特殊情况 upper=INF lower=0+epsilon
 */
