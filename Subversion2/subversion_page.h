#ifndef __subversion_page__
#define __subversion_page__

/**
@file
Subclass of SubversionPageBase, which is generated by wxFormBuilder.
*/

#include "subversionpagebase.h"
#include "svncommand.h"
#include "svntreedata.h"

class IManager;

/** Implementing SubversionPageBase */
class SubversionPage : public SubversionPageBase
{
	IManager *   m_manager;
	wxTreeItemId m_modifiedItems;
	SvnCommand   m_simpleCommand;

protected:
	// Handlers for SubversionPageBase events.
	void     OnChangeRootDir ( wxCommandEvent& event );
	void     OnTreeMenu      ( wxTreeEvent&    event );
	void     CreatGUIControls();
	void     BuildTree();
	void     ClearAll();
	void     DoAddNode(const wxString &title, int imgId, SvnTreeData::SvnNodeType nodeType, const wxArrayString &files);
	wxString DoGetSvnExeName();
	int      DoGetIconIndex(const wxString &filename);
protected:
	// Events
	void OnWorkspaceLoaded(wxCommandEvent &event);
	void OnWorkspaceClosed(wxCommandEvent &event);

public:
	/** Constructor */
	SubversionPage( wxWindow* parent, IManager *manager );
	void UpdateTree(const wxArrayString& modifiedFiles, const wxArrayString &conflictedFiles, const wxArrayString &unversionedFiles, const wxArrayString& newFiles, const wxArrayString& deletedFiles);
};

#endif // __subversion_page__
