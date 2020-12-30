#!/bin/bash

# N is the node number of hadoop cluster
N=$1

if [ $# = 0 ]
then
	echo "Please specify the node number of hadoop cluster!"
	exit 1
fi

# change slaves file
i=1
rm config/slaves
rm config/workers		#NOTE:ADDITIONAL ADDING
echo "cluster-master" >> config/slaves
echo "cluster-master" >> config/workers

while [ $i -lt $N ]
do
	echo "cluster-worker$i" >> config/slaves
	echo "cluster-worker$i" >> config/workers	#NOTE:ADDITIONAL ADDING
	((i++))
done 

echo ""

echo -e "\nbuild docker hadoop image\n"

# rebuild kiwenlau/hadoop image
# NOTE:UPDATE ORIGINAL AUTHOR'S NAME TO MINE
sudo docker build -t cherrooo/hadoop:2.0 .

echo ""
