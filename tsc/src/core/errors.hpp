#ifndef TSC_ERRORS_HPP
#define TSC_ERRORS_HPP
#include "global_game.hpp"
#include "../objects/movingsprite.hpp"

namespace TSC {

    /**
     * Base class of all TSC-related exceptions.
     */
    class TSCError: public std::exception {
    public:
        TSCError();
        TSCError(std::string message);
        virtual ~TSCError() throw();

        virtual const char* what() const throw();
    protected:
        std::string m_tsc_errmsg;
    };

    class ConfigurationError: public TSCError {
    public:
        ConfigurationError(std::string message);
        virtual ~ConfigurationError() throw();
    };

    /**
     * This exception is thrown when an expected XML
     * key is not found.
     */
    class XmlKeyDoesNotExist: public TSCError {
    public:
        XmlKeyDoesNotExist(std::string key);
        virtual ~XmlKeyDoesNotExist() throw();

        virtual const char* what() const throw();
        std::string Get_Key();
    protected:
        std::string m_key;
    };

    // Thrown if specific things are not implemented for
    // some reason.
    class NotImplementedError: public TSCError {
    public:
        NotImplementedError(std::string message);
        virtual ~NotImplementedError() throw();
    };

    class InvalidLevelError: public TSCError {
    public:
        InvalidLevelError(std::string message);
        virtual ~InvalidLevelError() throw();
    };

    class RestartedXmlParserError: public TSCError {
    public:
        RestartedXmlParserError();
        virtual ~RestartedXmlParserError() throw();
        virtual const char* what() const throw();
    };

    class InvalidMovingStateError: public TSCError {
    public:
        InvalidMovingStateError(Moving_state state);
        virtual ~InvalidMovingStateError() throw();
        virtual const char* what() const throw();
    protected:
        Moving_state m_state;
    };

    class EditorError: public TSCError {
    public:
        EditorError(std::string msg);
        virtual ~EditorError() throw();
    };

    class EditorSpriteCopyFailedError: public EditorError {
    public:
        EditorSpriteCopyFailedError(cSprite* p_sprite);
        virtual ~EditorSpriteCopyFailedError() throw();
        cSprite* Get_Sprite();
    protected:
        cSprite* mp_sprite;
    };

}

#endif
