// ShaderColorSelected3DOptimized
// Andre Berthiaume, july 2016

#ifndef _ShaderColorSelected3DOptimized
#define _ShaderColorSelected3DOptimized

#include "ShaderBase.h"
#include "Matrix.h"

struct ID3D11Buffer;
struct ID3D11Device;

class ShaderColorSelected3DOptimized : public ShaderBase
{
private:

	struct Data_WorldColorSelected
	{
		Matrix WVP;
		Vect Color;
	};

	ID3D11Buffer*           mpBuffWordColor;

public:
	ShaderColorSelected3DOptimized(const ShaderColorSelected3DOptimized&) = delete;				 // Copy constructor
	ShaderColorSelected3DOptimized(ShaderColorSelected3DOptimized&&) = default;                    // Move constructor
	ShaderColorSelected3DOptimized& operator=(const ShaderColorSelected3DOptimized&) & = default;  // Copy assignment operator
	ShaderColorSelected3DOptimized& operator=(ShaderColorSelected3DOptimized&&) & = default;       // Move assignment operator
	~ShaderColorSelected3DOptimized();		  							         // Destructor

	ShaderColorSelected3DOptimized(ID3D11Device* device);

	virtual void SetToContext(ID3D11DeviceContext* devcon) override;

	void SendWVPColor(const Matrix& world, const Vect& col);


};

#endif _ShaderColorSelected3DOptimized
