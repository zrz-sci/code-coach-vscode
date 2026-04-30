// LeetCode 736. Parse Lisp Expression
// 解法一：递归下降解析器 + scope 按值传递
// Time: O(n^2), Space: O(n^2)

class Solution {
public:
    int evaluate(string expression) {
        int pos = 0;
        unordered_map<string, int> scope;
        return eval(expression, pos, scope);
    }

private:
    // 递归求值核心函数
    // scope 按值传递，保证 let 的作用域隔离
    int eval(const string& s, int& pos, unordered_map<string, int> scope) {
        if (s[pos] == '(') {
            pos++; // 跳过 '('
            string op = readToken(s, pos);
            int result;

            if (op == "add") {
                pos++; // 跳过空格
                int a = eval(s, pos, scope);
                pos++; // 跳过空格
                int b = eval(s, pos, scope);
                result = a + b;
            } else if (op == "mult") {
                pos++; // 跳过空格
                int a = eval(s, pos, scope);
                pos++; // 跳过空格
                int b = eval(s, pos, scope);
                result = a * b;
            } else {
                // let 表达式: (let v1 e1 v2 e2 ... expr)
                // 循环读取赋值对，直到遇到最终返回表达式
                while (true) {
                    pos++; // 跳过空格
                    // 如果当前字符是 '('、数字或 '-'，说明是表达式而非变量名
                    if (s[pos] == '(' || s[pos] == '-' || isdigit(s[pos])) {
                        result = eval(s, pos, scope);
                        if (s[pos] == ')') break;
                        // 理论上不会到这里，因为表达式后只有 ')' 或空格
                    }
                    // 读取变量名
                    string varName = readToken(s, pos);
                    // 如果变量名后紧跟 ')'，说明这是最终返回值（变量引用）
                    if (s[pos] == ')') {
                        result = scope[varName];
                        break;
                    }
                    // 否则继续读取该变量对应的值表达式
                    pos++; // 跳过空格
                    int val = eval(s, pos, scope);
                    scope[varName] = val;
                }
            }

            pos++; // 跳过 ')'
            return result;
        } else if (s[pos] == '-' || isdigit(s[pos])) {
            // 整数字面量
            return readInt(s, pos);
        } else {
            // 变量引用
            string varName = readToken(s, pos);
            return scope[varName];
        }
    }

    // 读取一个 token（变量名或操作符），遇到空格或 ')' 停止
    string readToken(const string& s, int& pos) {
        int start = pos;
        while (pos < (int)s.size() && s[pos] != ' ' && s[pos] != ')') {
            pos++;
        }
        return s.substr(start, pos - start);
    }

    // 读取一个整数（可能带负号）
    int readInt(const string& s, int& pos) {
        int start = pos;
        if (s[pos] == '-') pos++;
        while (pos < (int)s.size() && isdigit(s[pos])) {
            pos++;
        }
        return stoi(s.substr(start, pos - start));
    }
};


// 解法二：栈式作用域优化 - 避免 scope 拷贝
// Time: O(n), Space: O(n)
// 核心思想：为每个变量维护值栈，进出 let 时 push/pop

class Solution2 {
public:
    int evaluate(string expression) {
        int pos = 0;
        return eval(expression, pos);
    }

private:
    // 变量名 -> 值栈（最后一个元素是当前作用域的值）
    unordered_map<string, vector<int>> varStack;

    int eval(const string& s, int& pos) {
        if (s[pos] == '(') {
            pos++; // 跳过 '('
            string op = readToken(s, pos);
            int result;

            if (op == "add") {
                pos++;
                int a = eval(s, pos);
                pos++;
                int b = eval(s, pos);
                result = a + b;
            } else if (op == "mult") {
                pos++;
                int a = eval(s, pos);
                pos++;
                int b = eval(s, pos);
                result = a * b;
            } else {
                // let 表达式 - 记录本层绑定的变量以便退出时回退
                vector<string> boundVars;

                while (true) {
                    pos++;
                    if (s[pos] == '(' || s[pos] == '-' || isdigit(s[pos])) {
                        result = eval(s, pos);
                        if (s[pos] == ')') break;
                    }

                    string varName = readToken(s, pos);
                    if (s[pos] == ')') {
                        // 最终返回值：从栈顶取变量值
                        result = varStack[varName].back();
                        break;
                    }

                    pos++;
                    int val = eval(s, pos);
                    varStack[varName].push_back(val);
                    boundVars.push_back(varName);
                }

                // 退出 let 时回退所有本层绑定
                for (auto& v : boundVars) {
                    varStack[v].pop_back();
                    if (varStack[v].empty()) {
                        varStack.erase(v);
                    }
                }
            }

            pos++; // 跳过 ')'
            return result;
        } else if (s[pos] == '-' || isdigit(s[pos])) {
            return readInt(s, pos);
        } else {
            string varName = readToken(s, pos);
            return varStack[varName].back();
        }
    }

    string readToken(const string& s, int& pos) {
        int start = pos;
        while (pos < (int)s.size() && s[pos] != ' ' && s[pos] != ')') {
            pos++;
        }
        return s.substr(start, pos - start);
    }

    int readInt(const string& s, int& pos) {
        int start = pos;
        if (s[pos] == '-') pos++;
        while (pos < (int)s.size() && isdigit(s[pos])) {
            pos++;
        }
        return stoi(s.substr(start, pos - start));
    }
};
