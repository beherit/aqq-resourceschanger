//---------------------------------------------------------------------------

#ifndef NewComputerFrmH
#define NewComputerFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "sBevel.hpp"
#include "sButton.hpp"
#include "sEdit.hpp"
#include "sPanel.hpp"
#include <ExtCtrls.hpp>
#include "sSkinManager.hpp"
#include "sSkinProvider.hpp"
#include "sLabel.hpp"
#include <ActnList.hpp>
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
