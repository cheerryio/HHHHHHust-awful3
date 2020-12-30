FROM daocloud.io/library/centos:latest

WORKDIR /root
USER root

# install openssh-server, openjdk and wget
# NOTE :UPDATE openjdk-7-jdk to openjdk-8-jdk
RUN yum -y update && yum -y install openssh openssh-server openssh-clients wget java-1.8.0-openjdk java-1.8.0-openjdk-devel

# NOTE:I FIND DOWNLOAD HADOOP PACKAGE FROM THE INTERNET ISNOT A GOOD CHOICE,SO,YOU MAY HAVE TO
# DOWNLOAD THE .tar.gz PACKAGE YOURSELF AND PUT THE PACKAGE IN THE REPOSITORY's ROOT DIRECTORY
# install hadoop 3.1.4
RUN wget http://mirrors.tuna.tsinghua.edu.cn/apache/hadoop/common/hadoop-3.2.1/hadoop-3.2.1.tar.gz && \
    tar -xzvf hadoop-3.2.1.tar.gz && \
    mv hadoop-3.2.1 /usr/local/hadoop && \
    rm hadoop-3.2.1.tar.gz

# set environment variable
# NOTE:UPDATE JAVA_HOME FROM /usr/lib/jvm/java-7-openjdk-amd64 to /usr/lib/jvm/java-8-openjdk-amd64 
ENV JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.265.b01-0.el8_2.x86_64
ENV HADOOP_HOME=/usr/local/hadoop
ENV PATH=$PATH:/usr/local/hadoop/bin:/usr/local/hadoop/sbin:$JAVA_HOME/bin

# ssh without key
RUN ssh-keygen -t rsa -f ~/.ssh/id_rsa -P '' && \
    cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys && \
    cat ~/.ssh/id_rsa >> /etc/ssh/ssh_host_rsa_key && \
    chmod 400 /etc/ssh/ssh_host_rsa_key

# mount and share directory with host and cluster
VOLUME /sys/fs/cgroup

RUN mkdir -p ~/hdfs/namenode && \ 
    mkdir -p ~/hdfs/datanode && \
    mkdir $HADOOP_HOME/logs

COPY config/* /tmp/

RUN mv /tmp/ssh_config ~/.ssh/config && \
    mv /tmp/hadoop-env.sh /usr/local/hadoop/etc/hadoop/hadoop-env.sh && \
    mv /tmp/hdfs-site.xml $HADOOP_HOME/etc/hadoop/hdfs-site.xml && \ 
    mv /tmp/core-site.xml $HADOOP_HOME/etc/hadoop/core-site.xml && \
    mv /tmp/mapred-site.xml $HADOOP_HOME/etc/hadoop/mapred-site.xml && \
    mv /tmp/yarn-site.xml $HADOOP_HOME/etc/hadoop/yarn-site.xml && \
    mv /tmp/slaves $HADOOP_HOME/etc/hadoop/slaves && \
    # NOTE:HADOOP VERSION 3.1.4 and 3.2.1 READS SLAVES'S HOSTNAMES FROM "worders" FILES
    mv /tmp/workers $HADOOP_HOME/etc/hadoop/workers && \
    mv /tmp/start-hadoop.sh ~/start-hadoop.sh && \
    mv /tmp/run-wordcount.sh ~/run-wordcount.sh

RUN chmod +x ~/start-hadoop.sh && \
    chmod +x ~/run-wordcount.sh && \
    chmod +x $HADOOP_HOME/sbin/start-dfs.sh && \
    chmod +x $HADOOP_HOME/sbin/start-yarn.sh 

# format namenode
RUN /usr/local/hadoop/bin/hdfs namenode -format

EXPOSE 22
CMD ["/usr/sbin/sshd", "-D"]
