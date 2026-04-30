/*
 * LeetCode 295: 数据流的中位数 (Find Median from Data Stream)
 *
 * 【题目本质】
 * 设计一个数据结构，支持动态添加数字，并能随时返回所有已添加数字的中位数。
 * 核心挑战：不需要全局有序，只需要快速获取"中间位置"的值。
 *
 * 【解法总览】
 * 解法1: 有序数组(插入排序) — addNum O(n), findMedian O(1) — 暴力直觉
 * 解法2: 双堆(大顶堆+小顶堆) — addNum O(log n), findMedian O(1) — 面试首选 ⭐
 * 解法3: 计数排序 (Follow-up) — addNum O(1), findMedian O(range) — Follow-up 最优
 */

// ============================================================
// 解法1: 有序数组（插入排序）
// addNum: O(n)   findMedian: O(1)   空间: O(n)
//
// 【思路】
// 最直觉的做法：维护一个有序数组，每次用二分找到插入位置。
// 中位数直接取中间元素。
// 
// 瓶颈：vector::insert 在中间位置需要移动 O(n) 个元素。
// 5*10^4 次调用 → 最坏 O(n^2) ≈ 2.5*10^9，会超时。
// 这个解法的价值在于展示"为什么需要更好的数据结构"。
// ============================================================
class Solution1 {
public:
    class MedianFinder {
        vector<int> sorted_arr;
    public:
        MedianFinder() {}
        
        void addNum(int num) {
            // 二分找插入位置 — O(log n)
            auto pos = lower_bound(sorted_arr.begin(), sorted_arr.end(), num);
            // 插入 — O(n)，这是瓶颈所在
            sorted_arr.insert(pos, num);
        }
        
        double findMedian() {
            int n = sorted_arr.size();
            if (n % 2 == 1) return sorted_arr[n / 2];
            // 注意用 2.0 而不是 2，否则整数除法丢失小数
            return (sorted_arr[n / 2 - 1] + sorted_arr[n / 2]) / 2.0;
        }
    };
};

// ============================================================
// 解法2: 双堆（大顶堆 + 小顶堆） ⭐ 面试首选
// addNum: O(log n)   findMedian: O(1)   空间: O(n)
//
// 【思路】
// 解法1的瓶颈是"维护全局有序的插入代价太高"。
// 但我们只需要中间值，不需要全局有序！
//
// 核心想法：把数据从中间切开：
//   - 较小的一半用大顶堆 maxHeap（堆顶 = 较小一半的最大值）
//   - 较大的一半用小顶堆 minHeap（堆顶 = 较大一半的最小值）
//
// 中位数就是两个堆顶的平均（偶数个）或大顶堆顶（奇数个）。
//
// 插入策略（三步走，简洁且正确）：
//   1. num 先进 maxHeap
//   2. maxHeap 堆顶弹出给 minHeap → 保证左半 ≤ 右半
//   3. 如果 minHeap 多了，搬一个回 maxHeap → 保证 size 平衡
//
// 示意图:
//
//  数据: [1, 2, 3, 4, 5]
//
//  maxHeap (较小一半)        minHeap (较大一半)
//      ┌───┐                    ┌───┐
//      │ 3 │ ← top             │ 4 │ ← top
//      ├───┤                    ├───┤
//      │ 2 │                    │ 5 │
//      ├───┤                    └───┘
//      │ 1 │
//      └───┘
//   size = 3                  size = 2
//
//   奇数个 → 中位数 = maxHeap.top() = 3
//
//  加入6后:
//  maxHeap: [1,2,3]  minHeap: [4,5,6]  各3个
//  偶数个 → 中位数 = (3 + 4) / 2.0 = 3.5
//
// 模拟完整插入过程:
//
//  addNum(1):
//    Step1: maxH=[1]      minH=[]
//    Step2: maxH=[]       minH=[1]
//    Step3: minH.size > maxH.size → 搬回 → maxH=[1] minH=[]
//    → 奇数，中位数 = 1
//
//  addNum(2):
//    Step1: maxH=[2,1]    minH=[]
//    Step2: maxH=[1]      minH=[2]
//    Step3: size 相等，不搬
//    → 偶数，中位数 = (1+2)/2 = 1.5
//
//  addNum(3):
//    Step1: maxH=[3,1]    minH=[2]    (3 加入，堆调整后3在顶)
//    Step2: maxH=[1]      minH=[2,3]
//    Step3: minH.size > maxH.size → 搬回 → maxH=[2,1] minH=[3]
//    → 奇数，中位数 = 2
// ============================================================
class Solution2 {
public:
    class MedianFinder {
        // maxHeap: 存较小的一半，堆顶是这一半中的最大值
        priority_queue<int> maxHeap;
        // minHeap: 存较大的一半，堆顶是这一半中的最小值
        priority_queue<int, vector<int>, greater<int>> minHeap;
        
    public:
        MedianFinder() {}
        
        void addNum(int num) {
            // 三步走策略：保证正确性 + size 平衡
            
            // Step1: 先放入大顶堆
            maxHeap.push(num);
            
            // Step2: 大顶堆的最大值挤到小顶堆
            // 为什么？num 可能比小顶堆里某些元素大，
            // 让它和 maxHeap 原有元素竞争，最大的那个去 minHeap
            minHeap.push(maxHeap.top());
            maxHeap.pop();
            
            // Step3: 保证 maxHeap.size() >= minHeap.size()
            // 这样奇数个元素时 maxHeap 多一个
            if (minHeap.size() > maxHeap.size()) {
                maxHeap.push(minHeap.top());
                minHeap.pop();
            }
        }
        
        double findMedian() {
            if (maxHeap.size() > minHeap.size()) {
                // 奇数个元素：maxHeap 多一个，堆顶就是中位数
                return maxHeap.top();
            }
            // 偶数个元素：两堆顶的平均
            // 注意 2.0 保证浮点除法
            return (maxHeap.top() + minHeap.top()) / 2.0;
        }
    };
};

// ============================================================
// 解法3: 计数排序 (Follow-up: 数据范围 [0, 100])
// addNum: O(1)   findMedian: O(100)=O(1)   空间: O(101)
//
// 【思路】
// Follow-up: "如果所有整数都在 [0, 100] 范围内，怎么优化？"
//
// 数据范围极小 → 用频次数组！
// count[i] = 值 i 出现的次数
// 找中位数 = 线性扫描频次数组，累加到中间位置
//
// 比双堆更优的地方：
//   - addNum: O(1) vs O(log n)
//   - 空间: O(101) 固定 vs O(n) 动态增长
//
// 进一步 Follow-up: 如果 99% 数据在 [0,100]，少数超范围？
// → 频次数组处理 [0,100]，超范围的用两个额外容器存储
// ============================================================
class Solution3 {
public:
    class MedianFinder {
        int count[101] = {};   // count[i] = 值 i 出现的次数
        int total = 0;         // 当前总元素数
        
    public:
        MedianFinder() {}
        
        void addNum(int num) {
            count[num]++;
            total++;
        }
        
        double findMedian() {
            // 统一处理奇偶：
            // 奇数个 → 找第 (total+1)/2 个，mid1 == mid2
            // 偶数个 → 找第 total/2 和第 total/2+1 个
            int mid1 = (total + 1) / 2;
            int mid2 = (total + 2) / 2;
            
            int val1 = -1, val2 = -1;
            int cumSum = 0;
            
            for (int i = 0; i <= 100; i++) {
                cumSum += count[i];
                // 累加到 >= mid1 时，val1 就是第 mid1 个数
                if (val1 == -1 && cumSum >= mid1) val1 = i;
                // 累加到 >= mid2 时，val2 就是第 mid2 个数
                if (val2 == -1 && cumSum >= mid2) val2 = i;
                if (val1 != -1 && val2 != -1) break;
            }
            
            return (val1 + val2) / 2.0;
        }
    };
};

// ============================================================
// 【解法对比】
//
// | 解法          | addNum  | findMedian | 空间   | 适用场景       |
// |---------------|---------|------------|--------|----------------|
// | 有序数组      | O(n)    | O(1)       | O(n)   | n 很小         |
// | 双堆 ⭐       | O(logn) | O(1)       | O(n)   | 通用，面试首选 |
// | 计数排序      | O(1)    | O(range)   | O(101) | 数据范围小     |
//
// 双堆是面试标准答案：
//   - 思路清晰：较小一半大顶堆 + 较大一半小顶堆
//   - 实现简洁：三步走策略
//   - 复杂度最优（通用场景下）
//
// 【易错点】
// 1. 整数除法：return (a + b) / 2; → 丢失小数，应该用 2.0
// 2. priority_queue 方向搞反：
//    - priority_queue<int> 是大顶堆（默认 less）
//    - priority_queue<int, vector<int>, greater<int>> 是小顶堆
//    搞反后 maxHeap 顶部不是最大值，整个逻辑全错
// 3. 三步走策略中忘记 Step3 的调整，导致 minHeap 可能比 maxHeap
//    多 2 个元素，findMedian 取到的不是中位数
// 4. Follow-up 计数排序：(total+1)/2 和 (total+2)/2 是 1-indexed
//    的第几个数，不是数组下标，累加 count 时要和它比较
//
// 【面试追问】
// Q1: 暴力解能过吗？
// → 有序数组插入 O(n)，n=5*10^4 次调用最坏 O(n^2)≈2.5*10^9，不能过。
//
// Q2: 三步走策略为什么比"直接判断放哪个堆"更好？
// → 直接判断需要处理空堆、相等、边界等多种 case。
//   三步走利用堆的自动排序，先让 num 和 maxHeap 竞争，
//   赢家（较大的）去 minHeap，天然保证左半≤右半。
//
// Q3: 数据范围 [0,100] 怎么优化？
// → 频次数组 count[101]，addNum O(1)，findMedian 扫描 101 格 = O(1)。
//
// Q4: 99% 在 [0,100]，少数超范围？
// → 频次数组 + 两个额外容器（存 <0 和 >100 的），
//   找中位数时综合三部分总数定位。
// ============================================================
