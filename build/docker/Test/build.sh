#!/usr/bin/env bash
# Run this script from the projects root directory

set -e # exit on error
set -x # echo commands

docker_image=machinekoder/qtquickvcp-docker-linux-x64:latest
project=QtQuickVcp
platform=test
recipe=Linux/test/Recipe

docker pull ${docker_image}
docker rm ${project}-${platform} &> /dev/null || true
docker run --name ${project}-${platform} -i \
     -v /etc/group:/etc/group:ro -v /etc/passwd:/etc/passwd:ro \
     -u $( id -u $USER ):$( id -g $USER ) \
     -v "${PWD}:/${project}" ${docker_image} \
     /bin/bash -c "cd ${project}; ./build/${recipe}"
docker rm ${project}-${platform} &> /dev/null
