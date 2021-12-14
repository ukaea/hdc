jHDC
====

Running examples
----------------

You can run the example:
```bash
export HDC_PREFIX=~/projects/hdc
java -cp "$HDC_PREFIX/java/jHDC/target/*:$HDC_PREFIX/java/jHDC/target/test-classes:$HDC_PREFIX/java/jHDC/target/dependency/*" -Djava.library.path=$HDC_PREFIX/build/java Example
```

After installation, you can set reduced classpath:
```bash
export HDC_PREFIX=~/projects/hdc
export JHDC_PREFIX=$HDC_PREFIX/install/share/java/
java -cp "$JHDC_PREFIX/*:$HDC_PREFIX/build/java/jHDC/target/test-classes" -Djava.library.path=$HDC_PREFIX/build/java Example
```

Manual building instructions
============================
The following howto is no longer needed since cmake can now do it for you. But for documenting purposes of non-trivial build procedure of jHDC, we will keep it here. Users are strongly encouraged to see [main readme](../README.md) instead. 

This howto assumes you have built hdc with ```-DENABLE_JAVA=ON``` option


Prerequisities
--------------
```bash
apt -y install maven 
```

Building
--------
Assuming you are in cmake build directory, the C++ header should be generated in the build by
```bash
cd java
javah -classpath jHDC/src/main/java -jni dev.libhdc.HDC
```

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

Building the bundle
-------------------
Maven can also build a single jar containing all necessary dependencies, in order to do that:
```bash
mvn clean compile assembly:single 
```

Building doc
------------
```bash
mvn site
```

Running the example
-------------------
```bash
mvn clean dependency:copy-dependencies package
java -Djava.library.path=../../build/java -cp "target/jHDC-0.17.3.jar:target/dependency/*" Example
```
Alternatively for for lazy and patient:
```bash
mvn clean compile assembly:single
java -Djava.library.path=../../build/java -cp target/jHDC-0.17.3-jar-with-dependencies.jar Example
```


Installation
------------
The following command installs jHDC package into maven repository (`~/.m2` by default)
```bash
mvn install
```

