/*      
	Audio Scout - audio content indexing software      
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

  unsafe public class PHashAudio 
  {

    /// <summary>
    ///   struct to keep state information while hashing values.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct AudioHashStInfo
    {
      UInt32 sr;
      UInt32 framelength;
      double *window;
      double **wts;
    }

    /// <summary>
    ///   calculate audio hash for given signal
    /// </summary>
    /// <param name="float*">buf, signal buffer (from readaudio) </param>
    /// <param name="UInt32**">phash, reference to a UInt32 ptr in which to return array of hash values</param>
    /// <param name="Double***">coeffs     null)</param>
    /// <param name="Byte***">bit_toggles  null</param>
    /// <param name="UInt32*">nbcoeffs     null</param>
    /// <param name="UInt32*">nbframes, ptr to unsigned int, in which to length of hash array.</param>
    /// <param name="double*">minB,        null</param>
    /// <param name="double*">maxB,        null</param>
    /// <param name="UInt32">buflen, length of signal buffer, buf (from readaudio function)</param>
    /// <param name="UInt32">P, number toggles,  keep 0</param>
    /// <param name="int">sr, sample rate of signal (same as passed to readaudio)</param>
    /// <param name="AudioHashStInfo*">hashstinfo, reference to struct, treat as opaque pointer.</param>
    /// <returns>int value, 0 for success, negative for error</returns>

    [DllImport("pHashAudio.dll")]
    public extern static int audiohash(float *buf, UInt32 **phash, double ***coeffs, Byte ***bit_toggles,
                                       UInt32 *nbcoeffs, UInt32 *nbframes, double *minB, double *maxB,
				       UInt32 buflen, UInt32 P, int sr,  AudioHashStInfo *hashstinfo);


    /// <summary>
    ///   free struct AudioHashStInfo after finished with all hashing
    /// </summary>
    /// <param name="void*">ptr to struct AudioHashStInfo</param>
    /// <returns> void </returns>

    [DllImport("pHashAudio.dll")]
    public extern static void ph_hashst_free(void *ptr);


    /// <summary>
    ///   free any memory allocated by audiohash function - i.e. the array of hash values.
    /// </summary>
    /// <param name="void*">pointer </param>
    /// <returns> void </returns>

    [DllImport("pHashAudio.dll")]
    public extern static void ph_free(void *ptr);

    /// <summary>
    ///   private ctor to keep from instantiation,  all members are static.
    /// </summary>
    private PHashAudio(){}

   }
  
}