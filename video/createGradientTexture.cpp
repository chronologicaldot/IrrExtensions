/*
Created by Nicolaus Anderson, 2012
See createGradientTexture.h for details.
*/

#include "createGradientTexture.h"

namespace irr
{
namespace video
{

IImage* createGradientImage(
	IVideoDriver* videoDriver,
	core::dimension2du size,
	bool horizontal,
	SColor colors[],
	u32 colorCount,
	f32 stops[]
	)
{
	// We do actually need something to work with
	if ( colorCount < 1 )
		return 0;

	// First, we need a blank image to work with
	IImage* img = videoDriver->createImage( ECF_A8R8G8B8, size );
	img->fill( 0xFF000000 ); // solid black

	SColor pixel; // - the pixel we're actually going to insert into the image
	u32 x = 0; // for iteration along the axis where the color changes
	u32 y = 0; // for iteration along the axis where color does not change
	u32 stop = 0; // current stop
	f32 percent = 0.0f; // percent transition between stopping points

	/* For single-color gradients, we simply create a solid color image
	and return it. */
	if ( colorCount == 1 )
	{
		img->fill( colors[0] );
		return img;
	}

	/* Now we draw the gradient */
	for (; x < (horizontal? size.Width:size.Height); x++ )
	{
		/* First, we increment to the next color if we are passed
		where it ends. */
		if ( stops[stop] < x && stop + 1 < colorCount )
			stop++;

		// Now, we find how far through the gradient we are
		percent =
			(f32)x *
			// range conversion: actual to gradient
			(stops[stop+1] - stops[stop])
			/ (f32)(horizontal? size.Width:size.Height);

		/* Next, we creat the pixel on the basis of how far we are through the
		gradient */
		pixel = colors[stop].getInterpolated( colors[stop+1], percent );

		/* Finally, we can apply this color to the image, drawing it
		across the image.
		This might be faster if there were a method for drawing a solid
		line across an image. */
		for (; y < (horizontal? size.Height:size.Width); y++ )
		{
			if ( horizontal )
				img->setPixel( x, y, pixel );
			else
				img->setPixel( y, x, pixel );
		}
	}

	return img;
}


ITexture* createGradientTexture(
	IVideoDriver* videoDriver,
	core::dimension2du size,
	bool horizontal,
	SColor colors[],
	u32 colorCount,
	f32 stops[],
	core::stringw name
	)
{
	IImage* img =
		createGradientImage(
			videoDriver, size, horizontal, colors, colorCount, stops
			);

	// The following code is mimicked from CGUIColorSelectDialog.cpp
	bool genMip = videoDriver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	videoDriver->setTextureCreationFlag( video::ETCF_CREATE_MIP_MAPS, false);

	ITexture* tex = videoDriver->addTexture( name.c_str(), img );
	img->drop();

	videoDriver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, genMip);

	return tex;
}

} // end namespace video
} // end namespace irr
