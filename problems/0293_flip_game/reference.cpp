/*
 * 293. Flip Game
 *
 * 【题目本质】
 * 扫描字符串找所有 "++" 位置，翻转为 "--" 生成所有可能状态。
 * 简单模拟题，是 Flip Game II (294) 的基础组件。
 *
 * 【解法总览】
 * Solution1: 线性扫描 + 拷贝构造  O(n^2) / O(n)  ⭐推荐
 * Solution2: string::find 查找      O(n^2) / O(n)
 * Solution3: 原地修改 + 还原        O(n^2) / O(n)  省一次拷贝
 */

// ===================== Solution1: 线性扫描 =====================
// 思路：逐位检查 s[i] 和 s[i+1] 是否都是 '+'，是则翻转并记录。
class Solution1 {
public:
    vector<string> generatePossibleNextMoves(string currentState) {
        vector<string> result;
        for (int i = 0; i + 1 < (int)currentState.size(); i++) {
            if (currentState[i] == '+' && currentState[i + 1] == '+') {
                string next = currentState;
                next[i] = '-';
                next[i + 1] = '-';
                result.push_back(next);
            }
        }
        return result;
    }
};

// ===================== Solution2: string::find =====================
// 思路：用 find("++") 定位连续++，比手动遍历更简洁。
class Solution2 {
public:
    vector<string> generatePossibleNextMoves(string currentState) {
        vector<string> result;
        size_t pos = 0;
        while ((pos = currentState.find("++", pos)) != string::npos) {
            string next = currentState;
            next[pos] = '-';
            next[pos + 1] = '-';
            result.push_back(next);
            pos++;
        }
        return result;
    }
};

// ===================== Solution3: 原地修改 + 还原 =====================
// 思路：直接在原字符串上修改，push 后还原，减少一次拷贝开销。
class Solution3 {
public:
    vector<string> generatePossibleNextMoves(string currentState) {
        vector<string> result;
        for (int i = 0; i + 1 < (int)currentState.size(); i++) {
            if (currentState[i] == '+' && currentState[i + 1] == '+') {
                currentState[i] = currentState[i + 1] = '-';
                result.push_back(currentState);
                currentState[i] = currentState[i + 1] = '+';
            }
        }
        return result;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | Solution1 线性扫描 | O(n^2) | O(n) | 最直观清晰 |
 * | Solution2 find | O(n^2) | O(n) | 利用 STL 更简洁 |
 * | Solution3 原地修改 | O(n^2) | O(n) | 略快，少一次中间拷贝 |
 *
 * 【易错点】
 * 1. unsigned 溢出：size()-1 对空串会溢出，用 i+1 < (int)size()
 * 2. 原地修改后忘记还原：导致后续位置判断错误
 * 3. 循环步长：找到 "++" 后 pos 只加1不加2，因为 "+++" 有两个合法位置
 *
 * 【面试追问】
 * Q1: 与 294 的关系？-> 294 用本题的结果做博弈搜索
 * Q2: 时间O(n^2)能否优化？-> 返回字符串必须O(n)拷贝，总体不可避免
 * Q3: 如果 "++" 可翻转为任意字符？-> 增加参数化即可
 */
