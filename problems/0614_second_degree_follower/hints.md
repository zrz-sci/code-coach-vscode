# 614. 二级关注者 - 提示

## 题目理解
- Follow 表记录了关注关系 (followee, follower)
- **二级关注者**：既关注了别人（出现在 follower 列），又被别人关注（出现在 followee 列）
- 需要统计每个二级关注者的粉丝数量，按字母序排列

## 提示 1 - 找出二级关注者
想一想：一个用户同时出现在 `follower` 列和 `followee` 列，说明什么？
- 出现在 `follower` 列 → 他关注了别人
- 出现在 `followee` 列 → 他被别人关注

## 提示 2 - 统计粉丝数
对于每个二级关注者，他作为 `followee` 出现的次数就是他的粉丝数。用 `GROUP BY` + `COUNT` 即可。

## 提示 3 - 筛选条件
关键在于如何筛选出"关注了至少一个人"的用户：
- 方法一：用 `WHERE followee IN (SELECT DISTINCT follower FROM Follow)` 过滤
- 方法二：用 `JOIN` 将 Follow 表自连接

## 关键知识点
- 自连接 (Self Join)
- `GROUP BY` + `COUNT` 聚合
- `IN` 子查询
- `DISTINCT` 去重

## 复杂度
- 时间：取决于表大小和索引
- 核心操作：一次子查询 + 一次分组聚合
