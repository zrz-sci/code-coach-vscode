/*
 * LeetCode 347: 前 K 个高频元素 (Top K Frequent Elements)
 * 
 * 【题目本质】
 * 统计频率 + Top K 选择。第一步所有解法都一样（哈希表统计频率），
 * 区别在于第二步如何高效选出频率最大的 K 个元素。
 *
 * 【解法总览】
 * 解法1: 排序法        — O(n log n) / O(n) — 最直觉，不满足 Follow-up
 * 解法2: 小顶堆        — O(n log k) / O(n) — 面试首选 ⭐
 * 解法3: 桶排序        — O(n) / O(n)       — 最优，满足 Follow-up
 * 解法4: 快速选择      — O(n) 期望 / O(n)  — 进阶，类快排思想
 */

// ============================================================
// 解法1: 排序法 — 统计频率后按频率排序取前 K 个
// 时间: O(n log n)  空间: O(n)
// 
// 【思路】
// 最直觉的方法：统计完频率后，把所有 (元素, 频率) 对按频率从大到小排序，
// 取前 K 个即可。
// 
// 瓶颈：排序是 O(n log n)，而我们只需要 Top K，不需要完全排序。
// 不满足 Follow-up 要求的 "优于 O(n log n)"。
// ============================================================
class Solution1 {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        // Step1: 哈希表统计频率 — O(n)
        unordered_map<int, int> freq;
        for (int num : nums) freq[num]++;
        
        // Step2: 转换为 vector 方便排序
        vector<pair<int, int>> freqVec(freq.begin(), freq.end());
        
        // Step3: 按频率降序排序 — O(n log n)，这是瓶颈
        sort(freqVec.begin(), freqVec.end(), 
             [](const pair<int,int>& a, const pair<int,int>& b) {
                 return a.second > b.second;
             });
        
        // Step4: 取前 K 个
        vector<int> result;
        for (int i = 0; i < k; i++) {
            result.push_back(freqVec[i].first);
        }
        return result;
    }
};

// ============================================================
// 解法2: 小顶堆 — 维护大小为 K 的小顶堆 ⭐ 面试首选
// 时间: O(n log k)  空间: O(n)
// 
// 【思路】
// 解法1的瓶颈是对所有 n 个不同元素排序（O(n log n)），但我们只要 Top K。
// 
// 用小顶堆维护"当前频率最大的 K 个元素"：
// - 堆大小 < K：直接入堆
// - 堆大小 == K：新元素频率 > 堆顶（当前第K大）→ 弹出堆顶，新元素入堆
//                新元素频率 <= 堆顶 → 不够资格，跳过
// 
// 堆大小始终为 K，每次操作 O(log k)，遍历 n 个不同元素，总共 O(n log k)。
// 
// 为什么用小顶堆而不是大顶堆？
// 小顶堆堆顶是最小值，充当"门槛"。比门槛大的才能进来替换最小的。
// 大顶堆要放入全部 n 个再弹 K 次，是 O(n log n)，没有优化效果。
//
// 堆的工作过程 (nums=[1,1,1,2,2,3], k=2):
// 频率: {1:3, 2:2, 3:1}
// 
//   处理(1,3): 堆 = [(3,1)]           堆未满，入堆
//   处理(2,2): 堆 = [(2,2),(3,1)]     堆未满，入堆。小顶堆堆顶=频率2
//   处理(3,1): 1 < 堆顶频率2 → 跳过   频率不够大，没资格
// 
//   最终堆: [(2,2),(3,1)] → 结果 [2, 1]
// ============================================================
class Solution2 {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        // Step1: 统计频率
        unordered_map<int, int> freq;
        for (int num : nums) freq[num]++;
        
        // Step2: 小顶堆，pair<频率, 元素> — 频率放 first 让堆按频率排序
        // greater<> 使得堆顶是最小值（小顶堆）
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> minHeap;
        
        for (auto& [num, cnt] : freq) {
            minHeap.push({cnt, num});
            // 堆大小超过 K 就弹出频率最小的，保持堆中是最大的 K 个
            if ((int)minHeap.size() > k) {
                minHeap.pop();
            }
        }
        
        // Step3: 堆中剩下的就是 Top K（顺序任意，题目允许）
        vector<int> result;
        while (!minHeap.empty()) {
            result.push_back(minHeap.top().second);
            minHeap.pop();
        }
        return result;
    }
};

// ============================================================
// 解法3: 桶排序 — 频率做下标，从高到低收集
// 时间: O(n)  空间: O(n)
// 
// 【思路】
// 关键洞察：频率的范围是 [1, n]（最多所有元素都相同，频率=n）。
// 值域有限 → 可以用计数/桶排序，避免比较排序的 O(n log n) 下界。
// 
// bucket[f] = {所有出现 f 次的元素}
// 从高频桶到低频桶遍历，凑够 K 个就停。
//
// 桶排序过程 (nums=[1,1,1,2,2,3], k=2):
//
// 频率: {1:3, 2:2, 3:1}
// 
// bucket[0]: []
// bucket[1]: [3]    ← 出现1次
// bucket[2]: [2]    ← 出现2次
// bucket[3]: [1]    ← 出现3次
// bucket[4]: []
// bucket[5]: []
// bucket[6]: []
// 
// 从高到低: bucket[6]→空, [5]→空, [4]→空
//           bucket[3]→[1] → 收集1个
//           bucket[2]→[2] → 收集2个 == k → 结束！
// 
// 结果: [1, 2]
// ============================================================
class Solution3 {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        // Step1: 统计频率
        unordered_map<int, int> freq;
        for (int num : nums) freq[num]++;
        
        // Step2: 建桶，桶大小为 n+1（频率范围 [0, n]）
        int n = nums.size();
        vector<vector<int>> bucket(n + 1);  // bucket[i] = 出现 i 次的元素列表
        for (auto& [num, cnt] : freq) {
            bucket[cnt].push_back(num);
        }
        
        // Step3: 从高频到低频遍历桶，凑够 K 个
        vector<int> result;
        for (int i = n; i >= 1 && (int)result.size() < k; i--) {
            for (int num : bucket[i]) {
                result.push_back(num);
                if ((int)result.size() == k) break;  // 凑够就停
            }
        }
        return result;
    }
};

// ============================================================
// 解法4: 快速选择 — 类快排 partition 找第 K 大
// 时间: O(n) 期望, O(n²) 最坏  空间: O(n)
// 
// 【思路】
// 我们不需要完全排序，只需要把频率最大的 K 个放到数组前 K 个位置。
// 这就是"选择问题"(Selection Problem)，用 QuickSelect 可以期望 O(n) 解决。
// 
// 过程类似快排：
// 1. 随机选 pivot
// 2. partition：频率 >= pivot频率 的放左边，< 的放右边
// 3. 如果左边刚好 K 个 → 搞定
//    左边 > K 个 → 递归左半
//    左边 < K 个 → 左边全要，递归右半找剩余的
// 
// 随机化 pivot 使得最坏情况概率极低。
// ============================================================
class Solution4 {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> freq;
        for (int num : nums) freq[num]++;
        
        // 收集所有不同元素
        vector<int> unique;
        for (auto& [num, cnt] : freq) unique.push_back(num);
        
        int n = unique.size();
        // 目标：让 unique[0..k-1] 是频率最高的 k 个（不要求内部有序）
        quickSelect(unique, freq, 0, n - 1, k);
        
        return vector<int>(unique.begin(), unique.begin() + k);
    }
    
private:
    void quickSelect(vector<int>& arr, unordered_map<int,int>& freq, 
                     int left, int right, int k) {
        if (left >= right) return;
        
        // 随机选 pivot，避免最坏情况
        int pivotIdx = left + rand() % (right - left + 1);
        int pivotFreq = freq[arr[pivotIdx]];
        swap(arr[pivotIdx], arr[right]); // 把 pivot 放末尾
        
        // partition：频率 >= pivotFreq 的放左边（降序partition）
        int storeIdx = left;
        for (int i = left; i < right; i++) {
            if (freq[arr[i]] >= pivotFreq) {
                swap(arr[i], arr[storeIdx]);
                storeIdx++;
            }
        }
        swap(arr[storeIdx], arr[right]); // pivot 归位
        
        // storeIdx 左边（含自身）有 count 个频率 >= pivotFreq 的元素
        int count = storeIdx - left + 1;
        if (count == k) {
            return; // 前 k 个就是答案
        } else if (count > k) {
            // 前 k 个在左半部分中
            quickSelect(arr, freq, left, storeIdx - 1, k);
        } else {
            // 左边 count 个全要，还需要从右半找 k-count 个
            quickSelect(arr, freq, storeIdx + 1, right, k - count);
        }
    }
};

// ============================================================
// 【解法对比】
// 
//              时间         空间    满足Follow-up   代码难度   面试推荐
// 排序法     O(n log n)    O(n)       ✗           简单       说一下就行
// 小顶堆    O(n log k)    O(n)       ✓           中等       ⭐ 首选
// 桶排序       O(n)        O(n)       ✓           简单       最优
// 快速选择  O(n)期望       O(n)       ✓           较难       了解即可
// 
// 面试策略：
// 1. 先说排序法展示理解 → 2. 写小顶堆 → 3. 被追问O(n)则说桶排序
//
// 【易错点】
// 1. 小顶堆 vs 大顶堆搞反：
//    ✗ 大顶堆放入全部 n 个再弹 K 次 → O(n log n)，白用堆了
//    ✓ 小顶堆维持大小 K，堆顶是"门槛"，只有更大的才能进入
//
// 2. priority_queue 中 pair 顺序写反：
//    ✗ push({num, cnt}) → 默认按 num 排序，频率没参与比较
//    ✓ push({cnt, num}) → 按频率排序，这才是我们要的
//
// 3. 桶数组大小开错：
//    ✗ vector<vector<int>> bucket(n)   → 下标最大 n-1，频率为 n 时越界
//    ✓ vector<vector<int>> bucket(n+1) → 下标 [0, n]，覆盖所有可能频率
//
// 4. 快速选择 partition 方向搞反：
//    ✗ 把频率小的放左边 → 前 K 个是频率最小的
//    ✓ 把频率大的放左边 → 前 K 个是频率最大的
//
// 【面试追问】
// Q1: 暴力排序法瓶颈在哪？
//     → 排序 O(n log n)，但只需要 Top K，对 K+1~n 的排序是浪费。
//
// Q2: 为什么用小顶堆不用大顶堆？
//     → 小顶堆维持大小 K，堆顶是"第K大的门槛"，比较成本 O(log k)。
//       大顶堆要放入全部 n 个再弹 K 次，成本 O(n log n)。
//
// Q3: 能否 O(n)？不能用堆。
//     → 桶排序。频率范围 [1, n] 有限，用频率做桶下标，O(n) 建桶 + O(n) 遍历。
//
// Q4: 如果频率表不变但 k 每次不同，怎么优化多次查询？
//     → 预处理：建好桶 + 从高到低的前缀和。查询时二分找第 K 大频率的桶，O(log n)。
//
// 【相关题】
// 215. 数组中的第K个最大元素 — 复用堆/快速选择框架，区别：直接在值上做 Top K
// 692. 前K个高频单词 — 几乎相同，区别：频率相同时按字典序，需自定义比较器
// 451. 根据字符出现频率排序 — 复用桶排序，区别：k=n 且要重复输出
// 973. 最接近原点的K个点 — 复用堆 Top K，比较对象从频率变为距离
// ============================================================
