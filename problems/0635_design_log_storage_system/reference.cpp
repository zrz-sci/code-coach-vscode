// LeetCode 635: Design Log Storage System - Reference Solution
// 方法: 按粒度截断时间字符串，利用字典序比较

class LogSystem {
    vector<pair<int, string>> logs;
    unordered_map<string, int> granMap;

public:
    LogSystem() {
        granMap["Year"]   = 4;
        granMap["Month"]  = 7;
        granMap["Day"]    = 10;
        granMap["Hour"]   = 13;
        granMap["Minute"] = 16;
        granMap["Second"] = 19;
    }

    void put(int id, string timestamp) {
        logs.push_back({id, timestamp});
    }

    vector<int> retrieve(string start, string end, string granularity) {
        int len = granMap[granularity];
        string s = start.substr(0, len);
        string e = end.substr(0, len);

        vector<int> result;
        for (auto& [id, ts] : logs) {
            string t = ts.substr(0, len);
            if (t >= s && t <= e) {
                result.push_back(id);
            }
        }
        return result;
    }
};
