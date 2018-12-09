#pragma once

//**************************************************************************/
// Copyright (c) 2017 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// AUTHOR: Tristan Lapalme
//***************************************************************************/

#ifdef NOTIFICATION_API_UTILS_MODULE
#define NotificationAPIUtilsExport __declspec(dllexport)
#else
#define NotificationAPIUtilsExport __declspec(dllimport)
#endif

#include <strclass.h>
#include <maxapi.h>

namespace MaxSDK
{;

/** 
* Struct NotificationAPIUtils.
* This namespace exposes 4 private internal calls to the 3dsmax application.
* Useful for activeshade renderers to keep track of the active view.
*/	
namespace NotificationAPIUtils
{;
	/** 
	* Gets Active ViewExp considering extended and non extended views.
	* \param IsAnExtendedView will be true if the view is an extended view.
	*/
	NotificationAPIUtilsExport ViewExp* GetActiveViewExpIncludingExtendedViews(bool& IsAnExtendedView);
	
	/**
	* Gets the ViewExp from its view undoID considering extended views and all tab panels not only the active tab panel.
	* \param viewUndoID the view undo ID
	* \param outIsAnExtendedView will be true if the view is an extended view.
	*/	
	NotificationAPIUtilsExport ViewExp* GetViewExpFromUndoIDIncludingExtendedViews(int viewUndoID, bool& outIsAnExtendedView);
	
	/**
	* Gets the viewport label from its view undoID considering extended views and all tab panels not only the active tab panel.
	* \param viewUndoID the view undo ID
	* \param outViewportLabel the viewport label.
	*/
	NotificationAPIUtilsExport void GetViewportLabelFromUndoIDIncludingExtendedViews(int viewUndoID, TSTR& outViewportLabel);
	
	/**
	* To know if the renderer is locked
	* \param renderSettingID the render setting ID
	*/
	NotificationAPIUtilsExport bool IsUsingActiveView(RenderSettingID renderSettingID);
};

};//end of namespace MaxSDK