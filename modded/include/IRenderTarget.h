// Copyright (C) 2015 Patryk Nadrowski
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_RENDER_TARGET_H_INCLUDED__
#define __I_RENDER_TARGET_H_INCLUDED__

#include "IReferenceCounted.h"
#include "EDriverTypes.h"
#include "irrArray.h"

namespace irr
{
namespace video
{
	class ITexture;

	//! Interface of a Render Target.
	class IRenderTarget : public virtual IReferenceCounted
	{
	public:

		//! constructor
		IRenderTarget() : DepthStencil(0), DriverType(EDT_NULL)
		{
		}

		//! Returns an array of previously set textures.
		const core::array<ITexture*>& getTexture() const
		{
			return Texture;
		}

		//! Returns a of previously set depth / depth-stencil texture.
		ITexture* getDepthStencil() const
		{
			return DepthStencil;
		}

		//! Set multiple textures.
		/** Set multiple textures for the render target.
		\param texture Array of texture objects. These textures are used for a color outputs.
		\param depthStencil Depth or packed depth-stencil texture. This texture is used as depth
		or depth-stencil buffer. */
		virtual void setTexture(const core::array<ITexture*>& texture, ITexture* depthStencil) = 0;

		//! Set one texture.
		void setTexture(ITexture* texture, ITexture* depthStencil)
		{
			core::array<ITexture*> textureArray(1);
			textureArray.push_back(texture);

			setTexture(textureArray, depthStencil);
		}

		//! Get driver type of render target.
		E_DRIVER_TYPE getDriverType() const
		{
			return DriverType;
		}

		// Method added by Nic Anderson
		// Original idea by Entity
		//! Returns the number of textures stored here
		u32 getTextureCount()
		{
			return Texture.size();
		}

		// Method added by Nic Anderson
		// Original idea by Entity
		//! Returns a pointer to a texture stored here by index
		ITexture* getTexture( u32 index )
		{
			return Texture[index];
		}

		// Method added by Nic Anderson
		// Original idea by Entity
		// Returns a pointer to the depth stencil
		ITexture* getDepthStencil()
		{
			return DepthStencil;
		}

	protected:

		//! Textures assigned to render target.
		core::array<ITexture*> Texture;

		//! Depth or packed depth-stencil texture assigned to render target.
		ITexture* DepthStencil;

		//! Driver type of render target.
		E_DRIVER_TYPE DriverType;

	private:
		// no copying (IReferenceCounted still allows that for reasons which take some time to work around)
		IRenderTarget(const IRenderTarget&);
		IRenderTarget& operator=(const IRenderTarget&);
	};

}
}

#endif
