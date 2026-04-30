/*
 * 【题目本质】
 * 验证整数数组是否构成合法的UTF-8编码序列。
 * 核心：按首字节前缀确定字符长度，逐一验证续字节格式。
 *
 * 【解法总览】
 * Solution1: 状态机(remaining计数)  O(n) / O(1) ⭐推荐
 * Solution2: 逐字符验证(前导1计数)  O(n) / O(1)
 */

// ===================== Solution 1: 状态机 =====================
// 思路：remaining记录当前字符还需要多少续字节
// 遇首字节 -> 根据前缀设定remaining
// 遇续字节 -> remaining--，检查格式10xxxxxx
class Solution1 {
public:
    bool validUtf8(vector<int>& data) {
        int remaining = 0;
        for (int byte : data) {
            if (remaining > 0) {
                if ((byte >> 6) != 0b10) return false;
                remaining--;
            } else {
                if ((byte >> 7) == 0)          remaining = 0;
                else if ((byte >> 5) == 0b110)   remaining = 1;
                else if ((byte >> 4) == 0b1110)  remaining = 2;
                else if ((byte >> 3) == 0b11110) remaining = 3;
                else return false;
            }
        }
        return remaining == 0;
    }
};

// ===================== Solution 2: 逐字符验证 =====================
// 思路：外层按字符推进，内层验证续字节
// countLeadingOnes确定字符长度，然后一次性检查所有续字节
class Solution2 {
public:
    bool validUtf8(vector<int>& data) {
        int i = 0, n = data.size();
        while (i < n) {
            int ones = 0;
            for (int mask = 0x80; mask > 0 && (data[i] & mask); mask >>= 1)
                ones++;

            if (ones == 0) { i++; continue; }       // 1字节ASCII
            if (ones == 1 || ones > 4) return false; // 非法首字节

            // 验证 ones-1 个续字节
            for (int j = 1; j < ones; j++) {
                if (i + j >= n) return false;            // 数据不足
                if ((data[i + j] >> 6) != 0b10) return false; // 格式错误
            }
            i += ones;
        }
        return true;
    }
};

/*
 * 【解法对比】
 * Solution1 状态机：适合流式处理，逐字节到达也能工作
 * Solution2 逐字符：逻辑更清晰，按完整字符处理
 * 两者时间空间复杂度相同
 *
 * 【易错点】
 * 1. 结尾必须检查 remaining==0，防止不完整的多字节字符
 * 2. 首字节前导1个数为1时非法（10xxxxxx只能做续字节）
 * 3. 前导1超过4个也非法（UTF-8最多4字节）
 * 4. 只用每个int的低8位（题目保证0-255）
 *
 * 【面试追问】
 * Q: 如何扩展为同时返回解码后的Unicode码点？
 * A: 在验证的同时，从首字节提取有效位(去掉前缀)，
 *    每个续字节贡献6位(去掉10前缀)，左移拼接即可。
 *    例如2字节：((byte1 & 0x1F) << 6) | (byte2 & 0x3F)
 */
