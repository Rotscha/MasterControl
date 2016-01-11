#ifndef __MCSingleton_h__
#define __MCSingleton_h__

#include <MCPrerequisites.h>

namespace MC
{

    template <class T>
    class Singleton
    {
        private:
           static T *  singleton;

        public:
            Singleton()
            {
                assert(!singleton);

                singleton = (T *) this;
            }

            virtual ~Singleton()
            {
                singleton = nullptr;
            }

            static T * getSingletonPtr()
            {
                assert(singleton);

                return singleton;
            }

    }; // class Singleton

    template < class T > T * Singleton < T >::singleton = nullptr;

} // namespace MC

#endif // __MCSingleton_h__