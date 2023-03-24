#pragma once
#include "ShadowMap.h"
class OmniShadowMap :
    public ShadowMap
{
private:
public:
	OmniShadowMap();
	~OmniShadowMap();
	virtual bool Init(GLuint width, GLuint height) override;
	virtual void Write() override;
	virtual void Read(GLenum textureUnit) override;
};

