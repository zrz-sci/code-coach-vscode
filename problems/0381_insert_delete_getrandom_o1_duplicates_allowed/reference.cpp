/*
 * 381. Insert Delete GetRandom O(1) - Duplicates allowed
 *
 * 【题目本质】
 * 设计一个支持重复元素的多重集(multiset)，要求 insert/remove/getRandom 均 O(1)。
 * 核心技巧：数组存元素 + 哈希表(值→索引集合) + swap-to-back 删除。
 *
 * 【解法总览】
 * Solution1: Array + HashMap<int, unordered_set<int>>    所有操作 O(1) 均摊
 * Solution2: Array + HashMap<int, vector<int>> + 额外映射  变体写法
 *
 * 【核心数据结构】
 * vector<int> nums:                    存储所有元素（含重复）
 * unordered_map<int, unordered_set<int>> idx:  值 → 该值在 nums 中的所有下标
 *
 * 【关键操作】
 * insert: push_back + 记录索引
 * remove: swap-to-back + pop_back + 更新索引
 * getRandom: rand() % size 随机取下标
 */

// ===================== Solution1: Array + HashMap (推荐) =====================
// 标准做法：unordered_set 存索引，保证删除索引时 O(1)
// 所有操作均摊 O(1)
class Solution1 {
public:
    // 类名应为 RandomizedCollection，这里用 Solution1 展示逻辑
    vector<int> nums;
    unordered_map<int, unordered_set<int>> idx;

    bool insert(int val) {
        bool firstTime = idx[val].empty();
        nums.push_back(val);
        idx[val].insert(nums.size() - 1);
        return firstTime;
    }

    bool remove(int val) {
        if (idx[val].empty()) return false;

        // 1. 取出 val 的任意一个索引
        int i = *idx[val].begin();
        int lastVal = nums.back();
        int lastIdx = nums.size() - 1;

        // 2. 将末尾元素覆盖到位置 i
        nums[i] = lastVal;

        // 3. 更新索引映射（注意顺序！）
        idx[val].erase(i);           // 删掉 val 在 i 的记录
        idx[lastVal].insert(i);      // lastVal 新增位置 i
        idx[lastVal].erase(lastIdx); // lastVal 删掉原末尾位置

        // 4. 弹出末尾
        nums.pop_back();

        return true;
    }

    int getRandom() {
        return nums[rand() % nums.size()];
    }
};

// ===================== Solution2: 精简写法 =====================
// 同样的思路，略微不同的索引更新顺序
// 重点展示 val == lastVal 边界情况的处理
class Solution2 {
public:
    vector<int> nums;
    unordered_map<int, unordered_set<int>> idx;

    bool insert(int val) {
        bool res = !idx.count(val) || idx[val].empty();
        idx[val].insert(nums.size());
        nums.push_back(val);
        return res;
    }

    bool remove(int val) {
        if (!idx.count(val) || idx[val].empty()) return false;

        int i = *idx[val].begin();
        idx[val].erase(idx[val].begin());

        if (i != (int)nums.size() - 1) {
            int last = nums.back();
            nums[i] = last;
            idx[last].erase(nums.size() - 1);
            idx[last].insert(i);
        }
        nums.pop_back();
        return true;
    }

    int getRandom() {
        return nums[rand() % nums.size()];
    }
};

/*
 * 【解法对比】
 * | 解法     | insert | remove | getRandom | 特点                     |
 * |----------|--------|--------|-----------|--------------------------|
 * | Solution1| O(1)*  | O(1)*  | O(1)      | 通用写法，不分支讨论       |
 * | Solution2| O(1)*  | O(1)*  | O(1)      | 显式处理 i==last 边界      |
 * * 均摊 O(1)，因为 unordered_set/map 的哈希操作均摊 O(1)
 *
 * 【易错点】
 * 1. 删除索引更新顺序：必须先 insert(i) 再 erase(lastIdx)
 *    否则当 val == lastVal 且 i == lastIdx 时逻辑出错
 *
 * 2. 用 vector 代替 unordered_set 存索引：
 *    删除特定索引时需要 O(n) 查找，整体退化为 O(n)
 *
 * 3. insert 返回值语义：返回 true 表示 val 之前不在集合中
 *    （第一次出现），false 表示已存在（但仍然插入了）
 *
 * 4. remove 后 nums 必须 pop_back：
 *    只做 swap 不缩小数组会导致 getRandom 概率不正确
 *
 * 5. 空集合检查：idx[val] 为空不等于 idx 中不存在 val 键
 *    要用 idx[val].empty() 判断，不能用 !idx.count(val)
 *
 * 【面试追问】
 * Q1: 与 380 题（不允许重复）的关键区别？
 * A1: 数据结构差异：380 用 map<int,int> 存唯一索引，
 *     381 用 map<int,set<int>> 存索引集合。
 *     insert 语义差异：380 存在时不插入返回 false，
 *     381 始终插入，返回值仅表示是否首次出现。
 *
 * Q2: unordered_set 换成 set (红黑树) 会怎样？
 * A2: 所有操作从 O(1) 退化为 O(log k)，k 是同值元素个数。
 *     unordered_set 是均摊 O(1) 但最坏 O(n)（哈希冲突），
 *     set 是确定性 O(log k)。工程中选择取决于冲突概率。
 *
 * Q3: 如何用 C++ STL 的 rand() 保证均匀分布？
 * A3: rand() % size 对于 RAND_MAX >> size 时近似均匀。
 *     严格均匀分布应使用 <random> 库：
 *     uniform_int_distribution<int>(0, size-1)(gen)。
 *     面试中 rand() % size 通常被接受。
 */
