// ShaderTexture
// Andre Berthiaume, july 2016

#ifndef _ShaderTexture
#define _ShaderTexture

#include "ShaderBase.h"
#include "Matrix.h"
#include <Texture.h>

struct ID3D11Buffer;
struct ID3D11Device;

class ShaderTexture : public ShaderBase
{
public:
	ShaderTexture(const ShaderTexture&) = delete;				 // Copy constructor
	ShaderTexture(ShaderTexture&&) = default;                    // Move constructor
	ShaderTexture& operator=(const ShaderTexture&) & = default;  // Copy assignment operator
	ShaderTexture& operator=(ShaderTexture&&) & = default;       // Move assignment operator
	~ShaderTexture();		  							         // Destructor

	ShaderTexture(ID3D11Device* device);

	virtual void SetToContext(ID3D11DeviceContext* devcon) override;

	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void ShaderTexture::SendWorld(const Matrix& world);

private:

	ID3D11Buffer* mpcbViewProj;

	struct ViewProjData
	{
		Matrix mView;
		Matrix mProjection;
	};

	ID3D11Buffer* mpcbWorld;

	struct WorldData
	{
		Matrix World;
		// The texture might change too, but that's done through mpTextureRV
	};
};

#endif _ShaderTexture
