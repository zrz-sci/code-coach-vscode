-- LeetCode 614: Second Degree Follower
-- 难度: Medium
-- 思路: 找出同时出现在 follower 和 followee 列中的用户，统计其粉丝数

-- 方法一：子查询
SELECT
    followee AS follower,
    COUNT(DISTINCT follower) AS num
FROM Follow
WHERE followee IN (SELECT DISTINCT follower FROM Follow)
GROUP BY followee
ORDER BY followee;

-- 方法二：自连接
-- SELECT
--     f1.followee AS follower,
--     COUNT(DISTINCT f1.follower) AS num
-- FROM Follow f1
-- JOIN (SELECT DISTINCT follower FROM Follow) f2
--     ON f1.followee = f2.follower
-- GROUP BY f1.followee
-- ORDER BY f1.followee;
