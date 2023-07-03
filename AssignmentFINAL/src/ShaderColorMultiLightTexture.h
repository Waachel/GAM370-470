// ShaderColorMultiLightTexture
// Andre Berthiaume, Feb 2017

#ifndef _ShaderColorMultiLightTexture
#define _ShaderColorMultiLightTexture

#include "ShaderBase.h"
#include "Matrix.h"

struct ID3D11Buffer;
struct ID3D11Device;

class ShaderColorMultiLightTexture : public ShaderBase
{

public:
	ShaderColorMultiLightTexture(const ShaderColorMultiLightTexture&) = delete;				 // Copy constructor
	ShaderColorMultiLightTexture(ShaderColorMultiLightTexture&&) = default;                    // Move constructor
	ShaderColorMultiLightTexture& operator=(const ShaderColorMultiLightTexture&) & = default;  // Copy assignment operator
	ShaderColorMultiLightTexture& operator=(ShaderColorMultiLightTexture&&) & = default;       // Move assignment operator
	~ShaderColorMultiLightTexture();		  							         // Destructor

	ShaderColorMultiLightTexture(ID3D11Device* device);

	virtual void SetToContext(ID3D11DeviceContext* devcon) override;

	void SetDirectionalLightParameters(const Vect& dir, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));
	void SetPointLightParameters(int index, const Vect& pos, float r, const Vect& att, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));
	void SetSpotLightParameters(int index, const Vect& pos, float r, const Vect& att, const Vect& dir, float spotExp, const Vect& amb = Vect(1, 1, 1), const Vect& dif = Vect(1, 1, 1), const Vect& sp = Vect(1, 1, 1));

	void SetFogParameters(const float& fs, const float& fr, const Vect& fc);

	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void SendLightParameters(const Vect& eyepos);
	void SendWorldAndMaterial(const Matrix& world, const Vect& amb = Vect(.5f, .5f, .5f), const Vect& dif = Vect(.5f, .5f, .5f), const Vect& sp = Vect(.5f, .5f, .5f));
	void SendWorld(const Matrix& world);

private:
	ID3D11Buffer* mpcbWorld;

	struct WorldData
	{
		Matrix World;
		// The texture might change too, but that's done through mpTextureRV
	};

	struct Material
	{
		Vect Ambient;
		Vect Diffuse;
		Vect Specular;
	};

	struct PhongADS
	{
		Vect Ambient;
		Vect Diffuse;
		Vect Specular;
	};

	struct Fog
	{
		float FogStart;
		float FogRange;
		Vect FogColor;
	};

	Fog FogData;


	struct DirectionalLight
	{
		PhongADS Light;
		Vect Direction;
	};

	DirectionalLight DirLightData;

	struct PointLight
	{
		PhongADS Light;
		Vect Position;
		Vect Attenuation;
		float Range;
	};

	PointLight PointLightData[3];

	struct SpotLight
	{
		PhongADS Light;
		Vect Position;
		Vect Attenuation;
		Vect Direction;
		float SpotExp;
		float Range;
	};

	SpotLight SpotLightData[2];

	struct CamMatrices
	{
		Matrix View;
		Matrix Projection;
	};

	ID3D11Buffer*  mpBufferCamMatrices;

	struct Data_WorldAndMaterial
	{
		Matrix World;
		Matrix WorlInv;
		Material Mat;
	};

	ID3D11Buffer*	mpBuffWordAndMaterial;

	struct Data_LightParams
	{
		DirectionalLight DirLight;
		PointLight PntLight[3];
		SpotLight SptLight[2];
		Fog f;
		Vect EyePosWorld;
	};

	ID3D11Buffer*  mpBufferLightParams;

};

#endif _ShaderColorMultiLightTexture

