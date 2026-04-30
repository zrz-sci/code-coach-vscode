/*
 * LeetCode 80: Remove Duplicates from Sorted Array II (删除有序数组中的重复项 II)
 *
 * 【题目本质】
 * 有序数组原地去重，使每个元素最多出现 2 次。返回新长度 k。
 * 核心技巧：双指针覆盖写 + 通用模板 (比较 nums[slow-k])。
 *
 * 【解法总览】
 * 解法1: 通用双指针模板(k=2) — O(n)/O(1) — 面试首选，6行代码
 * 解法2: 显式计数法          — O(n)/O(1) — 逻辑更直观
 * 解法3: 传统快慢指针        — O(n)/O(1) — 解法1的展开形式
 */

// ============================================================
// 解法1: 通用双指针模板 (k=2) — 面试首选 ⭐
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 维护写指针 slow，表示"下一个要写入的位置"。
// 对于每个元素 num:
//   - 如果 slow < 2: 前两个元素无条件写入
//   - 如果 num != nums[slow-2]: 说明 num 在已写入部分出现不到 2 次，可以写入
//   - 如果 num == nums[slow-2]: 说明 nums[slow-2] 和 nums[slow-1] 都等于 num
//     （因为有序，中间不会有其他值），已经 2 个了，跳过
//
// 为什么比较 nums[slow-2] 而不是 nums[slow-1]?
//   允许 2 次重复。如果 num == nums[slow-1] 但 != nums[slow-2]，
//   说明 num 才出现了 1 次，还能放 1 个。
//   如果 num == nums[slow-2]，那么 [slow-2, slow-1] 都是 num，满了。
//
// 示例: nums = [1,1,1,2,2,3]
//   slow=0: num=1, slow<2         → write! nums=[1,_,_,_,_,_] slow=1
//   slow=1: num=1, slow<2         → write! nums=[1,1,_,_,_,_] slow=2
//   slow=2: num=1, nums[0]=1, ==  → skip!  (已经有两个1了)
//   slow=2: num=2, nums[0]=1, !=  → write! nums=[1,1,2,_,_,_] slow=3
//   slow=3: num=2, nums[1]=1, !=  → write! nums=[1,1,2,2,_,_] slow=4
//   slow=4: num=3, nums[2]=2, !=  → write! nums=[1,1,2,2,3,_] slow=5
//   return 5 ✓
//
// 通用化: 把 2 改成 k 就能处理 "每个元素最多 k 次" 的问题
// ============================================================
class Solution1 {
public:
    int removeDuplicates(vector<int>& nums) {
        int slow = 0;
        for (int num : nums) {
            // 前 k 个直接放入；之后只有不等于 nums[slow-k] 才放入
            if (slow < 2 || num != nums[slow - 2]) {
                nums[slow++] = num;
            }
        }
        return slow;
    }
};

// 通用化版本 (任意 k)
class Solution1_General {
public:
    int removeDuplicates(vector<int>& nums, int k = 2) {
        int slow = 0;
        for (int num : nums) {
            if (slow < k || num != nums[slow - k]) {
                nums[slow++] = num;
            }
        }
        return slow;
    }
};

// ============================================================
// 解法2: 显式计数法
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 维护一个 count 变量，追踪当前值连续出现的次数。
// 每当和前一个相同 → count++；不同 → count = 1
// 只有 count <= 2 时才写入。
//
// 为什么这样是正确的？
//   因为数组有序，相同值必然连续。count 追踪的就是当前值已经保留了几个。
//   count > 2 时跳过，保证每个值最多保留 2 个。
//
// 示例: nums = [0,0,1,1,1,1,2,3,3]
//   i=0: (初始化) slow=0
//   i=1: nums[1]=0 == nums[0]=0 → count=2, <=2 → write nums[1]=0, slow=1
//   i=2: nums[2]=1 != nums[1]=0 → count=1       → write nums[2]=1, slow=2
//   i=3: nums[3]=1 == nums[2]=1 → count=2, <=2 → write nums[3]=1, slow=3
//   i=4: nums[4]=1 == nums[3]=1 → count=3, >2  → skip!
//   i=5: nums[5]=1 == nums[4]=1 → count=4, >2  → skip!
//   i=6: nums[6]=2 != nums[5]=1 → count=1       → write nums[4]=2, slow=4
//   i=7: nums[7]=3 != nums[6]=2 → count=1       → write nums[5]=3, slow=5
//   i=8: nums[8]=3 == nums[7]=3 → count=2, <=2 → write nums[6]=3, slow=6
//   return slow+1 = 7 ✓
//
// 注意: 这里 slow 指向 "最后写入的位置", 返回 slow+1
//       和解法1的 slow ("下一个写入位置" = 长度) 不同!
// ============================================================
class Solution2 {
public:
    int removeDuplicates(vector<int>& nums) {
        if (nums.empty()) return 0;

        int slow = 0;   // 最后一个写入的位置
        int count = 1;   // 当前值的连续出现次数

        for (int i = 1; i < (int)nums.size(); i++) {
            if (nums[i] == nums[i - 1]) {
                count++;
            } else {
                count = 1; // 新值，重置计数
            }

            if (count <= 2) {
                nums[++slow] = nums[i]; // 写入并前进
            }
        }

        return slow + 1; // slow 是索引, 长度 = 索引 + 1
    }
};

// ============================================================
// 解法3: 传统快慢指针 — 解法1的展开形式
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 显式使用 fast 和 slow 两个指针:
// - slow: 下一个要写入的位置 (前 2 个位置已经被预留)
// - fast: 当前正在检查的位置
//
// 前 2 个元素无条件保留 (无论是否相同), 所以都从 2 开始。
// fast 从 2 扫到 n-1, 每次判断 nums[fast] 和 nums[slow-2]:
//   不同 → 写入并 slow++
//   相同 → 跳过
// ============================================================
class Solution3 {
public:
    int removeDuplicates(vector<int>& nums) {
        int n = nums.size();
        if (n <= 2) return n; // 长度 <= 2 直接返回

        int slow = 2; // 前两个元素无条件保留
        for (int fast = 2; fast < n; fast++) {
            // 比较的是已写入的有效部分 nums[slow-2]
            // 不是原始数组的 nums[fast-2]!
            if (nums[fast] != nums[slow - 2]) {
                nums[slow] = nums[fast];
                slow++;
            }
        }
        return slow;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间  | 空间  | 代码行数 | 优势                          |
// |---------------|-------|-------|---------|-------------------------------|
// | 通用模板(k=2) | O(n)  | O(1)  | 6行     | 最短，通用性强，无需特殊边界    |
// | 显式计数法    | O(n)  | O(1)  | 12行    | 逻辑最直观，面试好解释          |
// | 传统快慢指针  | O(n)  | O(1)  | 8行     | 解法1的展开形式，指针角色清晰   |
//
// 面试选择: 先写通用模板 (6行搞定)。
// 如果面试官想看更详细的逻辑，切换到计数法解释。
//
// ============================================================
// 【易错点】
//
// 1. 比较对象搞错:
//    ✗ num != nums[i - 2]  → i-2 是原始数组位置，可能已被覆盖
//    ✓ num != nums[slow - 2] → slow-2 是已写入的有效部分
//
// 2. slow 含义不一致:
//    解法1: slow = "下一个写入位置" = "有效长度", return slow
//    解法2: slow = "最后写入的索引", return slow + 1
//    混淆会导致返回值差 1
//
// 3. slow < 2 保护遗漏:
//    ✗ if (num != nums[slow - 2])  → slow=0 或 1 时越界!
//    ✓ if (slow < 2 || num != nums[slow - 2])  → 短路求值保护
//
// 4. 空数组处理:
//    解法1: 自动处理 (不进入循环, return 0)
//    解法2: 需要 if (nums.empty()) return 0
//    解法3: 需要 if (n <= 2) return n
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 最多 k 个重复？
//     通用模板: if (slow < k || num != nums[slow - k])
//     k=1 → LeetCode 26, k=2 → 本题, k=3 → 每个元素最多3次
//
// Q2: 无序数组怎么去重保留最多 k 个？
//     1) 排序 O(nlogn) + 本题方法
//     2) unordered_map<int,int> 计数, O(n) 时间 O(n) 空间
//
// Q3: 这个模板的正确性怎么证明？
//     归纳法: nums[0..slow-1] 满足不变量 (每个值 <= k 次)。
//     新元素 num == nums[slow-k] → 从 slow-k 到 slow-1 都是 num (有序), 已 k 个
//     新元素 num != nums[slow-k] → 安全写入, 不变量保持
//
// Q4: 和 STL 的 unique/remove 有什么关系？
//     std::unique 就是 k=1 的版本。我们的模板是它的泛化。
//
// ============================================================
// 【相关题型】
//
// - 26. 删除有序数组中的重复项:
//   本题的 k=1 版本。通用模板 slow < 1 || num != nums[slow-1]
//
// - 27. 移除元素:
//   双指针覆盖写, if (num != val) nums[slow++] = num
//   是本模板的简化版 (没有 "往回比较" 的逻辑)
//
// - 283. 移动零:
//   把非零写到前面 + 剩余填零。复用双指针覆盖写思路。
//
// - 1089. 复写零:
//   有序数组原地操作, 但方向是 "扩展" 而非 "收缩"。
//   需要从后往前处理, 与本题的从前往后互补。
// ============================================================
