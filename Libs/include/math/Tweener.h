/*****************

 This port was based in a inital code from Jesus Gollonet, him port Penners easing equations to C/C++:

 http://www.jesusgollonet.com/blog/2007/09/24/penner-easing-cpp/
 http://robertpenner.com/easing/

 I\m just make a better wrapper a litlle more OOP e put some callbacks like the original Tweener
 (http://code.google.com/p/tweener/)


 wesley.marques@gmail.com  - Wesley Ferreira Marques
 http://codevein.com


 **********************/

#if !defined(__IZANAGI_STD_TWEENER_H__)
#define __IZANAGI_STD_TWEENER_H__

#include "izDefs.h"

namespace izanagi {
namespace math {

    class ITweenerEasing {
	public:
		ITweenerEasing() {}
		virtual ~ITweenerEasing() {}

		virtual IZ_FLOAT EaseIn(IZ_FLOAT t, IZ_FLOAT d) = 0;
		virtual IZ_FLOAT EaseOut(IZ_FLOAT t, IZ_FLOAT d) = 0;
		virtual IZ_FLOAT EaseInOut(IZ_FLOAT t, IZ_FLOAT d) = 0;
    };

    class CTweenerBack : public ITweenerEasing {
	public:
		IZ_FLOAT EaseIn(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseOut(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseInOut(IZ_FLOAT t, IZ_FLOAT d);
    };

    class CTweenerBounce : public ITweenerEasing {
	public:
		IZ_FLOAT EaseIn(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseOut(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseInOut(IZ_FLOAT t, IZ_FLOAT d);
    };

	class CTweenerCirc : public ITweenerEasing {
	public:
		IZ_FLOAT EaseIn(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseOut(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseInOut(IZ_FLOAT t, IZ_FLOAT d);
    };

    class CTweenerCubic : public ITweenerEasing {
	public:
		IZ_FLOAT EaseIn(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseOut(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseInOut(IZ_FLOAT t, IZ_FLOAT d);
    };

    class CTweenerElastic : public ITweenerEasing {
	public:
		IZ_FLOAT EaseIn(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseOut(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseInOut(IZ_FLOAT t, IZ_FLOAT d);
    };

    class CTweenerExpo : public ITweenerEasing {
	public:
		IZ_FLOAT EaseIn(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseOut(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseInOut(IZ_FLOAT t, IZ_FLOAT d);
    };

    class CTweenerQuad : public ITweenerEasing {
	public:
		IZ_FLOAT EaseIn(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseOut(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseInOut(IZ_FLOAT t, IZ_FLOAT d);
    };


    class CTweenerQuart : public ITweenerEasing {
	public:
		IZ_FLOAT EaseIn(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseOut(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseInOut(IZ_FLOAT t, IZ_FLOAT d);
    };

    class CTweenerQuint : public ITweenerEasing {
	public :
		IZ_FLOAT EaseIn(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseOut(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseInOut(IZ_FLOAT t, IZ_FLOAT d);
    };

    class CTweenerSine : public ITweenerEasing {
	public :
		IZ_FLOAT EaseIn(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseOut(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseInOut(IZ_FLOAT t, IZ_FLOAT d);
    };

    class CTweenerLinear : public ITweenerEasing {
	public :
		IZ_FLOAT EaseNone(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseIn(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseOut(IZ_FLOAT t, IZ_FLOAT d);
		IZ_FLOAT EaseInOut(IZ_FLOAT t, IZ_FLOAT d);
    };

}	// namespace math
}	// namespace izanagi;

#endif	// #if !defined(__IZANAGI_STD_TWEENER_H__)
