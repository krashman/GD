#ifndef CODEEDITOR_H
#define CODEEDITOR_H

//(*Headers(CodeEditor)
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/stc/stc.h>
//*)
#include <wx/ribbon/bar.h>
#include <wx/ribbon/buttonbar.h>
#include <wx/ribbon/toolbar.h>
#include <string>
#include <vector>
#include "GDL/MainEditorCommand.h"
class Game;
class Scene;

class CodeEditor: public wxPanel
{
	public:

		CodeEditor(wxWindow* parent, std::string filename, Game * game_, const MainEditorCommand & mainEditorCommand_);
		virtual ~CodeEditor();

		//(*Declarations(CodeEditor)
		wxMenuItem* MenuItem2;
		wxMenuItem* MenuItem1;
		wxMenuItem* MenuItem3;
		wxMenu contextMenu;
		wxStyledTextCtrl* textEditor;
		//*)

		std::string filename; ///< File being edited.
		Game * game; ///< Game associated with the file. Can be NULL.

		void SelectLine(size_t line);

		static void CreateRibbonPage(wxRibbonPage * page);
		void ConnectEvents();

        /**
         * Can be called by parent so as to refresh ribbon for this editor.
         */
        void ForceRefreshRibbonAndConnect();

	protected:

		//(*Identifiers(CodeEditor)
		static const long ID_CUSTOM1;
		static const long ID_MENUITEM1;
		static const long ID_MENUITEM2;
		static const long ID_MENUITEM3;
		static const long ID_MENUITEM4;
		static const long ID_MENUITEM5;
		//*)
		static const long idRibbonSave;
		static const long idRibbonCopy;
		static const long idRibbonCut;
		static const long idRibbonPaste;
		static const long idRibbonUndo;
		static const long idRibbonRedo;
		static const long idRibbonDocGDL;
		static const long idRibbonDocSFML;
		static const long idRibbonDocWxWidgets;
		static const long idRibbonDocBoost;

	private:

		//(*Handlers(CodeEditor)
		void OntextEditorRightUp(wxMouseEvent& event);
		void OnMenuCopySelected(wxCommandEvent& event);
		void OnMenuCutSelected(wxCommandEvent& event);
		void OnMenuPasteSelected(wxCommandEvent& event);
		void OnResize(wxSizeEvent& event);
		void OnMenuUndoSelected(wxCommandEvent& event);
		void OnMenuRedoSelected(wxCommandEvent& event);
		//*)
		void OnSaveSelected(wxRibbonButtonBarEvent& evt);
		void UpdateTextCtrl(wxStyledTextEvent& event);
		void OnCharAdded (wxStyledTextEvent &event);
		void OnDocGDLSelected(wxRibbonButtonBarEvent &event);
		void OnDocSFMLSelected(wxRibbonButtonBarEvent &event);
		void OnDocWxWidgetsSelected(wxRibbonButtonBarEvent &event);
		void OnDocBoostSelected(wxRibbonButtonBarEvent &event);
		void OpenLink(wxString link);

		MainEditorCommand mainEditorCommand;

        char lastCharEntered;
        long lastCharEnteredPos;

		DECLARE_EVENT_TABLE()
};

#endif
