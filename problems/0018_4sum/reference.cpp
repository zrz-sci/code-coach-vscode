/*
 * LeetCode 18: 四数之和 (4Sum)
 * 
 * 【题目本质】
 * 在数组中找出所有不重复的四元组，使得四数之和等于 target。
 * 本质是 3Sum 的扩展：外层多套一层循环，内层仍用双指针。
 * 
 * 【系列对比 — 与 3Sum 的代码差异】
 * 3Sum:  sort → for i → 双指针(l, r)          → O(n²)
 * 4Sum:  sort → for i → for j → 双指针(l, r)  → O(n³)
 * 新增: 一层 j 循环 + 对应的去重和剪枝，双指针逻辑不变。
 * 
 * 【解法总览】
 * 解法1: 暴力枚举         — O(n⁴)   / O(n)   — 直觉但超时
 * 解法2: 排序+双指针       — O(n³)   / O(1)   — ⭐面试首选
 * 解法3: 通用kSum递归     — O(n^(k-1)) / O(k) — 加分项/追问
 */

// ============================================================
// 解法1: 暴力枚举 — 四重循环 + set 去重
// 时间: O(n⁴)  空间: O(n) (set 存储去重)
//
// 【思路】
// 最直觉的做法：枚举所有四元组，检查和是否等于 target。
// 用 set 自动去重。排序保证 set 中元素顺序一致。
// 
// 瓶颈：n=200 时 n⁴ ≈ 1.6×10⁹，会 TLE。
// 但这个思路帮我们理解：优化的方向是减少枚举量。
// ============================================================
class Solution1 {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        int n = nums.size();
        sort(nums.begin(), nums.end());
        set<vector<int>> resultSet;
        
        for (int a = 0; a < n; a++)
            for (int b = a + 1; b < n; b++)
                for (int c = b + 1; c < n; c++)
                    for (int d = c + 1; d < n; d++) {
                        // 必须用 long long，四个 10⁹ 相加爆 int
                        long long sum = (long long)nums[a] + nums[b] + nums[c] + nums[d];
                        if (sum == target)
                            resultSet.insert({nums[a], nums[b], nums[c], nums[d]});
                    }
        
        return vector<vector<int>>(resultSet.begin(), resultSet.end());
    }
};

// ============================================================
// 解法2: 排序 + 双指针 — ⭐ 面试首选
// 时间: O(n³)  空间: O(1) (不计输出)
//
// 【思路】
// 解法1的瓶颈：最内两层循环在做"给定前两个数，找剩余两个数使和为remainder"。
// 这就是排序后的 Two Sum！用双指针 O(n) 搞定，于是四重变三重。
//
// 搜索过程示意 (nums = [-2,-1,0,0,1,2], target = 0):
//
// i=0(val=-2), j=1(val=-1): remainder=3
//   left=2, right=5: 0+2=2 < 3  → left++
//   left=3, right=5: 0+2=2 < 3  → left++
//   left=4, right=5: 1+2=3 == 3 → 找到 [-2,-1,1,2] ✓
//
// i=0(val=-2), j=2(val=0): remainder=2
//   left=3, right=5: 0+2=2 == 2 → 找到 [-2,0,0,2] ✓
//
// i=1(val=-1), j=2(val=0): remainder=1
//   left=3, right=5: 0+2=2 > 1  → right--
//   left=3, right=4: 0+1=1 == 1 → 找到 [-1,0,0,1] ✓
//
// 去重逻辑 (三层都要):
//   i层: i>0 且 nums[i]==nums[i-1] 则 skip
//   j层: j>i+1 且 nums[j]==nums[j-1] 则 skip
//   双指针层: 找到解后跳过相邻重复再移动
//
// 剪枝逻辑:
//   最小和剪枝: nums[i]+nums[i+1]+nums[i+2]+nums[i+3] > target → break
//   最大和剪枝: nums[i]+nums[n-3]+nums[n-2]+nums[n-1] < target → continue
//   (j层同理)
// ============================================================
class Solution2 {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        vector<vector<int>> result;
        int n = nums.size();
        if (n < 4) return result;
        
        sort(nums.begin(), nums.end());
        
        for (int i = 0; i < n - 3; i++) {
            // --- 第一层去重 ---
            // 为什么是 i > 0 而不是其他？因为 i 从 0 开始，i==0 时没有前一个可比
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            
            // --- 第一层剪枝 ---
            // 最小和: 从 i 开始连续4个数（它们是当前能组成的最小和）
            if ((long long)nums[i] + nums[i + 1] + nums[i + 2] + nums[i + 3] > target)
                break;  // break! 后面 i 更大，和只会更大
            // 最大和: nums[i] + 最大的3个数
            if ((long long)nums[i] + nums[n - 3] + nums[n - 2] + nums[n - 1] < target)
                continue;  // continue! 当前 i 太小，但下一个 i 可能行
            
            for (int j = i + 1; j < n - 2; j++) {
                // --- 第二层去重 ---
                // 为什么是 j > i+1？j 从 i+1 开始，j==i+1 时不应跳过
                // 否则 [2,2,2,2] target=8 会漏掉 [2,2,2,2]
                if (j > i + 1 && nums[j] == nums[j - 1]) continue;
                
                // --- 第二层剪枝 ---
                if ((long long)nums[i] + nums[j] + nums[j + 1] + nums[j + 2] > target)
                    break;
                if ((long long)nums[i] + nums[j] + nums[n - 2] + nums[n - 1] < target)
                    continue;
                
                // --- 双指针找剩余两个数 ---
                int left = j + 1, right = n - 1;
                while (left < right) {
                    long long sum = (long long)nums[i] + nums[j] + nums[left] + nums[right];
                    
                    if (sum < target) {
                        left++;  // 和太小，左指针右移让和变大
                    } else if (sum > target) {
                        right--;  // 和太大，右指针左移让和变小
                    } else {
                        // 找到一组解
                        result.push_back({nums[i], nums[j], nums[left], nums[right]});
                        
                        // 双指针层去重: 跳过相同元素
                        while (left < right && nums[left] == nums[left + 1]) left++;
                        while (left < right && nums[right] == nums[right - 1]) right--;
                        
                        // 跳过重复元素后，再各移一步到新的值
                        left++;
                        right--;
                    }
                }
            }
        }
        
        return result;
    }
};

// ============================================================
// 解法3: 通用 kSum 递归 — 适用于任意 k
// 时间: O(n^(k-1)), k=4 时 O(n³)  空间: O(k) 递归栈深度
//
// 【思路】
// 面试追问"如果是 5Sum、kSum 怎么办？"
// 
// 核心观察：kSum 问题可以递归分解:
//   kSum(target) = 固定一个数 nums[i] + (k-1)Sum(target - nums[i])
//   base case: 2Sum 用双指针
//
// 递归树 (以 4Sum 为例):
//   4Sum(target)
//     → 固定 nums[i], 递归 3Sum(target - nums[i])
//       → 固定 nums[j], 递归 2Sum(target - nums[i] - nums[j])
//         → 双指针找两个数
//
// 这个框架的优势：改一个参数 k 就能解 5Sum、6Sum...
// ============================================================
class Solution3 {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        return kSum(nums, (long long)target, 0, 4);
    }
    
private:
    vector<vector<int>> kSum(vector<int>& nums, long long target, int start, int k) {
        vector<vector<int>> result;
        int n = nums.size();
        
        // 剩余元素不够组成 k 元组
        if (start + k > n) return result;
        
        // ====== Base case: 2Sum 双指针 ======
        if (k == 2) {
            int left = start, right = n - 1;
            while (left < right) {
                long long sum = (long long)nums[left] + nums[right];
                if (sum < target) {
                    left++;
                } else if (sum > target) {
                    right--;
                } else {
                    result.push_back({nums[left], nums[right]});
                    // 去重
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    left++;
                    right--;
                }
            }
            return result;
        }
        
        // ====== 一般情况: 固定一个数，递归成 (k-1)Sum ======
        for (int i = start; i <= n - k; i++) {
            // 去重: 同一层不重复选相同的值
            if (i > start && nums[i] == nums[i - 1]) continue;
            
            // 剪枝: 最小 k 个数之和 > target → break
            long long minSum = 0;
            for (int m = 0; m < k; m++) minSum += nums[i + m];
            if (minSum > target) break;
            
            // 剪枝: 当前数 + 最大 k-1 个数之和 < target → continue
            long long maxSum = (long long)nums[i];
            for (int m = 1; m < k; m++) maxSum += nums[n - m];
            if (maxSum < target) continue;
            
            // 递归: 固定 nums[i]，求 (k-1)Sum(target - nums[i])
            auto subResult = kSum(nums, target - nums[i], i + 1, k - 1);
            
            // 合并: 把 nums[i] 加到每个子结果前面
            for (auto& sub : subResult) {
                sub.insert(sub.begin(), nums[i]);
                result.push_back(sub);
            }
        }
        
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | 解法1(暴力) | 解法2(双指针) | 解法3(kSum递归) |
// |-----------|------------|-------------|---------------|
// | 时间       | O(n⁴)     | O(n³)       | O(n³) k=4时   |
// | 空间       | O(n) set  | O(1)        | O(k) 递归栈    |
// | 去重方式   | set       | 排序+跳过    | 排序+跳过      |
// | 通用性     | 仅4Sum    | 仅4Sum      | 任意kSum       |
// | 面试推荐   | 说思路    | ⭐首选       | 追问时展示     |
//
// 解法2 是面试标准答案：代码清晰、效率好、不需要额外空间。
// 解法3 在面试官追问 "如果是 kSum 怎么办？" 时展示，证明理解深度。
//
// ============================================================
// 【易错点】
//
// 1. 去重条件的边界：
//    ✗ if (j > 0 && nums[j] == nums[j-1]) continue;
//      → j 从 i+1 开始，j > 0 会在 j==i+1 且值相同时错误跳过
//      → 例如 [2,2,2,2] target=8 会漏掉 [2,2,2,2]
//    ✓ if (j > i + 1 && nums[j] == nums[j-1]) continue;
//      → j 在自己这层第一次选某个值时不跳过
//
// 2. 整数溢出：
//    ✗ int sum = nums[i] + nums[j] + nums[left] + nums[right];
//      → 四个 10⁹ 相加 = 4×10⁹ > INT_MAX (2.1×10⁹)
//    ✓ long long sum = (long long)nums[i] + nums[j] + nums[left] + nums[right];
//      → 第一个加数转 long long 后整个表达式在 long long 下运算
//
// 3. break vs continue 搞混：
//    ✗ 最小和 > target 时用 continue → 失去剪枝效果，后面 i 更大和只会更大
//    ✓ 最小和 > target 时用 break → 提前终止整层循环
//    ✗ 最大和 < target 时用 break → 漏掉后面更大的 i 可能有解
//    ✓ 最大和 < target 时用 continue → 跳过当前，继续尝试更大的 i
//
// 4. 找到解后双指针忘记同时移动：
//    ✗ 只做了去重跳过，但忘记最后的 left++; right--;
//      → 死循环，一直停在同一对解上
//    ✓ 先跳重复，再各移一步
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: "这道题和 3Sum 的关系？代码改了什么？"
//   → 3Sum = 1层循环 + 双指针，4Sum = 2层循环 + 双指针。
//     新增: 一层 for j 循环 + 对应去重(j > i+1) + 对应剪枝。
//     双指针部分代码完全不变。
//
// Q2: "如果变成 kSum 怎么办？"
//   → 递归框架: k>2 时固定一个数递归 (k-1)Sum，k==2 时双指针。
//     时间 O(n^(k-1))。见解法3。
//
// Q3: "为什么用双指针不用哈希表？"
//   → 哈希表也能做（固定两数后 Two Sum 用哈希表），时间同为 O(n³)。
//     但双指针的优势：(1) O(1) 空间 (2) 去重更自然（排序后跳相邻）
//     (3) 代码更清晰，不容易 bug。面试中优先双指针。
//
// Q4: "剪枝不影响正确性吗？什么情况最有效？"
//   → 正确性：排序后数组递增，最小和/最大和的判断是精确的上下界。
//     最有效：数组元素集中在某个范围，而 target 在范围外时，
//     外层循环很快 break/continue，大幅减少内层执行。
//     最无效：所有元素都非常接近 target/4 时，几乎无法剪枝。
//
// 【相关题型】
// [1]   Two Sum — kSum 的 base case，哈希表 O(n)
// [15]  3Sum — 4Sum 的前置题，少一层 for 循环，双指针逻辑完全一样
// [16]  3Sum Closest — 不找精确值，维护 minDiff，双指针框架复用
// [454] 4Sum II — 四个独立数组，用哈希表 O(n²) 做，思路完全不同
// ============================================================
