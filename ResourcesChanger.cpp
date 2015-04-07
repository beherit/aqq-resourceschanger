//---------------------------------------------------------------------------
// Copyright (C) 2012-2015 Krzysztof Grochocki
//
// This file is part of ResourcesChanger
//
// ResourcesChanger is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// ResourcesChanger is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Radio. If not, see <http://www.gnu.org/licenses/>.
//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#pragma argsused
#include <Wlanapi.h>
#pragma comment(lib, "Wlanapi")
#include <IdHashMessageDigest.hpp>
#include <PluginAPI.h>
#include "FirstRunFrm.h"
#include "NewComputerFrm.h"
#include "SettingsFrm.h"
#define RESOURCESCHANGER_SYSTEM_RESOURCECHANGED L"ResourcesChanger/System/ResourceChanged"

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  return 1;
}
//---------------------------------------------------------------------------

//Uchwyt-do-form-------------------------------------------------------------
TSettingsForm* hSettingsForm;
TFirstRunForm* hFirstRunForm;
TNewComputerForm* hNewComputerForm;
//Struktury-glowne-----------------------------------------------------------
TPluginLink PluginLink;
TPluginInfo PluginInfo;
//---------------------------------------------------------------------------
//Sciezka do pliku ustawien
UnicodeString SettingsFileDir;
//Tryb dzialania wtyczki
int PluginModeChk;
//Zdefiniowany przez usera zasob dla danego komputera
UnicodeString ComputerResourceName;
//Aktulna nazwa zasobu
UnicodeString ActiveResourceName;
//Nazwa komputera
UnicodeString ComputerName;
//Aktuana nazwa SSID polaczenia Wi-Fi
UnicodeString ActiveSSID;
//Uchwyt do okna timera
HWND hTimerFrm;
//TIMERY---------------------------------------------------------------------
#define TIMER_CHKSSID 10
//FORWARD-AQQ-HOOKS----------------------------------------------------------
INT_PTR __stdcall OnModulesLoaded(WPARAM wParam, LPARAM lParam);
INT_PTR __stdcall OnSetLastState(WPARAM wParam, LPARAM lParam);
INT_PTR __stdcall OnThemeChanged(WPARAM wParam, LPARAM lParam);
INT_PTR __stdcall ServiceNewComputer(WPARAM, LPARAM);
//FORWARD-TIMER--------------------------------------------------------------
LRESULT CALLBACK TimerFrmProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//FORWARD-OTHER-FUNCTION-----------------------------------------------------
void LoadSettings();
void ChangeResources();
//---------------------------------------------------------------------------

//Pobieranie sciezki katalogu prywatnego wtyczek
UnicodeString GetPluginUserDir()
{
  return StringReplace((wchar_t*)PluginLink.CallService(AQQ_FUNCTION_GETPLUGINUSERDIR,0,0), "\\", "\\\\", TReplaceFlags() << rfReplaceAll);
}
//---------------------------------------------------------------------------

//Pobieranie sciezki do skorki kompozycji
UnicodeString GetThemeSkinDir()
{
  return StringReplace((wchar_t*)PluginLink.CallService(AQQ_FUNCTION_GETTHEMEDIR,0,0), "\\", "\\\\", TReplaceFlags() << rfReplaceAll) + "\\\\Skin";
}
//---------------------------------------------------------------------------

//Sprawdzanie czy  wlaczona jest zaawansowana stylizacja okien
bool ChkSkinEnabled()
{
  TStrings* IniList = new TStringList();
  IniList->SetText((wchar_t*)PluginLink.CallService(AQQ_FUNCTION_FETCHSETUP,0,0));
  TMemIniFile *Settings = new TMemIniFile(ChangeFileExt(Application->ExeName, ".INI"));
  Settings->SetStrings(IniList);
  delete IniList;
  UnicodeString SkinsEnabled = Settings->ReadString("Settings","UseSkin","1");
  delete Settings;
  return StrToBool(SkinsEnabled);
}
//---------------------------------------------------------------------------

//Sprawdzanie ustawien animacji AlphaControls
bool ChkThemeAnimateWindows()
{
  TStrings* IniList = new TStringList();
  IniList->SetText((wchar_t*)PluginLink.CallService(AQQ_FUNCTION_FETCHSETUP,0,0));
  TMemIniFile *Settings = new TMemIniFile(ChangeFileExt(Application->ExeName, ".INI"));
  Settings->SetStrings(IniList);
  delete IniList;
  UnicodeString AnimateWindowsEnabled = Settings->ReadString("Theme","ThemeAnimateWindows","1");
  delete Settings;
  return StrToBool(AnimateWindowsEnabled);
}
//---------------------------------------------------------------------------
bool ChkThemeGlowing()
{
  TStrings* IniList = new TStringList();
  IniList->SetText((wchar_t*)PluginLink.CallService(AQQ_FUNCTION_FETCHSETUP,0,0));
  TMemIniFile *Settings = new TMemIniFile(ChangeFileExt(Application->ExeName, ".INI"));
  Settings->SetStrings(IniList);
  delete IniList;
  UnicodeString GlowingEnabled = Settings->ReadString("Theme","ThemeGlowing","1");
  delete Settings;
  return StrToBool(GlowingEnabled);
}
//---------------------------------------------------------------------------

//Pobieranie ustawien koloru AlphaControls
int GetHUE()
{
  return (int)PluginLink.CallService(AQQ_SYSTEM_COLORGETHUE,0,0);
}
//---------------------------------------------------------------------------
int GetSaturation()
{
  return (int)PluginLink.CallService(AQQ_SYSTEM_COLORGETSATURATION,0,0);
}
//---------------------------------------------------------------------------

//Pobieranie aktualnego stanu wskazanego konta
int GetState()
{
  //Pobranie aktualnego stanu konta
  TPluginStateChange PluginStateChange;
  PluginLink.CallService(AQQ_FUNCTION_GETNETWORKSTATE,(WPARAM)(&PluginStateChange),0);
  int State = PluginStateChange.NewState;
  //Zwrocenie odpowiedniego indeksu
  return State;
}
//---------------------------------------------------------------------------

//Pobieranie sciezki ikony z interfejsu AQQ
UnicodeString GetIconPath(int Icon)
{
  return StringReplace((wchar_t*)PluginLink.CallService(AQQ_FUNCTION_GETPNG_FILEPATH,Icon,0), "\\", "\\\\", TReplaceFlags() << rfReplaceAll);
}
//--------------------------------------------------------------------------

//Ustawianie nowego stanu kont
void SetState(int NewState)
{
  if(NewState==0) PluginLink.CallService(AQQ_SYSTEM_RUNACTION,0,(LPARAM)L"aMacroOffline");
  else if(NewState==1) PluginLink.CallService(AQQ_SYSTEM_RUNACTION,0,(LPARAM)L"aMacroOnline");
  else if(NewState==2) PluginLink.CallService(AQQ_SYSTEM_RUNACTION,0,(LPARAM)L"aMacroChat");
  else if(NewState==3) PluginLink.CallService(AQQ_SYSTEM_RUNACTION,0,(LPARAM)L"aMacroAway");
  else if(NewState==4) PluginLink.CallService(AQQ_SYSTEM_RUNACTION,0,(LPARAM)L"aMacroXA");
  else if(NewState==5) PluginLink.CallService(AQQ_SYSTEM_RUNACTION,0,(LPARAM)L"aMacroDND");
  else if(NewState==6) PluginLink.CallService(AQQ_SYSTEM_RUNACTION,0,(LPARAM)L"aMacroInvisible");
}
//---------------------------------------------------------------------------

//Pobieranie nazwy SSID aktualnego polaczenia Wi-Fi
UnicodeString GetNetworkSSID()
{
  //Zmienne polaczenia WLAN
  HANDLE ClientHandle = NULL;
  DWORD ClientVersion = 2;
  DWORD NegotiatedVersion = 0;
  //Otwarcie polaczenia WLAN
  DWORD ReturnCode = WlanOpenHandle(ClientVersion, NULL, &NegotiatedVersion, &ClientHandle);
  //Polaczenie WLAN otwarte prawidlowo
  if(ReturnCode==ERROR_SUCCESS)
  {
	//Enumeracja wszystkich adapterow Wi-Fi
	PWLAN_INTERFACE_INFO_LIST InterfaceList = NULL;
	ReturnCode = WlanEnumInterfaces(ClientHandle, NULL, &InterfaceList);
	//Pobrano liste adapterow
	if(ReturnCode==ERROR_SUCCESS)
	{
	  //Przegladanie listy adaperow Wi-F
	  for(int InterfaceIdx=0; InterfaceIdx<InterfaceList->dwNumberOfItems; InterfaceIdx++)
	  {
		//Pobranie informacji o adapeterze Wi-Fi
		PWLAN_INTERFACE_INFO InterfaceInfo = (WLAN_INTERFACE_INFO*)&InterfaceList->InterfaceInfo[InterfaceIdx];
		//Status adaptera Wi-Fi prawidlowy
		if(InterfaceInfo->isState==wlan_interface_state_connected)
		{
		  //Pobranie wszystkich dostepnych polaczen Wi-Fi
          PWLAN_AVAILABLE_NETWORK_LIST NetworkList = NULL;
		  ReturnCode = WlanGetAvailableNetworkList(ClientHandle, &InterfaceInfo->InterfaceGuid, 0, NULL, &NetworkList);
		  //Pobrano liste dostepnych polaczen Wi-Fi
		  if(ReturnCode==ERROR_SUCCESS)
		  {
			//Enumeracja wszystkich dostepnych polaczen Wi-Fi
			for(int NetworkIdx=0; NetworkIdx<NetworkList->dwNumberOfItems; NetworkIdx++)
			{
			  //Pobranie informacji o polaczeniu Wi-Fi
			  PWLAN_AVAILABLE_NETWORK NetworkInfo = (WLAN_AVAILABLE_NETWORK *) &NetworkList->Network[NetworkIdx];
			  //Adapter Wi-Fi jest polaczony z dana siecia Wi-Fi
			  if(NetworkInfo->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED)
			  {
                //Pobranie SSID sieci
				UnicodeString SSID = (UnicodeString)NetworkInfo->strProfileName;
				//Zamkniecie polaczenia
				WlanCloseHandle(ClientHandle,NULL);
				//Zwrot prawidlowej nazwy SSID
				return SSID;
			  }
			}
		  }
		}
	  }
	}
  }
  //Zamkniecie polaczenia
  WlanCloseHandle(ClientHandle,NULL);
  //Zwrot pustej nazwy SSID
  return "";
}
//---------------------------------------------------------------------------

//Procka okna timera
LRESULT CALLBACK TimerFrmProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if(uMsg==WM_TIMER)
  {
	//Sprawdzanie aktualnego SSID polaczenia Wi-Fi
	if(wParam==TIMER_CHKSSID)
	{
	  //Pobieranie aktualnego SSID polaczenia Wi-Fi
	  UnicodeString SSID = GetNetworkSSID();
	  //SSID polaczenia Wi-Fi sie zmienilo
	  if(SSID!=ActiveSSID)
	   ChangeResources();
	}

	return 0;
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//---------------------------------------------------------------------------

//Hook na zaladowanie wszystkich modulow w AQQ
INT_PTR __stdcall OnModulesLoaded(WPARAM wParam, LPARAM lParam)
{
  //Odczyt ustawien w rdzeniu wtyczki
  LoadSettings();
  //Zmiana zasobow w kontach Jabber
  ChangeResources();

  return 0;
}
//---------------------------------------------------------------------------

//Hook na polaczenie sie z siecia przy wlaczeniu AQQ
INT_PTR __stdcall OnSetLastState(WPARAM wParam, LPARAM lParam)
{
  //Odczyt pliku ustawien
  TIniFile *Ini = new TIniFile(SettingsFileDir);
  //Zmiana stanu kont
  if(Ini->ReadInteger("General","Mode",1)==2)
  {
	if(Ini->SectionExists("State:"+ComputerName+":"+GetNetworkSSID()))
	 SetState(Ini->ReadInteger("State:"+ComputerName+":"+GetNetworkSSID(),"State",0));
	else if(Ini->SectionExists("State:"+ComputerName))
	 SetState(Ini->ReadInteger("State:"+ComputerName,"State",0));
  }
  //Zamkniecie pliku ustawien
  delete Ini;

  return 0;
}
//---------------------------------------------------------------------------

//Hook na zmianê kompozycji
INT_PTR __stdcall OnThemeChanged(WPARAM wParam, LPARAM lParam)
{
  //Okno ustawien zostalo juz stworzone
  if(hSettingsForm)
  {
	//Wlaczona zaawansowana stylizacja okien
	if(ChkSkinEnabled())
	{
	  //Pobieranie sciezki nowej aktywnej kompozycji
	  UnicodeString ThemeSkinDir = StringReplace((wchar_t*)lParam, "\\", "\\\\", TReplaceFlags() << rfReplaceAll) + "\\\\Skin";
	  //Plik zaawansowanej stylizacji okien istnieje
	  if(FileExists(ThemeSkinDir + "\\\\Skin.asz"))
	  {
		//Dane pliku zaawansowanej stylizacji okien
		ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
		hSettingsForm->sSkinManager->SkinDirectory = ThemeSkinDir;
		hSettingsForm->sSkinManager->SkinName = "Skin.asz";
		//Ustawianie animacji AlphaControls
		if(ChkThemeAnimateWindows()) hSettingsForm->sSkinManager->AnimEffects->FormShow->Time = 200;
		else hSettingsForm->sSkinManager->AnimEffects->FormShow->Time = 0;
		hSettingsForm->sSkinManager->Effects->AllowGlowing = ChkThemeGlowing();
		//Zmiana kolorystyki AlphaControls
		hSettingsForm->sSkinManager->HueOffset = GetHUE();
		hSettingsForm->sSkinManager->Saturation = GetSaturation();
		//Aktywacja skorkowania AlphaControls
		hSettingsForm->sSkinManager->Active = true;
	  }
	  //Brak pliku zaawansowanej stylizacji okien
	  else hSettingsForm->sSkinManager->Active = false;
	}
	//Zaawansowana stylizacja okien wylaczona
	else hSettingsForm->sSkinManager->Active = false;
  }
  //Okno pierwszego uruchomienia wtyczki zostalo juz stworzone
  if(hFirstRunForm)
  {
	//Wlaczona zaawansowana stylizacja okien
	if(ChkSkinEnabled())
	{
	  //Pobieranie sciezki nowej aktywnej kompozycji
	  UnicodeString ThemeSkinDir = StringReplace((wchar_t*)lParam, "\\", "\\\\", TReplaceFlags() << rfReplaceAll) + "\\\\Skin";
	  //Plik zaawansowanej stylizacji okien istnieje
	  if(FileExists(ThemeSkinDir + "\\\\Skin.asz"))
	  {
		//Dane pliku zaawansowanej stylizacji okien
		ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
		hFirstRunForm->sSkinManager->SkinDirectory = ThemeSkinDir;
		hFirstRunForm->sSkinManager->SkinName = "Skin.asz";
		//Ustawianie animacji AlphaControls
		if(ChkThemeAnimateWindows()) hFirstRunForm->sSkinManager->AnimEffects->FormShow->Time = 200;
		else hFirstRunForm->sSkinManager->AnimEffects->FormShow->Time = 0;
		hFirstRunForm->sSkinManager->Effects->AllowGlowing = ChkThemeGlowing();
		//Zmiana kolorystyki AlphaControls
		hFirstRunForm->sSkinManager->HueOffset = GetHUE();
		hFirstRunForm->sSkinManager->Saturation = GetSaturation();
		//Aktywacja skorkowania AlphaControls
		hFirstRunForm->sSkinManager->Active = true;
	  }
	  //Brak pliku zaawansowanej stylizacji okien
	  else hFirstRunForm->sSkinManager->Active = false;
	}
	//Zaawansowana stylizacja okien wylaczona
	else hFirstRunForm->sSkinManager->Active = false;
  }
  //Okno uzupelniania nazwy zasobu zostalo juz stworzone
  if(hNewComputerForm)
  {
	//Wlaczona zaawansowana stylizacja okien
	if(ChkSkinEnabled())
	{
	  //Pobieranie sciezki nowej aktywnej kompozycji
	  UnicodeString ThemeSkinDir = StringReplace((wchar_t*)lParam, "\\", "\\\\", TReplaceFlags() << rfReplaceAll) + "\\\\Skin";
	  //Plik zaawansowanej stylizacji okien istnieje
	  if(FileExists(ThemeSkinDir + "\\\\Skin.asz"))
	  {
		//Dane pliku zaawansowanej stylizacji okien
		ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
		hNewComputerForm->sSkinManager->SkinDirectory = ThemeSkinDir;
		hNewComputerForm->sSkinManager->SkinName = "Skin.asz";
		//Ustawianie animacji AlphaControls
		if(ChkThemeAnimateWindows()) hNewComputerForm->sSkinManager->AnimEffects->FormShow->Time = 200;
		else hNewComputerForm->sSkinManager->AnimEffects->FormShow->Time = 0;
		hNewComputerForm->sSkinManager->Effects->AllowGlowing = ChkThemeGlowing();
		//Zmiana kolorystyki AlphaControls
		hNewComputerForm->sSkinManager->HueOffset = GetHUE();
		hNewComputerForm->sSkinManager->Saturation = GetSaturation();
		//Aktywacja skorkowania AlphaControls
		hNewComputerForm->sSkinManager->Active = true;
	  }
	  //Brak pliku zaawansowanej stylizacji okien
	  else hNewComputerForm->sSkinManager->Active = false;
	}
	//Zaawansowana stylizacja okien wylaczona
	else hNewComputerForm->sSkinManager->Active = false;
  }

  return 0;
}
//---------------------------------------------------------------------------

INT_PTR __stdcall ServiceNewComputer(WPARAM, LPARAM)
{
  //Przypisanie uchwytu do formy pierwszego uruchomienia
  if(!hFirstRunForm)
  {
	Application->Handle = (HWND)NewComputerForm;
	hNewComputerForm = new TNewComputerForm(Application);
  }
  //Przekazanie nazwy komputera
  hNewComputerForm->pComputerName = ComputerName;
  hNewComputerForm->NewUserResourceNameLabel->Caption = "Podaj nazwê zasobu dla komputera \""+ComputerName+"\"";
  //Ustawienie szerokosci formy
  hNewComputerForm->ClientWidth = 18 + hNewComputerForm->NewUserResourceNameLabel->Width + 18;
  //Pokazanie formy uzupelniania nazwy zasobu
  hNewComputerForm->Show();

  return 0;
}
//---------------------------------------------------------------------------

//Zmiana zasobow w kontach Jabber
void ChangeResources()
{
  //Tryb prosty
  if(PluginModeChk==1)
  {
	//Nazwa komputera rozna od aktualnego zasobu
	if(ComputerName!=ActiveResourceName)
	{
	  //Zapamietanie aktualnej-nowej nazwy zasobu
	  ActiveResourceName = ComputerName;
	  //Zmiana nazwy zasobu
	  PluginLink.CallService(AQQ_SYSTEM_CHANGE_JABBERRESOURCES,1,(LPARAM)ComputerName.w_str());
	  //Wywo³anie notyfikacji RESOURCESCHANGER_SYSTEM_RESOURCECHANGED
	  TPluginHook PluginHook;
	  PluginHook.HookName = RESOURCESCHANGER_SYSTEM_RESOURCECHANGED;
	  PluginHook.wParam = 0;
	  PluginHook.lParam = (LPARAM)ComputerName.w_str();
	  PluginLink.CallService(AQQ_SYSTEM_SENDHOOK,(WPARAM)(&PluginHook),0);
	}
  }
  //Tryb zaawansowany
  else if(PluginModeChk==2)
  {
	//Zasob jest juz zdefiniowany
	if(!ComputerResourceName.IsEmpty())
	{
	  TIniFile *Ini = new TIniFile(GetPluginUserDir()+"\\\\ResourcesChanger\\\\Settings.ini");
	  //Tryb Chucka Norrisa
	  if(Ini->SectionExists("SSID:"+ComputerName))
	  {
		//Pobieranie nazwy zasobu na baziego aktualnego SSID sieci Wi-Fi
		UnicodeString SSIDResourceName = Ini->ReadString("SSID:"+ComputerName,GetNetworkSSID(),"");
		//Zdefiniowano nazwe zasobu dla aktualnego SSID sieci Wi-Fi
		if(!SSIDResourceName.IsEmpty())
		{
		  //Zdefiniowany zasob dla aktualnego SSID sieci Wi-Fi rozny od aktualnego zasobu
		  if(SSIDResourceName!=ActiveResourceName)
		  {
			//Zapamietanie aktualnej-nowej nazwy zasobu
			ActiveResourceName = SSIDResourceName;
			//Zmiana nazwy zasobu
			PluginLink.CallService(AQQ_SYSTEM_CHANGE_JABBERRESOURCES,1,(LPARAM)SSIDResourceName.w_str());
			//Wywo³anie notyfikacji RESOURCESCHANGER_SYSTEM_RESOURCECHANGED
			TPluginHook PluginHook;
			PluginHook.HookName = RESOURCESCHANGER_SYSTEM_RESOURCECHANGED;
			PluginHook.wParam = 0;
			PluginHook.lParam = (LPARAM)SSIDResourceName.w_str();
	  		PluginLink.CallService(AQQ_SYSTEM_SENDHOOK,(WPARAM)(&PluginHook),0);
		  }
		}
		//Nazwa zasobu dla aktualnego SSID sieci Wi-Fi nie zostala zdefiniowana
		else
		{
		  //Zdefiniowany zasob dla nazwy komputera rozny od aktualnego zasobu
		  if(ComputerResourceName!=ActiveResourceName)
		  {
			//Zapamietanie aktualnej-nowej nazwy zasobu
			ActiveResourceName = ComputerResourceName;
			//Zmiana nazwy zasobu
			PluginLink.CallService(AQQ_SYSTEM_CHANGE_JABBERRESOURCES,1,(LPARAM)ComputerResourceName.w_str());
			//Wywo³anie notyfikacji RESOURCESCHANGER_SYSTEM_RESOURCECHANGED
			TPluginHook PluginHook;
			PluginHook.HookName = RESOURCESCHANGER_SYSTEM_RESOURCECHANGED;
			PluginHook.wParam = 0;
			PluginHook.lParam = (LPARAM)ComputerResourceName.w_str();
			PluginLink.CallService(AQQ_SYSTEM_SENDHOOK,(WPARAM)(&PluginHook),0);
		  }
        }
	  }
	  //Zwykly tryb zaawansowany
	  else
	  {
		//Zdefiniowany zasob dla nazwy komputera rozny od aktualnego
		if(ComputerResourceName!=ActiveResourceName)
		{
		  //Zapamietanie aktualnej-nowej nazwy zasobu
		  ActiveResourceName = ComputerResourceName;
		  //Zmiana nazwy zasobu
		  PluginLink.CallService(AQQ_SYSTEM_CHANGE_JABBERRESOURCES,1,(LPARAM)ComputerResourceName.w_str());
		  //Wywo³anie notyfikacji RESOURCESCHANGER_SYSTEM_RESOURCECHANGED
		  TPluginHook PluginHook;
		  PluginHook.HookName = RESOURCESCHANGER_SYSTEM_RESOURCECHANGED;
		  PluginHook.wParam = 0;
		  PluginHook.lParam = (LPARAM)ComputerResourceName.w_str();
		  PluginLink.CallService(AQQ_SYSTEM_SENDHOOK,(WPARAM)(&PluginHook),0);
		}
	  }
	  delete Ini;
	}
	//Zasob nie zostal jeszcze zdefiniowany
	else
	{
      //Generowanie ID dla chmurek
	  DWORD TickID = GetTickCount();
	  //Tworzenie struktury dla chmurki informacyjnej
	  TPluginShowInfo PluginShowInfo;
	  //Pokazanie pierwszej chmurki informacyjnej
	  PluginShowInfo.cbSize = sizeof(TPluginShowInfo);
	  PluginShowInfo.Event = tmeMsgCap;
	  PluginShowInfo.Text = L"ResourcesChanger";
	  PluginShowInfo.ImagePath = (wchar_t*)PluginLink.CallService(AQQ_FUNCTION_GETPNG_FILEPATH,21,0);
	  PluginShowInfo.TimeOut = 1000 * 6;
	  PluginShowInfo.ActionID = L"sResourcesChangerNewComputer";
	  PluginShowInfo.Tick = TickID;
	  PluginLink.CallService(AQQ_FUNCTION_SHOWINFO,0,(LPARAM)(&PluginShowInfo));
	  //Pokazanie drugiej chmurki informacyjnej
	  PluginShowInfo.cbSize = sizeof(TPluginShowInfo);
	  PluginShowInfo.Event = tmeInfo;
	  PluginShowInfo.Text = ("Dla komputera \""+ComputerName+"\" nie zdefiniowano jeszcze nazwy zasobu! Kliknij w tê chmurkê aby to zrobiæ.").w_str();
	  PluginShowInfo.ImagePath = L"";
	  PluginShowInfo.TimeOut = 1000 * 6;
	  PluginShowInfo.ActionID = L"sResourcesChangerNewComputer";
	  PluginShowInfo.Tick = TickID;
	  PluginLink.CallService(AQQ_FUNCTION_SHOWINFO,0,(LPARAM)(&PluginShowInfo));
    }
  }
}
//---------------------------------------------------------------------------

//Zapisywanie zasobów
void ExtractRes(wchar_t* FileName, wchar_t* ResName, wchar_t* ResType)
{
  TPluginTwoFlagParams PluginTwoFlagParams;
  PluginTwoFlagParams.cbSize = sizeof(TPluginTwoFlagParams);
  PluginTwoFlagParams.Param1 = ResName;
  PluginTwoFlagParams.Param2 = ResType;
  PluginTwoFlagParams.Flag1 = (int)HInstance;
  PluginLink.CallService(AQQ_FUNCTION_SAVERESOURCE,(WPARAM)&PluginTwoFlagParams,(LPARAM)FileName);
}
//---------------------------------------------------------------------------

//Obliczanie sumy kontrolnej pliku
UnicodeString MD5File(UnicodeString FileName)
{
  if(FileExists(FileName))
  {
	UnicodeString Result;
    TFileStream *fs;

	fs = new TFileStream(FileName, fmOpenRead | fmShareDenyWrite);
	try
	{
	  TIdHashMessageDigest5 *idmd5= new TIdHashMessageDigest5();
	  try
	  {
	    Result = idmd5->HashStreamAsHex(fs);
	  }
	  __finally
	  {
	    delete idmd5;
	  }
    }
	__finally
    {
	  delete fs;
    }

    return Result;
  }
  else
   return 0;
}
//---------------------------------------------------------------------------

//Odczyt ustawien wtyczki
void LoadSettings()
{
  //Wylaczenie timera
  KillTimer(hTimerFrm,TIMER_CHKSSID);
  //Odczyt ustawien
  TIniFile *Ini = new TIniFile(SettingsFileDir);
  //Tryb dzialania
  PluginModeChk = Ini->ReadInteger("General","Mode",1);
  //Wlaczenie timera
  if(PluginModeChk==2) SetTimer(hTimerFrm,TIMER_CHKSSID,60000,(TIMERPROC)TimerFrmProc);
  //Zdefiniowany przez usera zasob
  ComputerResourceName = Ini->ReadString("Resources",ComputerName,"");
  //
  delete Ini;
}
//---------------------------------------------------------------------------

//Zaladowanie wtyczki
extern "C" INT_PTR __declspec(dllexport) __stdcall Load(PPluginLink Link)
{
  //Linkowanie wtyczki z komunikatorem
  PluginLink = *Link;
  //Pobranie sciezki do pliku ustawien
  SettingsFileDir = GetPluginUserDir()+"\\\\ResourcesChanger\\\\Settings.ini";
  //Tworzeniu z ustawieniami wtyczki
  if(!DirectoryExists(GetPluginUserDir()+"\\\\ResourcesChanger"))
   CreateDir(GetPluginUserDir()+"\\\\ResourcesChanger");
  //Wypakiwanie ikonki ResourcesChanger.dll.png
  //55F8F054BC7D7A92BEA90ED00C8A50DA
  if(!DirectoryExists(GetPluginUserDir()+"\\\\Shared"))
   CreateDir(GetPluginUserDir()+"\\\\Shared");
  if(!FileExists(GetPluginUserDir()+"\\\\Shared\\\\ResourcesChanger.dll.png"))
   ExtractRes((GetPluginUserDir()+"\\\\Shared\\\\ResourcesChanger.dll.png").w_str(),L"SHARED",L"DATA");
  else if(MD5File(GetPluginUserDir()+"\\\\Shared\\\\ResourcesChanger.dll.png")!="55F8F054BC7D7A92BEA90ED00C8A50DA")
   ExtractRes((GetPluginUserDir()+"\\\\Shared\\\\ResourcesChanger.dll.png").w_str(),L"SHARED",L"DATA");
  //Pobieranie zasobu glownego konta Jabber
  TPluginStateChange PluginStateChange;
  PluginLink.CallService(AQQ_FUNCTION_GETNETWORKSTATE,(WPARAM)(&PluginStateChange),0);
  ActiveResourceName = (wchar_t*)PluginStateChange.Resource;
  //Pobieranie nazwy komputera
  wchar_t compName[256];
  DWORD len = sizeof(compName);
  GetComputerNameEx(ComputerNameDnsHostname,compName,&len);
  ComputerName = compName;
  //Tworzenie serwisu do uzupelniania zasobu nowego komputera
  PluginLink.CreateServiceFunction(L"sResourcesChangerNewComputer",ServiceNewComputer);
  //Rejestowanie klasy okna timera
  WNDCLASSEX wincl;
  wincl.cbSize = sizeof (WNDCLASSEX);
  wincl.style = 0;
  wincl.lpfnWndProc = TimerFrmProc;
  wincl.cbClsExtra = 0;
  wincl.cbWndExtra = 0;
  wincl.hInstance = HInstance;
  wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
  wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
  wincl.lpszMenuName = NULL;
  wincl.lpszClassName = L"TResourcesChangerTimer";
  wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
  RegisterClassEx(&wincl);
  //Tworzenie okna timera
  hTimerFrm = CreateWindowEx(0, L"TResourcesChangerTimer", L"",	0, 0, 0, 0, 0, NULL, NULL, HInstance, NULL);
  //Hook na polaczenie sie z siecia przy wlaczeniu AQQ
  PluginLink.HookEvent(AQQ_SYSTEM_SETLASTSTATE,OnSetLastState);
  //Hook na zmianê kompozycji
  PluginLink.HookEvent(AQQ_SYSTEM_THEMECHANGED,OnThemeChanged);
  //Hook na zaladowanie wszystkich modulow w AQQ
  PluginLink.HookEvent(AQQ_SYSTEM_MODULESLOADED,OnModulesLoaded);
  //Jezeli wszystkie moduly w AQQ zostaly juz zaladowany przed wczytaniem wtyczki
  if(PluginLink.CallService(AQQ_SYSTEM_MODULESLOADED,0,0))
  {
    //Jezeli nie skonfigurowano jeszcze wtyczki
	if(!FileExists(SettingsFileDir))
	{
	  //Przypisanie uchwytu do formy pierwszego uruchomienia
	  if(!hFirstRunForm)
	  {
		Application->Handle = (HWND)FirstRunForm;
		hFirstRunForm = new TFirstRunForm(Application);
	  }
	  //Przekazanie nazwy komputera
	  hFirstRunForm->pComputerName = ComputerName;
	  hFirstRunForm->ComputerNameEdit->BoundLabel->Caption = ComputerName+":";
	  hFirstRunForm->ComputerNameEdit->Text = ComputerName;
	  hFirstRunForm->ComputerNameEdit->Left = 52 + (5*ComputerName.Length());
	  //Pokazanie okna pierwszego uruchomienia
	  hFirstRunForm->Show();
	}
	//Wtyczka zostala juz wczesniej skonfigurowana
	else
	{
	  //Odczyt ustawien w rdzeniu wtyczki
	  LoadSettings();
	  //Zmiana zasobow w kontach Jabber
	  ChangeResources();
    }
  }

  return 0;
}
//---------------------------------------------------------------------------

//Wyladowanie wtyczki
extern "C" INT_PTR __declspec(dllexport) __stdcall Unload()
{
  //Wyladowanie timerow
  KillTimer(hTimerFrm,TIMER_CHKSSID);
  //Usuwanie okna timera
  DestroyWindow(hTimerFrm);
  //Wyrejestowanie klasy okna timera
  UnregisterClass(L"TResourcesChangerTimer",HInstance);
  //Wyladowanie wszystkich hookow
  PluginLink.UnhookEvent(OnModulesLoaded);
  PluginLink.UnhookEvent(OnSetLastState);
  PluginLink.UnhookEvent(OnThemeChanged);
  //Usuwanie serwisu do uzupelniania zasobu nowego komputera
  PluginLink.DestroyServiceFunction(ServiceNewComputer);

  return 0;
}
//---------------------------------------------------------------------------

//Ustawienia wtyczki
extern "C" INT_PTR __declspec(dllexport)__stdcall Settings()
{
  //Przypisanie uchwytu do formy ustawien
  if(!hSettingsForm)
  {
	Application->Handle = (HWND)SettingsForm;
	hSettingsForm = new TSettingsForm(Application);
  }
  //Pokazanie okna ustawien
  hSettingsForm->Show();

  return 0;
}
//---------------------------------------------------------------------------

//Informacje o wtyczce
extern "C" PPluginInfo __declspec(dllexport) __stdcall AQQPluginInfo(DWORD AQQVersion)
{
  PluginInfo.cbSize = sizeof(TPluginInfo);
  PluginInfo.ShortName = L"ResourcesChanger";
  PluginInfo.Version = PLUGIN_MAKE_VERSION(1,3,4,0);
  PluginInfo.Description = L"Zmienia nazwê zasobu we wszystkich kontach Jabber zale¿nie od nazwy naszego komputera oraz innych danych (np. nazwy aktywnego po³¹czenia Wi-Fi).";
  PluginInfo.Author = L"Krzysztof Grochocki";
  PluginInfo.AuthorMail = L"kontakt@beherit.pl";
  PluginInfo.Copyright = L"Krzysztof Grochocki";
  PluginInfo.Homepage = L"http://beherit.pl";

  return &PluginInfo;
}
//---------------------------------------------------------------------------
