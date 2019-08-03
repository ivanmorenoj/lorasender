#!/bin/bash

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