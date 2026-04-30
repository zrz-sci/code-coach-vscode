/*
 * 【题目本质】
 * 返回杨辉三角的第 rowIndex 行（0-indexed），本质是高效计算一行组合数。
 * Follow-up 要求 O(rowIndex) 空间。
 *
 * 【解法总览】
 * 1. 原地滚动DP - 单数组从后往前更新   O(n^2) / O(n)
 * 2. 数学公式   - C(n,k) 递推计算      O(n)   / O(n)
 *
 * 杨辉三角与组合数的关系：
 *
 *   row 0:       C(0,0)                    1
 *   row 1:     C(1,0) C(1,1)              1 1
 *   row 2:    C(2,0) C(2,1) C(2,2)       1 2 1
 *   row 3:  C(3,0) C(3,1) C(3,2) C(3,3) 1 3 3 1
 *
 *   递推关系: C(n,k) = C(n-1,k-1) + C(n-1,k)
 *   row[j] = old_row[j-1] + old_row[j]  → 从后往前原地更新
 */

// ===================== 解法1：原地滚动DP =====================
// 思路：只维护一个数组表示"当前行"，从后往前更新。
// 为什么从后往前？因为 row[j] = row[j] + row[j-1]，如果从前往后，
// row[j-1] 已经被更新为当前行的值，而我们需要的是上一行的 row[j-1]。
// 从后往前时，row[j-1] 还没被处理，仍保持上一行的值。
class Solution1 {
public:
    vector<int> getRow(int rowIndex) {
        vector<int> row(rowIndex + 1, 0);
        row[0] = 1;

        for (int i = 1; i <= rowIndex; i++) {
            // 关键：从后往前遍历
            for (int j = i; j >= 1; j--) {
                row[j] = row[j] + row[j - 1];
            }
            // row[0] 始终为 1，不需要更新
        }

        return row;
    }
};

// ===================== 解法2：数学公式法 =====================
// 思路：第 n 行第 k 个元素 = C(n, k)。
// 利用递推: C(n, k) = C(n, k-1) * (n-k+1) / k
// 为什么能保证整除？因为连续 k 个整数中必有一个能被 k 整除。
// 更严格地说，C(n,k) 本身是整数，所以递推每步结果也一定是整数。
class Solution2 {
public:
    vector<int> getRow(int rowIndex) {
        vector<int> row(rowIndex + 1);
        row[0] = 1;

        for (int k = 1; k <= rowIndex; k++) {
            // 必须用 long long 防止中间乘法溢出
            // 例如 rowIndex=33, k=16 时 row[15]=1251677700, 乘以18会溢出int
            row[k] = (long long)row[k - 1] * (rowIndex - k + 1) / k;
        }

        return row;
    }
};

// ===================== 解法3：两数组交替 =====================
// 思路：用 prev 和 curr 两个数组交替，逐行构建。
// 比解法1多用一个数组，但更容易理解（不需要考虑遍历方向）。
class Solution3 {
public:
    vector<int> getRow(int rowIndex) {
        vector<int> prev = {1};

        for (int i = 1; i <= rowIndex; i++) {
            vector<int> curr(i + 1, 1);
            for (int j = 1; j < i; j++) {
                curr[j] = prev[j - 1] + prev[j];
            }
            prev = curr;
        }

        return prev;
    }
};

/*
 * 【解法对比】
 * | 解法       | 时间    | 空间   | 优点                  | 缺点            |
 * |-----------|--------|-------|----------------------|----------------|
 * | 原地滚动DP | O(n^2) | O(n)  | 最常用，面试首选        | 需理解遍历方向    |
 * | 数学公式   | O(n)   | O(n)  | 时间最优              | 溢出风险，需long long |
 * | 两数组交替 | O(n^2) | O(n)  | 直观                  | 多一个数组        |
 *
 * 【易错点】
 * 1. 滚动DP必须从后往前：从前往后会用已更新的值，导致结果错误
 *    ✗ for(j=1;j<=i;j++) row[j]+=row[j-1];  // row[j-1]已被当前行覆盖
 *    ✓ for(j=i;j>=1;j--) row[j]+=row[j-1];  // row[j-1]还是上一行的值
 * 2. 数学公式必须先乘后除且用long long：
 *    ✗ row[k] = row[k-1] * (rowIndex-k+1) / k;        // int溢出
 *    ✓ row[k] = (long long)row[k-1] * (rowIndex-k+1) / k;
 * 3. 数组大小是 rowIndex+1（第0行有1个元素，第n行有n+1个元素）
 *
 * 【面试追问】
 * Q1: 为什么滚动数组从后往前就安全了？
 *     → 计算 row[j] 时需要 row[j-1]（上一行），从后往前时 j-1 < j，
 *       还没被当前行更新过，所以值正确
 * Q2: 数学公式法的时间复杂度为什么是 O(n)？
 *     → 只有一层循环，每步 O(1) 的乘除运算，共 n 步
 * Q3: 如果需要对大素数 p 取模，怎么算 C(n,k) mod p？
 *     → 用 Lucas 定理或预处理阶乘 + 费马小定理求逆元
 */
