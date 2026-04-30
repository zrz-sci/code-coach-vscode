// 【题目本质】
// 将密码修改为满足长度[6,20]、包含大小写字母和数字、无三连重复的强密码，求最少操作次数。
// 核心：分三种长度区间讨论，利用删除操作按 len%3 优先级优化替换次数。

// 【解法总览】
// Solution1: 贪心分类讨论 O(n) / O(1) ⭐推荐
//   - n<6: max(missing, 6-n)
//   - 6<=n<=20: max(missing, replace)
//   - n>20: 删除del=n-20, 按len%3优先级减少replace, del + max(missing, replace)

// ===================== Solution1: 贪心分类讨论 =====================
// 思路：
// 1. 统计缺失字符类型数 missing
// 2. 扫描连续重复段，计算需要的替换次数 replace = sum(len/3)
// 3. 按长度分三种情况处理
class Solution1 {
public:
    int strongPasswordChecker(string password) {
        int n = password.size();
        // 统计缺失类型
        bool hasLower = false, hasUpper = false, hasDigit = false;
        for (char c : password) {
            if (islower(c)) hasLower = true;
            else if (isupper(c)) hasUpper = true;
            else if (isdigit(c)) hasDigit = true;
        }
        int missing = (!hasLower) + (!hasUpper) + (!hasDigit);

        // 统计连续重复段
        int replace = 0;
        int one = 0, two = 0; // len%3==0 的段数, len%3==1 的段数
        int i = 2;
        while (i < n) {
            if (password[i] == password[i-1] && password[i] == password[i-2]) {
                int len = 2;
                while (i < n && password[i] == password[i-1]) {
                    len++;
                    i++;
                }
                replace += len / 3;
                if (len % 3 == 0) one++;
                else if (len % 3 == 1) two++;
            } else {
                i++;
            }
        }

        // 情况1: 太短
        if (n < 6) return max(missing, 6 - n);

        // 情况2: 长度合适，只需替换
        if (n <= 20) return max(missing, replace);

        // 情况3: 太长，需要删除
        int del = n - 20;
        // 贪心：优先用删除减少替换次数
        // 删1个字符可以将 len%3==0 的段减少1次替换
        replace -= min(del, one);
        // 删2个字符可以将 len%3==1 的段减少1次替换
        replace -= min(max(del - one, 0), two * 2) / 2;
        // 删3个字符可以减少任意段1次替换
        replace -= max(del - one - two * 2, 0) / 3;
        replace = max(replace, 0);

        return del + max(missing, replace);
    }
};

// 【解法对比】
// 本题只有一种最优解法（贪心分类讨论），其他方法（如暴力/DP）不实用。
// 关键洞察：三种操作（插入/删除/替换）各有最适用的场景，分长度区间处理最清晰。

// 【易错点】
// 1. replace 被减为负数时忘记 clamp 到 0
// 2. 删除操作的优先级搞错：应先处理 len%3==0（性价比最高），再处理 len%3==1
// 3. 忘记 missing 也需要与操作次数取 max（插入/替换可同时修复缺失类型）
// 4. 连续段扫描时 i 的移动逻辑：匹配时 i 已经在 while 内递增，不匹配才 i++

// 【面试追问】
// Q1: 删除操作为什么按 len%3 分优先级？
// A1: len%3==0 的段（如长6），删1个变5，替换从2变1，"1删换1替"；
//     len%3==1 的段（如长7），删2个变5，替换从2变1，"2删换1替"；
//     其他段需要"3删换1替"。优先做性价比高的。
//
// Q2: 如果允许任意字符插入，长度<6时为什么不需要考虑连续重复？
// A2: 插入操作可以插到连续段中间打断三连，同时增长度、补类型、断重复，一举三得。
//     所以 max(missing, 6-n) 就够了。
//
// Q3: 此题时间复杂度的下界？
// A3: 必须扫描整个字符串一次来统计信息，所以下界是 O(n)，本解法已达最优。
