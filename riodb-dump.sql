-- MySQL dump 10.16  Distrib 10.2.22-MariaDB, for Linux (x86_64)
--
-- Host: localhost    Database: RIODB
-- ------------------------------------------------------
-- Server version	10.2.22-MariaDB-log

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
-- Table structure for table `Messages`
--

DROP TABLE IF EXISTS `Messages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Messages` (
  `MessageId` int(11) NOT NULL AUTO_INCREMENT,
  `Message` varchar(255) NOT NULL,
  PRIMARY KEY (`MessageId`)
) ENGINE=InnoDB AUTO_INCREMENT=52 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Messages`
--

LOCK TABLES `Messages` WRITE;
/*!40000 ALTER TABLE `Messages` DISABLE KEYS */;
INSERT INTO `Messages` VALUES (1,'DB MANUAL TEST [dum. iul. 28 05:23:28 2019]'),(2,'DB Init OK [dum. iul. 28 05:29:49 2019]'),(3,'DB Init OK [dum. iul. 28 05:29:49 2019]'),(4,'DB Init OK [dum. iul. 28 15:35:05 2019]'),(5,'DB Init OK [dum. iul. 28 15:35:05 2019]'),(6,'DB Init OK [dum. iul. 28 16:41:43 2019]'),(7,'DB Init OK [dum. iul. 28 16:41:43 2019]'),(8,'DB Init OK [dum. iul. 28 16:58:58 2019]'),(9,'DB Init OK [dum. iul. 28 16:58:58 2019]'),(10,'DB Init OK [dum. iul. 28 17:00:54 2019]'),(11,'DB Init OK [dum. iul. 28 17:00:54 2019]'),(12,'DB Init OK [dum. iul. 28 17:19:11 2019]'),(13,'DB Init OK [dum. iul. 28 17:19:11 2019]'),(14,'DB Init OK [dum. iul. 28 17:20:21 2019]'),(15,'DB Init OK [dum. iul. 28 17:20:21 2019]'),(16,'DB Init OK [dum. iul. 28 17:39:03 2019]'),(17,'DB Init OK [dum. iul. 28 17:39:03 2019]'),(18,'DB Init OK [dum. iul. 28 17:41:06 2019]'),(19,'DB Init OK [dum. iul. 28 17:41:06 2019]'),(20,'DB Init OK [dum. iul. 28 17:43:09 2019]'),(21,'DB Init OK [dum. iul. 28 17:43:09 2019]'),(22,'DB Init OK [dum. iul. 28 17:45:57 2019]'),(23,'DB Init OK [dum. iul. 28 17:45:57 2019]'),(24,'DB Init OK [dum. iul. 28 17:49:20 2019]'),(25,'DB Init OK [dum. iul. 28 17:49:20 2019]'),(26,'DB Init OK [dum. iul. 28 17:54:00 2019]'),(27,'DB Init OK [dum. iul. 28 17:54:00 2019]'),(28,'DB Init OK [dum. iul. 28 17:58:52 2019]'),(29,'DB Init OK [dum. iul. 28 17:58:52 2019]'),(30,'DB Init OK [dum. iul. 28 18:00:56 2019]'),(31,'DB Init OK [dum. iul. 28 18:00:56 2019]'),(32,'DB Init OK [dum. iul. 28 18:02:32 2019]'),(33,'DB Init OK [dum. iul. 28 18:02:32 2019]'),(34,'DB Init OK [dum. iul. 28 18:05:16 2019]'),(35,'DB Init OK [dum. iul. 28 18:05:16 2019]'),(36,'DB Init OK [dum. iul. 28 18:15:43 2019]'),(37,'DB Init OK [dum. iul. 28 18:15:43 2019]'),(38,'DB Init OK [dum. iul. 28 18:27:33 2019]'),(39,'DB Init OK [dum. iul. 28 18:27:33 2019]'),(40,'DB Init OK [dum. iul. 28 18:30:31 2019]'),(41,'DB Init OK [dum. iul. 28 18:30:31 2019]'),(42,'test'),(43,'test'),(44,'test'),(45,'test'),(46,'test'),(47,'test'),(48,'test34'),(49,'test34'),(50,'32432'),(51,'32432');
/*!40000 ALTER TABLE `Messages` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Sessions`
--

DROP TABLE IF EXISTS `Sessions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Sessions` (
  `SessionId` int(11) NOT NULL AUTO_INCREMENT,
  `BytesReceived` int(11) DEFAULT NULL,
  `Reconnects` int(11) DEFAULT NULL,
  PRIMARY KEY (`SessionId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Sessions`
--

LOCK TABLES `Sessions` WRITE;
/*!40000 ALTER TABLE `Sessions` DISABLE KEYS */;
/*!40000 ALTER TABLE `Sessions` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2019-07-29  1:43:00
