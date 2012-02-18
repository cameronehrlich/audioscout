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

  public class PHashAudio 
  {


    /// <summary>
    ///   data struct to hold commonly used info across hash calculations.
    /// </summary>

    [StructLayout(LayoutKind.Sequential)]
    public struct AudioHashStInfo
    {
      UInt32 sr;
      UInt32 framelength;
      IntPtr window;
      IntPtr wts;
    }

    /// <summary>
    ///   Calculate audio hash for signal buffer, buf
    /// </summary>
    /// <param name="float[]">signal buffer of which to hash</param>
    /// <param name="Int32">sr, sample rate of signal, e.g. 6000 samples per second</param>
    /// <param name="AudioHashStInfo">hash_st, ref to structure to hold commonly used info across calcs.</param>
    /// <returns>Int32[] array of hash values</returns>

    public static Int32[] audiohash(float[] buf,
                                     Int32 sr,
				     ref AudioHashStInfo hash_st)
    {
      IntPtr phash = IntPtr.Zero;
      UInt32 nbcoeffs=0, nbframes=0;
      double minB=0, maxB=0;
      int err = audiohash(buf, ref phash, IntPtr.Zero, IntPtr.Zero,
                          ref nbcoeffs, ref nbframes, 
                          ref minB, ref maxB, (UInt32)buf.Length,
                          0, sr, ref hash_st);

      Int32[] hasharray = new Int32[(Int32)nbframes];
      Marshal.Copy(phash, hasharray, 0, (Int32)nbframes);
      ph_free(phash);
      return hasharray;
    }
    
    /// <summary>
    ///   aux. extern function to native library
    /// </summary>
    [DllImport("libpHashAudio.dll", CallingConvention=CallingConvention.Cdecl)]
    private extern static Int32 audiohash(float[] buf, 
                                         ref IntPtr phash, 
                                         IntPtr coeffs, IntPtr toggles,
                                         ref UInt32 nbcoeffs, 
                                         ref UInt32 nbframes, 
                                         ref double minB, 
                                         ref double maxB,
                                         UInt32 buflen, 
                                         UInt32 P, 
                                         Int32 sr, 
                                         ref AudioHashStInfo hash_st); 

    /// <summary>
    ///   aux free function to free hash array in unmanaged code.
    /// </summary>
    [DllImport("libpHashAudio.dll", CallingConvention=CallingConvention.Cdecl)]
    private extern static void ph_free(IntPtr ptr);


    /// <summary>
    ///   free AudioHashStInfo struct members after done hashing a group of files.
    /// </summary>
    /// <param name="AudioHashStInfo">hash_st, ref to AudioHashStInfo struct</param>

    [DllImport("libpHashAudio.dll", CallingConvention=CallingConvention.Cdecl)]
    public extern static void ph_hashst_free(ref AudioHashStInfo hash_st);

    /// private ctor to keep from being instantiated.  All members are static.
    private PHashAudio(){}

   }
}