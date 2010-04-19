# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:  
#
# Description:

TEMPLATE = lib

TARGET = irqnwkinfoobserver
TARGET.CAPABILITY = CAP_GENERAL_DLL ReadDeviceData

MOC_DIR = moc
DEPENDPATH += src

INCLUDEPATH += inc

symbian{
  TARGET.UID3 = 0xe906e618
  TARGET.EPOCALLOWDLLDATA = 1
}

LIBS += -letel3rdparty

# Input
HEADERS += inc\irqnwkinfoobserver.h \
           inc\irqnwkinfoobserverimpl.h \
           inc\irqnwkobserverinterface.h

SOURCES += irqnwkinfoobserver.cpp \
           irqnwkinfoobserverimpl.cpp

SYMBIAN_PLATFORMS = WINSCW ARMV5

!exists( $(RVCT22BIN) ) {       
        !exists( $(RVCT31BIN) ) {
           exists( $(RVCT40BIN) ) {       
               QMAKE_CFLAGS.ARMCC += --import_all_vtbl   
           }           
        }
 }