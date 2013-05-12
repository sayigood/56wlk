-- MySQL dump 10.13  Distrib 5.1.67, for redhat-linux-gnu (x86_64)
--
-- Host: localhost    Database: chemanman
-- ------------------------------------------------------
-- Server version	5.1.67

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `carsource_table`
--

DROP TABLE IF EXISTS `carsource_table`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `carsource_table` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `start_place` varchar(32) DEFAULT NULL COMMENT '出发地',
  `to_place` varchar(32) DEFAULT NULL COMMENT '目的地',
  `contact_id` int(12) DEFAULT NULL COMMENT '联系id',
  `company_name` varchar(32) DEFAULT NULL COMMENT '企业名',
  `contact_name` varchar(16) CHARACTER SET ucs2 DEFAULT NULL COMMENT '联系人姓名',
  `users_type` varchar(16) DEFAULT NULL COMMENT '用户类型',
  `phone_contact` varchar(16) DEFAULT NULL COMMENT '手机',
  `tel` varchar(32) DEFAULT NULL COMMENT '座机',
  `car_id` int(11) DEFAULT NULL COMMENT '车辆id',
  `models` varchar(16) DEFAULT NULL COMMENT '车型',
  `license_plate` varchar(16) DEFAULT NULL COMMENT '车牌',
  `truck_long` varchar(16) DEFAULT NULL COMMENT '车长',
  `load` varchar(16) DEFAULT NULL COMMENT '载重',
  `truck_image` varchar(1024) DEFAULT NULL COMMENT '车辆图片',
  `fabu_ip` int(11) DEFAULT NULL COMMENT '发布人ip',
  `start_time` varchar(16) DEFAULT NULL COMMENT '发布时间',
  `changnian_lines` tinyint(1) DEFAULT '0' COMMENT '标记 常年线路',
  `specia_lines` tinyint(1) DEFAULT '0' COMMENT '专线标记',
  `user_id` int(11) DEFAULT NULL COMMENT '用户id',
  `ruku_time` timestamp NULL DEFAULT CURRENT_TIMESTAMP COMMENT '入库时间',
  `pub_time` varchar(16) DEFAULT NULL COMMENT '发布时间',
  `click_count` mediumint(9) DEFAULT NULL COMMENT '点击数',
  `url` varchar(1024) DEFAULT NULL COMMENT '采集url',
  `remark` varchar(4096) DEFAULT NULL COMMENT '备注',
  `trans_way` varchar(16) DEFAULT NULL COMMENT '货运方式，单程or往返',
  `trans_type` varchar(16) DEFAULT NULL COMMENT '运输方式，海运or货运',
  `period` varchar(16) DEFAULT NULL COMMENT '货运周期',
  `price` varchar(16) DEFAULT NULL COMMENT '运价',
  `addr` varchar(32) DEFAULT NULL COMMENT '地址',
  `tax` varchar(16) DEFAULT NULL COMMENT '传真',
  `im` varchar(32) DEFAULT NULL COMMENT '即时通讯工具号码',
  `truck_type` varchar(16) DEFAULT NULL COMMENT '车型',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `goodsource_table`
--

DROP TABLE IF EXISTS `goodsource_table`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `goodsource_table` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `pubdate` varchar(16) DEFAULT NULL COMMENT 'å‘å¸ƒæ—¶é—´',
  `userip` varchar(16) DEFAULT NULL COMMENT 'ç”¨æˆ·ip',
  `fromprovince` varchar(16) DEFAULT NULL COMMENT 'å‡ºå‘åœ°-çœ',
  `fromcity` varchar(16) DEFAULT NULL COMMENT 'å‡ºå‘åœ°-å¸‚',
  `fromcounty` varchar(16) DEFAULT NULL COMMENT 'å‡ºå‘åœ°-å›½å®¶',
  `toprovince` varchar(16) DEFAULT NULL COMMENT 'ç›®çš„åœ°-çœ',
  `tocity` varchar(16) DEFAULT NULL COMMENT 'ç›®çš„åœ°-å¸‚',
  `tocounty` varchar(16) DEFAULT NULL COMMENT 'ç›®çš„åœ°-å›½å®¶',
  `goodtype` varchar(16) DEFAULT NULL COMMENT 'è´§ç‰©ç±»åž‹',
  `goodamount` varchar(16) DEFAULT NULL COMMENT 'è´§ç‰©é‡é‡',
  `fee` varchar(16) DEFAULT NULL COMMENT 'è´¹ç”¨',
  `carload` varchar(16) DEFAULT NULL COMMENT 'è½½é‡',
  `carnumber` varchar(16) DEFAULT NULL COMMENT 'è½¦ç‰Œå·',
  `cartype` varchar(16) DEFAULT NULL COMMENT 'è½¦ç±»åž‹',
  `carlong` varchar(16) DEFAULT NULL COMMENT 'è½¦é•¿',
  `linkman` varchar(16) DEFAULT NULL COMMENT 'è”ç³»äºº',
  `phone` varchar(16) DEFAULT NULL COMMENT 'æ‰‹æœºå·ç ',
  `ruku_time` timestamp NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'å…¥åº“æ—¶é—´',
  `datafrom` varchar(16) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=39982 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2013-05-05 15:51:01
