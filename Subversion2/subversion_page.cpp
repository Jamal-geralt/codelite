#include "plugin.h"
#include <wx/dirdlg.h>
#include "fileextmanager.h"
#include "svnsettingsdata.h"
#include "svnstatushandler.h"
#include <wx/wupdlock.h>
#include "subversion_strings.h"
#include "subversion_page.h"
#include <wx/xrc/xmlres.h>
#include "svntreedata.h"
#include <wx/imaglist.h>
#include "imanager.h"
#include "workspace.h"
#include <wx/app.h>

SubversionPage::SubversionPage( wxWindow* parent, IManager *manager )
		: SubversionPageBase( parent )
		, m_manager(manager)
{
	CreatGUIControls();
	m_manager->GetTheApp()->Connect(wxEVT_WORKSPACE_LOADED, wxCommandEventHandler(SubversionPage::OnWorkspaceLoaded), NULL, this);
	m_manager->GetTheApp()->Connect(wxEVT_WORKSPACE_CLOSED, wxCommandEventHandler(SubversionPage::OnWorkspaceClosed), NULL, this);
}

void SubversionPage::OnChangeRootDir( wxCommandEvent& event )
{
	wxUnusedVar(event);
	wxString path(m_textCtrlRootDir->GetValue());
	wxString new_path = wxDirSelector(wxT("Select working directory:"), path, wxDD_DEFAULT_STYLE, wxDefaultPosition, this);
	if(new_path.IsEmpty() == false){
		m_textCtrlRootDir->SetValue(new_path);
		BuildTree();
	}
}

void SubversionPage::OnTreeMenu( wxTreeEvent& event )
{
	// TODO: Implement OnTreeMenu
}

void SubversionPage::CreatGUIControls()
{
	// Assign the image list
	wxImageList *imageList = new wxImageList(16, 16, true);

	imageList->Add(wxXmlResource::Get()->LoadBitmap(wxT("folder")));
	imageList->Add(wxXmlResource::Get()->LoadBitmap(wxT("error")));
	imageList->Add(wxXmlResource::Get()->LoadBitmap(wxT("edit_add")));
	imageList->Add(wxXmlResource::Get()->LoadBitmap(wxT("edit_delete")));
	imageList->Add(wxXmlResource::Get()->LoadBitmap(wxT("project_conflict")));
	imageList->Add(wxXmlResource::Get()->LoadBitmap(wxT("files_unversioned")));

	// File icons 6-13
	imageList->Add( wxXmlResource::Get()->LoadBitmap(wxT("project" ) ) ); // 6
	imageList->Add( wxXmlResource::Get()->LoadBitmap(wxT("folder" ) ) );  // 7
	imageList->Add( wxXmlResource::Get()->LoadBitmap(wxT("page_white_c" ) ) );//8
	imageList->Add( wxXmlResource::Get()->LoadBitmap(wxT("page_white_cplusplus" ) ) );//9
	imageList->Add( wxXmlResource::Get()->LoadBitmap(wxT("page_white_h" ) ) );//10
	imageList->Add( wxXmlResource::Get()->LoadBitmap(wxT("page_white_text" ) ) );//11
	imageList->Add( wxXmlResource::Get()->LoadBitmap(wxT("workspace" ) ) );//12
	imageList->Add( wxXmlResource::Get()->LoadBitmap(wxT("formbuilder" ) ) );//13

	m_treeCtrl->AssignImageList( imageList );

	if(m_manager->IsWorkspaceOpen()) {
		m_textCtrlRootDir->SetValue(m_manager->GetWorkspace()->GetWorkspaceFileName().GetPath());
	}
	BuildTree();
}

void SubversionPage::BuildTree()
{
	ClearAll();
	wxString rootDir = m_textCtrlRootDir->GetValue();

	wxTreeItemId root;
	if(rootDir.IsEmpty()) {
		root = m_treeCtrl->AddRoot(svnNO_FILES_TO_DISPLAY, 0, 0);
		return;
	} else {
		root = m_treeCtrl->AddRoot(rootDir, 0, 0);
	}

	wxString command;
	command << DoGetSvnExeName() << wxT("--xml -q --non-interactive status");
	m_simpleCommand.Execute(command, rootDir, new SvnStatusHandler(m_manager, this));
}

void SubversionPage::OnWorkspaceLoaded(wxCommandEvent& event)
{
	event.Skip();
	m_textCtrlRootDir->SetValue(m_manager->GetWorkspace()->GetWorkspaceFileName().GetPath());
	BuildTree();
}

void SubversionPage::OnWorkspaceClosed(wxCommandEvent& event)
{
	event.Skip();
	m_textCtrlRootDir->SetValue(wxT(""));
	ClearAll();
}

void SubversionPage::ClearAll()
{
	m_treeCtrl->DeleteAllItems();
}

void SubversionPage::UpdateTree(const wxArrayString& modifiedFiles, const wxArrayString& conflictedFiles, const wxArrayString& unversionedFiles, const wxArrayString& newFiles, const wxArrayString& deletedFiles)
{
	wxTreeItemId root = m_treeCtrl->GetRootItem();
	wxWindowUpdateLocker locker( m_treeCtrl );

	// TODO :: Compare original path with the current path set
	// if they different, skip this

	if(m_treeCtrl->ItemHasChildren(root)) {
		m_treeCtrl->DeleteChildren(root);
	}

	DoAddNode(svnMODIFIED_FILES,    1, SvnTreeData::SvnNodeTypeModifiedRoot,    modifiedFiles);
	DoAddNode(svnADDED_FILES,       2, SvnTreeData::SvnNodeTypeAddedRoot,       newFiles);
	DoAddNode(svnDELETED_FILES,     3, SvnTreeData::SvnNodeTypeDeletedRoot,     deletedFiles);
	DoAddNode(svnCONFLICTED_FILES,  4, SvnTreeData::SvnNodeTypeConflictRoot,    conflictedFiles);
	DoAddNode(svnUNVERSIONED_FILES, 5, SvnTreeData::SvnNodeTypeUnversionedRoot, unversionedFiles);

	if(m_treeCtrl->ItemHasChildren(root)) {
		m_treeCtrl->Expand(root);
	}
}

void SubversionPage::DoAddNode(const wxString& title, int imgId, SvnTreeData::SvnNodeType nodeType, const wxArrayString& files)
{
	wxTreeItemId root = m_treeCtrl->GetRootItem();
	wxString basePath = m_textCtrlRootDir->GetValue();

	// Add the basic four root items
	if(files.IsEmpty() == false) {

		wxTreeItemId parent = m_treeCtrl->AppendItem(root, title, imgId, imgId, new SvnTreeData(nodeType, wxT("")));

		// Add all children items
		for(size_t i=0; i<files.GetCount(); i++) {
			wxString   filename(files.Item(i));
			wxFileName fullpath(filename     );
			fullpath.Normalize(wxPATH_NORM_ALL & ~wxPATH_NORM_LONG, basePath);
			m_treeCtrl->AppendItem(parent, files.Item(i), DoGetIconIndex(filename), DoGetIconIndex(filename), new SvnTreeData(SvnTreeData::SvnNodeTypeFile, fullpath.GetFullPath().c_str()));
		}

		if( nodeType != SvnTreeData::SvnNodeTypeUnversionedRoot) {
			m_treeCtrl->Expand(parent);
		}
	}
}

wxString SubversionPage::DoGetSvnExeName()
{
	SvnSettingsData ssd;
	m_manager->GetConfigTool()->ReadObject(wxT("SvnSettingsData"), &ssd);
	wxString executeable;
	executeable << wxT("\"") << ssd.GetExecutable() << wxT("\" ");
	return executeable;
}

int SubversionPage::DoGetIconIndex(const wxString& filename)
{
	FileExtManager::Init();
	FileExtManager::FileType type = FileExtManager::GetType(filename);

	int iconIndex( 11 );

	switch ( type ) {
	case FileExtManager::TypeHeader:
		iconIndex = 10;
		break;

	case FileExtManager::TypeSourceC:
		iconIndex = 8;
		break;

	case FileExtManager::TypeSourceCpp:
		iconIndex = 9;
		break;

	case FileExtManager::TypeProject:
		iconIndex = 6;
		break;

	case FileExtManager::TypeWorkspace:
		iconIndex = 12;
		break;

	case FileExtManager::TypeFormbuilder:
		iconIndex = 13;
		break;
	default:
		iconIndex = 11;
		break;
	}
	return iconIndex;
}
