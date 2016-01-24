#include <libxml/parser.h>
#include <dirent.h>

#include <MCConfigManager.h>

//#include <libxml/tree.h>
//#include <libxml/xpath.h>

#define XML_DIR "./"

namespace MC
{

    ConfigManager::ConfigManager()
    {
    }


    ConfigManager::~ConfigManager()
    {
    }


    bool ConfigManager::loadFromFile(const string & filename)
    {
/*        xmlDoc * doc = NULL;

        if ((doc = xmlReadFile(filename.c_str(), nullptr, 0)) == nullptr)
        {
            LOG("[Config] Could not parse file %s!\n", filename.c_str());

            return false;
        }

        LOG("[Config] Parsed file %s\n", filename.c_str());


        return true;*/

        return false;
    }


    void ConfigManager::loadAll()
    {
        vector < string > files;

        DIR * dir;
        dirent * entry;

        if ((dir = opendir(XML_DIR)) == nullptr)
        {
            return;
        }

        while ((entry = readdir(dir)) != nullptr)
        {
            string fileName = entry->d_name;

            if ((fileName.length() < 5) || (fileName.substr(fileName.length() - 4) != ".xml"))
            {
                continue;
            }

            loadFromFile(XML_DIR + fileName);
        }

        closedir (dir);
    }


    vector <string> ConfigManager::getKeys(const string & category)
    {
        xmlDoc * doc = NULL;
        vector <string> result;
string filename = "mastercontrol.xml";


        if ((doc = xmlReadFile(filename.c_str(), nullptr, 0)) == nullptr)
        {
            LOG("[Config] Could not parse file %s!\n", filename.c_str());

            return result;
        }

        LOG("[Config] Parsed file %s\n", filename.c_str());


        xmlNode * node = xmlDocGetRootElement(doc);
       
        if ((node->type != XML_ELEMENT_NODE) || (string((const char *) node->name) != "mastercontrol"))
        {
            return result;
        }

        for (node = node->children; node != nullptr; node = node->next)
        {
            if ((node->type != XML_ELEMENT_NODE) || (string((const char *) node->name) != category))
            {
                continue;
            }

            xmlNode * subnode;

            for (subnode = node->children; subnode != nullptr; subnode = subnode->next)
            {
                if (subnode->type != XML_ELEMENT_NODE)
                {
                    continue;
                }

                result.push_back((const char *) subnode->name);

                printf("subnode type: Element, name: %s\n", subnode->name);

                /* xmlChar * prop = xmlGetProp(subnode, (const xmlChar*) "value");
                  printf("prop: %s\n", uri);
                  xmlFree(prop);*/

            }
        }

        xmlFreeDoc(doc);
        xmlCleanupParser();

        return result;
    }

    vector <string> ConfigManager::getValues(const string & category, const string & key)
    {
    }

}
 
 
