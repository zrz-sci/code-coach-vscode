/*
 * LeetCode 273: Integer to English Words (整数转换英文表示)
 *
 * 【题目本质】
 * 利用英语数字按千位分段的规则，递归/迭代地将每三位数字转换为英文，
 * 再加上对应的量级后缀（Thousand / Million / Billion）拼接成完整结果。
 *
 * 【解法总览】
 * 解法1: 迭代分段 — O(1) / O(1) — 从低位到高位每次取三位
 * 解法2: 递归分段 — O(1) / O(1) — 从高位到低位递归处理
 *
 * 数字分段示意图:
 *   2,147,483,647  (INT_MAX)
 *   [2] Billion  [147] Million  [483] Thousand  [647]
 *    ↓              ↓               ↓              ↓
 *   "Two"      "One Hundred    "Four Hundred   "Six Hundred
 *               Forty Seven"   Eighty Three"   Forty Seven"
 *
 * 三位数内部处理:
 *   num = 547
 *   百位: 5 → "Five Hundred"
 *   余数: 47 → 47 >= 20, 十位: 4 → "Forty", 个位: 7 → "Seven"
 *   结果: "Five Hundred Forty Seven"
 *
 *   num = 15
 *   百位: 无
 *   余数: 15 → 15 < 20, 直接查表 → "Fifteen"（不能拆成 "Ten Five"！）
 */

// ============================================================
// 解法1: 迭代分段 — 从低位到高位，每次处理三位
// 时间: O(1)  空间: O(1)  （数字最多约10位，分段数固定）
//
// 【思路】
// 英语数字按千位分组。我们从低位开始，每次取 num % 1000，
// 用辅助函数转成英文，然后加上当前段的后缀。
// 因为从低位开始，拼接时新段放到结果前面。
//
// 处理流程（以 1234567 为例）:
//   Round 1: chunk = 567, suffix = ""        → "Five Hundred Sixty Seven"
//   Round 2: chunk = 234, suffix = "Thousand" → "Two Hundred Thirty Four Thousand"
//   Round 3: chunk = 1,   suffix = "Million"  → "One Million"
//   拼接: "One Million Two Hundred Thirty Four Thousand Five Hundred Sixty Seven"
// ============================================================
class Solution1 {
public:
    string numberToWords(int num) {
        // num=0 是唯一输出 "Zero" 的场景
        // 递归/辅助函数中遇到 0 是跳过不输出的
        if (num == 0) return "Zero";
        
        string result;
        int i = 0; // 第几个千位段（0=个位段，1=千位段，2=百万段，3=十亿段）
        
        while (num > 0) {
            int chunk = num % 1000;
            if (chunk != 0) {
                // 只有非零段才输出（避免 1000000 输出多余的 "Thousand"）
                string part = threeDigits(chunk);
                if (!thousands[i].empty()) {
                    part += " " + thousands[i];
                }
                // 新段拼到前面（因为我们从低位开始处理）
                if (result.empty()) {
                    result = part;
                } else {
                    result = part + " " + result;
                }
            }
            num /= 1000;
            i++;
        }
        
        return result;
    }
    
private:
    // 1-19 的英文表示（下标0留空，因为0不输出任何东西）
    vector<string> below20 = {
        "", "One", "Two", "Three", "Four", "Five", "Six", "Seven",
        "Eight", "Nine", "Ten", "Eleven", "Twelve", "Thirteen",
        "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"
    };
    
    // 整十位的英文（下标0和1不用）
    vector<string> tens = {
        "", "", "Twenty", "Thirty", "Forty", "Fifty",
        "Sixty", "Seventy", "Eighty", "Ninety"
    };
    
    // 千位后缀，按段序号索引
    vector<string> thousands = {"", "Thousand", "Million", "Billion"};
    
    // 将 1-999 的数字转为英文字符串
    // 保证传入的 num 在 [1, 999] 范围内
    string threeDigits(int num) {
        string result;
        
        // 处理百位
        if (num >= 100) {
            result = below20[num / 100] + " Hundred";
            num %= 100;
        }
        
        // 处理十位和个位
        if (num >= 20) {
            // 20-99: 十位查表 + 可能的个位
            if (!result.empty()) result += " ";
            result += tens[num / 10];
            if (num % 10 != 0) {
                result += " " + below20[num % 10];
            }
        } else if (num > 0) {
            // 1-19: 整体查表（关键！10-19不能拆分）
            if (!result.empty()) result += " ";
            result += below20[num];
        }
        // num == 0 时什么都不加，因为百位已经处理完了
        
        return result;
    }
};

// ============================================================
// 解法2: 递归分段 — 从高位到低位，递归处理每个量级
// 时间: O(1)  空间: O(1)
//
// 【思路】
// 从最高量级（Billion）开始判断：
// - 如果 num >= 1000000000，先递归处理 num/10^9 得到 Billion 前的数，
//   加上 "Billion"，再递归处理余数。
// - 依次类推 Million、Thousand、Hundred。
// - 到 20-99 用十位表+个位，1-19 直接查表。
//
// 这种写法的优势：天然从高位到低位，不需要反转拼接。
//
// 递归展开示意（num = 1234567）:
//   helper(1234567)
//     → helper(1) + " Million" + " " + helper(234567)
//       → "One Million" + " " + helper(234567)
//         → helper(234) + " Thousand" + " " + helper(567)
//           → helper(2) + " Hundred" + " " + helper(34) + " Thousand"
//             → "Two Hundred" + " " + "Thirty Four" + " Thousand"
//           + " " + helper(567)
//             → "Five Hundred" + " " + "Sixty Seven"
//   最终: "One Million Two Hundred Thirty Four Thousand Five Hundred Sixty Seven"
// ============================================================
class Solution2 {
public:
    string numberToWords(int num) {
        if (num == 0) return "Zero";
        return helper(num);
    }
    
private:
    vector<string> below20 = {
        "", "One", "Two", "Three", "Four", "Five", "Six", "Seven",
        "Eight", "Nine", "Ten", "Eleven", "Twelve", "Thirteen",
        "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"
    };
    
    vector<string> tens = {
        "", "", "Twenty", "Thirty", "Forty", "Fifty",
        "Sixty", "Seventy", "Eighty", "Ninety"
    };
    
    string helper(int num) {
        if (num == 0) return "";
        
        string result;
        
        if (num >= 1000000000) {
            // 十亿级：递归处理商，加 "Billion"，再递归处理余数
            result = helper(num / 1000000000) + " Billion";
            int rem = num % 1000000000;
            if (rem > 0) result += " " + helper(rem);
        } else if (num >= 1000000) {
            // 百万级
            result = helper(num / 1000000) + " Million";
            int rem = num % 1000000;
            if (rem > 0) result += " " + helper(rem);
        } else if (num >= 1000) {
            // 千级
            result = helper(num / 1000) + " Thousand";
            int rem = num % 1000;
            if (rem > 0) result += " " + helper(rem);
        } else if (num >= 100) {
            // 百级：百位一定是 1-9，直接查表
            result = below20[num / 100] + " Hundred";
            int rem = num % 100;
            if (rem > 0) result += " " + helper(rem);
        } else if (num >= 20) {
            // 20-99：十位查表，个位如果非零则追加
            result = tens[num / 10];
            if (num % 10 > 0) {
                result += " " + below20[num % 10];
            }
        } else {
            // 1-19：整体查表
            // 这里是递归的基础情况之一
            result = below20[num];
        }
        
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度        | 解法1 (迭代)          | 解法2 (递归)          |
// |------------|----------------------|----------------------|
// | 处理方向    | 低位→高位(需前插拼接) | 高位→低位(自然顺序)   |
// | 代码结构    | while循环+辅助函数     | 单个递归函数          |
// | 可读性      | 辅助函数职责清晰       | 递归结构更简洁        |
// | 扩展性      | 加新量级改 thousands[] | 加新的 else if 分支   |
// | 面试推荐    | ✓ 均可                | ✓ 均可               |
//
// 两种本质相同，选自己更熟悉的。递归版更简洁，迭代版更易调试。
//
// ============================================================
// 【易错点】
//
// 1. 忘记 num=0 特判
//    ✗ 直接调用 helper(0) 返回空字符串 ""
//    ✓ 入口处 if (num == 0) return "Zero";
//    原因: "Zero" 只在输入本身为 0 时输出，中间段为 0 应该跳过
//
// 2. 10-19 被错误拆分
//    ✗ 15 → tens[1] + below20[5] → "Ten Five"
//    ✓ 15 → below20[15] → "Fifteen"
//    原因: 英语中 11-19 是特殊词，不能用十位+个位拼
//    修复: 先判断 num >= 20 走十位逻辑，否则 1-19 整体查表
//
// 3. 段为 0 时输出多余后缀
//    ✗ 1000000 → "One Million Thousand"（千位段是0但仍输出了Thousand）
//    ✓ if (chunk != 0) 才拼接后缀
//
// 4. 多余空格
//    ✗ "One Hundred  Twenty Three"（两个空格）
//    ✓ 每次拼接前检查是否需要空格，不要盲目加
//    具体bug: result += " " + helper(rem) 当 rem=0 时会多一个空格
//    修复: if (rem > 0) result += " " + helper(rem);
//
// 5. below20 数组大小不足
//    ✗ 数组只有 10 个元素，访问 below20[15] 越界
//    ✓ 数组大小必须为 20，下标 0-19
//
// ============================================================
// 【面试追问】
//
// Q1(基础): 为什么英文数字按三位分段？
//    → 英语计数体系以千为单位（Thousand/Million/Billion），
//      每三位内部的读法规则完全相同，天然适合分治处理。
//
// Q2(细节): 如果输入可以是负数怎么处理？
//    → 先判断 num < 0，输出 "Negative "，然后处理 abs(num)。
//      但注意 INT_MIN 的绝对值溢出 int 范围（-2^31 → 2^31 不在 int 内），
//      需要用 long 来存，或者先特判 INT_MIN。
//
// Q3(变体): 如果要转换成中文数字表示呢？
//    → 中文按四位分段（万、亿），且有"零"的插入规则（中间有零要读，
//      连续零只读一个，末尾零不读）。核心框架类似但分段改为 10000，
//      内部需要额外处理零的逻辑。
//
// Q4(工程): 这个函数在高并发场景下有什么优化空间？
//    → 字符串查表数组可以做成 static const 避免重复构造；
//      结果可以加缓存（LRU/HashMap）如果相同数字频繁查询；
//      字符串拼接可以改用 StringBuilder(Java)/ostringstream(C++) 减少拷贝。
// ============================================================
