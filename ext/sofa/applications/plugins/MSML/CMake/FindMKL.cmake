# - Find MKL
# The intel MKL is a multi-purpose numerical library
#
# This module defines the following variables
#  MKL_FOUND         - system has MKL
#  MKL_INCLUDE_DIRS  - where the MKL include directory can be found
#  MKL_LIBRARIES     - Link to this to use MKL
#

IF(UNIX)
  set(MKL_INCLUDE_DIRS_CAN "/opt/intel/mkl/include" "/usr/local/intel/mkl/include")
  FIND_PATH(MKL_INCLUDE_DIRS mkl.h ${MKL_INCLUDE_DIRS_CAN})
  
  set(MKL_LIB_PATHS "/opt/intel/mkl/lib/intel64" "/usr/local/intel/mkl/lib/intel64")
  #set(STANDARD_LIB_PATHS "/lib")
   
  #FIND_LIBRARY(MKL_LIBRARY MKL PATHS /opt/MKL/lib/linux)
  
  FIND_LIBRARY (MKL_LIBRARY mkl_rt
   # PATHS
    ${MKL_LIB_PATHS}
    DOC "The directory where the MKL libraries reside"
    )
    
    FIND_LIBRARY (INTELMP_LIBRARY iomp5 /opt/intel/lib/intel64 /usr/local/intel/lib/intel64)
    
    FIND_LIBRARY (PTHREAD_LIBRARY pthread /usr/lib)
    
   # MESSAGE(" #############################")    
    MESSAGE("MKL library path = ${MKL_LIBRARIES}")    
   # MESSAGE(" #############################")
      
     set(MKL_LIBRARIES ${MKL_LIBRARY} ${INTELMP_LIBRARY})
  
    MARK_AS_ADVANCED(MKL_INCLUDE_DIRS MKL_LIBRARIES)

  
ELSE(UNIX)
  MESSAGE( FATAL_ERROR "Not defined for Windows/Apple")
ENDIF(UNIX)
