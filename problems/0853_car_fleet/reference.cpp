/*
 * LeetCode 853: Car Fleet (车队)
 * 
 * 【题目本质】
 * 按位置从靠近终点到远离终点的顺序排列所有车，
 * 如果后方的车独立到达终点的时间 ≤ 前方的车，它会被前车阻挡合并成一个车队。
 * 问最终有多少个独立车队。
 *
 * 【解法总览】
 * 解法1: 排序 + 线性扫描     — O(n log n) / O(n) — 面试首选 ⭐
 * 解法2: 排序 + 显式单调栈   — O(n log n) / O(n) — 帮助理解栈思想
 */

// ============================================================
// 解法1: 排序 + 线性扫描 — 面试首选 ⭐
// 时间: O(n log n)  空间: O(n)
// 
// 【思路】
// 核心洞察：一辆车能否独立到达终点，取决于前方有没有更慢的车挡它。
// 
// 按位置从大到小排序后，从最靠近终点的车开始扫描：
// - 计算每辆车独立到达终点的时间 time = (target - pos) / speed
// - 如果 time > 当前最大时间(maxTime)，说明追不上前方车队，自成新车队
// - 如果 time ≤ maxTime，说明会被前方车队吞并
// 
// 为什么只需要比较 maxTime？
// 因为 maxTime 代表当前"最近的一个车队"到达终点的时间。
// 如果后方车的时间 ≤ maxTime，它一定会被这个车队挡住。
// maxTime 只增不减，保证了所有已合并的车队不会被"穿透"。
//
// 示例1 的扫描过程:
// target = 12
// 排序后:  位置[10, 8, 5, 3, 0], 速度[2, 4, 1, 3, 1]
//
//   0     3     5     8    10    12(target)
//   |-----|-----|-----|-----|-----|
//   car5  car4  car3  car2 car1  →
//   s=1   s=3   s=1   s=4  s=2
//   t=12  t=3   t=7   t=1  t=1
//
// 扫描(从右到左，即从位置大到小):
//   car1@10: time=1.0 > 0(maxTime) → 新车队! fleets=1, maxTime=1.0
//   car2@8:  time=1.0 ≤ 1.0       → 合并     fleets=1, maxTime=1.0
//   car3@5:  time=7.0 > 1.0       → 新车队! fleets=2, maxTime=7.0
//   car4@3:  time=3.0 ≤ 7.0       → 合并     fleets=2, maxTime=7.0
//   car5@0:  time=12.0 > 7.0      → 新车队! fleets=3, maxTime=12.0
//   结果: 3
// ============================================================
class Solution1 {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {
        int n = position.size();
        
        // 配对 (position, speed)，按位置降序排序
        vector<pair<int,int>> cars(n);
        for (int i = 0; i < n; i++) {
            cars[i] = {position[i], speed[i]};
        }
        sort(cars.begin(), cars.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
            return a.first > b.first; // 位置大的在前（最靠近终点的先处理）
        });
        
        int fleets = 0;
        double maxTime = 0.0; // 当前车队领头车的到达时间
        
        for (auto& [pos, spd] : cars) {
            // 这辆车如果不被阻挡，独立到达终点的时间
            double time = (double)(target - pos) / spd;
            
            if (time > maxTime) {
                // 到达时间更长 → 追不上前方车队 → 自成新车队
                fleets++;
                maxTime = time;
            }
            // time ≤ maxTime: 会在到达终点前（或恰好在终点）追上前方车队
            // → 被吞并，不计新车队
        }
        
        return fleets;
    }
};

// ============================================================
// 解法2: 排序 + 显式单调栈
// 时间: O(n log n)  空间: O(n)
// 
// 【思路】
// 和解法1 完全相同的逻辑，但用显式栈存储每个车队的到达时间。
// 
// 栈的含义：栈中每个元素代表一个独立车队的到达时间。
// - 栈从底到顶是单调递增的（后面的车队到达时间更长）
// - 新车如果 time ≤ 栈顶 → 被栈顶车队吞并，不入栈
// - 新车如果 time > 栈顶 → 自成新车队，入栈
// 
// 注意：这个栈没有弹出操作（因为前方车队不会被后方车队改变）。
// 但理解这个栈的结构对 Car Fleet II (1776) 很有帮助，
// 1776 中前方车队可能被更前方的车队吞并，需要弹栈。
//
// 栈的变化过程(示例1):
//   处理 car@10: stack = [1.0]           ← 第1个车队
//   处理 car@8:  1.0 ≤ 1.0, skip       stack = [1.0]
//   处理 car@5:  7.0 > 1.0, push       stack = [1.0, 7.0]    ← 第2个车队
//   处理 car@3:  3.0 ≤ 7.0, skip       stack = [1.0, 7.0]
//   处理 car@0:  12.0 > 7.0, push      stack = [1.0, 7.0, 12.0] ← 第3个车队
//   答案 = stack.size() = 3
// ============================================================
class Solution2 {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {
        int n = position.size();
        
        vector<pair<int,int>> cars(n);
        for (int i = 0; i < n; i++) {
            cars[i] = {position[i], speed[i]};
        }
        sort(cars.begin(), cars.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
            return a.first > b.first;
        });
        
        // 显式栈：存储每个独立车队的到达时间
        stack<double> st;
        
        for (auto& [pos, spd] : cars) {
            double time = (double)(target - pos) / spd;
            
            if (st.empty() || time > st.top()) {
                // 追不上前方车队（栈顶），自成新车队
                st.push(time);
            }
            // else: time ≤ 栈顶，被前方车队吞并，不入栈
        }
        
        return st.size();
    }
};

// ============================================================
// 【解法对比】
// 
// 解法1 vs 解法2:
// - 本质完全相同。解法1 的 maxTime 等价于解法2 的栈顶。
// - 解法1 更简洁（不需要栈，一个变量即可），面试首选。
// - 解法2 的价值：让"栈"显式化，帮助理解 Car Fleet II 的单调栈弹出逻辑。
// - 时间复杂度都是 O(n log n)，瓶颈在排序。
//
// 【易错点】
// 
// 1. 排序方向错误：
//    ✗ sort by position ascending → 从最远的车开始扫描，逻辑全反
//    ✓ sort by position descending → 从最靠近终点的车开始，前车阻挡后车
//
// 2. 整数除法导致精度丢失：
//    ✗ int time = (target - pos) / spd;  // (12-10)/2=1, (12-8)/4=1 都是1，但一个该合并一个不该
//    ✓ double time = (double)(target - pos) / spd;  // 1.0 和 1.0，正确比较
//
// 3. 合并条件用 >= 而不是 >：
//    ✗ if (time >= maxTime) fleets++;  // time==maxTime 时也加了新车队
//    ✓ if (time > maxTime) fleets++;   // time==maxTime 意味着恰好追上，应合并
//    题目明确："在 target 才赶上也算同一个车队"
//
// 4. 忘记配对排序：
//    ✗ sort(position.begin(), position.end()); // speed 没跟着动，对应关系全乱
//    ✓ 把 (position[i], speed[i]) 打包成 pair 后一起排序
//
// 【面试追问】
// 
// Q1: "为什么按位置排序而不是按到达时间排序？"
//   → 因为"不能超车"是空间上的约束：位置靠前的车物理上挡住后面的车。
//     必须知道谁在前谁在后，才能判断"谁挡谁"。按时间排序丢失了空间关系。
//
// Q2: "能否 O(n) 解决？"
//   → 如果 position 值域有限（0 < position < target ≤ 10^6），
//     可以用桶排序代替比较排序，达到 O(n + target)。
//     但一般面试中 O(n log n) 足够。
//
// Q3: "如果要输出每个车队包含哪些车？"
//   → 扫描时维护一个临时数组 currentFleet。当 time > maxTime 时，
//     把 currentFleet 存入结果，清空开始新一组。最后别忘了存最后一组。
//
// Q4: "Car Fleet II (1776) 和本题的区别？"
//   → 1776 问每辆车被追上的具体时间。关键区别：前方的车也可能被更前方的
//     车追上并消失。所以需要真正的单调栈 + 弹栈：当栈顶车会在被当前车
//     追上之前就被更前方的车吞并时，需要弹出栈顶重新计算。
// ============================================================
