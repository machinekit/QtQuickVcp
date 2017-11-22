#!/bin/bash
# Run this script from the projects root directory

set -e # exit on error
set -x # echo commands

docker_image=machinekoder/qtquickvcp-docker-linux-x64:latest
project=qtquickvp
platform=linux-x64
recipe=Linux/portable/Recipe

docker pull ${docker_image}
docker rm ${project}-${platform} &> /dev/null || true
docker run --name ${project}-${platform} -i \
     -v /etc/group:/etc/group:ro -v /etc/passwd:/etc/passwd:ro \
     -u $( id -u $USER ):$( id -g $USER ) \
     -v "${PWD}:/${project}" ${docker_image} \
     /bin/bash -c "cd ${project}; ./build/${recipe}"
docker rm ${project}-${platform} &> /dev/null

mv build.release/MachinekitClient.AppImage MachinekitClient.AppImage
mv build.release/QtQuickVcp.tar.gz QtQuickVcp_Linux_x64.tar.gz
