# 593. 有效的正方形 (Valid Square)

## 核心思路
四个点构成正方形的充要条件可以通过**距离法**判断：计算所有 C(4,2)=6 条边的距离平方，排序后应得到 4 条相等的短边和 2 条相等的长边（对角线），且短边大于 0。距离用平方避免浮点误差。

## 思维链
1. 四个点不保证顺序，不能假设 p1-p2 是一条边
2. 正方形有 4 条等边 + 2 条等对角线，对角线长度 = 边长 * sqrt(2)
3. 计算所有 6 对点距离的平方，避免开根号引入浮点误差
4. 排序后前 4 个值必须相等（边），后 2 个值必须相等（对角线）
5. 额外验证：对角线平方 = 2 * 边平方（这是正方形的独有性质，菱形不满足）
6. 边长必须 > 0，排除四点重合的退化情况

## 解法概览
| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| 6 距离排序法 | O(1) | O(1) | 最直观，面试首选 |
| 向量垂直+等长验证 | O(1) | O(1) | 更数学化，需要找邻接点 |
| 集合去重法 | O(1) | O(1) | 用 set 统计不同距离数 |

## 关键提示

### 提示 1 - 距离平方避免浮点
```
dist_sq(a, b) = (a.x - b.x)^2 + (a.y - b.y)^2
```
用整数运算，完全避免浮点比较。这是计算几何题的标准技巧。

### 提示 2 - 排序后的判定条件
将 6 个距离平方排序后得到数组 `d[0..5]`：
- `d[0] == d[1] == d[2] == d[3]`：四条边等长
- `d[4] == d[5]`：两条对角线等长
- `d[0] > 0`：排除退化情况（四点重合）

### 提示 3 - 为什么不需要额外验证对角线=sqrt(2)*边？
排序后 `d[0..3]` 是较小的 4 个值，`d[4..5]` 是较大的 2 个值。如果四边等长且两对角线等长，几何上只能是正方形或退化情况（已被 `d[0] > 0` 排除）。实际上菱形的 6 条距离排序后是 4 个相等 + 2 个不等（短对角线 < 长对角线），所以只要 `d[4] == d[5]` 就排除了菱形。

### 提示 4 - 集合法替代排序
也可以将 6 个距离放入 set，正方形的 set 大小恰好为 2（边和对角线两种值），且较大值 = 2 * 较小值。

## 解法详解

### 解法一：6 距离排序法（推荐）
```
class Solution {
public:
    bool validSquare(vector<int>& p1, vector<int>& p2,
                     vector<int>& p3, vector<int>& p4) {
        auto dist = [](vector<int>& a, vector<int>& b) -> long long {
            long long dx = a[0] - b[0], dy = a[1] - b[1];
            return dx * dx + dy * dy;
        };

        vector<long long> d = {
            dist(p1, p2), dist(p1, p3), dist(p1, p4),
            dist(p2, p3), dist(p2, p4), dist(p3, p4)
        };
        sort(d.begin(), d.end());

        return d[0] > 0
            && d[0] == d[1] && d[1] == d[2] && d[2] == d[3]
            && d[4] == d[5];
    }
};
```

### 解法二：集合去重法
```
class Solution {
public:
    bool validSquare(vector<int>& p1, vector<int>& p2,
                     vector<int>& p3, vector<int>& p4) {
        auto dist = [](vector<int>& a, vector<int>& b) -> long long {
            long long dx = a[0] - b[0], dy = a[1] - b[1];
            return dx * dx + dy * dy;
        };

        set<long long> s;
        vector<vector<int>*> pts = {&p1, &p2, &p3, &p4};
        for (int i = 0; i < 4; i++)
            for (int j = i + 1; j < 4; j++)
                s.insert(dist(*pts[i], *pts[j]));

        // 正方形恰好有两种不同的距离，且最小值 > 0
        return s.size() == 2 && *s.begin() > 0;
    }
};
```

## 易错点
1. **忘记排除四点重合**：四个相同的点也满足"4边等长+2对角线等长"（全为0），必须检查 `d[0] > 0`
2. **使用浮点距离**：`sqrt` 带来精度问题，应始终用距离平方进行整数比较
3. **误以为 4 边等长就是正方形**：菱形也四边等长，但对角线不等，需同时验证对角线
4. **int 溢出**：坐标范围 [-10^4, 10^4]，距离平方最大 2*10^8，int 够用；但如果坐标更大需要 long long
5. **混淆矩形和正方形**：矩形的 6 距离排序是 2+2+2（三种值），正方形是 4+2（两种值）

## 面试追问
1. **如何判断是否构成矩形？** - 6 距离排序后 `d[0]==d[1]&&d[2]==d[3]&&d[4]==d[5]`，三对相等
2. **如果输入是 N 个点，判断是否能构成正方形？** - C(N,4) 枚举所有四元组，每组检查
3. **如何判断正方形是否与坐标轴对齐？** - 检查是否存在两点 x 相等且两点 y 相等
4. **浮点坐标怎么处理？** - 用 epsilon 做近似比较，或将坐标乘以公倍数转为整数
5. **如何用向量叉积/点积来验证？** - 找到一个顶点的两条邻边向量，验证垂直（点积=0）且等长

## 相关题型
- [223. Rectangle Area](https://leetcode.com/problems/rectangle-area/) - 矩形面积计算
- [391. Perfect Rectangle](https://leetcode.com/problems/perfect-rectangle/) - 完美矩形判定
- [963. Minimum Area Rectangle II](https://leetcode.com/problems/minimum-area-rectangle-ii/) - 最小面积矩形
- [939. Minimum Area Rectangle](https://leetcode.com/problems/minimum-area-rectangle/) - 与坐标轴对齐的最小矩形
- [587. Erect the Fence](https://leetcode.com/problems/erect-the-fence/) - 凸包算法
