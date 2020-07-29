#pragma once

#include "hostio/seadHostIOEventListener.h"
#include "prim/seadSafeString.hpp"

namespace sead
{
class Heap;

namespace hostio
{
class Context;
class GenEvent;

class Reflexible : public NodeEventListener
{
public:
    enum class NodeClassType
    {
        /// sead::hostio::Reflexible
        Reflexible = 0,
        /// sead::hostio::Node
        Node = 1,
        /// Other classes
        Other = 2,
    };

    virtual NodeClassType getNodeClassType() const { return NodeClassType::Reflexible; }

    enum class AllocFlg
    {
        Name = 1u << 0u,
        Meta = 1u << 1u,
    };

#ifdef SEAD_DEBUG
    Reflexible();
    Reflexible(Heap* heap, IDisposer::HeapNullOption heap_null_option);
    ~Reflexible() override;

    void listenNodeEvent(const NodeEvent* event) override;
    virtual void genMessage(Context* context);
    virtual SafeString getMetaFilename();
    virtual void genObjectInfo(const GenEvent* event, u32);
    virtual Reflexible* searchNode(const SafeString& name);
    virtual void calcURL(BufferedSafeString* url);
    virtual void calcNodeURL(const Reflexible* reflexible, BufferedSafeString* url);

    void callGenMessage(Context*, u32);
    void correctChildNodeInfo(Context*);
    void baseListen(const PropertyEvent* event);
    void applyEventDataToMemory(const PropertyEvent* event);

    SafeString getNodeName() const;
    void setNodeName(const SafeString& name);
    void setNodeNameCopyString(const SafeString& name, Heap* heap);

    SafeString getNodeMeta() const;
    void setNodeMeta(const SafeString& meta);
    void setNodeMetaCopyString(const SafeString& meta, Heap* heap);

protected:
    void disposeHostIO() override;
    virtual void genChildNode(Context* context);
    virtual bool isHaveChild() const;

private:
    using ApplyEventDataToMemoryCallback = bool (*)(const PropertyEvent* event);

    void safeDelete_(AllocFlg flag);
    const char* createStringBuffer_(AllocFlg flag, const SafeString&, Heap* heap);
    void disposeHostIOImpl_();

    const char* mName;
    const char* mMeta;
    bool mIsGenerated = false;
    u8 mAllocFlg = 0;
    static ApplyEventDataToMemoryCallback sApplyEventDataToMemoryCallback;
#endif
};
}  // namespace hostio
}  // namespace sead
