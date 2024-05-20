#!/bin/bash
sudo -s <<EOF
cd ./libs/yolo/opencv/lib
DIR=$(cd $(dirname $0) && pwd )
cd /etc/ld.so.conf.d/
if [ -d "./opencv.conf" ]; then
    touch opencv.conf
fi
echo $DIR >> opencv.conf
ldconfig
EOF
