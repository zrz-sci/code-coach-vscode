/*
 * 面试题 08.06: 汉诺塔问题 (Hanota LCCI)
 * 难度: Easy | 标签: Recursion, Array
 *
 * 题目：将所有盘子从柱A移到柱C，遵守经典汉诺塔规则。
 *
 * =====================================================
 * 解法：递归分治 (经典三步法) — 唯一标准解法
 * =====================================================
 *
 * 核心思想：
 *   move(n, src, buf, dst) = 把 n 个盘从 src 移到 dst，buf 做缓冲
 *   1. move(n-1, src, dst, buf)  —— 把上面 n-1 个移到 buf (dst做缓冲)
 *   2. 把最大盘从 src 移到 dst
 *   3. move(n-1, buf, src, dst)  —— 把 n-1 个从 buf 移到 dst (src做缓冲)
 *
 * 递归树 (n=3, A→C):
 *
 *                        hanota(3, A→C, buf=B)
 *                       /         |         \
 *            hanota(2, A→B)     盘2:A→C     hanota(2, B→C)
 *            /    |    \                     /    |    \
 *     hanota(1,  盘1:   hanota(1,     hanota(1,  盘1:  hanota(1,
 *     A→C)      A→B    C→B)          B→A)      B→C    A→C)
 *      |                 |             |                 |
 *    盘0:A→C           盘0:C→B       盘0:B→A           盘0:A→C
 *
 * 7步移动序列 (2^3 - 1 = 7):
 *   A→C, A→B, C→B, A→C, B→A, B→C, A→C
 *
 * 步骤   操作      A状态       B状态       C状态
 * 初始    —       [2,1,0]     []          []
 *  1    A→C(盘0)  [2,1]       []          [0]
 *  2    A→B(盘1)  [2]         [1]         [0]
 *  3    C→B(盘0)  [2]         [1,0]       []
 *  4    A→C(盘2)  []          [1,0]       [2]         ← 最大盘到位
 *  5    B→A(盘0)  [0]         [1]         [2]
 *  6    B→C(盘1)  [0]         []          [2,1]
 *  7    A→C(盘0)  []          []          [2,1,0]     ← 完成
 *
 * 时间: O(2^n)  —— T(n) = 2*T(n-1)+1 = 2^n - 1 (数学最优, 不可能更快)
 * 空间: O(n)    —— 递归调用栈深度为 n
 */

class Solution {
public:
    void hanota(vector<int>& A, vector<int>& B, vector<int>& C) {
        // 入口：把 A.size() 个盘子从 A 移到 C，B 做缓冲
        move(A.size(), A, B, C);
    }

private:
    /*
     * move: 把 n 个盘子从 from 移到 to，buf 做缓冲
     *
     * 参数角色切换（递归的精妙之处）：
     *   原始调用:   move(n,   from, buf,  to)
     *   第一次递归:  move(n-1, from, to,   buf)    ← to 变成了缓冲!
     *   第三次递归:  move(n-1, buf,  from, to)     ← from 变成了缓冲!
     */
    void move(int n, vector<int>& from, vector<int>& buf, vector<int>& to) {
        // Base case: 没有盘子需要移动
        if (n == 0) return;

        // Step 1: 上面 n-1 个盘从 from → buf（to 充当缓冲）
        move(n - 1, from, to, buf);

        // Step 2: 第 n 个（最大的）盘从 from → to
        // 此时 from 上只剩这一个盘，上面的 n-1 个已在 buf 上
        to.push_back(from.back());  // 先读取值
        from.pop_back();             // 再删除（顺序不能反!）

        // Step 3: n-1 个盘从 buf → to（from 充当缓冲）
        move(n - 1, buf, from, to);
    }
};


/*
 * =====================================================
 * 解法二：迭代法（了解即可，面试中递归解法是标准答案）
 * =====================================================
 *
 * 汉诺塔有一个优美的迭代规律（3 柱情况）：
 *
 * 对于 n 为奇数: 每三步循环 A↔C, A↔B, B↔C
 * 对于 n 为偶数: 每三步循环 A↔B, A↔C, B↔C
 *
 * 其中 "X↔Y" = 在 X 和 Y 之间做一次合法移动（移动较小的盘子）。
 */
class Solution_Iterative {
public:
    void hanota(vector<int>& A, vector<int>& B, vector<int>& C) {
        int n = A.size();
        int total = (1 << n) - 1;  // 2^n - 1 步

        // 如果 n 为偶数，交换 B 和 C 的角色使迭代规律统一
        vector<int>* pegs[3];
        if (n % 2 == 0) {
            pegs[0] = &A; pegs[1] = &C; pegs[2] = &B;
        } else {
            pegs[0] = &A; pegs[1] = &B; pegs[2] = &C;
        }

        for (int step = 1; step <= total; step++) {
            int from_idx, to_idx;
            if (step % 3 == 1)      { from_idx = 0; to_idx = 2; }
            else if (step % 3 == 2) { from_idx = 0; to_idx = 1; }
            else                    { from_idx = 1; to_idx = 2; }

            moveBetween(*pegs[from_idx], *pegs[to_idx]);
        }
    }

private:
    // 在两个柱子间做一次合法移动（移动较小的盘子）
    void moveBetween(vector<int>& a, vector<int>& b) {
        if (a.empty()) {
            a.push_back(b.back()); b.pop_back();
        } else if (b.empty()) {
            b.push_back(a.back()); a.pop_back();
        } else if (a.back() < b.back()) {
            b.push_back(a.back()); a.pop_back();
        } else {
            a.push_back(b.back()); b.pop_back();
        }
    }
};


/*
 * =====================================================
 * 复杂度分析
 * =====================================================
 *
 * 递归解法:
 *   时间: O(2^n)
 *     T(n) = 2*T(n-1) + 1, 解为 T(n) = 2^n - 1
 *     这是数学最优解 — 不存在更快的算法
 *     n=14 时约 16383 次操作
 *
 *   空间: O(n)
 *     递归栈深度最多 n 层
 *
 * 迭代解法:
 *   时间: O(2^n) — 同样的总步数
 *   空间: O(1)   — 无递归栈
 *
 * =====================================================
 * 面试要点
 * =====================================================
 *
 * 1. 递归三步法是标准答案，务必熟练掌握
 * 2. 能画出递归树（至少 n=2 或 n=3）展示理解深度
 * 3. 能推导 T(n) = 2^n - 1 是加分项
 * 4. 参数角色切换（buffer/target 互换）是核心考点
 * 5. base case 用 n==0 比 n==1 更简洁优雅
 * 6. push_back/pop_back 的顺序: 先读取再删除
 *
 * 易错点:
 *   1. 递归参数顺序搞混 → 盘子移到错误柱子; 画出角色切换: 第一次递归 dst↔buf
 *   2. base case 用 n==1 → 空输入(n==0)时出错; 应用 n==0
 *   3. 先 pop_back 再 push_back → 值丢失; 必须先 push_back(back()) 再 pop_back
 *   4. 试图追踪每一步状态 → 思路混乱; 信任递归, 定义清楚函数语义即可
 *
 * 常见面试追问：
 * Q: 能否不用递归？  A: 有迭代规律，但实现复杂
 * Q: 4 柱的汉诺塔？  A: Frame-Stewart 算法，非经典考点
 * Q: 移动次数多少？  A: 2^n - 1，用递推或数学归纳证明
 */
