//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2017 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <tchar.h>
#include <wtypes.h>
#include <ObjIdl.h>
#include <vector>
#include <memory>

#ifdef DllExport
#undef DllExport
#endif
#ifdef BLD_CUSTOMFILESTREAM_API
#define DllExport __declspec( dllexport )
#else
#define DllExport __declspec( dllimport )
#endif

/*! \brief This API is used to support OLE structured storage streams containing user specified string data in 3ds Max scene files or 
other OLE Structured Storage based files.  See https://msdn.microsoft.com/en-us/library/windows/desktop/aa380369(v=vs.85).aspx for
a description of OLE Structured Storage.
Within 3ds Max, this API is used by CustomFileStream.dlu and implemented by CustomFileStreamAPI.dll source.  The source for these are in 
maxsdk\samples\utilities\CustomFileStream.
CustomFileStream.dlu defines the CustomSceneStreamManager and CustomFileStream FPS (Function Publishing System) interfaces.
CustomFileStreamAPI.dll defines the low level file access methods that are used by CustomFileStream.dlu, and can be used by external applications 
to read, write, and modify the CustomFileStream as supported by the CustomSceneStreamManager and CustomFileStream interfaces. 
To use this dll, you would link against CustomFileStreamAPI.lib in maxsdk\libs.  There are no dependencies by CustomFileStreamAPI.dll on other 3ds 
Max dlls. To see how these low level methods should be tied together, see the CustomFileStream.dlu source.
This API will read and write CustomFileStream to 3ds Max scene files written by any version of 3ds Max. 
The CustomSceneStreamManager is required to persist the custom data streams across scene file load and save operations.
CustomFileStream as supported by CustomSceneStreamManager and CustomFileStream are located in the CustomFileStream IStorage
of the 3ds Max scene file main IStorage. Each stream consists of a header consisting of a 1 WORD version number and two 32 bit DWORD 
values (see CustomFileStreamHeader below), followed by a non-null terminated wchar string or an array of null terminated wchar strings.
The current version number is 1.
The first 32 bit DWORD is considered private (flag bit usage defined by Autodesk) and the second is considered public (flag bit usage 
defined by the creator of the stream).
Currently, bits 1 to 3 of the private DWORD are used.
Bit 1 signifies that the stream is a persistent stream. When this bit is set, the CustomSceneStreamManager will write this stream to the scene file 
when the scene file is saved. This allows an CustomFileStream to be automatically persisted across a load/save operation.
Bit 2 signifies that the stream is to be saved to the scene file when the scene file is saved, but the stream is not necessarily a persistent 
stream. This allows the user to add streams to CustomSceneStreamManager's cache and have those streams be written to the scene file. This bit is 
not persisted to the scene file, and is cleared when read from the scene file.
Bit 3 signifies that the stream is not to be added to the CustomSceneStreamManager's cache when the scene file is loaded.
If neither of the first 2 bits are set, CustomSceneStreamManager does not write this stream to the scene file when the scene file is saved.
The content of the stream is considered to be an array of strings if the last character in the content is a null character (signifying
an array of null terminated strings), otherwise the content is considered to be a single non-null terminated string,
*/

namespace MaxSDK
{
	namespace CustomFileStreamAPI
	{
		static const WORD kCustomFileStreamVersion = 1;		//!< the current stream format version number
		static const TCHAR* kCustomFileStreamStorageName = _T("CustomFileStreamDataStorage"); //!< the CustomFileStream IStorage name
		static const int kMaxStreamNameLength = 31;		//!< OLE limit of 31 chars for stream name

		//! \brief stream header consists of WORD version number, a private flag DWORD, and a public flag DWORD
		struct CustomFileStreamHeader
		{
			CustomFileStreamHeader() {}
			WORD mVersion = kCustomFileStreamVersion;	//!< stream format version number
			DWORD mPrivateFlags = 0;			//!< flag bits whose flag bit usage is defined by Autodesk
			DWORD mPublicFlags = 0;				//!< flag bits whose flag bit usage is defined by the creator or consumer of the stream 
		};

		//! \brief enum values for the private DWORD flag - bits, so powers of 2
		enum kCustomFileStreamEnum {
			kPersistentStream = 1,			//!< stream is to be persisted across scene file load/save
			kSaveNonPersistentStream = 2,	//!< stream is to be written on scene file save 
			kNoLoadOnSceneLoad = 4			//!< stream is not to be read on scene file load
		};

		enum class kDeleteStream_result {
			kOk,							//!< stream deletion successful
			kBadArgument,					//!< bad function argument
			kFileStorageOpenFailed,			//!< failure to open file storage for write
			kCustomDataStorageOpenFailed,	//!< failure to open custom data storage for write
			kStreamDoesNotExist,			//!< stream does not exist
			kDestroyElementFailed			//!< DestroyElement call on stream failed
		};

		enum class kGetLastCharacterOfContent_result {
			kOk,							//!< call successful
			kBadArgument,					//!< bad function argument
			kStreamSeekFailure,				//!< seek failure on stream
			kStreamReadFailure,				//!< read failure on stream
			kNoStreamContent				//!< no content in stream
		};

		/*! \brief Checks if a file exists and contains a storage called CustomFileStreamDataStorage.
		\param fileName The file name.
		\return true if the file exists and contains a storage called CustomFileStreamDataStorage
		*/
		DllExport bool DoesCustomFileStreamStorageExist(const TCHAR* fileName);

		/*! \brief Open a file as an OLE structured storage file with read/write access.
		\param fileName The file name.
		\param pFileIStorage [out] The IStorage associated with the file. This IStorage needs to
		be Released at the same time as the returned IStorage is released.
		\return The CustomFileStream IStorage
		*/
		DllExport IStorage* OpenStorageForWrite(const TCHAR* fileName, IStorage* & pFileIStorage);

		/*! \brief Open a file as an OLE structured storage file with read access.
		\param fileName The file name.
		\param pFileIStorage [out] The IStorage associated with the file. This IStorage needs to
		be Released at the same time as the returned IStorage is released.
		\return The CustomFileStream IStorage
		*/
		DllExport IStorage* OpenStorageForRead(const TCHAR* fileName, IStorage* & pFileIStorage);

		/*! \brief Open a stream with read/write access, creating it if not present. 
		\param pIStorage The CustomFileStream IStorage.
		\param streamName The stream name. The stream name must be less than 32 characters.
		\param privateFlags The initial private flags value for the stream, if the stream is created.
		\param publicFlags The initial public flags value for the stream, if the stream is created.
		\param version The initial version number for the stream, if the stream is created.
		\return The IStream
		\note After opening the stream, validate the stream using ValidateStream before using other 
		functions in this API.
		*/
		DllExport IStream* OpenStreamForWrite(IStorage* pIStorage, const TCHAR* streamName, DWORD privateFlags, DWORD publicFlags = 0, WORD version = kCustomFileStreamVersion);

		/*! \brief Open a stream with read access.
		\param pIStorage The CustomFileStream IStorage.
		\param streamName The stream name. The stream name must be less than 32 characters.
		\return The IStream
		\note After opening the stream, validate the stream using ValidateStream before using other
		functions in this API on the stream.
		*/
		DllExport IStream* OpenStreamForRead(IStorage* pIStorage, const TCHAR* streamName);

		/*! \brief Validates that stream was created via CustomFileStream methods, captures private and public
		flag values if wanted, leaves IStream positioned immediately past header. Verifies the stream is long enough
		to contain the stream header data, that the header data was read successfully, and the version number is
		equal to or less than the current version number. After opening a stream, this function should be used to
		validate the stream before using other functions in this API on the stream.
		\param pIStream The IStream.
		\param stream_header [out] If not null, will hold the stream header data that was read from the file.
		\return true if the stream appears to have been created via CustomFileStream methods.
		*/
		DllExport bool ValidateStream(IStream* pIStream, CustomFileStreamHeader *stream_header = nullptr);

		/*! \brief Get the byte count of the content of the stream (the data past the header)
		\param pIStream The IStream.
		\return The byte count of the content of the stream (the data past the header).
		*/
		DllExport unsigned __int64 GetStreamContentByteCount(IStream* pIStream);


		/*! \brief Sets the stream position to the beginning of the content (the data past the header)
		\param pIStream The IStream.
		\return true if successfully positioned.
		*/
		DllExport bool SeekToStartOfStreamContent(IStream* pIStream);

		/*! \brief Determine if stream content is an array of strings
		\param pIStream The IStream.
		\return true if stream content is an array of null terminated strings.
		*/
		DllExport bool IsStreamContentAnArray(IStream* pIStream);

		/*! \brief Read the stream contents (the data past the header). If the contents contain an
		array of strings, the first element of the array is returned.
		\param pIStream The IStream.
		\param content [out] The content read from the stream.
		\return true if successfully read.
		*/
		DllExport bool ReadStreamContents(IStream* pIStream, std::wstring& content);

		/*! \brief Read the stream contents (the data past the header)
		\param pIStream The IStream.
		\param content [out] The content read from the stream.
		\return true if successfully read.
		*/
		DllExport bool ReadStreamContents(IStream* pIStream, std::vector<std::wstring>& content);

		/*! \brief Write the stream contents (the data past the header), erasing any previous content
		\param pIStream The IStream.
		\param content [out] The content to write to the stream.
		\return true if successfully written.
		*/
		DllExport bool WriteStreamContents(IStream* pIStream, const std::wstring& content);

		/*! \brief Write the stream contents (the data past the header), erasing any previous content
		\param pIStream The IStream.
		\param content [out] The content to write to the stream.
		\return true if successfully written.
		*/
		DllExport bool WriteStreamContents(IStream* pIStream, const std::vector<std::wstring>& content);

		/*! \brief Read the CustomFileStream names
		\param pIStorage The CustomFileStream IStorage.
		\param streamNames [out] The stream names.
		\return true if successfully read.
		*/
		DllExport bool GetStreamNames(IStorage* pIStorage, std::vector<std::wstring>& streamNames);

		/*! \brief Delete the specified CustomFileStream stream from the specified file. Only a stream in the
		CustomFileStream IStorage with the specified name will be deleted.
		\param fileName The file name.
		\param streamName The stream name. The stream name must be less than 32 characters.
		\return The success status
		*/
		DllExport kDeleteStream_result DeleteFileStream(const TCHAR* fileName, const TCHAR* streamName);

		/*! \brief Get the last character of content in a stream.
		\param fileName The file name.
		\param streamName The stream name. The stream name must be less than 32 characters.
		\return The success status
		*/
		DllExport kGetLastCharacterOfContent_result GetLastCharacterOfContent(IStream* pIStream, wchar_t& theChar);

		/*! \brief Helper class for controlling IStorage and IStream deletion.
		A typical usage is:
		\code
		IStorage* pFileIStorage = nullptr;
		IStorage* pIStorage = OpenStorageForWrite(fileName, pFileIStorage);
		std::unique_ptr<IUnknown, IUnknownDestructorPolicy> pIStorageAutoPtr1(pFileIStorage);
		std::unique_ptr<IUnknown, IUnknownDestructorPolicy> pIStorageAutoPtr2(pIStorage);
		if (pIStorage == nullptr)
			return false;
		\endcode
		*/
		class IUnknownDestructorPolicy
		{
		public:
			void operator()(IUnknown* pIUnknown)
			{
				if (pIUnknown)
					pIUnknown->Release();
			}
		};
	}
}
