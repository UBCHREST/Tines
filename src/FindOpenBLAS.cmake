# Check OpenBLAS installation
FIND_PATH(OPENBLAS_FOUND_INCLUDE_PATH openblas_config.h
  HINTS ${OPENBLAS_INSTALL_PATH}/include ${OPENBLAS_INSTALL_PATH}/include/openblas
  NO_DEFAULT_PATH
)
FIND_PATH(OPENBLAS_FOUND_CBLAS_HEADER cblas_openblas.h
  HINTS ${OPENBLAS_INSTALL_PATH}/include ${OPENBLAS_INSTALL_PATH}/include/openblas
  NO_DEFAULT_PATH
)
FIND_LIBRARY(OPENBLAS_FOUND_LIBRARY NAMES openblas
  HINTS ${OPENBLAS_INSTALL_PATH}/lib ${OPENBLAS_INSTALL_PATH}/lib64
)

IF (OPENBLAS_FOUND_INCLUDE_PATH AND OPENBLAS_FOUND_LIBRARY)
  ADD_LIBRARY(openblas INTERFACE)
  SET_TARGET_PROPERTIES(openblas PROPERTIES 
    INTERFACE_INCLUDE_DIRECTORIES ${OPENBLAS_FOUND_INCLUDE_PATH}
    INTERFACE_COMPILE_OPTIONS "-I${OPENBLAS_FOUND_INCLUDE_PATH}"
    INTERFACE_LINK_LIBRARIES ${OPENBLAS_FOUND_LIBRARY}
  )    
  SET(OPENBLAS_FOUND ON)
  INSTALL(TARGETS openblas
    EXPORT tines-targets
    LIBRARY DESTINATION "${TINES_INSTALL_LIB_PATH}"
    ARCHIVE DESTINATION "${TINES_INSTALL_LIB_PATH}"
  )
  MESSAGE("-- OpenBLAS is found at ${OPENBLAS_FOUND_LIBRARY}")
ELSE()
  MESSAGE(FATAL_ERROR "-- OpenBLAS is not found at ${OPENBLAS_INSTALL_PATH}")
ENDIF()

# Check LAPACK at the same location
FIND_PATH(OPENBLAS_FOUND_LAPACKE_HEADER lapacke.h
  HINTS ${OPENBLAS_INSTALL_PATH}/include ${OPENBLAS_INSTALL_PATH}/include/openblas
  NO_DEFAULT_PATH
)
IF (OPENBLAS_FOUND_LAPACKE_HEADER)
  SET(LAPACKE_FOUND ON)
  MESSAGE("-- OpenBLAS provides LAPACKE and header is found at ${OPENBLAS_FOUND_LAPACKE_HEADER}")
ENDIF()

