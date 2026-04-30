// ============================================================
// 638. Shopping Offers
// ============================================================
// n 种物品，每种有单价 price[i]；一些大礼包 special 以优惠价捆绑销售
// 求恰好满足 needs 的最低花费（不能超买，大礼包可重复使用）
// 约束: n <= 6, price[i]/needs[i] <= 10, special.length <= 100
// ============================================================


// ============================================================
// 解法一：记忆化搜索 + map（推荐）
// ============================================================
// 思路：
//   状态 = 当前 needs 向量
//   对每个状态，先算原价购买的花费（上界）
//   再尝试每个合法的大礼包，取 min
//   用 map<vector<int>, int> 记忆化
//
// 预处理：过滤掉不划算的大礼包（价格 >= 单独购买价）
//
// 时间: O(S * K * n), S = 状态数(最大11^6), K = 礼包数, n = 物品数
// 空间: O(S * n) 记忆化存储
class Solution {
public:
    int shoppingOffers(vector<int>& price, vector<vector<int>>& special, vector<int>& needs) {
        int n = price.size();

        // 预处理：过滤掉不划算的大礼包
        vector<vector<int>> validOffers;
        for (auto& offer : special) {
            int singleCost = 0;
            bool hasItem = false;
            for (int i = 0; i < n; ++i) {
                singleCost += offer[i] * price[i];
                if (offer[i] > 0) hasItem = true;
            }
            // 只保留价格低于单独购买、且至少包含一件物品的礼包
            if (hasItem && offer[n] < singleCost) {
                validOffers.push_back(offer);
            }
        }

        map<vector<int>, int> memo;
        return dfs(price, validOffers, needs, memo);
    }

private:
    int dfs(vector<int>& price, vector<vector<int>>& offers,
            vector<int>& needs, map<vector<int>, int>& memo) {
        // 查询记忆化
        auto it = memo.find(needs);
        if (it != memo.end()) return it->second;

        int n = price.size();

        // 基线：全部按原价购买
        int minCost = 0;
        for (int i = 0; i < n; ++i) {
            minCost += needs[i] * price[i];
        }

        // 尝试每个大礼包
        for (auto& offer : offers) {
            // 检查大礼包是否合法（不超出需求）
            vector<int> newNeeds(n);
            bool valid = true;
            for (int i = 0; i < n; ++i) {
                newNeeds[i] = needs[i] - offer[i];
                if (newNeeds[i] < 0) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                int cost = offer[n] + dfs(price, offers, newNeeds, memo);
                minCost = min(minCost, cost);
            }
        }

        memo[needs] = minCost;
        return minCost;
    }
};


// ============================================================
// 解法二：状态编码为整数 + unordered_map
// ============================================================
// 思路：
//   将 needs 向量编码为一个整数以提高哈希效率
//   每个物品需求量范围 [0, 10]，用基数 11 表示
//   6 种物品最多 11^6 ~ 177 万，一个 int 足够
//
// 编码: code = needs[0] * 1 + needs[1] * 11 + needs[2] * 121 + ...
//
// 时间: O(S * K * n)  空间: O(S)
class Solution2 {
public:
    int shoppingOffers(vector<int>& price, vector<vector<int>>& special, vector<int>& needs) {
        int n = price.size();

        // 预处理：过滤不划算的礼包
        vector<vector<int>> validOffers;
        for (auto& offer : special) {
            int singleCost = 0;
            for (int i = 0; i < n; ++i) {
                singleCost += offer[i] * price[i];
            }
            if (offer[n] < singleCost) {
                validOffers.push_back(offer);
            }
        }

        unordered_map<int, int> memo;
        return dfs(price, validOffers, needs, n, memo);
    }

private:
    // 将 needs 编码为整数
    int encode(vector<int>& needs, int n) {
        int code = 0;
        int base = 1;
        for (int i = 0; i < n; ++i) {
            code += needs[i] * base;
            base *= 11; // 每个维度范围 [0, 10]
        }
        return code;
    }

    int dfs(vector<int>& price, vector<vector<int>>& offers,
            vector<int>& needs, int n, unordered_map<int, int>& memo) {
        int key = encode(needs, n);
        auto it = memo.find(key);
        if (it != memo.end()) return it->second;

        // 基线：原价购买
        int minCost = 0;
        for (int i = 0; i < n; ++i) {
            minCost += needs[i] * price[i];
        }

        // 尝试每个大礼包
        for (auto& offer : offers) {
            vector<int> newNeeds(n);
            bool valid = true;
            for (int i = 0; i < n; ++i) {
                newNeeds[i] = needs[i] - offer[i];
                if (newNeeds[i] < 0) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                minCost = min(minCost, offer[n] + dfs(price, offers, newNeeds, n, memo));
            }
        }

        memo[key] = minCost;
        return minCost;
    }
};


// ============================================================
// 解法三：迭代式自底向上 DP（思路框架）
// ============================================================
// 思路：
//   枚举所有可能的 needs 状态（从全0到目标needs）
//   对每个状态计算最小花费
//   状态转移与解法一相同
//
//   实现时需要枚举所有 n 维组合，代码较冗长
//   对于 n <= 6, needs[i] <= 10 可行，但不如记忆化搜索直观
//
// 由于实现复杂度高且无明显优势，这里只给出思路框架：
//
// for each state s from (0,0,...,0) to needs:
//     dp[s] = sum(s[i] * price[i])  // 原价购买
//     for each offer:
//         prev = s - offer
//         if prev 合法:
//             dp[s] = min(dp[s], dp[prev] + offer_price)
//
// 关键难点在于如何高效枚举多维状态


// ============================================================
// 各解法对比
// ============================================================
// 解法一（推荐）: 代码简洁清晰，map 开销可接受
// 解法二: 编码优化，常数因子更小
// 解法三: 自底向上避免递归栈，但代码复杂
//
// 实际面试中推荐解法一，清晰展示 DFS + 记忆化思路
// 状态空间最大 11^6 ~ 177 万，远小于超时阈值
