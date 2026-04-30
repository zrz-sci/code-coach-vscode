// LeetCode 481: Magical String
// 核心：利用自描述性质，双指针模拟生成神奇字符串
//
// 神奇字符串 s = "1221121221221121122..."
// 分组: {1}{22}{11}{2}{1}{22}{1}{22}{11}...
// 组长: 1  2   2  1  1  2   1  2   2 ... = s 本身！
//
// 时间 O(n)，空间 O(n)

// ==================== 解法1: vector 模拟（推荐）====================
class Solution {
public:
    int magicalString(int n) {
        if (n == 0) return 0;
        if (n <= 3) return 1;

        // 初始种子："122"
        // 对应分组 {1}, {22}, 已经确定了前 3 个字符
        vector<int> s = {1, 2, 2};

        int i = 2;       // 读指针：s[i] 告诉我们当前组有多长
        int num = 1;     // 下一组要写入的字符（1 和 2 交替）

        // 持续生成直到长度 >= n
        while ((int)s.size() < n) {
            int groupLen = s[i];  // 当前组的长度（1 或 2）

            // 追加 groupLen 个 num
            for (int j = 0; j < groupLen; ++j) {
                s.push_back(num);
            }

            // 切换字符：1 -> 2 -> 1 -> 2 ...
            num = 3 - num;
            ++i;
        }

        // 统计前 n 个字符中 '1' 的个数
        int count = 0;
        for (int j = 0; j < n; ++j) {
            if (s[j] == 1) ++count;
        }
        return count;
    }
};

// ==================== 解法2: string 模拟 ====================
// 使用 string 类型，代码更简洁
class Solution_String {
public:
    int magicalString(int n) {
        if (n == 0) return 0;
        if (n <= 3) return 1;

        string s = "122";
        int i = 2;
        char ch = '1';  // 下一组要添加的字符

        while ((int)s.size() < n) {
            // s[i] - '0' 得到组长度（1 或 2）
            int groupLen = s[i] - '0';

            // append(count, char) 追加 count 个 char
            s.append(groupLen, ch);

            // 切换字符
            ch = (ch == '1') ? '2' : '1';
            ++i;
        }

        // 统计前 n 个字符中 '1' 的个数
        int count = 0;
        for (int j = 0; j < n; ++j) {
            if (s[j] == '1') ++count;
        }
        return count;
    }
};

// ==================== 解法3: 边生成边统计（优化常数）====================
// 在生成过程中同步统计 1 的个数，避免最后的一次遍历
class Solution_Optimized {
public:
    int magicalString(int n) {
        if (n == 0) return 0;
        if (n <= 3) return 1;

        vector<int> s = {1, 2, 2};
        int i = 2;
        int num = 1;
        int count = 1;  // 前 3 个字符中有 1 个 '1'

        while ((int)s.size() < n) {
            int groupLen = s[i];
            for (int j = 0; j < groupLen; ++j) {
                s.push_back(num);
                // 只有在还没超过 n 的范围内才统计
                if ((int)s.size() <= n && num == 1) {
                    ++count;
                }
            }
            num = 3 - num;
            ++i;
        }

        return count;
    }
};

// ==================== 模拟过程示例 ====================
// 初始: s = [1, 2, 2], i=2, num=1
//
// i=2: s[2]=2, 追加 2 个 1 → s = [1,2,2,1,1], num→2, i→3
// i=3: s[3]=1, 追加 1 个 2 → s = [1,2,2,1,1,2], num→1, i→4
// i=4: s[4]=1, 追加 1 个 1 → s = [1,2,2,1,1,2,1], num→2, i→5
// i=5: s[5]=2, 追加 2 个 2 → s = [1,2,2,1,1,2,1,2,2], num→1, i→6
//
// 前 6 个字符: "122112", 其中 1 出现 3 次 → 输出 3

// ==================== 复杂度分析 ====================
// 时间：O(n) — 每个字符最多被生成一次
// 空间：O(n) — 存储生成的字符串
//
// 注意：不能用 O(1) 空间，因为读指针需要回读之前生成的字符
// 来决定当前组的长度
