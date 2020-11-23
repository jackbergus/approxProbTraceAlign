#!/bin/sh

###
## ProM specific
###
PROGRAM=ProM
CP=./dist/ProM-Framework-6.10.110.jar:./dist/ProM-Contexts-6.10.62.jar:./dist/ProM-Models-6.10.40.jar:./dist/ProM-Plugins-6.9.70.jar
LIBDIR=./lib
MAIN=org.processmining.contexts.uitopia.UI

####
## Environment options
###
JAVA=java-se-8u41-ri/bin/java

###
## Main program
###

add() {
	CP=${CP}:$1
}


for lib in $LIBDIR/*.jar
do
	add $lib
done

$JAVA\
 -Xmx3G\
 -da\
 -classpath ${CP}\
 -Djava.library.path=${LIBDIR}\
 -Djava.system.class.loader=org.processmining.framework.util.ProMClassLoader\
 -Djava.util.Arrays.useLegacyMergeSort=true\
 ${MAIN}
