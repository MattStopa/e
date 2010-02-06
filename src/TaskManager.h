/*******************************************************************************
 *
 * Copyright (C) 2010, Matthew Stopa
 *
 ******************************************************************************/

#ifndef Task_Manager
#define Task_Manager

#include <algorithm>
#include "EditorCtrl.h"
#include <vector>
#include <iostream>
;

using std::vector;

class TaskPane;

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
;

class TaskManager {

public:
	TaskManager();

	void SetTask(wxString *taskData, int position, int openFiles);
	void OutputAllFiles(TaskPane* p);
	void DeleteTask(int number);
	void TaskManager::ClearTasks();
	
	wxString* GetTask(int position);
	int GetNumberOfFiles(int position);


private:
	int taskIndex;
	vector<wxString *> taskList;
}

#endif
;
static int openDocs[100];
static wxString fileNames[100][25];