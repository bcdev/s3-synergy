In the data section you find the values of the variables in the ADF. You may
directly change the values for the dimension variables to suit your needs.

For some variable values there is a placeholder e.g. 

    OLC_R_atm = ${OLC_R_ATM} ;

    SLN_R_atm = ${SLN_R_ATM} ;

    SLO_R_atm = ${SLO_R_ATM} ;

Here the plain text files I ask to obtain from you come into play. The value
of ${OLC_R_ATM} is the name of the plain text file that shall contain the
values of the OLC_R_atm variable in the LUT, in proper order. Do not edit
the lines where a placeholder appears, I take care of this!

The proper order becomes obvious from the header section in the template,
e.g.

    ubyte OLC_R_atm(ADA, SZA, OLC_VZA, air_pressure, water_vapour, T550, AMIN, OLC_channel) ;
        OLC_R_atm:long_name = "Atmospheric scattering term" ;
        OLC_R_atm:scale_factor = 0.004;
        OLC_R_atm:valid_min = 0u ;
        OLC_R_atm:valid_max = 250u ;
        OLC_R_atm:_DeflateLevel = 9;

Looping for writing the plain text file shall be done as in

  foreach (ADA) {
    foreach (SZA) {
       …
        foreach (AMIN) {
          foreach (OLC_channel) {
             write value of OLC_R_atm(ADA, SZA, …, AMIN, OLC_channel) followed by ", " into text file
          }
        }
       … 
    }
  }

The text file shall contain numerical values separated by comma & space.
Note that the numeric type is obvious from the header, for OLC_R_atm it
is unsigned byte, but you may write it as integer.  Note the scaling factor,
the physical values of OLC_R_atm are the value written to the plain text
file multiplied by 0.004. So you have to divide the physical value by 0.004
before writing it to the text file.
