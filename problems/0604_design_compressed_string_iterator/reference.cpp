// =============================================================================
// 604. Design Compressed String Iterator
// =============================================================================
// 核心思路: 解析压缩字符串中的 (字符, 重复次数) 对
//          维护当前字符和剩余计数, 每次 next() 递减计数
// =============================================================================

// =============================================================================
// 解法一: 预处理到队列 (推荐, 逻辑最清晰)
// =============================================================================
// 构造时一次性解析所有 (char, count) 对到队列
// next() 取队首字符并递减计数, 计数归零则弹出
// 时间: 构造 O(n), next/hasNext O(1)
// 空间: O(n) 存储字符对
class StringIterator {
    queue<pair<char, int>> q;

public:
    StringIterator(string compressedString) {
        int i = 0, n = compressedString.size();
        while (i < n) {
            char ch = compressedString[i++];
            int count = 0;
            // 解析多位数字 (重复次数可达 10^9)
            while (i < n && isdigit(compressedString[i])) {
                count = count * 10 + (compressedString[i++] - '0');
            }
            q.push({ch, count});
        }
    }

    char next() {
        if (!hasNext()) return ' ';
        auto& front = q.front();
        char ch = front.first;
        if (--front.second == 0) q.pop();
        return ch;
    }

    bool hasNext() {
        return !q.empty();
    }
};

// =============================================================================
// 解法二: 惰性解析 - 指针法 (O(1) 额外空间)
// =============================================================================
// 不预处理, 用指针按需解析下一组 (字符, 计数)
// 适合压缩串很长但只调用少量 next() 的场景
// 时间: 构造 O(1), next 均摊 O(1)
// 空间: O(1) 额外空间 (只存原始字符串引用)
class StringIterator_v2 {
    string s;
    int i;
    char ch;
    int count;

    void advance() {
        if (i >= (int)s.size()) return;
        ch = s[i++];
        count = 0;
        while (i < (int)s.size() && isdigit(s[i])) {
            count = count * 10 + (s[i++] - '0');
        }
    }

public:
    StringIterator_v2(string compressedString)
        : s(compressedString), i(0), ch(' '), count(0) {
        advance();
    }

    char next() {
        if (!hasNext()) return ' ';
        char result = ch;
        if (--count == 0) advance();
        return result;
    }

    bool hasNext() {
        return count > 0;
    }
};

// =============================================================================
// 解法三: vector 预处理 (支持随机访问和 reset)
// =============================================================================
// 与队列方案类似, 但用 vector + index 代替 queue
// 优点: 支持 reset() 只需将 index 重置为 0
// 时间: 构造 O(n), next/hasNext O(1)
// 空间: O(n)
class StringIterator_v3 {
    vector<pair<char, long>> groups;
    int idx;

public:
    StringIterator_v3(string compressedString) : idx(0) {
        int i = 0, n = compressedString.size();
        while (i < n) {
            char ch = compressedString[i++];
            long count = 0;
            while (i < n && isdigit(compressedString[i])) {
                count = count * 10 + (compressedString[i++] - '0');
            }
            groups.push_back({ch, count});
        }
    }

    char next() {
        if (!hasNext()) return ' ';
        char ch = groups[idx].first;
        if (--groups[idx].second == 0) idx++;
        return ch;
    }

    bool hasNext() {
        return idx < (int)groups.size();
    }
};
