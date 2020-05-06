#pragma once

// GXEZ
#include "GXEZ/GXEZDefinitions.h"
#include "GXEZ/IGXEZContext.h"


namespace GXEZ
{
    class GXEZEnvironment
    {
    public:
        // THIS - Singleton Instance
        static GXEZEnvironment   _singleton;

        static GXEZEnvironment& Get() noexcept { // pour obtenir le singleton
            return (_singleton);
        }

        IGXEZContext* GetContext() { return (_context); };

    private:

        GXEZEnvironment(IGXEZContext::Type context_type);

        ~GXEZEnvironment()
        {
            Release();
        }

        void Init(IGXEZContext::Type context_type);
        void Release();

        IGXEZContext*            _context;

    };
};


/// 
/// USEFULL Informations can be write down here
///
/// -> Events
///
/// Eventhanlder should be linkable to windows