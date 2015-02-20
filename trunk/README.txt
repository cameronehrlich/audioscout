AudioScout v1.1.0 by Aetilius, Inc. 2013
----------------------------------------------

AudioScout is an audio/track recognition system. Its purpose
is to recognize audio samples allowing for moderate distortion
of the audio signal. The system consists of a server which responds 
to message commands from client applications.  

Features
------------------------------------------------
* logging support with log4j
* back end krati key/hash database lookup
* a ready programming API for making client applications

----------------------------------------------
This project consists of 3 modules:

AudioScout   - server application for online storage/retrieval of audio
               fingerprints.
libAudioData - library for reading audio files.
JPHashAudio  - audio hash functions (for building client applications)

Information regarding each module can be found in the respective 
subdirectories.  Each module is an independent project using the maven
build system.  A 'mvn install' will build, test install the binaries into
your m2 maven repository. The libAudioData part depends on a c library, but
there are java bindings provided.

Install
---------------------------------------------
1a. Make the lib to read audio files.  

cd ./libAudioData
cmake .
make
make install

1b. Make the java bindings for reading audio files.

cd ./bindings/java
mvn install

2. Make the java library for audio hash functions

cd ../JPhashAudio
mvn install

3. AudioScout server application.

cd ../AudioScout
mvn package


No need to install the AudioScout server, since it is an application. Scripts to 
run the application are in the target/AudioScout/bin dir. 


dependencies
---------------------------------------------

java JRE1.7/JDK1.7 (http://www.oracle.com/technetwork/java/javase/downloads/index.html)

Zeromq 3.2 with jzmq java bindings
       (libzmq.so and libjzmq.so)
(http://www.zeromq.org/intro:get-the-software)
