#!/bin/bash
sudo apt-get install libxrender1:i386 libxtst6:i386 libxi6:i386
if [ ! -f "Sepsis Cases - Event Log.xes" ]; then
    wget -O sepsis.zip wget https://data.4tu.nl/ndownloader/articles/12707639/versions/1
    unzip sepsis.zip
    gzip -d "Sepsis Cases - Event Log.xes.gz"
    rm sepsis.zip
fi
echo "File 'Sepsis Cases - Event Log.xes' has been downloaded"
if [ ! -f "ProM610.sh" ]; then
     wget https://www.promtools.org/prom6/downloads/prom-6.10-all-platforms.zip
     unzip prom-6.10-all-platforms.zip
     rm prom-6.10-all-platforms.zip
fi
if [ ! -d java-se-8u41-ri ]; then
     wget https://download.java.net/openjdk/jdk8u41/ri/openjdk_jre_ri-8u41-b04-linux-i586-14_jan_2020.tar.gz
     tar xvzf openjdk_jre_ri-8u41-b04-linux-i586-14_jan_2020.tar.gz
     rm openjdk_jre_ri-8u41-b04-linux-i586-14_jan_2020.tar.gz
fi
chmod +x ProM610_openjdk.sh
