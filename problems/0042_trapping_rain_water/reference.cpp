/*
 * LeetCode 42: 接雨水 (Trapping Rain Water)
 *
 * 【题目本质】
 * 每个位置能接的水 = min(左边最高, 右边最高) - 自身高度。
 * 核心问题是如何高效求出每个位置的左右最大值。
 *
 * 【解法总览】
 * 解法1: 暴力法         — O(n²) / O(1)   — 最直觉，每个位置左右扫描
 * 解法2: 预处理数组(DP) — O(n) / O(n)    — 预计算leftMax[]和rightMax[]
 * 解法3: 双指针         — O(n) / O(1)    — 短板原理，两端向中间逼近 ⭐面试首选
 * 解法4: 单调递减栈     — O(n) / O(n)    — 按层横向填水
 */

// ============================================================
// 解法1: 暴力法 — 对每个位置左右扫描找最大值
// 时间: O(n²)  空间: O(1)
//
// 【思路】
// 最直觉的想法：对于每个位置 i，向左扫描找最高柱子 leftMax，
// 向右扫描找最高柱子 rightMax，当前位置积水 = min(leftMax, rightMax) - height[i]。
//
// 为什么每个位置包含自身来算 max？
// 因为柱子本身也算"墙"，这保证 min(leftMax,rightMax) >= height[i]，差值非负。
//
// 瓶颈：每个位置都重新扫描 O(n)，总共 O(n²)，存在大量重复计算。
// ============================================================
class Solution1 {
public:
    int trap(vector<int>& height) {
        int n = height.size(), ans = 0;
        for (int i = 0; i < n; i++) {
            int leftMax = 0, rightMax = 0;
            for (int j = 0; j <= i; j++)
                leftMax = max(leftMax, height[j]);
            for (int j = i; j < n; j++)
                rightMax = max(rightMax, height[j]);
            // 因为 leftMax >= height[i] 且 rightMax >= height[i]，差值一定非负
            ans += min(leftMax, rightMax) - height[i];
        }
        return ans;
    }
};

// ============================================================
// 解法2: 预处理数组 (DP) — 预计算每个位置的左右最大值
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 解法1的瓶颈：每个位置重复扫描找左右最大值。
// 优化：用两次 O(n) 遍历预计算 leftMax[] 和 rightMax[]。
//
// leftMax[i] = max(height[0], height[1], ..., height[i])
//            = max(leftMax[i-1], height[i])
//
// rightMax[i] = max(height[i], height[i+1], ..., height[n-1])
//             = max(rightMax[i+1], height[i])
//
// 填表方向和依赖关系：
//   leftMax:  → → → → →  （从左到右，每个依赖左邻）
//   rightMax: ← ← ← ← ←  （从右到左，每个依赖右邻）
//
// 示例 height = [0,1,0,2,1,0,1,3,2,1,2,1]:
//   leftMax  = [0,1,1,2,2,2,2,3,3,3,3,3]
//   rightMax = [3,3,3,3,3,3,3,3,2,2,2,1]
//   water[i] = min(L,R) - h = [0,0,1,0,1,2,1,0,0,1,0,0] → 总和=6
// ============================================================
class Solution2 {
public:
    int trap(vector<int>& height) {
        int n = height.size(), ans = 0;
        vector<int> leftMax(n), rightMax(n);

        // 从左到右预计算
        leftMax[0] = height[0];
        for (int i = 1; i < n; i++)
            leftMax[i] = max(leftMax[i - 1], height[i]);

        // 从右到左预计算
        rightMax[n - 1] = height[n - 1];
        for (int i = n - 2; i >= 0; i--)
            rightMax[i] = max(rightMax[i + 1], height[i]);

        // 逐列求和
        for (int i = 0; i < n; i++)
            ans += min(leftMax[i], rightMax[i]) - height[i];

        return ans;
    }
};

// ============================================================
// 解法3: 双指针 — 短板原理，O(1)空间最优解 ⭐面试首选
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法2的空间瓶颈：两个 O(n) 数组。能否不存数组？
//
// 关键洞察（短板原理）：
// - 维护 left、right 两个指针从两端向中间走
// - 维护 leftMax（左指针走过的最大值）和 rightMax（右指针走过的最大值）
// - 如果 height[left] <= height[right]：
//     右边至少有 height[right] 这么高的柱子，所以 rightMax >= height[right] >= height[left]
//     因此 min(leftMax, rightMax) = leftMax（leftMax 一定是短板）
//     left 位置的水量 = leftMax - height[left]，然后 left++
// - 对称地，height[left] > height[right] 时处理 right
//
// 为什么正确？
// 不变量：处理 left 时，rightMax >= height[right] >= height[left]，
//        所以真正的全局 rightMax 只会更大，不影响 left 的水位计算。
//
// 双指针过程演示 (height = [0,1,0,2,1,0,1,3,2,1,2,1]):
//
// left=0  right=11  lMax=0  rMax=0
//   h[0]=0 <= h[11]=1 → lMax=0, water+=0-0=0, left=1
// left=1  right=11  lMax=0  rMax=0
//   h[1]=1 <= h[11]=1 → lMax=1, water+=1-1=0, left=2
// left=2  right=11  lMax=1  rMax=0
//   h[2]=0 <= h[11]=1 → lMax=1, water+=1-0=1, left=3
// left=3  right=11  lMax=1  rMax=0
//   h[3]=2 > h[11]=1  → rMax=1, water+=1-1=0, right=10
// left=3  right=10  lMax=1  rMax=1
//   h[3]=2 > h[10]=2 不成立(<=) → 但实际 2<=2 → lMax=2, water+=2-2=0, left=4
//   ... 依此类推，最终 total = 6
// ============================================================
class Solution3 {
public:
    int trap(vector<int>& height) {
        int left = 0, right = (int)height.size() - 1;
        int leftMax = 0, rightMax = 0;
        int ans = 0;

        while (left <= right) {
            if (height[left] <= height[right]) {
                // 右边有"兜底"，左边是短板
                leftMax = max(leftMax, height[left]); // 先更新，保证差值非负
                ans += leftMax - height[left];
                left++;
            } else {
                // 左边有"兜底"，右边是短板
                rightMax = max(rightMax, height[right]);
                ans += rightMax - height[right];
                right--;
            }
        }
        return ans;
    }
};

// ============================================================
// 解法4: 单调递减栈 — 按层横向填水
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 前三种解法都是"按列"计算水量。单调栈是"按层"计算。
//
// 维护一个单调递减栈（从栈底到栈顶，高度递减）。
// 遍历柱子时：
// - 当前柱子 <= 栈顶 → 直接入栈（维持递减）
// - 当前柱子 > 栈顶  → 栈顶形成凹槽底部，弹出结算
//
// 结算过程：
//   弹出的栈顶 = bottom（凹槽底）
//   新的栈顶 = leftWall（左墙）
//   当前柱子 = rightWall（右墙）
//   宽度 = rightWall索引 - leftWall索引 - 1
//   高度 = min(height[leftWall], height[rightWall]) - height[bottom]
//   面积 = 宽度 × 高度
//
// 单调栈过程演示 (height = [0,1,0,2,1,0,1,3,2,1,2,1]):
//
// i=0: push 0.        栈(底→顶): [0]
// i=1: h[1]=1 > h[0]=0, 弹出0, 栈空无左墙, push 1.  栈: [1]
// i=2: h[2]=0 <= h[1]=1, push 2.  栈: [1,2]
// i=3: h[3]=2 > h[2]=0
//      弹出2(底=0), 左墙=1(h=1), 右墙=3(h=2)
//      宽=3-1-1=1, 高=min(1,2)-0=1, 面积=1. water=1
//      h[3]=2 > h[1]=1
//      弹出1(底=1), 栈空无左墙. push 3.  栈: [3]
// i=4: push 4.  栈: [3,4]
// i=5: push 5.  栈: [3,4,5]
// i=6: h[6]=1 > h[5]=0
//      弹出5(底=0), 左墙=4(h=1), 右墙=6(h=1)
//      宽=6-4-1=1, 高=min(1,1)-0=1, 面积=1. water=2
//      h[6]=1 不> h[4]=1, push 6.  栈: [3,4,6]
// i=7: h[7]=3 > h[6]=1
//      弹出6(底=1), 左墙=4(h=1), 右墙=7(h=3)
//      宽=7-4-1=2, 高=min(1,3)-1=0, 面积=0. water=2
//      h[7]=3 > h[4]=1
//      弹出4(底=1), 左墙=3(h=2), 右墙=7(h=3)
//      宽=7-3-1=3, 高=min(2,3)-1=1, 面积=3. water=5
//      h[7]=3 > h[3]=2
//      弹出3(底=2), 栈空无左墙. push 7.  栈: [7]
// i=8: push 8.  栈: [7,8]
// i=9: push 9.  栈: [7,8,9]
// i=10: h[10]=2 > h[9]=1
//       弹出9(底=1), 左墙=8(h=2), 右墙=10(h=2)
//       宽=10-8-1=1, 高=min(2,2)-1=1, 面积=1. water=6
//       h[10]=2 不> h[8]=2, push 10.  栈: [7,8,10]
// i=11: push 11.  栈: [7,8,10,11]
// 结束. total water = 6 ✓
// ============================================================
class Solution4 {
public:
    int trap(vector<int>& height) {
        stack<int> stk; // 存索引，对应高度从底到顶单调递减
        int ans = 0;

        for (int i = 0; i < (int)height.size(); i++) {
            // 当前柱子比栈顶高 → 栈顶是凹槽底部，可以结算
            while (!stk.empty() && height[i] > height[stk.top()]) {
                int bottom = stk.top();
                stk.pop();
                // 弹出底部后，栈空意味着左边没有墙，无法积水
                if (stk.empty()) break;

                int leftWall = stk.top();
                int width = i - leftWall - 1;
                int h = min(height[leftWall], height[i]) - height[bottom];
                ans += width * h;
            }
            stk.push(i);
        }
        return ans;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间   | 空间  | 思维方式       | 适用场景       |
// |-----------|--------|-------|---------------|---------------|
// | 暴力       | O(n²) | O(1) | 按列，逐个扫描  | 说思路用       |
// | 预处理DP   | O(n)  | O(n) | 按列，预计算    | 容易理解和编码  |
// | 双指针 ⭐  | O(n)  | O(1) | 按列，短板原理  | 面试首选       |
// | 单调栈     | O(n)  | O(n) | 按层，横向填充  | 面试官追问栈时  |
//
// 双指针 vs 预处理DP：
//   双指针本质是"在线"版的预处理——不需要存整个数组，因为短板那侧
//   的 max 足以确定当前位置的水位。
//
// 单调栈 vs 其他：
//   其他解法"按列"纵向切——每列上方多少水。
//   单调栈"按层"横向切——每个凹槽横向面积。两种角度殊途同归。
//
// ============================================================
// 【易错点】
//
// 1. 双指针 while 条件用 < 还是 <=？
//    ✗ while (left < right)  — 漏掉 left==right 那个位置
//    ✓ while (left <= right) — 每个位置都要参与计算
//    虽然 left==right 时水量一定是0（leftMax和rightMax都>=该值），
//    但逻辑上不应跳过。
//
// 2. 双指针中 max 更新的顺序：
//    ✗ ans += leftMax - height[left]; leftMax = max(leftMax, height[left]);
//      → 如果 height[left] > leftMax，差值为负数！
//    ✓ leftMax = max(leftMax, height[left]); ans += leftMax - height[left];
//      → 先更新保证 leftMax >= height[left]，差值非负
//
// 3. 单调栈弹出后忘记检查栈空：
//    ✗ int bottom = stk.top(); stk.pop(); int leftWall = stk.top();
//      → stk.pop() 后栈可能空了，stk.top() 未定义行为
//    ✓ stk.pop(); if (stk.empty()) break; int leftWall = stk.top();
//
// 4. 单调栈方向搞反：
//    ✗ 维护单调递增栈（遇到更小的弹出）→ 找不到凹槽
//    ✓ 维护单调递减栈（遇到更大的弹出）→ 弹出的就是凹槽底
//
// ============================================================
// 【面试追问】
//
// Q1: 暴力 O(n²) 能接受吗？瓶颈在哪？
//     → 对 n=2×10⁴ 勉强，但面试不够。瓶颈是每个位置重复扫描找左右max。
//
// Q2: 预处理用了 O(n) 空间，能优化到 O(1) 吗？
//     → 双指针。关键是"短板决定水位"：处理短板那侧时，另一侧至少有一个
//        不低于短板的柱子兜底，所以只需要维护短板侧的 max 即可。
//
// Q3: 如果变成二维矩阵接雨水（407题），思路怎么变？
//     → 双指针无法直接推广到二维。改用优先队列(最小堆)：
//        从矩阵四周边界入手，每次取最矮的边界格子向内扩展(BFS)，
//        更高的邻居直接入堆，更低的邻居可以积水后再入堆。
//        时间 O(mn·log(mn))，空间 O(mn)。
//
// Q4: 双指针正确性的形式化证明？
//     → 当 height[left] <= height[right] 时，rightMax 在之后只会增大。
//        所以 min(leftMax, rightMax_全局) 中，rightMax_全局 >= rightMax >= height[right] >= height[left]。
//        如果 leftMax <= rightMax_全局，min = leftMax，正确。
//        如果 leftMax > rightMax_全局，不可能，因为 leftMax 是 left 走过的最大值，
//        而我们只在 height[left] <= height[right] 时才移动 left，
//        所以 leftMax <= height[right] <= rightMax_全局。
// ============================================================
