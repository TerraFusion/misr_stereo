#! /bin/bash

#OBJS := $(MISRDIR)/obj/glutaux.o $(MISRDIR)/obj/hdfDataNode.o $(MISRDIR)/obj/hdfDataSource.o $(MISRDIR)/obj/hdfField.o \
#        $(MISRDIR)/obj/hdfFile.o $(MISRDIR)/obj/hdfGrid.o $(MISRDIR)/obj/help.o $(MISRDIR)/obj/main.o $(MISRDIR)/obj/stereoviewer.o \
#        $(MISRDIR)/obj/stringaux.o $(MISRDIR)/obj/viewport.o

bin/misr-stereo : $(MISRDIR)/lib/libgctp.a $(MISRDIR)/lib/libhdfeos.a $(MISRDIR)/stereo/stereo.pro
	$(QTBIN)/qmake -o $(MISRDIR)/stereo/Makefile $(MISRDIR)/stereo/stereo.pro
	cd $(MISRDIR)/stereo ; make 

$(MISRDIR)/lib/libgctp.a : $(MISRDIR)/gctp/gctp.pro
	$(QTBIN)/qmake -o $(MISRDIR)/gctp/Makefile $(MISRDIR)/gctp/gctp.pro
	cd $(MISRDIR)/gctp ; make

$(MISRDIR)/lib/libhdfeos.a : $(MISRDIR)/hdfeos/hdfeos.pro
	$(QTBIN)/qmake -o $(MISRDIR)/hdfeos/Makefile $(MISRDIR)/hdfeos/hdfeos.pro
	cd $(MISRDIR)/hdfeos ; make

$(MISRDIR)/hdfeos/hdfeos.pro : $(MISRDIR)/proto/hdfeos.proto
	sed -e"s:HDFINCDIR:$(HDF4INC):" -e"s:SZBASE:$(SZIPDIR):" < $(MISRDIR)/proto/hdfeos.proto > $(MISRDIR)/hdfeos/hdfeos.pro

$(MISRDIR)/stereo/stereo.pro : $(MISRDIR)/proto/stereo.proto
	sed -e"s:MISRBASE:$(MISRDIR):" -e"s:HDFINCDIR:$(HDF4INC):" -e"s:HDFLIBDIR:$(HDF4LIB):" -e"s:SZBASE:$(SZIPDIR):" < $(MISRDIR)/proto/stereo.proto > $(MISRDIR)/stereo/stereo.pro

clean :
	cd $(MISRDIR)
	rm -f bin/misr-stereo gctp/obj/*.o hdfeos/obj/*.pro lib/lib*.a stereo/obj/*.o

distclean :
	cd $(MISRDIR)
	rm -f bin/misr-stereo lib/lib*.a
	rm -f gctp/obj/*.o hdfeos/obj/*.o stereo/obj/*.o
	rm -f build.csh hdfeos/hdfeos.pro stereo/stereo.pro

