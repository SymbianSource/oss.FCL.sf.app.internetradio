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

#include <hbicon.h>
#include <QBrush>

#include "irchannelmodel.h"
#include "irqisdsdatastructure.h"
#include "iruidefines.h"

IrChannelModel::IrChannelModel(QObject *aParent): QAbstractListModel(aParent)
                                                , iChannelList(NULL)
{
    QIcon icon(":/stationlist/icon_stationdefault.png");
    iStationLogo = new HbIcon(icon);   
}

IrChannelModel::~IrChannelModel()
{
    clearAndDestroyItems();

    delete iStationLogo;
    iStationLogo = NULL;
    
    clearAndDestroyLogos();
}

int IrChannelModel::rowCount(const QModelIndex &aParent) const
{
    Q_UNUSED(aParent);
    int count = 0;
    
    if(iChannelList)
    {
        count = iChannelList->count();
    }
    return count;
}

QString IrChannelModel::imageUrl(int aRow)
{
    if (iChannelList)
    {
        return iChannelList->at(aRow)->imageURL;
    }
    else
    {
        return "";
    }
}

void IrChannelModel::setLogo(HbIcon *aIcon, int aIndex)
{
    iLogos[aIndex] = aIcon;
    emit dataChanged(index(aIndex), index(aIndex));
}

QVariant IrChannelModel::data(const QModelIndex &aIndex, int aRole) const
{
    if (!aIndex.isValid())
        return QVariant();

    if (aIndex.row() >= iChannelList->count())
        return QVariant();

    if (aRole == Qt::DisplayRole)
    {
        QVariantList list;
        int row = aIndex.row();

        list.append(iChannelList->at(row)->channelName);       
        // fix bug #9888,if left descriptions as blank, only one line appears
        QString tempDes = iChannelList->at(row)->shortDescription;        
        if( 0 == tempDes.length() )
        {
            tempDes = " ";
        }
        list.append(tempDes);       
        
        return list;
    }
    else if (aRole == Qt::DecorationRole)
    {
        QVariantList list;
        int row = aIndex.row();
        const HbIcon *icon = iLogos.value(row);
        if (icon)
        {
            list.append(*icon);
        }
        else
        {
            list.append(*iStationLogo);
        }
        
        return list;
    }
    else if (aRole == Qt::BackgroundRole)
    {
        if (aIndex.row() % 2 == 0)
        {
            return QBrush(KListEvenRowColor);
        }
        else
        {
            return QBrush(KListOddRowColor);
        }
    }
    else
    {
        return QVariant();
    }
}

void IrChannelModel::updateData(QList<IRQChannelItem*> *aPushItemsList)
{
    if (iChannelList != aPushItemsList)
    {
        clearAndDestroyItems();
        iChannelList = aPushItemsList;
    }
    
    clearAndDestroyLogos();
    
    emit dataAvailable();
}

void IrChannelModel::clearAndDestroyLogos()
{
    for (QMap<int, HbIcon*>::iterator it = iLogos.begin(); it != iLogos.end(); ++it)
    {
        delete it.value();
    }
    
    iLogos.clear();
}

void IrChannelModel::clearAndDestroyItems()
{
    if (iChannelList)
    {
        for (QList<IRQChannelItem*>::iterator it = iChannelList->begin(); it != iChannelList->end(); ++it)
        {
            delete *it;
        }
        delete iChannelList;
        iChannelList = NULL;
    }
}