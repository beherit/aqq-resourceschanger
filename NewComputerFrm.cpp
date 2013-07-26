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
__declspec(dllimport)bool ChkNativeEnabled();
__declspec(dllimport)void LoadSettings();
__declspec(dllimport)void ChangeResources();
//---------------------------------------------------------------------------

__fastcall TNewComputerForm::TNewComputerForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TNewComputerForm::FormShow(TObject *Sender)
{
  //Skorkowanie okna
  if(!ChkSkinEnabled())
  {
	UnicodeString ThemeSkinDir = GetThemeSkinDir();
	//Wlaczenie skorkowania
	if((FileExists(ThemeSkinDir + "\\\\Skin.asz"))&&(!ChkNativeEnabled()))
	{
	  ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
	  sSkinManager->SkinDirectory = ThemeSkinDir;
	  sSkinManager->SkinName = "Skin.asz";
	  sSkinProvider->DrawNonClientArea = false;
	  sSkinManager->Active = true;
	}
	//Wylaczenie skorkowania
	else
	 sSkinManager->Active = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TNewComputerForm::FormCreate(TObject *Sender)
{
  ///Skorkowanie okna
  if(ChkSkinEnabled())
  {
	UnicodeString ThemeSkinDir = GetThemeSkinDir();
	//Wlaczenie skorkowania
	if((FileExists(ThemeSkinDir + "\\\\Skin.asz"))&&(!ChkNativeEnabled()))
	{
	  ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
	  sSkinManager->SkinDirectory = ThemeSkinDir;
	  sSkinManager->SkinName = "Skin.asz";
	  sSkinProvider->DrawNonClientArea = true;
	  sSkinManager->Active = true;
	}
	//Wylaczenie skorkowania
	else
	 sSkinManager->Active = false;
  }
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

