TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#=============================
# Build the IOC support library

#LIBRARY_IOC_DEFAULT += -nil-
LIBRARY_IOC_vxWorks += scaler_dSupport
# xxxRecord.h will be created from xxxRecord.dbd
##DBDINC += xxxRecord
# Install devXxxSoft.dbd into <top>/dbd
##DBD += xxxSupport.dbd
DBD += scaler_dSupport.dbd
scaler_dSupport_DBD += base.dbd
scaler_dSupport_DBD += asyn.dbd

scaler_dSupport_DBD += drvAsynIPPort.dbd
scaler_dSupport_DBD += stdSupport.dbd
scaler_dSupport_DBD += calcSupport.dbd
scaler_dSupport_DBD += busySupport.dbd 
scaler_dSupport_DBD += sscanSupport.dbd 
scaler_dSupport_DBD += asSupport.dbd
scaler_dSupport_DBD += devSequencer.dbd
scaler_dSupport_DBD += stream.dbd
scaler_dSupport_DBD += streamSynApps.dbd
scaler_dSupport_DBD += vmeVXSupport.dbd
scaler_dSupport_DBD += std.dbd



scaler_dSupport_LIBS += $(EPICS_BASE_IOC_LIBS)


#=============================
# Build the IOC application

##PROD_IOC = scaler_d
PROD_IOC_vxWorks = scaler_d
# scaler_d.dbd will be created and installed
DBD += scaler_d.dbd

# scaler_d.dbd will be made up from these files:
scaler_d_DBD += scaler_dSupport.dbd

# scaler_d_registerRecordDeviceDriver.cpp derives from scaler_d.dbd

# Build the main IOC entry point on workstation OSs.
scaler_d_SRCS_DEFAULT += scaler_dMain.cpp
#scaler_d_SRCS_vxWorks += -nil-
scaler_d_SRCS_vxWorks += scaler_d_registerRecordDeviceDriver.cpp

# Add support from base/src/vxWorks if needed
scaler_d_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Link in the code from the support library
scaler_d_LIBS += scaler_dSupport
scaler_d_LIBS += asyn
scaler_d_LIBS += mca
scaler_d_LIBS += calc
scaler_d_LIBS += busy
scaler_d_LIBS += sscan
scaler_d_LIBS += autosave
scaler_d_LIBS += vme
scaler_d_LIBS += std
scaler_d_LIBS += stream streamSynApps
scaler_d_LIBS += asyn
scaler_d_LIBS += seqDev seq pv 


# NOTE: To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file

ifneq ($(SNCSEQ),)
    # Build sncExample into scaler_dSupport
##    sncExample_SNCFLAGS += +r
##    scaler_d_DBD += sncExample.dbd
##    scaler_dSupport_SRCS += sncExample.stt
#    scaler_d_restart_SNCFLAGS += +r
    scaler_dSupport_DBD += scaler_dSNC.dbd
    scaler_dSupport_SRCS += scaler_d_restart.st
    scaler_d_SRCS += scaler_d_restart.st
    scaler_dSupport_LIBS += seq pv
##    scaler_d_LIBS += seq pv

    # Build sncProgram as a standalone program
##    PROD_HOST += sncProgram
##    sncProgram_SNCFLAGS += +m
##    sncProgram_SRCS += sncProgram.st
##    sncProgram_LIBS += seq pv
##    sncProgram_LIBS += $(EPICS_BASE_HOST_LIBS)
endif

# Finally link to the EPICS Base libraries
scaler_d_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

