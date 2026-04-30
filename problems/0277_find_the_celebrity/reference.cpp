/*
 * LeetCode 277: 搜寻名人 (Find the Celebrity)
 * 
 * 【题目本质】
 * 在 n 个人的"认识"关系图中，找到入度=n-1、出度=0 的节点（名人），
 * 只能通过交互 API knows(a,b) 查询。核心是最小化 API 调用次数。
 *
 * 【解法总览】
 * 解法1: 暴力枚举        — O(n²) / O(1) — 最直觉
 * 解法2: 候选人淘汰法    — O(n)  / O(1) — 最优 (⭐ 面试首选)
 * 解法3: 栈淘汰法        — O(n)  / O(n) — 不同组织形式
 */

// 前向声明: knows API
bool knows(int a, int b);

// ============================================================
// 解法1: 暴力枚举 — 逐个检查每个人是否是名人
// 时间: O(n²)  空间: O(1)
//
// 【思路】
// 对每个人 i，遍历所有其他人 j，检查:
//   (1) j 是否认识 i  → 名人被所有人认识
//   (2) i 是否不认识 j → 名人不认识任何人
// 两个条件都满足才是名人。
//
// 瓶颈: 外层 n 人 × 内层 n-1 次检查 = O(n²) 次 knows 调用。
// 没有利用"一次 knows 调用能淘汰一个候选人"的信息。
// ============================================================
class Solution1 {
public:
    int findCelebrity(int n) {
        for (int i = 0; i < n; i++) {
            bool isCelebrity = true;
            for (int j = 0; j < n; j++) {
                if (i == j) continue;
                // 只要有一个人不认识i，或i认识某人，i就不是名人
                if (!knows(j, i) || knows(i, j)) {
                    isCelebrity = false;
                    break;
                }
            }
            if (isCelebrity) return i;
        }
        return -1;
    }
};

// ============================================================
// 解法2: 候选人淘汰法 — 每次调用淘汰一人，O(n) 找到候选再验证
// 时间: O(n)  空间: O(1)
// knows 调用次数: ≤ 3(n-1)  满足 Follow-up
//
// 【思路】
// 关键洞察: 每次 knows(a, b) 都能淘汰一人:
//   knows(a,b) = true  → a认识b → a不是名人 → 淘汰a
//   knows(a,b) = false → a不认识b → b不被所有人认识 → b不是名人 → 淘汰b
//
// 第一步(擂台赛): 从 0 号开始，依次和 1,2,...,n-1 比较，
//   每次淘汰一人，n-1 轮后剩唯一候选人。
//
// 第二步(验证): 候选人只是"最有可能"，需要确认:
//   - 所有人都认识他 (入度=n-1)
//   - 他不认识任何人 (出度=0)
//
// 示例1 淘汰过程:
//   graph = [[1,1,0],[0,1,0],[1,1,1]]
//
//   candidate=0
//   i=1: knows(0,1)=true → 0认识1 → 淘汰0 → candidate=1
//   i=2: knows(1,2)=false → 1不认识2 → 淘汰2 → candidate=1
//
//   验证 candidate=1:
//     i=0: knows(0,1)=true ✓, knows(1,0)=false ✓
//     i=2: knows(2,1)=true ✓, knows(1,2)=false ✓
//   → 返回 1
//
// 示例2 淘汰过程:
//   graph = [[1,0,1],[1,1,0],[0,1,1]]
//
//   candidate=0
//   i=1: knows(0,1)=false → 淘汰1 → candidate=0
//   i=2: knows(0,2)=true → 淘汰0 → candidate=2
//
//   验证 candidate=2:
//     i=0: knows(0,2)=true ✓, knows(2,0)=false ✓
//     i=1: knows(1,2)=false ✗ → 1不认识2 → 2不是名人
//   → 返回 -1
// ============================================================
class Solution2 {
public:
    int findCelebrity(int n) {
        // 第一步: 找候选人 — n-1 次 knows 调用
        int candidate = 0;
        for (int i = 1; i < n; i++) {
            if (knows(candidate, i)) {
                // candidate 认识 i → candidate 不可能是名人
                // 把 i 推上擂台
                candidate = i;
            }
            // else: candidate 不认识 i → i 不可能是名人 → candidate 保持
        }
        
        // 第二步: 验证候选人 — 最多 2(n-1) 次 knows 调用
        for (int i = 0; i < n; i++) {
            if (i == candidate) continue;
            
            // 条件1: 所有人都认识候选人
            // 条件2: 候选人不认识任何人
            if (!knows(i, candidate) || knows(candidate, i)) {
                return -1; // 候选人不满足名人条件 → 没有名人
            }
        }
        
        return candidate;
    }
};

// ============================================================
// 解法3: 栈淘汰法 — 用栈组织两两比较
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 与解法2 本质相同，用栈来组织淘汰过程:
//   - 把所有人入栈
//   - 每次弹出两个人比较，淘汰一个，胜者压回栈
//   - 最后栈中剩一人即为候选
//   - 验证与解法2 相同
//
// 淘汰过程示意 (n=4, 假设 2 是名人):
//   栈: [0, 1, 2, 3]
//   弹出3和2: knows(3,2)=true → 淘汰3 → 栈: [0, 1, 2]
//   弹出2和1: knows(2,1)=false → 淘汰1 → 栈: [0, 2]
//   弹出2和0: knows(2,0)=false → 淘汰0 → 栈: [2]
//   候选人=2, 验证通过 → 返回2
//
// 注意: 栈方法多用了 O(n) 空间，在面试中解法2更优。
// ============================================================
class Solution3 {
public:
    int findCelebrity(int n) {
        // 所有人入栈
        stack<int> stk;
        for (int i = 0; i < n; i++) {
            stk.push(i);
        }
        
        // 两两PK，每轮淘汰一人
        while (stk.size() > 1) {
            int a = stk.top(); stk.pop();
            int b = stk.top(); stk.pop();
            if (knows(a, b)) {
                // a认识b → a不是名人 → b胜出
                stk.push(b);
            } else {
                // a不认识b → b不是名人 → a胜出
                stk.push(a);
            }
        }
        
        int candidate = stk.top();
        
        // 验证 (与解法2完全相同)
        for (int i = 0; i < n; i++) {
            if (i == candidate) continue;
            if (!knows(i, candidate) || knows(candidate, i)) {
                return -1;
            }
        }
        
        return candidate;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法      | knows调用次数 | 空间  | 适用场景           |
// |-----------|---------------|-------|--------------------|
// | 暴力枚举  | O(n²)         | O(1)  | 理解题意的起步     |
// | 候选人淘汰| ≤ 3(n-1)      | O(1)  | ⭐ 面试首选        |
// | 栈淘汰    | ≤ 3(n-1)      | O(n)  | 概念上更直观       |
//
// 解法2 vs 解法3: 淘汰逻辑完全相同。解法2用线性扫描(擂台赛)，
// 解法3用栈(锦标赛)。解法2更简洁且省空间，面试中优先。
//
// 【易错点】
//
// 1. 找到候选人后直接返回不验证:
//    ✗ return candidate; // 候选只是"最有可能"，不一定是名人
//    ✓ 必须遍历所有人验证入度和出度条件
//
// 2. 验证时只检查单方向:
//    ✗ if (!knows(i, candidate)) return -1; // 漏检候选人的出度
//    ✓ if (!knows(i, candidate) || knows(candidate, i)) return -1;
//
// 3. 验证时忘记跳过 i==candidate:
//    knows(x, x) 恒为 true，如果不跳过会在 knows(candidate, i)
//    检查中误判 (因为 knows(candidate, candidate)=true，
//    会认为候选人"认识某人")
//
// 4. 候选人初始值写成 -1:
//    ✗ int candidate = -1; // 第一轮比较 knows(-1, 0) 未定义行为
//    ✓ int candidate = 0;  // 从第一个人开始
//
// 【面试追问】
//
// Q1: 一次 knows(a,b) 调用能获得什么信息？
// → 无论返回 true 或 false，都能排除 a 或 b 中的一个不是名人。
//   true: a认识b → 淘汰a。 false: a不认识b → 淘汰b。
//
// Q2: 为什么淘汰后还需要验证？举个反例？
// → 淘汰过程只保证候选人不因"认识别人"被淘汰，但没确认"所有人认识他"。
//   反例: n=3, 没有人被所有人认识，淘汰仍会留下一个候选，验证时才能发现不合格。
//
// Q3: 总共最少需要多少次 knows 调用？能否做到 < 3(n-1)？
// → 信息论下界: 需要确认 n-1 个人不是名人 + 验证候选人的 2(n-1) 条边，
//   但第一遍的 n-1 次调用中有些信息可在验证时复用(如 knows(candidate,i)
//   在淘汰时已知为 false)，所以验证时只需额外 n-1 次(检查别人认识候选人)，
//   实际可优化到约 2(n-1) 次。但渐近仍是 O(n)。
//
// Q4: 能否存在两个名人？
// → 不可能。若A和B都是名人，则A认识B(B被所有人认识)但A不认识任何人——矛盾。
// ============================================================
