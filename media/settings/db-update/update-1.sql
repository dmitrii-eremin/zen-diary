ALTER TABLE `notes` ADD COLUMN `mood` TEXT;

REPLACE INTO `settings` (`key`, `value`) VALUES('version', '2');