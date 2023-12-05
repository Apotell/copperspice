cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(CopperSpice CXX)

set(CopperSpice_BASE ${PROJECT_SOURCE_DIR})

set(CopperSpice_INCDIR "${CopperSpice_BASE}/include" CACHE STRING "Root location of CopperSpice headers" FORCE)

if(WIN32)
  set(SYMBOL_FILE_SUFFIX ".pdb")
elseif(UNIX)
  set(SYMBOL_FILE_SUFFIX ".so.debug")
endif()

set(CopperSpice_COMPILE_DEFINITIONS QT_NO_DEBUG QT_SHARED)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
  if(WIN32)
    set(CopperSpice_BINDIR "${CopperSpice_BASE}/bin/windows-cl_debug" CACHE STRING "Root location of CopperSpice Binaries" FORCE)
    set(CopperSpice_LIBDIR "${CopperSpice_BASE}/lib/windows-cl_debug" CACHE STRING "Root location of CopperSpice Binaries" FORCE)
  else()
    set(CopperSpice_BINDIR "${CopperSpice_BASE}/bin/jammy-gcc_debug" CACHE STRING "Root location of CopperSpice Binaries" FORCE)
    set(CopperSpice_LIBDIR "${CopperSpice_BASE}/lib/jammy-gcc_debug" CACHE STRING "Root location of CopperSpice Binaries" FORCE)
  endif()  
else()
  if(WIN32)
    set(CopperSpice_BINDIR "${CopperSpice_BASE}/bin/windows-cl_release" CACHE STRING "Root location of CopperSpice Binaries" FORCE)
    set(CopperSpice_LIBDIR "${CopperSpice_BASE}/lib/windows-cl_release" CACHE STRING "Root location of CopperSpice Binaries" FORCE)
  else()
    set(CopperSpice_BINDIR "${CopperSpice_BASE}/bin/jammy-gcc_release" CACHE STRING "Root location of CopperSpice Binaries" FORCE)
    set(CopperSpice_LIBDIR "${CopperSpice_BASE}/lib/jammy-gcc_release" CACHE STRING "Root location of CopperSpice Binaries" FORCE)
  endif()
endif()

set(CopperSpice_UIC_LOCATION "${CopperSpice_BINDIR}/uic${CMAKE_EXECUTABLE_SUFFIX}" CACHE STRING "Location of uic executable" FORCE)
set(CopperSpice_RCC_LOCATION "${CopperSpice_BINDIR}/rcc${CMAKE_EXECUTABLE_SUFFIX}" CACHE STRING "Location of rcc executable" FORCE)

message("CopperSpice Base Dir = ${CopperSpice_BASE}")
message("CopperSpice Inc Dir = ${CopperSpice_INCDIR}")
message("CopperSpice Uic Exe = ${CopperSpice_UIC_LOCATION}")
message("CopperSpice Rcc Exe = ${CopperSpice_RCC_LOCATION}")

add_library(CsCore SHARED IMPORTED GLOBAL)

set_target_properties(CsCore PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CopperSpice_INCDIR};${CopperSpice_INCDIR}/QtCore"
    IMPORTED_IMPLIB "${CopperSpice_LIBDIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}CsCore${CMAKE_IMPORT_LIBRARY_SUFFIX}"
    IMPORTED_LOCATION "${CopperSpice_BINDIR}/${CMAKE_SHARED_LIBRARY_PREFIX}CsCore${CMAKE_SHARED_LIBRARY_SUFFIX}"
    INTERFACE_COMPILE_DEFINITIONS "${CopperSpice_COMPILE_DEFINITIONS}"
    PDB_NAME "${CMAKE_SHARED_LIBRARY_PREFIX}CsCore${SYMBOL_FILE_SUFFIX}"
    PDB_OUTPUT_DIRECTORY "${CopperSpice_BINDIR}")

foreach(target Gui Network OpenGL Xml)
  add_library(Cs${target} SHARED IMPORTED GLOBAL)
  target_link_libraries(Cs${target} INTERFACE CsCore)
  set_target_properties(Cs${target} PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${CopperSpice_INCDIR}/Qt${target}"
      IMPORTED_IMPLIB "${CopperSpice_LIBDIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}Cs${target}${CMAKE_IMPORT_LIBRARY_SUFFIX}"
      IMPORTED_LOCATION "${CopperSpice_BINDIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Cs${target}${CMAKE_SHARED_LIBRARY_SUFFIX}"
      INTERFACE_COMPILE_DEFINITIONS "${CopperSpice_COMPILE_DEFINITIONS}"
      PDB_NAME "${CMAKE_SHARED_LIBRARY_PREFIX}Cs${target}${SYMBOL_FILE_SUFFIX}"
      PDB_OUTPUT_DIRECTORY "${CopperSpice_BINDIR}")
endforeach()

macro(COPY_COPPERSPICE_RUNTIME_DEPENDENCIES ROOT_TARGET DEPENDENCIES)
  add_custom_target(Copy-CopperSpice-Platform-Depedencies-For-${ROOT_TARGET} ALL
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CopperSpice_BINDIR}/platforms/ $<TARGET_FILE_DIR:${ROOT_TARGET}>/platforms/
      COMMENT "Copying platform dependencies")

  foreach(DEPENDENCY ${DEPENDENCIES})
    add_custom_target(Copy-${DEPENDENCY}-Depedencies-For-${ROOT_TARGET} ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${DEPENDENCY}> $<TARGET_FILE_DIR:${ROOT_TARGET}>
        COMMENT "Copying dependencies")

    if(WIN32)
      get_target_property(PDB_OUTPUT_DIRECTORY ${DEPENDENCY} PDB_OUTPUT_DIRECTORY)
      get_target_property(PDB_NAME ${DEPENDENCY} PDB_NAME)
      if(EXISTS "${PDB_OUTPUT_DIRECTORY}/${PDB_NAME}")
        add_custom_target(Copy-${DEPENDENCY}-Depedencies-Symbols-For-${ROOT_TARGET} ALL
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PDB_OUTPUT_DIRECTORY}/${PDB_NAME}" $<TARGET_FILE_DIR:${ROOT_TARGET}>
            COMMENT "Copying dependency symbols")
      endif()
    endif()
  endforeach(DEPENDENCY)
endmacro()

macro(INSTALL_COPPERSPICE_RUNTIME_DEPENDENCIES DEPENDENCIES)
  install(
    DIRECTORY ${CopperSpice_BINDIR}/platforms/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/platforms/
  )

  foreach(DEPENDENCY ${DEPENDENCIES})
    install(
      FILES $<TARGET_FILE:${DEPENDENCY}>
      DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    )

    if(WIN32)
      get_target_property(PDB_OUTPUT_DIRECTORY ${DEPENDENCY} PDB_OUTPUT_DIRECTORY)
      get_target_property(PDB_NAME ${DEPENDENCY} PDB_NAME)
      install(
        FILES ${PDB_OUTPUT_DIRECTORY}/${PDB_NAME}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        OPTIONAL
      )
    endif()
  endforeach(DEPENDENCY)
endmacro()

function(COPPERSPICE_WRAP_UI ui_file)
  get_filename_component(outfile ${ui_file} NAME_WE)
  get_filename_component(infile ${ui_file} ABSOLUTE)
  set(outfile ${CMAKE_CURRENT_BINARY_DIR}/ui_${outfile}.h)
  add_custom_command(OUTPUT ${outfile}
      COMMAND ${CopperSpice_UIC_LOCATION}
      ARGS ${ui_options} -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile} VERBATIM)
endfunction()

function(COPPERSPICE_COMPILE_RESOURCES qrc_file)
  get_filename_component(outfilename ${qrc_file} NAME_WE)
  get_filename_component(infile ${qrc_file} ABSOLUTE)
  set(outfile ${CMAKE_CURRENT_BINARY_DIR}/rcc_${outfilename}.cpp)
  add_custom_command(OUTPUT ${outfile}
      COMMAND ${CopperSpice_RCC_LOCATION}
      ARGS ${rcc_options} -name ${outfilename} -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile} VERBATIM)
endfunction()
