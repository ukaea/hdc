jHDC
====

Prerequisities
--------------
```bash
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
mvn clean package
```
if you want also all dependencies to be copied into `target/dependency` folder, run:
```bash
mvn clean dependency:copy-dependencies package
```

Inslallation
------------
The following command installs jHDC package into maven repository (`~/.m2` by default)
```bash
mvn install
```

Running
-------

You can run the example:
```bash
export HDC_PREFIX=~/projects/hdc
java -cp "$HDC_PREFIX/java/jHDC/target/jHDC-0.16.0.jar:$HDC_PREFIX/java/jHDC/target/test-classes:$HDC_PREFIX/java/jHDC/target/dependency/*" -Djava.library.path=$HDC_PREFIX/build/java Example
```

Building doc
------------
```bash
mvn site
```

Building the bundle
-------------------
Maven can also build a single jar containing all necessary dependencies, in order to do that:
```bash
mvn clean compile assembly:single 
```
In this case, the tests can be run as follows:
```bash
export HDC_PREFIX=~/projects/hdc
java -cp "$HDC_PREFIX/java/jHDC/target/jHDC-0.16.0-jar-with-dependencies.jar:$HDC_PREFIX/java/jHDC/target/test-classes" -Djava.library.path=$HDC_PREFIX/build/java Example
```
The size of the bundle can be reduced by striping unnecessary native libraries:
```bash
mvn clean compile assembly:single -Djavacpp.platform=linux-x86_64
```

