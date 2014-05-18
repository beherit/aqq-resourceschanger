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
#ifndef ChuckNorrisFrmH
#define ChuckNorrisFrmH
#define WM_ALPHAWINDOWS (WM_USER + 666)
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "sSkinProvider.hpp"
#include "sBevel.hpp"
#include "sButton.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include "sPageControl.hpp"
#include <Vcl.ComCtrls.hpp>
#include "sEdit.hpp"
#include "sListView.hpp"
#include "sSpeedButton.hpp"
#include <Vcl.Buttons.hpp>
#include "sPanel.hpp"
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TChuckNorrisForm : public TForm
{
__published:	// IDE-managed Components
	TsSkinProvider *sSkinProvider;
	TsBevel *Bevel;
	TsButton *CancelButton;
	TsButton *SaveButton;
	TActionList *ActionList;
	TAction *aExit;
	TsListView *ListView;
	TsSpeedButton *DeleteItemSpeedButton;
	TsSpeedButton *EditSpeedButton;
	TsEdit *NewResourceEdit;
	TsEdit *NewSSIDEdit;
	TsSpeedButton *NewItemSpeedButton;
	TPopupMenu *PopupMenu;
	TMenuItem *GetSSID;
	TAction *aLoadSettings;
	TAction *aSaveSettings;
	TPopupMenu *PopupMenu2;
	TMenuItem *ChangeStateItem;
	TMenuItem *ChangeStateInfo;
	void __fastcall aExitExecute(TObject *Sender);
	void __fastcall NewItemSpeedButtonClick(TObject *Sender);
	void __fastcall EditSpeedButtonClick(TObject *Sender);
	void __fastcall DeleteItemSpeedButtonClick(TObject *Sender);
	void __fastcall GetSSIDClick(TObject *Sender);
	void __fastcall SaveButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall aLoadSettingsExecute(TObject *Sender);
	void __fastcall aSaveSettingsExecute(TObject *Sender);
	void __fastcall ListViewSelectItem(TObject *Sender, TListItem *Item, bool Selected);
	void __fastcall PopupMenu2Popup(TObject *Sender);
	void __fastcall ChangeStateItemClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TChuckNorrisForm(TComponent* Owner);
	bool SkinManagerEnabled;
	UnicodeString ComputerName;
	void __fastcall WMTransparency(TMessage &Message);
	BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_ALPHAWINDOWS,TMessage,WMTransparency);
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TChuckNorrisForm *ChuckNorrisForm;
//---------------------------------------------------------------------------
#endif
