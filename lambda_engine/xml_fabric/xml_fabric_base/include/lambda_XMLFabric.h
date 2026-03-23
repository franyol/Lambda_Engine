#ifndef _LE_XML_FABRIC_H_
#define _LE_XML_FABRIC_H_

    #include "rapidxml.hpp"
    #include <string>
    #include <vector>
    #include <map>
    #include <iostream>
    #include <fstream>
    #include <cstring>
    #include <stdexcept>

    #define REC_LIMIT 50

    using namespace rapidxml;

    typedef std::map<std::string, std::string> Attr;

    /**
     * @brief Search and read node value, attributes and value
     *
     * Base class to parse xml files into usable game objects
     * */
    class LE_XMLNode
    {
        private:
            // Tag name to search
            std::string tagName;

            std::vector<LE_XMLNode*> childs;
            Attr attributes;
            std::string value;

            void (*onRead) ( const Attr& attr, const std::string value );
        public:
            LE_XMLNode ( std::string tagname ) : tagName(tagname) { onRead = nullptr; }

            ~LE_XMLNode() {
                childs.clear();
            }

            /**
             * @brief set onRead function
             *
             * Here is where objects must be created
             * */
            void setOnRead (
                void (*onread) ( const Attr& attr, const std::string value ) ) {
                onRead = onread;
            }

            /**
             * @brief add child object to be read
             *
             * Caution: childs won't be deallocated
             * when deleting the instance.
             *
             * @param child pointer to LE_XMLNode*
             * */
            void addChild ( LE_XMLNode* child ) {
                childs.push_back ( child );
            }

            /**
             * @brief read node and childs info
             *
             * @param node xml node object
             * @param pAttr parent attributes
             * */
            void read(xml_node<>* root, Attr& pAttr) {
                struct StackItem {
                    xml_node<>* xmlNode;
                    LE_XMLNode* self;
                    Attr parentAttr;
                };

                std::vector<StackItem> stack;
                stack.push_back({root, this, pAttr});

                while (!stack.empty()) {
                    StackItem current = stack.back();
                    stack.pop_back();

                    xml_node<>* node = current.xmlNode;
                    LE_XMLNode* self = current.self;

                    // Tag check
                    if (std::strcmp(self->tagName.c_str(), node->name()) != 0) {
                        continue;
                    }

                    // Value
                    if (node->value_size() > 0)
                        self->value = node->value();
                    else
                        self->value = "";

                    // Attributes
                    self->attributes.clear();
                    for (xml_attribute<>* attr = node->first_attribute(); attr;
                            attr = attr->next_attribute()) {
                        self->attributes[attr->name()] = attr->value();
                    }

                    // Merge parent attributes
                    self->attributes.insert(current.parentAttr.begin(),
                            current.parentAttr.end());

                    // Callback
                    if (self->onRead != nullptr) {
                        self->onRead(self->attributes, self->value);
                    }

                    // Traverse children
                    for (xml_node<>* nodeChild = node->first_node(); nodeChild;
                            nodeChild = nodeChild->next_sibling()) {

                        for (LE_XMLNode* child : self->childs) {
                            stack.push_back({nodeChild, child, self->attributes});
                        }
                    }
                }
            }

            /**
             * @brief start reading document
             *
             * @param filepath to the xml file
             * */
            void readDoc ( std::string filePath, Attr* pAttr = nullptr ) {
                std::ifstream file (filePath.c_str());
                std::string xmlString((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

                // Parse XML
                xml_document<> doc;
                doc.parse<0>(&xmlString[0]);

                Attr parentAtt;
                if ( pAttr != nullptr )
                    parentAtt = *pAttr;

                for ( xml_node<>* node = doc.first_node(); node;
                        node = node->next_sibling() )
                    read ( node, parentAtt );
            }

    };

#endif
