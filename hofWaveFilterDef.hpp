
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.

*********************************************************************************************/

#ifndef __HOFWAVEFILTERDEF_H__
#define __HOFWAVEFILTERDEF_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <getopt.h>


/* Local Includes. */

#include "nvutility.h"
#include "pfm.h"
#include "FileHydroOutput.h"
#include "FileWave.h"


#include <QtCore>
#include <QSharedMemory>


#define HWF_APD_SIZE  201
#define HWF_PMT_SIZE  501


typedef struct
{
  int32_t     pfm_file;
  int32_t     orig_rec;
  int32_t     rec;
} SORT_REC;


typedef struct
{
  double      mx;                        //  X position in meters
  double      my;                        //  Y position in meters
  uint8_t     check;                     //  Set if we need to check adjacent waveforms (e.g. this is an isolated point)
  int32_t     bot_bin_first;
  int32_t     bot_bin_second;
  uint8_t     apd[HWF_APD_SIZE];
  uint8_t     pmt[HWF_PMT_SIZE];
} WAVE_DATA;


typedef struct
{
  int32_t     count;
  int32_t     *data;
} BIN_DATA;


// General stuff.

typedef struct
{
  QSharedMemory *abeShare;                //  ABE's shared memory pointer.
  ABE_SHARE   *abe_share;                 //  Pointer to the ABE_SHARE structure in shared memory.
  QSharedMemory *dataShare;               //  Point cloud shared memory.
  POINT_CLOUD *data;                      //  Pointer to POINT_CLOUD structure in point cloud shared memory.  To see what is in the 
                                          //  POINT_CLOUD structure please see the ABE.h file in the nvutility library.
  int32_t     *points;                    //  Points within the search radius
  int32_t     point_count;                //  Number of points within search radius
  double      radius;
  int32_t     search_width;
  int32_t     rise_threshold;


  //  The following concern PFMs as layers.  There are a few things from ABE_SHARE that also need to be 
  //  addressed when playing with layers - open_args, display_pfm, and pfm_count.

  NV_F64_XYMBR total_mbr;                 //  MBR of all of the displayed PFMs
  int32_t     pfm_handle[MAX_ABE_PFMS];   //  PFM file handle
} MISC;


#endif
