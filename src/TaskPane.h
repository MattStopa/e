/*******************************************************************************
 *
 * Copyright (C) 2010, Matthew Stopa
 *
 * This software is licensed under the Open Company License as described
 * in the file license.txt, which you should have received as part of this
 * distribution. The terms are also available at http://opencompany.org/license.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ******************************************************************************/
#ifndef Task_Pane
#define Task_Pane

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "Catalyst.h"
#include <vector>
#include "Cell.h"
#include "IFrameUndoPane.h"
#include "EditorFrame.h"
;


// Pre-declarations
class EditorCtrl;
class TaskManager;

class TaskPane : public wxControl {
public:
	TaskPane(CatalystWrapper& cw, IFrameUndoPane* parentFrame, int win_id, wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
		void InsertItems(int position, const wxString* string, int repeatedTimes);
	void TaskPane::setEditorFrame(EditorFrame *ef);

	void Clear();

	//bool IsSelectionMode() const {return !m_rangeHistory.empty();};
	
	DECLARE_EVENT_TABLE();
	
	bool Show(bool show);
	void OnListDoubleClicked(wxCommandEvent& event);


private:


	void TaskPane::NewTask(wxCommandEvent& WXUNUSED(event));
	void TaskPane::DeleteTask(wxCommandEvent& WXUNUSED(event));
	void TaskPane::OpenTasks(wxCommandEvent& WXUNUSED(event));
	void TaskPane::SaveTasks(wxCommandEvent& WXUNUSED(event));

	wxListBox *myList;
	wxPanel *myPanel;
	wxFrame *myFrame;
	EditorFrame* e_frame;
	TaskManager* manager;
	bool isTaskActive;
	int activeTask;

	wxButton* newTask;
	wxButton* deleteTask;
	wxButton* loadTasks;
	wxButton* saveTasks;

}

#endif