// (C) 2020 Nicolaus Anderson

#ifndef IRR_SLICK_GUI_SKIN
#define IRR_SLICK_GUI_SKIN

#include <IGUISkin.h>
#include <irrString.h>

namespace irr {

namespace video {
	class IVideoDriver;
}

namespace gui {

class IGUISpriteBank;
class IGUIEnvironment;


class SlickGUISkin : public IGUISkin
{
	bool DrawMoreRound;
	IGUIEnvironment* Environment;
	irr::video::IVideoDriver* VideoDriver;
	video::SColor Colors[EGDC_COUNT];
	video::SColor SpareBackgroundColor;
	video::SColor SpareBackgroundActiveColor;
	s32 Sizes[EGDS_COUNT];
	u32 Icons[EGDI_COUNT];
	IGUIFont* Fonts[EGDF_COUNT];
	IGUISpriteBank* Sprites;
	core::stringw Texts[EGDT_COUNT];

public:
	/* startIconsFromZero is for when you create your own sprite bank that only holds sprites
	for this skin rather than using Irrlicht's default sprite bank. */
	SlickGUISkin( IGUIEnvironment*, bool startIconsFromZero, bool drawMoreRound );
	~SlickGUISkin();

	virtual video::SColor getColor(EGUI_DEFAULT_COLOR color) const override;

	virtual void setColor(EGUI_DEFAULT_COLOR which, video::SColor newColor) override;

	virtual s32 getSize(EGUI_DEFAULT_SIZE size) const override;

	virtual void setSize(EGUI_DEFAULT_SIZE which, s32 size) override;

	virtual const wchar_t* getDefaultText(EGUI_DEFAULT_TEXT which) const override;

	virtual void setDefaultText(EGUI_DEFAULT_TEXT which, const wchar_t* newText) override;

	virtual IGUIFont* getFont(EGUI_DEFAULT_FONT which=EGDF_DEFAULT) const override;

	virtual void setFont(IGUIFont* font, EGUI_DEFAULT_FONT which=EGDF_DEFAULT) override;

	virtual IGUISpriteBank* getSpriteBank() const override;

	virtual void setSpriteBank(IGUISpriteBank* bank) override;

	virtual u32 getIcon(EGUI_DEFAULT_ICON icon) const override;

	virtual void setIcon(EGUI_DEFAULT_ICON icon, u32 index) override;

	virtual void draw3DButtonPaneStandard(IGUIElement* element,
		const core::rect<s32>& rect,
		const core::rect<s32>* clip=0) override;

	virtual void draw3DButtonPanePressed(IGUIElement* element,
		const core::rect<s32>& rect,
		const core::rect<s32>* clip=0) override;

	virtual void draw3DSunkenPane(IGUIElement* element,
		video::SColor bgcolor, bool flat, bool fillBackGround,
		const core::rect<s32>& rect,
		const core::rect<s32>* clip=0) override;

	virtual core::rect<s32> draw3DWindowBackground(IGUIElement* element,
		bool drawTitleBar, video::SColor titleBarColor,
		const core::rect<s32>& rect,
		const core::rect<s32>* clip=0,
		core::rect<s32>* checkClientArea=0) override;

	virtual void draw3DMenuPane(IGUIElement* element,
		const core::rect<s32>& rect,
		const core::rect<s32>* clip=0) override;

	virtual void draw3DToolBar(IGUIElement* element,
		const core::rect<s32>& rect,
		const core::rect<s32>* clip=0) override;

	virtual void draw3DTabButton(IGUIElement* element, bool active,
		const core::rect<s32>& rect, const core::rect<s32>* clip=0, gui::EGUI_ALIGNMENT alignment=EGUIA_UPPERLEFT) override;

	virtual void draw3DTabBody(IGUIElement* element, bool border, bool background,
		const core::rect<s32>& rect, const core::rect<s32>* clip=0, s32 tabHeight=-1, gui::EGUI_ALIGNMENT alignment=EGUIA_UPPERLEFT ) override;

	virtual void drawIcon(IGUIElement* element, EGUI_DEFAULT_ICON icon,
		const core::position2di position, u32 starttime=0, u32 currenttime=0,
		bool loop=false, const core::rect<s32>* clip=0) override;

	virtual void draw2DRectangle(IGUIElement* element, const video::SColor &color,
		const core::rect<s32>& pos, const core::rect<s32>* clip = 0) override;

	//! get the type of this skin
	virtual EGUI_SKIN_TYPE getType() const { return EGST_UNKNOWN; }

	//! Draw a pixel clipped
	void drawPixel( u32 x, u32 y, const video::SColor&, const core::rect<s32>* clip=0 );

	//! Draw a line clipped
	void draw2DVerticalLine( core::vector2d<s32> start, u32 length,
			const video::SColor&, const core::rect<s32>* clip=0 );

	void draw2DHorizontalLine( core::vector2d<s32> start, u32 length,
			const video::SColor&, const core::rect<s32>* clip=0 );

	//! Draw a rectangle outline with rounded edges
	void drawRoundOutline(
		const core::rect<s32>& rect, const core::rect<s32>* clip,
		const video::SColor& colorLeft, const video::SColor& colorRight,
		const video::SColor& colorTop, const video::SColor& colorBottom
	);

	//! Draw a rectangle outline
	/* NOTE: IVideoDriver has alternative ways of drawing 2D rectangles */
	//void drawSquareOutline(
	//	const core::rect<s32>& rect, const core::rect<s32>* clip,
	//	const video::SColor& colorLeft, const video::SColor& colorRight,
	//	const video::SColor& colorTop, const video::SColor& colorBottom
	//);

	//! Draw a rectangle outline
	/* NOTE: IVideoDriver has alternative ways of drawing 2D rectangles */
	void drawSquareOutline(
		const core::rect<s32>& rect, const core::rect<s32>* clip,
		const video::SColor& color
	);
};

} // namespace gui
} // namespace irr

#endif // IRR_SLICK_GUI_SKIN
