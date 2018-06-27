
/*******************************************************************
** FILE:
**    Math.h
** DESCRIPTION:
**    This header file is intended to hold common mathematical
**    macros and commonly used constants.
********************************************************************/
#ifndef MATH_H
#define MATH_H


/*******************************************************************
** Defines *********************************************************
********************************************************************/

/* General Defines */

#define FALSE 0
#define TRUE 1

/* Data sizes */
#define ONE_KILOBYTE       1024 /* bYTES */
#define TWO_KILOBYTE       2048 /* bYTES */
#define TTHREE_KILOBYTE    3072 /* bYTES */
#define FOUR_KILOBYTE      4096 /* bYTES */
#define FIVE_KILOBYTE      5120 /* bYTES */
#define SIX_KILOBYTE       6144 /* bYTES */
#define SEVEN_KILOBYTE     7168 /* bYTES */
#define EIGHT_KILOBYTE     8192 /* bYTES */
#define NINE_KILOBYTE      9216 /* bYTES */
#define TEN_KILOBYTE      10240 /* bYTES */
#define ONE_MEGABYTE    1048576 /* bYTES */
#define TWO_MEGABYTE    2097152 /* bYTES */
#define TTHREE_MEGABYTE 3145728 /* bYTES */
#define FOUR_MEGABYTE   4194304 /* bYTES */
#define FIVE_MEGABYTE   5242880 /* bYTES */
#define SIX_MEGABYTE    6291456 /* bYTES */
#define SEVEN_MEGABYTE  7340032 /* bYTES */
#define EIGHT_MEGABYTE  8388608 /* bYTES */
#define NINE_MEGABYTE   9437184 /* bYTES */
#define TEN_MEGABYTE   10485760 /* bYTES */
#define ONE_GIGABYTE 1073741824 /* bYTES */

/* Constants */

#define GTOMPS2 (9.80665)
#define MPSTOMPH (2.23694)

#define PI (3.14159265359)
#define TWOPI (6.28318530718)

/* Macros */

#define TO_RAD(x) (x * 0.01745329252)  // deg to rad: *pi/180
#define TO_DEG(x) (x * 57.2957795131)  // rad to deg: *180/pi


#if EXE_MODE==1 /* Emulator mode */
  #define FCONSTRAIN(x,m,M) (fmin(fmax((x),m),M))
#else
  #define FCONSTRAIN constrain
#endif /* EXE_MODE */

//#define SIGN(x) ( (0<x)-(x<0)+(x==0) )
#define SIGN(x) ( (0<(x)) ? (-1) : (1) )
#define FABS(x) ( ( (x)>=0) ? (x) : -(x) )
#define ABS(x)  ( ( (x)>=0) ? (x) : -(x) )

#define MAX( a, b ) ( ( (a) > (b) ) ? (a) : (b) )
#define MIN( a, b ) ( ( (a) < (b) ) ? (a) : (b) )


#endif /* End MATH_H */
