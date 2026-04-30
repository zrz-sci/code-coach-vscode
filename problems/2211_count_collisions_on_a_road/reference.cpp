/*
 * LeetCode 2211: Count Collisions on a Road (统计道路上的碰撞次数)
 *
 * n 辆车在无限长公路上, 每辆车 L(左)/R(右)/S(静止), 速度相同。
 * R碰L → +2, 运动碰静止 → +1, 碰后车辆变静止。求总碰撞次数。
 *
 * 核心洞察:
 *   1. 最左端连续 L 向左逃逸, 最右端连续 R 向右逃逸
 *   2. 中间的每个 R 和 L 最终都会碰撞, 各贡献 1 次碰撞
 *   3. 答案 = 中间区间内非 'S' 字符的数量
 *
 * 面试追问:
 *   Q1: 为什么不模拟? → 连锁碰撞 O(n^2), 数学方法 O(n)
 *   Q2: 为什么中间 R/L 一定碰撞? → 中间最左非L, 最右非R, 两端有阻挡
 *   Q3: 跟 LC735 行星碰撞区别? → 735 需要知道剩余行星(栈), 本题只需总次数(数学)
 *
 * 易错点:
 *   ✗ 试图逐步模拟碰撞 → 连锁反应难处理, 时间 O(n^2)
 *   ✗ 忘记 S 本身不贡献碰撞 → S 只是被撞目标, 计数时不算
 *   ✗ 边界: 全 L 或全 R → left > right, 循环不执行, 答案为 0
 *
 * 碰撞过程演示 ("RLRSLL"):
 *
 *   初始: R  L  R  S  L  L
 *         →  ←  →  .  ←  ←
 *   t=1:  车0(R)碰车1(L) → +2, 变 S S
 *   t=2:  车2(R)碰车3(S) → +1, 车2变S
 *   t=3:  车4(L)碰车3(S) → +1, 车4变S
 *   t=4:  车5(L)碰车4(S) → +1, 车5变S
 *   总计: 2+1+1+1 = 5
 *
 * 三种解法:
 *   解法1: 双指针计数 (推荐, O(n)/O(1))
 *   解法2: 公式法 (最简洁, O(n)/O(1))
 *   解法3: 栈模拟 (直观但代码长, O(n)/O(n))
 */



// ===================== 解法1: 双指针计数 (推荐) =====================
//
// 1. left 从左端跳过连续 'L' (这些车向左逃逸, 永不碰撞)
// 2. right 从右端跳过连续 'R' (这些车向右逃逸, 永不碰撞)
// 3. 中间 [left, right] 的每个 'R' 和 'L' 都一定碰撞
//    (因为中间不可能有"通道"让它们逃走)
//    每个贡献 1 次碰撞
//
// 时间: O(n)   空间: O(1)

class Solution {
public:
    int countCollisions(string directions) {
        int n = directions.size();
        int left = 0, right = n - 1;

        // 跳过左端连续的 'L' (向左逃逸)
        while (left < n && directions[left] == 'L') {
            left++;
        }
        // 跳过右端连续的 'R' (向右逃逸)
        while (right >= 0 && directions[right] == 'R') {
            right--;
        }

        // 统计中间区间内非 'S' 字符数量
        // 每个 'R' 或 'L' 最终停下, 恰好贡献 1 次碰撞
        int collisions = 0;
        for (int i = left; i <= right; i++) {
            if (directions[i] != 'S') {
                collisions++;
            }
        }
        return collisions;
    }
};


// ===================== 解法2: 公式法 (最简洁) =====================
//
// 碰撞次数 = n - (左端连续L数) - (右端连续R数) - (总S数)
//
// 推导:
//   总车数 n = 左端L + 右端R + 中间R + 中间L + 总S
//   碰撞次数 = 中间R + 中间L = n - 左端L - 右端R - 总S
//
// 时间: O(n)   空间: O(1)

class Solution2 {
public:
    int countCollisions(string directions) {
        int n = directions.size();

        // 左端连续 L 的数量
        int leftL = 0;
        while (leftL < n && directions[leftL] == 'L') leftL++;

        // 右端连续 R 的数量
        int rightR = 0;
        while (rightR < n && directions[n - 1 - rightR] == 'R') rightR++;

        // 总 S 的数量
        int totalS = (int)count(directions.begin(), directions.end(), 'S');

        // 碰撞次数 = 总数 - 逃逸 - 静止
        return max(0, n - leftL - rightR - totalS);
    }
};


// ===================== 解法3: 栈模拟 =====================
//
// 从左到右遍历, 用栈维护"当前活跃的车"
// 栈中只会有 'R' 和 'S' (L 要么逃逸要么立即碰撞)
//
// 处理每辆车:
//   'R': 入栈 (暂不碰撞)
//   'S': 弹出所有栈中 R (每个+1碰撞), 入栈 S
//   'L': 若栈顶R → +2, 弹R, 继续弹剩余R(每个+1), 入栈S
//        若栈顶S → +1
//        若空/栈顶L → 逃逸
//
// 时间: O(n)   空间: O(n)

class Solution3 {
public:
    int countCollisions(string directions) {
        int n = directions.size();
        int collisions = 0;
        vector<char> stk;

        for (int i = 0; i < n; i++) {
            char d = directions[i];

            if (d == 'R') {
                // 向右的车暂时入栈
                stk.push_back('R');
            }
            else if (d == 'S') {
                // 静止车: 栈中所有 R 撞上来
                while (!stk.empty() && stk.back() == 'R') {
                    collisions++;   // 每辆 R 撞 S, +1
                    stk.pop_back();
                }
                stk.push_back('S');
            }
            else {
                // d == 'L'
                if (!stk.empty() && stk.back() == 'R') {
                    // R 和 L 相向碰撞: +2
                    collisions += 2;
                    stk.pop_back();
                    // 碰撞产生的 S 阻挡了栈中更多 R
                    while (!stk.empty() && stk.back() == 'R') {
                        collisions++;
                        stk.pop_back();
                    }
                    // 碰撞后变为静止
                    stk.push_back('S');
                }
                else if (!stk.empty() && stk.back() == 'S') {
                    // L 撞上静止车: +1
                    collisions++;
                }
                // else: 栈空或栈顶是L → 这个 L 向左逃逸
            }
        }
        return collisions;
    }
};


/*
 * ==================== 手动演算 ====================
 *
 * === 双指针法 ===
 *
 * 示例1: "RLRSLL"
 *   left: directions[0]='R' (不是L) → left=0
 *   right: directions[5]='L' (不是R) → right=5
 *   中间 [0,5] = "RLRSLL"
 *   非S: R(0), L(1), R(2), L(4), L(5) = 5
 *   答案: 5 ✓
 *
 * 示例2: "LLRR"
 *   left: L,L → left=2
 *   right: R,R → right=1
 *   left(2) > right(1) → 循环不执行
 *   答案: 0 ✓
 *
 * === 公式法 ===
 *
 * 示例1: "RLRSLL"
 *   leftL=0, rightR=0, totalS=1
 *   碰撞 = 6 - 0 - 0 - 1 = 5 ✓
 *
 * 示例2: "LLRR"
 *   leftL=2, rightR=2, totalS=0
 *   碰撞 = 4 - 2 - 2 - 0 = 0 ✓
 *
 * === 栈模拟法 ===
 *
 * 示例1: "RLRSLL"
 *   i=0, R: stk=[R]                       碰撞=0
 *   i=1, L: 栈顶R → +2, pop, push S      碰撞=2, stk=[S]
 *   i=2, R: stk=[S,R]                     碰撞=2
 *   i=3, S: pop R(+1), push S             碰撞=3, stk=[S,S]
 *   i=4, L: 栈顶S → +1                   碰撞=4
 *   i=5, L: 栈顶S → +1                   碰撞=5
 *   答案: 5 ✓
 *
 * 附加测试: "SSRSSRLLRSRLSSRL"
 *   leftL: 0 (首字符S)
 *   rightR: 0 (末字符L)
 *   totalS: 7 (位置0,1,3,4,10,11,14 ... 需要数)
 *   实际数S: S,S,_,S,S,_,_,_,_,S,_,_,S,S,_,_ = 7个S
 *   碰撞 = 16 - 0 - 0 - 7 = 9
 *
 *
 * ==================== 关键理解 ====================
 *
 * 为什么 "每个中间的 R 和 L 贡献恰好 1 次碰撞":
 *
 * R碰L: 两车各贡献1次, 总共+2 = 一个R(1) + 一个L(1)  ✓
 * R碰S: R贡献1次, 总共+1 = 一个R(1)                   ✓
 * L碰S: L贡献1次, 总共+1 = 一个L(1)                   ✓
 *
 * S 本身不产生碰撞 (它只是被撞的目标)
 * 所以碰撞总数 = 中间 R 的个数 + 中间 L 的个数
 */
