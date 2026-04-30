// LeetCode 592: Fraction Addition and Subtraction
// -----------------------------------------------
// 解法一：istringstream 流解析 (推荐)
// 思路：利用 C++ 输入流自动处理正负号，逐个读取 num/den 格式
//       累加器初始化为 0/1，每次通分后用 GCD 化简
// 时间 O(n)  空间 O(1)

class Solution {
public:
    string fractionAddition(string expression) {
        int numer = 0, denom = 1;
        istringstream ss(expression);
        int n, d;
        char slash;

        // >> int 自动处理 +/- 前缀
        while (ss >> n >> slash >> d) {
            // 通分: numer/denom + n/d = (numer*d + n*denom) / (denom*d)
            numer = numer * d + n * denom;
            denom = denom * d;

            // 化简——GCD 要求非负
            int g = abs(__gcd(numer, denom));
            numer /= g;
            denom /= g;
        }

        return to_string(numer) + "/" + to_string(denom);
    }
};


// -----------------------------------------------
// 解法二：手动索引扫描
// 思路：逐字符扫描，遇 +/- 确定符号，找 / 分割分子分母
//       适合面试时不确定流行为时使用
// 时间 O(n)  空间 O(1)

class Solution2 {
public:
    string fractionAddition(string expression) {
        int numer = 0, denom = 1;
        int i = 0, len = expression.size();

        while (i < len) {
            // 1) 确定符号
            int sign = 1;
            if (expression[i] == '+' || expression[i] == '-') {
                sign = (expression[i] == '-') ? -1 : 1;
                i++;
            }

            // 2) 提取分子
            int j = i;
            while (j < len && expression[j] != '/') j++;
            int n = sign * stoi(expression.substr(i, j - i));
            i = j + 1; // 跳过 '/'

            // 3) 提取分母
            j = i;
            while (j < len && expression[j] != '+' && expression[j] != '-') j++;
            int d = stoi(expression.substr(i, j - i));
            i = j;

            // 4) 通分 + 化简
            numer = numer * d + n * denom;
            denom = denom * d;
            int g = abs(__gcd(numer, denom));
            numer /= g;
            denom /= g;
        }

        return to_string(numer) + "/" + to_string(denom);
    }
};
