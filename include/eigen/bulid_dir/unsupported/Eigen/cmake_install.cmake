# Install script for directory: /home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE FILE FILES
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/AdolcForward"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/AlignedVector3"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/ArpackSupport"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/AutoDiff"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/BVH"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/EulerAngles"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/FFT"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/IterativeSolvers"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/KroneckerProduct"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/LevenbergMarquardt"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/MatrixFunctions"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/MoreVectorization"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/MPRealSupport"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/NonLinearOptimization"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/NumericalDiff"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/OpenGLSupport"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/Polynomials"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/Skyline"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/SparseExtra"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/SpecialFunctions"
    "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/Splines"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE DIRECTORY FILES "/home/local/eda03/zixuan/NewPar/include/eigen/unsupported/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/local/eda03/zixuan/NewPar/include/eigen/bulid_dir/unsupported/Eigen/CXX11/cmake_install.cmake")

endif()

