
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


/*********************************************************************************************

    This program is public domain software that was developed by the U.S. Naval Oceanographic
    Office.

    This is a work of the US Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the US Government.

    This software is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
    PARTICULAR PURPOSE.

*********************************************************************************************/

#ifndef VERSION

#define     VERSION     "PFM Software - hofWaveFilter V1.19 - 06/23/17"

#endif

/*

    Version 1.00
    Jan C. Depner
    04/12/11

    First version.


    Version 1.01
    Jan C. Depner
    04/21/11

    Speed improvements.


    Version 1.02
    Jan C. Depner
    05/03/11

    Now sorts on line and then original record number in order to reduce disk thrashing when reading HOF
    and INH records.


    Version 1.03
    Jan C. Depner
    05/05/11

    We're honoring the mask flag set by pfmEdit(3D).  That is, if the point is masked out in the editor we
    won't mark it for deletion.  Happy Cinco de Mayo!!!


    Version 1.10
    Jan C. Depner
    05/09/11

    Re-wrote the apd and pmt return filters to not try to process the entire waveform.  They now just use the
    bin that Optech has determined for each return and check before and after the bin to find the slopes.  This
    is WAAAAAAAAAYYYYYYYYYYYY faster than the old way.


    Version 1.11
    Jan C. Depner
    05/18/11

    Fixed the slope filter.  It wasn't finding the first drop correctly if it was a low slope first drop.
    Moved masking responsibility back into the editors (pfmEdit and pfmEdit3D).


    Version 1.12
    Jan C. Depner
    07/08/11

    Changed the default low slope val;ue to 0.5 instead of 0.75.  We were killing way too many valid points when
    looking for the steeper slope.  Removed slope filter kill for points before first drop.  We were killing points
    where they got near the surface.  The original plan for the prior to first drop kill was to remove surface
    returns but those are pretty easy to spot anyway.


    Version 1.13
    Jan C. Depner (PFM Software)
    02/26/14

    Cleaned up "Set but not used" variables that show up using the 4.8.2 version of gcc.


    Version 1.14
    Jan C. Depner (PFM Software)
    07/23/14

    - Switched from using the old NV_INT64 and NV_U_INT32 type definitions to the C99 standard stdint.h and
      inttypes.h sized data types (e.g. int64_t and uint32_t).


    Version 1.15
    Jan C. Depner (PFM Software)
    02/16/15

    - To give better feedback to shelling programs in the case of errors I've added the program name to all
      output to stderr.


    Version 1.16
    Jan C. Depner (PFM Software)
    06/03/17

    - Fixed bug caused by not reading the HOF header prior to reading the record.


    Version 1.17
    Jan C. Depner (PFM Software)
    06/04/17

    - I think I fixed the PMT and APD slope filters ;-)


    Version 1.18
    Jan C. Depner (PFM Software)
    06/06/17

    - Disable AC offset check if the required AC offset from pfmEdit3D is set to 0.


    Version 1.19
    Jan C. Depner (PFM Software)
    06/23/17

    - Fixed bug caused by not initializing point memory area.

*/
