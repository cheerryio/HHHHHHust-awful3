#!/bin/bash

# the default node number is 3
N=${1:-3}


# start hadoop master container
sudo docker rm -f cluster-master &> /dev/null
echo "start cluster-master container..."
# NOTE :FROM HADOOP3.0 RESOURCES ON PORT 50070 ARE SHIFTED TO PORT 9870
sudo docker run -itd \
                --net=hadoop-net \
                -p 18088:18088 \
                -p 9870:9870 \
                --name cluster-master \
                --hostname cluster-master \
				--ip 10.0.0.2 \
                cherrooo/hadoop:2.0 &> /dev/null # NOTE:UPDATE ORIGINAL AUTHOR'S NAME TO MINE


# start hadoop slave container
i=1
while [ $i -lt $N ]
do
	sudo docker rm -f cluster-worker$i &> /dev/null
	echo "start cluster-worker$i container..."
	sudo docker run -itd \
	                --net=hadoop-net \
	                --name cluster-worker$i \
	                --hostname cluster-worker$i \
					--ip 10.0.0.$(($i+2)) \
	                cherrooo/hadoop:2.0 &> /dev/null # NOTE:UPDATE ORIGINAL AUTHOR'S NAME TO MINE
	i=$(( $i + 1 ))
done 

# get into hadoop master container
sudo docker exec -it cluster-master bash
