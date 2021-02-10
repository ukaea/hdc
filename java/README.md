jHDC
====

Prerequisities
--------------
```
apt -y install maven
```
The following howto assumes you have installed hdc with ```-DENABLE_JAVA=ON``` option.

Building
--------
Currently the c++ header is checked into the repo but this should be generated in the build by

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
The actual build is done by:
```bash
cd jHDC
mvn package
```

Running
-------

You can run the example with (shorter way available below):

```bash
export ND4J_JARS=~/.m2/repository/org/nd4j/nd4j-native-api/1.0.0-beta7/nd4j-native-api-1.0.0-beta7.jar:~/.m2/repository/org/nd4j/nd4j-api/1.0.0-beta7/nd4j-api-1.0.0-beta7.jar:~/.m2/repository/org/nd4j/nd4j-common/1.0.0-beta7/nd4j-common-1.0.0-beta7.jar:~/.m2/repository/org/nd4j/jackson/1.0.0-beta7/jackson-1.0.0-beta7.jar:~/.m2/repository/org/nd4j/nd4j-native-platform/1.0.0-beta7/nd4j-native-platform-1.0.0-beta7.jar:~/.m2/repository/org/nd4j/protobuf/1.0.0-beta7/protobuf-1.0.0-beta7.jar:~/.m2/repository/org/nd4j/nd4j-native/1.0.0-beta7/nd4j-native-1.0.0-beta7-android-arm64.jar:~/.m2/repository/org/nd4j/nd4j-native/1.0.0-beta7/nd4j-native-1.0.0-beta7.jar:~/.m2/repository/org/nd4j/nd4j-native/1.0.0-beta7/nd4j-native-1.0.0-beta7-linux-ppc64le.jar:~/.m2/repository/org/nd4j/nd4j-native/1.0.0-beta7/nd4j-native-1.0.0-beta7-android-x86_64.jar:~/.m2/repository/org/nd4j/nd4j-native/1.0.0-beta7/nd4j-native-1.0.0-beta7-android-x86.jar:~/.m2/repository/org/nd4j/nd4j-native/1.0.0-beta7/nd4j-native-1.0.0-beta7-windows-x86_64.jar:~/.m2/repository/org/nd4j/nd4j-native/1.0.0-beta7/nd4j-native-1.0.0-beta7-linux-armhf.jar:~/.m2/repository/org/nd4j/nd4j-native/1.0.0-beta7/nd4j-native-1.0.0-beta7-macosx-x86_64.jar:~/.m2/repository/org/nd4j/nd4j-native/1.0.0-beta7/nd4j-native-1.0.0-beta7-linux-x86_64.jar:~/.m2/repository/org/nd4j/nd4j-native/1.0.0-beta7/nd4j-native-1.0.0-beta7-android-arm.jar:~/.m2/repository/org/nd4j/guava/1.0.0-beta7/guava-1.0.0-beta7.jar

export BLAS_JARS=~/.m2/repository/org/bytedeco/javacpp/1.5.3/javacpp-1.5.3.jar:~/.m2/repository/org/bytedeco/javacpp/1.5.3/javacpp-1.5.3-android-arm.jar:~/.m2/repository/org/bytedeco/javacpp/1.5.3/javacpp-1.5.3-linux-x86.jar:~/.m2/repository/org/bytedeco/javacpp/1.5.3/javacpp-1.5.3-android-x86_64.jar:~/.m2/repository/org/bytedeco/javacpp/1.5.3/javacpp-1.5.3-windows-x86.jar:~/.m2/repository/org/bytedeco/javacpp/1.5.3/javacpp-1.5.3-android-x86.jar:~/.m2/repository/org/bytedeco/javacpp/1.5.3/javacpp-1.5.3-linux-armhf.jar:~/.m2/repository/org/bytedeco/javacpp/1.5.3/javacpp-1.5.3-macosx-x86_64.jar:~/.m2/repository/org/bytedeco/javacpp/1.5.3/javacpp-1.5.3-android-arm64.jar:~/.m2/repository/org/bytedeco/javacpp/1.5.3/javacpp-1.5.3-windows-x86_64.jar:~/.m2/repository/org/bytedeco/javacpp/1.5.3/javacpp-1.5.3-ios-x86_64.jar:~/.m2/repository/org/bytedeco/javacpp/1.5.3/javacpp-1.5.3-ios-arm64.jar:~/.m2/repository/org/bytedeco/javacpp/1.5.3/javacpp-1.5.3-linux-ppc64le.jar:~/.m2/repository/org/bytedeco/javacpp/1.5.3/javacpp-1.5.3-linux-x86_64.jar:~/.m2/repository/org/bytedeco/javacpp/1.5.3/javacpp-1.5.3-linux-arm64.jar:~/.m2/repository/org/bytedeco/mkl-platform/2020.1-1.5.3/mkl-platform-2020.1-1.5.3.jar:~/.m2/repository/org/bytedeco/openblas-platform/0.3.9-1-1.5.3/openblas-platform-0.3.9-1-1.5.3.jar:~/.m2/repository/org/bytedeco/javacpp-platform/1.5.3/javacpp-platform-1.5.3.jar:~/.m2/repository/org/bytedeco/mkl/2020.1-1.5.3/mkl-2020.1-1.5.3-linux-x86_64.jar:~/.m2/repository/org/bytedeco/mkl/2020.1-1.5.3/mkl-2020.1-1.5.3-windows-x86.jar:~/.m2/repository/org/bytedeco/mkl/2020.1-1.5.3/mkl-2020.1-1.5.3-windows-x86_64.jar:~/.m2/repository/org/bytedeco/mkl/2020.1-1.5.3/mkl-2020.1-1.5.3.jar:~/.m2/repository/org/bytedeco/mkl/2020.1-1.5.3/mkl-2020.1-1.5.3-linux-x86.jar:~/.m2/repository/org/bytedeco/mkl/2020.1-1.5.3/mkl-2020.1-1.5.3-macosx-x86_64.jar:~/.m2/repository/org/bytedeco/openblas/0.3.9-1-1.5.3/openblas-0.3.9-1-1.5.3-ios-x86_64.jar:~/.m2/repository/org/bytedeco/openblas/0.3.9-1-1.5.3/openblas-0.3.9-1-1.5.3-linux-arm64.jar:~/.m2/repository/org/bytedeco/openblas/0.3.9-1-1.5.3/openblas-0.3.9-1-1.5.3-linux-x86_64.jar:~/.m2/repository/org/bytedeco/openblas/0.3.9-1-1.5.3/openblas-0.3.9-1-1.5.3-windows-x86_64.jar:~/.m2/repository/org/bytedeco/openblas/0.3.9-1-1.5.3/openblas-0.3.9-1-1.5.3-android-arm.jar:~/.m2/repository/org/bytedeco/openblas/0.3.9-1-1.5.3/openblas-0.3.9-1-1.5.3-linux-ppc64le.jar:~/.m2/repository/org/bytedeco/openblas/0.3.9-1-1.5.3/openblas-0.3.9-1-1.5.3-linux-x86.jar:~/.m2/repository/org/bytedeco/openblas/0.3.9-1-1.5.3/openblas-0.3.9-1-1.5.3-linux-armhf.jar:~/.m2/repository/org/bytedeco/openblas/0.3.9-1-1.5.3/openblas-0.3.9-1-1.5.3-ios-arm64.jar:~/.m2/repository/org/bytedeco/openblas/0.3.9-1-1.5.3/openblas-0.3.9-1-1.5.3.jar:~/.m2/repository/org/bytedeco/openblas/0.3.9-1-1.5.3/openblas-0.3.9-1-1.5.3-windows-x86.jar:~/.m2/repository/org/bytedeco/openblas/0.3.9-1-1.5.3/openblas-0.3.9-1-1.5.3-android-x86.jar:~/.m2/repository/org/bytedeco/openblas/0.3.9-1-1.5.3/openblas-0.3.9-1-1.5.3-android-x86_64.jar:~/.m2/repository/org/bytedeco/openblas/0.3.9-1-1.5.3/openblas-0.3.9-1-1.5.3-macosx-x86_64.jar:~/.m2/repository/org/bytedeco/openblas/0.3.9-1-1.5.3/openblas-0.3.9-1-1.5.3-android-arm64.jar

export MISC_JARS=~/.m2/repository/net/java/dev/jna/jna-platform/4.3.0/jna-platform-4.3.0.jar:~/.m2/repository/net/java/dev/jna/jna/4.3.0/jna-4.3.0.jar:~/.m2/repository/net/ericaro/neoitertools/1.0.0/neoitertools-1.0.0.jar:~/.m2/repository/commons-codec/commons-codec/1.10/commons-codec-1.10.jar:~/.m2/repository/classworlds/classworlds/1.1/classworlds-1.1.jar:~/.m2/repository/com/github/oshi/oshi-core/3.4.2/oshi-core-3.4.2.jar:~/.m2/repository/com/google/flatbuffers/flatbuffers-java/1.10.0/flatbuffers-java-1.10.0.jar:~/.m2/repository/com/google/code/findbugs/jsr305/2.0.1/jsr305-2.0.1.jar:~/.m2/repository/com/thoughtworks/qdox/qdox/2.0-M8/qdox-2.0-M8.jar:~/.m2/repository/com/thoughtworks/qdox/qdox/2.0-M9/qdox-2.0-M9.jar:~/.m2/repository/com/jakewharton/byteunits/byteunits/0.9.1/byteunits-0.9.1.jar:~/.m2/repository/junit/junit/4.12/junit-4.12.jar:~/.m2/repository/junit/junit/4.11/junit-4.11.jar:~/.m2/repository/commons-net/commons-net/3.1/commons-net-3.1.jar:~/.m2/repository/commons-io/commons-io/2.5/commons-io-2.5.jar:~/.m2/repository/org/iq80/snappy/snappy/0.4/snappy-0.4.jar:~/.m2/repository/org/apache/commons/commons-compress/1.11/commons-compress-1.11.jar:~/.m2/repository/org/apache/commons/commons-compress/1.18/commons-compress-1.18.jar:~/.m2/repository/org/apache/commons/commons-math3/3.5/commons-math3-3.5.jar:~/.m2/repository/org/apache/commons/commons-lang3/3.6/commons-lang3-3.6.jar:~/.m2/repository/org/hamcrest/hamcrest-core/1.3/hamcrest-core-1.3.jar:~/.m2/repository/org/codehaus/plexus/plexus-component-annotations/1.7.1/plexus-component-annotations-1.7.1.jar:~/.m2/repository/org/codehaus/plexus/plexus-component-annotations/1.6/plexus-component-annotations-1.6.jar:~/.m2/repository/org/codehaus/plexus/plexus-archiver/3.4/plexus-archiver-3.4.jar:~/.m2/repository/org/codehaus/plexus/plexus-compiler-api/2.8.4/plexus-compiler-api-2.8.4.jar:~/.m2/repository/org/codehaus/plexus/plexus-java/0.9.10/plexus-java-0.9.10.jar:~/.m2/repository/org/codehaus/plexus/plexus-interpolation/1.14/plexus-interpolation-1.14.jar:~/.m2/repository/org/codehaus/plexus/plexus-interpolation/1.24/plexus-interpolation-1.24.jar:~/.m2/repository/org/codehaus/plexus/plexus-interpolation/1.11/plexus-interpolation-1.11.jar:~/.m2/repository/org/codehaus/plexus/plexus-io/2.7.1/plexus-io-2.7.1.jar:~/.m2/repository/org/codehaus/plexus/plexus-classworlds/2.2.3/plexus-classworlds-2.2.3.jar:~/.m2/repository/org/codehaus/plexus/plexus-utils/1.5.15/plexus-utils-1.5.15.jar:~/.m2/repository/org/codehaus/plexus/plexus-utils/3.0.24/plexus-utils-3.0.24.jar:~/.m2/repository/org/codehaus/plexus/plexus-utils/2.0.4/plexus-utils-2.0.4.jar:~/.m2/repository/org/codehaus/plexus/plexus-compiler-javac/2.8.4/plexus-compiler-javac-2.8.4.jar:~/.m2/repository/org/codehaus/plexus/plexus-compiler-manager/2.8.4/plexus-compiler-manager-2.8.4.jar:~/.m2/repository/org/codehaus/plexus/plexus-container-default/1.0-alpha-9-stable-1/plexus-container-default-1.0-alpha-9-stable-1.jar:~/.m2/repository/org/threeten/threetenbp/1.3.3/threetenbp-1.3.3.jar:~/.m2/repository/org/tukaani/xz/1.5/xz-1.5.jar:~/.m2/repository/org/sonatype/aether/aether-util/1.7/aether-util-1.7.jar:~/.m2/repository/org/sonatype/aether/aether-impl/1.7/aether-impl-1.7.jar:~/.m2/repository/org/sonatype/aether/aether-spi/1.7/aether-spi-1.7.jar:~/.m2/repository/org/sonatype/aether/aether-api/1.7/aether-api-1.7.jar:~/.m2/repository/org/sonatype/plexus/plexus-cipher/1.4/plexus-cipher-1.4.jar:~/.m2/repository/org/sonatype/plexus/plexus-build-api/0.0.7/plexus-build-api-0.0.7.jar:~/.m2/repository/org/sonatype/plexus/plexus-sec-dispatcher/1.3/plexus-sec-dispatcher-1.3.jar:~/.m2/repository/org/sonatype/sisu/sisu-inject-bean/1.4.2/sisu-inject-bean-1.4.2.jar:~/.m2/repository/org/sonatype/sisu/sisu-inject-plexus/1.4.2/sisu-inject-plexus-1.4.2.jar:~/.m2/repository/org/sonatype/sisu/sisu-guice/2.1.7/sisu-guice-2.1.7-noaop.jar:~/.m2/repository/org/ow2/asm/asm/6.2/asm-6.2.jar:~/.m2/repository/org/slf4j/slf4j-jdk14/1.5.6/slf4j-jdk14-1.5.6.jar:~/.m2/repository/org/slf4j/slf4j-api/1.5.6/slf4j-api-1.5.6.jar:~/.m2/repository/org/slf4j/slf4j-api/1.7.21/slf4j-api-1.7.21.jar:~/.m2/repository/org/slf4j/jcl-over-slf4j/1.5.6/jcl-over-slf4j-1.5.6.jar:~/.m2/repository/backport-util-concurrent/backport-util-concurrent/3.1/backport-util-concurrent-3.1.jar

export HDC_PREFIX=~/projects/hdc

java -cp "$HDC_PREFIX/java/jHDC/target/jHDC-0.16.0.jar:$HDC_PREFIX/java/jHDC/target/test-classes:$ND4J_JARS:$MISC_JARS:$BLAS_JARS" -Djava.library.path=$HDC_PREFIX/build/java Example
```

Lazy people (like me) can try the following. Bear in the mind that here we assume empty *~/.m2/* before the build.
```bash
export ALL_JARS=$(find ~/.m2/repository -iname "*.jar" -printf "%p:")
java -cp "$HDC_PREFIX/java/jHDC/target/jHDC-0.16.0.jar:$HDC_PREFIX/java/jHDC/target/test-classes:$ALL_JARS" -Djava.library.path=$HDC_PREFIX/build/java Example
```

Building doc
```
mvn site
```
