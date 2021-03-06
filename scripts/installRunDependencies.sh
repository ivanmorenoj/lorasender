#!/bin/bash

set -e

#install packages to run 
apt-get update &&
apt-get install -y \
    libconfig++-dev \
    libmysqlcppconn-dev \
    wiringpi \
    mariadb-server

#clean
apt-get clean

echo 'Delete /var/lib/apt/lists/* /tmp/* /var/tmp/*' 
rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

mysql -u root -e "CREATE USER 'lorasender'@'localhost' IDENTIFIED BY 'lora-p@s2';"
mysql -u root -e "GRANT SELECT ON EMCA.* to 'lorasender'@'localhost';"
mysql -u root -e "FLUSH PRIVILEGES;"