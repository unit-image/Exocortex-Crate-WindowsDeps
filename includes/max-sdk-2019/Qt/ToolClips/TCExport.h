//-----------------------------------------------------------------------------
// Copyright 2017 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.
//-----------------------------------------------------------------------------

#ifndef __HToolClipsExportH__
#define __HToolClipsExportH__

#if defined(OSWin_)
#    if defined(BLD_TOOLCLIPS)
#      define TOOLCLIPS_EXPORT __declspec(dllexport)	
#    else
#      define TOOLCLIPS_EXPORT __declspec(dllimport)
#    endif
#elif defined(AW_HIDE_SYMBOLS)
#    define TOOLCLIPS_EXPORT __attribute__ ((visibility("default")))
#else
#    define TOOLCLIPS_EXPORT
#endif

#endif
