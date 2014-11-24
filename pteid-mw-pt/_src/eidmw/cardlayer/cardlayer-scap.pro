######################################################################
# Automatically generated by qmake (2.01a) Thu Dec 13 13:25:01 2007
######################################################################


include(../_Builds/eidcommon.mak)

TEMPLATE = lib
TARGET = pteid-scap-cardlayer
VERSION = $${CARDLAYERLIB_MAJ}.$${CARDLAYERLIB_MIN}.$${CARDLAYERLIB_REV}

message("Compile $$TARGET")

###
### Installation setup
###
target.path = $${INSTALL_DIR_LIB}
INSTALLS += target


CONFIG -= warn_on qt

## destination directory
OBJECTS_DIR = scap-obj
DESTDIR = ../lib
DEPENDPATH += . 
macx: LIBS += -L../lib -l$${COMMONLIB}
macx: LIBS += -l$${DLGLIB} \
              -Wl,-framework -Wl,PCSC 	

unix:LIBS += -L../lib -l$${COMMONLIB}
unix:LIBS += -l$${DLGLIB}
!macx:LIBS += -lpcsclite

INCLUDEPATH += . ../common 
INCLUDEPATH += $${PCSC_INCLUDE_DIR}

QMAKE_CXXFLAGS += -O2

DEFINES += PTEID_SCAP \
           EIDMW_CAL_EXPORT \
           CARDPLUGIN_IN_CAL CAL_PTEID
unix:!macx:  DEFINES += __UNIX__
#macx:  DEFINES += __OLD_PCSC_API__
#Support Fat binaries on Mac with both x86 and x86_64 architectures
macx: CONFIG += x86


# Input
HEADERS += \
           Cache.h \
           Card.h \
           CardFactory.h \
           CardLayer.h \
           CardLayerConst.h \
           Context.h \
           InternalConst.h \
           P15Correction.h \
           P15Objects.h \
           PCSC.h \
           GenericPinpad.h \
           Pinpad.h \
           PKCS15.h \
           PKCS15Parser.h \
           PkiCard.h \
           Reader.h \
           ReadersInfo.h \
           ThreadPool.h \
           UnknownCard.h \
           pinpad2.h \
	   GempcPinpad.h \
	   ACR83Pinpad.h \
           CardReaderInfo.h

SOURCES += \
           Cache.cpp \
           Card.cpp \
           CardFactory.cpp \
           CardLayer.cpp \
           CardReaderInfo.cpp \
           Context.cpp \
           PCSC.cpp \
           Pinpad.cpp \
           GenericPinpad.cpp \
           P15Correction.cpp \
           PKCS15.cpp \
           PKCS15Parser.cpp \
           PkiCard.cpp \
           Reader.cpp \
           ReadersInfo.cpp \
           ThreadPool.cpp \
	   GempcPinpad.cpp \
	   ACR83Pinpad.cpp \
           UnknownCard.cpp


##
## Headers and sources specific to a country
##

## do not define a conditional block with contains(PKG_NAME,pteid)
## otherwise the script which prepares the tarball will not
## be able to parse the project file correctly!
contains(PKG_NAME,pteid): HEADERS += cardpluginPteid/PteidCard.h \
          			cardpluginPteid/PteidP15Correction.h

contains(PKG_NAME,pteid): SOURCES +=  cardpluginPteid/PteidCard.cpp \
        	   		cardpluginPteid/PteidP15Correction.cpp 

contains(PKG_NAME,pteid): INCLUDEPATH += cardpluginPteid
