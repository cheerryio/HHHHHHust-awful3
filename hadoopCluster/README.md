# Docker-Hadoop-Script(specified to my own homework 5555)

# Homework Requirements
1. Centos
2. Hadoop-3.2.1
3. Master node with ResourceManager,DataNode,NameNode,NodeManager,SecondaryNameNode while worker node with DataNode and NameNode
   
# Simple usage

* *Without some configure, you may need to run `docker` commands with sudo*

## Get the hadoop docker image

There are two ways to get the docker image for the project

1. ### Using the docker image from dockerhub
    ```
    docker pull cherrooo/hadoop:2.0
    ```

2. Build from Dockerfile

   `./resize-cluster.sh 3`

   This command make the cluster number to 3 and build an image from Dockerfile

After getting the neccessary docker image, run

```
docker create network --subnet=10.0.0.0/16 hadoop-net
./start_container.sh
```
Then you would be in the cluster-master container, please type the command below to start hadoop master and hadoop workers
```
./start-hadoop.sh
```

To test your hadoop cluster, type
```
./run-wordcount.sh
```

Also, you can find more information about hadoop in **10.0.0.2:9870** and **10.0.0.2:18088**
