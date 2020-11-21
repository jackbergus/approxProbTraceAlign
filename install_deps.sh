#!/bin/bash
# Setting up the current directory's directories
mkdir -p antlr4/cpp

# Installing the required packages
sudo apt-get install -y uuid-dev mvn cmake clang libcdt5 libcgraph6 libgraphviz-dev libgcv6 libxdot4 libgvpr2 qtdeclarative5-dev qtdeclarative5-dev-tools libeigen3-dev libopenmpi-dev

## Pulling the repositories
git submodule update --init --recursive
git submodule update --recursive --remote

# Compiling via Maven antlr4
export MAVEN_OPTS="-Xmx1G"
pushd submodules/antlr4
mvn --fail-at-end -U -DskipTests package
popd

java -jar submodules/antlr4/tool/target/antlr4-4.8-2-SNAPSHOT-complete.jar -Dlanguage=Cpp -visitor -o antlr4/cpp antlr4/*.g
