
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

uint8_t waveform_check (MISC *misc, WAVE_DATA *wave_data, int32_t recnum)
{
  int32_t bin = wave_data[recnum].bot_bin_first;

  if (misc->data[recnum].sub) bin = wave_data[recnum].bot_bin_second;


  //  I'm not looking at surface data.

  if (bin < 20) return (NVFalse);


  for (int32_t i = 0 ; i < misc->point_count ; i++)
    {
      int32_t start_apd_search = qMax (20, bin - misc->abe_share->filterShare.search_width);
      int32_t start_pmt_search = qMax (20, bin - misc->abe_share->filterShare.search_width);
      int32_t end_apd_search = qMin (HWF_APD_SIZE - 1, bin + misc->abe_share->filterShare.search_width);
      int32_t end_pmt_search = qMin (HWF_PMT_SIZE - 1, bin + misc->abe_share->filterShare.search_width);
      int32_t rise_count = 0;
      int32_t ndx = misc->points[i];

      if (start_apd_search < HWF_APD_SIZE - 20)
        {
          for (int32_t j = start_apd_search ; j < end_apd_search ; j++)
            {
              if (wave_data[ndx].apd[j] - wave_data[ndx].apd[j - 1] > 0)
                {
                  rise_count++;
                }
              else if (wave_data[ndx].apd[j] - wave_data[ndx].apd[j - 1] < 0)
                {
                  rise_count = 0;
                }

              if (rise_count >= misc->abe_share->filterShare.rise_threshold) return (NVFalse);
            }
        }


      rise_count = 0;

      for (int32_t j = start_pmt_search ; j < end_pmt_search ; j++)
        {
          if (wave_data[ndx].pmt[j] - wave_data[ndx].pmt[j - 1] > 0)
            {
              rise_count++;
            }
          else if (wave_data[ndx].pmt[j] - wave_data[ndx].pmt[j - 1] < 0)
            {
              rise_count = 0;
            }

          if (rise_count >= misc->abe_share->filterShare.rise_threshold) return (NVFalse);
        }
    }


  return (NVTrue);
}
