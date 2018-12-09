//-----------------------------------------------------------------------------
// Copyright 2017 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.
//-----------------------------------------------------------------------------

#pragma once

#include <CoreExport.h>

#pragma warning ( push )
#pragma warning ( disable: 4127 4251 4275 4800 )
#include <QtCore/qobject.h>
#pragma warning ( pop )


/** \brief This message is sent to the parent of a Win32 ui control
* such as a CustButton in order to retrieve tool tip key.
* The WPARAM will be the ui controls Id and the LPARAM is a pointer to a MSTR 
* for returning the tool tip key string.
*/
#define WM_GET_TOOLTIP_KEY (WM_USER + 0x3320)

namespace MaxSDK
{

/** \brief The QmaxToolClips class provides a collection of tool clip related methods.
* The new tool clips replace the common Win32 / Qt tooltips and provide richer, 
* contextual information in a highly graphic fashion. 
* The tool clips are multi-staged, interactive and can offer animated contextual help.
* On level 1 the tool clips show just a title and shortcut information.
* Level 2 additionally offers a description text and a video or image.
* Level 3 can show up an extended description text and a resource list which can 
* contain links to help content like videos, images and web pages.
* The content for the tool clips is provided by xml control files, where a key
* that identifies a content block must match with the tool clip key that a ui control
* provides when the mouse hovers over it.
*/
class CoreExport QmaxToolClips
{
public:

    /** \brief Disables the tool clip feature on the specified ui object.
    * \param obj The ui component on which the tool clip should be disabled / enabled.
    * \param disable True if the tool clip should be disabled on the given ui component, false otherwise.
    */
    static void disableToolClip( QObject* obj, bool disable = true );

    /** \brief Retrieves if for a given ui object the tool clip feature is disabled.
    * \param obj The ui component for which the tool clip disabled state should be queried.
    * \return true if the tool clip is disabled for the given object, otherwise false.
    */
    static bool isToolClipDisabled( QObject* obj );

};

}

