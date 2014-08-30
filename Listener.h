//
// Created by Fateev on 30/08/14.
// Copyright (c) 2014 ___FULLUSERNAME___. All rights reserved.
//


#ifndef __Listener_H_
#define __Listener_H_

#include "Response.h"

namespace poison { namespace net { namespace http {
    class Listener {
    public:
        virtual void onRequestComplete(const Response& resp) = 0;
    };

}}}

#endif //__Listener_H_
