// (c) 2015 Nicolaus Anderson

#ifndef GUI_FILE_SELECT_PANEL_H
#define GUI_MATERIAL_PANEL_H

#include <IGUIElement.h>

namespace irr {

namespace io {
	class IFileSystem;
	class IFileList;
}

namespace gui {

using core::stringw;
using core::recti;
using io::IFileSystem;
using io::IFileList;

class IGUIButton;
class IGUIListBox;
class IGUIEditBox;

enum EGUIFileSelectPanelEvent
{
	/* No action or undefined action */
	EGFSPE_NONE=0,

	/* File selected.
	This does not mean a file has been confirmed for selection.
	It only means that a file has been clicked on in the list of files.
	It DOES refer to EXISTING files. */
	EGFSPE_REAL_FILE_SELECTED,

	/* File confirmed.
	This does not mean the file exists.
	It means that the "Select" button has been clicked, or the enter
	button has been pressed, or that the user has double-clicked on a file.
	When no real file has been selected, isSelectedReal() and isSelectedFileReal()
	should return false but getSelectedFile() should return a filename.
	Furthermore, getSelectedFilePath() will return getSelectedFile() with
	the current directory appended to the front. */
	EGFSPE_FILE_CONFIRMED,

	/* Action canceled.
	This is primarily intended for windows, where the cancel action indicates
	the window should close.
	It indicates that the "Cancel" button has been pressed. */
	EGFSPE_CANCEL,

	/* The number of events */
	EGFSPE_COUNT
};

//! GUI File Select Panel
/*
This is a panel that can be embedded, allowing it to be used in windows or
as the opening screen of a program. It is used for file and directory selection,
but the user can also enter in a file name. To prevent selecting non-existent
files, the existence of the selected file can be checked by isSelectedFileReal().
To check for real selection (regardless of it is a directory or not), use
isSelectedReal(). These two functions can be used in tandem if one wishes to
allow for directory selection and not file selection via
(isSelectedReal() && !isSelectedFileReal())
*/
class GUIFileSelectPanel : public IGUIElement
{
protected:
	IFileSystem* fileSystem;
	IFileList* fileList;
	IGUIButton* selectButton;
	IGUIButton* cancelButton;
	IGUIListBox* fileListBox;
	IGUIEditBox* fileNameEditBox;
	bool isFileSelectedFromList;
	EGUIFileSelectPanelEvent lastFileSelectPanelEvent;
	bool notifyWhenEditBoxChanges;
	bool restoreDirWhenDone;
	bool restoreDirWhenCancelled;

	io::path initialWorkingDir;
	io::path currentWorkingDir;

	bool drawBack;

public:
	GUIFileSelectPanel(
			IGUIEnvironment* pEnvironment,
			IGUIElement* pParent,
			recti pRect,
			io::path pStartDirectory,
			s32 id=-1
			);

	~GUIFileSelectPanel();

	//! Get panel button
	IGUIButton* getSelectButton();
	IGUIButton* getCancelButton();

	//! On event
	/* Handles events, such as from the GUI */
	virtual bool OnEvent( const SEvent& event );

	//! Get panel event
	/* Returns the last event from the panel.
	Parent GUI elements should check this when receiving events from this element. */
	EGUIFileSelectPanelEvent getLastEvent();

protected:
	void sendGUIEvent( EGUI_EVENT_TYPE pEventType, IGUIElement* pElement=0 );

public:
	//! Notify when edit box changes
	/* Sets whether this element should attempt to notify the parent
	whenever the editbox changes. This can slow things down, but it allows for
	the parent to perhaps update some file-related information being displayed. */
	void setNotifyWhenEditBoxChanges(bool yes);

	//!
	io::path getCurrentWorkingDirectory();
	//! Is the selected file or folder real
	bool isSelectedReal();
	//! Is the selected file a real file?
	bool isSelectedFileReal();
	//! Selected file name
	io::path getSelectedFile(); // See important note below
	//! Absolute path of the selected file
	io::path getSelectedFilePath();
	//! Relative path from initial directory
	io::path getSelectedFileRelativePath();

	//! Restore the working directory to the initial directory when a file is selected
	void setRestoreDirectoryWhenDone(bool);

	//! Restore the working directory to the initial directory when cancelling
	void setRestoreDirectoryWhenCancelled(bool);

	//! Activate
	/* Restores this instance's current working directory. */
	void reactivate();

	//! Done
	/* This should be called when the element is done being used but won't be deleted.
	It restores the initial directory. */
	void deactivate();

	/* IMPORTANT NOTE:
	If ever IGUIFileOpenDialog is to be inherited (unlikely),
	existing function names should be available.
	Hence, getFileName() and getDirectory() are not overridden here,
	especially since they return const wchar_t and const io::path&
	respectively.
	*/

protected:
	//! Open selected directory
	/* Attempts to open the selected directory. */
	void openSelectedDirectory();

	//! Fill file list
	/* Fills the file list. */
	void fillFileList();

	//! Path conversion
	/* Converts a path from multibyte to widechar. */
	void pathToStringW(irr::core::stringw&, const irr::io::path&);

public:
	//! Draw
	virtual void draw();
};

}}

#endif
