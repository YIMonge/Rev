#ifndef __REVCOLOR_H__
#define __REVCOLOR_H__

#include "revTypedef.h"

class revColor
{
public:
	static const revColor red;
	static const revColor green;
	static const revColor blue;
	static const revColor white;
	static const revColor black;


    revColor(f32 _r, f32 _g, f32 _b, f32 _a):
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
			f32 r;
			f32 g;
			f32 b;
			f32 a;
		};
		f32 data[4];
	};
 
    
};

#endif /* defined(__Rev__revColor__) */
