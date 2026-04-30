// 690. Employee Importance
// 难度: Medium
// 标签: Tree, DFS, BFS, Hash Table

/*
 * 题目描述：
 * 给定员工信息数组（每个员工包含 id、importance、subordinates 列表），
 * 求指定 id 员工及其所有直接/间接下属的 importance 总和。
 *
 * 核心思路：
 * 先用哈希表建立 id -> Employee 映射，然后从目标 id 出发做 DFS/BFS 遍历子树累加。
 *
 * 假设 Employee 类已定义:
 * class Employee {
 * public:
 *     int id;
 *     int importance;
 *     vector<int> subordinates;
 * };
 */

// ==================== 解法一：DFS + HashMap（推荐）====================
// 时间 O(N)，空间 O(N)
// 思路：建立 id->Employee 映射后，递归遍历子树累加 importance
class Solution {
public:
    int getImportance(vector<Employee*> employees, int id) {
        unordered_map<int, Employee*> mp;
        for (auto e : employees) {
            mp[e->id] = e;
        }
        return dfs(mp, id);
    }

private:
    int dfs(unordered_map<int, Employee*>& mp, int id) {
        Employee* emp = mp[id];
        int total = emp->importance;
        for (int subId : emp->subordinates) {
            total += dfs(mp, subId);
        }
        return total;
    }
};

// ==================== 解法二：BFS + HashMap ====================
// 时间 O(N)，空间 O(N)
// 思路：用队列逐层展开所有下属，迭代方式避免递归栈溢出
class Solution_BFS {
public:
    int getImportance(vector<Employee*> employees, int id) {
        // 建立 id -> Employee* 的映射
        unordered_map<int, Employee*> mp;
        for (auto e : employees) {
            mp[e->id] = e;
        }

        int total = 0;
        queue<int> q;
        q.push(id);

        while (!q.empty()) {
            int curId = q.front();
            q.pop();

            Employee* emp = mp[curId];
            total += emp->importance;

            // 将所有直接下属的 id 入队
            for (int subId : emp->subordinates) {
                q.push(subId);
            }
        }

        return total;
    }
};

// ==================== 解法三：DFS Lambda 写法 ====================
// 时间 O(N)，空间 O(N)
// 思路：用 lambda 简化递归，适合面试快速书写
class Solution_Lambda {
public:
    int getImportance(vector<Employee*> employees, int id) {
        unordered_map<int, Employee*> mp;
        for (auto e : employees) {
            mp[e->id] = e;
        }

        // 使用 function<int(int)> 定义递归 lambda
        function<int(int)> dfs = [&](int curId) -> int {
            Employee* emp = mp[curId];
            int sum = emp->importance;
            for (int subId : emp->subordinates) {
                sum += dfs(subId);
            }
            return sum;
        };

        return dfs(id);
    }
};

/*
 * 复杂度分析：
 * - 三种解法时间复杂度均为 O(N)，N 为员工总数
 * - 空间复杂度 O(N)：哈希表存储 + 递归栈/队列
 *
 * 关键要点：
 * 1. 必须先建哈希表将 id 查找降为 O(1)，否则每次遍历数组导致 O(N^2)
 * 2. subordinates 存储的是 id（int），不是指针，需要通过映射表转换
 * 3. importance 可以为负值，不能做提前剪枝
 * 4. 树形结构保证无环，不需要 visited 集合
 * 5. DFS 和 BFS 性能相当，DFS 代码更简洁，BFS 在极深树中更安全
 */
