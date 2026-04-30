/*
 * LeetCode 15: 三数之和 (3Sum)
 *
 * 【题目本质】
 * 在数组中找所有不重复的三元组使其和为 0。
 * 核心：三数之和 = 固定一个数 + Two Sum。难点在去重。
 *
 * 【解法总览】
 * 解法1: 暴力三重循环 + set去重 — O(n³) / O(n) — 最直觉，用于理解题意
 * 解法2: 排序 + 双指针       — O(n²) / O(1) — 面试标准答案
 * 解法3: 哈希表做Two Sum      — O(n²) / O(n) — 另一种思路，但去重较复杂
 */

// ============================================================
// 解法1: 暴力三重循环 + set去重 — O(n³) / O(n)
//
// 【思路】
// 最直觉的做法：枚举所有 (i, j, k) 三元组，检查和是否为 0。
// 用 set<vector<int>> 自动去重（前提是先排序，让三元组有唯一表示）。
// 
// 为什么会超时？n=3000 → n³ = 2.7×10¹⁰，远超 10⁸ 的安全线。
// 瓶颈：内两层循环本质是"找两个数和为 target"，可以优化。
// ============================================================
class Solution1 {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        int n = nums.size();
        set<vector<int>> resultSet;
        sort(nums.begin(), nums.end()); // 排序使三元组有唯一表示
        
        for (int i = 0; i < n - 2; i++) {
            for (int j = i + 1; j < n - 1; j++) {
                for (int k = j + 1; k < n; k++) {
                    if (nums[i] + nums[j] + nums[k] == 0) {
                        resultSet.insert({nums[i], nums[j], nums[k]});
                    }
                }
            }
        }
        
        return vector<vector<int>>(resultSet.begin(), resultSet.end());
    }
};

// ============================================================
// 解法2: 排序 + 双指针 — O(n²) / O(1)  ⭐ 面试首选
//
// 【思路】
// 解法1的瓶颈在内两层循环——这其实就是 Two Sum 问题。
// 排序后可以用双指针 O(n) 完成 Two Sum，同时去重也变简单。
//
// 整体策略：
// 1. 排序数组
// 2. 外层循环固定 nums[i]，转化为在 [i+1, n-1] 中找两数和 == -nums[i]
// 3. 双指针从两端向中间收缩
// 4. 三处去重：(a) 外层跳重复i (b) 找到答案后左跳重复 (c) 右跳重复
//
// 【双指针为什么正确？】
// 排序后数组单调递增。设 sum = nums[L] + nums[R]：
//   - sum < target: L是当前最小可选值，R怎么减都更小 → 只能 L++
//   - sum > target: R是当前最大可选值，L怎么加都更大 → 只能 R--
//   - 这保证了不会漏掉任何合法配对
//
// 【执行过程示意】
// 输入: [-1, 0, 1, 2, -1, -4]
// 排序: [-4, -1, -1, 0, 1, 2]
//         i=0          i=1       i=2(跳过)  i=3
//
// i=1, nums[i]=-1, target=1:
//   [-4, -1, -1, 0, 1, 2]
//              i   L     R   sum=-1+2=1 ✓ → [-1,-1,2]
//              i      L  R   sum=0+1=1  ✓ → [-1,0,1]
//              i      L=R    结束
// ============================================================
class Solution2 {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;
        int n = nums.size();
        sort(nums.begin(), nums.end());
        
        for (int i = 0; i < n - 2; i++) {
            // 剪枝：排序后最小值 > 0，后面全正，不可能凑出 0
            // 注意是 > 0 不是 >= 0，[0,0,0] 是合法答案
            if (nums[i] > 0) break;
            
            // 去重(a)：跳过重复的 nums[i]
            // 为什么向前看(i-1)而不是向后看(i+1)？
            // 向后看会跳过 [-1,-1,2] 这种 i 和 j 用同一个值的情况
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            
            int left = i + 1, right = n - 1;
            int target = -nums[i];
            
            while (left < right) {
                int sum = nums[left] + nums[right];
                
                if (sum < target) {
                    left++;   // 太小了，需要更大的左值
                } else if (sum > target) {
                    right--;  // 太大了，需要更小的右值
                } else {
                    result.push_back({nums[i], nums[left], nums[right]});
                    
                    // 去重(b)(c)：跳过 left/right 的重复值
                    // 注意 while 里必须加 left < right 防止越界
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    
                    // 去重 while 只是跳到"最后一个重复值"，还需各移一步到新值
                    left++;
                    right--;
                }
            }
        }
        
        return result;
    }
};

// ============================================================
// 解法3: 排序 + 哈希表做Two Sum — O(n²) / O(n)
//
// 【思路】
// 另一种思路：固定 nums[i] 后，用 unordered_set 在一次遍历中
// 完成 Two Sum（边遍历边查找 target - nums[j] 是否已存在）。
//
// 优点：不需要理解双指针的正确性证明。
// 缺点：去重逻辑比双指针方案复杂，空间 O(n)。
//       面试中不如解法2简洁，但可以作为"另一种方式"来展示思维广度。
// ============================================================
class Solution3 {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;
        int n = nums.size();
        sort(nums.begin(), nums.end()); // 仍然需要排序来方便去重
        
        for (int i = 0; i < n - 2; i++) {
            if (nums[i] > 0) break;
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            
            unordered_set<int> seen; // 记录当前 i 轮中已经遍历过的 nums[j]
            int target = -nums[i];
            
            for (int j = i + 1; j < n; j++) {
                int complement = target - nums[j];
                
                if (seen.count(complement)) {
                    result.push_back({nums[i], complement, nums[j]});
                    // 去重：跳过后续相同的 nums[j]
                    while (j + 1 < n && nums[j] == nums[j + 1]) j++;
                }
                
                seen.insert(nums[j]);
            }
        }
        
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间    | 空间  | 去重复杂度 | 推荐场景       |
// |------|---------|-------|-----------|---------------|
// | 暴力 | O(n³)   | O(n)  | set自动   | 理解题意       |
// | 双指针| O(n²)  | O(1)  | 简单跳过   | ⭐面试首选     |
// | 哈希表| O(n²)  | O(n)  | 中等      | 展示多种思路    |
//
// 解法2 是面试标准答案：时间最优，空间 O(1)（不算排序的 O(log n) 栈空间），
// 去重逻辑最清晰。
//
// 【易错点】
//
// 1. 第一层去重方向：
//    ✗ if (nums[i] == nums[i+1]) continue;  → [-1,-1,2] 被跳过
//    ✓ if (i > 0 && nums[i] == nums[i-1]) continue;
//    原因：向后看会导致 nums[i] 和 nums[i+1] 作为同一组中的两个元素时被误跳
//
// 2. 剪枝条件：
//    ✗ if (nums[i] >= 0) break;  → [0,0,0] 被漏掉
//    ✓ if (nums[i] > 0) break;
//    原因：nums[i]=0 时还可能后面有两个 0
//
// 3. 找到答案后的指针移动：
//    ✗ 只写了去重 while，忘了 left++; right--;  → 死循环
//    ✓ 去重 while 跳到最后一个重复值后，还需各移一步
//
// 4. 去重 while 缺少越界保护：
//    ✗ while (nums[left] == nums[left+1]) left++;  → 可能 left+1 越界
//    ✓ while (left < right && nums[left] == nums[left+1]) left++;
//
// 5. 忘记排序：
//    双指针的正确性依赖有序性，去重的正确性也依赖相同值相邻
//
// 【面试追问】
//
// Q1: 暴力 O(n³) 能过吗？瓶颈在哪？
//     → 不能，n=3000 时 ~10¹⁰ 次。瓶颈在内两层循环是 Two Sum，
//       排序后双指针可以 O(n) 完成。
//
// Q2: 为什么排序不影响正确性？如果题目要返回索引呢？
//     → 本题返回值不返回索引，排序不影响。如果要索引（如 Two Sum），
//       不能排序，需要哈希表+索引映射。
//
// Q3: 推广到 4Sum / kSum 怎么做？
//     → 4Sum：外面再套一层，O(n³)。kSum：递归降维，每层固定一个数，
//       k=2 时用双指针。时间 O(n^(k-1))。
//
// Q4: 如果数组极大（10⁶ 级别），还能 O(n²) 吗？
//     → O(n²) 在 n=10⁶ 时约 10¹² 次操作，太慢。需要考虑：
//       (a) 值域小的话用计数数组，枚举值而非索引
//       (b) 排序后利用剪枝大量跳过
//       但一般面试不会给这么大的 n。
//
// 【相关题型】
//
// - 1. Two Sum — 本题的子问题，区别：找索引用哈希表，找值用双指针
// - 16. 3Sum Closest — 复用同样的排序+双指针框架，
//       改动：sum==0 的判断 → 维护 abs(sum-target) 最小值
// - 18. 4Sum — 在 3Sum 外面多套一层循环，多一层去重
// - 167. Two Sum II — 有序数组双指针，就是本题的内层操作
// ============================================================
