/*
 * LeetCode 346: 数据流中的移动平均值 (Moving Average from Data Stream)
 *
 * 【题目本质】
 * 维护一个固定大小的滑动窗口，每次新元素进来时高效计算窗口内平均值。
 * 核心在于：用什么数据结构维护窗口 + 如何避免每次都重新遍历求和。
 *
 * 【解法总览】
 * 解法1: 暴力数组       — O(size) / O(n)    — 最直觉，每次重新求和
 * 解法2: 队列 + 累计和  — O(1) / O(size)    — 面试首选，增量更新
 * 解法3: 循环数组       — O(1) / O(size)    — 底层优化，无动态内存
 */

// ============================================================
// 解法1: 暴力数组 — 存所有值，每次取最后 size 个求和
// 时间: O(size) per next    空间: O(n) — n是总调用次数
//
// 【思路】
// 最朴素的想法：把所有历史数据存入数组，每次 next 时
// 从数组末尾往前取 min(count, size) 个元素求和。
// 简单但有两个缺点：
//   1. 每次 next 都要遍历 size 个元素
//   2. 空间随调用次数无限增长，但其实只需要最近 size 个
// ============================================================
class MovingAverage1 {
public:
    int windowSize;
    vector<int> data;

    MovingAverage1(int size) {
        windowSize = size;
    }

    double next(int val) {
        data.push_back(val);
        // 取最后 windowSize 个元素（不足则取全部）
        int count = min((int)data.size(), windowSize);
        double sum = 0;
        for (int i = (int)data.size() - count; i < (int)data.size(); i++) {
            sum += data[i];
        }
        return sum / count;
    }
};

// ============================================================
// 解法2: 队列 + 累计和 — 增量更新，O(1) per next  ⭐ 面试首选
// 时间: O(1) per next    空间: O(size)
//
// 【思路】
// 解法1的瓶颈：每次遍历窗口求和。但相邻两次 next 之间，
// 窗口只变化了一个元素（加入新的，可能淘汰最老的）。
// 
// 关键优化：维护一个 running sum，每次只做：
//   - 窗口满了？sum -= 被淘汰的（队首）
//   - sum += 新元素
//   - return sum / 当前窗口大小
//
// 为什么用队列？因为要淘汰"最早进入"的元素 = FIFO。
//
// 窗口变化过程 (size=3):
//   next(1):  queue=[1]       sum=1   → 1/1 = 1.0
//   next(10): queue=[1,10]    sum=11  → 11/2 = 5.5
//   next(3):  queue=[1,10,3]  sum=14  → 14/3 = 4.667
//   next(5):  淘汰1, queue=[10,3,5] sum=18 → 18/3 = 6.0
// ============================================================
class MovingAverage2 {
public:
    queue<int> window;
    int windowSize;
    double sum;

    MovingAverage2(int size) {
        windowSize = size;
        sum = 0;
    }

    double next(int val) {
        // 窗口已满 → 淘汰最老的元素（队首）
        if ((int)window.size() == windowSize) {
            sum -= window.front();
            window.pop();
        }
        // 新元素入窗口
        window.push(val);
        sum += val;
        // 除数是实际元素个数，不是 windowSize（窗口可能还没填满）
        return sum / window.size();
    }
};

// ============================================================
// 解法3: 循环数组 + 累计和 — 固定内存，无动态分配
// 时间: O(1) per next    空间: O(size)
//
// 【思路】
// 用固定大小数组代替队列。通过 count % size 计算写入位置，
// 新元素直接覆盖最老元素的位置（像一个环）。
//
// 好处：无动态内存分配（queue 的 push/pop 可能有），
// 在嵌入式或性能敏感场景更优。
//
// 循环数组示意 (size=3):
//
//   count=0: idx=0  [0,0,0] → 写入1  → [1,0,0]  sum=1
//   count=1: idx=1  [1,0,0] → 写入10 → [1,10,0] sum=11
//   count=2: idx=2  [1,10,0]→ 写入3  → [1,10,3] sum=14
//   count=3: idx=0  [1,10,3]→ 减旧1,写5 → [5,10,3] sum=18
//             ↑ 回到位置0，覆盖最老的1
//   count=4: idx=1  [5,10,3]→ 减旧10,写2→ [5,2,3] sum=10
//                ↑ 覆盖位置1的10
// ============================================================
class MovingAverage3 {
public:
    vector<int> window;
    int windowSize;
    int count;
    double sum;

    MovingAverage3(int size) {
        windowSize = size;
        window.resize(size, 0);  // 预分配并初始化为0
        count = 0;
        sum = 0;
    }

    double next(int val) {
        int idx = count % windowSize;
        // 减去即将被覆盖位置的旧值
        // （窗口未满时 window[idx]=0，减0不影响正确性）
        sum -= window[idx];
        // 新值写入该位置
        window[idx] = val;
        sum += val;
        count++;
        // 实际窗口内元素个数 = min(count, windowSize)
        return sum / min(count, windowSize);
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | next 时间 | 空间    | 特点                        |
// |------|-----------|---------|----------------------------|
// | 暴力 | O(size)   | O(n)    | 最直觉，但重复计算          |
// | 队列 | O(1)      | O(size) | 面试首选，代码清晰          |
// | 循环 | O(1)      | O(size) | 无动态内存，底层场景更优    |
//
// 解法2 vs 解法3: 时间空间复杂度相同。
// - 队列方案代码更直观（push/pop 语义清晰）
// - 循环数组方案避免了队列内部的动态内存操作，
//   适合嵌入式或对内存分配敏感的场景
//
// ============================================================
// 【易错点】
//
// 1. 除数用 windowSize 而不是实际元素个数：
//    ✗ return sum / windowSize;  // 前几次调用窗口没满，除数太大
//    ✓ return sum / window.size();（队列）
//    ✓ return sum / min(count, windowSize);（循环数组）
//
// 2. 循环数组中忘记减去旧值：
//    ✗ window[idx] = val; sum += val;  // 旧值没从 sum 中去掉
//    ✓ sum -= window[idx]; window[idx] = val; sum += val;
//
// 3. sum 声明为 int 导致整数除法：
//    ✗ int sum; ... return sum / count;  // 整数除法丢失小数
//    ✓ double sum; ... return sum / count;
//
// 4. 循环数组 count 溢出（极端情况）：
//    count 是 int，最多 10^4 次调用不会溢出。
//    如果调用次数极大（>2*10^9），count 应改为 long long，
//    或者改用 count 只在 [0, size) 范围内循环。
//
// ============================================================
// 【面试追问】
//
// Q1（基础理解）: 为什么选队列不选栈？
//    → 需要淘汰"最老的"元素 = FIFO，栈是 LIFO 只能淘汰最新的。
//
// Q2（优化）: 如果 size 非常大（10^9）但调用次数很少（10^4），
//    循环数组方案有什么问题？
//    → 循环数组预分配 size 大小空间会浪费。此时用队列方案，
//      队列最多存 min(调用次数, size) 个元素，按需分配。
//
// Q3（变体）: 如果还要支持"移动中位数"怎么办？
//    → 中位数无法用 sum 增量更新，需要有序结构。
//      经典方案：双堆（大顶堆 + 小顶堆）+ 延迟删除。
//      参考 LeetCode 480: Sliding Window Median。
//
// ============================================================
