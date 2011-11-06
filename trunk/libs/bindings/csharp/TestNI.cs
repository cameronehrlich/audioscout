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
using System.Text;
using PHash;

class TestPHashAudioBindings
{
  static int Main(string[] args)
  {
    if (args.Length < 1)
      {
	System.Console.WriteLine("not enough args");
	return 0;
      }
    string file = args[0];

    unsafe
      {

	System.Console.WriteLine("file: " + file);

	PHashAudio.AudioHashStInfo hashinfo;
	UInt32 *phash;
	UInt32 buflen = 0, nbframes = 0;
	int error = 0;
	void *hashptr = null;
	float *buf = AudioData.readaudio(file, 6000, null, &buflen, 0, null, &error);
	
	System.Console.WriteLine("buffer length " + buflen);

	int rc = PHashAudio.audiohash(buf, &phash, null, null, null, &nbframes, null, null, buflen, 0, 6000, &hashinfo);

	System.Console.WriteLine("hash ret code " + rc + " # frames " + nbframes);

	AudioData.audiodata_free(buf);
	PHashAudio.ph_free(phash);
	PHashAudio.ph_hashst_free(&hashinfo);
      }

    return 0;   
  }
}