// 770. Basic Calculator IV
// 难度: Hard
// 解法: 递归下降解析 + 多项式运算
// 时间复杂度: O(2^n * n), n 为自由变量数
// 空间复杂度: O(2^n * n)

// 多项式类型: map<vector<string>, int>
// key = 排序后的变量名列表, value = 系数
// 常数项: key 为空 vector

class Solution {
public:
    // 多项式类型定义
    // 每一项: (排序后的变量名列表) -> 系数
    // 例如: 3*a*b 表示为 {["a","b"] -> 3}
    // 常数 5 表示为 {[] -> 5}
    typedef map<vector<string>, int> Poly;

    vector<string> tokens;
    int pos;

    // 创建常数多项式: val -> {[] -> val}
    Poly makeConst(int val) {
        Poly p;
        if (val != 0) p[{}] = val;
        return p;
    }

    // 创建变量多项式: var -> {[var] -> 1}
    Poly makeVar(const string& var) {
        Poly p;
        p[{var}] = 1;
        return p;
    }

    // 多项式加法/减法 (sign=1 加, sign=-1 减)
    // 合并两个 map, 相同 key 的系数做加减
    Poly add(const Poly& a, const Poly& b, int sign = 1) {
        Poly result = a;
        for (auto& [vars, coeff] : b) {
            result[vars] += sign * coeff;
            // 及时清理零系数项, 避免无效数据累积
            if (result[vars] == 0) result.erase(vars);
        }
        return result;
    }

    // 多项式乘法
    // 双重循环: 每对项配对, 变量列表合并排序, 系数相乘
    Poly mul(const Poly& a, const Poly& b) {
        Poly result;
        for (auto& [va, ca] : a) {
            for (auto& [vb, cb] : b) {
                // 合并两个排序后的变量列表 (用 merge 保持有序)
                vector<string> vars;
                merge(va.begin(), va.end(),
                      vb.begin(), vb.end(),
                      back_inserter(vars));
                result[vars] += ca * cb;
                if (result[vars] == 0) result.erase(vars);
            }
        }
        return result;
    }

    // ========== 递归下降解析器 ==========

    // parseExpr: 处理加减法 (最低优先级)
    // expr = term (('+' | '-') term)*
    Poly parseExpr() {
        Poly result = parseTerm();
        while (pos < (int)tokens.size() &&
               (tokens[pos] == "+" || tokens[pos] == "-")) {
            string op = tokens[pos++];
            Poly right = parseTerm();
            result = add(result, right, op == "+" ? 1 : -1);
        }
        return result;
    }

    // parseTerm: 处理乘法 (较高优先级)
    // term = factor ('*' factor)*
    Poly parseTerm() {
        Poly result = parseFactor();
        while (pos < (int)tokens.size() && tokens[pos] == "*") {
            pos++; // 跳过 '*'
            Poly right = parseFactor();
            result = mul(result, right);
        }
        return result;
    }

    // parseFactor: 处理括号和基本元素 (最高优先级)
    // factor = '(' expr ')' | number | variable
    Poly parseFactor() {
        if (tokens[pos] == "(") {
            pos++; // 跳过 '('
            Poly result = parseExpr();
            pos++; // 跳过 ')'
            return result;
        }

        string& tok = tokens[pos++];
        // 判断是数字还是变量
        if (isdigit(tok[0])) {
            return makeConst(stoi(tok));
        }
        return makeVar(tok);
    }

    // ========== 主函数 ==========

    vector<string> basicCalculatorIV(string expression,
                                      vector<string>& evalvars,
                                      vector<int>& evalints) {
        // 步骤1: 建立变量替换映射
        unordered_map<string, int> evalMap;
        for (int i = 0; i < (int)evalvars.size(); i++) {
            evalMap[evalvars[i]] = evalints[i];
        }

        // 步骤2: Tokenize 表达式
        // 按空格分割, 括号作为独立 token
        tokens.clear();
        pos = 0;
        string token;
        for (char c : expression) {
            if (c == ' ') {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
            } else if (c == '(' || c == ')') {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
                tokens.push_back(string(1, c));
            } else {
                token += c;
            }
        }
        if (!token.empty()) tokens.push_back(token);

        // 步骤3: 变量替换 - 将已知变量替换为对应数值
        for (auto& t : tokens) {
            if (evalMap.count(t)) {
                t = to_string(evalMap[t]);
            }
        }

        // 步骤4: 递归下降解析
        Poly poly = parseExpr();

        // 步骤5: 排序输出
        // 排序规则: degree 降序, 同 degree 按变量名字典序升序
        vector<pair<vector<string>, int>> terms(poly.begin(), poly.end());
        sort(terms.begin(), terms.end(), [](const auto& a, const auto& b) {
            if (a.first.size() != b.first.size())
                return a.first.size() > b.first.size();
            return a.first < b.first;
        });

        // 步骤6: 格式化为字符串
        vector<string> result;
        for (auto& [vars, coeff] : terms) {
            if (coeff == 0) continue;
            string s = to_string(coeff);
            for (auto& v : vars) s += "*" + v;
            result.push_back(s);
        }
        return result;
    }
};

// ============================================================
// 要点总结:
// 1. 核心数据结构: map<vector<string>, int> 表示多项式
// 2. 三个运算: add(合并key相同项), mul(配对+合并变量列表)
// 3. 递归下降解析: parseExpr -> parseTerm -> parseFactor
// 4. 变量替换在 tokenize 后、解析前完成
// 5. merge() 合并已排序的变量列表, 保持排序不变性
// 6. 输出: degree降序 -> 字典序升序 -> 过滤零系数
// ============================================================
