#ifndef __REVCOLOR_H__
#define __REVCOLOR_H__

class revColor
{
public:
	static const revColor red;
	static const revColor green;
	static const revColor blue;
	static const revColor white;
	static const revColor black;


    revColor(float _r, float _g, float _b, float _a);
    
  	union
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
		float data[4];
	};
 
    
};

inline revColor::revColor(float _r, float _g, float _b, float _a) :
r(_r),
g(_g),
b(_b),
a(_a)
{
}

#endif /* defined(__Rev__revColor__) */
