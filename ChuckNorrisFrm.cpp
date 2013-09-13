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
#include <Wlanapi.h>
#pragma hdrstop
#include "ChuckNorrisFrm.h"
#include "SetStateFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sSkinProvider"
#pragma link "sBevel"
#pragma link "sButton"
#pragma link "sPageControl"
#pragma link "sEdit"
#pragma link "sListView"
#pragma link "sSpeedButton"
#pragma link "sPanel"
#pragma resource "*.dfm"
#pragma comment(lib, "Wlanapi")
TChuckNorrisForm *ChuckNorrisForm;
//---------------------------------------------------------------------------
__declspec(dllimport)UnicodeString GetPluginUserDir();
__declspec(dllimport)UnicodeString GetNetworkSSID();
__declspec(dllimport)void ChangeResources();
//---------------------------------------------------------------------------

__fastcall TChuckNorrisForm::TChuckNorrisForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TChuckNorrisForm::WMTransparency(TMessage &Message)
{
  Application->ProcessMessages();
  if(SkinManagerEnabled) sSkinProvider->BorderForm->UpdateExBordersPos(true,(int)Message.LParam);
}
//---------------------------------------------------------------------------

void __fastcall TChuckNorrisForm::FormCreate(TObject *Sender)
{
  //Hack na blad w AC
  #if defined(_WIN64)
  ListView->SkinData->SkinSection = "";
  #endif
}
//---------------------------------------------------------------------------

void __fastcall TChuckNorrisForm::FormShow(TObject *Sender)
{
  //Odczyt ustawien wtyczki
  aLoadSettings->Execute();
  //Wylaczanie przycisku zapisywania
  SaveButton->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TChuckNorrisForm::aExitExecute(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TChuckNorrisForm::aLoadSettingsExecute(TObject *Sender)
{
  TIniFile *Ini = new TIniFile(GetPluginUserDir()+"\\\\ResourcesChanger\\\\Settings.ini");
  ListView->Items->Clear();
  TStringList *ResourcesList = new TStringList;
  Ini->ReadSection("SSID:"+ComputerName,ResourcesList);
  if(ResourcesList->Count)
  {
	for(int Count=0;Count<ResourcesList->Count;Count++)
	{
	  UnicodeString SSID = ResourcesList->Strings[Count];
	  UnicodeString Resource = Ini->ReadString("SSID:"+ComputerName,SSID,"");
	  ListView->Items->Add();
	  ListView->Items->Item[ListView->Items->Count-1]->Caption = SSID;
	  ListView->Items->Item[ListView->Items->Count-1]->SubItems->Add(Resource);
	}
  }
  delete ResourcesList;
  delete Ini;
}
//---------------------------------------------------------------------------

void __fastcall TChuckNorrisForm::aSaveSettingsExecute(TObject *Sender)
{
  TIniFile *Ini = new TIniFile(GetPluginUserDir()+"\\\\ResourcesChanger\\\\Settings.ini");
  Ini->EraseSection("SSID:"+ComputerName);
  if(ListView->Items->Count)
  {
	for(int ItemCount=0;ItemCount<ListView->Items->Count;ItemCount++)
	 Ini->WriteString("SSID:"+ComputerName,ListView->Items->Item[ItemCount]->Caption,ListView->Items->Item[ItemCount]->SubItems->Strings[0]);
  }
  delete Ini;
}
//---------------------------------------------------------------------------

void __fastcall TChuckNorrisForm::NewItemSpeedButtonClick(TObject *Sender)
{
  //Kasowania pol szybkiej edycji
  NewSSIDEdit->Text = "";
  NewResourceEdit->Text = "";
  ListView->ItemIndex = -1;
}
//---------------------------------------------------------------------------

void __fastcall TChuckNorrisForm::EditSpeedButtonClick(TObject *Sender)
{
  //Edycja juz dodanych elementow
  if(ListView->ItemIndex!=-1)
  {
	//Zmiana elementow
	ListView->Items->Item[ListView->ItemIndex]->Caption = NewSSIDEdit->Text;
	ListView->Items->Item[ListView->ItemIndex]->SubItems->Strings[0] = NewResourceEdit->Text;
	//Aktywowanie przycisku zapisywania ustawien
	SaveButton->Enabled = true;
  }
  //Dowananie nowych elementow
  else
  {
	if((!NewSSIDEdit->Text.IsEmpty())&&(!NewResourceEdit->Text.IsEmpty()))
	{
	  //Dodawanie elementow
	  ListView->Items->Add();
	  ListView->Items->Item[ListView->Items->Count-1]->Caption = NewSSIDEdit->Text;
	  ListView->Items->Item[ListView->Items->Count-1]->SubItems->Add(NewResourceEdit->Text);
	  //Aktywowanie przycisku zapisywania ustawien
	  SaveButton->Enabled = true;
	}
  }
}
//---------------------------------------------------------------------------

void __fastcall TChuckNorrisForm::DeleteItemSpeedButtonClick(TObject *Sender)
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

void __fastcall TChuckNorrisForm::ListViewSelectItem(TObject *Sender, TListItem *Item,
          bool Selected)
{
  //Wczytyawanie elementow do szybkiej edycji
  if(ListView->ItemIndex!=-1)
  {
	NewSSIDEdit->Text = ListView->Items->Item[ListView->ItemIndex]->Caption;
	NewResourceEdit->Text = ListView->Items->Item[ListView->ItemIndex]->SubItems->Strings[0];
  }
}
//---------------------------------------------------------------------------

void __fastcall TChuckNorrisForm::GetSSIDClick(TObject *Sender)
{
  NewSSIDEdit->Text = GetNetworkSSID();
}
//---------------------------------------------------------------------------

void __fastcall TChuckNorrisForm::SaveButtonClick(TObject *Sender)
{
  //Zapisanie ustawien
  aSaveSettings->Execute();
  //Zamkniecie formy
  Close();
  //Zmiana zasobow w kontach Jabber
  ChangeResources();
}
//---------------------------------------------------------------------------

void __fastcall TChuckNorrisForm::PopupMenu2Popup(TObject *Sender)
{
  TIniFile *Ini = new TIniFile(GetPluginUserDir()+"\\\\ResourcesChanger\\\\Settings.ini");
  ChangeStateItem->Checked = Ini->SectionExists("State:"+ComputerName+":"+ListView->Items->Item[ListView->ItemIndex]->Caption);
  delete Ini;
}
//---------------------------------------------------------------------------

void __fastcall TChuckNorrisForm::ChangeStateItemClick(TObject *Sender)
{
  SetStateForm = new TSetStateForm(Application);
  SetStateForm->SkinManagerEnabled = SkinManagerEnabled;
  SetStateForm->ComputerName = ComputerName+":"+ListView->Items->Item[ListView->ItemIndex]->Caption;
  SetStateForm->Caption = SetStateForm->Caption + " - " + ComputerName+":"+ListView->Items->Item[ListView->ItemIndex]->Caption;
  SetStateForm->ShowModal();
  delete SetStateForm;
}
//---------------------------------------------------------------------------
