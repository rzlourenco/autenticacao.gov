/* ****************************************************************************

 * eID Middleware Project.
 * Copyright (C) 2008-2009 FedICT.
 * Copyright (C) 2019 Caixa Magica Software.
 * Copyright (C) 2011 Vasco Silva - <vasco.silva@caixamagica.pt>
 * Copyright (C) 2012, 2016-2018 André Guerreiro - <aguerreiro1985@gmail.com>
 * Copyright (C) 2012 lmcm - <lmcm@caixamagica.pt>
 * Copyright (C) 2017 Luiz Lemos - <luiz.lemos@caixamagica.pt>
 * Copyright (C) 2019 Miguel Figueira - <miguelblcfigueira@gmail.com>
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version
 * 3.0 as published by the Free Software Foundation.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, see
 * http://www.gnu.org/licenses/.

**************************************************************************** */

#include "stdafx.h"
#include "../dialogs.h"
#include "dlgWndAskPIN.h"
#include "dlgWndAskPINs.h"
#include "dlgWndBadPIN.h"
#include "dlgWndPinpadInfo.h"
#include "dlgWndAskCmd.h"
#include "dlgWndAskCmdOtp.h"
#include "resource.h"
#include "../langUtil.h"
#include "Config.h"
#include "Log.h"

using namespace eIDMW;

HMODULE g_hDLLInstance = (HMODULE)NULL;

typedef std::map< unsigned long, dlgWndPinpadInfo* > TD_WNDPINPAD_MAP;
typedef std::pair< unsigned long, dlgWndPinpadInfo* > TD_WNDPINPAD_PAIR;

dlgWndPinpadInfo *dlgModal = NULL;
TD_WNDPINPAD_MAP dlgPinPadInfoCollector;
unsigned long dlgPinPadInfoCollectorIndex = 0;

std::wstring lang1 = CConfig::GetString(CConfig::EIDMW_CONFIG_PARAM_GENERAL_LANGUAGE);

HWND appWindow;

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hDLLInstance = hModule;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

std::wstring getPinName( DlgPinUsage usage, const wchar_t *inPinName ){
    std::wstring PinName;

    switch( usage ) {
        case DLG_PIN_AUTH:
            PinName = GETSTRING_DLG(AuthenticationPin);
            break;

        case DLG_PIN_SIGN:
            PinName = GETSTRING_DLG(SignaturePin);
            break;

        case DLG_PIN_ADDRESS:
            PinName = GETSTRING_DLG(AddressPin);
            break;

        default:
            if ( inPinName == NULL ) {
                PinName = GETSTRING_DLG(UnknownPin);
            }
			else {
                 if( wcslen( inPinName ) == 0 ){
                    PinName = GETSTRING_DLG(Pin);
                } else {
                    PinName = inPinName;
                }
            }
            break;
    }

    return PinName;
}

	/************************
	*       DIALOGS
	************************/
#ifdef WIN32
DLGS_EXPORT void eIDMW::SetApplicationWindow(HWND app) {
	appWindow = app;
}
#endif
DLGS_EXPORT DlgRet eIDMW::DlgAskPin(DlgPinOperation operation,
			DlgPinUsage usage, const wchar_t *csPinName,
			DlgPinInfo pinInfo, wchar_t *csPin, unsigned long ulPinBufferLen, void *wndGeometry)
{
	MWLOG(LEV_DEBUG, MOD_DLG, L"DlgAskPin() called with arguments usage: %d, PinName: %s, Operation:%d",
		usage, csPinName,operation);

	CLang::ResetInit();				// Reset language to take into account last change

	dlgWndAskPIN *dlg = NULL;
	try
	{
		std::wstring PINName;
        PINName = getPinName( usage, csPinName );

		std::wstring sMessage;
		switch( operation )
		{
		case DLG_PIN_OP_VERIFY:
			switch( usage )
			{
			case DLG_PIN_AUTH:

				sMessage += GETSTRING_DLG(PleaseEnterYourPin);
				sMessage += L", ";
				sMessage += GETSTRING_DLG(InOrderToAuthenticateYourself);
				break;
			case DLG_PIN_SIGN:
				sMessage += GETSTRING_DLG(Caution);
				sMessage += L" ";
				sMessage += GETSTRING_DLG(YouAreAboutToMakeALegallyBindingElectronic);

				break;
			case DLG_PIN_ADDRESS:
				sMessage += GETSTRING_DLG(PleaseEnterYourPin);
				break;

			default:
				sMessage += L"----Default PIN Type????----";
				sMessage += L"\n";
				break;
			}
			break;
		case DLG_PIN_OP_UNBLOCK_NO_CHANGE:
			sMessage = GETSTRING_DLG(PleaseEnterYourPuk);
			sMessage += L", ";
			sMessage = GETSTRING_DLG(ToUnblock);
			sMessage += L" ";
			sMessage = GETSTRING_DLG(Your);
			sMessage += L" \"";
			if( wcslen(csPinName)==0 )
				sMessage += csPinName;
			else
				sMessage += GETSTRING_DLG(Pin);
			sMessage += L"\"\n";

			break;
		default:
			MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgAskPin() returns DLG_BAD_PARAM");
			return DLG_BAD_PARAM;
			break;
		}

		dlg = new dlgWndAskPIN(pinInfo, usage, sMessage, PINName, appWindow);
		if( dlg->exec() )
		{
			eIDMW::DlgRet dlgResult = dlg->dlgResult;
			wcscpy_s(csPin,ulPinBufferLen,dlg->PinResult);

			delete dlg;
			MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgAskPin() returns DLG_OK");
			return DLG_OK;
		}
		delete dlg;
	}
	catch( ... )
	{
		if( dlg )
			delete dlg;
		MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgAskPin() returns DLG_ERR");
		return DLG_ERR;
	}
	MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgAskPin() returns DLG_CANCEL");
	return DLG_CANCEL;
}

DLGS_EXPORT DlgRet eIDMW::DlgAskPins(DlgPinOperation operation,
			DlgPinUsage usage, const wchar_t *csPinName,
			DlgPinInfo pin1Info, wchar_t *csPin1, unsigned long ulPin1BufferLen,
			DlgPinInfo pin2Info, wchar_t *csPin2, unsigned long ulPin2BufferLen, void *wndGeometry )
{
	MWLOG(LEV_DEBUG, MOD_DLG, L"DlgAskPins() called");

	CLang::ResetInit();				// Reset language to take into account last change

	dlgWndAskPINs *dlg = NULL;
	try
	{
		std::wstring PINName;
		std::wstring Header;
		bool isUnlock = true;

		switch( operation )
		{
		case DLG_PIN_OP_CHANGE:
            PINName = getPinName( usage, csPinName );

			Header = GETSTRING_DLG(EnterYour);
			Header += L" ";
			Header += PINName;
			Header += L" ";
			isUnlock = false;
			break;
		case DLG_PIN_OP_UNBLOCK_CHANGE:
			Header = GETSTRING_DLG(UnlockDialogHeader);
			PINName = getPinName(usage, csPinName);
			break;
		case DLG_PIN_OP_UNBLOCK_CHANGE_NO_PUK:
			//This message doesn't mention introducing any PUK
			Header = GETSTRING_DLG(UnblockPinHeader);
			PINName = getPinName(usage, csPinName);
			MWLOG(LEV_DEBUG, MOD_DLG, L"dlgsWin32: Performing operation UNBLOCK_CHANGE_NO_PUK");
			break;
		default:
			MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgAskPins() returns DLG_BAD_PARAM");
			return DLG_BAD_PARAM;
			break;
		}

		dlg = new dlgWndAskPINs(pin1Info, pin2Info, Header, PINName, isUnlock, operation == DLG_PIN_OP_UNBLOCK_CHANGE_NO_PUK, appWindow);
		if( dlg->exec() )
		{
			eIDMW::DlgRet dlgResult = dlg->dlgResult;
			if (operation != DLG_PIN_OP_UNBLOCK_CHANGE_NO_PUK)
				wcscpy_s(csPin1, ulPin1BufferLen, dlg->Pin1Result);
			wcscpy_s(csPin2,ulPin2BufferLen,dlg->Pin2Result);

			delete dlg;
			MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgAskPins() returns DLG_OK");
			return DLG_OK;
		}
		delete dlg;
	}
	catch( ... )
	{
		if( dlg )
			delete dlg;
		MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgAskPins() returns DLG_ERR");
		return DLG_ERR;
	}
	MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgAskPins() returns DLG_CANCEL");
	return DLG_CANCEL;
}

DLGS_EXPORT DlgRet eIDMW::DlgBadPin(
			DlgPinUsage usage, const wchar_t *csPinName,
			unsigned long ulRemainingTries, void *wndGeometry)
{
	MWLOG(LEV_DEBUG, MOD_DLG, L"DlgBadPin() called");

	CLang::ResetInit();				// Reset language to take into account last change

	dlgWndBadPIN *dlg = NULL;
	try
	{
		std::wstring PINName;
		if ( ( usage == DLG_PIN_UNKNOWN ) && ( wcslen(csPinName)==0 ) ){
            MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgBadPin() returns DLG_BAD_PARAM");
            return DLG_BAD_PARAM;
        }
        PINName = getPinName( usage, csPinName );

		dlg = new dlgWndBadPIN(PINName, ulRemainingTries, appWindow);
		if( dlg->exec() )
		{
			eIDMW::DlgRet dlgResult = dlg->dlgResult;
			//csPin = dlg->PinResult;

			delete dlg;
			MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgBadPin() returns %ld",dlgResult);
			return dlgResult;
		}
		delete dlg;
	}
	catch( ... )
	{
		if( dlg )
			delete dlg;
		MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgBadPin() returns DLG_ERR");
		return DLG_ERR;
	}
	MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgBadPin() returns DLG_CANCEL");
	return DLG_CANCEL;
}

DLGS_EXPORT DlgRet eIDMW::DlgDisplayPinpadInfo(DlgPinOperation operation,
			const wchar_t *csReader, DlgPinUsage usage, const wchar_t *csPinName,
			const wchar_t *csMessage,
			unsigned long *pulHandle, void *wndGeometry)
{
	MWLOG(LEV_DEBUG, MOD_DLG, L"DlgDisplayPinpadInfo() called");

	CLang::ResetInit();				// Reset language to take into account last change

	try
	{
		std::wstring PINName;
		switch( usage )
		{
		case DLG_PIN_UNKNOWN:
			if( wcslen(csPinName)==0 )
			{
				MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgDisplayPinpadInfo() returns DLG_BAD_PARAM");
				return DLG_BAD_PARAM;
			}
			PINName = csPinName;
			break;
		default:
			if( wcslen(csPinName)==0 )
				PINName = GETSTRING_DLG(Pin);
			else
				PINName = csPinName;
			break;
		}

		std::wstring sMessage;
		if(wcslen(csMessage)!=0)
		{
			sMessage=csMessage;
		}
		else
		{
			switch( operation )
			{
			case DLG_PIN_OP_VERIFY:
					sMessage = GETSTRING_DLG(PleaseEnterYourPinOnThePinpadReader);
					break;
			case DLG_PIN_OP_UNBLOCK_NO_CHANGE:
				sMessage = GETSTRING_DLG(PleaseEnterYourPukOnThePinpadReader);
	
				sMessage += L", ";
				sMessage = GETSTRING_DLG(ToUnblock);
				sMessage += L" ";
				sMessage += GETSTRING_DLG(Your);
				sMessage += L" \"";
				if( wcslen(csPinName)!=0 )
					sMessage += csPinName;
				else
					sMessage += GETSTRING_DLG(Pin);
				sMessage += L"\"\n";

				break;
			case DLG_PIN_OP_CHANGE:
				sMessage += GETSTRING_DLG(EnterOldNewVerify);
				sMessage += L"\n";
				break;
			case DLG_PIN_OP_UNBLOCK_CHANGE:
				sMessage = L"\n";
				sMessage += GETSTRING_DLG(UnlockDialogInstructions);
				break;
			case DLG_PIN_OP_UNBLOCK_CHANGE_NO_PUK:
				sMessage = L"\n";
				sMessage += GETSTRING_DLG(UnlockWithoutPUKInstructions);
				break;
			default:
				MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgDisplayPinpadInfo() returns DLG_BAD_PARAM");
				return DLG_BAD_PARAM;
				break;
			}
		}
		//Small hack for the PIN unlock dialog :)
		std::wstring pin_name_label;
		if (operation == DLG_PIN_OP_UNBLOCK_CHANGE || operation == DLG_PIN_OP_UNBLOCK_CHANGE_NO_PUK)
		{
		 pin_name_label +=  GETSTRING_DLG(UnblockPinHeader);
		 pin_name_label += L": ";
		 pin_name_label += csPinName;
		}
		else {
			pin_name_label = PINName;
		}

		//QString buf = "dlg num: " + QString().setNum( dlgPinPadInfoCollectorIndex );
		dlgPinPadInfoCollectorIndex++;
		dlgModal = new dlgWndPinpadInfo(dlgPinPadInfoCollectorIndex, usage,
			operation, csReader, pin_name_label, sMessage, appWindow);

		dlgPinPadInfoCollector.insert(TD_WNDPINPAD_PAIR(dlgPinPadInfoCollectorIndex, dlgModal));
		if (pulHandle)
			*pulHandle = dlgPinPadInfoCollectorIndex;

		// Loop
		dlgModal->exec();

		delete dlgModal;
		dlgModal = NULL;

		MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgDisplayPinpadInfo() returns DLG_OK");
		return DLG_OK;
	}
	catch(...)
	{
		if( dlgModal )
			delete dlgModal;
		MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgDisplayPinpadInfo() returns DLG_ERR");
		return DLG_ERR;
	}
	MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgDisplayPinpadInfo() returns DLG_CANCEL");
	return DLG_CANCEL;
}

DLGS_EXPORT void eIDMW::DlgClosePinpadInfo( unsigned long ulHandle )
{
	MWLOG(LEV_DEBUG, MOD_DLG, L"DlgClosePinpadInfo() called");
	TD_WNDPINPAD_MAP::iterator it_WndPinpad_Map = dlgPinPadInfoCollector.find(ulHandle);
	if (it_WndPinpad_Map != dlgPinPadInfoCollector.end())
		dlgModal = (*it_WndPinpad_Map).second;
	else
		dlgModal = NULL;

	if (dlgModal)
	{
		dlgModal->stopExec();
	}
	dlgPinPadInfoCollector.erase(ulHandle);
	MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgClosePinpadInfo() returns");
}

DLGS_EXPORT DlgRet eIDMW::DlgAskCMD(
			DlgUserIdType userIdUsage, DlgPinUsage pinUsage,
			wchar_t *csUserId, unsigned long ulUserIdBufferLen,
			wchar_t *csPin, unsigned long ulPinBufferLen,
			const wchar_t *csUserName, unsigned long ulUserNameBufferLen, void * wndGeometry)
{
	MWLOG(LEV_DEBUG, MOD_DLG, L"DlgAskCMD() called with arguments DlgUserIdUsage: %d, DlgPinUsage: %d",
		userIdUsage, pinUsage);

	CLang::ResetInit();				// Reset language to take into account last change

	dlgWndAskCmd *dlg = NULL;

	try
	{
		// The only user identification supported is the mobile number
		if (userIdUsage != DLG_USERID_MOBILE && userIdUsage != DLG_USERID_MOBILE_FIXED)
		{
			MWLOG(LEV_ERROR, MOD_DLG, L"  --> DlgAskCMD() returns DLG_BAD_PARAM (only userId supported is mobile number)");
			return DLG_BAD_PARAM;
		}
		if ((userIdUsage != DLG_USERID_MOBILE_FIXED && ulUserIdBufferLen < 16) || ulPinBufferLen < 9)
		{
			MWLOG(LEV_ERROR, MOD_DLG, L"  --> DlgAskCMD() returns DLG_BAD_PARAM: buffer does not have enough size");
			return DLG_BAD_PARAM;
		}

		std::wstring PINName;

		std::wstring sMessage;

		switch (pinUsage)
		{
		case DLG_PIN_SIGN:
			sMessage += GETSTRING_DLG(Caution);
			sMessage += L" ";
			sMessage += GETSTRING_DLG(YouAreAboutToMakeALegallyBindingElectronicWithCmd);
			break;
		default:
			MWLOG(LEV_ERROR, MOD_DLG, L"  --> DlgAskCMD() returns DLG_BAD_PARAM");
			return DLG_BAD_PARAM;
		}

		std::wstring userName;
		std::wstring userId;
		if (userIdUsage == DLG_USERID_MOBILE_FIXED)
		{
			userName.append(csUserName, ulUserNameBufferLen);
			userId.append(csUserId, ulUserIdBufferLen);
		}
		dlg = new dlgWndAskCmd(userIdUsage, pinUsage, sMessage, PINName, 
			&userId, &userName, appWindow);
		if (dlg->exec())
		{
			eIDMW::DlgRet dlgResult = dlg->dlgResult;
			if (userIdUsage != DLG_USERID_MOBILE_FIXED)
			{
				wcscpy_s(csUserId, ulUserIdBufferLen, dlg->UserIdResult);
			}
			wcscpy_s(csPin, ulPinBufferLen, dlg->PinResult);

			delete dlg;
			MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgAskCMD() returns DLG_OK");
			return DLG_OK;
		}
		delete dlg;
	}
	catch (...)
	{
		if (dlg)
			delete dlg;
		MWLOG(LEV_ERROR, MOD_DLG, L"  --> DlgAskCMD() returns DLG_ERR");
		return DLG_ERR;
	}
	MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgAskCMD() returns DLG_CANCEL");
	return DLG_CANCEL;
}

DLGS_EXPORT DlgRet eIDMW::DlgAskCMDOtp(wchar_t *csOtp, unsigned long ulOtpBufferLen,
	wchar_t *csDocname, void *wndGeometry)
{
	MWLOG(LEV_DEBUG, MOD_DLG, L"DlgAskCMDOtp() called");

	CLang::ResetInit();				// Reset language to take into account last change

	dlgWndAskCmdOtp *dlg = NULL;

	if (ulOtpBufferLen < 7)
	{
		MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgAskCMDOtp() returns DLG_BAD_PARAM: otp buffer must have len of 6 chars");
        return DLG_BAD_PARAM;
	}
	try
	{
		std::wstring sMessage;
		sMessage += GETSTRING_DLG(InsertOtp);

		dlg = new dlgWndAskCmdOtp(sMessage, csDocname, appWindow);
		if (dlg->exec())
		{
			eIDMW::DlgRet dlgResult = dlg->dlgResult;
			wcscpy_s(csOtp, ulOtpBufferLen, dlg->OtpResult);

			delete dlg;
			MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgAskCMDOtp() returns DLG_OK");
			return DLG_OK;
		}
		delete dlg;
	}
	catch (...)
	{
		if (dlg)
			delete dlg;
		MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgAskCMDOtp() returns DLG_ERR");
		return DLG_ERR;
	}
	MWLOG(LEV_DEBUG, MOD_DLG, L"  --> DlgAskCMDOtp() returns DLG_CANCEL");
	return DLG_CANCEL;
}
