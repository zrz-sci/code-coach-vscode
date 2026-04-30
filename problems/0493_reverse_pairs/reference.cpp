/*
 * 【题目本质】
 * 统计数组中满足 i < j 且 nums[i] > 2 * nums[j] 的对数。
 * 本质是"修改条件的逆序对"问题。
 *
 * 【解法总览】
 * 1. 归并排序 — O(n log n), 分治统计跨越左右的翻转对
 * 2. 树状数组 (BIT) — O(n log n), 离散化 + 在线查询
 */

// ===================== Solution 1: 归并排序 =====================
// 时间: O(n log n)  空间: O(n)
// 核心: 分治 -> 统计跨越左右的翻转对 -> 归并排序
class Solution1 {
public:
    int reversePairs(vector<int>& nums) {
        if (nums.empty()) return 0;
        return mergeSort(nums, 0, (int)nums.size() - 1);
    }

private:
    int mergeSort(vector<int>& nums, int left, int right) {
        if (left >= right) return 0;
        int mid = left + (right - left) / 2;
        int count = mergeSort(nums, left, mid) + mergeSort(nums, mid + 1, right);

        // Step 1: 统计翻转对 (左右各自有序, 双指针)
        int j = mid + 1;
        for (int i = left; i <= mid; i++) {
            while (j <= right && (long long)nums[i] > 2LL * nums[j]) {
                j++;
            }
            count += (j - mid - 1);
        }

        // Step 2: 归并排序
        vector<int> temp(right - left + 1);
        int p1 = left, p2 = mid + 1, k = 0;
        while (p1 <= mid && p2 <= right) {
            if (nums[p1] <= nums[p2]) temp[k++] = nums[p1++];
            else temp[k++] = nums[p2++];
        }
        while (p1 <= mid) temp[k++] = nums[p1++];
        while (p2 <= right) temp[k++] = nums[p2++];
        copy(temp.begin(), temp.end(), nums.begin() + left);

        return count;
    }
};

// ===================== Solution 2: 树状数组 (BIT) =====================
// 时间: O(n log n)  空间: O(n)
// 核心: 离散化所有值和 2x+1 值，从左到右遍历，
//       查询已插入的值中 > 2*nums[i] 的个数
class Solution2 {
public:
    int reversePairs(vector<int>& nums) {
        // 离散化: 收集 nums[i] 和 2*nums[i]+1
        vector<long long> sorted_vals;
        for (int x : nums) {
            sorted_vals.push_back(x);
            sorted_vals.push_back(2LL * x + 1);
        }
        sort(sorted_vals.begin(), sorted_vals.end());
        sorted_vals.erase(unique(sorted_vals.begin(), sorted_vals.end()), sorted_vals.end());

        int n = sorted_vals.size();
        vector<int> bit(n + 2, 0);
        int count = 0;

        for (int i = (int)nums.size() - 1; i >= 0; i--) {
            // 查询: 已插入的值中有多少 < nums[i] / 2
            // 即查询 rank(nums[i]) 之前的前缀和 (值 <= nums[i]-1 的个数? 不对)
            // 更直接: 从右往左, 查询已插入的值 v 满足 nums[i] > 2v
            // 即 v < nums[i]/2, 查询离散化后 < ceil(nums[i]/2) 的个数
            int idx = lower_bound(sorted_vals.begin(), sorted_vals.end(), (long long)nums[i]) - sorted_vals.begin() + 1;
            count += query(bit, idx - 1);

            idx = lower_bound(sorted_vals.begin(), sorted_vals.end(), 2LL * nums[i] + 1) - sorted_vals.begin() + 1;
            update(bit, idx, n);
        }
        return count;
    }

private:
    void update(vector<int>& bit, int i, int n) {
        for (; i <= n; i += i & (-i)) bit[i]++;
    }

    int query(vector<int>& bit, int i) {
        int s = 0;
        for (; i > 0; i -= i & (-i)) s += bit[i];
        return s;
    }
};

/*
 * 【解法对比】
 * Solution1 (归并排序): 代码直观, 面试首选, 和经典逆序对框架一致
 * Solution2 (BIT): 更通用, 适用于各种偏序统计, 但需要离散化
 *
 * 【易错点】
 * 1. 2 * nums[j] 溢出 int -> 必须用 (long long) 或 2LL *
 * 2. 归并排序中统计和合并必须分两步 (条件不同于排序条件)
 * 3. base case: left >= right 返回 0
 * 4. BIT 离散化时需要同时加入 x 和 2x+1
 *
 * 【面试追问】
 * Q1: 和 LC315 逆序对的区别? — 条件从 > 变为 > 2x, 统计与归并必须分离
 * Q2: 能否用平衡BST? — 可以, 用 order-statistics tree 在线维护
 * Q3: CDQ分治? — 归并排序本身就是CDQ分治的特例
 */
