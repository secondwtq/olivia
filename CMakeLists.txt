cmake_minimum_required(VERSION 3.3)
project(VMForOlive)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(/usr/local/opt/llvm/include)
link_directories(/usr/local/opt/llvm/lib)
add_definitions(-D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS)

set(SOURCE_FILES main.cpp LOValue.cxx LOValue.hxx LOCell.hxx LOCell.cxx LOCell.hxx
        LOLexer.cxx LOLexer.hxx LOFrontendSpirit.cxx LOFrontendSpirit.hxx
        LOAST.cxx LOAST.hxx LOParser.cxx LOParser.hxx LOScript.cxx LOScript.hxx
        LOCommon.hxx LOASTConstant.cxx LOASTConstant.hxx
        LOLBackend.cxx LOLBackend.hxx LOLModule.cxx LOLModule.hxx LOASTVisitor.cxx LOASTVisitor.hxx LOLCodegen.cxx LOLCodegen.hxx LOResolver.cxx LOResolver.hxx LOHLBasic.cxx LOHLBasic.hxx LOHLASTVisitor.cxx LOHLASTVisitor.hxx LOType.cxx LOType.hxx LOIntrusiveList.cxx LOIntrusiveList.hxx LOHLBuilder.cxx LOHLBuilder.hxx)
add_executable(VMForOlive ${SOURCE_FILES})
set(VMOLIVE_LIBS LLVMCore LLVMSupport)
list(APPEND VMOLIVE_LIBS curses)
target_link_libraries(VMForOlive ${VMOLIVE_LIBS})
