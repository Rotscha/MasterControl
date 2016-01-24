#ifndef __MCDevice_h__
#define __MCDevice_h__

#include <MCPrerequisites.h>

namespace MC
{

    class Device
    {
        protected:
            string           node;
            int              idVendor;
            int              idProduct;
            string           strVendor;
            string           strProduct;
            string           reportDescriptor;


        public:
                             Device();
            virtual        ~ Device();

            virtual bool     enable()                                 = 0;
            virtual void     disable()                                = 0;
            virtual int      recvReport(void * data, int size)        = 0;
            virtual int      sendReport(void * data, int size)        = 0;
            virtual int      recvFeatureReport(void * data, int size) = 0;
            virtual int      sendFeatureReport(void * data, int size) = 0;

            virtual string   getName() const;
            virtual string   getNode() const;
            virtual int      getVendorId() const;
            virtual int      getProductId() const;
            virtual string   getReportDescriptor() const;

    }; // class Device

} // namespace MC

#endif // __MCDevice_h__ 
