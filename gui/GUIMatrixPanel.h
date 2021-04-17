// (c) 2015 Nicolaus Anderson

#ifndef GUI_MATRIX_PANEL_H
#define GUI_MATRIX_PANEL_H

#include <vector3d.h>
#include <IGUIElement.h>
// #include <matrix4.h>
#include "../math/SpaceMatrix.h"

namespace irr {
namespace gui {

using core::vector3d;
using core::rect;
using core::matrix4;

class GUIVectorPanel;

//! Enum GUI Matrix Panel Action
/* Used for indicating the last action of the matrix.
It is also designed to save time in calculating the full space matrix (something that may never need to be done). */
enum EGUIMatrixPanelAction
{
	EGUIMTXPA_None = 0,
	EGUIMTXPA_TranslationChanged,
	EGUIMTXPA_RotationChanged,
	EGUIMTXPA_ScaleChanged,
	EGUIMTXPA_COUNT
};

//! Class GUI Matrix Panel
/* Allows for the easy editing of a matrix. */
class GUIMatrixPanel : public IGUIElement
{
protected:
	GUIVectorPanel* translationVectorPanel;
	GUIVectorPanel* rotationVectorPanel;
	GUIVectorPanel* scaleVectorPanel;
	//SpaceMatrix spaceMatrix;

private:
	EGUIMatrixPanelAction lastAction;
	bool rotInDeg;

public:
	GUIMatrixPanel( bool pRotateInDegrees, IGUIEnvironment* pEnvironment, IGUIElement* pParent, rect<s32> pRect, s32 id=-1 );

	//matrix4 getMatrix();
	void setSpaceMatrix( SpaceMatrix& pMatrix );
	void setTranslation( vector3d<f32> pTranslation );
	void setRotation( vector3d<f32> pRotation );
	void setScale( vector3d<f32> pScale );
	SpaceMatrix getSpaceMatrix();
	vector3d<f32> getTranslation();
	vector3d<f32> getRotation();
	vector3d<f32> getScale();
	void reset();

	virtual bool OnEvent( const SEvent& event );
	EGUIMatrixPanelAction getLastAction();

	// Matrix uses degrees for rotation
	/* Indicates if this panel keeps its rotation in degrees instead of radians.
	The returned matrix from getSpaceMatrix() will be converted to radians, but
	getRotation() will be returned with degrees. */
	bool rotatesInDegrees();

	virtual void serializeAttributes(
		irr::io::IAttributes* out,
		irr::io::SAttributeReadWriteOptions* options=0
		);

	virtual void deserializeAttributes(
		irr::io::IAttributes* in,
		irr::io::SAttributeReadWriteOptions* options=0
		);

	virtual const c8* getTypeName() const { return staticTypeName(); }
	static const c8* staticTypeName() { return "matrixPanel"; }

private:
	void sendGUIEvent( EGUI_EVENT_TYPE pEventType, IGUIElement* pElement );
};

}}

#endif // #ifndef GUI_MATRIX_PANEL_H
