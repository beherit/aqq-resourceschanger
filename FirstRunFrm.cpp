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
#include "FirstRunFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "acPNG"
#pragma link "sBevel"
#pragma link "sButton"
#pragma link "sEdit"
#pragma link "sLabel"
#pragma link "sListView"
#pragma link "sPanel"
#pragma link "sRadioButton"
#pragma link "sSkinManager"
#pragma link "sSkinProvider"
#pragma link "sSpeedButton"
#pragma resource "*.dfm"
TFirstRunForm *FirstRunForm;
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

__fastcall TFirstRunForm::TFirstRunForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::WMTransparency(TMessage &Message)
{
  Application->ProcessMessages();
  if(sSkinManager->Active) sSkinProvider->BorderForm->UpdateExBordersPos(true,(int)Message.LParam);
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::FormCreate(TObject *Sender)
{
  //Hack na blad w AC
  #if defined(_WIN64)
  ListView->SkinData->SkinSection = "";
  #endif
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

void __fastcall TFirstRunForm::FormShow(TObject *Sender)
{
  //Skorkowanie poszczegolnych komponentow
  if(sSkinManager->Active)
  {
	//Kolor WebLabel'ow
	WebWelcomeLabel->Font->Color = sSkinManager->GetGlobalFontColor();
	WebWelcomeLabel->HoverFont->Color = sSkinManager->GetGlobalFontColor();
  }
  else
  {
	//Kolor WebLabel'ow
	WebWelcomeLabel->Font->Color = clWindowText;
	WebWelcomeLabel->HoverFont->Color = clWindowText;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::aNextButtonExecute(TObject *Sender)
{
  //Pokazanie II panelu
  if(Panel1->Visible)
  {
	Panel1->Visible = false;
	Panel2->Visible = true;
	aPrevButton->Enabled = true;
	NextButton->SetFocus();
  }
  //Pokazanie III/IV panelu
  else if(Panel2->Visible)
  {
	//Pokazanie IV panelu
	if(SimpleModeRadioButton->Checked)
	{
	  Panel2->Visible = false;
	  Panel4->Visible = true;
      aNextButton->Visible = false;
	  SaveButton->Visible = true;
	  SaveButton->SetFocus();
	}
	//Pokazanie III panelu
	else if(!ComputerNameEdit->Text.IsEmpty())
	{
	  Panel2->Visible = false;
	  Panel3->Visible = true;
	  NextButton->SetFocus();
    }
  }
  //Pokazanie IV panelu
  else if(Panel3->Visible)
  {
	Panel3->Visible = false;
	Panel4->Visible = true;
	aNextButton->Visible = false;
	SaveButton->Visible = true;
	SaveButton->SetFocus();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::aPrevButtonExecute(TObject *Sender)
{
  //Pokazanie I panelu
  if(Panel2->Visible)
  {
	Panel2->Visible = false;
	Panel1->Visible = true;
	aPrevButton->Enabled = false;
	NextButton->SetFocus();
  }
  //Pokazanie II panelu
  else if(Panel3->Visible)
  {
	Panel3->Visible = false;
	Panel2->Visible = true;
	PrevButton->SetFocus();
  }
  //Pokazanie II/III panelu
  else if(Panel4->Visible)
  {
	//Pokazanie II panelu
	if(SimpleModeRadioButton->Checked)
	{
	  Panel4->Visible = false;
	  Panel2->Visible = true;
	  aNextButton->Visible = true;
	  SaveButton->Visible = false;
	  PrevButton->SetFocus();
	}
	//Pokazanie III panelu
	else
	{
      Panel4->Visible = false;
	  Panel3->Visible = true;
	  aNextButton->Visible = true;
	  SaveButton->Visible = false;
	  PrevButton->SetFocus();
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::aModeChkExecute(TObject *Sender)
{
  //Aktywacja pola do wpisywania zasobu
  ComputerNameEdit->Enabled = AdvancedModeRadioButton->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::NewItemSpeedButtonClick(TObject *Sender)
{
  //Kasowania pol szybkiej edycji
  NewComputerEdit->Text = "";
  NewResourceEdit->Text = "";
  ListView->ItemIndex = -1;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::EditSpeedButtonClick(TObject *Sender)
{
  //Edycja juz dodanych elementow
  if(ListView->ItemIndex!=-1)
  {
	ListView->Items->Item[ListView->ItemIndex]->Caption = NewComputerEdit->Text;
	ListView->Items->Item[ListView->ItemIndex]->SubItems->Strings[0] = NewResourceEdit->Text;
  }
  //Dowananie nowych elementow
  else
  {
	if((!NewComputerEdit->Text.IsEmpty())&&(!NewResourceEdit->Text.IsEmpty()))
	{
	  ListView->Items->Add();
	  ListView->Items->Item[ListView->Items->Count-1]->Caption = NewComputerEdit->Text;
	  ListView->Items->Item[ListView->Items->Count-1]->SubItems->Add(NewResourceEdit->Text);
	}
  }
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::DeleteItemSpeedButtonClick(TObject *Sender)
{
  //Usuwanie elementow
  if(ListView->ItemIndex!=-1)
   ListView->Items->Item[ListView->ItemIndex]->Delete();
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::ListViewSelectItem(TObject *Sender, TListItem *Item,
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

void __fastcall TFirstRunForm::aSaveSettingsExecute(TObject *Sender)
{
  TIniFile *Ini = new TIniFile(GetPluginUserDir()+"\\\\ResourcesChanger\\\\Settings.ini");
  //Tryb dzialania
  if(SimpleModeRadioButton->Checked)
   Ini->WriteInteger("General","Mode",1);
  else
   Ini->WriteInteger("General","Mode",2);
  //Nazwy komputerow i zasobow
  if(!ComputerNameEdit->Text.IsEmpty())
   Ini->WriteString("Resources",pComputerName,ComputerNameEdit->Text);
  if(ListView->Items->Count)
  {
	for(int ItemCount=0;ItemCount<ListView->Items->Count;ItemCount++)
	 Ini->WriteString("Resources",ListView->Items->Item[ItemCount]->Caption,ListView->Items->Item[ItemCount]->SubItems->Strings[0]);
  }
  delete Ini;
}
//---------------------------------------------------------------------------

void __fastcall TFirstRunForm::SaveButtonClick(TObject *Sender)
{
  //Zapisanie ustawien
  aSaveSettings->Execute();
  //Zamkniecie formy
  Close();
  //Odczyt ustawien w rdzeniu wtyczki
  LoadSettings();
  //Zmiana zasobow w kontach Jabber
  ChangeResources();
}
//---------------------------------------------------------------------------

