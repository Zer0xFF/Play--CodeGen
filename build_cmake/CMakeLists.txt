cmake_minimum_required(VERSION 2.8)
project(CodeGen)

set(CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/../../Dependencies/cmake-modules
	${CMAKE_MODULE_PATH}
)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING
		"Choose the type of build, options are: None Debug Release"
		FORCE)
endif()

if(NOT MSVC)
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
		add_definitions(-DNDEBUG)
	else()
		add_definitions(-D_DEBUG)
	endif()
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT TARGET Framework)
	add_subdirectory(
		${CMAKE_CURRENT_SOURCE_DIR}/../../Framework/build_cmake/Framework
		${CMAKE_CURRENT_BINARY_DIR}/Framework
	)
endif()

if(ANDROID)
	if(NOT TARGET cpufeatures)
		add_library(cpufeatures STATIC ${CMAKE_ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c)
	endif()
	include_directories("${CMAKE_ANDROID_NDK}/sources/android/cpufeatures")
	list(APPEND PROJECT_LIBS cpufeatures)
endif()

include_directories(../include ../../Framework/include)

add_library(CodeGen 
	../src/AArch32Assembler.cpp
	../src/AArch64Assembler.cpp
	../src/Jitter_CodeGen_AArch32.cpp
	../src/Jitter_CodeGen_AArch32_64.cpp
	../src/Jitter_CodeGen_AArch32_Fpu.cpp
	../src/Jitter_CodeGen_AArch32_Md.cpp
	../src/Jitter_CodeGen_AArch64.cpp
	../src/Jitter_CodeGen_AArch64_64.cpp
	../src/Jitter_CodeGen_AArch64_Fpu.cpp
	../src/Jitter_CodeGen_AArch64_Md.cpp
	../src/X86Assembler.cpp
	../src/X86Assembler_Fpu.cpp
	../src/X86Assembler_Sse.cpp
	../src/Jitter_CodeGen_x86_32.cpp
	../src/Jitter_CodeGen_x86_64.cpp
	../src/Jitter_CodeGen_x86.cpp
	../src/Jitter_CodeGen_x86_Fpu.cpp
	../src/Jitter_CodeGen_x86_Md.cpp
	../src/CoffObjectFile.cpp
	../src/Jitter_CodeGen.cpp
	../src/Jitter_CodeGenFactory.cpp
	../src/Jitter.cpp
	../src/Jitter_Optimize.cpp
	../src/Jitter_RegAlloc.cpp
	../src/Jitter_Statement.cpp
	../src/Jitter_SymbolTable.cpp
	../src/MachoObjectFile.cpp
	../src/MemoryFunction.cpp
	../src/ObjectFile.cpp
)

enable_testing()

set(CodeGenTest_SRC
	../tests/AliasTest.cpp
	../tests/AliasTest2.cpp
	../tests/Alu64Test.cpp
	../tests/Call64Test.cpp
	../tests/ConditionTest.cpp
	../tests/Cmp64Test.cpp
	../tests/CompareTest.cpp
	../tests/Crc32Test.cpp
	../tests/CursorTest.cpp
	../tests/DivTest.cpp
	../tests/FpIntMixTest.cpp
	../tests/FpuTest.cpp
	../tests/HugeJumpTest.cpp
	../tests/LogicTest.cpp
	../tests/Logic64Test.cpp
	../tests/LzcTest.cpp
	../tests/Main.cpp
	../tests/MdAddTest.cpp
	../tests/MdCallTest.cpp
	../tests/MdCmpTest.cpp
	../tests/MdFpFlagTest.cpp
	../tests/MdFpTest.cpp
	../tests/MdLogicTest.cpp
	../tests/MdManipTest.cpp
	../tests/MdMemAccessTest.cpp
	../tests/MdMinMaxTest.cpp
	../tests/MdShiftTest.cpp
	../tests/MdSubTest.cpp
	../tests/MdTest.cpp
	../tests/MdUnpackTest.cpp
	../tests/MemAccessTest.cpp
	../tests/Merge64Test.cpp
	../tests/MultTest.cpp
	../tests/NestedIfTest.cpp
	../tests/RandomAluTest2.cpp
	../tests/RandomAluTest3.cpp
	../tests/RandomAluTest.cpp
	../tests/RegAllocTest.cpp
	../tests/Shift64Test.cpp
	../tests/ShiftTest.cpp
	../tests/SimpleMdTest.cpp
)

if(ANDROID)
	add_library(CodeGenTestSuite SHARED ${CodeGenTest_SRC})
else()
	add_executable(CodeGenTestSuite ${CodeGenTest_SRC})
	add_test(CodeGenTestSuite CodeGenTestSuite)
endif()

target_link_libraries(CodeGenTestSuite CodeGen Framework ${PROJECT_LIBS})
