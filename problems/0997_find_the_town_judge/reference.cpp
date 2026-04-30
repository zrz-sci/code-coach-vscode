// [file://solution.cpp](./solution.cpp)
//
// LeetCode 997: Find the Town Judge
// 解法一: 净信任度数组 (推荐)
// 解法二: 分别统计入度和出度
// 解法三: HashSet 排除法

// ============================================================
// 解法一: 净信任度数组
// 时间 O(E + n), 空间 O(n)
// 核心思想: score[i] = 被信任次数 - 信任他人次数
//           法官的 score 恰好等于 n-1
// ============================================================
class Solution {
public:
    int findJudge(int n, vector<vector<int>>& trust) {
        // score[i] 表示第 i 个人的净信任度
        // 被信任时 +1, 信任别人时 -1
        vector<int> score(n + 1, 0);

        for (auto& t : trust) {
            int a = t[0];  // a 信任 b
            int b = t[1];
            score[a]--;    // a 信任了别人, 不可能是法官
            score[b]++;    // b 被信任, 增加候选分数
        }

        // 法官: 被所有其他 n-1 人信任, 且自己不信任任何人
        // 等价于 score == n - 1
        for (int i = 1; i <= n; i++) {
            if (score[i] == n - 1) {
                return i;
            }
        }

        return -1;  // 没有法官
    }
};

// ============================================================
// 解法二: 分别统计入度和出度
// 时间 O(E + n), 空间 O(n)
// 更直观地分别检查两个条件
// ============================================================
class SolutionInOutDegree {
public:
    int findJudge(int n, vector<vector<int>>& trust) {
        // inDeg[i]  = 有多少人信任 i
        // outDeg[i] = i 信任了多少人
        vector<int> inDeg(n + 1, 0);
        vector<int> outDeg(n + 1, 0);

        for (auto& t : trust) {
            outDeg[t[0]]++;
            inDeg[t[1]]++;
        }

        // 法官: 入度 == n-1 (所有其他人信任他)
        //       出度 == 0   (他不信任任何人)
        for (int i = 1; i <= n; i++) {
            if (inDeg[i] == n - 1 && outDeg[i] == 0) {
                return i;
            }
        }

        return -1;
    }
};

// ============================================================
// 解法三: HashSet 排除法
// 时间 O(E + n), 空间 O(n)
// 先排除所有信任别人的人, 再验证候选法官
// ============================================================
class SolutionHashSet {
public:
    int findJudge(int n, vector<vector<int>>& trust) {
        // 收集所有信任他人的人 (这些人不可能是法官)
        unordered_set<int> trustsSomeone;
        // 统计每个人被信任的次数
        vector<int> trustedBy(n + 1, 0);

        for (auto& t : trust) {
            trustsSomeone.insert(t[0]);
            trustedBy[t[1]]++;
        }

        // 在不信任任何人的候选中, 找被 n-1 人信任的
        for (int i = 1; i <= n; i++) {
            if (trustsSomeone.find(i) == trustsSomeone.end()
                && trustedBy[i] == n - 1) {
                return i;
            }
        }

        return -1;
    }
};

// ============================================================
// 补充: O(n) 通用汇点算法 (邻接矩阵版本)
// 面试追问: 如何在邻接矩阵中 O(n) 找到通用汇点?
// 双指针法: 每次比较排除一个候选
// ============================================================
class SolutionUniversalSink {
public:
    // 假设 adj[i][j] = true 表示 i 信任 j
    int findJudgeMatrix(int n, vector<vector<bool>>& adj) {
        // 候选法官从 1 开始
        int candidate = 1;

        // 第一轮: 排除非法官候选
        for (int i = 2; i <= n; i++) {
            // 如果候选信任 i, 候选不可能是法官
            if (adj[candidate][i]) {
                candidate = i;
            }
            // 如果候选不信任 i, i 不可能是法官 (法官被所有人信任)
            // 所以 candidate 保持不变
        }

        // 第二轮: 验证 candidate 是否真的是法官
        for (int i = 1; i <= n; i++) {
            if (i == candidate) continue;
            // 法官不信任任何人, 且被所有人信任
            if (adj[candidate][i] || !adj[i][candidate]) {
                return -1;
            }
        }

        return candidate;
    }
};
