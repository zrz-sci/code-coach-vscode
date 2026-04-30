/*
 * 【题目本质】
 * 摆动排序 II：将数组重排使 nums[0]<nums[1]>nums[2]<nums[3]...（严格不等）
 * 关键在于处理重复元素，避免相等元素相邻
 *
 * 【解法总览】
 * Solution1: 排序 + 逆序交替填充 — O(n log n) 时间, O(n) 空间
 * Solution2: nth_element + 虚拟索引三路划分 — O(n) 时间, O(1) 空间
 */

// ===================== Solution1: 排序 + 逆序交替填充 =====================
// 思路：排序后分为较小半和较大半，逆序交替填入偶数位和奇数位
// 逆序填充的目的：避免中位数相等的元素出现在相邻位置
// 时间 O(n log n), 空间 O(n)
class Solution1 {
public:
    void wiggleSort(vector<int>& nums) {
        int n = nums.size();
        vector<int> sorted(nums);
        sort(sorted.begin(), sorted.end());

        // 较小半: sorted[0..(n-1)/2]，逆序填入偶数位 0,2,4,...
        // 较大半: sorted[(n-1)/2+1..n-1]，逆序填入奇数位 1,3,5,...
        int small = (n - 1) / 2;  // 较小半的末尾
        int large = n - 1;        // 较大半的末尾

        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) {
                nums[i] = sorted[small--];  // 偶数位放较小值
            } else {
                nums[i] = sorted[large--];  // 奇数位放较大值
            }
        }
    }
};

// ===================== Solution2: O(n) 时间, O(1) 空间 =====================
// 思路：nth_element 找中位数 + 虚拟索引映射 + 三路划分
// 虚拟索引: (1 + 2*i) % (n|1) 把线性索引映射到摆动位置
// 三路划分: > mid 放虚拟前部(映射到奇数位), < mid 放虚拟后部(映射到偶数位)
// 时间 O(n), 空间 O(1)
class Solution2 {
public:
    void wiggleSort(vector<int>& nums) {
        int n = nums.size();

        // Step 1: 找中位数 — O(n) 平均
        auto midIt = nums.begin() + n / 2;
        nth_element(nums.begin(), midIt, nums.end());
        int mid = *midIt;

        // Step 2: 虚拟索引映射
        // i=0 -> 1, i=1 -> 3, i=2 -> 5, ... 然后回绕到 0, 2, 4, ...
        auto mapIdx = [n](int i) -> int {
            return (1 + 2 * i) % (n | 1);
        };

        // Step 3: 三路划分 (Dutch National Flag) 在虚拟索引空间
        int lo = 0, hi = n - 1, cur = 0;
        while (cur <= hi) {
            if (nums[mapIdx(cur)] > mid) {
                // 大于中位数 -> 放到虚拟前部 (映射到奇数位)
                swap(nums[mapIdx(cur)], nums[mapIdx(lo)]);
                lo++;
                cur++;
            } else if (nums[mapIdx(cur)] < mid) {
                // 小于中位数 -> 放到虚拟后部 (映射到偶数位)
                swap(nums[mapIdx(cur)], nums[mapIdx(hi)]);
                hi--;
                // cur 不动，因为交换来的元素还没检查
            } else {
                // 等于中位数 -> 留在中间
                cur++;
            }
        }
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | Solution1 排序+逆序填充 | O(n log n) | O(n) | 直观易理解，面试推荐 |
 * | Solution2 虚拟索引+三路 | O(n) | O(1) | 最优复杂度，Follow-up |
 *
 * 【逆序填充的关键理解】
 * 示例: [1,2,2,2,3,3] 排序后
 * 较小半: [1,2,2], 较大半: [2,3,3]
 * 正序填: nums = [1,2,2,3,2,3] -> nums[4]=2,nums[3]=3 OK, 但 nums[2]=2,nums[1]=2 不满足 >
 * 逆序填: nums = [2,3,2,3,1,3] -> 全部满足严格不等
 *
 * 【虚拟索引映射解析】
 * n=6: n|1=7
 * i: 0  1  2  3  4  5
 * m: 1  3  5  0  2  4
 * 即先填奇数位(1,3,5)再填偶数位(0,2,4)
 * 三路划分使: 大值->奇数位, 小值->偶数位, 中位数->中间
 *
 * 【易错点】
 * 1. 正序填充会导致中位数相邻，必须逆序
 * 2. 较小半个数 = (n+1)/2（奇数长度时偶数位多一个）
 * 3. 虚拟索引公式中 n|1 确保模数为奇数（关键！）
 * 4. 三路划分中 cur < mid 时 cur 不递增（新换来的没检查过）
 *
 * 【面试追问】
 * Q1: Wiggle Sort I vs II 的区别？
 *     I: 非严格 <=/>= 可用O(n)贪心; II: 严格 </>  必须分组处理
 * Q2: 为什么 n|1 很关键？
 *     保证映射覆盖所有索引。若 n 为偶数，(1+2i)%n 只覆盖奇数索引
 * Q3: nth_element 最坏 O(n^2) 如何应对？
 *     C++ STL 的 Introselect 保证 O(n)；或手写中位数的中位数算法
 */
