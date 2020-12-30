#!/bin/bash

# the default node number is 3
N=${1:-3}

echo "deleting cluster-master"
sudo docker rm -f cluster-master &> /dev/null

i=1
while [ $i -lt $N ]
do
    echo "deleting cluster-worker$i"
	sudo docker rm -f cluster-worker$i &> /dev/null
    i=$(( $i + 1 ))
done