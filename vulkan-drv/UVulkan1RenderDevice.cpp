#include "UVulkan1RenderDevice.h"
#include "resource.h"

//UObject glue
IMPLEMENT_PACKAGE(Vulkan1Drv);
IMPLEMENT_CLASS(UVulkan1RenderDevice);


UVulkan1RenderDevice::UVulkan1RenderDevice()
{
	// Do not remove this constructor because without it, the game crashes when switching to fullscreen mode. Using C++11 "ctor() = default" does not help either.
	// Because in this case the game will call GetClass() that will return nullptr for some unknown reason (the field UObject::Class is not initialized?)
	// Probably a bug in the Microsoft (R) C/C++ Optimizing Compiler Version 19.00.24215.1 for x86 (Visual Studio 2015)
	// or caused by crappy Unreal's custom new operators.
	// Visual studio 2013 toolset does not have such problem.
}


/**
Constructor called by the game when the renderer is first created.
\note Required to compile for Unreal Tournament.
\note Binding settings to the preferences window needs to done here instead of in init() or the game crashes when starting a map if the renderer's been restarted at least once.
*/
void UVulkan1RenderDevice::StaticConstructor()
{

}

/**
Initialization of renderer.
- Set parent class options. Some of these are settings for the renderer to heed, others control what the game does.
	- URenderDevice::SpanBased; Probably for software renderers.
	- URenderDevice::Fullscreen; Only for Voodoo cards.
	- URenderDevice::SupportsTC; Game sends compressed textures if present.
	- URenderDevice::SupportsDistanceFog; Distance fog. Don't know how this is supposed to be implemented.
	- URenderDevice::SupportsLazyTextures; Renderer loads and unloads texture info when needed (???).
	- URenderDevice::PrefersDeferredLoad; Renderer prefers not to cache textures in advance (???).
	- URenderDevice::ShinySurfaces; Renderer supports detail textures. The game sends them always, so it's meant as a detail setting for the renderer.
	- URenderDevice::Coronas; If enabled, the game draws light coronas.
	- URenderDevice::HighDetailActors; If enabled, game sends more detailed models (???).
	- URenderDevice::VolumetricLighting; If enabled, the game sets fog textures for surfaces if needed.
	- URenderDevice::PrecacheOnFlip; The game will call the PrecacheTexture() function to load textures in advance. Also see Flush().
	- URenderDevice::Viewport; Always set to InViewport.
- Initialize graphics api.
- Resize buffers (convenient to use SetRes() for this).

\param InViewport viewport parameters, can get the window handle.
\param NewX Viewport width.
\param NewY Viewport height.
\param NewColorBytes Color depth.
\param Fullscreen Whether fullscreen mode should be used.
\return 1 if init succesful. On 0, game errors out.

\note This renderer ignores color depth.
*/
UBOOL UVulkan1RenderDevice::Init(UViewport *InViewport, INT NewX, INT NewY, INT NewColorBytes, UBOOL Fullscreen)
{

	return 1;
}

/**
Resize buffers and viewport.
\return 1 if resize succesful. On 0, game errors out.

\note Switching to fullscreen exits and reinitializes the renderer.
\note Fullscreen can have values other than 0 and 1 for some reason.
\note This function MUST call URenderDevice::Viewport->ResizeViewport() or the game will stall.
*/
UBOOL UVulkan1RenderDevice::SetRes(INT NewX, INT NewY, INT NewColorBytes, UBOOL Fullscreen)
{

	return 1;
}

/**
Cleanup.
*/
void UVulkan1RenderDevice::Exit()
{

}

/**
Empty texture cache.
\param AllowPrecache Enabled if the game allows us to precache; respond by setting URenderDevice::PrecacheOnFlip = 1 if wanted. This does make load times longer.
*/
#if UNREALGOLD
void UVulkan1RenderDevice::Flush()
#else
void UVulkan1RenderDevice::Flush(UBOOL AllowPrecache)
#endif
{

}

/**
Clear screen and depth buffer, prepare buffers to receive data.
\param FlashScale To do with flash effects, see notes.
\param FlashFog To do with flash effects, see notes.
\param ScreenClear The color with which to clear the screen. Used for Rune fog.
\param RenderLockFlags Signify whether the screen should be cleared. Depth buffer should always be cleared.
\param InHitData Something to do with clipping planes; safe to ignore.
\param InHitSize Something to do with clipping planes; safe to ignore.

\note 'Flash' effects are fullscreen colorization, for example when the player is underwater (blue) or being hit (red).
Depending on the values of the related parameters (see source code) this should be drawn; the games don't always send a blank flash when none should be drawn.
EndFlash() ends this, but other renderers actually save the parameters and start drawing it there (probably so it is drawn with the correct depth).
\note RenderLockFlags aren't always properly set, this results in for example glitching in the Unreal castle flyover, in the wall of the tower with the Nali on it.
*/
void UVulkan1RenderDevice::Lock(FPlane FlashScale, FPlane FlashFog, FPlane ScreenClear, DWORD RenderLockFlags, BYTE* InHitData, INT* InHitSize)
{

}

/**
Finish rendering.
/param Blit Whether the front and back buffers should be swapped.
*/
void UVulkan1RenderDevice::Unlock(UBOOL Blit)
{
	
}

/**
Complex surfaces are used for map geometry. They consists of facets which in turn consist of polys (triangle fans).
\param Frame The scene. See SetSceneNode().
\param Surface Holds information on the various texture passes and the surface's PolyFlags.
	- PolyFlags contains the correct flags for this surface. See polyflags.h
	- Texture is the diffuse texture.
	- DetailTexture is the nice close-up detail that's modulated with the diffuse texture for walls. It's up to the renderer to only draw these on near surfaces.
	- LightMap is the precalculated map lighting. Should be drawn with a -.5 pan offset.
	- FogMap is precalculated fog. Should be drawn with a -.5 pan offset. Should be added, not modulated. Flags determine if it should be applied, see polyflags.h.
	- MacroTexture is similar to a detail texture but for far away surfaces. Rarely used.
\param Facet Contains coordinates and polygons.
	- MapCoords are used to calculate texture coordinates. Involved. See code.
	- Polys is a linked list of triangle fan arrays; each element is similar to the models used in DrawGouraudPolygon().

\note DetailTexture and FogMap are mutually exclusive.
\note Check if submitted polygons are valid (3 or more points).
*/
void UVulkan1RenderDevice::DrawComplexSurface(FSceneNode* Frame, FSurfaceInfo& Surface, FSurfaceFacet& Facet)
{
	
}

/**
Gouraud shaded polygons are used for 3D models and surprisingly shadows.
They are sent with a call of this function per triangle fan, worldview transformed and lit. They do have normals and texture coordinates (no panning).
\param Frame The scene. See SetSceneNode().
\param Info The texture for the model. Models only come with diffuse textures.
\param Pts A triangle fan stored as an array. Each element has a normal, light (i.e. color) and fog (color due to being in fog).
\param NumPts Number of verts in fan.
\param PolyFlags Contains the correct flags for this model. See polyflags.h
\param Span Probably for software renderers.

\note Modulated models (i.e. shadows) shouldn't have a color, and fog should only be applied to models with the correct flags for that. The D3D10 renderer handles this in the shader.
\note Check if submitted polygons are valid (3 or more points).
*/
void UVulkan1RenderDevice::DrawGouraudPolygon(FSceneNode* Frame, FTextureInfo& Info, FTransTexture** Pts, int NumPts, DWORD PolyFlags, FSpanBuffer* Span)
{

}

/**
Used for 2D UI elements, coronas, etc.
\param Frame The scene. See SetSceneNode().
\param Info The texture for the quad.
\param X X coord in screen space.
\param Y Y coord in screen space.
\param XL Width in pixels
\param YL Height in pixels
\param U Texure U coordinate for left.
\param V Texture V coordinate for top.
\param UL U+UL is coordinate for right.
\param VL V+VL is coordinate for bottom.
\param Span Probably for software renderers.
\param Z coordinate (similar to that of other primitives).
\param Color color
\param Fog fog
\param PolyFlags Contains the correct flags for this tile. See polyflags.h

\note Need to set scene node here otherwise Deus Ex dialogue letterboxes will look wrong; they aren't properly sent to SetSceneNode() it seems.
\note Drawn by converting pixel coordinates to -1,1 ranges in vertex shader and drawing quads with X/Y perspective transform disabled.
The Z coordinate however is transformed and divided by W; then W is set to 1 in the shader to get correct depth and yet preserve X and Y.
Other renderers take the opposite approach and multiply X by RProjZ*Z and Y by RProjZ*Z*aspect so they are preserved and then transform everything.
*/
void UVulkan1RenderDevice::DrawTile(FSceneNode* Frame, FTextureInfo& Info, FLOAT X, FLOAT Y, FLOAT XL, FLOAT YL, FLOAT U, FLOAT V, FLOAT UL, FLOAT VL, class FSpanBuffer* Span, FLOAT Z, FPlane Color, FPlane Fog, DWORD PolyFlags)
{
	
}

/**
For UnrealED.
*/
void UVulkan1RenderDevice::Draw2DLine(FSceneNode* Frame, FPlane Color, DWORD LineFlags, FVector P1, FVector P2)
{
}

/**
For UnrealED.
*/
void UVulkan1RenderDevice::Draw2DPoint(FSceneNode* Frame, FPlane Color, DWORD LineFlags, FLOAT X1, FLOAT Y1, FLOAT X2, FLOAT Y2, FLOAT Z)
{
}

/**
Clear the depth buffer. Used to draw the skybox behind the rest of the geometry, and weapon in front.
\note It is important that any vertex buffer contents be commited before actually clearing the depth!
*/
void UVulkan1RenderDevice::ClearZ(FSceneNode* Frame)
{
	
}

/**
Something to do with clipping planes, not needed.
*/
void UVulkan1RenderDevice::PushHit(const BYTE* Data, INT Count)
{
}

/**
Something to do with clipping planes, not needed.
*/
void UVulkan1RenderDevice::PopHit(INT Count, UBOOL bForce)
{
}

/**
Something to do with FPS counters etc, not needed.
*/
void UVulkan1RenderDevice::GetStats(TCHAR* Result)
{

}

/**
Used for screenshots and savegame previews.
\param Pixels An array of 32 bit pixels in which to dump the back buffer.
*/
void UVulkan1RenderDevice::ReadPixels(FColor* Pixels)
{
	
}

/**
Various command from the game. Can be used to intercept input. First let the parent class handle the command.

\param Cmd The command
	- GetRes Should return a list of resolutions in string form "HxW HxW" etc.
	- Brightness is intercepted here
\param Ar A class to which to log responses using Ar.Log().

\note Deus Ex ignores resolutions it does not like.
*/
UBOOL UVulkan1RenderDevice::Exec(const TCHAR * Cmd, FOutputDevice& Ar)
{
	return 1;
}



/**
This optional function can be used to set the frustum and viewport parameters per scene change instead of per drawXXXX() call.
\param Frame Contains various information with which to build frustum and viewport.
\note Standard Z parameters: near 1, far 32760. However, it seems ComplexSurfaces (except water's surface when in it) are at least at Z = ~13; models in DX cut scenes ~7. Can be utilized to gain increased z-buffer precision.
Unreal/UT weapons all seem to fall within ZWeapons: Z<12. Can be used to detect, clear depth (to prevent intersecting world) and move them. Only disadvantage of using increased zNear is that water surfaces the player is bobbing in don't look as good.
The D3D10 renderer moves gouraud polygons and tiles with Z < zNear (or Z < ZWeapons if needed) inside the range, allowing Unreal/UT weapons (after a depth clear) and tiles to be displayed correctly. ComplexSurfaces are not moved as this results in odd looking water surfaces.
*/
void UVulkan1RenderDevice::SetSceneNode(FSceneNode* Frame)
{
	
}

/**
Store a texture in the renderer-kept texture cache. Only called by the game if URenderDevice::PrecacheOnFlip is 1.
\param Info Texture (meta)data. Includes a CacheID with which to index.
\param PolyFlags Contains the correct flags for this texture. See polyflags.h

\note Already cached textures are skipped, unless it's a dynamic texture, in which case it is updated.
\note Extra care is taken to recache textures that aren't saved as masked, but now have flags indicating they should be (masking is not always properly set).
	as this couldn't be anticipated in advance, the texture needs to be deleted and recreated.
*/
void UVulkan1RenderDevice::PrecacheTexture(FTextureInfo& Info, DWORD PolyFlags)
{
	
}

/**
Other renderers handle flashes here by saving the related structures; this one does it in Lock().
*/
void  UVulkan1RenderDevice::EndFlash()
{

}

#ifdef RUNE
/**
Rune world fog is drawn by clearing the screen in the fog color, clipping the world geometry outside the view distance
and then overlaying alpha blended planes. Unfortunately this function is only called once it's actually time to draw the
fog, as such it's difficult to move this into a shader.

\param Frame The scene. See SetSceneNode().
\param ForSurf Fog plane information. Should be drawn with alpha blending enabled, color alpha = position.z/FogDistance.
\note The pre- and post function for this are meant to set blend state but aren't really needed.
*/
void UVulkan1RenderDevice::DrawFogSurface(FSceneNode* Frame, FFogSurf &FogSurf)
{
	float mult = 1.0 / FogSurf.FogDistance;
	_d3d->setProjectionMode(D3D::PROJ_NORMAL);

	_d3d->setFlags(PF_AlphaBlend, 0);
	_d3d->setTexture(D3D::PASS_DIFFUSE, NULL);
	_d3d->setTexture(D3D::PASS_LIGHT, NULL);
	_d3d->setTexture(D3D::PASS_DETAIL, NULL);
	_d3d->setTexture(D3D::PASS_FOG, NULL);
	_d3d->setTexture(D3D::PASS_MACRO, NULL);
	for (FSavedPoly* Poly = FogSurf.Polys; Poly; Poly = Poly->Next)
	{
		_d3d->indexTriangleFan(Poly->NumPts);
		for (int i = 0; i < Poly->NumPts; i++)
		{
			D3D::Vertex* v = _d3d->getVertex();
			v->flags = FogSurf.PolyFlags;
			v->Color = *((d3dmath::Vec4*)&FogSurf.FogColor.X);
			v->Pos = *(d3dmath::Vec3*)&Poly->Pts[i]->Point.X;
			v->Color.w = v->Pos.z*mult;
			v->flags = PF_AlphaBlend;
		}
	}
}

/**
Rune object fog is normally drawn using the API's linear fog methods. In the D3D10 case, in the shader.
This function tells us how to configure the fog.

\param Frame The scene. See SetSceneNode().
\param FogDistance The end distance of the fog (start distance is always 0)
\param FogColor The fog's color.
*/
void UVulkan1RenderDevice::PreDrawGouraud(FSceneNode *Frame, FLOAT FogDistance, FPlane FogColor)
{
	if (FogDistance > 0)
	{
		d3dmath::Vec4 *color = ((d3dmath::Vec4*)&FogColor.X);
		_d3d->fog(FogDistance, color);
	}
}

/**
Turn off fogging off.
\param FogDistance Distance with which fog was previously turned on.
*/
void UVulkan1RenderDevice::PostDrawGouraud(FLOAT FogDistance)
{
	if (FogDistance > 0)
	{
		_d3d->fog(0, nullptr);
	}
}
#endif