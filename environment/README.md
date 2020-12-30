# Data Mining homework

## Environment build
run `docker build .` to build the image from dockerfile

run `./start-container.sh` to launch a container from the built image

Please wait for seconds after you are inside the docker container because it takes some to for hadoop to start up.After that, visit http://127.0.0.1:8088 to check the resourceManager site and http://127.0.0.1:50070 to check the hdfs site.