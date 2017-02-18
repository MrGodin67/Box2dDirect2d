#pragma once
class RenderTarget
{
public:
	virtual ~RenderTarget() {}
	virtual void Rasterize(class Drawable& obj) = 0;
};