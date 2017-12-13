
set(HIFLOW_INCLUDE_PATHS "/opt/HiFlow/include/hiflow3" "/usr/local/include/hiflow3")
set(HIFLOW_LIB_PATHS "/opt/HiFlow/lib" "/usr/local/lib/")



FIND_LIBRARY (HIFLOW_LIBRARY hiflow
   # PATHS
    ${HIFLOW_LIB_PATHS}
    DOC "The directory where the SOFA libraries reside"
    )    

FIND_PATH(HIFLOW_INCLUDE_DIRS hiflow.h ${HIFLOW_INCLUDE_PATHS})    
SET( HIFLOW_FOUND "YES" )
SET( HIFLOW_LIBRARIES          
            ${HIFLOW_LIBRARY}         
          )




MARK_AS_ADVANCED(
	HIFLOW_LIBRARY
  )



  
    

    
