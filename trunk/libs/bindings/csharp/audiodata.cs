/*      Audio Scout - audio content indexing software      
        Copyright (C) 2010  D. Grant Starkweather & Evan Klinger            
      	Audio Scout is free software: you can redistribute it and/or modify      
 	it under the terms of the GNU General Public License as published by      
	the Free Software Foundation, either version 3 of the License, or      
	(at your option) any later version.        

	This program is distributed in the hope that it will be useful,      
	but WITHOUT ANY WARRANTY; without even the implied warranty of      
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      
	GNU General Public License for more details.        
	You should have received a copy of the GNU General Public License      
	along with this program.  If not, see <http://www.gnu.org/licenses/>.        

	D. Grant Starkweather - dstarkweather@phash.org      
	Evan Klinger          - eklinger@phash.org  

*/
  
using System;
using System.Runtime.InteropServices;

namespace PHash
{

  unsafe public class AudioData
  {
    [StructLayout(LayoutKind.Sequential)]
    public struct AudioMetaData
    {
      public char *composer;
      public char *title1;
      public char *title2;
      public char *title3;

      public char *tpe1;
      public char *tpe2;
      public char *tpe3;
      public char *tpe4;

      public char *date;
      public int year;
      public char *album;
      public char *genre;
      public int duration;
      public int partofset;
    }


    /// <summary>
    ///   Read in an audio file and return a buffer of samples at given samplerate, sr
    /// </summary>
    /// <param name="strign">name of file</param>
    /// <param name="Int32">sr samplerate of samples, e.g. 6000 samples per sec</param>
    /// <param name="float*">buf pre-allocated buffer of floats in which to put the signal, can be null,
    ///                      in which case a buffer is allocated by the function.</param>
    /// <param name="UInt32*">&buflen, reference to unsigned int to return the length of the buffer.</param>
    /// <param name="float">nbsecs, number of seconds to read from the file, 0 for complete file.</param>
    /// <param name="AudioMetaData*">*mdata, reference to struct AudioMetaData to return metadata in file.</param>
    /// <param name="int">*error, pointer to int to indicate error code in case of error.</param>
    /// <returns>float ptr to a buffer of samples </returns>

    [DllImport("AudioData.dll", CharSet=CharSet.Ansi)]
    public extern static float* readaudio(String filename, int sr, float *buf, UInt32 *buflen,
                                          float nbsecs, AudioMetaData *mdata, int *error);

    /// <summary>
    ///   initialize the struct AudioMetaData parameter
    /// </summary>
    /// <param name="AudoMetadata*">mdata, ref to AudioMetaData struct</param>
    /// <returns> void </returns>

    [DllImport("AudioData.dll")]
    public extern static void init_mdata(AudioMetaData *mdata);

    /// <summary>
    ///   free the elements in the audiometadata struct.
    /// </summary>
    /// <param name="AudioMetaData*">mdata, ref to AudioMetaData struct</param>
    /// <returns> void </returns>

    [DllImport("AudioData.dll")]
    public extern static void free_mdata(AudioMetaData *mdata);

    /// <summary>
    ///   free signal buffer allocated in readaudio function.
    /// </summary>
    /// <param name="void*">ptr</param>
    /// <returns>void</return>

    [DllImport("AudioData.dll")]
    public extern static void audiodata_free(void *ptr);

    /// <summary>
    ///   private ctor to keep from being instantiated, all methods are static
    /// </summary>
    private AudioData(){ }
  }
}
