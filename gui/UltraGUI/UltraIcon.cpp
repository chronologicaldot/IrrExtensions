/*
(c) Nicolaus Anderson
*/

#include "UltraIcon.h"

namespace irr
{
namespace gui
{

UltraIcon::UltraIcon(
		IGUIEnvironment* environment,
		IGUIElement* parent,
		irr::s32 id,
		irr::core::recti rectangle
		)
	: IGUIElement( EGUIET_ELEMENT, environment, parent, id, rectangle )
{
}

UltraIcon::~UltraIcon()
{
}

UltraIcon::shouldShow( bool setting )
{
	setVisible( setting );
}

}} // end namespace gui and irr