IF(UNIX)
    FIND_PATH(SOLID_INCLUDE_DIRS SOLID.h /opt/include/SOLID/ /usr/local/include/SOLID/ /usr/include/SOLID/)
    FIND_LIBRARY(SOLID_LIBRARIES solid PATHS /opt/lib/ /usr/local/lib /usr/lib)
    
    IF(SOLID_INCLUDE_DIRS AND SOLID_LIBRARIES)
        SET(SOLID_FOUND "YES")
    ENDIF(SOLID_INCLUDE_DIRS AND SOLID_LIBRARIES)   
    MARK_AS_ADVANCED(SOLID_INCLUDE_DIRS SOLID_LIBRARIES)
ELSE(UNIX)
    MESSAGE( FATAL_ERROR "Not defined for Windows/Apple")
ENDIF(UNIX)