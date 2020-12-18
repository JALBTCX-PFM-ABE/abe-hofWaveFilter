
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

#include "hofWaveFilter.hpp"


/***************************************************************************\
*                                                                           *
*   Module Name:        pmt_return_filter                                   *
*                                                                           *
*   Programmer(s):      Jan C. Depner                                       *
*                                                                           *
*   Date Written:       January 2009                                        *
*                                                                           *
*   Purpose:            If the return is "insignificant" based on the       *
*                       leading slope, the trailing slope, and the run      *
*                       count then it will be marked as invalid.  Default   *
*                       value for the slopes was empirically derived by     *
*                       examining shitloads (that's a technical term) of    *
*                       data.                                               *
*                                                                           *
*   Arguments:          rec            - record number (used for debugging) *
*                       sub_rec        - sub_record number (0 is primary,   *
*                                        1 is secondary).                   *
*                       hof_record     - the HOF record                     *
*                       run_req        - run length required (default is 6) *
*                       slope_req      - required significant slope,        *
*                                        if the slope or backslope is less  *
*                                        than this the hit is insignificant *
*                       ac_zero_offset - AC zero offset                     *
*                       ac_off_req     - points selected less than this     *
*                                        value above the AC zero offset     *
*                                        will be marked invalid             *
*                                                                           *
*   Return Value:       uint8_t        - NVTrue if we need to kill the      *
*                                        return                             *
*                                                                           *
\***************************************************************************/

uint8_t pmt_return_filter (int32_t rec __attribute__ ((unused)), int32_t sub_rec, HYDRO_OUTPUT_T *hof_record, int32_t pmt_run_req,
                           float slope_req, int32_t ac_zero_offset, int32_t ac_off_req, WAVE_DATA_T *wave_rec)
{
  //  Make sure the return we're looking for is not shallow water algorithm, shoreline depth swapped, or land.

  if ((sub_rec == 0 && (hof_record->abdc == 72 || hof_record->abdc == 74 || hof_record->abdc == 70)) ||
      (sub_rec == 1 && (hof_record->sec_abdc == 72 || hof_record->sec_abdc == 74 || hof_record->sec_abdc == 70))) return (NVFalse);


  //  Shorthand ;^)

  int32_t bin = hof_record->bot_bin_first;
  if (sub_rec == 0)
    {
      bin = hof_record->bot_bin_first;
    }
  else
    {
      bin = hof_record->bot_bin_second;
    }


  //  Check the AC zero offset (don't do the check if the required offset is set to 0).

  if (ac_off_req && (wave_rec->pmt[bin] - ac_zero_offset < ac_off_req)) return (NVTrue);


  //  Initialize the run and slope variables for the PMT data.

  int32_t rise = 0;
  int32_t drop = 0;
  int32_t start_data = 0;
  int32_t end_data = 0;
  int32_t peak = 0;
  int32_t run = 0;
  int32_t back_run = 0;
  float slope = 0.0;
  float backslope = 0.0;


  //  Initialize the first_drop index.  We don't want to start searching for runs until we've cleared the surface
  //  return.  This is not how Optech does it but I'm not really interested in very shallow water for this filter.

  int32_t first_drop = 0;


  //  Loop through the PMT data looking for the first drop (from the surface).  Skip the first 20
  //  bins so that we don't start looking in the noisy section prior to the surface return.

  for (int32_t i = 20 ; i < HWF_PMT_SIZE ; i++)
    {
      //  If we get three zeros in a row we want to reset the drop counter.

      int32_t change = (wave_rec->pmt[i] - wave_rec->pmt[i - 1]) + (wave_rec->pmt[i - 1] - wave_rec->pmt[i - 2]) +
        (wave_rec->pmt[i - 2] - wave_rec->pmt[i - 3]);

      if (!change) drop = 0;


      if (wave_rec->pmt[i] - wave_rec->pmt[i - 1] <= 0)
        {
          //  Increment the drop counter.

          drop++;


          if (drop >= 5)
            {
              first_drop = i;
              break;
            }
        }
      else
        {
          drop = 0;
        }
    }
              

  //  If the return bin is prior to the first drop (ie surface return) we want to go ahead and kill it.

  if (bin < first_drop) return (NVTrue);


  rise = 0;
  start_data = 0;


  //  Find the start of the run.

  for (int32_t i = bin ; i >= 20 ; i--)
    {
      if (wave_rec->pmt[i] - wave_rec->pmt[i - 1] <= 0)
        {
          if (!start_data) start_data = i;

          rise++;

          if (rise >= 2) break;
        }
      else
        {
          rise = 0;
          start_data = 0;
        }
    }


  drop = 0;
  peak = 0;


  //  Find the peak.

  int32_t length = qMin (bin + 50, HWF_PMT_SIZE - 1);

  for (int32_t i = bin ; i < length ; i++)
    {
      if (wave_rec->pmt[i] - wave_rec->pmt[i - 1] < 0)
        {
          if (!peak) peak = i;

          drop++;

          if (drop >= 2) break;
        }
      else
        {
          drop = 0;
          peak = 0;
        }
    }


  //  Compute the slope.

  run = peak - start_data;
  slope = (float) (wave_rec->pmt[peak] - wave_rec->pmt[start_data]) / (float) run;


  length = qMin (peak + 50, HWF_PMT_SIZE - 1);


  //  Find the end of the backslope.

  for (int32_t i = peak ; i < length ; i++)
    {
      if (wave_rec->pmt[i] - wave_rec->pmt[i - 1] > 1)
        {
          end_data = i;
          break;
        }
    }


  //  Force the backslope to be ignored if we don't have enough points to compute a decent one.
  //  The assumption being that if you got X rise points you want X fall points.

  back_run = end_data - peak;

  if (back_run < run)
    {
      backslope = 999.0;
    }
  else
    {
      backslope = (float) (wave_rec->pmt[peak] - wave_rec->pmt[end_data]) / (float) back_run;
    }


  //  At one point we were using the backslope to determine if we wanted to kill a return.  Later it became a problem.
  //  The following line is just here to suppress a compiler warning.  I want to leave the code above in case it is
  //  needed in the future.  Just remove the following line to make it test it again.

  backslope = slope_req;


  //  If the slope is less than "slope_req" or the backslope is less than "slope_req" or the run length
  //  is less than "pmt_run_req" we want to kill the return.

  if (run < pmt_run_req || slope < slope_req || backslope < slope_req)
    {
      //fprintf(stderr,"%s %s %d %d %d %d %d\n",__FILE__,__FUNCTION__,__LINE__,bin,start_data, peak, end_data);
      //fprintf(stderr,"%s %s %d %d %d %f %f %f\n",__FILE__,__FUNCTION__,__LINE__,run,pmt_run_req,slope, backslope,slope_req);
      return (NVTrue);
    }


  //  If we got to here we didn't find anything.

  return (NVFalse);
}
