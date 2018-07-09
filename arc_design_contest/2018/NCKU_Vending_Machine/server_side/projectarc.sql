-- phpMyAdmin SQL Dump
-- version 4.7.7
-- https://www.phpmyadmin.net/
--
-- 主機: 127.0.0.1
-- 產生時間： 2018 年 06 月 24 日 16:12
-- 伺服器版本: 10.1.30-MariaDB
-- PHP 版本： 7.2.2

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- 資料庫： `projectarc`
--

-- --------------------------------------------------------

--
-- 資料表結構 `costomer`
--

CREATE TABLE `costomer` (
  `Rfid` int(11) NOT NULL,
  `Time` date DEFAULT NULL,
  `like1` int(11) DEFAULT NULL,
  `like2` int(11) DEFAULT NULL,
  `like3` int(11) DEFAULT NULL,
  `like4` int(11) DEFAULT NULL,
  `Balance` int(11) DEFAULT NULL,
  `State` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- 資料表的匯出資料 `costomer`
--

INSERT INTO `costomer` (`Rfid`, `Time`, `like1`, `like2`, `like3`, `like4`, `Balance`, `State`) VALUES
(1234, '2018-05-16', 9, 8, 12, 13, 1741, 0);

-- --------------------------------------------------------

--
-- 資料表結構 `item`
--

CREATE TABLE `item` (
  `序號` int(10) UNSIGNED NOT NULL,
  `Name` varchar(10) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL,
  `Price` mediumint(9) DEFAULT NULL,
  `Type` varchar(10) CHARACTER SET utf32 COLLATE utf32_unicode_ci DEFAULT NULL,
  `Exp_Date` date DEFAULT NULL,
  `Num` mediumint(9) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- 資料表的匯出資料 `item`
--

INSERT INTO `item` (`序號`, `Name`, `Price`, `Type`, `Exp_Date`, `Num`) VALUES
(1, 'Soda', 27, 'drinks', '2018-06-23', 2),
(2, 'Water', 25, 'drinks', '2018-06-29', 5),
(3, 'Coffee', 25, 'drinks', '2018-07-31', 4),
(4, 'Chips', 25, 'food', '2018-07-03', 6);

-- --------------------------------------------------------

--
-- 資料表結構 `record`
--

CREATE TABLE `record` (
  `Buy_Time` datetime NOT NULL,
  `Rfid` int(11) NOT NULL,
  `Item` varchar(20) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `Cost` int(11) NOT NULL,
  `Balance` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- 資料表的匯出資料 `record`
--

INSERT INTO `record` (`Buy_Time`, `Rfid`, `Item`, `Cost`, `Balance`) VALUES
('2018-06-17 12:02:05', 1234, 'Chips', 25, 492),
('2018-06-17 12:03:08', 1234, 'Chips', 25, 467),
('2018-06-17 12:03:35', 1234, 'Chips', 25, 442),
('2018-06-17 12:03:53', 1234, 'Chips', 25, 417),
('2018-06-17 12:04:42', 1234, 'Chips', 25, 392),
('2018-06-17 12:08:12', 1234, 'Chips', 25, 367),
('2018-06-17 12:09:33', 1234, 'Chips', 25, 342),
('2018-06-17 12:14:47', 1234, 'Water', 25, 317),
('2018-06-17 12:15:37', 1234, 'Water', 25, 292),
('2018-06-17 12:27:51', 1234, 'Water', 25, 267),
('2018-06-17 12:31:18', 1234, 'Water', 25, 242),
('2018-06-17 12:31:58', 1234, 'Water', 25, 217),
('2018-06-17 12:59:15', 1234, 'Water', 25, 192),
('2018-06-17 18:06:18', 1234, 'Soda', 27, 1893),
('2018-06-17 18:06:36', 1234, 'Soda', 27, 1866),
('2018-06-17 18:07:08', 1234, 'Coffee', 25, 1841),
('2018-06-17 18:07:22', 1234, 'Coffee', 25, 1816),
('2018-06-20 15:49:43', 1234, 'Coffee', 25, 1791),
('2018-06-20 15:59:00', 1234, 'Coffee', 25, 1766),
('2018-06-20 15:59:35', 1234, 'Coffee', 25, 1741);

-- --------------------------------------------------------

--
-- 資料表結構 `sign_data`
--

CREATE TABLE `sign_data` (
  `account` varchar(30) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `Authority` int(5) DEFAULT NULL,
  `password` varchar(65) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- 資料表的匯出資料 `sign_data`
--

INSERT INTO `sign_data` (`account`, `Authority`, `password`) VALUES
('joanne', 1, '9c64dcb56c7495e197add6833f310ce064da11ccfba07c53c3a17911b7d6cf19'),
('peter1719', 1, '02f6ce48da6a03ea57db8446f5d5a944b72e96859d8aa6577516503ad6f48859');

-- --------------------------------------------------------

--
-- 資料表結構 `state`
--

CREATE TABLE `state` (
  `M_ID` int(11) NOT NULL,
  `State` int(11) NOT NULL,
  `Temperature` float NOT NULL,
  `Time` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- 資料表的匯出資料 `state`
--

INSERT INTO `state` (`M_ID`, `State`, `Temperature`, `Time`) VALUES
(1, 1, 26.25, '2018-06-20 15:59:35');

--
-- 已匯出資料表的索引
--

--
-- 資料表索引 `costomer`
--
ALTER TABLE `costomer`
  ADD UNIQUE KEY `Rfid` (`Rfid`);

--
-- 資料表索引 `item`
--
ALTER TABLE `item`
  ADD PRIMARY KEY (`序號`),
  ADD UNIQUE KEY `序號` (`序號`),
  ADD KEY `序號_2` (`序號`);

--
-- 資料表索引 `record`
--
ALTER TABLE `record`
  ADD UNIQUE KEY `Buy_Time` (`Buy_Time`);

--
-- 資料表索引 `sign_data`
--
ALTER TABLE `sign_data`
  ADD UNIQUE KEY `account` (`account`);

--
-- 資料表索引 `state`
--
ALTER TABLE `state`
  ADD UNIQUE KEY `MID` (`M_ID`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
