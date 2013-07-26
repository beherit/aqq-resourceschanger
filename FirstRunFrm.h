//---------------------------------------------------------------------------
#ifndef FirstRunFrmH
#define FirstRunFrmH
//---------------------------------------------------------------------------
#include "acPNG.hpp"
#include "sBevel.hpp"
#include "sButton.hpp"
#include "sEdit.hpp"
#include "sLabel.hpp"
#include "sListView.hpp"
#include "sPanel.hpp"
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
