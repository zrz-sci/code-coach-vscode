// 697. Degree of an Array
// 难度: Easy
// 标签: Array, Hash Table

/*
 * 题目描述：
 * 给定一个非空非负整数数组 nums，数组的"度"定义为出现频率最高的元素的频率。
 * 找到与 nums 具有相同度的最短连续子数组，返回其长度。
 *
 * 核心思路：
 * 一次遍历，用哈希表记录每个元素的 count、firstIndex、lastIndex，
 * 然后在所有达到最大频率的元素中取 last - first + 1 最小的。
 */

// ==================== 解法一：三哈希表一次遍历（推荐）====================
// 时间 O(N)，空间 O(N)
// 思路：一次遍历收集频率、首末位置，动态维护答案
class Solution {
public:
    int findShortestSubArray(vector<int>& nums) {
        // count: 元素 -> 出现次数
        // first: 元素 -> 首次出现位置
        // last:  元素 -> 末次出现位置
        unordered_map<int, int> count, first, last;

        int maxFreq = 0;  // 数组的度
        int minLen = nums.size();  // 最短子数组长度

        for (int i = 0; i < (int)nums.size(); i++) {
            int x = nums[i];
            count[x]++;

            // 首次出现时记录位置（不覆盖）
            if (first.find(x) == first.end()) {
                first[x] = i;
            }
            // 每次都更新末次出现位置
            last[x] = i;

            int len = last[x] - first[x] + 1;

            // 更新最大频率和最短长度
            if (count[x] > maxFreq) {
                maxFreq = count[x];
                minLen = len;
            } else if (count[x] == maxFreq) {
                minLen = min(minLen, len);
            }
        }

        return minLen;
    }
};

// ==================== 解法二：结构体合并哈希表 ====================
// 时间 O(N)，空间 O(N)
// 思路：将 count/first/last 合并到一个 map 中，减少哈希表数量
class Solution_Struct {
public:
    int findShortestSubArray(vector<int>& nums) {
        // info[x] = {count, firstIndex, lastIndex}
        unordered_map<int, vector<int>> info;

        for (int i = 0; i < (int)nums.size(); i++) {
            int x = nums[i];
            if (info.find(x) == info.end()) {
                info[x] = {1, i, i};
            } else {
                info[x][0]++;    // count++
                info[x][2] = i;  // 更新 lastIndex
            }
        }

        int maxFreq = 0, minLen = nums.size();

        for (auto& [val, v] : info) {
            int cnt = v[0], fi = v[1], la = v[2];
            int len = la - fi + 1;

            if (cnt > maxFreq) {
                maxFreq = cnt;
                minLen = len;
            } else if (cnt == maxFreq) {
                minLen = min(minLen, len);
            }
        }

        return minLen;
    }
};

// ==================== 解法三：两步法 ====================
// 时间 O(N)，空间 O(N)
// 思路：先找度，再找最短区间（逻辑更清晰）
class Solution_TwoPass {
public:
    int findShortestSubArray(vector<int>& nums) {
        int n = nums.size();

        // Step 1: 统计每个元素的频率、首末位置
        unordered_map<int, int> count, first, last;
        int maxFreq = 0;

        for (int i = 0; i < n; i++) {
            int x = nums[i];
            count[x]++;
            if (first.find(x) == first.end()) first[x] = i;
            last[x] = i;
            maxFreq = max(maxFreq, count[x]);
        }

        // Step 2: 在所有达到最大频率的元素中找最短区间
        int minLen = n;
        for (auto& [val, cnt] : count) {
            if (cnt == maxFreq) {
                minLen = min(minLen, last[val] - first[val] + 1);
            }
        }

        return minLen;
    }
};

// ==================== 解法四：数组替代哈希表（空间优化） ====================
// 时间 O(N)，空间 O(50000) = O(1)
// 思路：题目约束 nums[i] <= 49999，可以用固定大小数组替代哈希表
class Solution_Array {
public:
    int findShortestSubArray(vector<int>& nums) {
        const int MAXVAL = 50000;
        vector<int> count(MAXVAL, 0);
        vector<int> first(MAXVAL, -1);
        vector<int> last(MAXVAL, -1);

        int maxFreq = 0;

        for (int i = 0; i < (int)nums.size(); i++) {
            int x = nums[i];
            count[x]++;
            if (first[x] == -1) first[x] = i;
            last[x] = i;
            maxFreq = max(maxFreq, count[x]);
        }

        int minLen = nums.size();
        for (int x = 0; x < MAXVAL; x++) {
            if (count[x] == maxFreq) {
                minLen = min(minLen, last[x] - first[x] + 1);
            }
        }

        return minLen;
    }
};

/*
 * 复杂度分析：
 * - 时间：所有解法均为 O(N)，N = nums.length
 * - 空间：O(N) 或 O(50000)（数组版本）
 *
 * 关键要点：
 * 1. 度 = 最大频率；最短同度子数组 = 包含某最高频元素所有出现的最短连续区间
 * 2. 区间长度 = lastIndex - firstIndex + 1，注意 +1 避免 off-by-one
 * 3. first 只在元素首次出现时记录，last 每次更新
 * 4. 多个元素可能同时达到最大频率，需要全部检查取最短
 * 5. 一次遍历解法边扫描边更新答案，适合流式数据
 *
 * 面试推荐：解法一最优，代码简洁高效。
 * 解法三逻辑更清晰，适合解释思路时使用。
 */
