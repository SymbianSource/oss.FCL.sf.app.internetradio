/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Contains logic for watching certain disk's space
*
*/
#include <pathinfo.h>
#include "irqdiskspaceobserver.h"
#include "irqdiskspaceobserverinterface.h"

const TDriveNumber KQtIRDefaultDrive = EDriveC;

const TUint KDriveAValue = 'A';
const TUint KDriveZValue = 'Z';
const TUint KDriveaValue = 'a';
const TUint KDrivezValue = 'z';

static TDriveNumber phoneMemRootDrive();

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
IRQDiskSpaceObserver* IRQDiskSpaceObserver::NewL(MIRQDiskSpaceObserver* aObserver)
{
    IRQDiskSpaceObserver* self = new (ELeave) IRQDiskSpaceObserver(aObserver);

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    
}

// ---------------------------------------------------------------------------
// Second stage constructor
// ---------------------------------------------------------------------------
//
void IRQDiskSpaceObserver::ConstructL()
{
    CActiveScheduler::Add(this);
    User::LeaveIfError(iFs.Connect());
}

// ---------------------------------------------------------------------------
// C++ Constructor
// ---------------------------------------------------------------------------
//
IRQDiskSpaceObserver::IRQDiskSpaceObserver( MIRQDiskSpaceObserver* aObserver )
                              : CActive( CActive::EPriorityStandard ),       
                                iObserver( aObserver ),
                                iCriticalLevel(0)
{
}
            
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//            
IRQDiskSpaceObserver::~IRQDiskSpaceObserver()
{
    Cancel();
    iFs.Close();    
}

// ---------------------------------------------------------------------------
// Critical Level
// ---------------------------------------------------------------------------
//
TInt64 IRQDiskSpaceObserver::criticalLevel() const
{
    if(IsActive())
    {
        return iCriticalLevel;
    }
    else
    {
        return -1;
    }    
}

// ---------------------------------------------------------------------------
// Start Monitoring
// ---------------------------------------------------------------------------
//
void IRQDiskSpaceObserver::start(const TInt64 aCriticalLevel)
{
    Cancel();
    iCriticalLevel = aCriticalLevel;
    run();
}


// ---------------------------------------------------------------------------
// From class CActive. 
// Cancels the pending request.
// ---------------------------------------------------------------------------
//
void IRQDiskSpaceObserver::DoCancel() 
{    
    iFs.NotifyDiskSpaceCancel(iStatus);
}


// ---------------------------------------------------------------------------
// From class CActive. 
// Invoked when the observed disk's free disk space has run below
// the supplied critical level.
// Notifies the observer and regenerates the request to RFs' NotifyDiskSpace.
// ---------------------------------------------------------------------------
//
void IRQDiskSpaceObserver::RunL()
{       
    run();   
}
    

// ---------------------------------------------------------------------------
// Returns true if below the critical level in defined disk, otherwise false
// ---------------------------------------------------------------------------
//
bool IRQDiskSpaceObserver::isBelowCriticalLevel(const TInt64 aCriticalLevel) const
{
    TVolumeInfo volInfo;
    if(KErrNone == iFs.Volume(volInfo, KQtIRDefaultDrive))
    {
        if(volInfo.iFree < aCriticalLevel)
        {
            return true;
        }
    }
    
    return false;
}

void IRQDiskSpaceObserver::run()
{
    if( iObserver && isBelowCriticalLevel(iCriticalLevel))
    {
        iObserver->notifyLowDiskSpace((qint64)iCriticalLevel);
    }
        
    iFs.NotifyDiskSpace(iCriticalLevel, phoneMemRootDrive(), iStatus);
    SetActive(); 
}

TDriveNumber phoneMemRootDrive()
{
    TParsePtrC phoneMemoryRootPath( PathInfo::PhoneMemoryRootPath() );
    TPtrC drive( phoneMemoryRootPath.Drive() );
    TUint driveValue = drive[0];
    TUint delta = 0;
    if( driveValue >= KDriveAValue &&
        driveValue <= KDriveZValue)
    {
        delta = driveValue - KDriveAValue;
    }
    else if( driveValue >= KDriveaValue &&
            driveValue <= KDrivezValue)
    {
        delta = driveValue - KDriveaValue;
    }
    else
    {
        return KQtIRDefaultDrive;
    }
    
    return static_cast<TDriveNumber>(EDriveA + delta);
}
