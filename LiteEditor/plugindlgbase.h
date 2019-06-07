//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: plugindlgbase.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _CODELITE_LITEEDITOR_PLUGINDLGBASE_BASE_CLASSES_H
#define _CODELITE_LITEEDITOR_PLUGINDLGBASE_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include "clThemedSplitterWindow.h"
#include <wx/panel.h>
#include <wx/dataview.h>
#include "clThemedListCtrl.h"
#include <wx/richtext/richtextctrl.h>
#include <wx/button.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

class PluginMgrDlgBase : public wxDialog
{
protected:
    clThemedSplitterWindow* m_splitter36;
    wxPanel* m_splitterPage40;
    clThemedListCtrl* m_dvListCtrl;
    wxPanel* m_splitterPage44;
    wxRichTextCtrl* m_richTextCtrl;
    wxButton* m_button20;
    wxButton* m_button22;
    wxStdDialogButtonSizer* m_stdBtnSizer26;
    wxButton* m_buttonOK;
    wxButton* m_button30;

protected:
    virtual void OnItemSelected(wxDataViewEvent& event) { event.Skip(); }
    virtual void OnCheckAll(wxCommandEvent& event) { event.Skip(); }
    virtual void OnCheckAllUI(wxUpdateUIEvent& event) { event.Skip(); }
    virtual void OnUncheckAll(wxCommandEvent& event) { event.Skip(); }
    virtual void OnUncheckAllUI(wxUpdateUIEvent& event) { event.Skip(); }
    virtual void OnButtonOK(wxCommandEvent& event) { event.Skip(); }

public:
    clThemedListCtrl* GetDvListCtrl() { return m_dvListCtrl; }
    wxPanel* GetSplitterPage40() { return m_splitterPage40; }
    wxRichTextCtrl* GetRichTextCtrl() { return m_richTextCtrl; }
    wxPanel* GetSplitterPage44() { return m_splitterPage44; }
    clThemedSplitterWindow* GetSplitter36() { return m_splitter36; }
    wxButton* GetButton20() { return m_button20; }
    wxButton* GetButton22() { return m_button22; }
    PluginMgrDlgBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Manage Plugins"),
                     const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(600, 400),
                     long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    virtual ~PluginMgrDlgBase();
};

#endif
