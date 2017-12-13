
set(PARALUTION_INCLUDE_PATHS "/opt/paralution/src" )
set(PARALUTION_LIB_PATHS "/opt/paralution/lib/")



FIND_LIBRARY (PARALUTION_LIBRARY paralution
   # PATHS
    ${PARALUTION_LIB_PATHS}
    DOC "The directory where the Paralution libraries reside"
    )    

FIND_PATH(PARALUTION_INCLUDE_DIRS paralution.hpp ${PARALUTION_INCLUDE_PATHS})    
SET( PARALUTION_FOUND "YES" )
SET( PARALUTION_LIBRARIES          
            ${PARALUTION_LIBRARY}         
          )




MARK_AS_ADVANCED(
	PARALUTION_LIBRARY
  )



  
    

    
