/*
 * 【题目本质】
 * 在无序数组中，用线性时间找排序后相邻元素的最大差值。
 * 核心洞察：鸽巢原理 + 桶排序，最大间距一定跨越空桶。
 *
 * 【解法总览】
 * 解法1: 桶排序（桶间最大差值） O(n)/O(n) ⭐ 面试最佳
 * 解法2: 基数排序（LSD排序后扫描） O(d*n)/O(n) 更通用
 *
 * 桶排序核心图示:
 *
 *   nums = [3, 6, 9, 1], min=1, max=9, n=4
 *   bucketSize = (9-1)/(4-1) = 2
 *   bucketCount = (9-1)/2 + 1 = 5
 *
 *   桶:  [1,2]  [3,4]  [5,6]  [7,8]  [9,10]
 *         {1}    {3}    {6}    空!     {9}
 *   min:   1      3      6      -       9
 *   max:   1      3      6      -       9
 *
 *   桶间差值: 3-1=2, 6-3=3, 9-6=3
 *   最大间距 = 3
 *
 *   为什么最大间距不在桶内：
 *   桶宽=2 <= 平均间距=(9-1)/3=2.67
 *   桶内任意两元素差 < 桶宽，所以最大间距一定跨桶
 */

// ===================== 解法1: 桶排序 =====================
// 思路：利用鸽巢原理，n个数放n-1个桶必有空桶
// 为什么只存桶的min/max：最大间距一定跨桶，桶内差值不用考虑
class Solution1 {
public:
    int maximumGap(vector<int>& nums) {
        int n = nums.size();
        if (n < 2) return 0;

        int minVal = *min_element(nums.begin(), nums.end());
        int maxVal = *max_element(nums.begin(), nums.end());
        if (minVal == maxVal) return 0;  // 所有元素相同

        // 桶大小：保证最大间距 >= bucketSize（即不在桶内）
        int bucketSize = max(1, (maxVal - minVal) / (n - 1));
        int bucketCount = (maxVal - minVal) / bucketSize + 1;

        // 每个桶只需 min 和 max
        vector<int> bMin(bucketCount, INT_MAX);
        vector<int> bMax(bucketCount, INT_MIN);
        vector<bool> used(bucketCount, false);

        // 分桶
        for (int num : nums) {
            int idx = (num - minVal) / bucketSize;
            used[idx] = true;
            bMin[idx] = min(bMin[idx], num);
            bMax[idx] = max(bMax[idx], num);
        }

        // 扫描相邻非空桶的 max→min 差值
        int ans = 0;
        int prevMax = bMax[0];  // 桶0一定包含 minVal

        for (int i = 1; i < bucketCount; ++i) {
            if (!used[i]) continue;  // 跳过空桶
            // 为什么用 bMin[i] - prevMax：这是跨桶的最小可能差值
            ans = max(ans, bMin[i] - prevMax);
            prevMax = bMax[i];
        }

        return ans;
    }
};

// ===================== 解法2: 基数排序 (LSD) =====================
// 思路：按每一位从低到高排序，排完后扫描相邻差值
// 为什么不用 std::sort：题目要求线性时间
class Solution2 {
public:
    int maximumGap(vector<int>& nums) {
        int n = nums.size();
        if (n < 2) return 0;

        int maxVal = *max_element(nums.begin(), nums.end());
        vector<int> buf(n);
        long exp = 1;

        // 对每一位做计数排序
        while (maxVal / exp > 0) {
            vector<int> count(10, 0);

            for (int num : nums) {
                count[(num / exp) % 10]++;
            }

            // 前缀和：确定排序后的位置
            for (int i = 1; i < 10; ++i) {
                count[i] += count[i - 1];
            }

            // 从右到左放置保持稳定性
            for (int i = n - 1; i >= 0; --i) {
                int digit = (nums[i] / exp) % 10;
                buf[--count[digit]] = nums[i];
            }

            nums = buf;
            exp *= 10;
        }

        // 排序完成，扫描最大相邻差
        int ans = 0;
        for (int i = 1; i < n; ++i) {
            ans = max(ans, nums[i] - nums[i - 1]);
        }
        return ans;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | 桶排序 | O(n) | O(n) | 不真正排序，只看桶间差值 |
 * | 基数排序 | O(d*n) | O(n) | d最多10位，实际O(10n) |
 *
 * 【易错点】
 * 1. bucketSize 为 0 → 除零崩溃，用 max(1, ...) 保护
 * 2. 忘记 n < 2 特判 → 访问越界
 * 3. prevMax 初始化为 minVal 而非 bucketMax[0] → 桶0多元素时算错
 * 4. 基数排序忘记从右到左放置 → 不稳定，高位排序破坏低位顺序
 *
 * 【面试追问】
 * Q1: 为什么最大间距不在桶内？ → 桶宽<=平均间距，桶内差值不可能超过平均间距
 * Q2: 桶排序 vs 基数排序？ → 桶排序更巧妙，体现数学思维；基数排序更通用
 * Q3: 有负数怎么办？ → (num-minVal)保证非负，桶排序思路不变
 */
