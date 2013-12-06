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
 |   Prototypes for basic arithmetic operators                               |
 |___________________________________________________________________________|
*/

void ETSI_setOverflow(Flag val);
Flag ETSI_getOverflow();
void ETSI_setCarry(Flag val);
Flag ETSI_getCarry();

Word16 ETSI_add (Word16 var1, Word16 var2);    /* Short add,           1   */
Word16 ETSI_sub (Word16 var1, Word16 var2);    /* Short sub,           1   */
Word16 ETSI_abs_s (Word16 var1);               /* Short abs,           1   */
Word16 ETSI_shl (Word16 var1, Word16 var2);    /* Short shift left,    1   */
Word16 ETSI_shr (Word16 var1, Word16 var2);    /* Short shift right,   1   */
Word16 ETSI_mult (Word16 var1, Word16 var2);   /* Short mult,          1   */
Word32 ETSI_L_mult (Word16 var1, Word16 var2); /* Long mult,           1   */
Word16 ETSI_negate (Word16 var1);              /* Short negate,        1   */
Word16 ETSI_extract_h (Word32 L_var1);         /* Extract high,        1   */
Word16 ETSI_extract_l (Word32 L_var1);         /* Extract low,         1   */
Word16 ETSI_round (Word32 L_var1);             /* Round,               1   */
Word32 ETSI_L_mac (Word32 L_var3, Word16 var1, Word16 var2);   /* Mac,  1  */
Word32 ETSI_L_msu (Word32 L_var3, Word16 var1, Word16 var2);   /* Msu,  1  */
Word32 ETSI_L_macNs (Word32 L_var3, Word16 var1, Word16 var2); /* Mac without
                                                             sat, 1   */
Word32 ETSI_L_msuNs (Word32 L_var3, Word16 var1, Word16 var2); /* Msu without
                                                             sat, 1   */
Word32 ETSI_L_add (Word32 L_var1, Word32 L_var2);    /* Long add,        2 */
Word32 ETSI_L_sub (Word32 L_var1, Word32 L_var2);    /* Long sub,        2 */
Word32 ETSI_L_add_c (Word32 L_var1, Word32 L_var2);  /* Long add with c, 2 */
Word32 ETSI_L_sub_c (Word32 L_var1, Word32 L_var2);  /* Long sub with c, 2 */
Word32 ETSI_L_negate (Word32 L_var1);                /* Long negate,     2 */
Word16 ETSI_mult_r (Word16 var1, Word16 var2);       /* Mult with round, 2 */
Word32 ETSI_L_shl (Word32 L_var1, Word16 var2);      /* Long shift left, 2 */
Word32 ETSI_L_shr (Word32 L_var1, Word16 var2);      /* Long shift right, 2*/
Word16 ETSI_shr_r (Word16 var1, Word16 var2);        /* Shift right with
                                                   round, 2           */
Word16 ETSI_mac_r (Word32 L_var3, Word16 var1, Word16 var2); /* Mac with
                                                           rounding,2 */
Word16 ETSI_msu_r (Word32 L_var3, Word16 var1, Word16 var2); /* Msu with
                                                           rounding,2 */
Word32 ETSI_L_deposit_h (Word16 var1);        /* 16 bit var1 -> MSB,     2 */
Word32 ETSI_L_deposit_l (Word16 var1);        /* 16 bit var1 -> LSB,     2 */

Word32 ETSI_L_shr_r (Word32 L_var1, Word16 var2); /* Long shift right with
                                                round,  3             */
Word32 ETSI_L_abs (Word32 L_var1);            /* Long abs,              3  */
Word32 ETSI_L_sat (Word32 L_var1);            /* Long saturation,       4  */
Word16 ETSI_norm_s (Word16 var1);             /* Short norm,           15  */
Word16 ETSI_div_s (Word16 var1, Word16 var2); /* Short division,       18  */
Word16 ETSI_norm_l (Word32 L_var1);           /* Long norm,            30  */   

