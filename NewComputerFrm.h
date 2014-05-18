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
#ifndef NewComputerFrmH
#define NewComputerFrmH
#define WM_ALPHAWINDOWS (WM_USER + 666)
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
	void __fastcall NewUserResourceNameEditChange(TObject *Sender);
	void __fastcall SaveButtonClick(TObject *Sender);
	void __fastcall aExitExecute(TObject *Sender);
private:	// User declarations
public:		// User declarations
	UnicodeString pComputerName;
	__fastcall TNewComputerForm(TComponent* Owner);
	void __fastcall WMTransparency(TMessage &Message);
	BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_ALPHAWINDOWS,TMessage,WMTransparency);
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TNewComputerForm *NewComputerForm;
//---------------------------------------------------------------------------
#endif
