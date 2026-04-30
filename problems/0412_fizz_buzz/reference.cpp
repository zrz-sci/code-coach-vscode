// 【题目本质】
// 根据整除关系将数字映射为字符串，考察条件判断和可扩展性设计

// 【解法总览】
// Solution1: 直接条件判断 — O(n) / O(1) — 面试首选
// Solution2: 字符串拼接法 — O(n) / O(1) — 可扩展性强

// ===================== Solution 1: 直接条件判断 =====================
class Solution1 {
public:
    vector<string> fizzBuzz(int n) {
        vector<string> res;
        for (int i = 1; i <= n; i++) {
            if (i % 15 == 0)      res.push_back("FizzBuzz");
            else if (i % 3 == 0)  res.push_back("Fizz");
            else if (i % 5 == 0)  res.push_back("Buzz");
            else                  res.push_back(to_string(i));
        }
        return res;
    }
};

// ===================== Solution 2: 字符串拼接（可扩展） =====================
class Solution2 {
public:
    vector<string> fizzBuzz(int n) {
        vector<string> res;
        for (int i = 1; i <= n; i++) {
            string s;
            if (i % 3 == 0) s += "Fizz";
            if (i % 5 == 0) s += "Buzz";
            // 若新增规则如 7->"Jazz"，只需加一行:
            // if (i % 7 == 0) s += "Jazz";
            if (s.empty()) s = to_string(i);
            res.push_back(s);
        }
        return res;
    }
};

// 【解法对比】
// Solution1: 简洁直观，适合无 follow-up 场景
// Solution2: 可扩展，新增规则只需加一行 if，面试中展示设计思维

// 【易错点】
// 1. 遍历从 1 开始（1-indexed），不是 0
// 2. 直接判断法必须先判 15（或 3&&5），否则被 3/5 提前截获
// 3. to_string(i) 别忘了，不能直接 push_back(i)

// 【面试追问】
// Q1: 如何扩展到任意规则? → 映射表 + 字符串拼接
// Q2: 能否避免取模? → 计数器法，fizz_count/buzz_count
// Q3: 大规模并行? → 每个 i 独立，可分段并行填充
