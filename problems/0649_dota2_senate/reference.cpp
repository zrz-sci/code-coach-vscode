// LeetCode 649: Dota2 Senate
// 解法一：双队列贪心
// 时间复杂度: O(n)  空间复杂度: O(n)

// ============================================================
// 核心思想：
// 用两个队列分别存储 R 和 D 参议员的索引。每次比较两个
// 队列的队首，索引小的先行动并 ban 对方，然后加 n 入队
// 代表进入下一轮。某一方队列为空时另一方获胜。
// ============================================================

class Solution {
public:
    string predictPartyVictory(string senate) {
        int n = senate.size();
        queue<int> qR, qD;

        // 将每个参议员的索引分别放入对应队列
        for (int i = 0; i < n; ++i) {
            if (senate[i] == 'R') {
                qR.push(i);
            } else {
                qD.push(i);
            }
        }

        // 模拟投票过程
        while (!qR.empty() && !qD.empty()) {
            int rIdx = qR.front(); qR.pop();
            int dIdx = qD.front(); qD.pop();

            if (rIdx < dIdx) {
                // R 先行动，ban D
                // R 进入下一轮（索引 + n）
                qR.push(rIdx + n);
            } else {
                // D 先行动，ban R
                // D 进入下一轮（索引 + n）
                qD.push(dIdx + n);
            }
        }

        return qR.empty() ? "Dire" : "Radiant";
    }
};

// ============================================================
// 解法二：模拟 + ban 计数器
// 时间复杂度: O(n^2) 最坏情况  空间复杂度: O(n)
// ============================================================

class Solution_Simulate {
public:
    string predictPartyVictory(string senate) {
        int n = senate.size();
        // banR = 待消耗的 ban R 的次数（由 D 积累）
        // banD = 待消耗的 ban D 的次数（由 R 积累）
        int banR = 0, banD = 0;

        // 用 string 模拟，被 ban 的不加入下一轮
        bool changed = true;

        while (changed) {
            changed = false;
            string next;

            for (char c : senate) {
                if (c == 'R') {
                    if (banR > 0) {
                        // R 被之前的 D ban 了
                        banR--;
                        changed = true;
                        // 不加入 next（被淘汰）
                    } else {
                        // R 行使权利，ban 下一个 D
                        banD++;
                        next += 'R';
                    }
                } else if (c == 'D') {
                    if (banD > 0) {
                        // D 被之前的 R ban 了
                        banD--;
                        changed = true;
                        // 不加入 next（被淘汰）
                    } else {
                        // D 行使权利，ban 下一个 R
                        banR++;
                        next += 'D';
                    }
                }
            }

            senate = next;

            // 检查是否只剩一方
            bool hasR = false, hasD = false;
            for (char c : senate) {
                if (c == 'R') hasR = true;
                if (c == 'D') hasD = true;
            }
            if (!hasR) return "Dire";
            if (!hasD) return "Radiant";
        }

        // 不会到达这里
        return "";
    }
};

// ============================================================
// 解法三：单队列计数变体
// 时间复杂度: O(n)  空间复杂度: O(n)
// ============================================================

class Solution_Count {
public:
    string predictPartyVictory(string senate) {
        int n = senate.size();
        queue<int> q;  // 存储活跃参议员的索引

        // 初始化：所有参议员入队
        for (int i = 0; i < n; ++i) {
            q.push(i);
        }

        // pendingBanR: R 阵营被 ban 的积压数
        // pendingBanD: D 阵营被 ban 的积压数
        int pendingBanR = 0, pendingBanD = 0;
        int rCount = count(senate.begin(), senate.end(), 'R');
        int dCount = n - rCount;

        while (rCount > 0 && dCount > 0) {
            int idx = q.front(); q.pop();
            char c = senate[idx % n];  // 使用原始字符

            if (c == 'R') {
                if (pendingBanR > 0) {
                    // 这个 R 被 ban 了
                    pendingBanR--;
                    rCount--;
                } else {
                    // R 行使 ban 权，ban 下一个 D
                    pendingBanD++;
                    q.push(idx + n);
                }
            } else {
                if (pendingBanD > 0) {
                    // 这个 D 被 ban 了
                    pendingBanD--;
                    dCount--;
                } else {
                    // D 行使 ban 权，ban 下一个 R
                    pendingBanR++;
                    q.push(idx + n);
                }
            }
        }

        return rCount > 0 ? "Radiant" : "Dire";
    }
};

// ============================================================
// 手动模拟（双队列）：senate = "RDD"
//
// 初始：qR = [0], qD = [1, 2]
// n = 3
//
// 第 1 轮迭代：
//   rIdx=0, dIdx=1 => 0 < 1, R 先行动 ban D
//   qR.push(0+3=3), qR=[3], qD=[2]
//
// 第 2 轮迭代：
//   rIdx=3, dIdx=2 => 3 > 2, D 先行动 ban R
//   qD.push(2+3=5), qR=[], qD=[5]
//
// qR 为空 => 返回 "Dire"
// ============================================================

// ============================================================
// 手动模拟（双队列）：senate = "RD"
//
// 初始：qR = [0], qD = [1]
// n = 2
//
// 第 1 轮：
//   rIdx=0, dIdx=1 => 0 < 1, R 先行动 ban D
//   qR.push(0+2=2), qR=[2], qD=[]
//
// qD 为空 => 返回 "Radiant"
// ============================================================

// ============================================================
// 手动模拟（双队列）：senate = "DRRDRDRDRDDRDRDR"
//
// 初始：
//   qR = [2, 3, 5, 7, 9, 12, 14]  (7 个 R)
//   qD = [0, 1, 4, 6, 8, 10, 11, 13, 15]  (9 个 D)
//   n = 16
//
// D 数量多于 R，直觉上 D 会赢。
// 每次比较队首：
//   D[0]=0 vs R[2]=2: D 赢, qD.push(16)
//   D[1]=1 vs R[3]=3: D 赢, qD.push(17)
//   D[4]=4 vs R[5]=5: D 赢, qD.push(20)
//   ...
// 最终 qR 清空 => "Dire"
// ============================================================
