#
# Makefile: builds the Proxies for BOARD_PROXI
#

CC = cc
CXX = CC
MAKE = gmake
CFLAGS = $(FLAGS) -g -I. -I$(DIMDIR)/dim -I$(SMIDIR)/smixx
EXTRALIBS =
ODIR =

ifeq ($(OS),SunOS)
	ODIR=sunos
endif
ifeq ($(OS),Solaris)
	EXTRALIBS = -lpthread -lposix4 -lsocket -lnsl
	FLAGS += -mt
	ODIR = solaris
endif
ifeq ($(OS),HP-UX)
	ODIR = hp
endif
ifeq ($(OS),OSF1)
	ODIR = osf
endif
ifeq ($(OS),AIX)
	ODIR = aix
endif
ifeq ($(OS),Linux)
	EXTRALIBS = -lpthread
	CXX = g++
	ODIR = linux
endif
all:	$(ODIR)/boardclass smidom smiproxy

$(ODIR)/boardclass : boardclass.cxx
	$(CXX) $(CFLAGS) boardclass.cxx -L$(SMIDIR)/$(ODIR) -lsmi -L$(DIMDIR)/$(ODIR) -ldim $(EXTRALIBS) -o $(ODIR)/boardclass

.PHONY: smidom
smidom: hv_test.smi
	killall smiSM;
	smiTrans hv_test.smi;
	smiSM CLAS12 hv_test.sobj &

.PHONY: smiproxy
smiproxy: $(ODIR)/boardclass
	killall boardclass;
	$(ODIR)/boardclass CLAS12

clean:
	rm $(ODIR)/boardclass *~;
	killall smiSM; killall boardclass;
