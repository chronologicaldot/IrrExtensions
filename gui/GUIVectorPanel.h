// (c) 2015 Nicolaus Anderson

#ifndef GUI_VECTOR_PANEL_H
#define GUI_VECTOR_PANEL_H

#include <vector3d.h>
#include <IGUIElement.h>

namespace irr {
namespace gui {

using core::vector2d;
using core::vector3d;
using core::rect;

class IGUISpinBox;

//! GUI Vector Panel
/* This class is a GUI element that contains three edit-boxes (spin boxes, at the moment) for modifying a vector.
Optionally, this class can edit a 3D vector instead of a 2D one. */
class GUIVectorPanel : public IGUIElement
{
	// Dimensions
	f32 X, Y, Z;
	bool edit3D;
	bool dirtyRect;

	IGUISpinBox* xEdit;
	IGUISpinBox* yEdit;
	IGUISpinBox* zEdit;

public:
	GUIVectorPanel( IGUIEnvironment* pEnvironment, IGUIElement* pParent, rect<s32> pRect, bool pEdit3D=false, s32 id=-1 );

	void set2DValue( f32 pX, f32 pY );
	void set2DVectorValue( vector2d<f32> pValue );
	void set3DValue( f32 pX, f32 pY, f32 pZ );
	void set3DVectorValue( vector3d<f32> pValue );
	void reset();

	/* Set editing a vector.
	DO NOT CHANGE! In the future, this may be set to allowing editing 4D vectors. */
	void setEdit2DVector(); // Force editing a 2D vector
	void setEdit3DVector(); // Force editing a 3D vector
	bool isEditing3D();

	vector2d<f32> getVector2D();
	vector3d<f32> getVector3D();

	virtual void updateAbsolutePosition(); // changes the sizes of the child elements
	virtual bool OnEvent( const SEvent& event );

	virtual const c8* getTypeName() const { return staticTypeName(); }
	static const c8* staticTypeName() { return "vectorPanel"; }

	virtual void serializeAttributes(
		irr::io::IAttributes* out,
		irr::io::SAttributeReadWriteOptions* options=0
		);

	virtual void deserializeAttributes(
		irr::io::IAttributes* in,
		irr::io::SAttributeReadWriteOptions* options=0
		);

private:
	void sendGUIEvent( EGUI_EVENT_TYPE pEventType, IGUIElement* pElement );
};

}}

#endif // #ifndef GUI_VECTOR_PANEL_H
