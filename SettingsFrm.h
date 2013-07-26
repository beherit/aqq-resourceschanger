//---------------------------------------------------------------------------

#ifndef SettingsFrmH
#define SettingsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sSkinManager.hpp"
#include "sSkinProvider.hpp"
#include "sBevel.hpp"
#include "sButton.hpp"
#include <ActnList.hpp>
#include <ExtCtrls.hpp>
#include "sEdit.hpp"
#include "sLabel.hpp"
#include "sPageControl.hpp"
#include "sRadioButton.hpp"
#include <ComCtrls.hpp>
#include "sListView.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
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
};
//---------------------------------------------------------------------------
extern PACKAGE TSettingsForm *SettingsForm;
//---------------------------------------------------------------------------
#endif
