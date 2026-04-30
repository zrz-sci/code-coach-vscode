/*
 * 【题目本质】
 * 构造一个 2^n 长度的排列，使得相邻元素的二进制表示恰好差一位（含首尾循环）。
 *
 * 【解法总览】
 * 1. 镜像反射法 - 递推构建，每轮翻转+加高位  O(2^n) / O(2^n)
 * 2. 位运算公式 - G(i) = i ^ (i >> 1)          O(2^n) / O(2^n)
 * 3. 回溯搜索   - DFS + visited 搜索合法排列   O(2^n * n) / O(2^n)
 *
 * 镜像反射法示意（n=3）：
 *
 *   n=1:  [0, 1]
 *          ↓ 镜像翻转 [1, 0] + 加高位 10 → [11, 10]
 *   n=2:  [00, 01, 11, 10]
 *          ↓ 镜像翻转 [10, 11, 01, 00] + 加高位 100 → [110, 111, 101, 100]
 *   n=3:  [000, 001, 011, 010, 110, 111, 101, 100]
 */

// ===================== 解法1：镜像反射法 =====================
// 思路：n 位格雷码 = (n-1 位格雷码高位补0) + (n-1 位格雷码倒序高位补1)
// 为什么倒序？因为接缝处（原序列最后一个元素和翻转后第一个元素）是同一个数，
// 只在新的高位不同，所以恰好差一位。
class Solution1 {
public:
    vector<int> grayCode(int n) {
        vector<int> result;
        result.push_back(0);

        for (int i = 0; i < n; i++) {
            int size = result.size();
            // 核心：从后向前遍历，每个数加上第 i 位的 1
            // 这样保证 result[size-1] 和 result[size]（新加的第一个）只差第 i 位
            for (int j = size - 1; j >= 0; j--) {
                result.push_back(result[j] | (1 << i));
            }
        }

        return result;
    }
};

// ===================== 解法2：位运算公式法 =====================
// 思路：格雷编码的第 i 个值 = i XOR (i >> 1)
// 为什么成立？考虑 i 和 i+1：它们的二进制从最低位开始第一个不同的位为 k，
// i 在第 k 位为 0（后面都是 1），i+1 在第 k 位为 1（后面都是 0）。
// 经过 XOR 右移一位后，恰好只在一位上不同。
class Solution2 {
public:
    vector<int> grayCode(int n) {
        vector<int> result;
        int total = 1 << n;

        for (int i = 0; i < total; i++) {
            result.push_back(i ^ (i >> 1));
        }

        return result;
    }
};

// ===================== 解法3：回溯法 =====================
// 思路：从 0 开始，每次尝试翻转一位，搜索一条合法的哈密顿回路。
// 虽然理论复杂度高，但由于格雷码总是存在且选择少（每步只有 n 个选择），
// 实际回溯很少发生。
class Solution3 {
public:
    vector<int> grayCode(int n) {
        int total = 1 << n;
        vector<int> result;
        vector<bool> visited(total, false);

        result.push_back(0);
        visited[0] = true;
        backtrack(result, visited, n, total);

        return result;
    }

private:
    bool backtrack(vector<int>& result, vector<bool>& visited, int n, int total) {
        if ((int)result.size() == total) {
            // 验证首尾只差一位
            int diff = result.front() ^ result.back();
            return diff > 0 && (diff & (diff - 1)) == 0;
        }

        int last = result.back();
        for (int i = 0; i < n; i++) {
            int next = last ^ (1 << i);
            if (!visited[next]) {
                visited[next] = true;
                result.push_back(next);
                if (backtrack(result, visited, n, total)) return true;
                result.pop_back();
                visited[next] = false;
            }
        }
        return false;
    }
};

/*
 * 【解法对比】
 * | 解法       | 时间     | 空间     | 优点             | 缺点           |
 * |-----------|---------|---------|-----------------|---------------|
 * | 镜像反射法  | O(2^n)  | O(2^n)  | 直观，面试推导加分  | 需要理解镜像原理  |
 * | 位运算公式  | O(2^n)  | O(2^n)  | 代码极简          | 需要记公式       |
 * | 回溯法     | O(2^n*n)| O(2^n)  | 通用思路          | 较慢，不推荐     |
 *
 * 【易错点】
 * 1. 镜像法必须逆序遍历，正序遍历会导致接缝处差多位
 * 2. 位运算优先级：i ^ (i >> 1) 中括号不能省（虽然 >> 优先级高于 ^，但习惯加括号）
 * 3. 回溯法验证首尾时，diff==0 表示首尾相同（不合法），要单独排除
 *
 * 【面试追问】
 * Q1: 镜像法为什么首尾也只差一位？
 *     → 首元素始终是 0，尾元素是 1<<(n-1)，只在最高位不同
 * Q2: 给定格雷码如何求原始二进制？
 *     → num=0; while(g) { num^=g; g>>=1; }
 * Q3: 如何生成从任意起点开始的格雷码？（LC 1238）
 *     → 先生成标准格雷码，再找到 start 的位置做循环移位，
 *       或者直接对每个元素 XOR start
 */
