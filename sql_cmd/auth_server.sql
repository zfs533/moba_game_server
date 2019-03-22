-- --------------------------------------------------------
-- 主机:                           127.0.0.1
-- 服务器版本:                        5.7.19-log - MySQL Community Server (GPL)
-- 服务器操作系统:                      Win32
-- HeidiSQL 版本:                  9.5.0.5196
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


-- 导出 auth_center 的数据库结构
CREATE DATABASE IF NOT EXISTS `auth_center` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `auth_center`;

-- 导出  表 auth_center.uinfo 结构
CREATE TABLE IF NOT EXISTS `uinfo` (
  `uid` int(11) NOT NULL AUTO_INCREMENT COMMENT '用户唯一ID',
  `unick` varchar(32) NOT NULL DEFAULT '""' COMMENT '用户的昵称',
  `usex` int(8) NOT NULL DEFAULT '0' COMMENT '性别：0:男，1：女',
  `uface` int(8) NOT NULL DEFAULT '0' COMMENT '系统默认的用户头像，整数',
  `uname` varchar(32) NOT NULL DEFAULT '""' COMMENT '用户名字，登录账号',
  `upwd` varchar(32) NOT NULL DEFAULT '""' COMMENT '用户密码，登录密码',
  `phone` int(16) NOT NULL DEFAULT '0' COMMENT '用户电话',
  `email` varchar(64) NOT NULL DEFAULT '""' COMMENT '用户邮件',
  `address` varchar(128) NOT NULL DEFAULT '""' COMMENT '用户城市',
  `uvip` int(8) NOT NULL DEFAULT '0' COMMENT '用户vip等级',
  `vip_end_time` int(32) NOT NULL DEFAULT '0' COMMENT '用户vip结束的时间戳',
  `is_guest` int(8) NOT NULL DEFAULT '0' COMMENT '是否为游客账号0：不是，1：是',
  `guestkey` varchar(64) NOT NULL DEFAULT '0' COMMENT '游客账号的key，游客的唯一的key',
  `status` int(8) NOT NULL DEFAULT '0' COMMENT '0正常，其他根据需求来定',
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB AUTO_INCREMENT=47 DEFAULT CHARSET=utf8 COMMENT='用户中心数据库';

-- 数据导出被取消选择。
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
