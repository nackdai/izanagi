/*****************

 This port was based in a inital code from Jesus Gollonet, him port Penners easing equations to C/C++:

 http://www.jesusgollonet.com/blog/2007/09/24/penner-easing-cpp/
 http://robertpenner.com/easing/

 I\m just make a better wrapper a litlle more OOP e put some callbacks like the original Tweener
 (http://code.google.com/p/tweener/)


 wesley.marques@gmail.com  - Wesley Ferreira Marques
 http://codevein.com


 **********************/

#if !defined(__URANUS_STD_TWEENER_H__)
#define __URANUS_STD_TWEENER_H__

#include "unDefs.h"

namespace uranus {

    class ITweenerEasing {
	public:
		ITweenerEasing() {}
		virtual ~ITweenerEasing() {}

		virtual UN_FLOAT EaseIn(UN_FLOAT t, UN_FLOAT d) = 0;
		virtual UN_FLOAT EaseOut(UN_FLOAT t, UN_FLOAT d) = 0;
		virtual UN_FLOAT EaseInOut(UN_FLOAT t, UN_FLOAT d) = 0;
    };

    class CTweenerBack : public ITweenerEasing {
	public:
		UN_FLOAT EaseIn(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseOut(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseInOut(UN_FLOAT t, UN_FLOAT d);
    };

    class CTweenerBounce : public ITweenerEasing {
	public:
		UN_FLOAT EaseIn(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseOut(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseInOut(UN_FLOAT t, UN_FLOAT d);
    };

	class CTweenerCirc : public ITweenerEasing {
	public:
		UN_FLOAT EaseIn(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseOut(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseInOut(UN_FLOAT t, UN_FLOAT d);
    };

    class CTweenerCubic : public ITweenerEasing {
	public:
		UN_FLOAT EaseIn(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseOut(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseInOut(UN_FLOAT t, UN_FLOAT d);
    };

    class CTweenerElastic : public ITweenerEasing {
	public:
		UN_FLOAT EaseIn(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseOut(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseInOut(UN_FLOAT t, UN_FLOAT d);
    };

    class CTweenerExpo : public ITweenerEasing {
	public:
		UN_FLOAT EaseIn(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseOut(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseInOut(UN_FLOAT t, UN_FLOAT d);
    };

    class CTweenerQuad : public ITweenerEasing {
	public:
		UN_FLOAT EaseIn(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseOut(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseInOut(UN_FLOAT t, UN_FLOAT d);
    };


    class CTweenerQuart : public ITweenerEasing {
	public:
		UN_FLOAT EaseIn(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseOut(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseInOut(UN_FLOAT t, UN_FLOAT d);
    };

    class CTweenerQuint : public ITweenerEasing {
	public :
		UN_FLOAT EaseIn(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseOut(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseInOut(UN_FLOAT t, UN_FLOAT d);
    };

    class CTweenerSine : public ITweenerEasing {
	public :
		UN_FLOAT EaseIn(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseOut(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseInOut(UN_FLOAT t, UN_FLOAT d);
    };

    class CTweenerLinear : public ITweenerEasing {
	public :
		UN_FLOAT EaseNone(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseIn(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseOut(UN_FLOAT t, UN_FLOAT d);
		UN_FLOAT EaseInOut(UN_FLOAT t, UN_FLOAT d);
    };
}	// namespace uranus;

#endif	// #if !defined(__URANUS_STD_TWEENER_H__)
