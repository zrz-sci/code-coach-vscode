/*
 * LeetCode 315: Count of Smaller Numbers After Self (计算右侧小于当前元素的个数)
 *
 * 【题目本质】
 * 对数组中每个元素，统计其右侧严格小于它的元素个数。
 * 核心问题：如何在排序/处理过程中"顺便"统计逆序对？
 *
 * 【解法总览】
 * 解法1: 归并排序(带索引追踪) — O(n log n) / O(n)   — 面试首选，merge阶段统计
 * 解法2: 树状数组(BIT)       — O(n log M) / O(M)   — 从右往左扫描+BIT前缀查询
 * 解法3: 线段树              — O(n log M) / O(M)   — 更通用但代码量大
 */

// ============================================================
// 解法1: 归并排序（带索引追踪） — 面试首选 ⭐
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 逆序对经典方法：在归并排序的 merge 阶段统计。
// 合并 left[] 和 right[] 时，当 left[i] 放入结果，
// right 中已经放入结果的元素个数就是"右侧比 left[i] 小"的贡献。
//
// 关键：nums 排序的同时，index[] 也要同步排序，
// 这样才能把统计结果写入正确的 counts[原始位置]。
//
// 示例: nums = [5, 2, 6, 1], index = [0, 1, 2, 3]
//
// merge [5] 和 [2]:
//   2 < 5, 先放2(右边) → 右已放1个
//   放5(左边), counts[0] += 1
//   结果: [2, 5], index = [1, 0]
//
// merge [6] 和 [1]:
//   1 < 6, 先放1(右边) → 右已放1个
//   放6(左边), counts[2] += 1
//   结果: [1, 6], index = [3, 2]
//
// merge [2, 5] 和 [1, 6]:
//   1 < 2, 先放1 → 右已放1个
//   2 < 6, 放2(左), counts[1] += 1  (1比2小)
//   5 < 6, 放5(左), counts[0] += 1  (1比5小)
//   放6
//
// 最终: counts = [2, 1, 1, 0] ✓
// ============================================================
class Solution1 {
public:
    vector<int> countSmaller(vector<int>& nums) {
        int n = nums.size();
        vector<int> counts(n, 0);
        vector<int> index(n);
        iota(index.begin(), index.end(), 0); // index = [0, 1, ..., n-1]

        // 预分配临时数组，避免每次 merge 都创建
        vector<int> tempNums(n), tempIndex(n);
        mergeSort(nums, index, counts, tempNums, tempIndex, 0, n - 1);
        return counts;
    }

private:
    void mergeSort(vector<int>& nums, vector<int>& index,
                   vector<int>& counts,
                   vector<int>& tN, vector<int>& tI,
                   int left, int right) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;
        mergeSort(nums, index, counts, tN, tI, left, mid);
        mergeSort(nums, index, counts, tN, tI, mid + 1, right);
        merge(nums, index, counts, tN, tI, left, mid, right);
    }

    void merge(vector<int>& nums, vector<int>& index,
               vector<int>& counts,
               vector<int>& tN, vector<int>& tI,
               int left, int mid, int right) {
        int i = left, j = mid + 1, k = left;

        while (i <= mid && j <= right) {
            if (nums[i] <= nums[j]) {
                // 左边元素 <= 右边元素，放入左边的
                // j - (mid+1) = 右半已放入结果的个数（都比 nums[i] 小）
                counts[index[i]] += (j - mid - 1);
                tN[k] = nums[i];
                tI[k] = index[i];
                i++;
            } else {
                // 右边元素更小，先放入右边的
                tN[k] = nums[j];
                tI[k] = index[j];
                j++;
            }
            k++;
        }

        // 左半剩余 — 右半已经全部放入，j = right + 1
        while (i <= mid) {
            counts[index[i]] += (j - mid - 1); // 别忘了这里也要加！
            tN[k] = nums[i];
            tI[k] = index[i];
            i++; k++;
        }

        // 右半剩余 — 不需要加 counts（右半元素不会对左半产生贡献）
        while (j <= right) {
            tN[k] = nums[j];
            tI[k] = index[j];
            j++; k++;
        }

        // 拷贝回原数组
        for (int p = left; p <= right; p++) {
            nums[p] = tN[p];
            index[p] = tI[p];
        }
    }
};

// ============================================================
// 解法2: 树状数组 (Binary Indexed Tree / BIT)
// 时间: O(n log M)  空间: O(M), M = 值域范围 = 20002
//
// 【思路】
// 从右往左遍历数组：
//   1. query(val - 1): 查询已有元素中值 < val 的个数
//   2. update(val): 将当前值加入树状数组
//
// 由于 nums[i] 属于 [-10^4, 10^4]，而 BIT 下标从 1 开始，
// 需要偏移: val = nums[i] + 10001, 映射到 [1, 20001]
//
// 示例: nums = [5, 2, 6, 1]
// 偏移后: [10006, 10003, 10007, 10002]
//
// i=3: val=10002, query(10001)=0 → counts[3]=0, update(10002)
// i=2: val=10007, query(10006)=1 → counts[2]=1  (已有10002)
// i=1: val=10003, query(10002)=1 → counts[1]=1  (已有10002)
// i=0: val=10006, query(10005)=2 → counts[0]=2  (已有10002,10003)
//
// 结果: [2, 1, 1, 0] ✓
//
// BIT 核心操作:
//   update(i): 从 i 开始，沿 i += lowbit(i) 路径加1
//   query(i):  从 i 开始，沿 i -= lowbit(i) 路径求和
//   lowbit(i) = i & (-i): 取 i 的最低位1
// ============================================================
class Solution2 {
public:
    vector<int> countSmaller(vector<int>& nums) {
        int n = nums.size();
        int offset = 10001;   // 偏移量: -10^4 → 1
        int size = 20002;     // BIT 大小: [1, 20001] + 1
        vector<int> tree(size, 0);

        vector<int> counts(n);
        // 从右往左遍历：先 query 再 update
        for (int i = n - 1; i >= 0; i--) {
            int val = nums[i] + offset;
            // 查询值 < val 的元素个数（前缀和 [1, val-1]）
            counts[i] = query(tree, val - 1);
            // 将当前值加入 BIT
            update(tree, val, size);
        }
        return counts;
    }

private:
    // BIT 单点更新: 在位置 i 上 +1
    void update(vector<int>& tree, int i, int size) {
        for (; i < size; i += i & (-i)) {
            tree[i]++;
        }
    }

    // BIT 前缀查询: 求 tree[1..i] 的和
    int query(vector<int>& tree, int i) {
        int sum = 0;
        for (; i > 0; i -= i & (-i)) {
            sum += tree[i];
        }
        return sum;
    }
};

// ============================================================
// 解法3: 线段树
// 时间: O(n log M)  空间: O(4M)
//
// 【思路】
// 与 BIT 类似，从右往左遍历。
// 线段树维护值域 [1, 20001] 中每个值出现的次数。
// query(1, val-1) 查询值域 [1, val-1] 的总出现次数。
// update(val) 将 val 出现次数 +1。
//
// 线段树比 BIT 更通用（支持区间修改、区间查询），
// 但代码量大、常数大。面试中除非要求否则不写。
// ============================================================
class Solution3 {
public:
    vector<int> countSmaller(vector<int>& nums) {
        int n = nums.size();
        int offset = 10001;
        int maxVal = 20001;
        vector<int> seg(4 * (maxVal + 1), 0);

        vector<int> counts(n);
        for (int i = n - 1; i >= 0; i--) {
            int val = nums[i] + offset;
            // 查询值域 [1, val-1] 的元素总数
            counts[i] = (val > 1) ? query(seg, 1, 1, maxVal, 1, val - 1) : 0;
            // 将当前值加入线段树
            update(seg, 1, 1, maxVal, val);
        }
        return counts;
    }

private:
    void update(vector<int>& seg, int node, int lo, int hi, int pos) {
        if (lo == hi) {
            seg[node]++;
            return;
        }
        int mid = (lo + hi) / 2;
        if (pos <= mid) update(seg, 2 * node, lo, mid, pos);
        else update(seg, 2 * node + 1, mid + 1, hi, pos);
        seg[node] = seg[2 * node] + seg[2 * node + 1];
    }

    int query(vector<int>& seg, int node, int lo, int hi, int l, int r) {
        if (l > r || lo > r || hi < l) return 0;
        if (l <= lo && hi <= r) return seg[node];
        int mid = (lo + hi) / 2;
        return query(seg, 2 * node, lo, mid, l, r) +
               query(seg, 2 * node + 1, mid + 1, hi, l, r);
    }
};

// ============================================================
// 【解法对比】
//
// | 解法        | 时间       | 空间   | 值域依赖 | 代码量 |
// |-------------|-----------|--------|---------|--------|
// | 归并排序    | O(nlogn)  | O(n)   | 无      | 中等   |
// | 树状数组    | O(nlogM)  | O(M)   | 有      | 短     |
// | 线段树      | O(nlogM)  | O(4M)  | 有      | 长     |
//
// 面试首选归并排序：不受值域限制，展示分治思想。
// 树状数组是有价值的备选，代码短且好写。
//
// ============================================================
// 【易错点】
//
// 1. 归并排序 <= vs < 判断:
//    ✗ if (nums[i] < nums[j])  → 相等时右边先放入，等于的也被计入
//    ✓ if (nums[i] <= nums[j]) → 相等时左边先放入，不计入
//
// 2. 左半剩余元素忘记加 counts:
//    ✗ 只在 while(i<=mid && j<=right) 里加了 counts
//    ✓ while(i<=mid) 里也要 counts[index[i]] += (j - mid - 1)
//
// 3. BIT 偏移量算错:
//    ✗ offset = 10000 → -10^4 + 10000 = 0, BIT 下标不能为0
//    ✓ offset = 10001 → 最小值映射到1
//
// 4. BIT query(val) 而非 query(val-1):
//    ✗ query(val) → 包含等于 nums[i] 的元素
//    ✓ query(val - 1) → 严格小于
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 这道题和"逆序对"有什么关系？
//     counts[i] 就是以 i 为左端点的逆序对个数。
//     经典逆序对问题（剑指Offer 51）只需总数，本题需要每个位置的。
//
// Q2: 如果值域非常大（10^9），BIT 还能用吗？
//     需要离散化：对所有值排序去重，映射到 [1, n]。
//     归并排序不受值域影响，这是它的优势。
//
// Q3: 能否用 BST 来解？
//     可以，从右往左插入 BST，利用左子树大小统计。
//     但普通 BST 最坏 O(n^2)，需要平衡树保证 O(nlogn)。
//     面试中不推荐，除非你能现场写平衡树。
//
// ============================================================
// 【相关题型】
//
// - 剑指 Offer 51. 数组中的逆序对:
//   本题的简化版，只需逆序对总数。归并排序中改为全局计数器。
//
// - 493. 翻转对 (Reverse Pairs):
//   条件变为 nums[i] > 2*nums[j]，归并排序变体。
//   merge 前先单独扫描统计，再正常 merge。
//
// - 327. 区间和的个数:
//   归并排序统计满足条件的前缀和对数，思路相同。
// ============================================================
