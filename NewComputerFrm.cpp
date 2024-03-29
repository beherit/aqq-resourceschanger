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

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "NewComputerFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sBevel"
#pragma link "sButton"
#pragma link "sEdit"
#pragma link "sLabel"
#pragma link "sPanel"
#pragma link "sSkinManager"
#pragma link "sSkinProvider"
#pragma resource "*.dfm"
TNewComputerForm *NewComputerForm;
//---------------------------------------------------------------------------
__declspec(dllimport)UnicodeString GetPluginUserDir();
__declspec(dllimport)UnicodeString GetThemeSkinDir();
__declspec(dllimport)bool ChkSkinEnabled();
__declspec(dllimport)bool ChkThemeAnimateWindows();
__declspec(dllimport)bool ChkThemeGlowing();
__declspec(dllimport)int GetSaturation();
__declspec(dllimport)int GetHUE();
__declspec(dllimport)void LoadSettings();
__declspec(dllimport)void ChangeResources();
//---------------------------------------------------------------------------

__fastcall TNewComputerForm::TNewComputerForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TNewComputerForm::WMTransparency(TMessage &Message)
{
  Application->ProcessMessages();
  if(sSkinManager->Active) sSkinProvider->BorderForm->UpdateExBordersPos(true,(int)Message.LParam);
}
//---------------------------------------------------------------------------

void __fastcall TNewComputerForm::FormCreate(TObject *Sender)
{
  //Wlaczona zaawansowana stylizacja okien
  if(ChkSkinEnabled())
  {
	UnicodeString ThemeSkinDir = GetThemeSkinDir();
	//Plik zaawansowanej stylizacji okien istnieje
	if(FileExists(ThemeSkinDir + "\\\\Skin.asz"))
	{
	  //Dane pliku zaawansowanej stylizacji okien
	  ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
	  sSkinManager->SkinDirectory = ThemeSkinDir;
	  sSkinManager->SkinName = "Skin.asz";
	  //Ustawianie animacji AlphaControls
	  if(ChkThemeAnimateWindows()) sSkinManager->AnimEffects->FormShow->Time = 200;
	  else sSkinManager->AnimEffects->FormShow->Time = 0;
	  sSkinManager->Effects->AllowGlowing = ChkThemeGlowing();
	  //Zmiana kolorystyki AlphaControls
	  sSkinManager->HueOffset = GetHUE();
	  sSkinManager->Saturation = GetSaturation();
	  //Aktywacja skorkowania AlphaControls
	  sSkinManager->Active = true;
	}
	//Brak pliku zaawansowanej stylizacji okien
	else sSkinManager->Active = false;
  }
  //Zaawansowana stylizacja okien wylaczona
  else sSkinManager->Active = false;
}
//---------------------------------------------------------------------------

void __fastcall TNewComputerForm::NewUserResourceNameEditChange(TObject *Sender)
{
  //Blokowanie/odblokowanie przycisku zapisania nazwy zasobu
  SaveButton->Enabled = !NewUserResourceNameEdit->Text.IsEmpty();
}
//---------------------------------------------------------------------------

void __fastcall TNewComputerForm::SaveButtonClick(TObject *Sender)
{
  //Zapisanie nowej nazwy zasobu
  TIniFile *Ini = new TIniFile(GetPluginUserDir()+"\\\\ResourcesChanger\\\\Settings.ini");
  Ini->WriteString("Resources",pComputerName,NewUserResourceNameEdit->Text);
  delete Ini;
  //Zamkniecie formy
  Close();
  //Odczyt ustawien w rdzeniu wtyczki
  LoadSettings();
  //Zmiana zasobow w kontach Jabber
  ChangeResources();
}
//---------------------------------------------------------------------------

void __fastcall TNewComputerForm::aExitExecute(TObject *Sender)
{
  //Anulowanie dodawania nazwy zasobu
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TNewComputerForm::sSkinManagerSysDlgInit(TacSysDlgData DlgData, bool &AllowSkinning)
{
  AllowSkinning = false;
}
//---------------------------------------------------------------------------

