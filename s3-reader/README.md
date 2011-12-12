Levell:
* filename in Manifest xml and file name on disk not consistent.
  radianceOa6.nc <--> radiancesOa06.nc
* in qualityFlags.nc the variable for the meaning of the flags has the wrong name
  is flag_meaning should be flag_meanings (according to CF Convention)
* in manifest file the Product_Name in the Main_Product_Header has questionable value
  is OL_1_ERR should be more like File_Name in Fixed_Header
* File_Type in Fixed_Header has questionable spaces in front of its value: '  OL_1_ERR'
* description and name are the same in radiance NetCDF files
* Tie-point grids do not have a description nor a unit

Level2:
* geoCoodinates.nc use 0 as no-data value which is in valid range.
  However it is not set as fill value.

Unsere Probleme:
* timeCoordinates.nc can not be read. The Variable has a rank of 1.
  Such variables are ignored by NetCDF CF reader.

