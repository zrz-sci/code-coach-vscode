/*
 * LeetCode 165: 比较版本号 (Compare Version Numbers)
 * 难度: Medium | 标签: Two Pointers, String
 *
 * 题意: 给定两个版本号字符串 version1 和 version2，按修订号逐段比较。
 *       修订号是其整数值（忽略前导零）。缺失修订号视为 0。
 *       返回 -1 / 0 / 1。
 *
 * 核心思路:
 *   双指针同步扫描，按 '.' 分段提取整数比较。
 *   缺失段视为 0。前导零由 v=v*10+digit 自然忽略。
 *
 * 示例:
 *   "1.2" vs "1.10" → 第一段 1==1, 第二段 2<10 → -1
 *   "1.01" vs "1.001" → 第一段 1==1, 第二段 1==1 → 0
 *   "1.0" vs "1.0.0.0" → 所有段(含补0)都相等 → 0
 */


// ============================================================
// 解法1: 双指针逐段解析 ⭐ 推荐
// 时间: O(n+m)  空间: O(1)
//
// i, j 分别扫描 version1, version2
// 每轮提取一个段的数字值: v = v*10 + (c-'0')
// 循环条件: i < n1 || j < n2 (OR 保证两方都处理完)
// 一方越界时 v 保持 0 → 缺失段 = 0
//
// 前导零处理: v=v*10+digit 自然忽略 ("001" → 0→0→1)
// 跳过 '.': 提取完后 i++/j++, 越界也安全
// ============================================================
class Solution1 {
public:
    int compareVersion(string version1, string version2) {
        int i = 0, j = 0;
        int n1 = version1.size(), n2 = version2.size();

        while (i < n1 || j < n2) {
            // 提取 version1 当前段的数字
            int v1 = 0;
            while (i < n1 && version1[i] != '.') {
                v1 = v1 * 10 + (version1[i] - '0');
                i++;
            }
            // 提取 version2 当前段的数字
            int v2 = 0;
            while (j < n2 && version2[j] != '.') {
                v2 = v2 * 10 + (version2[j] - '0');
                j++;
            }
            // 比较当前段
            if (v1 < v2) return -1;
            if (v1 > v2) return 1;
            // 跳过 '.'
            i++;
            j++;
        }
        return 0;  // 所有段都相等
    }
};

// ============================================================
// 解法2: split 分段后比较
// 时间: O(n+m)  空间: O(n+m)
//
// 预处理: 按 '.' 分割成整数数组
// 比较: 逐段比较, 缺失段补 0
// 优点: 代码清晰, 不容易写错
// ============================================================
class Solution2 {
public:
    int compareVersion(string version1, string version2) {
        vector<int> v1 = split(version1);
        vector<int> v2 = split(version2);
        int n = max(v1.size(), v2.size());

        for (int i = 0; i < n; i++) {
            int a = i < (int)v1.size() ? v1[i] : 0;  // 缺失段 → 0
            int b = i < (int)v2.size() ? v2[i] : 0;
            if (a < b) return -1;
            if (a > b) return 1;
        }
        return 0;
    }

private:
    vector<int> split(const string& s) {
        vector<int> res;
        stringstream ss(s);
        string seg;
        while (getline(ss, seg, '.')) {
            res.push_back(stoi(seg));  // stoi 自动忽略前导零
        }
        return res;
    }
};

// ============================================================
// 解法3: istringstream 替换分隔符
// 时间: O(n+m)  空间: O(n+m)
//
// 把 '.' 替换成空格, 用 >> 逐个读 int
// 代码最简洁, 但修改了原字符串
// ============================================================
class Solution3 {
public:
    int compareVersion(string version1, string version2) {
        // '.' → ' ' 后用 istringstream 读取整数
        replace(version1.begin(), version1.end(), '.', ' ');
        replace(version2.begin(), version2.end(), '.', ' ');
        istringstream ss1(version1), ss2(version2);

        while (ss1 || ss2) {
            int v1 = 0, v2 = 0;
            ss1 >> v1;  // 流结束时 v1 保持 0
            ss2 >> v2;
            if (v1 < v2) return -1;
            if (v1 > v2) return 1;
        }
        return 0;
    }
};

/*
 * ============================================================
 * 解法对比
 * ============================================================
 * | 解法         | 时间    | 空间    | 修改原串 | 面试推荐 |
 * |-------------|---------|---------|---------|---------|
 * | 双指针解析   | O(n+m)  | O(1)   | 否      | ⭐首选   |
 * | split 分段   | O(n+m)  | O(n+m) | 否      | 快速实现 |
 * | 流替换       | O(n+m)  | O(n+m) | 是      | 了解即可 |
 *
 * ============================================================
 * 易错点
 * ============================================================
 * 1. "1.0" != "1.0.0.0":
 *    错误: while(i<n1 && j<n2) 用 AND → 短版本结束就退出
 *    正确: while(i<n1 || j<n2) 用 OR → 长版本的剩余段与 0 比较
 *
 * 2. 把版本号当浮点数:
 *    stof("1.2")=1.2, stof("1.10")=1.1 → 1.2>1.1 → return 1
 *    但正确答案是 -1! 第二段是 2 vs 10, 不是小数。
 *
 * 3. 整数溢出:
 *    不能把整个版本号转成一个大整数。
 *    逐段比较, 题目保证每段在 32-bit int 范围内。
 *
 * 4. 前导零特殊处理:
 *    不需要! v=v*10+digit 自然处理。"001" → 1。
 *
 * 5. 跳过 '.' 时没递增指针:
 *    提取完数字后忘记 i++, 下次循环读到 '.'
 *    → v = '.' - '0' = 负数, 结果错误!
 *
 * ============================================================
 * 面试追问
 * ============================================================
 * Q1: 有字母后缀 (如 "1.2.3-beta")?
 *     A: 分离数字和字母。数字按整数比较, 字母按字典序。
 *        参考 Semantic Versioning (semver) 规范。
 *
 * Q2: 支持 semver 完整比较?
 *     A: MAJOR.MINOR.PATCH-prerelease+build
 *        数字段整数比较; pre-release 逐字段比较(数字>字母);
 *        有 pre-release < 无 pre-release; build 不参与比较。
 *
 * Q3: 段数非常多 (100+)?
 *     A: 无性能问题。双指针 O(n+m) 一遍扫描。
 *
 * Q4: 不用乘法提取数字?
 *     A: stoi(s.substr(start, len)) 但需先找 '.' 位置。
 *
 * Q5: 排序多个版本号?
 *     A: 自定义 comparator 调用 compareVersion。
 *        或 split 成 vector<int> + lexicographical_compare。
 */
