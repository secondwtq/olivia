cmake_minimum_required(VERSION 3.3)
project(VMForOlive)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp LOValue.cxx LOValue.hxx LOCell.hxx LOCell.cxx LOCell.hxx LOLexer.cxx LOLexer.hxx LOFrontendSpirit.cxx LOFrontendSpirit.hxx LOAST.cxx LOAST.hxx LOParser.cxx LOParser.hxx LOScript.cxx LOScript.hxx LOCommon.hxx LOASTConstant.cxx LOASTConstant.hxx)
add_executable(VMForOlive ${SOURCE_FILES})