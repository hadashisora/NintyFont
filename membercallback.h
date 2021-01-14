// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Quick and dirty callback handler
#pragma once

namespace NintyFont
{
    typedef void (CallbackFunction)(void *, void *); //Argument one is this pointer, the second is the optional arument( struct)

    struct MemberCallback
    {
        CallbackFunction *method;
        void *object;
        void *args;
    };

    inline void doMemberCallback(MemberCallback *callback)
    {
        if (callback == nullptr) return;
        if (callback->method == nullptr || callback->object == nullptr) return;
        callback->method(callback->object, callback->args);
    }
}