#ifndef GITCONSOLE_H
#define GITCONSOLE_H
#include "gitui.h"
#include "bitmap_loader.h"
#include <wx/dataview.h>

class GitPlugin;
class GitConsole : public GitConsoleBase
{
    GitPlugin* m_git;
    bool       m_isVerbose;
    wxDataViewItem m_itemModified;
    wxDataViewItem m_itemUntracked;
    wxDataViewItem m_itemNew;
    wxDataViewItem m_itemDeleted;
    BitmapLoader   *m_bitmapLoader;
    BitmapLoader::BitmapMap_t m_bitmaps;
    wxBitmap m_modifiedBmp;
    wxBitmap m_untrackedBmp;
    wxBitmap m_folderBmp;
    wxBitmap m_newBmp;
    wxBitmap m_deleteBmp;
    
protected:
    void DeleteLastLine();
    
public:
    GitConsole(wxWindow* parent, GitPlugin* git);
    virtual ~GitConsole();
    void AddRawText(const wxString &text);
    void AddText(const wxString &text);
    bool IsVerbose() const;
    void UpdateTreeView(const wxString &output);
    
    //
    // Progress bar API
    //
    void ShowProgress(const wxString& message, bool pulse = false);
    void HideProgress();
    void UpdateProgress(unsigned long current, const wxString &message);
    bool IsProgressShown() const;
    void PulseProgress();
    
protected:
    virtual void OnApplyPatch(wxCommandEvent& event);
    virtual void OnFileActivated(wxDataViewEvent& event);
    virtual void OnItemSelectedUI(wxUpdateUIEvent& event);
    virtual void OnAddFile(wxCommandEvent& event);
    virtual void OnResetFile(wxCommandEvent& event);
    virtual void OnContextMenu(wxDataViewEvent& event);
    virtual void OnStopGitProcessUI(wxUpdateUIEvent& event);
    virtual void OnClearGitLogUI(wxUpdateUIEvent& event);
    virtual void OnClearGitLog(wxCommandEvent& event);
    virtual void OnStopGitProcess(wxCommandEvent& event);
    void OnOpenFile(wxCommandEvent &e);
    void OnWorkspaceClosed(wxCommandEvent &e);
    void OnConfigurationChanged(wxCommandEvent &e);
    void OnEditorThemeChanged(wxCommandEvent &e);
    
    
    void OnGitPullDropdown(wxAuiToolBarEvent& event) {
        DoOnDropdown(event, "git_pull", XRCID("git_pull"));
    }

    void DoOnDropdown(wxAuiToolBarEvent& e, const wxString& commandName, int id);
    void OnDropDownMenuEvent(wxCommandEvent& e);

};
#endif // GITCONSOLE_H
