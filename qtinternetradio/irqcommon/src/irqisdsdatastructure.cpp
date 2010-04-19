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
#include <QtAlgorithms>
#include <QDataStream>
#include "irqisdsdatastructure.h"
#include "irqenums.h"

void IRQChannelServerURL::internalize(QDataStream &aStream)
{
    aStream>>serverName>>url>>bitrate;
}

void IRQChannelServerURL::externalize(QDataStream &aStream) const
{
    aStream<<serverName<<url<<bitrate;
}

EXPORT_C IRQPreset::IRQPreset()
{
    iIsSorted = false;
    iChannelUrlCount = 0;
}

EXPORT_C IRQPreset::~IRQPreset()
{
    urlArray.clear();
    logoData.Close();
}

EXPORT_C IRQPreset& IRQPreset::operator =(const IRQPreset& aOther)
{
    if (this == &aOther)
        return *this;

    urlArray.clear();
    int count = aOther.urlArray.count();
    IRQChannelServerURL aURL;
    for (int i = 0; i < count; i++)
    {
        aURL.bitrate = aOther.urlArray[i].bitrate;
        aURL.serverName = aOther.urlArray[i].serverName;
        aURL.url = aOther.urlArray[i].url;
        this->urlArray.append(aURL);
    }

    this->iIsSorted = aOther.iIsSorted;
    this->imgUrl = aOther.imgUrl;
    this->advertisementInUse = aOther.advertisementInUse;
    this->advertisementUrl = aOther.advertisementUrl;
    this->bitrate = aOther.bitrate;
    this->genreId = aOther.genreId;
    this->genreName = aOther.genreName;
    this->shortDesc = aOther.shortDesc;
    this->description = aOther.description;
    this->logoData.Close();
    this->logoData.Create(aOther.logoData);
    this->musicStoreStatus = aOther.musicStoreStatus;
    this->lastModified = aOther.lastModified;
    this->countryCode = aOther.countryCode;
    this->countryName = aOther.countryName;
    this->languageCode = aOther.languageCode;
    this->languageName = aOther.languageName;
    this->name = aOther.name;
    this->presetId = aOther.presetId;
    this->index = aOther.index;
    this->type = aOther.type;
    this->iChannelUrlCount = aOther.iChannelUrlCount;
    this->uniqID = aOther.uniqID;
    return *this;
}

EXPORT_C void IRQPreset::internalize(QDataStream &aStream)
{
    aStream>>uniqID>>presetId>>index>>type>>iChannelUrlCount;
    
    aStream>>languageCode>>languageName 
           >>countryCode>>countryName
           >>genreName>>genreId
           >>name>>lastModified
           >>advertisementUrl>>advertisementInUse
           >>imgUrl>>description>>shortDesc
           >>musicStoreStatus;
    
    urlArray.clear();
    for (int i = 0; i < iChannelUrlCount; ++i)
    {
        IRQChannelServerURL url;
        url.internalize(aStream);
        urlArray.append(url);
    }
}

EXPORT_C void IRQPreset::externalize(QDataStream &aStream) const
{
    aStream<<uniqID<<presetId<<index<<type<<iChannelUrlCount;
    
    aStream<<languageCode<<languageName
           <<countryCode<<countryName
           <<genreName<<genreId
           <<name<<lastModified
           <<advertisementUrl<<advertisementInUse
           <<imgUrl<<description<<shortDesc
           <<musicStoreStatus;
    
    for (int i = 0; i < iChannelUrlCount; ++i)
    {
        urlArray.at(i).externalize(aStream);
    }
}
   
//get the bitrates of the preset. Duplicated bitrate will just save once.
EXPORT_C void IRQPreset::getAvailableBitrates(QList<int>& aBitrates) const
{
    aBitrates.clear();
    int bitrate = 0;

    for (int i = 0; i < urlArray.count(); i++)
    {
        bitrate = urlArray[i].bitrate;
        if (!aBitrates.contains(bitrate))
        {
            aBitrates.append(bitrate);
        }
    }
}

//get the url for the specify bitrate
//the caller should delete the QList 
//
EXPORT_C QList<QString> * IRQPreset::getURLsForBitrate(int aBitrate) const
{
    int upLimit = urlArray.count();
    
    //if the urls list has no item
    if( 0 == upLimit )
    {
        return NULL;
    }
    
    //check if the bitrate in our range
    QList<int> bitRateList;
    getAvailableBitrates(bitRateList);
    if(!bitRateList.contains(aBitrate))
    {         
        return NULL;
    }
    
    QList<QString> *stringList = new QList<QString> (); 
    for (int i = 0; i < upLimit; i++)
    {
        if (urlArray[i].bitrate == aBitrate)
        {
            stringList->append(urlArray[i].url);
        }
    }

    return stringList;
}

//the following functions may be changed in future 
//
EXPORT_C int IRQPreset::getChannelUrlAt(int aIndex, QString& aURL) const
{
    int upLimit = urlArray.count();

    if (aIndex < 0 || aIndex >= upLimit)
        return EIRQErrorBadParameter;

    aURL = urlArray[aIndex].url;
    return EIRQErrorNone;
}

EXPORT_C int IRQPreset::getChannelBitrate(int aIndex, int &aBitrate) const
{
    if (aIndex >= 0 && aIndex < urlArray.count())
    {
        aBitrate = urlArray[aIndex].bitrate;
        return EIRQErrorNone;
    }
    
    return EIRQErrorBadParameter;
}

//append a channel server into a internal list 
//
EXPORT_C void IRQPreset::insertChannelServer(const IRQChannelServerURL& aURL)
{
    urlArray.append(aURL);
    iChannelUrlCount++;
    iIsSorted = false;
}

EXPORT_C void IRQPreset::clearChannelServerList()
{
    urlArray.clear();
    iChannelUrlCount = 0;
    iIsSorted = false;
}

//sort the urlArray with bitrates by ascending. for 
//there are not many items, here we use bubble sorting.
//
EXPORT_C void IRQPreset::sortURLArray()
{
    if (iIsSorted)
        return;

    qSort(urlArray.begin(), urlArray.end(),  bitrateLessThan);

    iIsSorted = true;
}

EXPORT_C bool IRQPreset::setUrlBitrate(const int aUrlIndex, const int aBitrate)
{
    if(aUrlIndex < 0 || aUrlIndex > (urlArray.count()-1) )
    {
        return false;
    }
    
    urlArray[aUrlIndex].bitrate = aBitrate;
    return true;
}

//check wether the internal list is sorted 
//
EXPORT_C bool IRQPreset::isSorted() const
{
    return iIsSorted;
}

EXPORT_C int IRQPreset::getChannelURLCount() const
{
    return iChannelUrlCount;
}

bool bitrateLessThan(const IRQChannelServerURL &url1,
        const IRQChannelServerURL &url2)
{
    return url1.bitrate < url2.bitrate;
}

 



