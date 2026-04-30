// LeetCode 593: Valid Square
// -----------------------------------------------
// 解法一：6 距离排序法 (推荐)
// 思路：计算所有 C(4,2)=6 对点的距离平方，排序后
//       正方形满足前4个相等(边)、后2个相等(对角线)、最小值>0
// 时间 O(1)  空间 O(1)  —— 常数个点

class Solution {
public:
    bool validSquare(vector<int>& p1, vector<int>& p2,
                     vector<int>& p3, vector<int>& p4) {
        // 距离平方，避免浮点误差
        auto dist = [](vector<int>& a, vector<int>& b) -> long long {
            long long dx = a[0] - b[0], dy = a[1] - b[1];
            return dx * dx + dy * dy;
        };

        vector<long long> d = {
            dist(p1, p2), dist(p1, p3), dist(p1, p4),
            dist(p2, p3), dist(p2, p4), dist(p3, p4)
        };
        sort(d.begin(), d.end());

        // 四条边相等 + 两条对角线相等 + 非退化
        return d[0] > 0
            && d[0] == d[1] && d[1] == d[2] && d[2] == d[3]
            && d[4] == d[5];
    }
};


// -----------------------------------------------
// 解法二：集合去重法
// 思路：正方形的 6 条距离恰好只有 2 种不同的值
//       (4 条边 + 2 条对角线)，且最小值 > 0
// 时间 O(1)  空间 O(1)

class Solution2 {
public:
    bool validSquare(vector<int>& p1, vector<int>& p2,
                     vector<int>& p3, vector<int>& p4) {
        auto dist = [](vector<int>& a, vector<int>& b) -> long long {
            long long dx = a[0] - b[0], dy = a[1] - b[1];
            return dx * dx + dy * dy;
        };

        vector<vector<int>*> pts = {&p1, &p2, &p3, &p4};
        set<long long> s;
        for (int i = 0; i < 4; i++)
            for (int j = i + 1; j < 4; j++)
                s.insert(dist(*pts[i], *pts[j]));

        // 恰好两种距离 + 最小距离 > 0
        return s.size() == 2 && *s.begin() > 0;
    }
};


// -----------------------------------------------
// 解法三：向量验证法
// 思路：选定一个点，找到两条邻边向量
//       验证：(1)等长 (2)垂直(点积=0) (3)第四点位置正确
// 时间 O(1)  空间 O(1)

class Solution3 {
public:
    bool validSquare(vector<int>& p1, vector<int>& p2,
                     vector<int>& p3, vector<int>& p4) {
        // 尝试所有排列来找到正确的邻接关系
        return check(p1, p2, p3, p4) || check(p1, p3, p2, p4)
            || check(p1, p2, p4, p3);
    }

private:
    bool check(vector<int>& a, vector<int>& b,
               vector<int>& c, vector<int>& d) {
        // a-b 和 a-c 是邻边，d 是对角点
        int abx = b[0] - a[0], aby = b[1] - a[1];
        int acx = c[0] - a[0], acy = c[1] - a[1];

        // 非零长度
        if (abx == 0 && aby == 0) return false;

        // 等长
        if (abx * abx + aby * aby != acx * acx + acy * acy) return false;

        // 垂直 (点积 = 0)
        if (abx * acx + aby * acy != 0) return false;

        // 第四点 d 应该等于 b + ac 向量 (即 a + ab + ac)
        return d[0] == a[0] + abx + acx && d[1] == a[1] + aby + acy;
    }
};
