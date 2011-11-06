C# bindings to pHashAudio.dll and AudioData.dll
------------------------------------------------------

This project contain the files for building the c#
bindings to pHashAudio.dll and AudioData.dll

To build the project, simply:

   cmake -g "NMake Makefiles" .
   nmake all

The files produced should be pHashAudio-ni.dll for 
managed code interface to native bindings and a TestNI.exe
executable to test the bindings.  Run this with the
name of an audio file to test the read and hash functions.

Note: The AudioData.dll and pHashAudio.dll must be in your
PATH env variable.


DEPENDENCIES
------------------------------------------------------

.NET Framework v4.0
