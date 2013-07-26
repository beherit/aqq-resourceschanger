//---------------------------------------------------------------------------

#ifndef FirstRunFrmH
#define FirstRunFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sBevel.hpp"
#include "sSkinManager.hpp"
#include "sSkinProvider.hpp"
#include <ExtCtrls.hpp>
#include "sPanel.hpp"
#include "sButton.hpp"
#include <ActnList.hpp>
#include "acPNG.hpp"
#include "sLabel.hpp"
#include "sEdit.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include "sRadioButton.hpp"
#include "sCheckBox.hpp"
#include "sListView.hpp"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFirstRunForm : public TForm
{
__published:	// IDE-managed Components
	TsBevel *Bevel;
	TsSkinProvider *sSkinProvider;
	TsSkinManager *sSkinManager;
	TsPanel *Panel1;
	TsButton *NextButton;
	TsButton *PrevButton;
	TsButton *SaveButton;
	TActionList *ActionList;
	TAction *aSaveSettings;
	TAction *aNextButton;
	TAction *aPrevButton;
	TsLabel *WelcomeLabel1;
	TImage *WelcomeImage;
	TsLabel *WelcomeLabel2;
	TsLabel *WelcomeLabel3;
	TsWebLabel *WebWelcomeLabel;
	TsPanel *Panel2;
	TsEdit *ComputerNameEdit;
	TsLabel *ModeLabel;
	TsRadioButton *SimpleModeRadioButton;
	TsRadioButton *AdvancedModeRadioButton;
	TAction *aModeChk;
	TsPanel *Panel3;
	TsLabel *FillListViewLabel;
	TsListView *ListView;
	TsEdit *NewComputerEdit;
	TsEdit *NewResourceEdit;
	TsSpeedButton *DeleteItemSpeedButton;
	TsSpeedButton *EditSpeedButton;
	TsSpeedButton *NewItemSpeedButton;
	TsPanel *Panel4;
	TImage *EndImage;
	TsLabel *EndLabel1;
	TsLabel *EndLabel2;
	TsLabel *EndLabel3;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall aNextButtonExecute(TObject *Sender);
	void __fastcall aModeChkExecute(TObject *Sender);
	void __fastcall aPrevButtonExecute(TObject *Sender);
	void __fastcall NewItemSpeedButtonClick(TObject *Sender);
	void __fastcall EditSpeedButtonClick(TObject *Sender);
	void __fastcall DeleteItemSpeedButtonClick(TObject *Sender);
	void __fastcall ListViewSelectItem(TObject *Sender, TListItem *Item, bool Selected);
	void __fastcall aSaveSettingsExecute(TObject *Sender);
	void __fastcall SaveButtonClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
	UnicodeString pComputerName;
	__fastcall TFirstRunForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFirstRunForm *FirstRunForm;
//---------------------------------------------------------------------------
#endif
