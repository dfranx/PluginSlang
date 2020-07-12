cbuffer cbData
{
	float uMultiplier;
}

struct PSInput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

interface IObject
{
	float4 DummyMethod();
};
struct Object : IObject
{
	float4 Color;
	
	float4 DummyMethod()
	{
		return Color * 0.5f;
	}
};

float4 genericTest<O : IObject>(O obj)
{
	return obj.DummyMethod() * uMultiplier;
}

float4 main(PSInput pin) : SV_TARGET
{
	Object obj;
	obj.Color = pin.Color;
	return genericTest(obj);
}
