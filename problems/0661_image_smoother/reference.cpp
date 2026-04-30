// 661. Image Smoother
// 难度: Easy
// 标签: Array, Matrix

// =====================================================
// 解法一：暴力遍历（推荐）
// 时间 O(m*n)  空间 O(m*n)
// =====================================================
// 对每个像素遍历 3x3 邻域，累加有效像素值和计数，取均值

class Solution {
public:
    vector<vector<int>> imageSmoother(vector<vector<int>>& img) {
        int m = img.size(), n = img[0].size();
        vector<vector<int>> result(m, vector<int>(n, 0));

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                int sum = 0, count = 0;

                // 遍历 3x3 邻域
                for (int ni = i - 1; ni <= i + 1; ni++) {
                    for (int nj = j - 1; nj <= j + 1; nj++) {
                        // 检查边界
                        if (ni >= 0 && ni < m && nj >= 0 && nj < n) {
                            sum += img[ni][nj];
                            count++;
                        }
                    }
                }

                result[i][j] = sum / count;  // 整数除法自动向下取整
            }
        }

        return result;
    }
};

// =====================================================
// 解法二：原地修改（位编码）
// 时间 O(m*n)  空间 O(1)（不算输出）
// =====================================================
// 利用像素值 0~255 只占 8 位，将新值编码到 int 高位
// 第一遍编码新值到高位，第二遍提取

class Solution2 {
public:
    vector<vector<int>> imageSmoother(vector<vector<int>>& img) {
        int m = img.size(), n = img[0].size();

        // 第一遍：计算新值并编码到高 8 位
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                int sum = 0, count = 0;

                for (int ni = i - 1; ni <= i + 1; ni++) {
                    for (int nj = j - 1; nj <= j + 1; nj++) {
                        if (ni >= 0 && ni < m && nj >= 0 && nj < n) {
                            // 只读取低 8 位（原始值）
                            sum += (img[ni][nj] & 0xFF);
                            count++;
                        }
                    }
                }

                int newVal = sum / count;
                // 将新值编码到高位（第 8~15 位）
                img[i][j] |= (newVal << 8);
            }
        }

        // 第二遍：提取新值
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                img[i][j] >>= 8;  // 右移 8 位得到新值
            }
        }

        return img;
    }
};

// =====================================================
// 解法三：二维前缀和
// 时间 O(m*n)  空间 O(m*n)
// =====================================================
// 预计算前缀和后 O(1) 查询任意矩形区域和

class Solution3 {
public:
    vector<vector<int>> imageSmoother(vector<vector<int>>& img) {
        int m = img.size(), n = img[0].size();

        // 构建前缀和数组 (m+1) x (n+1)，避免边界特判
        vector<vector<int>> prefix(m + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                prefix[i][j] = img[i-1][j-1]
                             + prefix[i-1][j]
                             + prefix[i][j-1]
                             - prefix[i-1][j-1];
            }
        }

        vector<vector<int>> result(m, vector<int>(n, 0));

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                // 确定 3x3 邻域的有效边界
                int r1 = max(0, i - 1), c1 = max(0, j - 1);
                int r2 = min(m - 1, i + 1), c2 = min(n - 1, j + 1);

                // 有效像素数量
                int count = (r2 - r1 + 1) * (c2 - c1 + 1);

                // 用前缀和 O(1) 求区域和
                // 注意前缀和数组下标偏移 +1
                int sum = prefix[r2+1][c2+1]
                        - prefix[r1][c2+1]
                        - prefix[r2+1][c1]
                        + prefix[r1][c1];

                result[i][j] = sum / count;
            }
        }

        return result;
    }
};

// =====================================================
// 示例模拟
// =====================================================
// img = [[1,1,1],[1,0,1],[1,1,1]]
//
// 像素 (0,0): 邻域 = {(0,0)=1, (0,1)=1, (1,0)=1, (1,1)=0}
//   sum=3, count=4, result=3/4=0
//
// 像素 (1,1): 邻域 = 全部 9 个像素
//   sum=1+1+1+1+0+1+1+1+1=8, count=9, result=8/9=0
//
// 所有像素均值 < 1，向下取整均为 0 -> [[0,0,0],[0,0,0],[0,0,0]]
//
// img = [[100,200,100],[200,50,200],[100,200,100]]
//
// 像素 (0,0): 邻域 = {100, 200, 200, 50}
//   sum=550, count=4, 550/4=137
//
// 像素 (0,1): 邻域 = {100, 200, 100, 200, 50, 200}
//   sum=850, count=6, 850/6=141
//
// 像素 (1,1): 邻域 = 全部 9 个
//   sum=100+200+100+200+50+200+100+200+100=1250, count=9, 1250/9=138
