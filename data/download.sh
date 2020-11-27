#!/bin/bash
sudo apt-get install libxrender1:i386 libxtst6:i386 libxi6:i386
if [ ! -f "Sepsis Cases - Event Log.xes" ]; then
    wget -O sepsis.zip https://data.4tu.nl/ndownloader/articles/12707639/versions/1
    unzip sepsis.zip
    gzip -d "Sepsis Cases - Event Log.xes.gz"
    rm sepsis.zip
fi
echo "File 'Sepsis Cases - Event Log.xes' has been downloaded. Donwloading ProM"
if [ ! -f "ProM610.sh" ]; then
     wget https://www.promtools.org/prom6/downloads/prom-6.10-all-platforms.zip
     unzip prom-6.10-all-platforms.zip
     rm prom-6.10-all-platforms.zip
fi
echo "Preparing SplitMiner 2.0"
if [ ! -d sm2 ]; then
     wget -O sm.zip https://ndownloader.figshare.com/files/24559268
     unzip sm.zip -d ./sm2
     rm -rf sm.zip
     # How to generate the BPMN from it: java -cp sm2/sm2.jar:sm2/lib/* au.edu.unimelb.services.ServiceProvider SM2  sepsis_23000000.xes sepsis_23000000_out 0.05
fi
echo "Using OpenJDK for running ProM"
if [ ! -d java-se-8u41-ri ]; then
     wget https://download.java.net/openjdk/jdk8u41/ri/openjdk_jre_ri-8u41-b04-linux-i586-14_jan_2020.tar.gz
     tar xvzf openjdk_jre_ri-8u41-b04-linux-i586-14_jan_2020.tar.gz
     rm openjdk_jre_ri-8u41-b04-linux-i586-14_jan_2020.tar.gz
fi
chmod +x ProM610_openjdk.sh
