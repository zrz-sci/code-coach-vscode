/*
 * LeetCode 380: Insert Delete GetRandom O(1)
 *
 * 【题目本质】
 * 设计一个数据结构，使插入、删除、随机获取三个操作都是平均 O(1)。
 * 核心矛盾：哈希表擅长增删但不能随机访问，数组擅长随机访问但不能 O(1) 删除。
 * 解决方案：数组 + 哈希表组合，用"交换到末尾再删除"实现数组 O(1) 删除。
 *
 * 【解法总览】
 * 解法1: 数组 + 哈希表 — O(1) 均摊 / O(n) — 唯一正解
 *   数组存元素支持随机访问，哈希表存 {值→下标} 支持 O(1) 定位
 */

// ============================================================
// 解法1: 数组 + 哈希表
// 时间: O(1) 均摊 (insert / remove / getRandom)
// 空间: O(n) — n 为当前元素个数
//
// 【思路】
// 分析每个操作的需求:
//   - insert: 检查存在性 O(1) → 哈希表; 添加元素 O(1) → 数组尾部 + 哈希表记录下标
//   - remove: 检查存在性 O(1) → 哈希表; 删除元素 O(1) → 与末尾交换 + pop_back
//   - getRandom: 等概率随机 O(1) → 数组随机下标
//
// 【为什么 "与末尾交换" 能 O(1) 删除？】
// 数组中间删除元素要移动后面所有元素 O(n)。
// 但如果我们不在乎顺序，只需要元素连续存储，那么：
//   1. 把待删元素和末尾元素交换
//   2. pop_back 删除末尾
// 交换需要知道待删元素的下标 → 哈希表 O(1) 提供
//
// 【操作过程可视化】
//
// 初始: nums = [3, 7, 1, 9],  map = {3:0, 7:1, 1:2, 9:3}
//
// remove(7):
//   idx = map[7] = 1,  lastVal = nums[3] = 9
//   Step1: nums[1] = 9       → nums = [3, 9, 1, 9]
//   Step2: map[9] = 1        → map = {3:0, 7:1, 1:2, 9:1}
//   Step3: nums.pop_back()   → nums = [3, 9, 1]
//   Step4: map.erase(7)      → map = {3:0, 9:1, 1:2}
//
// insert(5):
//   Step1: nums.push_back(5) → nums = [3, 9, 1, 5]
//   Step2: map[5] = 3        → map = {3:0, 9:1, 1:2, 5:3}
//
// getRandom():
//   rand() % 4 → 随机返回 nums[0..3] 中的一个
//
// 【边界情况: 删除的恰好是末尾元素】
//   nums = [3, 9, 1],  remove(1):
//   idx = 2, lastVal = nums[2] = 1 (和 val 相同!)
//   Step1: nums[2] = 1 (自己赋给自己, 无影响)
//   Step2: map[1] = 2  (赋了相同值, 无影响)
//   Step3: pop_back → nums = [3, 9]
//   Step4: erase(1) → map = {3:0, 9:1}
//   结论: 先交换再 erase 的顺序保证了这个边界是安全的
// ============================================================
class RandomizedSet {
public:
    RandomizedSet() {}
    
    bool insert(int val) {
        // 已存在则不插入
        if (valToIndex.count(val)) return false;
        
        // 放到数组末尾，哈希表记录其下标
        nums.push_back(val);
        valToIndex[val] = nums.size() - 1;
        return true;
    }
    
    bool remove(int val) {
        // 不存在则无法删除
        if (!valToIndex.count(val)) return false;
        
        int idx = valToIndex[val];      // 待删元素在数组中的位置
        int lastVal = nums.back();      // 末尾元素的值
        
        // 核心: 用末尾元素覆盖待删位置
        // 为什么不用 swap? 因为待删位置之后会被 pop 掉，不需要把 val 放到末尾
        nums[idx] = lastVal;
        valToIndex[lastVal] = idx;      // 更新末尾元素在哈希表中的下标
        
        // 删除末尾 + 删除哈希表条目
        // 注意顺序: 必须在 erase(val) 之前更新 lastVal 的映射
        // 否则当 val == lastVal 时会出 bug (先删了又加回来)
        nums.pop_back();
        valToIndex.erase(val);
        
        return true;
    }
    
    int getRandom() {
        // 等概率: 在 [0, size-1] 中随机取一个下标
        return nums[rand() % nums.size()];
    }
    
private:
    vector<int> nums;                    // 连续数组，支持 O(1) 随机访问
    unordered_map<int, int> valToIndex;  // 值 → 下标，支持 O(1) 定位
};

/**
 * Your RandomizedSet object will be instantiated and called as such:
 * RandomizedSet* obj = new RandomizedSet();
 * bool param_1 = obj->insert(val);
 * bool param_2 = obj->remove(val);
 * int param_3 = obj->getRandom();
 */

// ============================================================
// 【解法对比】
//
// | 方案               | insert | remove | getRandom | 可行？ |
// |-------------------|--------|--------|-----------|--------|
// | 只用 unordered_set | O(1)   | O(1)   | O(n) ✗   | 不可行 |
// | 只用 vector        | O(n)*  | O(n)   | O(1)     | 不可行 |
// | vector + hashmap   | O(1)   | O(1)   | O(1)     | ✓ 正解 |
//
// *vector 的 insert 如果需要去重检查则是 O(n) 线性查找
//
// 【易错点】
// 1. 删除时忘记更新被交换元素的哈希表映射:
//    ✗ nums[idx] = lastVal; nums.pop_back(); valToIndex.erase(val);
//      → 漏了 valToIndex[lastVal] = idx; 后续 lastVal 的定位全错
//    ✓ 必须加 valToIndex[lastVal] = idx;
//
// 2. 删除时先 erase(val) 再更新 lastVal 的映射:
//    ✗ valToIndex.erase(val); valToIndex[lastVal] = idx;
//      → 当 val == lastVal 时 (删末尾元素), erase 删了，下一行又加回来
//    ✓ 先更新 valToIndex[lastVal] = idx, 最后 erase(val)
//
// 3. 删除时先 pop_back 再赋值:
//    ✗ int lastVal = nums.back(); nums.pop_back(); nums[idx] = lastVal;
//      → 当 idx == size-1 (删末尾) 时, pop 后 idx 越界
//    ✓ 先完成所有赋值操作, 最后 pop_back
//
// 4. getRandom 中 rand() 的种子问题:
//    在本地测试时如果不 srand(time(0)), rand() 每次返回相同序列
//    LeetCode 判题系统不会卡这个，但面试时可以提一下
//
// 【面试追问】
// Q1: 为什么不能用 unordered_set?
//   → 哈希表迭代器不支持随机访问, advance(it, k) 是 O(k), getRandom 无法 O(1)
//
// Q2: 删除为什么要和末尾交换?
//   → 数组中间删除要移动元素 O(n), 与末尾交换后 pop_back 是 O(1)
//     代价是牺牲了元素顺序, 但本题不要求有序
//
// Q3: 如果允许重复元素怎么办? (LeetCode 381)
//   → 哈希表从 unordered_map<int, int> 改为 unordered_map<int, unordered_set<int>>
//     每个值对应一组下标。删除时从集合中取一个下标做交换, 注意:
//     - 先从 val 的集合中删除 idx
//     - 将 lastVal 的集合中的旧下标移除, 加入新下标 idx
//     - 再 pop_back
//     边界: 当 val == lastVal 时, 操作顺序要特别小心
//
// Q4: rand() % n 是等概率的吗?
//   → 不严格等概率 (模偏差), 可用 C++11 的 uniform_int_distribution 实现严格等概率
//
// 【相关题型】
// - 381. O(1) 时间插入删除获取随机 - 允许重复
//   复用本题框架, 改动: 哈希表 value 从 int 变为 unordered_set<int>
// - 710. 黑名单中的随机数
//   复用 "交换到末尾" 的映射思想, 将黑名单数映射到尾部区域外
// - 146. LRU 缓存
//   同为 "组合数据结构" 设计题, 用 HashMap + 双向链表实现 O(1) 操作
// ============================================================
