# 635. 设计日志存储系统 - 提示

## 题目关键信息
- 存储日志 (id, timestamp)，timestamp 格式: "Year:Month:Day:Hour:Minute:Second"
- retrieve 按粒度查询: 根据 granularity 截断时间后做范围比较
- 粒度包括: Year, Month, Day, Hour, Minute, Second

## 提示 1 - 核心思路
关键在于 granularity 的处理 -- 根据粒度截断时间字符串，只比较对应精度的部分。

例如 granularity = "Day"，只比较 "Year:Month:Day" 部分。

## 提示 2 - 截断位置
timestamp 格式固定，每个粒度对应截取的长度:
- Year: 4   ("2017")
- Month: 7  ("2017:01")
- Day: 10   ("2017:01:01")
- Hour: 13  ("2017:01:01:23")
- Minute: 16 ("2017:01:01:23:59")
- Second: 19 ("2017:01:01:23:59:59")

用 map 存储粒度到截取长度的映射。

## 提示 3 - 存储和查询
- put: 直接把 (id, timestamp) 存入 vector
- retrieve: 遍历所有日志，截断后做字符串比较

## 提示 4 - 字符串比较的巧妙之处
由于时间格式是固定宽度、零填充的，字符串的字典序就是时间顺序，可以直接用 `>=` 和 `<=` 比较。

## 复杂度
- put: O(1)
- retrieve: O(n) 其中 n 为日志数量
