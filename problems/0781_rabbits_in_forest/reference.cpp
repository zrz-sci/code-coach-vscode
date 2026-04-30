// LeetCode 781: Rabbits in Forest
// 难度: Medium | 标签: Greedy, Array, Hash Table, Math
//
// 解法一：哈希表计数 + 贪心（推荐）
// 时间 O(n)  空间 O(n)
//
// 核心思路：
// 回答 k 的兔子属于大小 k+1 的颜色组。
// 回答相同的尽量归入同组（贪心），超出容量则开新组。
// 每组贡献 k+1 只兔子。

class Solution {
public:
    int numRabbits(vector<int>& answers) {
        unordered_map<int, int> count;
        for (int a : answers) {
            count[a]++;
        }

        int total = 0;
        for (auto& [k, cnt] : count) {
            int groupSize = k + 1;                     // 每组有 k+1 只兔子
            int groups = (cnt + groupSize - 1) / groupSize; // 向上取整
            total += groups * groupSize;
        }
        return total;
    }
};

// =============================================================
// 解法二：哈希表计数 + 数学公式（等价写法）
// 时间 O(n)  空间 O(n)
//
// 用 (cnt + k) / (k + 1) * (k + 1) 替代向上取整，
// 更简洁但本质相同。

class Solution2 {
public:
    int numRabbits(vector<int>& answers) {
        unordered_map<int, int> count;
        for (int a : answers) count[a]++;

        int total = 0;
        for (auto& [k, cnt] : count) {
            // ceil(cnt / (k+1)) * (k+1)
            // = ((cnt + k) / (k + 1)) * (k + 1)
            total += (cnt + k) / (k + 1) * (k + 1);
        }
        return total;
    }
};

// =============================================================
// 解法三：排序 + 贪心分组
// 时间 O(n log n)  空间 O(1)（除排序外）
//
// 排序后相同回答值相邻，顺序扫描分组。
// 每组最多容纳 k+1 只兔子，满了就开新组。

class Solution3 {
public:
    int numRabbits(vector<int>& answers) {
        sort(answers.begin(), answers.end());
        int total = 0;
        int i = 0;
        int n = answers.size();

        while (i < n) {
            int k = answers[i];
            int groupSize = k + 1;

            // 贪心：尽量把相同回答值的兔子塞进一组
            // 一组最多 groupSize 只
            total += groupSize;
            int slots = groupSize; // 当前组还能容纳多少只

            // 消耗掉当前组的槽位
            i++; // 第一只已经占了一个位
            slots--;

            while (i < n && answers[i] == k && slots > 0) {
                i++;
                slots--;
            }
            // 此时要么 i 越界、要么答案不同、要么当前组满了
        }
        return total;
    }
};

// =============================================================
// 解法四：在线处理（边遍历边计算）
// 时间 O(n)  空间 O(n)
//
// 不等统计完再算，而是遍历过程中维护每个回答值当前组的剩余容量。
// 当剩余容量用完或者第一次见到该值时，开一个新组。

class Solution4 {
public:
    int numRabbits(vector<int>& answers) {
        unordered_map<int, int> remaining; // 每个回答值当前组的剩余容量
        int total = 0;

        for (int k : answers) {
            if (remaining.find(k) == remaining.end() || remaining[k] == 0) {
                // 开新组
                total += k + 1;
                remaining[k] = k; // 还能容纳 k 只（自己已占 1 位）
            } else {
                // 加入现有组
                remaining[k]--;
            }
        }
        return total;
    }
};

// =============================================================
// 解法五：数组代替哈希表（常数优化）
// 时间 O(n + M)  空间 O(M)，M = 1000
//
// 由于 answers[i] < 1000，用数组代替哈希表，减少常数因子。

class Solution5 {
public:
    int numRabbits(vector<int>& answers) {
        int count[1000] = {};
        for (int a : answers) count[a]++;

        int total = 0;
        for (int k = 0; k < 1000; k++) {
            if (count[k] == 0) continue;
            int groupSize = k + 1;
            int groups = (count[k] + groupSize - 1) / groupSize;
            total += groups * groupSize;
        }
        return total;
    }
};
