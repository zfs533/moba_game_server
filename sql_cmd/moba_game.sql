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


-- 导出 moba_game 的数据库结构
CREATE DATABASE IF NOT EXISTS `moba_game` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `moba_game`;

-- 导出  表 moba_game.login_bonues 结构
CREATE TABLE IF NOT EXISTS `login_bonues` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '领取奖励的唯一ID号',
  `uid` int(11) DEFAULT NULL COMMENT '用户的UID',
  `bonues` int(11) DEFAULT '0' COMMENT '奖励的数目',
  `status` int(11) DEFAULT '0' COMMENT '是否已经领取，0未领取，1已领取',
  `bonues_time` int(11) DEFAULT NULL COMMENT '发放奖励的时间',
  `days` int(11) DEFAULT '0' COMMENT '连续登录的天数',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 COMMENT='登录奖励管理';

-- 数据导出被取消选择。
-- 导出  表 moba_game.ugame 结构
CREATE TABLE IF NOT EXISTS `ugame` (
  `id` int(32) NOT NULL AUTO_INCREMENT COMMENT '记录唯一的id号',
  `uid` int(32) NOT NULL COMMENT '用户唯一的id号',
  `uchip` int(32) NOT NULL DEFAULT '0' COMMENT '用户的金币数目',
  `uchip1` int(32) NOT NULL DEFAULT '0' COMMENT '用户的其他货币或等价物品',
  `uchip2` int(32) NOT NULL DEFAULT '0' COMMENT '用户的其他货币或等价物品',
  `uvip` int(8) NOT NULL DEFAULT '0' COMMENT '用户在本游戏中的vip等级',
  `uvip_endtime` int(32) NOT NULL DEFAULT '0' COMMENT 'vip结束时间',
  `udata1` int(32) NOT NULL DEFAULT '0' COMMENT '用户在游戏中的道具1',
  `udata2` int(32) NOT NULL DEFAULT '0' COMMENT '用户在游戏中的道具2',
  `udata3` int(32) NOT NULL DEFAULT '0' COMMENT '用户在游戏中的道具3',
  `uexp` int(32) NOT NULL DEFAULT '0' COMMENT '用户的经验值',
  `ustatus` int(8) NOT NULL DEFAULT '0' COMMENT '用户账号状态，0正常，其他不正常',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COMMENT='存放玩家在moba这个游戏中的玩家的游戏数据，主要的游戏数据；\r\n金币，其他货币，道具，游戏中的vip等级，账号状态，玩家经验；\r\nuid:标识玩家的，id:作为自增长得唯一id号';

-- 数据导出被取消选择。
-- 导出  表 moba_game.ulevel 结构
CREATE TABLE IF NOT EXISTS `ulevel` (
  `id` int(8) NOT NULL DEFAULT '0' COMMENT '唯一id标识',
  `ulevel` int(8) DEFAULT '0' COMMENT '当前经验对应的等级',
  `uexp` int(32) DEFAULT '0' COMMENT '升级所需对应的经验值',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='游戏玩家等级配置，每升一级需要多少经验';

-- 数据导出被取消选择。
-- 导出  表 moba_game.urank 结构
CREATE TABLE IF NOT EXISTS `urank` (
  `id` int(16) NOT NULL AUTO_INCREMENT COMMENT '唯一标识id',
  `uid` int(16) NOT NULL DEFAULT '0' COMMENT '用户唯一ID',
  `unick` varchar(64) NOT NULL DEFAULT '""' COMMENT '用户昵称',
  `usex` int(8) NOT NULL DEFAULT '0' COMMENT '性别：0:男，1：女',
  `uvip` int(8) NOT NULL DEFAULT '0' COMMENT '用户游戏vip等级',
  `uchip1` int(32) NOT NULL DEFAULT '0' COMMENT '用户金币数',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='排行榜';

-- 数据导出被取消选择。
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
