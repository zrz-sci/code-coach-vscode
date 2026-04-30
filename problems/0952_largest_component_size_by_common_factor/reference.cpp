// LeetCode 952: Largest Component Size by Common Factor
// 解法一：Union-Find + 质因数分解（推荐）
// 时间复杂度: O(n * sqrt(M)), M = max(nums[i])
// 空间复杂度: O(M)

class Solution {
public:
    // Union-Find 数据结构
    vector<int> parent, rank_;

    int find(int x) {
        // 路径压缩
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        // 按秩合并
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
    }

    int largestComponentSize(vector<int>& nums) {
        // 找到最大值，确定 parent 数组大小
        int maxVal = *max_element(nums.begin(), nums.end());

        // 初始化 Union-Find
        parent.resize(maxVal + 1);
        rank_.resize(maxVal + 1, 0);
        for (int i = 0; i <= maxVal; i++) {
            parent[i] = i;
        }

        // 对每个数进行质因数分解，与其质因数 union
        for (int num : nums) {
            int n = num;
            // 试除法分解质因数
            for (int p = 2; p * p <= n; p++) {
                if (n % p == 0) {
                    // 将 num 与质因数 p 合并
                    unite(num, p);
                    // 除尽所有的 p
                    while (n % p == 0) {
                        n /= p;
                    }
                }
            }
            // 如果剩余部分 > 1，说明有一个大质因数
            if (n > 1) {
                unite(num, n);
            }
        }

        // 统计每个连通分量中原始数字的个数
        unordered_map<int, int> componentSize;
        int maxSize = 0;

        for (int num : nums) {
            int root = find(num);
            componentSize[root]++;
            maxSize = max(maxSize, componentSize[root]);
        }

        return maxSize;
    }
};


// 解法二：埃氏筛预处理 + Union-Find
// 用最小质因数表加速分解
// 时间复杂度: O(M * loglogM + n * logM)
// 空间复杂度: O(M)

class Solution2 {
public:
    vector<int> parent, rank_;

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
    }

    int largestComponentSize(vector<int>& nums) {
        int maxVal = *max_element(nums.begin(), nums.end());

        // 埃氏筛：预处理最小质因数（Smallest Prime Factor）
        vector<int> spf(maxVal + 1, 0);
        for (int i = 2; i <= maxVal; i++) {
            if (spf[i] == 0) {  // i 是质数
                for (int j = i; j <= maxVal; j += i) {
                    if (spf[j] == 0) {
                        spf[j] = i;
                    }
                }
            }
        }

        // 初始化 Union-Find
        parent.resize(maxVal + 1);
        rank_.resize(maxVal + 1, 0);
        for (int i = 0; i <= maxVal; i++) parent[i] = i;

        // 用 SPF 表快速分解并 union
        for (int num : nums) {
            int n = num;
            while (n > 1) {
                int p = spf[n];
                unite(num, p);
                while (n % p == 0) n /= p;
            }
        }

        // 统计结果
        unordered_map<int, int> componentSize;
        int maxSize = 0;
        for (int num : nums) {
            int root = find(num);
            componentSize[root]++;
            maxSize = max(maxSize, componentSize[root]);
        }

        return maxSize;
    }
};


// 解法三：HashMap 版 Union-Find（节省空间）
// 当 nums 值域很大但数量少时更优
// 时间复杂度: O(n * sqrt(M))
// 空间复杂度: O(n * 质因数个数)

class Solution3 {
public:
    unordered_map<int, int> parent, rank_;

    int find(int x) {
        if (parent.find(x) == parent.end()) {
            parent[x] = x;
            rank_[x] = 0;
        }
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
    }

    int largestComponentSize(vector<int>& nums) {
        for (int num : nums) {
            int n = num;
            for (int p = 2; p * p <= n; p++) {
                if (n % p == 0) {
                    unite(num, p);
                    while (n % p == 0) n /= p;
                }
            }
            if (n > 1) unite(num, n);
        }

        unordered_map<int, int> count;
        int maxSize = 0;
        for (int num : nums) {
            maxSize = max(maxSize, ++count[find(num)]);
        }
        return maxSize;
    }
};
