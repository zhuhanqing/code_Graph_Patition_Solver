# Install script for directory: /home/local/eda03/zixuan/NewPar/include/eigen/Eigen

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE FILE FILES
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/Cholesky"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/CholmodSupport"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/Core"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/Dense"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/Eigen"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/Eigenvalues"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/Geometry"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/Householder"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/IterativeLinearSolvers"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/Jacobi"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/LU"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/MetisSupport"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/OrderingMethods"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/PaStiXSupport"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/PardisoSupport"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/QR"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/QtAlignedMalloc"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/SPQRSupport"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/SVD"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/Sparse"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/SparseCholesky"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/SparseCore"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/SparseLU"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/SparseQR"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/StdDeque"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/StdList"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/StdVector"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/SuperLUSupport"
    "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/UmfPackSupport"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE DIRECTORY FILES "/home/local/eda03/zixuan/NewPar/include/eigen/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

