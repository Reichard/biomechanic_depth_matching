
set(MUMPS_INCLUDE_PATHS "/usr/include" )
set(MUMPS_LIB_PATHS "/usr/lib")



FIND_LIBRARY (MUMPS_LIBRARY dmumps_seq
   # PATHS
    ${MUMPS_LIB_PATHS}
    DOC "The directory where the MUMPS libraries reside"
    )    

FIND_PATH(MUMPS_INCLUDE_DIRS dmumps_c.h ${MUMPS_INCLUDE_PATHS})    
SET( MUMPS_FOUND "YES" )
SET( MUMPS_LIBRARIES          
            ${MUMPS_LIBRARY}         
          )




MARK_AS_ADVANCED(
	MUMPS_LIBRARY
  )



  
    

    
