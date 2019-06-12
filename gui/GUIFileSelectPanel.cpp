// (c) 2015 Nicolaus Anderson

#ifndef GUI_FILE_SELECT_PANEL_CPP
#define GUI_MATERIAL_PANEL_CPP

#include "GUIFileSelectPanel.h"
#include <IGUIEnvironment.h>
#include <IFileSystem.h>
#include <IFileList.h>
#include <locale.h>
//#include <os.h> // needed for locale
#include <IGUIButton.h>
#include <IGUIListBox.h>
#include <IGUIEditBox.h>

namespace irr {
namespace gui {

using core::string;
using core::vector2di;
using core::dimension2di;

GUIFileSelectPanel::GUIFileSelectPanel(
			IGUIEnvironment* pEnvironment,
			IGUIElement* pParent,
			recti pRect,
			io::path pStartDirectory,
			s32 id )
	: IGUIElement( EGUIET_ELEMENT, pEnvironment, pParent, id, pRect )
	, fileSystem(0)
	, fileList(0)
	, selectButton(0)
	, cancelButton(0)
	, fileListBox(0)
	, fileNameEditBox(0)
	, isFileSelectedFromList(false)
	, lastFileSelectPanelEvent(EGFSPE_NONE)
	, notifyWhenEditBoxChanges(false)
	, restoreDirWhenDone(true)
	, restoreDirWhenCancelled(true)
	, drawBack(true)
{
	fileSystem = Environment->getFileSystem();
	fileSystem->grab();

	recti selectButtonRect( vector2di(pRect.getWidth()-130, pRect.getHeight()-25), dimension2di(60,20) );
	selectButton = Environment->addButton( selectButtonRect, this, -1, L"Select", L"Confirm file name" );
	selectButton->setAlignment( EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT );
	selectButton->setSubElement(true);
	selectButton->grab();

	recti cancelButtonRect( vector2di(pRect.getWidth()-65, pRect.getHeight()-25), dimension2di(60,20) );
	cancelButton = Environment->addButton( cancelButtonRect, this, -1, L"Cancel" );
	cancelButton->setAlignment( EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT );
	cancelButton->setSubElement(true);
	cancelButton->grab();

	recti fileListRect( 5, 5, pRect.getWidth()-5, pRect.getHeight()-55 );
	fileListBox = Environment->addListBox( fileListRect, this, -1, true );
	fileListBox->setAlignment( EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT );
	fileListBox->setSubElement(true);
	fileListBox->grab();

	recti fileNameEditRect( 5, pRect.getHeight()-50, pRect.getWidth()-5, pRect.getHeight()-30 );
	fileNameEditBox = Environment->addEditBox( L"", fileNameEditRect, true, this, -1 );
	fileNameEditBox->setAlignment( EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT );
	fileNameEditBox->setSubElement(true);
	fileNameEditBox->grab();

	initialWorkingDir = fileSystem->getAbsolutePath( fileSystem->getWorkingDirectory() );
	if ( pStartDirectory.size() )
	{
		fileSystem->changeWorkingDirectoryTo( pStartDirectory );
		currentWorkingDir = pStartDirectory;
	} else {
		currentWorkingDir = initialWorkingDir;
	}
	fillFileList();
}

GUIFileSelectPanel::~GUIFileSelectPanel()
{
	fileSystem->changeWorkingDirectoryTo( initialWorkingDir );

	if ( selectButton )
		selectButton->drop();
	if ( cancelButton )
		cancelButton->drop();
	if ( fileListBox )
		fileListBox->drop();
	if ( fileNameEditBox )
		fileNameEditBox->drop();

	fileSystem->drop();
}

IGUIButton* GUIFileSelectPanel::getSelectButton()
{
	return selectButton;
}

IGUIButton* GUIFileSelectPanel::getCancelButton()
{
	return cancelButton;
}

bool GUIFileSelectPanel::OnEvent( const SEvent& event )
{
	if ( event.EventType != EET_GUI_EVENT )
		return false;

	/*
	How this should work:
	Typing in an edit-box filters the list to show only matches unless
	the editbox value is "", in which case, everything is shown.
	The file selected is set to the file name.

	isSelectedFileReal() indicates if the selected file is taken from the list.
	*/

	/*
	DO NOT RESTORE THE DIRECTORY AFTER SELECTION.
	It doesn't make sense to restore the directory before the file path has been obtained by the user.
	*/

	switch ( event.GUIEvent.EventType )
	{
	case EGET_BUTTON_CLICKED:
		if ( event.GUIEvent.Caller == selectButton )
		{
			lastFileSelectPanelEvent = EGFSPE_FILE_CONFIRMED;
			sendGUIEvent( EGET_FILE_SELECTED );
			if ( restoreDirWhenDone )
				fileSystem->changeWorkingDirectoryTo(initialWorkingDir);
			return true;
		}
		else if ( event.GUIEvent.Caller == cancelButton )
		{
			lastFileSelectPanelEvent = EGFSPE_CANCEL;
			sendGUIEvent( EGET_FILE_CHOOSE_DIALOG_CANCELLED );
			//if ( restoreDirWhenDone || restoreDirWhenCancelled ) // DO NOT RESTORE (see above note)
			//	fileSystem->changeWorkingDirectoryTo(initialWorkingDir);
			return true;
		}
		break;

	case EGET_EDITBOX_CHANGED:
		if ( event.GUIEvent.Caller == fileNameEditBox )
		{
			isFileSelectedFromList = false;
			fillFileList();
			if ( notifyWhenEditBoxChanges )
				sendGUIEvent( EGET_EDITBOX_CHANGED, fileNameEditBox );
			return true;
		}
		break;

	case EGET_EDITBOX_ENTER: // another means of selection
		if ( event.GUIEvent.Caller == fileNameEditBox )
		{
			// confirm file selection
			lastFileSelectPanelEvent = EGFSPE_FILE_CONFIRMED;
			//sendGUIEvent( EGET_EDITBOX_ENTER, fileNameEditBox ); // Doesn't allow file_selected filtering in parent OnEvent
			sendGUIEvent( EGET_FILE_SELECTED );
			//if ( restoreDirWhenDone ) // DO NOT RESTORE (see above note)
			//	fileSystem->changeWorkingDirectoryTo(initialWorkingDir);
			return true;
		}
		break;

	case EGET_LISTBOX_CHANGED:
		if ( event.GUIEvent.Caller == fileListBox )
		{
			isFileSelectedFromList = true;
			lastFileSelectPanelEvent = EGFSPE_REAL_FILE_SELECTED;
			//sendGUIEvent( EGET_LISTBOX_CHANGED, fileListBox ); // Doesn't allow file_selected filtering in parent OnEvent
			sendGUIEvent( EGET_FILE_SELECTED );
			return true;
		}
		break;

	case EGET_LISTBOX_SELECTED_AGAIN:
		if ( event.GUIEvent.Caller == fileListBox )
		{
			// confirm file selection
			lastFileSelectPanelEvent = EGFSPE_FILE_CONFIRMED;
			if ( isSelectedFileReal() )
			{
				//sendGUIEvent( EGET_LISTBOX_SELECTED_AGAIN, fileListBox ); // Doesn't allow file_selected filtering in parent OnEvent
				sendGUIEvent( EGET_FILE_SELECTED );
				//if ( restoreDirWhenDone ) // DO NOT RESTORE (see above note)
				//	fileSystem->changeWorkingDirectoryTo(initialWorkingDir);
				return true;
			} else {
				// Selected is real (since it's in the list), but it is a directory
				openSelectedDirectory();
			}
		}
		break;

	default: break;
	}

	return false;
}

EGUIFileSelectPanelEvent GUIFileSelectPanel::getLastEvent()
{
	return lastFileSelectPanelEvent;
}

void GUIFileSelectPanel::sendGUIEvent( EGUI_EVENT_TYPE pEventType, IGUIElement* pElement )
{
	SEvent event;
	event.EventType = EET_GUI_EVENT;
	event.GUIEvent.Caller = this;
	event.GUIEvent.Element = pElement;
	event.GUIEvent.EventType = pEventType;

	if ( Parent )
		Parent->OnEvent(event);
}

void GUIFileSelectPanel::setNotifyWhenEditBoxChanges(bool yes)
{
	notifyWhenEditBoxChanges = yes;
}

void GUIFileSelectPanel::setRestoreDirWhenDone(bool yes)
{
	restoreDirWhenDone = yes;
}

void GUIFileSelectPanel::setRestoreDirWhenCancelled(bool yes)
{
	restoreDirWhenCancelled = yes;
}

io::path GUIFileSelectPanel::getCurrentWorkingDirectory()
{
	fileSystem->flattenFilename(currentWorkingDir);
	return currentWorkingDir;
}

bool GUIFileSelectPanel::isSelectedReal()
{
	return isFileSelectedFromList;
}

bool GUIFileSelectPanel::isSelectedFileReal()
{
	// Must be selected from list and not be a directory
	if ( isFileSelectedFromList )
	{
		for ( u32 i=0; i < fileList->getFileCount(); i++ )
		{
			if ( fileList->getFileName(i) == fileListBox->getListItem( fileListBox->getSelected() ) )
			{
				return ! ( fileList->isDirectory(i) );
			}
		}
	}
	return false;
}

io::path GUIFileSelectPanel::getSelectedFile()
{
	if ( isFileSelectedFromList )
	{
		return io::path( fileListBox->getListItem( fileListBox->getSelected() ) );
	}
	io::path filename = fileNameEditBox->getText();
	fileSystem->flattenFilename( filename );
	return filename;
}

io::path GUIFileSelectPanel::getSelectedFilePath()
{
	return fileSystem->getAbsolutePath( getSelectedFile() );
}

io::path GUIFileSelectPanel::getSelectedFileRelativePath()
{
	return fileSystem->getRelativeFilename( getSelectedFile(), initialWorkingDir );
}

void GUIFileSelectPanel::reactivate()
{
	fileSystem->changeWorkingDirectoryTo( currentWorkingDir );
}

void GUIFileSelectPanel::deactivate()
{
	fileSystem->changeWorkingDirectoryTo( initialWorkingDir );
}

void GUIFileSelectPanel::openSelectedDirectory()
{
	fileSystem->changeWorkingDirectoryTo(
				currentWorkingDir + io::path("/") +
				io::path(fileListBox->getListItem( fileListBox->getSelected() ))
				);
	currentWorkingDir = fileSystem->getWorkingDirectory();

	fillFileList();
}

void GUIFileSelectPanel::fillFileList()
{
#if !defined(_IRR_WINDOWS_CE_PLATFORM_)
	setlocale(LC_ALL,"");
#endif

	if ( fileList )
		fileList->drop();
	fileList = fileSystem->createFileList();
	fileListBox->clear();
	s32 folderIcon = Environment->getSkin()->getIcon(EGDI_DIRECTORY);
	stringw filterText(fileNameEditBox->getText());
	stringw fileNameTemp;

	for ( u32 i=0; i < fileList->getFileCount(); i++ )
	{
		// Some code from CGUIFileOpenDialog.cpp by Nikolaus Gebhardt
#ifndef _IRR_WCHAR_FILESYSTEM
		const c8 *cs = (const c8 *)fileList->getFileName(i).c_str();
		wchar_t *ws = new wchar_t[strlen(cs) + 1];
		int len = mbstowcs(ws,cs,strlen(cs));
		//mbstowcs_s( ptr_num_chars_converted, destination_buffer, source_buffer, max_char_count )
		ws[len] = 0;
		fileNameTemp = ws;
		delete [] ws;
#else
		fileNameTemp = fileList->getFileName(i).c_str();
#endif

		if ( filterText.size() == 0
			|| fileNameTemp.subString(0,filterText.size()) == filterText
			)
		{
			fileListBox->addItem(
							fileNameTemp.c_str(),
							(fileList->isDirectory(i)?folderIcon:0)
							);
		}
	}
}

void GUIFileSelectPanel::draw()
{
	if ( drawBack )
	{
		Environment->getSkin()->draw3DSunkenPane(this,
									Environment->getSkin()->getColor(EGDC_3D_FACE), true, true,
									AbsoluteRect, &AbsoluteClippingRect );
	}

	IGUIElement::draw();
}

}}

#endif
