// 解法一：逐滴模拟（推荐）
// 时间 O(volume * n)，空间 O(1) 额外
// 每滴水从 k 出发，先向左找最低点，不行再向右，都不行留原地
class Solution {
public:
    vector<int> pourWater(vector<int>& heights, int volume, int k) {
        int n = heights.size();

        for (int v = 0; v < volume; ++v) {
            int dropPos = k; // 水最终停留位置

            // 第一步：向左扫描，寻找能下降到的最低位置
            int bestLeft = k;
            for (int i = k - 1; i >= 0; --i) {
                if (heights[i] > heights[i + 1]) {
                    // 遇到上坡，不能继续向左
                    break;
                }
                if (heights[i] < heights[bestLeft]) {
                    bestLeft = i;
                }
            }

            if (heights[bestLeft] < heights[k]) {
                // 左边有更低的位置，水流到那里
                dropPos = bestLeft;
            } else {
                // 第二步：左边不行，向右扫描
                int bestRight = k;
                for (int i = k + 1; i < n; ++i) {
                    if (heights[i] > heights[i - 1]) {
                        // 遇到上坡，不能继续向右
                        break;
                    }
                    if (heights[i] < heights[bestRight]) {
                        bestRight = i;
                    }
                }

                if (heights[bestRight] < heights[k]) {
                    // 右边有更低的位置
                    dropPos = bestRight;
                }
                // 否则 dropPos 保持为 k
            }

            // 水落在 dropPos
            heights[dropPos]++;
        }

        return heights;
    }
};

// 解法二：更清晰的辅助函数版本
// 将向某方向扫描最低点抽取为 helper
class Solution2 {
public:
    vector<int> pourWater(vector<int>& heights, int volume, int k) {
        for (int v = 0; v < volume; ++v) {
            int pos = findDropPosition(heights, k);
            heights[pos]++;
        }
        return heights;
    }

private:
    // 找到一滴水从位置 k 出发最终停留的位置
    int findDropPosition(const vector<int>& heights, int k) {
        int n = heights.size();

        // 向左扫描
        int leftPos = scanDirection(heights, k, -1, n);
        if (leftPos != k) return leftPos;

        // 向右扫描
        int rightPos = scanDirection(heights, k, 1, n);
        if (rightPos != k) return rightPos;

        // 水留在原地
        return k;
    }

    // direction: -1 向左, +1 向右
    // 返回水能到达的最低位置，如果不能下降则返回 start
    int scanDirection(const vector<int>& heights, int start, int dir, int n) {
        int bestPos = start;
        int i = start + dir;

        while (i >= 0 && i < n) {
            if (heights[i] > heights[i - dir]) {
                // 遇到上坡，停止
                break;
            }
            if (heights[i] < heights[bestPos]) {
                bestPos = i;
            }
            i += dir;
        }

        // 只有严格低于起始高度才算「最终下降」
        if (heights[bestPos] < heights[start]) {
            return bestPos;
        }
        return start;
    }
};

// 解法三：优化版 - 减少重复扫描
// 在连续倒水时，如果地形变化不大，可以缓存上次的扫描结果
// 但由于 n <= 100, volume <= 2000, 暴力已足够
// 此处展示一种用双指针快速定位的紧凑写法
class Solution3 {
public:
    vector<int> pourWater(vector<int>& heights, int volume, int k) {
        int n = heights.size();

        for (int v = 0; v < volume; ++v) {
            // 向左：找 k 左侧的谷底
            int left = k;
            int cur = k - 1;
            while (cur >= 0 && heights[cur] <= heights[cur + 1]) {
                if (heights[cur] < heights[left]) {
                    left = cur;
                }
                --cur;
            }

            if (left != k) {
                heights[left]++;
                continue;
            }

            // 向右：找 k 右侧的谷底
            int right = k;
            cur = k + 1;
            while (cur < n && heights[cur] <= heights[cur - 1]) {
                if (heights[cur] < heights[right]) {
                    right = cur;
                }
                ++cur;
            }

            if (right != k) {
                heights[right]++;
                continue;
            }

            // 留在原地
            heights[k]++;
        }

        return heights;
    }
};
