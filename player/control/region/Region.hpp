#pragma once

#include "control/media/Media.hpp"
#include "control/widgets/Widget.hpp"

#include <boost/signals2/signal.hpp>
#include <memory>

using SignalRegionExpired = boost::signals2::signal<void(int)>;

namespace Xibo
{
    class Region
    {
    public:
        virtual ~Region() = default;

        virtual void addMedia(std::unique_ptr<Media>&& media) = 0;
        virtual void start() = 0;
        virtual SignalRegionExpired& expired() = 0;
        virtual std::shared_ptr<Widget> view() = 0;
    };
}
