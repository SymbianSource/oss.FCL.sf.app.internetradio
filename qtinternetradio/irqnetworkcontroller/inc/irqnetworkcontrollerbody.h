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
* Description:
*
*/
#ifndef IRQNETWORKCONTROLLERBODY_H_
#define IRQNETWORKCONTROLLERBODY_H_

#include <QObject>
#include "irnetworkcontroller.h"
#include "irqenums.h"
#include "irqevent.h"

/**
 * This class implements the IR Network Controller component
 */

class IRQNetworkControllerBody : public QObject
                               , public MIRActiveNetworkObserver
                               , public MIRNetworkController
{
    Q_OBJECT

public:
    /**
     *  Create resources
     */
    void initL();
    /**
     *  Default C++ Destructor
     */
    ~IRQNetworkControllerBody();

    /**
     *  Return the variable which indicates if connection is active or not
     *  @return bool
     */
    bool getNetworkStatus() const;

    /**
     *  Return the IAP Id of the chosen IAP
     *  @return int Error code
     */
    IRQError getIAPId(unsigned long& aIapId) const;

    /**
     *  Returns the list of available access points
     *  @return QStringList &aList specifying the access point names
     */
    void getAccessPointList(QStringList &aList);

    /**
     *  Returns the list of iap ids for available access points
     *  @return QList<unsigned long> &aList specifying the iap ids
     */
    void getApIdList(QList<unsigned long> &aList);

    /**
     *  Returns the list of bearer ids for available access points
     *  @return QList<unsigned long> &aList specifying the bearer ids
     */
    void getBearerList(QList<unsigned long> &aList);

    /**
     *  Returns the list of network ids for available access points
     *  @return QList<unsigned long> &aList specifying the network ids
     */
    void getNetworkList(QList<unsigned long> &aList);
    
    /**
     *  Configure the Access Point which is used by all the components for network connectivity
     */
    void  chooseAccessPoint();

    /*
     * Cancel configuring access point
     */
    void cancelConnecting();
    
    /**
     *  This api is used to determine if the phone is in offline mode
     *  @return True if the phone is in offline mode else False
     */
    bool isOfflineMode();

    /**
     *  This api is used to determine if the phone supports WLan usage
     *  @return True if the phone supports else False
     */
    bool isWlanSupported() const;

    /**
     *  Reset the connection status to Disconnected state
     */
    void resetConnectionStatus();

    /**
     *  Used to determine the type of connection
     *  @return enum describing the type of connection ( GPRS/3G/WiFi )
     */
    IRQConnectionType identifyConnectionType() const;

    /**
     *  Notifies all observers whose network request is active to reissue the request
     */
    void notifyActiveNetworkObservers(IRQNetworkEvent aEvent);

    /**
     *  Indicates if the hand over of network connection has happened
     */
    bool isHandlingOverConnection();

    /**
     *  Indicates if chooseAccessPoint is called
     */
    bool isConnectRequestIssued() const;

    /**
     *  MIRActiveNetworkObserver::NotifyActiveNetworkObserversL()
     *  Callback which notifies all observers whose network request is active to reissue the request
     *  @param aEvent Indicates the type of network event that occurred
     */
    void NotifyActiveNetworkObserversL(TIRNetworkEvent aEvent);

    /**
     *  MIRActiveNetworkObserver::ResetPendingRequests()
     *  Callback which notifies all observers whose network request is active to reset the pending
     *  request status
     */
    void ResetPendingRequests(TBool aValue);

    /**
     *  MIRNetworkController::IRNetworkEventL()
     *  @param aEvent Indicates the type of network event that occurred
     */
    void IRNetworkEventL(TIRNetworkEvent aEvent);
    
signals:

    /**
     *  Notifies all observers whose network request is active to reissue the request
     *  Called in NotifyActiveNetworkObserversL()
     */
    void networkRequestNotified(IRQNetworkEvent aEvent);

    /**
     *  Notifies all observers whose network request is active to reset the pending request status
     *  Called in ResetPendingRequests()
     */
    void pendingRequestsReset(bool aValue);

    /**
     *  Notifies the type of network event that occurred
     *  Called in IRNetworkEventL
     */
    void networkEventNotified(IRQNetworkEvent aEvent);

    /**
     *  Notifies the error
     */
    void errorOccured(IRQError aError);

private:

    /**
     *  NetworkController singleton instance
     */
    CIRNetworkController* iNetworkController;
};

#endif /* IRQNETWORKCONTROLLERBODY_H_ */