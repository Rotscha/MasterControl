#ifndef __MCConfigManager_h__
#define __MCConfigManager_h__

#include <MCPrerequisites.h>

namespace MC
{

    class ConfigManager
    {
        protected:


        public:
                              ConfigManager();
                            ~ ConfigManager();

            bool              loadFromFile(const string & filename);
            void              loadAll();
            vector <string>   getKeys(const string & category);
            vector <string>   getValues(const string & category, const string & key);

    }; // class ConfigManager

} // namespace MC

#endif // __MCConfigManager_h__ 
 
