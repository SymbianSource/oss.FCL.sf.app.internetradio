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
#ifndef IRQISDSDATASTRUCTURE_H
#define IRQISDSDATASTRUCTURE_H
#include <QList>
#include <QString>

#include <e32base.h>

#define DEBUG_ISDS_DATA 0

class QDataStream;

class IRQBrowseCategoryItem
{
public:  
  QString catName;
  int     size;
};


class IRQChannelItem
{
public:
   QString channelName;
   QString shortDescription;
   QString imageURL;
};

class IRQChannelServerURL
{
public:
    void internalize(QDataStream &aStream);
    void externalize(QDataStream &aStream) const;
    
public:
   QString serverName;
   QString url;
   int     bitrate;
};

class IRQPreset
{
public:
  enum IRQPresetType  
    {   
      EUser,
      EIsds
    };
  
   
   IMPORT_C IRQPreset();
   IMPORT_C ~IRQPreset();  
   IMPORT_C IRQPreset& operator = (const IRQPreset& aOther); 
   
   IMPORT_C void internalize(QDataStream &aStream);
   IMPORT_C void externalize(QDataStream &aStream) const;
   
   /* get the bitrates of the preset.
    */
   IMPORT_C void getAvailableBitrates(QList<int>& aBitrates) const;
   /* get the url for the specify bitrate
    * the caller should delete the QList */
   IMPORT_C QList<QString> * getURLsForBitrate(int aBitrate) const;
   /* the following functions may be changed in future */
   IMPORT_C int getChannelUrlAt(int aIndex, QString& aURL) const;   
   IMPORT_C int getChannelBitrate(int aIndex, int &aBitrate) const;    
   
   /* sort the urlArray with bitrates by ascending. for 
    * there are not many items, here we use bubble sorting.
    */
   IMPORT_C void sortURLArray();
   /* check wether the internal list is sorted 
    */
   IMPORT_C bool isSorted() const;
   /* append a channel server into a internal list 
    */
   IMPORT_C void insertChannelServer(const IRQChannelServerURL& aURL);
   /* clear the internal list 
    */
   IMPORT_C void clearChannelServerList();   
   
   IMPORT_C int getChannelURLCount() const;
   
   IMPORT_C bool setUrlBitrate(const int aUrlIndex, const int aBitrate);
  
   int uniqID; //unique id generated for the preset saved at favourite
   
   int type; //0 for user defined 1 for isds type.
   int index;//index in the preset table
   int presetId;//isds generated preset id
   QString name; //preset name        
   QString languageCode;//preset language code
   QString languageName; // preset language
   QString countryCode;//preset country of origin code     
   QString countryName;//preset country of origin      
   QString lastModified;//preset last modified     
   QString musicStoreStatus;//musicStoreEnabled value
   //QByteArray logoData;//Logo Data
   RBuf8   logoData;
   QString description;//channel description       
   QString shortDesc;//channel short desription    
   QString genreName;//channel genre name 
   QString genreId;//channel genre id     
   int bitrate;//channel bitrate

   //in our spec but not in current code 
   QString advertisementUrl; 
   QString advertisementInUse; 
   QString imgUrl;        
   
 
private:
  /* for one preset, there may be many servers*/
  QList<IRQChannelServerURL> urlArray;
  bool iIsSorted;
  int  iChannelUrlCount; //no of channel urls
  
private:
    friend class IRQUtility;
 
};

bool bitrateLessThan(const IRQChannelServerURL &url1, const IRQChannelServerURL &url2);

#endif /* IRQISDSDATASTRUCTURE_H */