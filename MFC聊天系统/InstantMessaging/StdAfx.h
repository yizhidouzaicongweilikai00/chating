// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__46423D1C_16F1_4114_BB64_E10342DC39F5__INCLUDED_)
#define AFX_STDAFX_H__46423D1C_16F1_4114_BB64_E10342DC39F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions
#include "Global.h"			/// ȫ�ֺ궨��

#include <afxtempl.h>		/// ʹ��CArray

/// ʹ��TransparentBlt͸����ͼ
#pragma comment( lib, "C:\\Program Files\\Microsoft Visual Studio\\VC98\\Lib\\MSIMG32.LIB" )

/// ʹ��PlaySound����
#include <mmsystem.h>
#pragma comment( lib, "WINMM.LIB" )

/// ʹ��zlib�����ݵ�ѹ�����ѹ
#include "zconf.h"
#include "zlib.h"
#pragma comment( lib, "zdll.lib" )

/// ʹ��pow()����
#include <math.h>

/// ����ͷ
#include <vfw.h>			
#pragma comment( lib, "vfw32.lib" )

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__46423D1C_16F1_4114_BB64_E10342DC39F5__INCLUDED_)
