#ifndef __REVMATERIAL_H__
#define __REVMATERIAL_H__

class revColor;
class revShader;

class revMaterial
{
public:
	revMaterial();
	revMaterial(const revMaterial& source);

private:
	revColor   color;
	revShader* shader;
};

#endif