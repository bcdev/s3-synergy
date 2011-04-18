export JAVA_HOME=/usr/lib/jvm/java-6-sun
export SAFE_HOME=/usr/local/safe-suite
export OS_ARCH=amd64

if [ -n ${CLASSPATH} ]
then
  export CLASSPATH=${SAFE_HOME}/etc:${CLASSPATH}
else
  export CLASSPATH=${SAFE_HOME}/etc
fi
for jar in ${SAFE_HOME}/lib/java/*.jar
do
  export CLASSPATH=${CLASSPATH}:$jar
done

if [ -n ${LD_LIBRARY_PATH} ]
then
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${JAVA_HOME}/jre/lib/${OS_ARCH}
else
  export LD_LIBRARY_PATH=${JAVA_HOME}/jre/lib/${OS_ARCH}
fi
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${JAVA_HOME}/jre/lib/${OS_ARCH}/client
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${JAVA_HOME}/jre/lib/${OS_ARCH}/server
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${SAFE_HOME}/lib/c++/LINUX
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib

echo $PATH
echo ##################
echo $LD_LIBRARY_PATH
echo ##################
echo $CLASSPATH