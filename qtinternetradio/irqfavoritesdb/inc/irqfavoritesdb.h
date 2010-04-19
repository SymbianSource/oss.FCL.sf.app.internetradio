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
#ifndef IRQFAVORITESDB_H
#define IRQFAVORITESDB_H

#include <QObject>
#include <pspresetobserver.h> 

class CIRFavoritesDb;
class IRQPreset;

class IRQFavoritesDB : public QObject, public MPSPresetObserver
{
Q_OBJECT
public:

    IMPORT_C IRQFavoritesDB();

    ~IRQFavoritesDB();

    /* add a preset 
     * @param IRQPreset& the isds preset,
     * @return  errcode
     */
    IMPORT_C int addPreset(const IRQPreset& aPreset);

    /* add a preset manually
     * @return  errcode
     * @param 
     */
    IMPORT_C int addPreset(const QString& aName, const QString& aURL);

    /* get a preset uniq id
     * @return 
     * @param
     */
    IMPORT_C int getUniqId(int aNum);
    /* delete a preset by uniq id
     * @return errcode
     * @param
     */
    IMPORT_C int deletePreset(int aUniqId);
    /* search a preset by uniqpresetId / isdspresetid
     * warning: the function needs further checking
     *  @return errcode
     */
    IMPORT_C int searchPreset(int aUniqPresetId, int aIsdsPresetId);

    /* get the previouse preset index in the internal list
     * @return the index
     */
    IMPORT_C int getPreviousPreset(int aIndex);

    /* get the next preset index
     * @return the index
     */
    IMPORT_C int getNextPreset(int aIndex); 

    /* replace with new preset
     * @return errcode 
     */
    IMPORT_C int replacePreset(const IRQPreset& aNewPreset);

    /* @return errcode
     */
    IMPORT_C int replaceUserDefinedPreset(const IRQPreset& aNewPreset);

    /* change the preset type to user defined
     * @return errcode
     */
    IMPORT_C int makePresetUserDefined(int aChannelId,
            int aUserDefinedChannelId);

    /* get the empty preset left count
     * @return the count of empty presets left 
     */
    IMPORT_C int emptyPresetCount() const;

    /* get the max preset count supported now
     * @return errcode 
     */
    IMPORT_C int maxPresetCount();

    /* the interface is not used currently.
     **/
    IMPORT_C void setMoveStatus(bool aStatus);

    /* the interface is not used currently 
     */
    IMPORT_C bool getMoveStatus();

    /* for CIRPreset is just an interface so we can wrapper it into the IRQPreset.
     * the interface get the IRQPreset list. The upper layer will free all the items
     * in the list and the list self*/
    IMPORT_C QList<IRQPreset*>* getPresets() const;

    /*
     * Increase the played times of the preset if it's in the favorites
     */
    IMPORT_C void increasePlayedTimes(const IRQPreset &aPreset);
    
    /*
     * get the CIRFavoritesDb 
     */
    IMPORT_C CIRFavoritesDb * getCIRFavoriteDB() const;

signals:
    /*
     * the signal will triggered when we receive the message that preset is changed
     */
    void presetChanged(int aId, TUid aDataHandler,
            MPSPresetObserver::TPSReason aType);

private:
    CIRFavoritesDb * iIRFavoritesDb;
    void addPresetL(const IRQPreset& aPreset, int& aRetValue);
    void replacePresetL(const IRQPreset& aNewPreset);
    void replaceUserDefinedPresetL(const IRQPreset& aNewPreset);
    void increasePlayedTimesL(const IRQPreset &aPreset);
    
private:
    /* from MPSPresetObserver */
    void HandlePresetChangedL(TInt aId, TUid aDataHandler,
            MPSPresetObserver::TPSReason aType);
};
#endif