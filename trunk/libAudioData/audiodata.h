/*
    Audio Scout - audio content indexing software
    Copyright (C) 2010  D. Grant Starkweather
    
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

    David Starkweather - dstarkweather@phash.org
*/

#ifndef _AUDIODATA_H
#define _AUDIODATA_H

#include <stdint.h>

#if defined(BUILD_DLL)
#define AUDIODATA_EXPORT __declspec(dllexport)
#elif defined(BUILD_EXE)
#define AUDIODATA_EXPORT __declspec(dllimport)
#else  
#define AUDIODATA_EXPORT
#endif

typedef struct  ametadata_t {
  char *composer;
  char *title1, *title2, *title3;
  char *tpe1, *tpe2, *tpe3, *tpe4;
  char *date;
  int year;
  char *album;
  char *genre;
  int duration;
  int partofset;
} AudioMetaData;

/* errors for readaudio function */
/* if value of error is < 1000, it is an error from mpg123 (look in mpg123.h for error code) */
/* if value is 0 or >= 1000, then follow this enum */ 
AUDIODATA_EXPORT
enum ph_phashaudio_error {
  PHERR_SUCCESS = 0, 
  PHERR_NULLARG = 1000,
  PHERR_NOBUF =1001,
  PHERR_BADSR = 1002,
  PHERR_NOBUFALLOCD = 1003,
  PHERR_SRCCONTXT = 1004,
  PHERR_SRCPROC = 1005,
  PHERR_SNDFILEOPEN = 1006,
  PHERR_MEMALLOC = 1007,
  PHERR_NOSAMPLES = 1008,
  PHERR_NOFORMAT = 1009,
  PHERR_NOENCODING = 1010,
  PHERR_MP3NEW = 1011,
};
/**
 * init_mdata
 * initialize AudioMetaData struct to all zeros
 **/ 
AUDIODATA_EXPORT
void init_mdata(AudioMetaData *mdata);

/**
 * free_mdata
 * release all memory allocated to fields and zero out.
 **/ 
AUDIODATA_EXPORT
void free_mdata(AudioMetaData *mdata);

/**
 * readaudio function
 * read audio samples into buffer 
 * PARAM filename - char string of filename to read 
 * PARAM sr - int value of desired sample rate of signal
 * PARAM nbsecs - float for number of seconds to take from file - use 0.0 for the whole file.
 * PARAM sigbuf - signal buffer in which to put the extracted signal
 *                (can be NULL for internal allocation of buffer)
 * PARAM buflen - number of elements in signal buffer returned. [in][out] 
 *                [in] the maximum length of the sig buffer (if not set to null)
 *                [out] no. samples in returned buffer
 * PARAM mdata - ptr to AudioMetaData struct to be filled in by function, can be NULL.
 * PARAM error - ptr to int value of error code (0 for success)
 * RETURN ptr to float array
 **/
AUDIODATA_EXPORT
float* readaudio(const char *filename, const int sr,  const float nbsecs, float *sigbuf, int *buflen, 
                                         AudioMetaData *mdata, int *error);

/** free buffer returned by readaudio
 *  @param ptr 
 *  @return void
 **/
AUDIODATA_EXPORT
void freebuffer(void *ptr);

#endif
