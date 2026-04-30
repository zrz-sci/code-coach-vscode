// LeetCode 838: Push Dominoes
// 解法一：两次扫描（力的模拟）
// 时间复杂度: O(n), 空间复杂度: O(n)

class Solution {
public:
    string pushDominoes(string dominoes) {
        int n = dominoes.size();

        // rightForce[i] = 位置i受到的右推力（从左边的R传来）
        // leftForce[i]  = 位置i受到的左推力（从右边的L传来）
        vector<int> rightForce(n, 0);
        vector<int> leftForce(n, 0);

        // 从左到右扫描，计算右推力
        // 遇到'R'时力为n，之后每步衰减1，遇到'L'时力清零
        int force = 0;
        for (int i = 0; i < n; i++) {
            if (dominoes[i] == 'R') {
                force = n; // 设置最大力
            } else if (dominoes[i] == 'L') {
                force = 0; // 被L阻挡，右推力消失
            } else {
                force = max(0, force - 1); // 衰减
            }
            rightForce[i] = force;
        }

        // 从右到左扫描，计算左推力
        force = 0;
        for (int i = n - 1; i >= 0; i--) {
            if (dominoes[i] == 'L') {
                force = n;
            } else if (dominoes[i] == 'R') {
                force = 0;
            } else {
                force = max(0, force - 1);
            }
            leftForce[i] = force;
        }

        // 根据净力决定每个位置的最终状态
        string result(n, '.');
        for (int i = 0; i < n; i++) {
            if (rightForce[i] > leftForce[i]) {
                result[i] = 'R';
            } else if (leftForce[i] > rightForce[i]) {
                result[i] = 'L';
            } else {
                result[i] = '.'; // 力平衡，保持竖立
            }
        }

        return result;
    }
};

// ================================================================
// 解法二：双指针分段处理
// 时间复杂度: O(n), 空间复杂度: O(1)（在原字符串上修改）

class Solution_TwoPointers {
public:
    string pushDominoes(string dominoes) {
        int n = dominoes.size();
        // 添加虚拟边界: 左端加'L'，右端加'R'
        // 这样所有'.'段都被非'.'字符包围
        string s = "L" + dominoes + "R";
        int len = s.size();
        string result = s;

        int i = 0; // 左指针，指向非'.'字符
        for (int j = 1; j < len; j++) {
            if (s[j] == '.') continue;

            // s[i] 和 s[j] 是两个相邻的非'.'字符
            // 中间段是 s[i+1..j-1]

            if (s[i] == 'L' && s[j] == 'L') {
                // L...L: 全部变L
                for (int k = i + 1; k < j; k++) {
                    result[k] = 'L';
                }
            } else if (s[i] == 'R' && s[j] == 'R') {
                // R...R: 全部变R
                for (int k = i + 1; k < j; k++) {
                    result[k] = 'R';
                }
            } else if (s[i] == 'L' && s[j] == 'R') {
                // L...R: 全部保持'.'（两边的力背向而行）
                for (int k = i + 1; k < j; k++) {
                    result[k] = '.';
                }
            } else {
                // R...L: 两端向中间推
                int left = i + 1, right = j - 1;
                while (left < right) {
                    result[left] = 'R';
                    result[right] = 'L';
                    left++;
                    right--;
                }
                if (left == right) {
                    result[left] = '.'; // 中间位置力平衡
                }
            }

            i = j; // 移动左指针
        }

        // 去掉虚拟边界
        return result.substr(1, n);
    }
};

// ================================================================
// 解法三：BFS模拟（多源BFS）
// 时间复杂度: O(n), 空间复杂度: O(n)

class Solution_BFS {
public:
    string pushDominoes(string dominoes) {
        int n = dominoes.size();
        string result = dominoes;

        // 队列中存储 (位置, 方向)
        queue<pair<int, char>> q;

        // 将所有初始推倒的骨牌加入队列
        for (int i = 0; i < n; i++) {
            if (dominoes[i] != '.') {
                q.push({i, dominoes[i]});
            }
        }

        while (!q.empty()) {
            int sz = q.size();
            // 记录本轮被推倒的位置和方向
            // 用map来检测同一位置是否被左右同时推
            unordered_map<int, int> affected; // position -> net force (R=+1, L=-1)

            for (int i = 0; i < sz; i++) {
                auto [pos, dir] = q.front(); q.pop();

                int nextPos = (dir == 'R') ? pos + 1 : pos - 1;

                if (nextPos < 0 || nextPos >= n) continue;
                if (result[nextPos] != '.') continue; // 已被推倒

                affected[nextPos] += (dir == 'R') ? 1 : -1;
            }

            // 处理本轮结果
            for (auto& [pos, force] : affected) {
                if (force > 0) {
                    result[pos] = 'R';
                    q.push({pos, 'R'});
                } else if (force < 0) {
                    result[pos] = 'L';
                    q.push({pos, 'L'});
                }
                // force == 0: 力平衡，保持'.'
            }
        }

        return result;
    }
};

// ================================================================
// 解法四：利用时间戳（记录每个位置被推倒的时间）
// 时间复杂度: O(n), 空间复杂度: O(n)

class Solution_Timestamp {
public:
    string pushDominoes(string dominoes) {
        int n = dominoes.size();

        // timeR[i] = 从右边推来的力到达位置i的时间（INF表示不可达）
        // timeL[i] = 从左边推来的力到达位置i的时间
        vector<int> timeR(n, INT_MAX);
        vector<int> timeL(n, INT_MAX);

        // 从左到右：计算R推力到达每个位置的时间
        for (int i = 0; i < n; i++) {
            if (dominoes[i] == 'R') {
                timeR[i] = 0;
            } else if (dominoes[i] == 'L') {
                timeR[i] = INT_MAX; // L阻断R的推力
            } else if (i > 0 && timeR[i - 1] != INT_MAX) {
                timeR[i] = timeR[i - 1] + 1;
            }
        }

        // 从右到左：计算L推力到达每个位置的时间
        for (int i = n - 1; i >= 0; i--) {
            if (dominoes[i] == 'L') {
                timeL[i] = 0;
            } else if (dominoes[i] == 'R') {
                timeL[i] = INT_MAX;
            } else if (i < n - 1 && timeL[i + 1] != INT_MAX) {
                timeL[i] = timeL[i + 1] + 1;
            }
        }

        // 根据到达时间决定最终状态
        string result(n, '.');
        for (int i = 0; i < n; i++) {
            if (timeR[i] < timeL[i]) {
                result[i] = 'R';
            } else if (timeL[i] < timeR[i]) {
                result[i] = 'L';
            } else if (timeR[i] != INT_MAX) {
                result[i] = '.'; // 同时到达，保持竖立
            }
            // 如果两个都是INT_MAX，表示没有力到达，保持'.'
        }

        return result;
    }
};
