FROM ubuntu:18.04

LABEL maintainer="cherro cherro@qq.com"

ARG TZ="Asia/Shanghai"

WORKDIR /root
ENV TZ ${TZ}
RUN ln -snf /usr/share/zoneinfo/${TZ} /etc/localtime && \
    echo ${TZ} > /etc/timezone

COPY config/sources.list /etc/apt

COPY packages/* /tmp/
RUN dpkg -i /tmp/libssl1.1_1.1.1-1ubuntu2.1_18.04.6_amd64.deb && \
    dpkg -i /tmp/openssl_1.1.1-1ubuntu2.1_18.04.6_amd64.deb && \
    dpkg -i /tmp/ca-certificates_20201027ubuntu0.18.04.1_all.deb

RUN apt-get update && \
    apt-get install -y openssh-server wget

#needed packages
RUN tar -xzvf /tmp/jdk-7u71-linux-x64.tar.gz && \
    tar -xzvf /tmp/hadoop-2.4.1.tar.gz && \
    tar -xzvf /tmp/scala-2.11.6.tgz && \
    tar -xzvf /tmp/spark-1.3.1-bin-hadoop2.6.tgz && \
    mv jdk1.7.0_71 /usr/local/java && \
    mv hadoop-2.4.1 /usr/local/hadoop && \
    mv scala-2.11.6 /usr/local/scala && \
    mv spark-1.3.1-bin-hadoop2.6 /usr/local/spark

ENV PATH $PATH:/usr/local/scala/bin
ENV PATH $PATH:/usr/local/spark/bin
ENV JAVA_HOME /usr/local/java
ENV PATH $PATH:${JAVA_HOME}/bin
# set hadoop's environment variables
ENV HADOOP_HOME /usr/local/hadoop
ENV HADOOP_MAPRED_HOME ${HADOOP_HOME}
ENV HADOOP_COMMON_HOME ${HADOOP_HOME}
ENV HADOOP_HDFS_HOME ${HADOOP_HOME}
ENV YARN_HOME ${HADOOP_HOME}
ENV HADOOP_COMMON_LIB_NATIVE_DIR ${HADOOP_HOME}/lib/native
ENV PATH $PATH:${HADOOP_HOME}/bin:${HADOOP_HOME}/sbin 

#about java alternatives setting
RUN update-alternatives --install /usr/bin/java java /usr/local/java/bin/java 2 && \
    update-alternatives --install /usr/bin/javac javac /usr/local/java/bin/javac 2 && \
    update-alternatives --install /usr/bin/jar jar /usr/local/java/bin/jar 2 && \
    update-alternatives --set java /usr/local/java/bin/java && \
    update-alternatives --set javac /usr/local/java/bin/javac && \
    update-alternatives --set jar /usr/local/java/bin/jar

# ssh without key
RUN ssh-keygen -t rsa -f ~/.ssh/id_rsa -P '' && \
    cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys && \
    mkdir -p ~/hdfs/namenode && \ 
    mkdir -p ~/hdfs/datanode && \
    mkdir $HADOOP_HOME/logs

COPY config/* /tmp/
#hadoop config file setting
RUN mv /tmp/ssh_config ~/.ssh/config && \
    mv /tmp/hadoop-env.sh /usr/local/hadoop/etc/hadoop/hadoop-env.sh && \
    mv /tmp/mapred-site.xml $HADOOP_HOME/etc/hadoop/mapred-site.xml && \
    mv /tmp/yarn-site.xml $HADOOP_HOME/etc/hadoop/yarn-site.xml && \
    mv /tmp/core-site.xml $HADOOP_HOME/etc/hadoop/core-site.xml && \
    mv /tmp/hdfs-site.xml $HADOOP_HOME/etc/hadoop/hdfs-site.xml && \
    mv /tmp/workers $HADOOP_HOME/etc/hadoop/workers && \
    mv /tmp/start-hadoop.sh ~/start-hadoop.sh && \
    mv /tmp/run-wordcount.sh ~/run-wordcount.sh

#shell script mode
RUN chmod +x ~/start-hadoop.sh && \
    chmod +x ~/run-wordcount.sh && \
    chmod +x $HADOOP_HOME/sbin/start-dfs.sh && \
    chmod +x $HADOOP_HOME/sbin/start-yarn.sh

# format namenode
RUN /usr/local/hadoop/bin/hdfs namenode -format

EXPOSE 8088 50070

CMD ["sh","-c","service ssh start ; /root/start-hadoop.sh ; bash"]