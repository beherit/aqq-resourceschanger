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
#ifndef FirstRunFrmH
#define FirstRunFrmH
#define WM_ALPHAWINDOWS (WM_USER + 666)
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
	void __fastcall WMTransparency(TMessage &Message);
	BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_ALPHAWINDOWS,TMessage,WMTransparency);
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TFirstRunForm *FirstRunForm;
//---------------------------------------------------------------------------
#endif
