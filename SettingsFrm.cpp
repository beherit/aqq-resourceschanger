//---------------------------------------------------------------------------
// Copyright (C) 2012-2013 Krzysztof Grochocki
//
// This file is part of ResourcesChanger
//
// ResourcesChanger is free software; you can redistribute it and/or modify
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
// along with GNU Radio; see the file COPYING. If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street,
// Boston, MA 02110-1301, USA.
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "SettingsFrm.h"
#include "ChuckNorrisFrm.h"
#include "SetStateFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sBevel"
#pragma link "sButton"
#pragma link "sEdit"
#pragma link "sLabel"
#pragma link "sListView"
#pragma link "sPageControl"
#pragma link "sRadioButton"
#pragma link "sSkinManager"
#pragma link "sSkinProvider"
#pragma link "sSpeedButton"
#pragma resource "*.dfm"
TSettingsForm *SettingsForm;
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

__fastcall TSettingsForm::TSettingsForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::WMTransparency(TMessage &Message)
{
  Application->ProcessMessages();
  if(sSkinManager->Active) sSkinProvider->BorderForm->UpdateExBordersPos(true,(int)Message.LParam);
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::FormCreate(TObject *Sender)
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

void __fastcall TSettingsForm::FormShow(TObject *Sender)
{
  //Odczyt ustawien wtyczki
  aLoadSettings->Execute();
  //Wylaczanie przycisku zapisywania
  SaveButton->Enabled = false;
  //Ustawianie domyslnej karty
  PageControl->ActivePage = ModeTabSheet;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::aLoadSettingsExecute(TObject *Sender)
{
  TIniFile *Ini = new TIniFile(GetPluginUserDir()+"\\\\ResourcesChanger\\\\Settings.ini");
  //Tryb dzialania
  if(Ini->ReadInteger("General","Mode",1)==1)
   SimpleModeRadioButton->Checked = true;
  else
   AdvancedModeRadioButton->Checked = true;
  //Zdefiniowane przez usera zasoby
  ListView->Items->Clear();
  TStringList *ResourcesList = new TStringList;
  Ini->ReadSection("Resources",ResourcesList);
  if(ResourcesList->Count)
  {
	for(int Count=0;Count<ResourcesList->Count;Count++)
	{
	  UnicodeString Computer = ResourcesList->Strings[Count];
	  UnicodeString Resource = Ini->ReadString("Resources",Computer,"");
	  ListView->Items->Add();
	  ListView->Items->Item[ListView->Items->Count-1]->Caption = Computer;
	  ListView->Items->Item[ListView->Items->Count-1]->SubItems->Add(Resource);
	}
  }
  delete ResourcesList;
  delete Ini;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::aSaveSettingsExecute(TObject *Sender)
{
  TIniFile *Ini = new TIniFile(GetPluginUserDir()+"\\\\ResourcesChanger\\\\Settings.ini");
  //Tryb dzialania
  if(SimpleModeRadioButton->Checked)
   Ini->WriteInteger("General","Mode",1);
  else
   Ini->WriteInteger("General","Mode",2);
  //Nazwy komputerow i zasobow
  Ini->EraseSection("Resources");
  if(ListView->Items->Count)
  {
	for(int ItemCount=0;ItemCount<ListView->Items->Count;ItemCount++)
	 Ini->WriteString("Resources",ListView->Items->Item[ItemCount]->Caption,ListView->Items->Item[ItemCount]->SubItems->Strings[0]);
  }
  delete Ini;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::aResourcesListChkExecute(TObject *Sender)
{
  SaveButton->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::NewItemSpeedButtonClick(TObject *Sender)
{
  //Kasowania pol szybkiej edycji
  NewComputerEdit->Text = "";
  NewResourceEdit->Text = "";
  ListView->ItemIndex = -1;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::EditSpeedButtonClick(TObject *Sender)
{
  //Edycja juz dodanych elementow
  if(ListView->ItemIndex!=-1)
  {
	//Zmiana elementow
	ListView->Items->Item[ListView->ItemIndex]->Caption = NewComputerEdit->Text;
	ListView->Items->Item[ListView->ItemIndex]->SubItems->Strings[0] = NewResourceEdit->Text;
	//Aktywowanie przycisku zapisywania ustawien
	SaveButton->Enabled = true;
  }
  //Dowananie nowych elementow
  else
  {
	if((!NewComputerEdit->Text.IsEmpty())&&(!NewResourceEdit->Text.IsEmpty()))
	{
	  //Dodawanie elementow
	  ListView->Items->Add();
	  ListView->Items->Item[ListView->Items->Count-1]->Caption = NewComputerEdit->Text;
	  ListView->Items->Item[ListView->Items->Count-1]->SubItems->Add(NewResourceEdit->Text);
	  //Aktywowanie przycisku zapisywania ustawien
	  SaveButton->Enabled = true;
	}
  }
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::DeleteItemSpeedButtonClick(TObject *Sender)
{
  //Usuwanie zaznaczonych elementow
  if(ListView->ItemIndex!=-1)
  {
	//Usuwanie elementu
	ListView->Items->Item[ListView->ItemIndex]->Delete();
	//Aktywowanie przycisku zapisywania ustawien
	SaveButton->Enabled = true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::ListViewSelectItem(TObject *Sender, TListItem *Item,
          bool Selected)
{
  //Wczytyawanie elementow do szybkiej edycji
  if(ListView->ItemIndex!=-1)
  {
	NewComputerEdit->Text = ListView->Items->Item[ListView->ItemIndex]->Caption;
	NewResourceEdit->Text = ListView->Items->Item[ListView->ItemIndex]->SubItems->Strings[0];
  }
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::aExitExecute(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::SaveButtonClick(TObject *Sender)
{
  //Wylaczanie przyciskow
  SaveButton->Enabled = false;
  CancelButton->Enabled = false;
  OKButton->Enabled = false;
  //Zapisanie ustawien
  aSaveSettings->Execute();
  //Odczyt ustawien w rdzeniu wtyczki
  LoadSettings();
  //Zmiana zasobow w kontach Jabber
  ChangeResources();
  //Wlaczanie przyciskow
  CancelButton->Enabled = true;
  OKButton->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::OKButtonClick(TObject *Sender)
{
  //Wylaczanie przyciskow
  SaveButton->Enabled = false;
  CancelButton->Enabled = false;
  OKButton->Enabled = false;
  //Zapisanie ustawien
  aSaveSettings->Execute();
  //Zamkniecie formy
  Close();
  //Odczyt ustawien w rdzeniu wtyczki
  LoadSettings();
  //Zmiana zasobow w kontach Jabber
  ChangeResources();
  //Wlaczanie przyciskow
  CancelButton->Enabled = true;
  OKButton->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::ChuckNorrisItemClick(TObject *Sender)
{
  ChuckNorrisForm = new TChuckNorrisForm(Application);
  ChuckNorrisForm->SkinManagerEnabled = sSkinManager->Active;
  ChuckNorrisForm->ComputerName = ListView->Items->Item[ListView->ItemIndex]->Caption;
  ChuckNorrisForm->Caption = ChuckNorrisForm->Caption + " - " + ListView->Items->Item[ListView->ItemIndex]->Caption;
  ChuckNorrisForm->ShowModal();
  delete ChuckNorrisForm;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::ChangeStateItemClick(TObject *Sender)
{
  SetStateForm = new TSetStateForm(Application);
  SetStateForm->SkinManagerEnabled = sSkinManager->Active;
  SetStateForm->ComputerName = ListView->Items->Item[ListView->ItemIndex]->Caption;
  SetStateForm->Caption = SetStateForm->Caption + " - " + ListView->Items->Item[ListView->ItemIndex]->Caption;
  SetStateForm->ShowModal();
  delete SetStateForm;
}
//---------------------------------------------------------------------------

void __fastcall TSettingsForm::PopupMenuPopup(TObject *Sender)
{
  if(ListView->ItemIndex!=-1)
  {
	TIniFile *Ini = new TIniFile(GetPluginUserDir()+"\\\\ResourcesChanger\\\\Settings.ini");
	ChuckNorrisItem->Enabled = true;
	ChuckNorrisItem->Checked = Ini->SectionExists("SSID:"+ListView->Items->Item[ListView->ItemIndex]->Caption);
	ChangeStateItem->Enabled = true;
	ChangeStateItem->Checked = Ini->SectionExists("State:"+ListView->Items->Item[ListView->ItemIndex]->Caption);
	delete Ini;
  }
  else
  {
	ChuckNorrisItem->Enabled = false;
	ChuckNorrisItem->Checked = false;
	ChangeStateItem->Enabled = false;
	ChangeStateItem->Checked = false;
  }
}
//---------------------------------------------------------------------------
