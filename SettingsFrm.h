//---------------------------------------------------------------------------
#ifndef SettingsFrmH
#define SettingsFrmH
#define WM_ALPHAWINDOWS (WM_USER + 666)
//---------------------------------------------------------------------------
#include "sBevel.hpp"
#include "sButton.hpp"
#include "sEdit.hpp"
#include "sLabel.hpp"
#include "sListView.hpp"
#include "sPageControl.hpp"
#include "sRadioButton.hpp"
#include "sSkinManager.hpp"
#include "sSkinProvider.hpp"
#include "sSpeedButton.hpp"
#include <System.Actions.hpp>
#include <System.Classes.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.StdCtrls.hpp>
//---------------------------------------------------------------------------
class TSettingsForm : public TForm
{
__published:	// IDE-managed Components
	TsSkinManager *sSkinManager;
	TsSkinProvider *sSkinProvider;
	TsButton *SaveButton;
	TsButton *CancelButton;
	TsButton *OKButton;
	TsBevel *Bevel;
	TActionList *ActionList;
	TAction *aExit;
	TsPageControl *PageControl;
	TsRadioButton *AdvancedModeRadioButton;
	TsRadioButton *SimpleModeRadioButton;
	TsTabSheet *ModeTabSheet;
	TsTabSheet *ResourcesListTabSheet;
	TsSpeedButton *DeleteItemSpeedButton;
	TsSpeedButton *EditSpeedButton;
	TsListView *ListView;
	TsEdit *NewComputerEdit;
	TsSpeedButton *NewItemSpeedButton;
	TsEdit *NewResourceEdit;
	TsLabel *AdvancedModeLabel;
	TAction *aResourcesListChk;
	TAction *aLoadSettings;
	TAction *aSaveSettings;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall aExitExecute(TObject *Sender);
	void __fastcall aResourcesListChkExecute(TObject *Sender);
	void __fastcall aLoadSettingsExecute(TObject *Sender);
	void __fastcall aSaveSettingsExecute(TObject *Sender);
	void __fastcall NewItemSpeedButtonClick(TObject *Sender);
	void __fastcall EditSpeedButtonClick(TObject *Sender);
	void __fastcall DeleteItemSpeedButtonClick(TObject *Sender);
	void __fastcall ListViewSelectItem(TObject *Sender, TListItem *Item, bool Selected);
	void __fastcall SaveButtonClick(TObject *Sender);
	void __fastcall OKButtonClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TSettingsForm(TComponent* Owner);
	void __fastcall WMTransparency(TMessage &Message);
	BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_ALPHAWINDOWS,TMessage,WMTransparency);
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TSettingsForm *SettingsForm;
//---------------------------------------------------------------------------
#endif
