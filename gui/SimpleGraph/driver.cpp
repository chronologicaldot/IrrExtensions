/*
By Nic Anderson
Used for debugging SGraph2D, the simple implementation of Graph2D.
*/

#include <iostream>

using std::cout;

#include <irrlicht.h>

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif


#include "SGraph2D.h"

using namespace irr;
using namespace gui;


void plotsine( SGraph2D* );
void plotline( SGraph2D* );
void plotspin( SGraph2D* );
void plotspintrail( SGraph2D* );

void plotsine( SGraph2D* win )
{
	Inc<f32> dis;
	core::vector2df pt;

	dis.copy( win->getIterableXRange() );
	dis.setStep( 1.0f / 1000.0f );

	do {
		pt.X = dis.Val();
		pt.Y = 10 * sin( dis.Val() );

		win->drawOnGraph( pt, video::SColor(255,255,0,0) );
	} while ( !++dis );
}

void plotline( SGraph2D* win )
{
	Inc<f32> dis;
	core::vector2df pt;

	dis.copy( win->getIterableXRange() );
	dis.setStep( 1.0f / 1000.0f );

	do {
		pt.X = dis.Val();
		pt.Y = dis.Val();

		win->drawOnGraph( pt, video::SColor(255,255,0,0) );
	} while ( !++dis );
}

void plotspin( SGraph2D* win )
{
	Inc<f32> twirl( CYC_DEFAULT, 0.0f, 0.1f, 0.0f, 360.0f );

	do {
		win->drawOnGraphPolar(
				5*cos( 5*twirl.Val() ), // alternating radius
				twirl.Val() / core::DEGTORAD,
				video::SColor(
					255,
					0,//(((s32)twirl.Val()*5)%255),
					(((s32)twirl.Val()*5+100)%255),
					(((s32)twirl.Val()*5+200)%255)
					)
				);
	} while ( !++twirl );
}

void plotspintrail( SGraph2D* win )
{
	static s32 loops=6; // # loops per flower
	static f32 sizeup;
	static f32 spacing = 1.2f;
	static f32 shiftup = 2;
	static f32 resolution = 200;

	Inc<f32> twirl( CYC_DEFAULT, 0.0f, 1.0f, 0.0f, (f32)loops );
	Inc<f32> innertwirl( CYC_DEFAULT, 0.0f, 1.0f, 0.0f, 360.0f );
	core::vector2df pt;

	do {
		innertwirl.setStep( 360.0f / (twirl.Val() + 1) / resolution );
		
		do {
			sizeup = twirl.Val() + shiftup;

			// Create point in polar coordinates
				// radius
			pt.X = sizeup // increasing size
					// alternating radius
					* cos( loops * innertwirl.Val() * core::DEGTORAD );

			// Convert to cartessian
			pt.Y = pt.X * sin( innertwirl.Val() * core::DEGTORAD );
			pt.X = pt.X * cos( innertwirl.Val() * core::DEGTORAD );

			// Offset
			pt.X += //(loops - sizeup + shiftup)
				spacing * sizeup * cos( twirl.Val() * 360 / loops * core::DEGTORAD );
			pt.Y += //(loops - sizeup + shiftup)
				spacing * sizeup * sin( twirl.Val() * 360 / loops * core::DEGTORAD );

			// Draw
			win->drawOnGraph(
				pt,
				video::SColor(
						255,
						0,//(((s32)twirl.Val()*360*5)%255),
						(((s32)twirl.Val()*360*5+100)%255),
						(((s32)twirl.Val()*360*5+200)%255)
						)
				);

		} while ( !++innertwirl );

		innertwirl.restart();

	} while ( !++twirl );

}



void main()
{
	IrrlichtDevice* device = createDevice(
		video::EDT_BURNINGSVIDEO,
		irr::core::dimension2du( 1000,700 )
		);
	video::IVideoDriver* vid = device->getVideoDriver();
	IGUIEnvironment* envir = device->getGUIEnvironment();

	SGraph2D* graph = new SGraph2D(
		envir,											// environment
		envir->getRootGUIElement(),						// parent
		0,												// id
		irr::core::recti(50,50,800,600),				// element boundaries
		irr::core::rectf(-10.0f,-15.0f,15.0f,10.0f),	// graph window
		true,											// marks?
		true											// ticks?
		);

	graph->setBackgroundColor( video::SColor(255,200,200,200) );
	graph->usePolygons( 2, 4 );
	graph->setPointClipping( false );

	graph->markerSpacingX( 1.5 );

	//plotsine( graph );
	//plotline( graph );
	//plotspin( graph );
	plotspintrail( graph );

	graph->autoAdjust();

	graph->setGraphScale( irr::core::rectf( 1.1f, 1.1f, 1.1f, 1.2f ) );

	graph->drop();
	graph = 0;

	// Change the font
	//envir->getSkin()->getFont()->setKerningHeight(14);
	//envir->getSkin()->getFont()->setKerningWidth(14);

	while ( device->run() )
	{
		vid->beginScene();
		envir->drawAll();
		vid->endScene();
	}

	device->drop();
}
