This folder includes files that are part of the Irrlicht Engine but have been modified by Nic Anderson for specific purposes.
Files are based on revisions 5589 and 5823. Most of the official files have remained relatively the same over the years, so you can drop these in as replacements.

The following methods have been added:

vector2D::move()
rect::set()
rect::move()
rect::delta() // and fixed it
rect::makeDelta()
IRenderTarget::getTextureCount()
IRenderTarget::getTexture(u32)
IRenderTarget::getDepthStencil()

matrix4::getInverse() now uses "T d" instead of "f32 d" and "iszero(d)" instead of "iszero(d, FLT_MIN)" so that matrices of doubles will be accurate.

quaternion::slerp() now has a call to normalize() at the end of the method (prior to the return) in order to compensate for calculation rounding errors. Problem originally reported by porcus:
http://irrlicht.sourceforge.net/forum/viewtopic.php?f=7&t=49597

I had to change IGUIElement so that it would correctly assign default values to the element in deserializeAttributes() (which is now being done in the latest Irrlicht). I copied my IGUIElement::deserializeAttributes() from my modified Irrlicht 5589, but I had to leave out the part for using the often-checked-attributes enum value check. I also changed the part for setAlignment so that it'd use the new defaultNotFound value.

I added AlignmentInfo to EGUIAlignment.h as well as the following to IGUIElement.h:
	void setAlignment( AlignmentInfo info )
	AlignmentInfo  getAlignment( u32 i )

I added IrrlichtDevice::setApplicationClass(const char*) and added an implementation to CIrrDeviceLinux.
