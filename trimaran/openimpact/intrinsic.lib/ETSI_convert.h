/******************************************************************************

      Internal release for ETSI SMG2 Speech Experts Group

      GSM Enhanced Full Rate C source code v5.0.0 with the changes defined
      in the following Change Requests (presented and approved in ETSI
      SMG2-SEG #7 meeting in Nuremberg, 19-21.06.1996) implemented:

      Subject                                Source                 Date
      =========================================================================
      Moving of the pre-channel coding       Nokia                  19.06.1996
      from the 06 series to the 05 series

      Correction to Bit Exact Code           Motorola, Nokia        19.06.1996

      Initialisation of algebraic code-      TI, Nokia              19.06.1996
      book search

      L_Extract routine                      TI, Nokia              19.06.1996

      Correction to Bit Exact Code           Motorola               19.06.1996

      Removal of unused lines in error       Nokia                  19.06.1996
      concealment C code

      Copyright (C) European Telecommunications Standards Institute 1996

******************************************************************************/
/*___________________________________________________________________________
 |                                                                           |
 |   Prototypes for basic arithmetic operators.                              |
 |   ETSI names conflict with real benchmark function names (ex. m88ksim)    |
 |      Use this header file to convert ETSI benchmarks to ETSI_* names.     |
 |___________________________________________________________________________|
*/

#define setOverflow ETSI_setOverflow
#define getOverflow ETSI_getOverflow
#define setCarry    ETSI_setCarry
#define getCarry    ETSI_getCarry

#define add ETSI_add
#define sub ETSI_sub
#define abs_s ETSI_abs_s
#define shl ETSI_shl
#define shr ETSI_shr
#define mult ETSI_mult
#define L_mult ETSI_L_mult
#define negate ETSI_negate
#define extract_h ETSI_extract_h
#define extract_l ETSI_extract_l
#define round ETSI_round
#define L_mac ETSI_L_mac
#define L_msu ETSI_L_msu
#define L_macNs ETSI_L_macNs
 
#define L_msuNs ETSI_L_msuNs

#define L_add ETSI_L_add
#define L_sub ETSI_L_sub
#define L_add_c ETSI_L_add_c
#define L_sub_c ETSI_L_sub_c
#define L_negate ETSI_L_negate
#define mult_r ETSI_mult_r
#define L_shl ETSI_L_shl
#define L_shr ETSI_L_shr
#define shr_r ETSI_shr_r

#define mac_r ETSI_mac_r

#define msu_r ETSI_msu_r

#define L_deposit_h ETSI_L_deposit_h
#define L_deposit_l ETSI_L_deposit_l

#define L_shr_r ETSI_L_shr_r

#define L_abs ETSI_L_abs
#define L_sat ETSI_L_sat
#define norm_s ETSI_norm_s
#define div_s ETSI_div_s
#define norm_l ETSI_norm_l

