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

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "./table-4.3.0phmodified/table.h"
#include "fft.h"
#include "phash_audio.h"
#include <stdio.h>

#ifdef __unix__
#include <dirent.h>
#include <unistd.h>
#endif


#ifndef JUST_AUDIOHASH

PHASH_EXPORT
AudioIndex open_audioindex(const char *idx_file, int add, int nbbuckets){

    int error;
    AudioIndex audio_index = NULL;
    if (add){
	audio_index = (AudioIndex)table_read(idx_file, &error);
	if (error != TABLE_ERROR_NONE){
	    audio_index = table_alloc(nbbuckets, &error);
	    if (error != TABLE_ERROR_NONE){
		return NULL;
	    }
	}
	/*adjusting table is NOT concurrent! */
	/*table_attr(audio_index, TABLE_FLAG_AUTO_ADJUST); */
    } else {
	audio_index = table_mmap(idx_file, &error);
	if (error != TABLE_ERROR_NONE){
	    audio_index = table_alloc(nbbuckets, &error);
	    if (error != TABLE_ERROR_NONE){
		return NULL;
	    }
	    table_write(audio_index, idx_file, 0755);
	    table_free(audio_index);
	    audio_index = table_mmap(idx_file, &error);
	    if (error != TABLE_ERROR_NONE){
		return NULL;
	    }
	}
    }

    return audio_index;
}

PHASH_EXPORT
int merge_audioindex(const char *dst_idxfile, const char *src_idxfile){
    /* merge table in src_idxfile into dst_idxfile - clear source */ 
    int ret = 0, err, nb_bkts, nb_entries, key_size, data_size;
    struct stat src_info;
    table_t *srctbl, *dsttbl;
    void *pkey, *pdata;
    table_linear_t linear_st;

    if (!dst_idxfile || !src_idxfile) return -1;
    stat(src_idxfile, &src_info);

    if (!stat(src_idxfile, &src_info) && src_info.st_size > 0) {
	/* open source  table */
	srctbl = open_audioindex(src_idxfile, 1, 0);
	if (srctbl == NULL) return -3;
	table_attr(srctbl, 0);

	err = table_info(srctbl, &nb_bkts, &nb_entries);
	
	if (nb_entries > 0){
	    /* open destination table */
	    dsttbl = open_audioindex(dst_idxfile, 1, 0);
	    if (dsttbl == NULL) {
		table_free(srctbl);
		return -2;
	    }
	    table_attr(dsttbl, 0);

	    /* do merge */
	    err = table_first_r(srctbl, &linear_st, &pkey, &key_size, &pdata, &data_size);
	    while (err == TABLE_ERROR_NONE){
		err = table_insert_kd(dsttbl, pkey, key_size, pdata, data_size, NULL, NULL, 0);
		if (err != TABLE_ERROR_NONE && err != TABLE_ERROR_OVERWRITE){
		    ret = -4;
		    break;
		}
		err = table_next_r(srctbl, &linear_st, &pkey, &key_size, &pdata,&data_size);
	    }
	    table_clear(srctbl);
	    table_write(srctbl, src_idxfile, 0755);
	    table_write(dsttbl, dst_idxfile, 0755);
	    table_free(dsttbl);
	}
	table_free(srctbl);
    } else {
	ret = 1;
    }

    return ret;
}

PHASH_EXPORT
int close_audioindex(AudioIndex audioindex, int add){
    int error;
    if (add){
	error = table_free((table_t*)audioindex);
    } else {
	error = table_munmap((table_t*)audioindex);
    }
    if (error != TABLE_ERROR_NONE){
	return -1;
    }
    return 0;
}

PHASH_EXPORT
int insert_into_audioindex(AudioIndex audio_index, uint32_t id, uint32_t *hash, int nbframes){
	const char ovrwrt = 0;
	TableValue entry;
	int err, i;

	entry.id = id;
	for (i=0;i<nbframes;i++){
	    entry.pos = (uint32_t)i;
	    err = table_insert_kd((table_t*)audio_index,&(hash[i]),sizeof(uint32_t), \
                                    &entry,sizeof(TableValue),\
                                    NULL, NULL, ovrwrt);
	    if (err != TABLE_ERROR_NONE && err != TABLE_ERROR_OVERWRITE){
		return -1;
	    }
	}
	return 0;
}

PHASH_EXPORT
int stat_audioindex(AudioIndex audio_index, int *nbbuckets, int *nbentries){
    table_info((table_t*)audio_index, nbbuckets, nbentries);
    return 0;
}

PHASH_EXPORT
int flush_audioindex(AudioIndex audio_index, const char *filename){
    int error = table_write((table_t*)audio_index, filename, 00755);
    if (error != TABLE_ERROR_NONE){
	return -1;
    }
    return 0;
}

PHASH_EXPORT
int grow_audioindex(AudioIndex audio_index, const float load){
    int nbbuckets, nbentries, error;
    double current_load;

    stat_audioindex(audio_index, &nbbuckets, &nbentries);
    current_load = (double)nbentries/(double)nbbuckets;
    if (current_load > load) {
	error = table_adjust((table_t*)audio_index, 0); /* adjust to number of entries */ 
    }

    return 0;
}

#endif /* JUST_AUDIOHASH */ 

#ifndef _WIN32

PHASH_EXPORT
char** readfilenames(const char *dirname,unsigned int *nbfiles){

    struct dirent *dir_entry;
    DIR *dir = opendir(dirname);
    if (!dir){
        return NULL;
    }

    /*count files */
    /* counts all entries as file names except for . and .. */
    /* so far, no other way to test if an entry is a dir or a file */
    /* so, some names returned could still be directories  */ 
    *nbfiles = 0;
    while ((dir_entry = readdir(dir)) != NULL){
      if ( strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name,"..")){
	    (*nbfiles)++;
	}
    }

    /* alloc list of files */
    char **files = (char**)malloc((*nbfiles)*sizeof(char*));
    if (!files)
	return NULL;

    int index = 0;
    char path[FILENAME_MAX];
    path[0] = '\0';
    rewinddir(dir);
  
    /* read in files names */ 
    while ((dir_entry = readdir(dir)) != 0){
      if ( strcmp(dir_entry->d_name,".") && strcmp(dir_entry->d_name,"..")){
	    strncat(path, dirname, strlen(dirname));
	    strncat(path,SEPARATOR , 1);
	    strncat(path, dir_entry->d_name, strlen(dir_entry->d_name));
	    files[index++] = strdup(path);
	    path[0]='\0';
       }
    }

    closedir(dir);
    return files;
}

#endif /* _WIN32 */

static
void sort_barkdiffs(double *barkdiffs,uint8_t *bits,unsigned int length){
    int i,j, min;
    double tmpd;
    uint8_t tmpb;
    for (i=0;i<(int)length;i++){
	min = i;
	for (j=i+1;j<(int)length;j++){
	    if (barkdiffs[j] < barkdiffs[min])
		min = j;
	}
	if (i != min){
	    tmpd = barkdiffs[i];

	    barkdiffs[i] = barkdiffs[min];
	    barkdiffs[min] = tmpd;

	    tmpb = bits[i];
	    bits[i] = bits[min];
	    bits[min] = tmpb;
	}
    }
}


static const unsigned int nfilts = 33;


static const double barkwidth = 1.06;

static const double barkfreqs[33] = { 50.0, 100.0,  150.0, 200.0, 250.0,\
                                          300.0, 350.0, 400.0, 450.0, 470.0, 510.0, 570.0,\
                                          635.0, 700.0, 770.0, 840.0, 920.0, 1000.0,\
                                          1085.0, 1170.0, 1270.0, 1370.0, 1485.0,\
                                          1600.0, 1725.0, 1850.0, 2000.0, 2150.0,\
				      2325.0, 2500.0, 2700.0, 2900.0, 3000.0 }; 

PHASH_EXPORT
void ph_free(void * ptr){
  free(ptr);
}


PHASH_EXPORT
void ph_hashst_free(AudioHashStInfo *ptr){
  unsigned int i;
  if (ptr->window){
    free(ptr->window);
    ptr->window = NULL;
  }
  if (ptr->wts){
    for (i = 0; i < nfilts; i++){
      free(ptr->wts[i]);
    }
    free(ptr->wts);
  }
  ptr->sr = 0;
  ptr->framelength = 0;
}

PHASH_EXPORT
int audiohash(float *buf, uint32_t **phash, double ***coeffs, uint8_t ***bit_toggles,\
              unsigned int *nbcoeffs, unsigned int *nbframes, double *minB, double *maxB,\
              unsigned int buflen, unsigned int P, int sr, AudioHashStInfo *hash_st){
  int index;
  unsigned int i, j, m, nfft, nfft_half, N, framelength, start, end, overlap, advance;
  const double maxfreq = 3000;
  double *binbarks, temp, lof, hif, bark_diff, f_bark_mid;
  double *window, **wts, *frame, *magnF, *barkdiffs, **barkcoeffs;
  double max_bark, min_bark, maxF, H, mdouble;
  PHComplex *pF;
  uint8_t *tmptoggles;
  uint32_t *hash, hashtmp;

  if (buf == NULL || phash == NULL || nbframes == NULL || hash_st == NULL) return -1;

  if (sr != hash_st->sr){ 
      hash_st->framelength = 2048;
      hash_st->sr = sr;
      hash_st->window = (double*)malloc((hash_st->framelength)*sizeof(double));
      if (hash_st->window == NULL) return -1;
      
      for (i = 0;i<hash_st->framelength;i++){
	  /*hamming window*/
	  hash_st->window[i] = 0.54 - 0.46*cos(2*PI*i/(hash_st->framelength-1));
      }

      nfft_half = hash_st->framelength/2;
      binbarks = (double*)malloc(nfft_half*sizeof(double));
      if (binbarks == NULL) return -1;
      
      for (i=0; i < nfft_half;i++){
	  temp = i*maxfreq/nfft_half/600.0;
	  binbarks[i] = 6*log(temp + sqrt(temp*temp + 1.0));
      }
    
      hash_st->wts = (double**)malloc(nfilts*sizeof(double*));
      for (i=0;i < nfilts;i++){
	  hash_st->wts[i] = (double*)malloc(nfft_half*sizeof(double));
	  /*calculate wts for each filter */
	  f_bark_mid = barkfreqs[i]/600.0;
	  f_bark_mid = 6*log(f_bark_mid + sqrt(f_bark_mid*f_bark_mid + 1.0));
	  for (j=0;j < nfft_half ;j++){
	      bark_diff = binbarks[j] - f_bark_mid;
	      lof = -2.5*(bark_diff/barkwidth - 0.5);
	      hif = bark_diff/barkwidth + 0.5;
	      mdouble = lof < hif ? lof : hif;
	      mdouble = (mdouble < 0) ? mdouble : 0; 
	      mdouble = pow(10,mdouble);
	      hash_st->wts[i][j] = mdouble;
	  }
      } 
      free(binbarks);
  }

  N = buflen; 
  framelength = hash_st->framelength;
  nfft = framelength;
  nfft_half = (hash_st->framelength)/2;
  start = 0;
  end = start + hash_st->framelength - 1;
  overlap = 31*(hash_st->framelength)/32;
  advance = hash_st->framelength - overlap;

  *nbframes = (unsigned int)(floor(N/advance) - floor(framelength/advance) + 1);
  
  window = hash_st->window;
  wts = hash_st->wts;

  frame = (double*)malloc((framelength)*sizeof(double));
  pF = (PHComplex*)malloc(sizeof(PHComplex)*(nfft));
  magnF = (double*)malloc((nfft_half)*sizeof(double));


  if (coeffs && nbcoeffs) *nbcoeffs = nfilts;

  barkdiffs = (double*)malloc((nfilts-1)*sizeof(double));
  tmptoggles = (uint8_t*)malloc((nfilts-1)*sizeof(uint8_t));
  if (P > 0 && bit_toggles){
    *bit_toggles = (uint8_t**)malloc((*nbframes)*sizeof(uint8_t*));
    for (j=0;j < *nbframes;j++){
      (*bit_toggles)[j] = (uint8_t*)malloc(P*sizeof(uint8_t));
    }
  }

  barkcoeffs = (double**)malloc((*nbframes)*sizeof(double*));
  for (i=0;i < *nbframes;i++){
    barkcoeffs[i] = (double*)malloc((nfilts)*sizeof(double));
  }
  if (coeffs && nbcoeffs) *coeffs = barkcoeffs;

   index = 0;
   max_bark = 0.0;
   min_bark = 100000000.0;
   maxF = 0.0;
   while (end < N){
       maxF = 0.0;
       for (i = 0;i<framelength;i++){
	   frame[i] = window[i]*buf[start+i];
       }
       if (fft(frame, framelength, pF) < 0){
	   return -1;
       }
       for (i=0; i < nfft_half;i++){
           magnF[i] = complex_abs(pF[i]);
	   if (magnF[i] > maxF){
	       maxF = magnF[i];
	   }
       }

       for (i=0;i<nfilts;i++){

	   barkcoeffs[index][i] = 0.0;
	   for (j=0;j < nfft_half;j++){
	       barkcoeffs[index][i] += wts[i][j]*magnF[j];
	   }
           if (barkcoeffs[index][i] > max_bark){
	       max_bark = barkcoeffs[index][i];
	   }
	   if (barkcoeffs[index][i] < min_bark){
	       min_bark = barkcoeffs[index][i];
	   }
       }
       index += 1;
       start += advance;
       end   += advance;
   }

   if (minB) *minB = min_bark;
   if (maxB) *maxB = max_bark;

    hash = (uint32_t*)malloc((*nbframes)*sizeof(uint32_t));
    *phash = hash;
    for (i=0;i < *nbframes;i++){
	hashtmp = 0;
	for (m=0;m < nfilts-1;m++){
	    if (i > 0){
		H = (barkcoeffs[i][m] - barkcoeffs[i][m+1]) - (barkcoeffs[i-1][m] - barkcoeffs[i-1][m+1]); 
	    } else {
		H = barkcoeffs[i][m] - barkcoeffs[i][m+1];
	    }
	    barkdiffs[m] = H;
	    tmptoggles[m] = m;
	    hashtmp <<= 1;
	    if ((barkcoeffs[i][m] > 100.0 || barkcoeffs[i][m+1] > 100.0) && 
                                      abs(H) > 10.0 && H > 0.0){
		hashtmp |= 0x00000001;
	    }
	}
    
        if (bit_toggles){
	    sort_barkdiffs(barkdiffs,tmptoggles,nfilts-1);
	    for (m=0;m<P;m++){
		(*bit_toggles)[i][m] = tmptoggles[m];
	    }
	}
	hash[i] = hashtmp;
   }

    if (coeffs == NULL){
	for (i=0;i < *nbframes; i++){
	    free(barkcoeffs[i]);
	}
	free(barkcoeffs);
    }

    free(tmptoggles);
    free(barkdiffs);
    free(frame);
    free(magnF);
    free(pF);

    return 0;
}

#ifndef JUST_AUDIOHASH

static
uint32_t* get_candidates(uint32_t val, uint8_t *toggles, const unsigned int P,int *nbcands){
    unsigned i, curr_i, curr_candidate;
    int bitnum;

    uint32_t *cands = (uint32_t*)malloc((*nbcands)*sizeof(uint32_t));

    *nbcands = 1<<P;
    cands[0] = val;
    for (i=1;i < (unsigned int)*nbcands;i++){
	bitnum=0;
	curr_candidate = val;

	curr_i = i;
	while (curr_i) {
	    if (curr_i & 0x00000001){
		curr_candidate = TOGGLE_BIT(curr_candidate, toggles[bitnum]);
	    }

	    curr_i >>= 1;
	    bitnum++;
	}
	cands[i] = curr_candidate;
    }
    return cands;
}

PHASH_EXPORT
int lookupaudiohash(AudioIndex index_table,uint32_t *hash,uint8_t **toggles, int nbframes,\
                    int P, int blocksize,float threshold, uint32_t *id, float *cs){

    int max_results = 3*blocksize, nbresults = 0, max_cnt = 0, max_pos = 0, total = 0, error = 0;
    int i,j,k,m, nbcandidates, already_added;
    uint32_t *results = (uint32_t*)malloc(max_results*sizeof(uint32_t));
    uint32_t *last_positions = (uint32_t*)malloc(max_results*sizeof(uint32_t));
    uint32_t *subhash, *candidates;
    uint8_t *curr_toggles;
    int *result_cnts = (int*)malloc(max_results*sizeof(int));
    float lvl;
    TableValue *lookup_val;

    if (results == NULL || last_positions == NULL || result_cnts == NULL){
	return -1;
    }

    for (i=0;i<nbframes-blocksize+1;i+=blocksize){
	subhash = hash+i;
	for (j=0;j<blocksize;j++){
	    curr_toggles = (toggles) ? toggles[i+j] : NULL;
	    candidates = get_candidates(subhash[j], curr_toggles, P, &nbcandidates);
	    lookup_val;
	    for (k = 0;k < nbcandidates; k++){
		lookup_val = NULL;
		error = table_retrieve((table_t*)index_table, &candidates[k],sizeof(uint32_t),\
                                           (void*)&lookup_val,NULL); 
		if (lookup_val != NULL){
		    already_added = 0;
		    for (m=0;m<nbresults;m++){
			if (results[m] == lookup_val->id &&\
			    lookup_val->pos > last_positions[m] &&\
                            lookup_val->pos <= last_positions[m]+ 2*blocksize ){
			    result_cnts[m]++;
			    last_positions[m] = lookup_val->pos;
			    total++;
			    if (result_cnts[m] > max_cnt){
				max_cnt = result_cnts[m];
				max_pos = m;
			    }
			    already_added = 1;
			    break;
			}
		    }
		    if (!already_added && nbresults < max_results){
			results[nbresults] = (uint32_t)lookup_val->id;
			last_positions[nbresults] = (uint32_t)lookup_val->pos;
			result_cnts[nbresults] = 1;
			total++;
			if (result_cnts[nbresults] > max_cnt){
			    max_cnt = result_cnts[nbresults];
			    max_pos = nbresults;
			}
			nbresults++;
		    }
		}
	    }

	    free(candidates);

	}
	lvl = (float)max_cnt/(float)blocksize;
	if (lvl >= threshold){
	    break;
	}
    }

    if (nbresults > 0 && lvl >= threshold){
	*id = results[max_pos];
	*cs = lvl;
    } else {
	*id = 0;
	*cs = -1.0f;
    }
    
    free(results);
    free(result_cnts);
    free(last_positions);

    return 0;
}

#endif /* JUST_AUDIOHASH*/
