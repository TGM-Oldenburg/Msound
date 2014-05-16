/*------------------------------------------------------------------------//
//                           Copyright (c) 2009                           //
//                              Sven Fischer                              //
//------------------------------------------------------------------------//
//                                                                        //
// Permission is hereby granted, free of  charge, to any person obtaining //
// a copy  of this  software  and  associated  documentation  files  (the //
// "Software"), to  deal in the  Software without  restriction, including //
// without limitation the  rights to  use, copy, modify,  merge, publish, //
// distribute,  sublicense, and / or sell copies of  the Software, and to //
// permit  persons to whom  the Software  is furnished to  do so, subject //
// to the following conditions:                                           //
//   1) The above  copyright notice and  this permission notice  shall be //
//      included in all copies or substantial portions of the Software.   //
//   2) The origin of this Software must not be misrepresented;  you must //
//      not claim that you  wrote the original Software.  If you use this //
//      Software  in   a  product,  an   acknowledgment  in  the  product //
//      documentation would be appreciated but is not required.           //
//   3) Altered versions must be  plainly marked as such, and must not be //
//      misrepresented as being the original Software.                    //
//                                                                        //
// THE  SOFTWARE IS  PROVIDED  "AS IS",  WITHOUT  WARRANTY  OF ANY  KIND, //
// EXPRESS  OR IMPLIED,  INCLUDING  BUT  NOT LIMITED  TO  THE  WARRANTIES //
// OF   MERCHANTABILITY,   FITNESS   FOR   A   PARTICULAR   PURPOSE   AND //
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE //
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION //
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN  CONNECTION //
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.        //
//                                                                        //
//------------------------------------------------------------------------*/


/*---------------------------------------------------------------------*//**
\file
\brief General header file for building MATLAB MEX-files at the IHA.
\version 0.01
\date 2009-01-04

This file includes all required MATLAB MEX-file SDK headers. It also adds
some MEX-file specific data types (#mwSize, #mwIndex, and #mwSignedIndex),
unless the provided MEX-file SDK version does include them, already. Thus
making current MEX-file source code compatible with older SDK versions.\n
\n
In addition some routines for MEX-file input argument type checking are
provided. Whereas these routines are implemented as \a inline functions for
C++ or as macro definitions to be used with "pure" C compilers.

\author Sven Fischer
*//*----------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/
#ifndef IHA_IHAMEXFILE_H_INCLUDED
#define IHA_IHAMEXFILE_H_INCLUDED
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// Prepare MATLAB Executable / MEX-file SDK.                              //
//------------------------------------------------------------------------*/
#ifndef MATLAB_MEX_FILE
#define MATLAB_MEX_FILE
#endif
/*------------------------------------------------------------------------//
// Include MATLAB mex* functions, does include "matrix.h" also.           //
//------------------------------------------------------------------------*/
#include <mex.h>
/*------------------------------------------------------------------------//
// MATLAB MEX-file SDK versions prior to 7.1.4 (R14SP3) are missing some  //
// data type defintions. So define them, if they're missing.              //
//------------------------------------------------------------------------*/
#ifndef MWSIZE_MIN
	/** \brief Type for size values.
	 *
	 *  \a mwSize is a type that represents size values, such as array
	 *  dimensions. This function is provided for purposes of cross-platform
	 *  flexibility. By default, \a mwSize is equivalent to \c int in C.
	 *  When using the <tt>mex -largeArrayDims</tt> switch, \a mwSize is
	 *  equivalent to \c size_t in C. (Documentation copied from MATLAB
	 *  7.5.0.342 (R2007b))
	 *
	 *  \note
	 *  The definition of this data type included because older MATLAB
	 *  external interfaces SDKs (prior to 7.1.4 (R14SP3)) lack its
	 *  defenition and would be incompatible, otherwise.
	 *
	 *  \see mwIndex, mwSignedIndex.
	 */
	typedef int mwSize;
#endif
#ifndef MWINDEX_MIN
	/** \brief Type for index values.
	 *
	 *  \a mwIndex is a type that represents index values, such as indices
	 *  into arrays. This function is provided for purposes of
	 *  cross-platform flexibility. By default, \a mwIndex is equivalent to
	 *  \c int in C. When using the <tt>mex -largeArrayDims</tt> switch,
	 *  \a mwIndex is equivalent to \c size_t in C. (Documentation copied
	 *  from MATLAB 7.5.0.342 (R2007b))
	 *
	 *  \note
	 *  The definition of this data type included because older MATLAB
	 *  external interfaces SDKs (prior to 7.1.4 (R14SP3)) lack its
	 *  defenition and would be incompatible, otherwise.
	 *
	 *  \see mwSignedIndex, mwSize.
	 */
	typedef int mwIndex;
	/** \brief Type for signed index values.
	 *
	 *  \a mwIndex is a type that represents signed index values, such as
	 *  indices into arrays. This function is provided for purposes of
	 *  cross-platform flexibility. By default, \a mwIndex is equivalent to
	 *  \c int in C. When using the <tt>mex -largeArrayDims</tt> switch,
	 *  \a mwIndex is equivalent to \c size_t in C. (Documentation copied
	 *  from MATLAB 7.5.0.342 (R2007b))
	 *
	 *  \note
	 *  The definition of this data type included because older MATLAB
	 *  external interfaces SDKs (prior to 7.1.4 (R14SP3)) lack its
	 *  defenition and would be incompatible, otherwise.
	 *
	 *  \see mwIndex, mwSize.
	 */
	typedef int mwSignedIndex;
#endif
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------//
// If this is Microsoft Visual C++, configure required MATLAB libraries   //
// "libmex.lib" and "libmx.lib" to be included automatically.             //
//------------------------------------------------------------------------*/
#ifdef _MSC_VER
	#pragma comment(lib, "libmex.lib")
	#pragma comment(lib, "libmx.lib" )
#endif
/*------------------------------------------------------------------------*/


/*////////////////////////////////////////////////////////////////////////*/
/* Use inline functions for C++ */
#if defined(__cplusplus)
/*////////////////////////////////////////////////////////////////////////*/

/// \brief Check if the provided argument is a single data string.
///
/// Check if the provided argument is a single non-complex data string.
///
/// \param[in] pArray Pointer to the MATLAB data array to be checked.
///
/// \return Returns \c 1 if the provided data object passed the check,
///         otherwise the value \c 0 is returned.
inline int isString(const mxArray *pArray)
{
	return ( ( mxIsChar (pArray)      ) &&
	         ( mxGetM   (pArray) == 1 ) &&
	         (!mxIsEmpty(pArray)      ) );
}

/// \brief Check if the provided argument is a non-complex double scalar.
///
/// Check if the provided argument is a non-complex scalar double value.
///
/// \param[in] pArray Pointer to the MATLAB data array to be checked.
///
/// \return Returns \c 1 if the provided data object passed the check,
///         otherwise the value \c 0 is returned.
inline int isNonComplexDoubleScalar(const mxArray *pArray)
{
	return ( (!mxIsComplex          (pArray)      ) &&
	         ( mxIsDouble           (pArray)      ) &&
	         ( mxGetNumberOfElements(pArray) == 1 ) );
}

/// \brief Check if the provided argument is a non-complex double vector.
///
/// Check if the provided argument is a one-dimensional non-complex double
/// data matrix.
///
/// \param[in] pArray Pointer to the MATLAB data array to be checked.
///
/// \return Returns \c 1 if the provided data object passed the check,
///         otherwise the value \c 0 is returned.
inline int isNonComplexDoubleVector(const mxArray *pArray)
{
	return ( ( mxGetNumberOfDimensions(pArray) == 2 ) &&
	         (!mxIsComplex            (pArray)      ) &&
	         ( mxIsDouble             (pArray)      ) &&
	         (!mxIsEmpty              (pArray)      ) &&
	         ( ( mxGetM(pArray) == 1 ) ||
	           ( mxGetN(pArray) == 1 )              ) );
}

/// \brief Check if the provided argument is a non-complex double matrix.
///
/// Check if the provided argument is a two-dimensional non-complex double
/// data matrix.
///
/// \param[in] pArray Pointer to the MATLAB data array to be checked.
///
/// \return Returns \c 1 if the provided data object passed the check,
///         otherwise the value \c 0 is returned.
inline int isNonComplexDoubleMatrix(const mxArray *pArray)
{
	return ( ( mxGetNumberOfDimensions(pArray) == 2 ) &&
	         (!mxIsComplex            (pArray)      ) &&
	         ( mxIsDouble             (pArray)      ) &&
	         (!mxIsEmpty              (pArray)      ) );
}

/// \brief Check if the provided argument is single data struct.
///
/// Check if the provided argument is a single data struct.
///
/// \param[in] pArray Pointer to the MATLAB data array to be checked.
///
/// \return Returns \c 1 if the provided data object passed the check,
///         otherwise the value \c 0 is returned.
inline int isSingleStruct(const mxArray *pArray)
{
	return ( ( mxIsStruct           (pArray)      ) &&
	         ( mxGetNumberOfElements(pArray) == 1 ) );
}

/*////////////////////////////////////////////////////////////////////////*/
/* Otherwise use C-style macros */
#else /* #if defined(__cplusplus) */
/*////////////////////////////////////////////////////////////////////////*/

/** \brief Check if the provided argument is a single data string.

    Check if the provided argument is a single non-complex data string.

    \param[in] pArray Pointer to the MATLAB data array to be checked.

    \return Returns \c 1 if the provided data object passed the check,
            otherwise the value \c 0 is returned.
*/
#define isString(pArray) ( \
	( mxIsChar (pArray)      ) && \
	( mxGetM   (pArray) == 1 ) && \
	(!mxIsEmpty(pArray)      ) )

/** \brief Check if the provided argument is a non-complex double scalar.

    Check if the provided argument is a non-complex scalar double value.

    \param[in] pArray Pointer to the MATLAB data array to be checked.

    \return Returns \c 1 if the provided data object passed the check,
            otherwise the value \c 0 is returned.
*/
#define isNonComplexDoubleScalar(pArray) ( \
	(!mxIsComplex          (pArray)      ) && \
	( mxIsDouble           (pArray)      ) && \
	( mxGetNumberOfElements(pArray) == 1 ) )

/** \brief Check if the provided argument is a non-complex double vector.

    Check if the provided argument is a one-dimensional non-complex double
    data matrix.

    \param[in] pArray Pointer to the MATLAB data array to be checked.

    \return Returns \c 1 if the provided data object passed the check,
            otherwise the value \c 0 is returned.
*/
#define isNonComplexDoubleVector(pArray) ( \
	( mxGetNumberOfDimensions(pArray) == 2 ) && \
	(!mxIsComplex            (pArray)      ) && \
	( mxIsDouble             (pArray)      ) && \
	(!mxIsEmpty              (pArray)      ) && \
	( ( mxGetM(pArray) == 1 ) ||                \
	  ( mxGetN(pArray) == 1 )              ) )

/** \brief Check if the provided argument is a non-complex double matrix.

    Check if the provided argument is a two-dimensional non-complex double
    data matrix.

    \param[in] pArray Pointer to the MATLAB data array to be checked.

    \return Returns \c 1 if the provided data object passed the check,
            otherwise the value \c 0 is returned.
*/
#define isNonComplexDoubleMatrix(pArray) ( \
	( mxGetNumberOfDimensions(pArray) == 2 ) && \
	(!mxIsComplex            (pArray)      ) && \
	( mxIsDouble             (pArray)      ) && \
	(!mxIsEmpty              (pArray)      ) )

/** \brief Check if the provided argument is single data struct.

    Check if the provided argument is a single data struct.

    \param[in] pArray Pointer to the MATLAB data array to be checked.

    \return Returns \c 1 if the provided data object passed the check,
            otherwise the value \c 0 is returned.
*/
#define isSingleStruct(pArray) ( \
	( mxIsStruct           (pArray)      ) && \
	( mxGetNumberOfElements(pArray) == 1 ) )

/*////////////////////////////////////////////////////////////////////////*/
#endif /* #if defined(__cplusplus) #else */
/*////////////////////////////////////////////////////////////////////////*/


/*------------------------------------------------------------------------*/
#endif /* end of: #ifndef IHA_IHAMEXFILE_H_INCLUDED */
/*------------------------------------------------------------------------*/
