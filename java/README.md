# Java Wrapper

## Building

Current the c++ header is checked into the repo but this should be generated in the build by

```bash
javah -jni dev.libhdc.HDC
```

This needs to be done after building the Java wrapper.

The Java wrapper is built using Maven:

```bash
cd jHDC
mvn package
```

## Running

You can run the tests with:

```bash
export ND4J_JARS=/home/jhollocombe/.m2/repository/org/nd4j/nd4j-native-platform/0.9.1/nd4j-native-platform-0.9.1.jar:/home/jhollocombe/.m2/repository/org/nd4j/nd4j-native/0.9.1/nd4j-native-0.9.1.jar:/home/jhollocombe/.m2/repository/org/nd4j/nd4j-native/0.9.1/nd4j-native-0.9.1-linux-x86_64.jar:/home/jhollocombe/.m2/repository/org/nd4j/nd4j-native/0.9.1/nd4j-native-0.9.1-android-arm.jar:/home/jhollocombe/.m2/repository/org/nd4j/nd4j-native/0.9.1/nd4j-native-0.9.1-android-x86.jar:/home/jhollocombe/.m2/repository/org/nd4j/nd4j-native/0.9.1/nd4j-native-0.9.1-linux-ppc64le.jar:/home/jhollocombe/.m2/repository/org/nd4j/nd4j-native/0.9.1/nd4j-native-0.9.1-macosx-x86_64.jar:/home/jhollocombe/.m2/repository/org/nd4j/nd4j-native/0.9.1/nd4j-native-0.9.1-windows-x86_64.jar:/home/jhollocombe/.m2/repository/org/nd4j/nd4j-native-api/0.9.1/nd4j-native-api-0.9.1.jar:/home/jhollocombe/.m2/repository/org/nd4j/nd4j-buffer/0.9.1/nd4j-buffer-0.9.1.jar:/home/jhollocombe/.m2/repository/org/nd4j/nd4j-context/0.9.1/nd4j-context-0.9.1.jar:/home/jhollocombe/.m2/repository/org/nd4j/nd4j-common/0.9.1/nd4j-common-0.9.1.jar:/home/jhollocombe/.m2/repository/org/nd4j/jackson/0.9.1/jackson-0.9.1.jar:/home/jhollocombe/.m2/repository/org/nd4j/nd4j-api/0.9.1/nd4j-api-0.9.1.jar
export BLAS_JARS=/home/jhollocombe/.m2/repository/org/bytedeco/javacpp-presets/openblas-platform/0.2.19-1.3/openblas-platform-0.2.19-1.3.jar:/home/jhollocombe/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3.jar:/home/jhollocombe/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-android-arm.jar:/home/jhollocombe/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-android-x86.jar:/home/jhollocombe/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-linux-armhf.jar:/home/jhollocombe/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-linux-ppc64le.jar:/home/jhollocombe/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-linux-x86.jar:/home/jhollocombe/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-linux-x86_64.jar:/home/jhollocombe/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-macosx-x86_64.jar:/home/jhollocombe/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-windows-x86.jar:/home/jhollocombe/.m2/repository/org/bytedeco/javacpp-presets/openblas/0.2.19-1.3/openblas-0.2.19-1.3-windows-x86_64.jar
export MISC_JARS=/home/jhollocombe/.m2/repository/org/slf4j/slf4j-api/1.7.10/slf4j-api-1.7.10.jar:/home/jhollocombe/.m2/repository/org/reflections/reflections/0.9.10/reflections-0.9.10.jar:/home/jhollocombe/.m2/repository/com/google/guava/guava/15.0/guava-15.0.jar:/home/jhollocombe/.m2/repository/org/bytedeco/javacpp/1.3.3/javacpp-1.3.3.jar:/home/jhollocombe/.m2/repository/org/apache/commons/commons-math3/3.4.1/commons-math3-3.4.1.jar:/home/jhollocombe/.m2/repository/org/javassist/javassist/3.19.0-GA/javassist-3.19.0-GA.jar:/home/jhollocombe/.m2/repository/commons-io/commons-io/2.5/commons-io-2.5.jar

java -cp "$HOME/hdc/java/jHDC/target/jHDC-1.0-SNAPSHOT.jar:$HOME/hdc/java/jHDC/target/test-classes:$ND4J_JARS:$MISC_JARS:$BLAS_JARS" -Djava.library.path=$HOME/hdc/build/java Test
```