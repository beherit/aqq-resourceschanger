//---------------------------------------------------------------------------
#ifndef NewComputerFrmH
#define NewComputerFrmH
//---------------------------------------------------------------------------
#include "sBevel.hpp"
#include "sButton.hpp"
#include "sEdit.hpp"
#include "sLabel.hpp"
#include "sPanel.hpp"
#include "sSkinManager.hpp"
#include "sSkinProvider.hpp"
#include <System.Actions.hpp>
#include <System.Classes.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.StdCtrls.hpp>
//---------------------------------------------------------------------------
class TNewComputerForm : public TForm
{
__published:	// IDE-managed Components
	TsPanel *Panel;
	TsEdit *NewUserResourceNameEdit;
	TsButton *CancelButton;
	TsButton *SaveButton;
	TsSkinProvider *sSkinProvider;
	TsSkinManager *sSkinManager;
	TsBevel *Bevel;
	TsLabel *NewUserResourceNameLabel;
	TActionList *ActionList;
	TAction *aExit;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall NewUserResourceNameEditChange(TObject *Sender);
	void __fastcall SaveButtonClick(TObject *Sender);
	void __fastcall aExitExecute(TObject *Sender);
private:	// User declarations
public:		// User declarations
	UnicodeString pComputerName;
	__fastcall TNewComputerForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TNewComputerForm *NewComputerForm;
//---------------------------------------------------------------------------
#endif
