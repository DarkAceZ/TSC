#ifndef TSC_WORLD_LAYER_LOADER_HPP
#define TSC_WORLD_LAYER_LOADER_HPP
#include "../core/global_game.hpp"
#include "../core/xml_attributes.hpp"
#include "world_layer.hpp"

namespace TSC {

    class cOverworldLayerLoader: public xmlpp::SaxParser {
    public:
        // layers can only be created for a specific overworld.
        cOverworldLayerLoader(cOverworld* p_overworld);
        virtual ~cOverworldLayerLoader();

        // Parse the given filename.
        virtual void parse_file(boost::filesystem::path filename);

        cLayer* Get_Layer();
        cOverworld* Get_Overworld();
    protected:
        // SAX parser callbacks
        virtual void on_start_document();
        virtual void on_end_document();
        virtual void on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& properties);
        virtual void on_end_element(const Glib::ustring& name);

    private:
        void handle_line();

        // The layer we’re building
        cLayer* mp_layer;
        // The overworld we’re building the layer for.
        cOverworld* mp_overworld;
        // The file we’re parsing
        boost::filesystem::path m_layerfile;
        // The <property> results we found before the current tag.
        XmlAttributes m_current_properties;
    };

}

#endif
