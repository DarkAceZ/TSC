#ifndef TSC_SCRIPTING_GOLD_100_EVENT_HPP
#define TSC_SCRIPTING_GOLD_100_EVENT_HPP
#include "../scripting.hpp"
#include "event.hpp"

namespace TSC {
    namespace Scripting {

        class cGold_100_Event: public cEvent {
        public:
            virtual std::string Event_Name()
            {
                return "gold_100";
            }
        };
    }
}

#endif
