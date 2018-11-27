-- --------------------------------------------------------
-- 主机:                           127.0.0.1
-- Server version:               5.7.19-log - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL 版本:                  9.5.0.5332
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


-- Dumping database structure for my808
CREATE DATABASE IF NOT EXISTS `my808` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `my808`;

-- Dumping structure for table my808.box_log
CREATE TABLE IF NOT EXISTS `box_log` (
  `phone` varchar(12) NOT NULL,
  `time` varchar(23) NOT NULL,
  `percent` smallint(6) NOT NULL,
  `power` tinyint(4) NOT NULL,
  `weidu` double NOT NULL,
  `jindu` double NOT NULL,
  `temp` float NOT NULL,
  `gps` tinyint(4) NOT NULL,
  `gprs` tinyint(4) NOT NULL,
  `distance` int(11) NOT NULL,
  `event` varchar(4) DEFAULT NULL,
  KEY `Index 1` (`phone`),
  KEY `Index 2` (`time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table my808.oillog
CREATE TABLE IF NOT EXISTS `oillog` (
  `id` varchar(12) DEFAULT NULL,
  `time` varchar(23) DEFAULT NULL,
  `value` double DEFAULT NULL,
  `realvalue` double DEFAULT NULL,
  `wendu` double DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- Data exporting was unselected.
-- Dumping structure for table my808.orgnization
CREATE TABLE IF NOT EXISTS `orgnization` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `org` varchar(50) NOT NULL,
  KEY `Index 1` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table my808.productinfo
CREATE TABLE IF NOT EXISTS `productinfo` (
  `id` char(12) NOT NULL,
  `status` char(6) DEFAULT NULL,
  `volume` int(11) DEFAULT NULL,
  `power` int(11) DEFAULT NULL,
  `address` varchar(50) DEFAULT NULL,
  `last` char(23) DEFAULT NULL,
  `jindu` double DEFAULT NULL,
  `weidu` double DEFAULT NULL,
  `temp` float DEFAULT NULL,
  `boxstatus` varchar(4) DEFAULT NULL,
  UNIQUE KEY `Index 1` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='满溢';

-- Data exporting was unselected.
-- Dumping structure for table my808.rawlog
CREATE TABLE IF NOT EXISTS `rawlog` (
  `phone` varchar(12) NOT NULL,
  `msgid` varchar(4) NOT NULL,
  `time` datetime NOT NULL,
  `log` blob NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table my808.rawlog_s
CREATE TABLE IF NOT EXISTS `rawlog_s` (
  `phone` varchar(12) DEFAULT NULL,
  `msgid` varchar(4) DEFAULT NULL,
  `time` datetime DEFAULT NULL,
  `log` varchar(3320) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table my808.rawlog_s1
CREATE TABLE IF NOT EXISTS `rawlog_s1` (
  `phone` varchar(12) DEFAULT NULL,
  `msgid` varchar(4) DEFAULT NULL,
  `time` varchar(23) DEFAULT NULL,
  `log` varchar(3320) DEFAULT NULL,
  KEY `time` (`time`) USING BTREE,
  KEY `msgid` (`msgid`) USING BTREE,
  KEY `phone` (`phone`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- Data exporting was unselected.
-- Dumping structure for table my808.soundboxcan_log
CREATE TABLE IF NOT EXISTS `soundboxcan_log` (
  `id` varchar(12) NOT NULL,
  `time` varchar(23) NOT NULL,
  `stime` varchar(10) NOT NULL,
  `canid` char(8) NOT NULL,
  `data` char(16) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table my808.soundboxinfo
CREATE TABLE IF NOT EXISTS `soundboxinfo` (
  `id` varchar(12) NOT NULL,
  `time` varchar(23) DEFAULT NULL,
  `j` double DEFAULT NULL,
  `w` double DEFAULT NULL,
  `cj` double DEFAULT NULL,
  `cw` double DEFAULT NULL,
  `status` varchar(50) DEFAULT NULL,
  `acc` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='桑德盒子最新的状态';

-- Data exporting was unselected.
-- Dumping structure for table my808.terminal
CREATE TABLE IF NOT EXISTS `terminal` (
  `no` char(12) NOT NULL,
  `orgid` int(11) NOT NULL,
  PRIMARY KEY (`no`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
-- Dumping structure for table my808.waterlog
CREATE TABLE IF NOT EXISTS `waterlog` (
  `id` varchar(12) DEFAULT NULL,
  `time` varchar(23) DEFAULT NULL,
  `value` double DEFAULT NULL,
  `realvalue` double DEFAULT NULL,
  `tmp` double DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Data exporting was unselected.
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
