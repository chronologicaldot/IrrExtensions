/*
Created by Nicolaus Anderson, 2012

Subject to the same terms as in the irrlicht engine.
See irrlicht.h for more details.
*/

#include <irrlicht.h>

#ifndef IRR_CREATE_GRADIENT_IMAGE_TEXTURE_H
#define IRR_CREATE_GRADIENT_IMAGE_TEXTURE_H

namespace irr
{
namespace video
{

//! Create Gradient Image
/* Creates an image that is a gradient of the given colors.
The locations of complete transitions from one color to the next are
given by the stops, which are percentages of the overall size.
There must be an equal number of stops as there are colors.
NOTE: You will not need to grab the image, but you will need to drop it.
*/
IImage* createGradientImage(
	IVideoDriver* videoDriver,
	core::dimension2du size,
	bool horizontal,
	SColor colors[],
	u32 colorCount,
	f32 stops[]
	);

//! Create Gradient Texture
/* Creates a texture that is a gradient of the given colors.
The locations of complete transitions from one color to the next are
given by the stops, which are percentages of the overall size.
The return is a texture created by the video driver from an image generated
by this function.
There must be an equal number of stops as there are colors.
*/
ITexture* createGradientTexture(
	IVideoDriver* videoDriver,
	core::dimension2du size,
	bool horizontal,
	SColor colors[],
	u32 colorCount,
	f32 stops[],
	core::stringw name
	);

} // end namespace video
} // end namespace irr

#endif
