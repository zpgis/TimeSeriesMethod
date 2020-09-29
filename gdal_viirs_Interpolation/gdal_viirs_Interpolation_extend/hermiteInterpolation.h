//
// MATLAB Compiler: 6.6 (R2018a)
// Date: Mon Jun 15 08:49:19 2020
// Arguments:
// "-B""macro_default""-W""cpplib:hermiteInterpolation""-T""link:lib""Hermite.m"
// "-C"
//

#ifndef __hermiteInterpolation_h
#define __hermiteInterpolation_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_hermiteInterpolation_C_API 
#define LIB_hermiteInterpolation_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_hermiteInterpolation_C_API 
bool MW_CALL_CONV hermiteInterpolationInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_hermiteInterpolation_C_API 
bool MW_CALL_CONV hermiteInterpolationInitialize(void);

extern LIB_hermiteInterpolation_C_API 
void MW_CALL_CONV hermiteInterpolationTerminate(void);

extern LIB_hermiteInterpolation_C_API 
void MW_CALL_CONV hermiteInterpolationPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_hermiteInterpolation_C_API 
bool MW_CALL_CONV mlxHermite(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_hermiteInterpolation
#define PUBLIC_hermiteInterpolation_CPP_API __declspec(dllexport)
#else
#define PUBLIC_hermiteInterpolation_CPP_API __declspec(dllimport)
#endif

#define LIB_hermiteInterpolation_CPP_API PUBLIC_hermiteInterpolation_CPP_API

#else

#if !defined(LIB_hermiteInterpolation_CPP_API)
#if defined(LIB_hermiteInterpolation_C_API)
#define LIB_hermiteInterpolation_CPP_API LIB_hermiteInterpolation_C_API
#else
#define LIB_hermiteInterpolation_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_hermiteInterpolation_CPP_API void MW_CALL_CONV Hermite(int nargout, mwArray& r, const mwArray& x, const mwArray& y, const mwArray& xq);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
