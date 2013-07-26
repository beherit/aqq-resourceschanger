#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#pragma argsused
#include <PluginAPI.h>
#include "FirstRunFrm.h"
#include "NewComputerFrm.h"
#include "SettingsFrm.h"
#define RESOURCESCHANGER_SYSTEM_RESOURCECHANGED L"ResourcesChanger/System/ResourceChanged"
#include <IdHashMessageDigest.hpp>

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
//Zdefiniowany przez usera zasob
UnicodeString UserResourceName;
//Zasob glownego konta Jabber
UnicodeString DefaultResourceName;
//Nazwa komputera
UnicodeString ComputerName;
//FORWARD-AQQ-HOOKS----------------------------------------------------------
int __stdcall OnModulesLoaded(WPARAM wParam, LPARAM lParam);
int __stdcall OnThemeChanged(WPARAM wParam, LPARAM lParam);
int __stdcall ServiceNewComputer(WPARAM, LPARAM);
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

//Hook na zaladowanie wszystkich modulow w AQQ
int __stdcall OnModulesLoaded(WPARAM wParam, LPARAM lParam)
{
  //Odczyt ustawien w rdzeniu wtyczki
  LoadSettings();
  //Zmiana zasobow w kontach Jabber
  ChangeResources();

  return 0;
}
//---------------------------------------------------------------------------

//Hook na zmianê kompozycji
int __stdcall OnThemeChanged(WPARAM wParam, LPARAM lParam)
{
  //Pobieranie sciezki nowej aktywnej kompozycji
  UnicodeString ThemeDir = StringReplace((wchar_t*)lParam, "\\", "\\\\", TReplaceFlags() << rfReplaceAll);
  //Okno ustawien zostalo juz stworzone
  if(hSettingsForm)
  {
	//Wlaczona zaawansowana stylizacja okien
	if(ChkSkinEnabled())
	{
	  UnicodeString ThemeSkinDir = GetThemeSkinDir();
	  //Plik zaawansowanej stylizacji okien istnieje
	  if(FileExists(ThemeSkinDir + "\\\\Skin.asz"))
	  {
		ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
		hSettingsForm->sSkinManager->SkinDirectory = ThemeSkinDir;
		hSettingsForm->sSkinManager->SkinName = "Skin.asz";
		if(ChkThemeAnimateWindows()) hSettingsForm->sSkinManager->AnimEffects->FormShow->Time = 200;
		else hSettingsForm->sSkinManager->AnimEffects->FormShow->Time = 0;
		hSettingsForm->sSkinManager->Effects->AllowGlowing = ChkThemeGlowing();
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
	  UnicodeString ThemeSkinDir = GetThemeSkinDir();
	  //Plik zaawansowanej stylizacji okien istnieje
	  if(FileExists(ThemeSkinDir + "\\\\Skin.asz"))
	  {
		ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
		hFirstRunForm->sSkinManager->SkinDirectory = ThemeSkinDir;
		hFirstRunForm->sSkinManager->SkinName = "Skin.asz";
		if(ChkThemeAnimateWindows()) hFirstRunForm->sSkinManager->AnimEffects->FormShow->Time = 200;
		else hFirstRunForm->sSkinManager->AnimEffects->FormShow->Time = 0;
		hFirstRunForm->sSkinManager->Effects->AllowGlowing = ChkThemeGlowing();
		hFirstRunForm->sSkinManager->Active = true;
	  }
	  //Brak pliku zaawansowanej stylizacji okien
	  else hFirstRunForm->sSkinManager->Active = false;
	}
	//Zaawansowana stylizacja okien wylaczona
	else hFirstRunForm->sSkinManager->Active = false;
	//Skorkowanie poszczegolnych komponentow
	if(hFirstRunForm->sSkinManager->Active)
	{
	  //Kolor WebLabel'ow
	  hFirstRunForm->WebWelcomeLabel->Font->Color = hFirstRunForm->sSkinManager->GetGlobalFontColor();
	  hFirstRunForm->WebWelcomeLabel->HoverFont->Color = hFirstRunForm->sSkinManager->GetGlobalFontColor();
	}
	else
	{
	  //Kolor WebLabel'ow
	  hFirstRunForm->WebWelcomeLabel->Font->Color = clWindowText;
	  hFirstRunForm->WebWelcomeLabel->HoverFont->Color = clWindowText;
	}
  }
  //Okno uzupelniania nazwy zasobu zostalo juz stworzone
  if(hNewComputerForm)
  {
	//Wlaczona zaawansowana stylizacja okien
	if(ChkSkinEnabled())
	{
	  UnicodeString ThemeSkinDir = GetThemeSkinDir();
	  //Plik zaawansowanej stylizacji okien istnieje
	  if(FileExists(ThemeSkinDir + "\\\\Skin.asz"))
	  {
		ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
		hNewComputerForm->sSkinManager->SkinDirectory = ThemeSkinDir;
		hNewComputerForm->sSkinManager->SkinName = "Skin.asz";
		if(ChkThemeAnimateWindows()) hNewComputerForm->sSkinManager->AnimEffects->FormShow->Time = 200;
		else hNewComputerForm->sSkinManager->AnimEffects->FormShow->Time = 0;
		hNewComputerForm->sSkinManager->Effects->AllowGlowing = ChkThemeGlowing();
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

int __stdcall ServiceNewComputer(WPARAM, LPARAM)
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
	if(DefaultResourceName!=ComputerName)
	{
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
	if(!UserResourceName.IsEmpty())
	{
	  if(DefaultResourceName!=UserResourceName)
	  {
		//Zmiana nazwy zasobu
		PluginLink.CallService(AQQ_SYSTEM_CHANGE_JABBERRESOURCES,1,(LPARAM)UserResourceName.w_str());
		//Wywo³anie notyfikacji RESOURCESCHANGER_SYSTEM_RESOURCECHANGED
		TPluginHook PluginHook;
		PluginHook.HookName = RESOURCESCHANGER_SYSTEM_RESOURCECHANGED;
		PluginHook.wParam = 0;
		PluginHook.lParam = (LPARAM)UserResourceName.w_str();
		PluginLink.CallService(AQQ_SYSTEM_SENDHOOK,(WPARAM)(&PluginHook),0);
	  }
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
  TIniFile *Ini = new TIniFile(SettingsFileDir);
  //Tryb dzialania
  PluginModeChk = Ini->ReadInteger("General","Mode",1);
  //Zdefiniowany przez usera zasob
  UserResourceName = Ini->ReadString("Resources",ComputerName,"");
  delete Ini;
}
//---------------------------------------------------------------------------

//Zaladowanie wtyczki
extern "C" int __declspec(dllexport) __stdcall Load(PPluginLink Link)
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
  DefaultResourceName = (wchar_t*)PluginStateChange.Resource;
  //Pobieranie nazwy komputera
  wchar_t compName[256];
  DWORD len = sizeof(compName);
  GetComputerNameEx(ComputerNameDnsHostname,compName,&len);
  ComputerName = compName;
  //Tworzenie serwisu do uzupelniania zasobu nowego komputera
  PluginLink.CreateServiceFunction(L"sResourcesChangerNewComputer",ServiceNewComputer);
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
extern "C" int __declspec(dllexport) __stdcall Unload()
{
  //Wyladowanie wszystkich hookow
  PluginLink.UnhookEvent(OnModulesLoaded);
  PluginLink.UnhookEvent(OnThemeChanged);
  //Usuwanie serwisu do uzupelniania zasobu nowego komputera
  PluginLink.DestroyServiceFunction(ServiceNewComputer);

  return 0;
}
//---------------------------------------------------------------------------

//Ustawienia wtyczki
extern "C" int __declspec(dllexport)__stdcall Settings()
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
  PluginInfo.Version = PLUGIN_MAKE_VERSION(1,2,0,0);
  PluginInfo.Description = L"Zmienia nazwê zasobu we wszystkich kontach Jabber zale¿nie od nazwy naszego komputera.";
  PluginInfo.Author = L"Krzysztof Grochocki (Beherit)";
  PluginInfo.AuthorMail = L"kontakt@beherit.pl";
  PluginInfo.Copyright = L"Krzysztof Grochocki (Beherit)";
  PluginInfo.Homepage = L"http://beherit.pl";

  return &PluginInfo;
}
//---------------------------------------------------------------------------
