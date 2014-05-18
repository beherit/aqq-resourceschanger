//---------------------------------------------------------------------------
// Copyright (C) 2012-2014 Krzysztof Grochocki
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
#include "SetStateFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "sBevel"
#pragma link "sButton"
#pragma link "sSkinProvider"
#pragma link "sComboBox"
#pragma link "acAlphaImageList"
#pragma resource "*.dfm"
TSetStateForm *SetStateForm;
//---------------------------------------------------------------------------
__declspec(dllimport)UnicodeString GetPluginUserDir();
__declspec(dllimport)UnicodeString GetIconPath(int Icon);
__declspec(dllimport)int GetState();
//---------------------------------------------------------------------------

__fastcall TSetStateForm::TSetStateForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TSetStateForm::WMTransparency(TMessage &Message)
{
  Application->ProcessMessages();
  if(SkinManagerEnabled) sSkinProvider->BorderForm->UpdateExBordersPos(true,(int)Message.LParam);
}
//---------------------------------------------------------------------------

void __fastcall TSetStateForm::FormShow(TObject *Sender)
{
  //Wczytanie ikon z interfesju AQQ
  sAlphaImageList->AcBeginUpdate();
  sAlphaImageList->Clear();
  sAlphaImageList->LoadFromFile(GetIconPath(2));
  sAlphaImageList->LoadFromFile(GetIconPath(1));
  sAlphaImageList->LoadFromFile(GetIconPath(7));
  sAlphaImageList->LoadFromFile(GetIconPath(3));
  sAlphaImageList->LoadFromFile(GetIconPath(4));
  sAlphaImageList->LoadFromFile(GetIconPath(5));
  sAlphaImageList->LoadFromFile(GetIconPath(6));
  //Odczyt ustawien wtyczki
  aLoadSettings->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TSetStateForm::aExitExecute(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TSetStateForm::aLoadSettingsExecute(TObject *Sender)
{
  //Odczyt ustawien
  TIniFile *Ini = new TIniFile(GetPluginUserDir()+"\\\\ResourcesChanger\\\\Settings.ini");
  StateComboBox->ItemIndex = Ini->ReadInteger("State:"+ComputerName,"State",GetState());
  delete Ini;
}
//---------------------------------------------------------------------------

void __fastcall TSetStateForm::aSaveSettingsExecute(TObject *Sender)
{
  //Zapis ustawien
  TIniFile *Ini = new TIniFile(GetPluginUserDir()+"\\\\ResourcesChanger\\\\Settings.ini");
  Ini->WriteInteger("State:"+ComputerName,"State",StateComboBox->ItemIndex);
  delete Ini;
}
//---------------------------------------------------------------------------

void __fastcall TSetStateForm::DontSetStateButtonClick(TObject *Sender)
{
  //Usuniecie informacji z pliku ustawien
  TIniFile *Ini = new TIniFile(GetPluginUserDir()+"\\\\ResourcesChanger\\\\Settings.ini");
  Ini->EraseSection("State:"+ComputerName);
  delete Ini;
  //Zamkniecie formy
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TSetStateForm::StateComboBoxDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State)
{
  //Dodanie ikonek do ComboBox
  if(State.Contains(odSelected)) StateComboBox->Canvas->DrawFocusRect(Rect);
  StateComboBox->Canvas->Brush->Style = bsClear;
  sAlphaImageList->Draw(StateComboBox->Canvas,Rect.left+2,Rect.top+2,Index);
  StateComboBox->Canvas->TextOutW(Rect.left+22,Rect.top+3,StateComboBox->Items->Strings[Index]);
}
//---------------------------------------------------------------------------

void __fastcall TSetStateForm::SaveButtonClick(TObject *Sender)
{
  //Zapis ustawien
  aSaveSettings->Execute();
  //Zamkniecie formy
  Close();
}
//---------------------------------------------------------------------------
