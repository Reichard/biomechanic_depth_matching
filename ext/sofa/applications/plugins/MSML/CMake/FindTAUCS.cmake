
set(TAUCS_INCLUDE_DIRS_CAN "/opt/taucs/include" "/usr/local/taucs/include")
set(TAUCS_LIB_PATHS "/opt/taucs/lib64" "/usr/local/taucs/lib64")

FIND_PATH(TAUCS_INCLUDE_DIRS taucs.h ${TAUCS_INCLUDE_DIRS_CAN})

FIND_LIBRARY (TAUCS_LIBRARY taucs
   # PATHS
    ${TAUCS_LIB_PATHS}
    DOC "The directory where the SOFA libraries reside"
    )    

    
SET( TAUCS_FOUND "YES" )
SET( TAUCS_LIBRARIES          
            ${TAUCS_LIBRARY}         
          )




MARK_AS_ADVANCED(
	TAUCS_LIBRARY
  )


  
    

    
