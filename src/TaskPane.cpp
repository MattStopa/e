#include <algorithm>
#include <iostream>
#include <fstream>
#include "TaskPane.h"
#include "EditorCtrl.h"
#include "TaskManager.h"
;

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
;


enum TaskPane_Ids {
	ID_NEW_BTN,
	ID_DELETE_BTN,
	ID_OPEN_TASKS_BTN,
	ID_SAVE_TASKS_BTN
};

enum TaskPane_IDs {
	CTRL_LISTBOX = 1
};

BEGIN_EVENT_TABLE(TaskPane, wxControl)
	EVT_LISTBOX_DCLICK(wxID_ANY, TaskPane::OnListDoubleClicked)
	EVT_BUTTON(ID_NEW_BTN, TaskPane::NewTask)
	EVT_BUTTON(ID_DELETE_BTN, TaskPane::DeleteTask)
	EVT_BUTTON(ID_OPEN_TASKS_BTN, TaskPane::OpenTasks)
	EVT_BUTTON(ID_SAVE_TASKS_BTN, TaskPane::SaveTasks)
END_EVENT_TABLE()

TaskPane::TaskPane(CatalystWrapper& cw, IFrameUndoPane* parentFrame, int win_id, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size):
wxControl(parent, id, pos, size, wxNO_BORDER|wxWANTS_CHARS|wxCLIP_CHILDREN|wxNO_FULL_REPAINT_ON_RESIZE)
{
	 wxString choices[] =
    {
        _T("This"),
        _T("is one of my"),
        _T("really"),
        _T("wonderful"),
        _T("examples.")
    };


	myPanel = new wxPanel(this, 140, pos, wxSize(500,500));
	myPanel->SetAutoLayout(true);

	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	myPanel->SetSizer(buttonSizer);
	myList = new wxListBox(myPanel, 130, wxPoint(pos.x, pos.y+28), wxSize(300, 800), 0, choices, wxLB_SINGLE);
	
	wxButton* newTask = new wxButton(myPanel,ID_NEW_BTN, _("+"), pos, wxSize(25,25));
	wxButton* deleteTask = new wxButton(myPanel,ID_DELETE_BTN, _("-"), wxPoint(pos.x + 25, pos.y), wxSize(25,25));
	wxButton* loadTasks = new wxButton(myPanel,ID_OPEN_TASKS_BTN, _("Load"), wxPoint(pos.x + 50, pos.y), wxSize(50,25));
	wxButton* saveTasks = new wxButton(myPanel,ID_SAVE_TASKS_BTN, _("Save"), wxPoint(pos.x + 100, pos.y),wxSize(50,25));

	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

	manager = new TaskManager();
	isTaskActive = false;
	activeTask = -1;
									

}

void TaskPane::setEditorFrame(EditorFrame *ef) {
	e_frame = ef;
}

void TaskPane::NewTask(wxCommandEvent& WXUNUSED(event)){
	wxTextEntryDialog *entry = new wxTextEntryDialog(this, _T("Please enter a name for the task"), _T("New Task")); 
	int returnVal = entry->ShowModal();
	if (returnVal == wxID_OK) {
		wxString *ptr = &entry->GetValue();
	
		myList->InsertItems(1, ptr, myList->GetCount());
	}

}

void TaskPane::DeleteTask(wxCommandEvent& WXUNUSED(event)) {

	if (myList->GetSelection() == -1) {
		wxMessageBox(_T("You haven't selected anything"));
	} else {
		manager->DeleteTask(myList->GetSelection());
		myList->Delete(myList->GetSelection());

	}
}

void TaskPane::OpenTasks(wxCommandEvent& WXUNUSED( event)) {
	
	wxFileDialog dlg(this, _("Open Your Tasks"), wxEmptyString, wxEmptyString, _("Task File (*.tsk)|*.tsk"), wxFD_OPEN);
	if (dlg.ShowModal() != wxID_OK) return;
	const wxString path = dlg.GetPath();
	if (path.empty()) return;
	
	string line;
	ifstream myfile(path.mb_str());
	myList->Clear();

	int x = 0;
	
	if (myfile.is_open())
	{
		while (! myfile.eof() )
		{
			
			getline (myfile,line);
			wxString *aString = new wxString(line.c_str(), wxConvUTF8);
			myList->InsertItems(1, aString, myList->GetCount());

			wxString strArray[25];
			for (int i = 0; i < 25; i++) 
			{
				line = "";
				getline (myfile,line);
				wxString *aString = new wxString(line.c_str(), wxConvUTF8);
				strArray[i] = *aString;
			}
			wxString *ptr = strArray;
			manager->SetTask(ptr, x, 25);
			x++;

		}
		myfile.close();
	}

	else cout << "Unable to open file"; 

}

void TaskPane::SaveTasks(wxCommandEvent& WXUNUSED(event)) {

	wxFileDialog dlg(this, _("Save Your Tasks"), wxEmptyString, wxEmptyString, _("Task File (*.tsk)|*.tsk"), wxFD_SAVE);
	if (dlg.ShowModal() != wxID_OK) return;
	const wxString path = dlg.GetPath();
	if (path.empty()) return;
	
	string line;
	
	ofstream myfile;
	myfile.open (path.mb_str());

	int xx = myList->GetCount();
	for (int i = 0; i < myList->GetCount(); i++) 
	{
		myfile << myList->GetString(i).mb_str(wxConvUTF8) << "\n";
		wxString* text = manager->GetTask(i);
		for (int j = 0; j < 25; j++) 
		{
			wxString aLine = *text[j];
			myfile << text[j].mb_str(wxConvUTF8) << "\n";
		}
		int xxaaa = 0;
	}
	myfile.close();
	
}

void TaskPane::OnListDoubleClicked(wxCommandEvent& event){
	
	wxString *aNewString = NULL;
	aNewString = e_frame->GetAllOpenFiles();
	int si = e_frame->GetOpenFilesCount();
	//for (int i = 0; i < si; i ++) {
	//	wxMessageBox(aNewString[i]);
	//}

	if (isTaskActive) {

		/**********************************************
		 * Remove the "*" to show that there is no task open
		 **********************************************/
		
		wxString f = myList->GetString(activeTask);
		f = f.substr(2, f.length());
		myList->SetString(activeTask, f);

		/**********************************************
		 * Save files and close tab
		 **********************************************/

		int size = e_frame->GetOpenFilesCount();
		manager->SetTask(aNewString, activeTask, size);
		
		e_frame->CloseAllTabs();
		isTaskActive=false;
		activeTask = -1;

	} else {
		
		int size = manager->GetNumberOfFiles(event.GetInt());

		/**********************************************
		 * Insert a "*" to show that there is an open task
		 **********************************************/
		
		wxString f = myList->GetStringSelection();
		f.Prepend(_T("* "));
		myList->SetString(event.GetInt(), f);

		/**********************************************
		 * Open the files
		 **********************************************/

		wxString* text = manager->GetTask(event.GetInt());
		e_frame->OpenFileList(text, size);

		/*for (int i = 1; i < size; i ++) {
			wxString ffaa = text[i];
			wxMessageBox(text[i]);
		} */

		
				
		isTaskActive=true;
		activeTask = event.GetInt();
	}

	



	/*wxString aNewOne[] =
	{
		_T("Success")
	};
	
	myList->InsertItems(1, aNewOne, 1);

	e_frame->AddTab();
	e_frame->CloseAllTabs();
	wxString thePath = _T("c:\\Decaf.main.html");
	wxString thePath2 = _T("c:\\blah.java");
	e_frame->OpenFile(thePath);
	e_frame->OpenFile(thePath2);

	wxString *aNewString;
	aNewString = e_frame->GetAllOpenFiles();
	e_frame->SetTitle(*aNewString);
	myList->InsertItems(1, aNewString, 1);

	TaskManager *manager = new TaskManager();
	manager->AddTask(aNewString);
	*/
	

}

void TaskPane::InsertItems(int position, const wxString *string, int repeatedTimes) {
	myList->InsertItems(position, string, repeatedTimes);
}

void TaskPane::Clear() 
{ 


}
// 
bool TaskPane::Show(bool show) {
	bool result = wxControl::Show(show);

	return result;
}

