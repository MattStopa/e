/*******************************************************************************
 *
 * Copyright (C) 2010, Matthew Stopa
 *
 ******************************************************************************/


#include <vector>
#include <algorithm>
#include "EditorCtrl.h"
#include "TaskManager.h"
#include "TaskPane.h"

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
;

using std::vector;

TaskManager::TaskManager() {
	wxString x = _T("EMPTY");
	//wxString *ptr = &x;
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j<25; j++) {
			fileNames[i][j] = x;
		}
	}
	
	taskList.resize(100);
}

void TaskManager::DeleteTask(int number) {
	if (number == -1) {
		wxMessageBox(_T("ERROR Number is -1"));
	}

	/**********************************************
	 * This moves up all the file count if something 
	 * was deleted
	 **********************************************/

	for (int i = number; i < 99; i++) {
		openDocs[i] = openDocs[i+1];
	}
	

	/**********************************************
	 * This moves up all the files in the array if 
	 * something was deleted
	 **********************************************/



	for (int i = number; i < 99; i++) {
		
		for (int j = 0; j<25; j++) { 
			/* If the strings are not empty then copy them */
			wxString blank = _T("");
			blank << i;
			blank = _T("");
			
			if (blank.Cmp(fileNames[i+1][j])){
				fileNames[i][j] = fileNames[i+1][j];
			}
		}
					
	}

	int x = 0;

	// move all items in array up 1 level

}

void TaskManager::SetTask(wxString *taskData, int position, int openFiles) {
	
	openDocs[position] = openFiles;
	
	/**********************************************
	 * This resets all the variables into blanks
	 **********************************************/

	wxString var = _T("");
	wxString *temp = &var;
	for (int i = 0; i < 25; i++) {
		fileNames[position][i] = temp[0];
	}

	/**********************************************
	 * This assigns the files
	 **********************************************/

	//taskList.at(position) = taskData;

	for (int i = 0; i < openFiles; i++) {
		fileNames[position][i] = taskData[i];
	}
}

wxString* TaskManager::GetTask(int position) {

	/**********************************************
	 * Return the task listed in a certain position
	 **********************************************/

	wxString val;
	val << position;
	return fileNames[position];
	//return taskList.at(position);
}

int TaskManager::GetNumberOfFiles(int position) {

	/**********************************************
	 * Return the number of files in a position
	 **********************************************/

	return openDocs[position];
}

void TaskManager::ClearTasks() {
	wxString x = _T("EMPTY");
	//wxString *ptr = &x;
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j<25; j++) {
			fileNames[i][j] = x;
		}
	}
}



void TaskManager::OutputAllFiles(TaskPane* p) {
	wxString *xx = taskList.at(0);
	p->InsertItems(1, xx, 1);
}

