/*
Created by Nicolaus Anderson, 2012

For license terms, see xplo.h
*/

#include "divClass.h"

void divBox::setClipped( bool clip )
{
	this->NoClip = !clip;
}

bool divBox::hidesOverflow()
{
	return ( overflow == HTM_OVF_hide );
}

void divBox::overflowShow() { overflow = HTM_OVF_show; }
void divBox::overflowHide() { overflow = HTM_OVF_hide; }
void divBox::overflowAuto() { overflow = HTM_OVF_auto; }

//void divBox::setOnPressFunction( bool NewFunction() )
//{
//	onPressFunction->set( NewFunction );
//}

bool divBox::isLeftAlignedLeft()
{
	return ( AlignLeft == irr::gui::EGUIA_UPPERLEFT );
}

bool divBox::isTopAlignedTop()
{
	return ( AlignTop == irr::gui::EGUIA_UPPERLEFT );
}

void divBox::setPosition( HTMLPosType pos_type )
{
	position_type = pos_type;
}

void divBox::setBackgroundColor( irr::video::SColor color )
{
	background_color = color;
}

void divBox::setImage( irr::io::path filename )
{
	background_image = irrdev->getVideoDriver()->getTexture( filename );
}

void divBox::draw()
{
	irr::video::IVideoDriver * vid = irrdev->getVideoDriver();

	// Draw the background color (default is transparent)
	vid->draw2DRectangle( background_color, AbsoluteRect, &AbsoluteClippingRect);

	if ( background_image != 0 )
	{

		// Box in which to draw the image	
		image_box = new irr::core::recti(
				irr::core::vector2di(0,0),
				background_image->getOriginalSize()
			);

		vid->draw2DImage(
				background_image,
				irr::core::vector2di(0,0),
				*image_box,
				&AbsoluteClippingRect,
				irr::video::SColor(0,0,0,0), true
				);
	}

	if ( ifDrawBorder )
		vid->draw2DRectangleOutline( AbsoluteRect, outline_color );

	//irr::gui::IGUIElement::draw();
	irr::core::list<irr::gui::IGUIElement*>::Iterator it = Children.begin();
	for ( ; it != Children.end(); it++ )
	{
		(*it)->draw();
	}
}

bool divBox::OnEvent( const irr::SEvent& event )
{
	if ( isClickable && isVisible() && event.EventType == irr::EET_MOUSE_INPUT_EVENT )
	{
		if ( event.MouseInput.isLeftPressed() )
		{
			//if ( onPressFunction != 0 )
			//	return onPressFunction->use();
			//else
			//	return false;
			
			irr::SEvent newEvent;
			newEvent.eventType = irr::EET_GUI_EVENT;
			nevEvent.GUIEvent.Caller = this;
			nevEvent.GUIEvent.Element = 0;
			nevEvent.GUIEvent.EventType = EGET_BUTTON_PRESSED;
			if ( Parent )
				Parent->OnEvent(event);
		}
		return false;
	}
	
	// else
	return irr::gui::IGUIElement::OnEvent(event);
}

void divBox::TransferParameters( irr::gui::IGUIElement * elem, bool dumpMe )
{
	// change the parent
	irr::gui::IGUIElement * prt = elem->getParent();
	*prt = *(this->Parent);
	Parent->addChild(elem);

	if ( dumpMe )
	{
		this->grab();
		this->Parent->remove();
		Parent = 0;
	}

	// transfer boundary
	elem->setRelativePosition( this->RelativeRect );
	//elem->AbsoluteRect = this->AbsoluteRect;
	//elem->AbsoluteClippingRect = this->AbsoluteClippingRect;

	elem->setAlignment(
		this->AlignLeft, this->AlignRight, this->AlignTop, this->AlignBottom
		);

	elem->setVisible( !NoClip );

	// transfer text contents
	elem->setText( this->getText() );
	
	if ( dumpMe )
		this->drop();
}