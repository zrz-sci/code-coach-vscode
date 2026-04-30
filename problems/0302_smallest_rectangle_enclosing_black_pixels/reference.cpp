/*
 * 302. Smallest Rectangle Enclosing Black Pixels
 *
 * 【题目本质】
 * 找包含所有连通黑像素的最小轴对齐矩形。
 * 利用连通性 -> 行列投影单调 -> 二分搜索四边界。
 *
 * 【解法总览】
 * Solution1: 二分搜索四边界  O(m*logn + n*logm) / O(1)  ⭐推荐
 * Solution2: DFS 遍历        O(k) / O(k)  k=黑像素数
 */

// ===================== Solution1: 二分搜索四边界 =====================
// 思路：分别二分查找上/下/左/右边界。
// 关键：连通黑像素投影到行/列上是连续的，所以可以二分。
// top    = [0, x] 中第一个含黑像素的行
// bottom = [x, m-1] 中最后一个含黑像素的行
// left   = [0, y] 中第一个含黑像素的列
// right  = [y, n-1] 中最后一个含黑像素的列
class Solution1 {
public:
    int minArea(vector<vector<char>>& image, int x, int y) {
        int m = image.size(), n = image[0].size();

        int top    = searchFirst(image, 0, x, true);
        int bottom = searchLast(image, x, m - 1, true) + 1;
        int left   = searchFirst(image, 0, y, false);
        int right  = searchLast(image, y, n - 1, false) + 1;

        return (bottom - top) * (right - left);
    }

private:
    // 找第一个含黑像素的行/列 (左边界二分)
    int searchFirst(vector<vector<char>>& image, int lo, int hi, bool isRow) {
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (hasBlack(image, mid, isRow))
                hi = mid;
            else
                lo = mid + 1;
        }
        return lo;
    }

    // 找最后一个含黑像素的行/列 (右边界二分)
    int searchLast(vector<vector<char>>& image, int lo, int hi, bool isRow) {
        while (lo < hi) {
            int mid = lo + (hi - lo + 1) / 2;  // 上取整！
            if (hasBlack(image, mid, isRow))
                lo = mid;
            else
                hi = mid - 1;
        }
        return lo;
    }

    // 判断某行/列是否含黑像素
    bool hasBlack(vector<vector<char>>& image, int idx, bool isRow) {
        if (isRow) {
            for (char c : image[idx])
                if (c == '1') return true;
        } else {
            for (auto& row : image)
                if (row[idx] == '1') return true;
        }
        return false;
    }
};

// ===================== Solution2: DFS 遍历 =====================
// 思路：从已知黑像素出发 DFS，遍历所有黑像素，记录上下左右极值。
// 缺点：时间 O(k)（k=黑像素数），最坏 O(mn)。
class Solution2 {
public:
    int minArea(vector<vector<char>>& image, int x, int y) {
        int m = image.size(), n = image[0].size();
        int top = m, bottom = 0, left = n, right = 0;

        dfs(image, x, y, m, n, top, bottom, left, right);

        return (bottom - top + 1) * (right - left + 1);
    }

private:
    void dfs(vector<vector<char>>& image, int r, int c,
             int m, int n, int& top, int& bot, int& left, int& right) {
        if (r < 0 || r >= m || c < 0 || c >= n || image[r][c] != '1')
            return;

        image[r][c] = '2';  // mark visited
        top   = min(top, r);
        bot   = max(bot, r);
        left  = min(left, c);
        right = max(right, c);

        int dirs[] = {0, 1, 0, -1, 0};
        for (int d = 0; d < 4; d++) {
            dfs(image, r + dirs[d], c + dirs[d + 1],
                m, n, top, bot, left, right);
        }
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | Solution1 二分 | O(m*logn+n*logm) | O(1) | 满足<O(mn)要求 |
 * | Solution2 DFS | O(k) | O(k) | 简单但不满足题意复杂度 |
 *
 * 【易错点】
 * 1. 右边界二分用上取整 mid = lo+(hi-lo+1)/2，否则死循环
 * 2. 面积计算：统一 exclusive 右边界 (bottom-top)*(right-left)
 * 3. 二分搜索区间必须包含已知黑像素坐标 x/y
 * 4. hasBlack 检查行时遍历列，检查列时遍历行，别搞反
 *
 * 【面试追问】
 * Q1: 为什么能二分？-> 连通黑像素投影到行/列是连续区间
 * Q2: 不连通怎么办？-> 投影不连续，二分失效，只能O(mn)扫描
 * Q3: 能否只用2次二分？-> 不行，需要分别找4个边界方向
 */
