/*
 * 【题目本质】
 * 设计数据结构支持动态 add 和 find(两数之和)，核心在于根据
 * add/find 调用频率的比例选择"懒计算"还是"预计算"策略。
 *
 * 【解法总览】
 * 解法1: 哈希表计数 — add O(1), find O(n)，适合 add 多的场景
 * 解法2: 预计算所有和 — add O(n), find O(1)，适合 find 多的场景
 */

// ===================== 解法1: 哈希表计数 =====================
// 思路：懒计算，add 只记录出现次数，find 时再遍历查找 complement
// 关键：用 map 而非 set，因为需要处理 value = 2*num 的重复元素情况
class Solution1 {
    // 记录每个数字出现的次数，而非仅存在性
    unordered_map<int, int> cnt;
public:
    Solution1() {}

    void add(int number) {
        cnt[number]++;
    }

    bool find(int value) {
        for (auto& [num, freq] : cnt) {
            int comp = value - num;
            if (comp == num) {
                // 自身配对：需要同一个数出现 >= 2 次
                if (freq >= 2) return true;
            } else {
                // 不同数字配对：只需要 complement 存在即可
                if (cnt.count(comp)) return true;
            }
        }
        return false;
    }
};

// ===================== 解法2: 预计算所有和 =====================
// 思路：add 时与所有已有元素计算和并存入 set，find 时 O(1) 查询
// 适用场景：find 调用远多于 add 时，用空间换 find 的时间
class Solution2 {
    vector<int> nums;
    unordered_set<int> sums;  // 所有可能的两数之和
public:
    Solution2() {}

    void add(int number) {
        // 与已有的每个数字组合，存储所有可能的和
        for (int num : nums) {
            sums.insert(num + number);
        }
        nums.push_back(number);
    }

    bool find(int value) {
        return sums.count(value);
    }
};

/*
 * 【解法对比】
 * | 解法 | add | find | 空间 | 最佳场景 |
 * |------|-----|------|------|----------|
 * | 哈希表计数 | O(1) | O(n) | O(n) | add 多 |
 * | 预计算所有和 | O(n) | O(1) | O(n^2) | find 多 |
 *
 * 【易错点】
 * 1. 解法1中 value = 2*num 时，必须检查 freq >= 2 而非 cnt.count(comp)
 *    例如: add(3), find(6) 应返回 false（只有一个 3）
 * 2. 解法1遍历的是 map 的 key 而非原始数组，避免重复配对
 * 3. 解法2中 add 必须先计算和再 push_back，否则会自己和自己配对
 *
 * 【面试追问】
 * Q1: 两种解法如何选择？ → 看 add 和 find 的调用频率比
 * Q2: 能否做到 add O(1) find O(1)？ → 不能，信息论角度至少一个是 O(n)
 * Q3: 多线程环境下怎么处理？ → find 遍历时需要加读锁，add 需要写锁
 */
