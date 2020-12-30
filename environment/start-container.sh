#!/bin/bash

# the default node number is 3
N=${1:-3}


# start hadoop master container
sudo docker rm -f hadoop-master &> /dev/null
echo "start hadoop-master container..."
# NOTE :FROM HADOOP3.0 RESOURCES ON PORT 50070 ARE SHIFTED TO PORT 9870
sudo docker run -itd \
                -p 50070:50070 \
                -p 8088:8088 \
                --name hadoop-master \
                --hostname hadoop-master \
                cherrooo/datamining:1.0 &> /dev/null # NOTE:UPDATE ORIGINAL AUTHOR'S NAME TO MINE

# get into hadoop master container
sudo docker exec -it hadoop-master bash