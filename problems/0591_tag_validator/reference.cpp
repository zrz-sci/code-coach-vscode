// 591. Tag Validator
// 栈 + 逐字符解析  时间 O(n)  空间 O(n)

class Solution {
public:
    bool isValid(string code) {
        stack<string> tagStack;
        int i = 0, n = code.size();

        while (i < n) {
            if (i > 0 && tagStack.empty()) {
                // 外层标签已关闭但字符串还没结束
                return false;
            }

            if (code.substr(i, 9) == "<![CDATA[") {
                // --- CDATA 块 ---
                if (tagStack.empty()) return false;  // CDATA 不能在标签外
                int j = code.find("]]>", i + 9);
                if (j == string::npos) return false;
                i = j + 3;  // 跳过 "]]>"
            }
            else if (code.substr(i, 2) == "</") {
                // --- 结束标签 ---
                int j = code.find('>', i + 2);
                if (j == string::npos) return false;
                string tagName = code.substr(i + 2, j - i - 2);
                if (!isValidTagName(tagName)) return false;
                if (tagStack.empty() || tagStack.top() != tagName) return false;
                tagStack.pop();
                i = j + 1;
            }
            else if (code[i] == '<') {
                // --- 开始标签 ---
                int j = code.find('>', i + 1);
                if (j == string::npos) return false;
                string tagName = code.substr(i + 1, j - i - 1);
                if (!isValidTagName(tagName)) return false;
                tagStack.push(tagName);
                i = j + 1;
            }
            else {
                // --- 普通字符 ---
                if (tagStack.empty()) return false;  // 字符在标签外
                ++i;
            }
        }

        return tagStack.empty();
    }

private:
    bool isValidTagName(const string& name) {
        if (name.empty() || name.size() > 9) return false;
        for (char c : name) {
            if (c < 'A' || c > 'Z') return false;
        }
        return true;
    }
};

// ============================================================
// 关键测试用例说明:
//
// "<DIV>This is the first line <![CDATA[<div>]]></DIV>"
//   -> true: 合法闭合标签, CDATA 内容不解析
//
// "<DIV>>>  ![cdata[]] <![CDATA[<div>]>]]>]]>>]</DIV>"
//   -> true: '>>' 是普通字符, CDATA 正确终止于第一个 "]]>"
//
// "<A>  <B> </A>   </B>"
//   -> false: 交叉嵌套, B 在 A 关闭前未关闭
//
// "<A></A><B></B>"
//   -> false: A 关闭后还有 <B>, 不是被单个外层标签包裹
//
// "<DIV>  div tag is not valid  </DIV1>"
//   -> false: 结束标签名 DIV1 与开始标签名 DIV 不匹配
//
// "<DIV>  unmatched <  </DIV>"
//   -> false: 单独的 '<' 后面不是合法的标签/CDATA
//
// "<A><![CDATA[</A>]]></A>"
//   -> true: CDATA 中的 </A> 不被解析为结束标签
// ============================================================
