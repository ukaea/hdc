# Java Wrapper

## Prerequisities
```
apt -y install maven
```


## Building

Current the c++ header is checked into the repo but this should be generated in the build by

```bash
cd java
javah -classpath jHDC/src/main/java -jni dev.libhdc.HDC
```

This needs to be done after building the Java wrapper.

The Java wrapper is built using Maven. Java 8 should be selected prior building.
The environment on ubuntu bionic can be set up by:
```bash
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
export JRE_HOME=/usr/lib/jvm/java-8-openjdk-amd64/jre
```
The actual buid is done by:
```bash
cd jHDC
mvn package
```

## Running

You can run the tests with:

```bash
export ND4J_JARS=~/.m2/repository/org/nd4j/nd4j-native-platform/0.9.1/nd4j-native-platform-0.9.1.jar:~/.m2/repository/org/nd4j/nd4j-native/0.9.1/nd4j-native-0.9.1.jar:~/.m2/repository/org/nd4j/nd4j-native/0.9.1/nd4j-native-0.9.1-linux-x86_64.jar:~/.m2/repository/org/nd4j/nd4j-native/0.9.1/nd4j-native-0.9.1-android-arm.jar:~/.m2/repository/org/nd4j/nd4j-native/0.9.1/nd4j-native-0.9.1-android-x86.jar:~/.m2/repository/org/nd4j/nd4j-native/0.9.1/nd4j-native-0.9.1-linux-ppc64le.jar:~/.m2/repository/org/nd4j/nd4j-native/0.9.1/nd4j-native-0.9.1-macosx-x86_64.jar:~/.m2/repository/org/nd4j/nd4j-native/0.9.1/nd4j-native-0.9.1-windows-x86_64.jar:~/.m2/repository/org/nd4j/nd4j-native-api/0.9.1/nd4j-native-api-0.9.1.jar:~/.m2/repository/org/nd4j/nd4j-buffer/0.9.1/nd4j-buffer-0.9.1.jar:~/.m2/repository/org/nd4j/nd4j-context/0.9.1/nd4j-context-0.9.1.jar:~/.m2/repository/org/nd4j/nd4j-common/0.9.1/nd4j-common-0.9.1.jar:~/.m2/repository/org/nd4j/jackson/0.9.1/jackson-0.9.1.jar:~/.m2/repository/org/nd4j/nd4j-api/0.9.1/nd4j-api-0.9.1.jar
export BLAS_JARS=~/.m2/repository/org/bytedeco/javacpp-presets/openblas-platform/0.2.19-1.3/openblas-platform-0.2.19-1.3.jar:~/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3.jar:~/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-android-arm.jar:~/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-android-x86.jar:~/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-linux-armhf.jar:~/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-linux-ppc64le.jar:~/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-linux-x86.jar:~/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-linux-x86_64.jar:~/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-macosx-x86_64.jar:~/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-windows-x86.jar:~/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-windows-x86_64.jar
export MISC_JARS=~/.m2/repository/org/slf4j/slf4j-api/1.7.10/slf4j-api-1.7.10.jar:~/.m2/repository/org/reflections/reflections/0.9.10/reflections-0.9.10.jar:~/.m2/repository/com/google/guava/guava/15.0/guava-15.0.jar:~/.m2/repository/org/bytedeco/javacpp/1.3.3/javacpp-1.3.3.jar:~/.m2/repository/org/apache/commons/commons-math3/3.4.1/commons-math3-3.4.1.jar:~/.m2/repository/org/javassist/javassist/3.19.0-GA/javassist-3.19.0-GA.jar:~/.m2/repository/commons-io/commons-io/2.5/commons-io-2.5.jar

export HDC_PREFIX=~/projects/hdc

java -cp "$HDC_PREFIX/java/jHDC/target/jHDC-0.14.0.jar:$HDC_PREFIX/java/jHDC/target/test-classes:$ND4J_JARS:$MISC_JARS:$BLAS_JARS" -Djava.library.path=$HDC_PREFIX/build/java Test
```
