// ========================================
// 657. Robot Return to Origin
// ========================================
// 解法一：坐标模拟（推荐）
// 时间 O(n)  空间 O(1)
// ========================================
class Solution {
public:
    bool judgeCircle(string moves) {
        int x = 0, y = 0;

        for (char c : moves) {
            switch (c) {
                case 'U': y++; break;
                case 'D': y--; break;
                case 'L': x--; break;
                case 'R': x++; break;
            }
        }

        // 回到原点当且仅当水平和垂直方向净位移都为零
        return x == 0 && y == 0;
    }
};

// ========================================
// 解法二：字符计数法
// 时间 O(n)  空间 O(1)
// ========================================
// 回到原点等价于 L 和 R 数量相等，U 和 D 数量相等
class Solution2 {
public:
    bool judgeCircle(string moves) {
        int countU = 0, countD = 0, countL = 0, countR = 0;

        for (char c : moves) {
            if (c == 'U') countU++;
            else if (c == 'D') countD++;
            else if (c == 'L') countL++;
            else if (c == 'R') countR++;
        }

        return countU == countD && countL == countR;
    }
};

// ========================================
// 解法三：STL count + 提前剪枝
// 时间 O(n)  空间 O(1)
// ========================================
// 利用 STL 的 count 函数使代码更简洁
// 先检查字符串长度奇偶性做快速排除
class Solution3 {
public:
    bool judgeCircle(string moves) {
        // 剪枝：奇数长度的移动序列不可能回到原点
        if (moves.size() % 2 != 0) return false;

        // 统计各方向出现次数
        int u = count(moves.begin(), moves.end(), 'U');
        int d = count(moves.begin(), moves.end(), 'D');
        int l = count(moves.begin(), moves.end(), 'L');
        int r = count(moves.begin(), moves.end(), 'R');

        return u == d && l == r;
    }
};

// ========================================
// 解法四：哈希表计数（通用方案）
// 时间 O(n)  空间 O(1)
// ========================================
// 使用 map 计数，适合方向较多的扩展场景
class Solution4 {
public:
    bool judgeCircle(string moves) {
        // 每个方向对坐标的影响
        // U -> (0,1), D -> (0,-1), L -> (-1,0), R -> (1,0)
        unordered_map<char, pair<int,int>> dir = {
            {'U', {0, 1}},
            {'D', {0, -1}},
            {'L', {-1, 0}},
            {'R', {1, 0}}
        };

        int x = 0, y = 0;
        for (char c : moves) {
            auto& [dx, dy] = dir[c];
            x += dx;
            y += dy;
        }

        return x == 0 && y == 0;
    }
};

// ========================================
// 解法五：位运算技巧（趣味解法）
// 时间 O(n)  空间 O(1)
// ========================================
// 将水平和垂直位移分别用两个变量跟踪
// 利用字符的 ASCII 性质做分支优化
class Solution5 {
public:
    bool judgeCircle(string moves) {
        int horizontal = 0;  // L/R 方向净位移
        int vertical = 0;    // U/D 方向净位移

        for (char c : moves) {
            // ASCII: U=85, D=68, L=76, R=82
            // U 和 D 差 17，L 和 R 差 6
            if (c == 'U' || c == 'D') {
                vertical += (c == 'U') ? 1 : -1;
            } else {
                horizontal += (c == 'R') ? 1 : -1;
            }
        }

        return horizontal == 0 && vertical == 0;
    }
};
