/** \copyright
 * Copyright (c) 2013, Balazs Racz
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are  permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \file NMRAnetAsyncDefaultNode.hxx
 *
 * Default AsyncNode implementation for a fat virtual node.
 *
 * @author Balazs Racz
 * @date 7 December 2013
 */

#ifndef _NMRAnetAsyncDefaultNode_hxx_
#define _NMRAnetAsyncDefaultNode_hxx_

#include "nmranet/NMRAnetAsyncNode.hxx"

namespace NMRAnet
{

class DefaultAsyncNode : public AsyncNode
{
public:
    DefaultAsyncNode(AsyncIf* interface, NodeID node_id);
    virtual ~DefaultAsyncNode();

    virtual NodeID node_id()
    {
        return nodeId_;
    }
    virtual AsyncIf* interface()
    {
        return interface_;
    }
    virtual bool is_initialized()
    {
        return isInitialized_;
    }

private:
    /** 48-bit node identifier of this node. */
    NodeID nodeId_ : 48;
    /** 1 if the node has reached initialized state. */
    unsigned isInitialized_ : 1;
    /** Interface this node is bound to. */
    AsyncIf* interface_;
};

} // namespace NMRAnet

#endif // _NMRAnetAsyncDefaultNode_hxx_