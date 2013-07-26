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
  sSkinProvider->BorderForm->UpdateExBordersPos(true,(int)Message.LParam);
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
	  ThemeSkinDir = StringReplace(ThemeSkinDir, "\\\\", "\\", TReplaceFlags() << rfReplaceAll);
	  sSkinManager->SkinDirectory = ThemeSkinDir;
	  sSkinManager->SkinName = "Skin.asz";
	  if(ChkThemeAnimateWindows()) sSkinManager->AnimEffects->FormShow->Time = 200;
	  else sSkinManager->AnimEffects->FormShow->Time = 0;
	  sSkinManager->Effects->AllowGlowing = ChkThemeGlowing();
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

