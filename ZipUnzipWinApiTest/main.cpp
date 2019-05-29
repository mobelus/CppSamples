
#include <windows.h>
#include <atlstr.h>

#include <shellapi.h>
#include <objbase.h>

//#pragma comment(lib, "SHLWAPIX.LIB")
//#pragma comment(lib, "USER32.LIB")


/* add this lines to your src
#ifndef APIZIP_H
#include "apizip.h"
#endif
*/


#include <windows.h>
#include <ole2.h>
#include <ShlDisp.h>

#include <algorithm>
#include <iterator>
#include <set>
#include <vector>

//#include "toolhlp.h"

#pragma comment(lib,"ole32")

inline
void getCurrentDirectory(::std::string &dir)
{
	char  ch = 0;
	char *buf = &ch;
	DWORD size = ::GetCurrentDirectoryA(1, buf);
	if (!size) { dir = ::std::string(); return; }
	buf = (char*)_alloca(size*sizeof(char));
	::GetCurrentDirectoryA(size, buf);
	dir = ::std::string(buf);
}

//--------------------------------------------------------------------
inline
void getCurrentDirectory(::std::wstring &dir)
{
	wchar_t  ch = 0;
	wchar_t *buf = &ch;
	DWORD size = ::GetCurrentDirectoryW(1, buf);
	if (!size) { dir = ::std::wstring(); return; }
	buf = (wchar_t*)_alloca(size*sizeof(wchar_t));
	::GetCurrentDirectoryW(size, buf);
	dir = ::std::wstring(buf);
}



struct CDoNothing
{
	CDoNothing(){}
	bool operator()(const std::string &filename) const { return true; }
};

struct CIgnoreErr
{
	CIgnoreErr(){}
	bool operator()(const std::string &filename) const { return true;  /* continue, don't stop on error  */ }
};


template <typename TProggressCallback, typename TErrorCallback >
HRESULT addFilesToZipFolder(const std::string &zipName
	, std::vector< std::string > &filesToZip
	, bool createNewArchive // erase all archive content or update existing
	, const TProggressCallback &onStartZip
	, const TErrorCallback &onZipError
	, DWORD waitTicksMax = 10000
	//, ::std::string *pErrFileName = 0// put error file name here
	)
{
	// first, create new archive if not exist, or clear previous
	{
		DWORD dwCreationDisposition = CREATE_NEW; // if file exist, don't touch it
		if (createNewArchive) dwCreationDisposition = CREATE_ALWAYS; // overwrite existing archive
		HANDLE hZipFile = CreateFile(zipName.c_str(), GENERIC_WRITE, 0, 0
			, CREATE_NEW // we do not need open existing archive
			, FILE_ATTRIBUTE_NORMAL, 0);
		if (hZipFile != INVALID_HANDLE_VALUE)
		{
			char sig[22] = { 'P', 'K', 0x05, 0x06, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			DWORD written = 0;
			WriteFile(hZipFile, &sig[0], 22, &written, 0);
			CloseHandle(hZipFile);
		}
	}

	waitTicksMax /= 100;
	if (waitTicksMax<30) waitTicksMax = 30; // 3 sec

	// here we are ready to put files to archive
	OleInitialize(NULL);

	IShellDispatch *pSD = NULL;

	HRESULT hres = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_SERVER, IID_IShellDispatch, (LPVOID *)&pSD);
	if (SUCCEEDED(hres)) // CoCreateInstance pSD
	{
		::std::string zipFullName = zipName;
		::std::string::size_type spos = zipFullName.find_first_of("\\/");
		if (spos == ::std::string::npos)
		{
			::std::string dir;
			getCurrentDirectory(dir);
			zipFullName = dir + ::std::string("\\") + zipFullName;
		}

		VARIANT vZipDir;
		VariantInit(&vZipDir);
		vZipDir.vt = VT_BSTR;
		int zipDirLen = (lstrlen(zipFullName.c_str()) + 1) * sizeof(WCHAR);
		BSTR bstrZip = SysAllocStringByteLen(NULL, zipDirLen);
		MultiByteToWideChar(CP_ACP, 0, zipFullName.c_str(), -1, bstrZip, zipDirLen);
		vZipDir.bstrVal = bstrZip;

		Folder* pZipFolder = NULL;
		hres = pSD->NameSpace(vZipDir, &pZipFolder);

		if (SUCCEEDED(hres))
		{
			VARIANT vOptions;
			VariantInit(&vOptions);
			vOptions.vt = VT_I4;
			vOptions.lVal = 4 + 16 + 1024 + 4096; //0;
			//4 Do not display a progress dialog box.  
			//8 Give the file being operated on a new name in a move, copy, or rename operation if a file with the target name already exists.  
			//16 Respond with "Yes to All" for any dialog box that is displayed.  
			//64 Preserve undo information, if possible. 
			//128 Perform the operation on files only if a wildcard file name (*.*) is specified.  
			//256 Display a progress dialog box but do not show the file names.  
			//512 Do not confirm the creation of a new directory if the operation requires one to be created.  
			//1024 Do not display a user interface if an error occurs.  
			//2048 Version 4.71. Do not copy the security attributes of the file. 
			//4096 Only operate in the local directory. Don't operate recursively into subdirectories. 
			//9182 Version 5.0. Do not copy connected files as a group. Only copy the specified files.

			::std::vector< ::std::string >::const_iterator fzIt = filesToZip.begin();
			bool fStop = false;
			for (; !fStop && fzIt != filesToZip.end(); ++fzIt)
			{
				::std::string fullName = *fzIt;
				::std::string::size_type spos = fullName.find_first_of("\\/");
				if (spos == ::std::string::npos)
				{
					::std::string dir;
					getCurrentDirectory(dir);
					fullName = dir + ::std::string("\\") + fullName;
				}

				VARIANT vFileNameToZip;
				VariantInit(&vFileNameToZip);
				vFileNameToZip.vt = VT_BSTR;
				int fnLen = (lstrlen(fullName.c_str()) + 1) * sizeof(WCHAR);
				BSTR bstrFileToZip = SysAllocStringByteLen(NULL, fnLen);
				MultiByteToWideChar(CP_ACP, 0, fullName.c_str(), -1, bstrFileToZip, fnLen);
				vFileNameToZip.bstrVal = bstrFileToZip;

				// pZipFolder->CopyHere internally creates thread and we can get it handle for wait on it,
				// so, we detect changes in current process threads

				onStartZip(*fzIt);

				std::set<DWORD> tidsBefore;
				getProcessThreads(tidsBefore);

				hres = pZipFolder->CopyHere(vFileNameToZip, vOptions);

				DWORD curLim = waitTicksMax;
				do{
					::Sleep(100);
					std::set<DWORD> tidsAfter;
					getProcessThreads(tidsAfter);

					std::set<DWORD> tidsDiff;
					setDifference(tidsBefore.begin(), tidsBefore.end(), tidsAfter.begin(), tidsAfter.end(), std::inserter(tidsDiff, tidsDiff.end()));
					if (tidsDiff.empty()) curLim = 0;
					else curLim--;
				} while (curLim);

				if (!SUCCEEDED(hres))
				{
					//if (pErrFileName) *pErrFileName = *fzIt;
					if (!onZipError(*fzIt)) fStop = true;
				}

				SysFreeString(bstrFileToZip);
				VariantClear(&vFileNameToZip);
			} // for(; !fStop && fzIt != filesToZip.end(); ++fzIt)

			pZipFolder->Release();
			VariantClear(&vOptions);
			SysFreeString(bstrZip);
			VariantClear(&vZipDir);

		} // if (SUCCEEDED(pSD->NameSpace(vZipDir,&pZipFolder)))

		pSD->Release();
	} // if (SUCCEEDED(hres)) // CoCreateInstance pSD

	OleUninitialize();
	return hres;
}

// IShellFolder::EnumObjects 
inline
HRESULT extractFilesFromZipFolder(const ::std::wstring &zipName, const ::std::wstring &outPath
, DWORD waitTicksMax = 10000
)
{
	waitTicksMax /= 100;
	if (waitTicksMax<30) waitTicksMax = 30; // 3 sec

	// here we are ready to put files to archive
	OleInitialize(NULL);

	IShellDispatch *pSD = NULL;

	HRESULT hres = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_SERVER, IID_IShellDispatch, (LPVOID *)&pSD);
	if (SUCCEEDED(hres)) // CoCreateInstance pSD
	{
		VARIANT vZipDir;
		VariantInit(&vZipDir);
		vZipDir.vt = VT_BSTR;
		int zipDirLen = (lstrlen(zipName.c_str()) + 1) * sizeof(WCHAR);
		BSTR bstrZip = SysAllocStringByteLen(NULL, zipDirLen);
		//MultiByteToWideChar(CP_ACP, 0, zipName.c_str(), -1, bstrZip, zipDirLen);
		bstrZip = (BSTR)(zipName.c_str());
		vZipDir.bstrVal = bstrZip;

		Folder* pZipFolder = NULL;
		if (SUCCEEDED(pSD->NameSpace(vZipDir, &pZipFolder)))
		{
			VARIANT vOptions;
			VariantInit(&vOptions);
			vOptions.vt = VT_I4;
			vOptions.lVal = 4 + 16 + 1024 + 4096; //0;

			pZipFolder->Release();
			VariantClear(&vOptions);
			SysFreeString(bstrZip);
			VariantClear(&vZipDir);

		} // if (SUCCEEDED(pSD->NameSpace(vZipDir,&pZipFolder)))

		pSD->Release();
	} // if (SUCCEEDED(hres)) // CoCreateInstance pSD

	OleUninitialize();
	return hres;
}



void UnZipFile(CString strSrc, CString strDest)
{	
	BSTR source = strSrc.AllocSysString();
	BSTR dest = strDest.AllocSysString();

	HRESULT hResult = S_FALSE;
	IShellDispatch *pIShellDispatch = NULL;
	Folder *pToFolder = NULL;
	VARIANT variantDir, variantFile, variantOpt;

	CoInitialize(NULL);

	hResult = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER,
		IID_IShellDispatch, (void **)&pIShellDispatch);
	if (SUCCEEDED(hResult))
	{
		VariantInit(&variantDir);
		variantDir.vt = VT_BSTR;
		variantDir.bstrVal = dest;
		hResult = pIShellDispatch->NameSpace(variantDir, &pToFolder);

		if (SUCCEEDED(hResult))
		{
			Folder *pFromFolder = NULL;
			VariantInit(&variantFile);
			variantFile.vt = VT_BSTR;
			variantFile.bstrVal = source;
			pIShellDispatch->NameSpace(variantFile, &pFromFolder);

			FolderItems *fi = NULL;
			pFromFolder->Items(&fi);

			VariantInit(&variantOpt);
			variantOpt.vt = VT_I4;
			variantOpt.lVal = FOF_NO_UI;

			VARIANT newV;
			VariantInit(&newV);
			newV.vt = VT_DISPATCH;
			newV.pdispVal = fi;
			hResult = pToFolder->CopyHere(newV, variantOpt);
			Sleep(1000);
			pFromFolder->Release();
			pToFolder->Release();
		}
		pIShellDispatch->Release();
	}

	CoUninitialize();
}


#include <Windows.h>    // Win32 SDK
#include <Shldisp.h>    // Shell COM objects
#include <atlbase.h>    // CComPtr

#include <stdio.h>      // printf for console output


//
// Helper class for exception-safe scope-based 
// CoInitialize/CoUninitialize calls.
//
class CComScopedInit
{
public:

	CComScopedInit()
	{
		HRESULT hr = ::CoInitialize(NULL);
		if (FAILED(hr))
			AtlThrow(hr);
	}

	~CComScopedInit()
	{
		::CoUninitialize();
	}


	// Ban copy
private:
	CComScopedInit(const CComScopedInit&);
	CComScopedInit& operator=(const CComScopedInit&);
};


//
// Unzip a zip file to the specified folder.
//
HRESULT UnzipToFolder(PCWSTR pszZipFile, PCWSTR pszDestFolder)
{
	CComScopedInit comInit;

	HRESULT hr;

	CComPtr<IShellDispatch> spISD;
	hr = spISD.CoCreateInstance(CLSID_Shell);
	if (FAILED(hr))
		return hr;

	CComVariant vtZipFile(pszZipFile);
	CComPtr<Folder> spZipFile;
	hr = spISD->NameSpace(vtZipFile, &spZipFile);
	if (FAILED(hr))
		return hr;

	CComVariant vtDestFolder(pszDestFolder);
	CComPtr<Folder> spDestination;
	hr = spISD->NameSpace(vtDestFolder, &spDestination);
	if (FAILED(hr))
		return hr;
	if (!spDestination)
		return E_POINTER;

	CComPtr<FolderItems> spFilesInside;
	hr = spZipFile->Items(&spFilesInside);
	if (FAILED(hr))
		return hr;

	CComPtr<IDispatch> spDispItem;
	hr = spFilesInside.QueryInterface(&spDispItem);
	if (FAILED(hr))
		return hr;

	CComVariant vtItem(spDispItem);
	CComVariant vtOptions(FOF_NO_UI);
	hr = spDestination->CopyHere(vtItem, vtOptions);
	if (FAILED(hr))
		return hr;

	return S_OK;
}


#include <stdio.h>
#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <shldisp.h>

#define FOF_NO_UI (FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR)

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")

int main(void)
{
	std::wstring strSrc(L"C:/_test/_orig.zip");
	std::wstring strDest(L"C:/_test/_orig");;

	//UnZipFile( strSrc.c_str(), strDest.c_str() );
	//HRESULT res = extractFilesFromZipFolder(strSrc, strDest);

	//Unzip2Folder(::SysAllocString(_T("c:\\filename.zip")), ::SysAllocString(_T("c:\\folder")));
	/*
	HRESULT hr = UnzipToFolder(strSrc.c_str(), strDest.c_str());

	if (FAILED(hr))
	{
	printf("Error: hr=0x%08X.\n", hr);
	}

	printf("Unzip All right.\n");
	*/

	int a = 0;

	wchar_t fin[30000];
	wchar_t odir[30000];
	HRESULT hr;
	IShellDispatch *pISD;
	Folder *pToFolder = NULL;
	VARIANT vDir, vFile, vOpt;
	//wchar_t** wargv = CommandLineToArgvW(GetCommandLineW(), &argc);

	//if (argc != 3) {
	//	printf("Usage: %S infile emptyDir/non-existentDir\n", wargv[0]);
	//	return 1;
	//}
	if (PathFileExistsW(strSrc.c_str())) {
		if (PathIsDirectoryW(strSrc.c_str()))
			return 1;
		else
		if (GetFullPathNameW(strSrc.c_str(), 30000, fin, NULL) < 4) return 1;
	}
	else return 1;
	if (PathFileExistsW(strDest.c_str())) {
		if (PathIsDirectoryW(strDest.c_str())) {
			if (PathIsDirectoryEmpty(strDest.c_str())) {
				if (GetFullPathNameW(strDest.c_str(), 30000, odir, NULL) < 4) return 1;
			}
			else
				return 1;
		}
	}
	else {
		if (GetFullPathNameW(strDest.c_str(), 30000, odir, NULL) < 4) return 1;
		if (SHCreateDirectoryExW(NULL, odir, NULL) != ERROR_SUCCESS) return 1; //deprecation or just die?
	}

	CoInitialize(NULL);

	hr = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER, IID_IShellDispatch, (void **)&pISD);

	if (SUCCEEDED(hr))
	{
		VariantInit(&vDir);
		vDir.vt = VT_BSTR;
		vDir.bstrVal = odir;
		// Destination is our zip file
		hr = pISD->NameSpace(vDir, &pToFolder);
		if (SUCCEEDED(hr))
		{
			Folder *pFromFolder = NULL;
			VariantInit(&vFile);
			vFile.vt = VT_BSTR;
			vFile.bstrVal = fin;

			pISD->NameSpace(vFile, &pFromFolder);
			FolderItems *fi = NULL;
			pFromFolder->Items(&fi);

			VariantInit(&vOpt);
			vOpt.vt = VT_I4;
			vOpt.lVal = FOF_NO_UI; // Do not display a progress dialog box

			// Creating a new Variant with pointer to FolderItems to be copied
			VARIANT newV;
			VariantInit(&newV);

			newV.vt = VT_DISPATCH;
			newV.pdispVal = fi;

			hr = pToFolder->CopyHere(newV, vOpt);
			Sleep(500);
			pFromFolder->Release();
			pToFolder->Release();
		}
		pISD->Release();
	}
	CoUninitialize();
	if (SUCCEEDED(hr))
		return 0;
	return 1;
}


//
// Test
//



/*
bool Unzip2Folder(BSTR lpZipFile, BSTR lpFolder)
{
	IShellDispatch *pISD;

	Folder  *pZippedFile = 0L;
	Folder  *pDestination = 0L;

	long FilesCount = 0;
	IDispatch* pItem = 0L;
	FolderItems *pFilesInside = 0L;

	VARIANT Options, OutFolder, InZipFile, Item;
	CoInitialize(NULL);
	__try{
		if (CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER, IID_IShellDispatch, (void **)&pISD) != S_OK)
			return 1;

		InZipFile.vt = VT_BSTR;
		InZipFile.bstrVal = lpZipFile;
		pISD->NameSpace(InZipFile, &pZippedFile);
		if (!pZippedFile)
		{
			pISD->Release();
			return 1;
		}

		OutFolder.vt = VT_BSTR;
		OutFolder.bstrVal = lpFolder;
		pISD->NameSpace(OutFolder, &pDestination);
		if (!pDestination)
		{
			pZippedFile->Release();
			pISD->Release();
			return 1;
		}

		pZippedFile->Items(&pFilesInside);
		if (!pFilesInside)
		{
			pDestination->Release();
			pZippedFile->Release();
			pISD->Release();
			return 1;
		}

		pFilesInside->get_Count(&FilesCount);
		if (FilesCount < 1)
		{
			pFilesInside->Release();
			pDestination->Release();
			pZippedFile->Release();
			pISD->Release();
			return 0;
		}

		pFilesInside->QueryInterface(IID_IDispatch, (void**)&pItem);

		Item.vt = VT_DISPATCH;
		Item.pdispVal = pItem;

		Options.vt = VT_I4;
		Options.lVal = 1024 | 512 | 16 | 4;//http://msdn.microsoft.com/en-us/library/bb787866(VS.85).aspx

		bool retval = pDestination->CopyHere(Item, Options) == S_OK;

		pItem->Release(); pItem = 0L;
		pFilesInside->Release(); pFilesInside = 0L;
		pDestination->Release(); pDestination = 0L;
		pZippedFile->Release(); pZippedFile = 0L;
		pISD->Release(); pISD = 0L;

		return retval;

	}
	__finally
	{
		CoUninitialize();
	}
}


bool DecompressZIP(_In_ const wpath& pathFile, _In_ const wpath& pathDstDir)
{
	BSTR source = _bstr_t(pathFile.string().c_str());
	BSTR dest = _bstr_t(pathDstDir.string().c_str());

	HRESULT hResult = S_FALSE;
	IShellDispatch *pIShellDispatch = NULL;
	Folder *pToFolder = NULL;
	VARIANT variantDir, variantFile, variantOpt;

	CoInitialize(NULL);

	hResult = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER,
		IID_IShellDispatch, (void **)&pIShellDispatch);

	if (SUCCEEDED(hResult) && NULL != pIShellDispatch)
	{
		VariantInit(&variantDir);
		variantDir.vt = VT_BSTR;
		variantDir.bstrVal = dest;
		hResult = pIShellDispatch->NameSpace(variantDir, &pToFolder);

		if (SUCCEEDED(hResult) && NULL != pToFolder)
		{
			Folder *pFromFolder = NULL;
			VariantInit(&variantFile);
			variantFile.vt = VT_BSTR;
			variantFile.bstrVal = source;
			hResult = pIShellDispatch->NameSpace(variantFile, &pFromFolder);

			if (SUCCEEDED(hResult) && NULL != pFromFolder)
			{
				FolderItems *fi = NULL;
				pFromFolder->Items(&fi);

				VariantInit(&variantOpt);
				variantOpt.vt = VT_I4;
				variantOpt.lVal = FOF_NO_UI;

				VARIANT newV;
				VariantInit(&newV);
				newV.vt = VT_DISPATCH;
				newV.pdispVal = fi;
				hResult = pToFolder->CopyHere(newV, variantOpt);
				Sleep(1000);
				pFromFolder->Release();
				pToFolder->Release();
			}
		}
		pIShellDispatch->Release();
	}

	CoUninitialize();

	return true;
}
*/



