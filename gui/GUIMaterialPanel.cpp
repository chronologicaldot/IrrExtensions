// (c) 2014 Nicolaus Anderson

#include "GUIMaterialPanel.h"
#include <IGUIButton.h>
#include <IGUICheckBox.h>
#include <IGUIWindow.h>
#include <IGUIEditBox.h>
#include <SMaterial.h>
#include "GUIColorEditBox.h"
#include "GUIColorSample.h"
#include "GUISColorSelect.h"
#include <IMeshSceneNode.h>
#include <ISceneManager.h>
#include "scene/GUIScene.h"
#include "GUIWindow2.h"
#include "GUIFileSelectPanel.h"

namespace irr {
namespace gui {

using core::vector2di;
using core::recti;
using core::dimension2d;
using video::SMaterial;

GUIMaterialPanel::GUIMaterialPanel(
			IGUIEnvironment* pEnvironment,
			ISceneManager* pSceneManager,
			IGUIElement* pParent,
			recti pRect,
			s32 id
			)
		: IGUIElement( EGUIET_ELEMENT, pEnvironment, pParent, id, pRect )
		, material(0)

		, meshMaterialScene(0)

		, ambientColorEditBox(0)
		, ambientPanelSample(0)
		, ambientColorButton(0)
		, ambientColorSelectWindow(0)
		, ambientColorSelect(0)
		, ambientColorSample(0)

		, diffuseColorEditBox(0)
		, diffusePanelSample(0)
		, diffuseColorButton(0)
		, diffuseColorSelectWindow(0)
		, diffuseColorSelect(0)
		, diffuseColorSample(0)

		, emissiveColorEditBox(0)
		, emissivePanelSample(0)
		, emissiveColorButton(0)
		, emissiveColorSelectWindow(0)
		, emissiveColorSelect(0)
		, emissiveColorSample(0)

		, specularColorEditBox(0)
		, specularPanelSample(0)
		, specularColorButton(0)
		, specularColorSelectWindow(0)
		, specularColorSelect(0)
		, specularColorSample(0)

		, wireframeCheckbox(0)
		, gourandShadingCheckbox(0)
		, lightingCheckbox(0)
		, mipmapsCheckbox(0)
		, colorSelectWindowRect(0, 0, 210, 210)

		, textureNameEditBox(0)
		, textureSelectButton(0)
		, textureRemoveButton(0)

		, selectTextureWindow(0)
		, selectTextureFilePanel(0)
{
	// Create the user interface
	setTabGroup(true);

	/* When designing a mesh that must become transparent or fade,
	you can use this setting: */
	// EMT_TRANSPARENT_VERTEX_ALPHA
	/* This setting (which applies to the material; see EMaterialTypes.h)
	sets the transparency based on the vertex color alpha.
	It would be useful to add a checkbox that allows the user to
	select use-the-alpha-value so that the mesh can either be transparent
	or solid (for faster rendering).
	NOTE: When doing this, if you want to combine both vertex-alpha (to fade
	the whole thing) with texture alpha, you will have to write a shader. */

	s32 startY = 15; // In case the mesh scene is added

	// create GUI Mesh Scene with sphere with material
	//meshMaterialScene = new GUIScene( Environment, pSceneManager, this, recti() );
	//materialSceneNode = meshMaterialScene->GetScene()->addSphereSceneNode();
	//meshMaterialScene->SetOptimalCamDistToNode( materialSceneNode );
	//updateMeshScene();

	s32 boxW, boxH;
	boxW = 65; boxH = 20;
	recti textRect(0,startY,boxW+5,boxH+15);
	recti valueRect(textRect);
	valueRect += vector2di( boxW+15, 0 );
	vector2di shiftY(0,boxH+5);
	recti sampleRect( 0, 0, boxH, boxH );
	sampleRect += vector2di( valueRect.LowerRightCorner.X + 15, valueRect.UpperLeftCorner.Y );

	// ********* Ambient color
	ambientColorButton = Environment->addButton(
										textRect,
										this, -1,
										L"Ambient",
										L"Open ambient color selection window" );
	//ambientColorButton->setTabStop(true);
	//ambientColorButton->setTabOrder(0);

	ambientColorEditBox = new GUIColorEditBox( Environment, this, valueRect );
	ambientColorEditBox->setTabStop(true);
	ambientColorEditBox->setTabOrder(-1);

	ambientPanelSample = new GUIColorSample( pEnvironment, this, sampleRect );
	ambientPanelSample->setDrawBorder(true);
	ambientPanelSample->showOneColor( 0xffffffff );

	// ********* Diffuse color
	textRect += shiftY;
	diffuseColorButton = Environment->addButton(
										textRect,
										this, -1,
										L"Diffuse",
										L"Open diffuse color selection window" );
	//diffuseColorButton->setTabStop(true);
	//diffuseColorButton->setTabOrder(2);

	valueRect += shiftY;
	diffuseColorEditBox = new GUIColorEditBox( Environment, this, valueRect );
	diffuseColorEditBox->setTabStop(true);
	diffuseColorEditBox->setTabOrder(-1);

	sampleRect += shiftY;
	diffusePanelSample = new GUIColorSample( pEnvironment, this, sampleRect );
	diffusePanelSample->setDrawBorder(true);
	diffusePanelSample->showOneColor( 0xffffffff );

	// ********* Emissive color
	textRect += shiftY;
	emissiveColorButton = Environment->addButton(
										textRect,
										this, -1,
										L"Emissize",
										L"Open emissive color selection window" );
	//emissiveColorButton->setTabStop(true);
	//emissiveColorButton->setTabOrder(4);

	valueRect += shiftY;
	emissiveColorEditBox = new GUIColorEditBox( Environment, this, valueRect );
	emissiveColorEditBox->setTabStop(true);
	emissiveColorEditBox->setTabOrder(-1);

	sampleRect += shiftY;
	emissivePanelSample = new GUIColorSample( pEnvironment, this, sampleRect );
	emissivePanelSample->setDrawBorder(true);
	emissivePanelSample->showOneColor( 0xffffffff );

	// ********* Specular color
	textRect += shiftY;
	specularColorButton = Environment->addButton(
										textRect,
										this, -1,
										L"Specular",
										L"Open specular color selection window" );
	//specularColorButton->setTabStop(true);
	//specularColorButton->setTabOrder(6);

	valueRect += shiftY;
	specularColorEditBox = new GUIColorEditBox( Environment, this, valueRect );
	specularColorEditBox->setTabStop(true);
	specularColorEditBox->setTabOrder(-1);

	sampleRect += shiftY;
	specularPanelSample = new GUIColorSample( pEnvironment, this, sampleRect );
	specularPanelSample->setDrawBorder(true);
	specularPanelSample->showOneColor( 0xffffffff );


	// Enlargen the text box to fit the text and checkbox
	textRect.LowerRightCorner = valueRect.LowerRightCorner;

	// ********* Wireframe
	textRect += shiftY;
	wireframeCheckbox = Environment->addCheckBox(
										false, // checked?
										textRect,
										this, -1,
										L"Wireframe" );
	wireframeCheckbox->setTabStop(true);
	wireframeCheckbox->setTabOrder(-1);

	// ********* Gourand Shading
	textRect += shiftY;
	gourandShadingCheckbox = Environment->addCheckBox(
											false,
											textRect,
											this, -1,
											L"Gourand Shading" );
	gourandShadingCheckbox->setTabStop(true);
	gourandShadingCheckbox->setTabOrder(-1);

	// ********* Lighting
	textRect += shiftY;
	lightingCheckbox = Environment->addCheckBox(
										false,
										textRect,
										this, -1,
										L"Lit by Lights" );
	lightingCheckbox->setTabStop(true);
	lightingCheckbox->setTabOrder(-1);

	// ********* Mipmaps
	textRect += shiftY;
	mipmapsCheckbox = Environment->addCheckBox(
										false,
										textRect,
										this, -1,
										L"Use Mipmaps" );
	mipmapsCheckbox->setTabStop(true);
	mipmapsCheckbox->setTabOrder(-1);

	// ********* Texture
	textRect += shiftY + vector2di(0,5);
	Environment->addStaticText(L"Texture...", textRect, false, false, this );

	textRect += shiftY;
	textureNameEditBox = Environment->addEditBox( L"", textRect, true, this, -1);
	textureNameEditBox->setEnabled(false);

	recti textureButtonRect( vector2di(0,0), dimension2d<s32>(textRect.getHeight(), textRect.getHeight()) );
	textureButtonRect += vector2di( textRect.LowerRightCorner.X + 5, textRect.UpperLeftCorner.Y );
	textureSelectButton = Environment->addButton( textureButtonRect, this, -1, L"...", L"Select texture." );

	textureButtonRect += vector2di( textureButtonRect.getWidth() + 5, 0 );
	textureRemoveButton = Environment->addButton( textureButtonRect, this, -1, L"", L"Remove texture." );
	textureRemoveButton->setSpriteBank( Environment->getSkin()->getSpriteBank() );
	textureRemoveButton->setSprite(
							EGBS_BUTTON_NOT_FOCUSED,
							Environment->getSkin()->getIcon( EGDI_WINDOW_CLOSE ),
							Environment->getSkin()->getColor( EGDC_WINDOW_SYMBOL )
							);
	textureRemoveButton->setSprite(
							EGBS_BUTTON_FOCUSED,
							Environment->getSkin()->getIcon( EGDI_WINDOW_CLOSE ),
							Environment->getSkin()->getColor( EGDC_WINDOW_SYMBOL )
							);

	// ********* Mesh scene
	// create GUI Mesh Scene with sphere with material
	core::dimension2di sceneSize;
	sceneSize.Width = pRect.getWidth() * 2 / 3;
	sceneSize.Height = sceneSize.Width;
	meshMaterialScene = new GUIScene(
								Environment,
								pSceneManager,
								this,
								recti(
									vector2di(0, textRect.LowerRightCorner.Y + 15),
									sceneSize
									)
								);

	materialSceneNode = meshMaterialScene->GetScene()->addCubeSceneNode();
	meshMaterialScene->SetOptimalCamDistToNode( materialSceneNode );
	meshMaterialScene->enableCamControl(true);
	meshMaterialScene->setExtraCamDistance(5);
	meshMaterialScene->setDrawBorder(true);
}

GUIMaterialPanel::~GUIMaterialPanel()
{
	closeWindows();

	if ( ambientColorEditBox )
		ambientColorEditBox->drop();
	if ( diffuseColorEditBox )
		diffuseColorEditBox->drop();
	if ( emissiveColorEditBox )
		emissiveColorEditBox->drop();
	if ( specularColorEditBox )
		specularColorEditBox->drop();
}

void GUIMaterialPanel::closeWindows()
{
	if ( ambientColorSelectWindow )
	{
		ambientColorSelectWindow->remove();
		ambientColorSelectWindow = 0;
		ambientColorSelect = 0;
	}
	if ( diffuseColorSelectWindow )
	{
		diffuseColorSelectWindow->remove();
		diffuseColorSelectWindow = 0;
		diffuseColorSelect = 0;
	}
	if ( emissiveColorSelectWindow )
	{
		emissiveColorSelectWindow->remove();
		emissiveColorSelectWindow = 0;
		emissiveColorSelect = 0;
	}
	if ( specularColorSelectWindow )
	{
		specularColorSelectWindow->remove();
		specularColorSelectWindow = 0;
		specularColorSelect = 0;
	}
}

void GUIMaterialPanel::setMaterial( SMaterial& pMaterial )
{
	material = &pMaterial;

	// Update everything

	ambientColorEditBox->setColor( pMaterial.AmbientColor, false );
	ambientPanelSample->showOneColor( pMaterial.AmbientColor );
	diffuseColorEditBox->setColor( pMaterial.DiffuseColor, false );
	diffusePanelSample->showOneColor( pMaterial.DiffuseColor );
	emissiveColorEditBox->setColor( pMaterial.EmissiveColor, false );
	emissivePanelSample->showOneColor( pMaterial.EmissiveColor );
	specularColorEditBox->setColor( pMaterial.SpecularColor, false );
	specularPanelSample->showOneColor( pMaterial.SpecularColor );

	if ( ambientColorSelectWindow )
	{
		ambientColorSelect->setColor( pMaterial.AmbientColor, false );
		ambientColorSample->showOneColor( pMaterial.AmbientColor );
	}
	if ( diffuseColorSelectWindow )
	{
		diffuseColorSelect->setColor( pMaterial.DiffuseColor, false );
		diffuseColorSample->showOneColor( pMaterial.DiffuseColor );
	}
	if ( emissiveColorSelectWindow )
	{
		emissiveColorSelect->setColor( pMaterial.EmissiveColor, false );
		emissiveColorSample->showOneColor( pMaterial.EmissiveColor );
	}
	if ( specularColorSelectWindow )
	{
		specularColorSelect->setColor( pMaterial.SpecularColor, false );
		specularColorSample->showOneColor( pMaterial.SpecularColor );
	}

	wireframeCheckbox->setChecked( pMaterial.Wireframe );
	gourandShadingCheckbox->setChecked( pMaterial.GouraudShading );
	lightingCheckbox->setChecked( pMaterial.Lighting );
	mipmapsCheckbox->setChecked( pMaterial.UseMipMaps );

	if ( pMaterial.getTexture(0) )
	{
		textureNameEditBox->setText( stringw(pMaterial.getTexture(0)->getName().getPath()).c_str() );
	} else {
		textureNameEditBox->setText(L"");
	}

	meshMaterialScene->setDrawBackground( ! pMaterial.Wireframe );
	updateMeshScene();
}

void GUIMaterialPanel::setAmbientLight( irr::video::SColor pColor )
{
	meshMaterialScene->GetScene()->setAmbientLight( pColor );
}

bool GUIMaterialPanel::OnEvent( const SEvent& event )
{
	// Only respond to GUI events
	if ( isEnabled() && isVisible() )
	switch ( event.EventType )
	{
	case EET_GUI_EVENT:
		if ( OnGuiEvent(event) )
			return true;
		break;

	default: break;
	}

	return IGUIElement::OnEvent(event);
}

bool GUIMaterialPanel::OnGuiEvent( const SEvent& event )
{
	switch ( event.GUIEvent.EventType )
	{
	case EGET_EDITBOX_CHANGED:
		if ( event.GUIEvent.Caller == ambientColorEditBox )
		{
			if ( ambientColorSelectWindow )
			{
				ambientColorSelect->setColor( ambientColorEditBox->getColor(), false );
				ambientColorSample->showOneColor( ambientColorEditBox->getColor() );
			}
			if ( material )
				material->AmbientColor = ambientColorEditBox->getColor();
			ambientPanelSample->showOneColor( ambientColorEditBox->getColor() );

			sendGUIEvent( EGET_EDITBOX_CHANGED, ambientColorEditBox );
			return true;
		}
		else if ( event.GUIEvent.Caller == ambientColorSelect )
		{
			ambientColorEditBox->setColor( ambientColorSelect->getColor(), false );
			ambientPanelSample->showOneColor( ambientColorSelect->getColor() );
			ambientColorSample->showOneColor( ambientColorSelect->getColor() );
			if ( material )
				material->AmbientColor = ambientColorSelect->getColor();

			sendGUIEvent( EGET_EDITBOX_CHANGED, ambientColorSelect );
			return true;
		}
		else if ( event.GUIEvent.Caller == diffuseColorEditBox )
		{
			if ( diffuseColorSelectWindow )
			{
				diffuseColorSelect->setColor( diffuseColorEditBox->getColor(), false );
				diffuseColorSample->showOneColor( diffuseColorEditBox->getColor() );
			}
			if ( material )
				material->DiffuseColor = diffuseColorEditBox->getColor();
			diffusePanelSample->showOneColor( diffuseColorEditBox->getColor() );

			sendGUIEvent( EGET_EDITBOX_CHANGED, diffuseColorEditBox );
			return true;
		}
		else if ( event.GUIEvent.Caller == diffuseColorSelect )
		{
			diffuseColorEditBox->setColor( diffuseColorSelect->getColor(), false );
			diffusePanelSample->showOneColor( diffuseColorSelect->getColor() );
			diffuseColorSample->showOneColor( diffuseColorSelect->getColor() );
			if ( material )
				material->DiffuseColor = diffuseColorSelect->getColor();

			sendGUIEvent( EGET_EDITBOX_CHANGED, diffuseColorSelect );
			return true;
		}
		else if ( event.GUIEvent.Caller == emissiveColorEditBox )
		{
			if ( emissiveColorSelectWindow )
			{
				emissiveColorSelect->setColor( emissiveColorEditBox->getColor(), false );
				emissiveColorSample->showOneColor( emissiveColorEditBox->getColor() );
			}
			if ( material )
				material->EmissiveColor = emissiveColorEditBox->getColor();
			emissivePanelSample->showOneColor( emissiveColorEditBox->getColor() );

			sendGUIEvent( EGET_EDITBOX_CHANGED, emissiveColorEditBox );
			return true;
		}
		else if ( event.GUIEvent.Caller == emissiveColorSelect )
		{
			emissiveColorEditBox->setColor( emissiveColorSelect->getColor(), false );
			emissivePanelSample->showOneColor( emissiveColorSelect->getColor() );
			emissiveColorSample->showOneColor( emissiveColorSelect->getColor() );
			if ( material )
				material->EmissiveColor = emissiveColorSelect->getColor();

			sendGUIEvent( EGET_EDITBOX_CHANGED, emissiveColorSelect );
			return true;
		}
		else if ( event.GUIEvent.Caller == specularColorEditBox )
		{
			if ( specularColorSelectWindow )
			{
				specularColorSelect->setColor( specularColorEditBox->getColor(), false );
				specularColorSample->showOneColor( specularColorEditBox->getColor() );
			}
			if ( material )
				material->SpecularColor = specularColorEditBox->getColor();
			specularPanelSample->showOneColor( specularColorEditBox->getColor() );

			sendGUIEvent( EGET_EDITBOX_CHANGED, specularColorEditBox );
			return true;
		}
		else if ( event.GUIEvent.Caller == specularColorSelect )
		{
			specularColorEditBox->setColor( specularColorSelect->getColor(), false );
			specularPanelSample->showOneColor( specularColorSelect->getColor() );
			specularColorSample->showOneColor( specularColorSelect->getColor() );
			if ( material )
				material->SpecularColor = specularColorSelect->getColor();

			sendGUIEvent( EGET_EDITBOX_CHANGED, specularColorSelect );
			return true;
		}
		break; //end case EGET_EDITBOX_CHANGED

	case EGET_BUTTON_CLICKED:
		if ( event.GUIEvent.Caller == ambientColorButton )
		{
			closeWindows(); // Prevent windows from tying into eachother
			if ( !ambientColorSelectWindow )
			{
				recti r (colorSelectWindowRect);
				r.UpperLeftCorner += vector2di(5, 25);
				r.LowerRightCorner -= vector2di(5);
					// Room for the color sample
				r.LowerRightCorner.X -= 50;

				ambientColorSelectWindow = createColorSelectWindow();
				
				ambientColorSelect = new GUISColorSelect(
												Environment,
												ambientColorSelectWindow,
												r);
				ambientColorSelect->drop(); // balance reference counting

				r.UpperLeftCorner.X = r.LowerRightCorner.X + 20;
				r.LowerRightCorner.X = colorSelectWindowRect.LowerRightCorner.X - 10;
				ambientColorSample = new GUIColorSample(
												Environment,
												ambientColorSelectWindow,
												r);
				ambientColorSample->setDrawBorder(true);
				ambientColorSample->drop(); // balance reference counting
			}
			ambientColorSelect->setColor( ambientColorEditBox->getColor(), false );
			ambientColorSample->showOneColor( ambientColorEditBox->getColor() );
			Environment->getRootGUIElement()->bringToFront( ambientColorSelectWindow );
			Environment->setFocus( ambientColorSelectWindow );
			return true;
		}
		else if ( event.GUIEvent.Caller == diffuseColorButton )
		{
			closeWindows(); // Prevent windows from tying into eachother
			if ( !diffuseColorSelectWindow )
			{
				recti r (colorSelectWindowRect);
				r.UpperLeftCorner += vector2di(5, 25);
				r.LowerRightCorner -= vector2di(5);
					// Room for the color sample
				r.LowerRightCorner.X -= 50;

				diffuseColorSelectWindow = createColorSelectWindow();
				
				diffuseColorSelect = new GUISColorSelect(
												Environment,
												diffuseColorSelectWindow,
												r);
				diffuseColorSelect->drop(); // balance reference counting

				r.UpperLeftCorner.X = r.LowerRightCorner.X + 20;
				r.LowerRightCorner.X = colorSelectWindowRect.LowerRightCorner.X - 10;
				diffuseColorSample = new GUIColorSample(
												Environment,
												diffuseColorSelectWindow,
												r);
				diffuseColorSample->setDrawBorder(true);
				diffuseColorSample->drop(); // balance reference counting
			}
			diffuseColorSelect->setColor( diffuseColorEditBox->getColor(), false );
			diffuseColorSample->showOneColor( diffuseColorEditBox->getColor() );
			Environment->getRootGUIElement()->bringToFront( diffuseColorSelectWindow );
			Environment->setFocus( diffuseColorSelectWindow );
			return true;
		}
		else if ( event.GUIEvent.Caller == emissiveColorButton )
		{
			closeWindows(); // Prevent windows from tying into eachother
			if ( !emissiveColorSelectWindow )
			{
				recti r (colorSelectWindowRect);
				r.UpperLeftCorner += vector2di(5, 25);
				r.LowerRightCorner -= vector2di(5);
					// Room for the color sample
				r.LowerRightCorner.X -= 50;

				emissiveColorSelectWindow = createColorSelectWindow();
				
				emissiveColorSelect = new GUISColorSelect(
												Environment,
												emissiveColorSelectWindow,
												r);
				emissiveColorSelect->drop(); // balance reference counting

				r.UpperLeftCorner.X = r.LowerRightCorner.X + 20;
				r.LowerRightCorner.X = colorSelectWindowRect.LowerRightCorner.X - 10;
				emissiveColorSample = new GUIColorSample(
												Environment,
												emissiveColorSelectWindow,
												r);
				emissiveColorSample->setDrawBorder(true);
				emissiveColorSample->drop(); // balance reference counting
			}
			emissiveColorSelect->setColor( emissiveColorEditBox->getColor(), false );
			emissiveColorSample->showOneColor( emissiveColorEditBox->getColor() );
			Environment->getRootGUIElement()->bringToFront( emissiveColorSelectWindow );
			Environment->setFocus( emissiveColorSelectWindow );
			return true;
		}
		else if ( event.GUIEvent.Caller == specularColorButton )
		{
			closeWindows(); // Prevent windows from tying into eachother
			if ( !specularColorSelectWindow )
			{
				recti r (colorSelectWindowRect);
				r.UpperLeftCorner += vector2di(5, 25);
				r.LowerRightCorner -= vector2di(5);
					// Room for the color sample
				r.LowerRightCorner.X -= 50;

				specularColorSelectWindow = createColorSelectWindow();
				
				specularColorSelect = new GUISColorSelect(
												Environment,
												specularColorSelectWindow,
												r);
				specularColorSelect->drop(); // balance reference counting

				r.UpperLeftCorner.X = r.LowerRightCorner.X + 20;
				r.LowerRightCorner.X = colorSelectWindowRect.LowerRightCorner.X - 10;
				specularColorSample = new GUIColorSample(
												Environment,
												specularColorSelectWindow,
												r);
				specularColorSample->setDrawBorder(true);
				specularColorSample->drop(); // balance reference counting
			}
			specularColorSelect->setColor( specularColorEditBox->getColor(), false );
			specularColorSample->showOneColor( specularColorEditBox->getColor() );
			Environment->getRootGUIElement()->bringToFront( specularColorSelectWindow );
			Environment->setFocus( specularColorSelectWindow );
			return true;
		}
		else if ( event.GUIEvent.Caller == textureSelectButton )
		{
			createSelectTextureDialog();
			return true;
		}
		else if ( event.GUIEvent.Caller == textureRemoveButton )
		{
			if ( material )
			{
				material->setTexture(0,0);
			}
			updateMeshScene();
			textureNameEditBox->setText(L"");
			return true;
		}
		break; // end case EGET_BUTTON_CLICKED

	case EGET_CHECKBOX_CHANGED:
		if ( event.GUIEvent.Caller == wireframeCheckbox )
		{
			if ( material )
				material->Wireframe = wireframeCheckbox->isChecked();

			meshMaterialScene->setDrawBackground( ! wireframeCheckbox->isChecked() );

			sendGUIEvent( EGET_CHECKBOX_CHANGED, wireframeCheckbox );
			return true;
		}
		else if ( event.GUIEvent.Caller == gourandShadingCheckbox )
		{
			if ( material )
				material->GouraudShading = gourandShadingCheckbox->isChecked();

			sendGUIEvent( EGET_CHECKBOX_CHANGED, gourandShadingCheckbox );
			return true;
		}
		else if ( event.GUIEvent.Caller == lightingCheckbox )
		{
			if ( material )
				material->Lighting = lightingCheckbox->isChecked();

			sendGUIEvent( EGET_CHECKBOX_CHANGED, lightingCheckbox );
			return true;
		}
		else if ( event.GUIEvent.Caller == mipmapsCheckbox )
		{
			if ( material )
				material->UseMipMaps = mipmapsCheckbox->isChecked();

			sendGUIEvent( EGET_CHECKBOX_CHANGED, mipmapsCheckbox );
			return true;
		}
		break; // end case EGET_CHECKBOX_CHANGED

	case EGET_ELEMENT_CLOSED:
		// And we hope this doesn't cause memory leaks
		if ( event.GUIEvent.Caller == ambientColorSelectWindow )
		{
			ambientColorSelectWindow = 0;
			ambientColorSelect = 0;
		}
		else if ( event.GUIEvent.Caller == diffuseColorSelectWindow )
		{
			diffuseColorSelectWindow = 0;
			diffuseColorSelect = 0;
		}
		else if ( event.GUIEvent.Caller == emissiveColorSelectWindow )
		{
			emissiveColorSelectWindow = 0;
			emissiveColorSelect = 0;
		}
		else if ( event.GUIEvent.Caller == specularColorSelectWindow )
		{
			specularColorSelectWindow = 0;
			specularColorSelect = 0;
		}
		break; // end case EGET_ELEMENT_CLOSED

	case EGET_FILE_SELECTED:
		if ( event.GUIEvent.Caller == selectTextureFilePanel )
		{
			switch ( selectTextureFilePanel->getLastEvent() )
			{
			case EGFSPE_FILE_CONFIRMED:
				if ( material )
				{
					material->setTexture(0,
								Environment->getVideoDriver()->getTexture(
											selectTextureFilePanel->getSelectedFile()
											)
								);

					if ( material->getTexture(0) )
					{
						textureNameEditBox->setText( stringw(selectTextureFilePanel->getSelectedFile()).c_str() );
						updateMeshScene();
					}
				}
				selectTextureWindow->remove();
				selectTextureWindow = 0;
				selectTextureFilePanel = 0;
				break;

			default:
				break;
			}
		}
		break;

	case EGET_FILE_CHOOSE_DIALOG_CANCELLED:
		if ( event.GUIEvent.Caller == selectTextureFilePanel )
		{
			selectTextureWindow->remove();
			selectTextureWindow = 0;
			selectTextureFilePanel = 0;
		}
		break;

	default: break;
	}

	return false;
}

void GUIMaterialPanel::sendGUIEvent( EGUI_EVENT_TYPE pEventType, IGUIElement* pElement )
{
	updateMeshScene();

	if ( ! Parent ) return;

	SEvent event;
	event.EventType = EET_GUI_EVENT;
	event.GUIEvent.Caller = this;
	event.GUIEvent.Element = pElement;
	event.GUIEvent.EventType = pEventType;

	Parent->OnEvent(event);
}

void GUIMaterialPanel::updateMeshScene()
{
	if ( !material )
		return;

	// There should be at least one
	if ( materialSceneNode->getMaterialCount() )
	{
		materialSceneNode->getMaterial(0) = *material;
	}
}

IGUIWindow* GUIMaterialPanel::createColorSelectWindow()
{
	s32 bumpX = 5, bumpY = 5;
	recti r(colorSelectWindowRect);
	r.UpperLeftCorner -= vector2di( bumpX, bumpY );
	//r.LowerRightCorner += vector2di( 50, 0 );

	// Center in root window
	dimension2d<s32> ss = Environment->getRootGUIElement()->getAbsolutePosition().getSize() / 2;
	dimension2d<s32> rs = r.getSize() / 2;
	vector2di startPos( ss.Width - rs.Width, ss.Height - rs.Height );

	//IGUIWindow* window = Environment->addWindow( r, false, 0, this, -1 );
	GUIWindow2* window = new GUIWindow2( Environment, Environment->getRootGUIElement(), r );
	window->setEventParent( this );
	window->setNotClipped(true); // Does not allow full roaming
	window->setDraggable(true);
	window->setDrawBackground(true);
	window->setDrawTitlebar(true); // space for close button
	window->getMinimizeButton()->remove();
	window->getMaximizeButton()->remove();

	window->move( startPos );

	window->drop(); // balance reference counting

	return window;
}

void GUIMaterialPanel::createSelectTextureDialog()
{
	dimension2di windowSize(400,300);
	dimension2di rootGUISize = Environment->getRootGUIElement()->getAbsolutePosition().getSize();
	recti windowRect(
					vector2di( rootGUISize.Width - windowSize.Width,
								rootGUISize.Height - windowSize.Height )/2,
					windowSize
					);

	IGUIElement* modalScreen = Environment->addModalScreen(Environment->getRootGUIElement());

	selectTextureWindow = new GUIWindow2( Environment, 0, windowRect );
	selectTextureWindow->setEventParent(this);
	modalScreen->addChild( selectTextureWindow );
	selectTextureWindow->getMinimizeButton()->remove();

	selectTextureFilePanel = new GUIFileSelectPanel(
											Environment,
											selectTextureWindow,
											selectTextureWindow->getClientRect(),
											io::path(".")
											);

	selectTextureWindow->drop();
	selectTextureFilePanel->drop();
}

}
}
