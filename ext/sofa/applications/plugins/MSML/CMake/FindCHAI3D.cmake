set(CHAI3D_INCLUDE_DIRS "/opt/haptic/chai3d/src" "/usr/local/chai3d/src/")
set(CHAI3D_LIB_PATHS "/opt/haptic/chai3d/lib/lin-x86_64" "/opt/haptic/chai3d/external/DHD/lib/lin-x86_64" "/usr/local/chai3d/lib/lin-x86_64/" "/usr/local/chai3d/external/DHD/lib/lin-x86_64/")




FIND_LIBRARY (CHAI3D_LIBRARY chai3d
   # PATHS
    ${CHAI3D_LIB_PATHS}
    DOC "The directory where the CHAI3D libraries reside"
    )
    
FIND_LIBRARY (CHAI3D_LIBRARY_EXTERNAL dhd
   # PATHS
    ${CHAI3D_LIB_PATHS}
    DOC "The directory where the CHAI3D libraries reside"
    )
    
FIND_LIBRARY (CHAI3D_LIBRARY_EXTERNAL_USB usb
   # PATHS
    ${CHAI3D_LIB_PATHS}
    DOC "The directory where the CHAI3D libraries reside"
    )
    
  #  /usr/lib/x86_64-linux-gnu/libusb.so
    
FIND_PATH(CHAI3D_INCLUDE_DIR chai3d.h ${CHAI3D_INCLUDE_DIRS})
    
SET( CHAI3D_FOUND "YES" )



SET( CHAI3D_LIBRARIES 
            ${CHAI3D_LIBRARY}
            ${CHAI3D_LIBRARY_EXTERNAL}
            ${CHAI3D_LIBRARY_EXTERNAL_USB}
          
          )




MARK_AS_ADVANCED(
   CHAI3D_LIBRARY
   CHAI3D_LIBRARY_EXTERNAL
   CHAI3D_LIBRARY_EXTERNAL_USB
  )


  
    

    
