###########################################################################
## Makefile generated for component 'robotIK'. 
## 
## Makefile     : robotIK_rtw.mk
## Generated on : Thu Feb 03 15:23:30 2022
## Final product: .\robotIK.lib
## Product type : static-library
## 
###########################################################################

###########################################################################
## MACROS
###########################################################################

# Macro Descriptions:
# PRODUCT_NAME            Name of the system to build
# MAKEFILE                Name of this makefile
# COMPILER_COMMAND_FILE   Compiler command listing model reference header paths
# CMD_FILE                Command file
# MODELLIB                Static library target

PRODUCT_NAME              = robotIK
MAKEFILE                  = robotIK_rtw.mk
MATLAB_ROOT               = E:\matlab
MATLAB_BIN                = E:\matlab\bin
MATLAB_ARCH_BIN           = $(MATLAB_BIN)\win64
START_DIR                 = C:\Users\User\Documents\MATLAB\ThirdYearProject\codegen\lib\robotIK
TGT_FCN_LIB               = ISO_C
SOLVER_OBJ                = 
CLASSIC_INTERFACE         = 0
MODEL_HAS_DYNAMICALLY_LOADED_SFCNS = 
RELATIVE_PATH_TO_ANCHOR   = .
COMPILER_COMMAND_FILE     = robotIK_rtw_comp.rsp
CMD_FILE                  = robotIK_rtw.rsp
C_STANDARD_OPTS           = 
CPP_STANDARD_OPTS         = 
NODEBUG                   = 1
MODELLIB                  = robotIK.lib

###########################################################################
## TOOLCHAIN SPECIFICATIONS
###########################################################################

# Toolchain Name:          Microsoft Visual C++ 2017 v15.0 | nmake (64-bit Windows)
# Supported Version(s):    15.0
# ToolchainInfo Version:   2020b
# Specification Revision:  1.0
# 
#-------------------------------------------
# Macros assumed to be defined elsewhere
#-------------------------------------------

# NODEBUG
# cvarsdll
# cvarsmt
# conlibsmt
# ldebug
# conflags
# cflags

#-----------
# MACROS
#-----------

MW_EXTERNLIB_DIR    = $(MATLAB_ROOT)\extern\lib\win64\microsoft
MW_LIB_DIR          = $(MATLAB_ROOT)\lib\win64
CPU                 = AMD64
APPVER              = 5.02
CVARSFLAG           = $(cvarsmt)
CFLAGS_ADDITIONAL   = -D_CRT_SECURE_NO_WARNINGS
CPPFLAGS_ADDITIONAL = -EHs -D_CRT_SECURE_NO_WARNINGS /wd4251
LIBS_TOOLCHAIN      = $(conlibs)

TOOLCHAIN_SRCS = 
TOOLCHAIN_INCS = 
TOOLCHAIN_LIBS = 

#------------------------
# BUILD TOOL COMMANDS
#------------------------

# C Compiler: Microsoft Visual C Compiler
CC = cl

# Linker: Microsoft Visual C Linker
LD = link

# C++ Compiler: Microsoft Visual C++ Compiler
CPP = cl

# C++ Linker: Microsoft Visual C++ Linker
CPP_LD = link

# Archiver: Microsoft Visual C/C++ Archiver
AR = lib

# MEX Tool: MEX Tool
MEX_PATH = $(MATLAB_ARCH_BIN)
MEX = "$(MEX_PATH)\mex"

# Download: Download
DOWNLOAD =

# Execute: Execute
EXECUTE = $(PRODUCT)

# Builder: NMAKE Utility
MAKE = nmake


#-------------------------
# Directives/Utilities
#-------------------------

CDEBUG              = -Zi
C_OUTPUT_FLAG       = -Fo
LDDEBUG             = /DEBUG
OUTPUT_FLAG         = -out:
CPPDEBUG            = -Zi
CPP_OUTPUT_FLAG     = -Fo
CPPLDDEBUG          = /DEBUG
OUTPUT_FLAG         = -out:
ARDEBUG             =
STATICLIB_OUTPUT_FLAG = -out:
MEX_DEBUG           = -g
RM                  = @del
ECHO                = @echo
MV                  = @ren
RUN                 = @cmd /C

#--------------------------------------
# "Faster Runs" Build Configuration
#--------------------------------------

ARFLAGS              = /nologo
CFLAGS               = $(cflags) $(CVARSFLAG) $(CFLAGS_ADDITIONAL) \
                       /O2 /Oy-
CPPFLAGS             = /TP $(cflags) $(CVARSFLAG) $(CPPFLAGS_ADDITIONAL) \
                       /O2 /Oy-
CPP_LDFLAGS          = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN)
CPP_SHAREDLIB_LDFLAGS  = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN) \
                         -dll -def:$(DEF_FILE)
DOWNLOAD_FLAGS       =
EXECUTE_FLAGS        =
LDFLAGS              = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN)
MEX_CPPFLAGS         =
MEX_CPPLDFLAGS       =
MEX_CFLAGS           =
MEX_LDFLAGS          =
MAKE_FLAGS           = -f $(MAKEFILE)
SHAREDLIB_LDFLAGS    = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN) \
                       -dll -def:$(DEF_FILE)



###########################################################################
## OUTPUT INFO
###########################################################################

PRODUCT = .\robotIK.lib
PRODUCT_TYPE = "static-library"
BUILD_TYPE = "Static Library"

###########################################################################
## INCLUDE PATHS
###########################################################################

INCLUDES_BUILDINFO = 

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_CUSTOM = 
DEFINES_STANDARD = -DMODEL=robotIK

DEFINES = $(DEFINES_CUSTOM) $(DEFINES_STANDARD)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(START_DIR)\robotIK_data.c $(START_DIR)\rt_nonfinite.c $(START_DIR)\rtGetNaN.c $(START_DIR)\rtGetInf.c $(START_DIR)\robotIK_initialize.c $(START_DIR)\robotIK_terminate.c $(START_DIR)\robotIK.c $(START_DIR)\mod.c $(START_DIR)\rotm2eul.c $(START_DIR)\all.c $(START_DIR)\unique.c $(START_DIR)\sortLE.c $(START_DIR)\RigidBodyTreeUtils.c $(START_DIR)\sqrt.c $(START_DIR)\asin.c $(START_DIR)\sort.c $(START_DIR)\wrapToPi.c $(START_DIR)\sortIdx.c $(START_DIR)\robotIK_rtwutil.c

ALL_SRCS = $(SRCS)

###########################################################################
## OBJECTS
###########################################################################

OBJS = robotIK_data.obj rt_nonfinite.obj rtGetNaN.obj rtGetInf.obj robotIK_initialize.obj robotIK_terminate.obj robotIK.obj mod.obj rotm2eul.obj all.obj unique.obj sortLE.obj RigidBodyTreeUtils.obj sqrt.obj asin.obj sort.obj wrapToPi.obj sortIdx.obj robotIK_rtwutil.obj

ALL_OBJS = $(OBJS)

###########################################################################
## PREBUILT OBJECT FILES
###########################################################################

PREBUILT_OBJS = 

###########################################################################
## LIBRARIES
###########################################################################

LIBS = 

###########################################################################
## SYSTEM LIBRARIES
###########################################################################

SYSTEM_LIBS = 

###########################################################################
## ADDITIONAL TOOLCHAIN FLAGS
###########################################################################

#---------------
# C Compiler
#---------------

CFLAGS_BASIC = $(DEFINES) @$(COMPILER_COMMAND_FILE)

CFLAGS = $(CFLAGS) $(CFLAGS_BASIC)

#-----------------
# C++ Compiler
#-----------------

CPPFLAGS_BASIC = $(DEFINES) @$(COMPILER_COMMAND_FILE)

CPPFLAGS = $(CPPFLAGS) $(CPPFLAGS_BASIC)

###########################################################################
## INLINED COMMANDS
###########################################################################


!include $(MATLAB_ROOT)\rtw\c\tools\vcdefs.mak


###########################################################################
## PHONY TARGETS
###########################################################################

.PHONY : all build clean info prebuild download execute set_environment_variables


all : build
	@cmd /C "@echo ### Successfully generated all binary outputs."


build : set_environment_variables prebuild $(PRODUCT)


prebuild : 


download : $(PRODUCT)


execute : download


set_environment_variables : 
	@set INCLUDE=$(INCLUDES);$(INCLUDE)
	@set LIB=$(LIB)


###########################################################################
## FINAL TARGET
###########################################################################

#---------------------------------
# Create a static library         
#---------------------------------

$(PRODUCT) : $(OBJS) $(PREBUILT_OBJS)
	@cmd /C "@echo ### Creating static library "$(PRODUCT)" ..."
	$(AR) $(ARFLAGS) -out:$(PRODUCT) @$(CMD_FILE)
	@cmd /C "@echo ### Created: $(PRODUCT)"


###########################################################################
## INTERMEDIATE TARGETS
###########################################################################

#---------------------
# SOURCE-TO-OBJECT
#---------------------

.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(START_DIR)}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(START_DIR)}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{C:\Users\User\Documents\MATLAB\ThirdYearProject}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{C:\Users\User\Documents\MATLAB\ThirdYearProject}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


robotIK_data.obj : $(START_DIR)\robotIK_data.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\robotIK_data.c


rt_nonfinite.obj : $(START_DIR)\rt_nonfinite.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\rt_nonfinite.c


rtGetNaN.obj : $(START_DIR)\rtGetNaN.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\rtGetNaN.c


rtGetInf.obj : $(START_DIR)\rtGetInf.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\rtGetInf.c


robotIK_initialize.obj : $(START_DIR)\robotIK_initialize.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\robotIK_initialize.c


robotIK_terminate.obj : $(START_DIR)\robotIK_terminate.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\robotIK_terminate.c


robotIK.obj : $(START_DIR)\robotIK.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\robotIK.c


mod.obj : $(START_DIR)\mod.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\mod.c


rotm2eul.obj : $(START_DIR)\rotm2eul.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\rotm2eul.c


all.obj : $(START_DIR)\all.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\all.c


unique.obj : $(START_DIR)\unique.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\unique.c


sortLE.obj : $(START_DIR)\sortLE.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\sortLE.c


RigidBodyTreeUtils.obj : $(START_DIR)\RigidBodyTreeUtils.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\RigidBodyTreeUtils.c


sqrt.obj : $(START_DIR)\sqrt.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\sqrt.c


asin.obj : $(START_DIR)\asin.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\asin.c


sort.obj : $(START_DIR)\sort.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\sort.c


wrapToPi.obj : $(START_DIR)\wrapToPi.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\wrapToPi.c


sortIdx.obj : $(START_DIR)\sortIdx.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\sortIdx.c


robotIK_rtwutil.obj : $(START_DIR)\robotIK_rtwutil.c
	$(CC) $(CFLAGS) -Fo"$@" $(START_DIR)\robotIK_rtwutil.c


###########################################################################
## DEPENDENCIES
###########################################################################

$(ALL_OBJS) : rtw_proj.tmw $(COMPILER_COMMAND_FILE) $(MAKEFILE)


###########################################################################
## MISCELLANEOUS TARGETS
###########################################################################

info : 
	@cmd /C "@echo ### PRODUCT = $(PRODUCT)"
	@cmd /C "@echo ### PRODUCT_TYPE = $(PRODUCT_TYPE)"
	@cmd /C "@echo ### BUILD_TYPE = $(BUILD_TYPE)"
	@cmd /C "@echo ### INCLUDES = $(INCLUDES)"
	@cmd /C "@echo ### DEFINES = $(DEFINES)"
	@cmd /C "@echo ### ALL_SRCS = $(ALL_SRCS)"
	@cmd /C "@echo ### ALL_OBJS = $(ALL_OBJS)"
	@cmd /C "@echo ### LIBS = $(LIBS)"
	@cmd /C "@echo ### MODELREF_LIBS = $(MODELREF_LIBS)"
	@cmd /C "@echo ### SYSTEM_LIBS = $(SYSTEM_LIBS)"
	@cmd /C "@echo ### TOOLCHAIN_LIBS = $(TOOLCHAIN_LIBS)"
	@cmd /C "@echo ### CFLAGS = $(CFLAGS)"
	@cmd /C "@echo ### LDFLAGS = $(LDFLAGS)"
	@cmd /C "@echo ### SHAREDLIB_LDFLAGS = $(SHAREDLIB_LDFLAGS)"
	@cmd /C "@echo ### CPPFLAGS = $(CPPFLAGS)"
	@cmd /C "@echo ### CPP_LDFLAGS = $(CPP_LDFLAGS)"
	@cmd /C "@echo ### CPP_SHAREDLIB_LDFLAGS = $(CPP_SHAREDLIB_LDFLAGS)"
	@cmd /C "@echo ### ARFLAGS = $(ARFLAGS)"
	@cmd /C "@echo ### MEX_CFLAGS = $(MEX_CFLAGS)"
	@cmd /C "@echo ### MEX_CPPFLAGS = $(MEX_CPPFLAGS)"
	@cmd /C "@echo ### MEX_LDFLAGS = $(MEX_LDFLAGS)"
	@cmd /C "@echo ### MEX_CPPLDFLAGS = $(MEX_CPPLDFLAGS)"
	@cmd /C "@echo ### DOWNLOAD_FLAGS = $(DOWNLOAD_FLAGS)"
	@cmd /C "@echo ### EXECUTE_FLAGS = $(EXECUTE_FLAGS)"
	@cmd /C "@echo ### MAKE_FLAGS = $(MAKE_FLAGS)"


clean : 
	$(ECHO) "### Deleting all derived files..."
	@if exist $(PRODUCT) $(RM) $(PRODUCT)
	$(RM) $(ALL_OBJS)
	$(ECHO) "### Deleted all derived files."


