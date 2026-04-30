/*
 * LeetCode 1929: 数组串联 (Concatenation of Array)
 *
 * 题目本质：构造 ans[0..2n-1]，其中 ans[i] = nums[i % n]。
 *          虽然是简单的数组操作题，但面试中可以展示对 STL 容器操作、
 *          内存管理和循环数组思维的理解。
 *
 * 解法总览：
 *   Solution1 - 双写循环         O(n)/O(n)  面试首选，逻辑清晰
 *   Solution2 - STL insert       O(n)/O(n)  一行代码，最简洁
 *   Solution3 - 取模映射         O(n)/O(n)  循环数组通用技巧
 *   Solution4 - STL copy + 预分配 O(n)/O(n) 展示底层操作理解
 */

// ============================================================
// 解法1: 手动双写循环 — O(n)/O(n) ⭐面试首选
// ============================================================
// 思考过程：
//   最直观：创建 2n 数组，一次循环中同时写前半和后半。
//   vector<int> ans(2*n) 预分配空间，避免 push_back 多次扩容。
class Solution1 {
public:
    vector<int> getConcatenation(vector<int>& nums) {
        int n = nums.size();
        vector<int> ans(2 * n);  // 预分配 2n 空间，初始化为 0
        for (int i = 0; i < n; i++) {
            ans[i] = nums[i];        // 前半: ans[0..n-1] = nums[0..n-1]
            ans[i + n] = nums[i];    // 后半: ans[n..2n-1] = nums[0..n-1]
        }
        return ans;
    }
};

// ============================================================
// 解法2: STL insert 一行解 — O(n)/O(n)
// ============================================================
// 思考过程：
//   vector::insert 可以把一个范围追加到末尾。
//   直接把 nums 的 [begin, end) 追加到 nums 自身末尾。
//   注意：虽然源和目标是同一个 vector，标准库实现是安全的
//   （insert 在操作前会处理好扩容和源数据拷贝）。
class Solution2 {
public:
    vector<int> getConcatenation(vector<int>& nums) {
        nums.insert(nums.end(), nums.begin(), nums.end());
        return nums;  // 注意：修改了输入 nums
    }
};

// ============================================================
// 解法3: 取模映射 — O(n)/O(n)
// ============================================================
// 思考过程：
//   ans[i] = nums[i % n]，取模自然实现了"循环重复"。
//   这是处理循环数组的通用技巧（LC 503, LC 918 等）。
class Solution3 {
public:
    vector<int> getConcatenation(vector<int>& nums) {
        int n = nums.size();
        vector<int> ans(2 * n);
        for (int i = 0; i < 2 * n; i++) {
            ans[i] = nums[i % n];  // i%n: 0,1,...,n-1,0,1,...,n-1
        }
        return ans;
    }
};

// ============================================================
// 解法4: STL copy + 预分配 — O(n)/O(n)
// ============================================================
// 思考过程：
//   展示 C++ STL 算法的使用。reserve 预分配避免扩容，
//   copy + back_inserter 追加第二份。
class Solution4 {
public:
    vector<int> getConcatenation(vector<int>& nums) {
        int n = nums.size();
        vector<int> ans;
        ans.reserve(2 * n);  // 预分配，避免多次扩容

        // 第一份拷贝
        ans.assign(nums.begin(), nums.end());

        // 第二份追加
        copy(nums.begin(), nums.end(), back_inserter(ans));

        return ans;
    }
};

/*
 * ============================================================
 * 解法对比
 * ============================================================
 * | 解法      | 时间 | 空间 | 特点                          |
 * |-----------|------|------|-------------------------------|
 * | Solution1 | O(n) | O(n) | 双写循环，最清晰              |
 * | Solution2 | O(n) | O(n) | insert 一行，最简洁           |
 * | Solution3 | O(n) | O(n) | 取模映射，循环数组通用        |
 * | Solution4 | O(n) | O(n) | STL copy，展示底层操作        |
 *
 * ============================================================
 * 易错点
 * ============================================================
 * 1. 数组大小分配错误:
 *    ✗ vector<int> ans(n) + ans[i+n] = ... -> 越界！
 *    ✓ vector<int> ans(2*n) -> 正确分配 2n 空间
 *
 * 2. insert 后迭代器失效:
 *    nums.insert(nums.end(), nums.begin(), nums.end()) 看似危险
 *    （insert 可能触发 realloc 导致 begin/end 失效），
 *    但标准库实现会在操作前保存源数据范围，所以是安全的。
 *    面试时提到这个细节能加分。
 *
 * 3. 修改了输入数组:
 *    Solution2 直接修改 nums，如果题目不允许修改输入需要先拷贝。
 *    面试中应主动问面试官是否可以修改输入。
 *
 * ============================================================
 * 面试追问
 * ============================================================
 * Q1: 不能 O(1) 空间完成?
 *   -> 输出本身是 2n 数组，至少 O(n) 存储。额外空间确实是 O(1)。
 *
 * Q2: vector insert 扩容的时间复杂度?
 *   -> 扩容: 分配新内存 + 移动旧元素 O(n) + 拷贝新元素 O(n) = O(n)
 *   -> reserve(2n) 可避免移动旧元素这步
 *
 * Q3: i%n 有性能开销吗?
 *   -> 取模是除法操作，比加法慢 3-5x。优化:
 *      - n 是 2 的幂时用 i & (n-1)
 *      - 两次独立循环替代取模
 *      - 条件分支: if (idx >= n) idx -= n
 *
 * Q4: 实际应用场景?
 *   -> 循环数组模拟 (LC 503)、字符串旋转判断 (LC 796)、
 *      音频循环缓冲、图形纹理重复
 */
