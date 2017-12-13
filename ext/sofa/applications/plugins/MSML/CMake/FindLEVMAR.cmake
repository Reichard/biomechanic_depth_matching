set(LEVMAR_INCLUDE_DIRS_CAN "/opt/optimization/levmar" "/usr/local/levmar/")
set(LEVMAR_LIB_PATHS "/opt/optimization/levmar" "/usr/local/levmar/")

FIND_PATH(LEVMAR_INCLUDE_DIRS levmar.h ${LEVMAR_INCLUDE_DIRS_CAN})

FIND_LIBRARY (LEVMAR_LIBRARY levmar
   # PATHS
    ${LEVMAR_LIB_PATHS}
    DOC "The directory where the levmar library reside"
    )    

    
SET( LEVMAR_FOUND "YES" )
SET( LEVMAR_LIBRARIES          
            ${LEVMAR_LIBRARY}         
          )




MARK_AS_ADVANCED(
	LEVMAR_LIBRARY
  )


  
    

    
