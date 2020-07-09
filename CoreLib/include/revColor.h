#ifndef __REVCOLOR_H__
#define __REVCOLOR_H__

class revColor
{
public:
	static const revColor RED;
	static const revColor GREEN;
	static const revColor BLUE;
	static const revColor WHITE;
	static const revColor BLACK;

	revColor():
		r(0),
		g(0),
		b(0),
		a(0)
	{
	}


    revColor(float _r, float _g, float _b, float _a):
		r(_r),
		g(_g),
		b(_b),
		a(_a)
	{
	}
    
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

#endif /* defined(__Rev__revColor__) */
