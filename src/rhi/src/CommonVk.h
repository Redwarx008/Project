#pragma once

#include <vulkan/vulkan.h>

#include <string>
#include <iostream>

#include "rhi/Rhi.h"



namespace rhi
{




    struct VkContext
    {
        VkInstance instace;
        VkPhysicalDevice physicalDevice;
        VkDevice device;

        MessageCallback messageCallBack;

        void Error(const std::string& msg) const
        {
            if (messageCallBack)
            {
                messageCallBack(MessageSeverity::Error, msg.c_str());
            }
            else
            {
                // Select prefix depending on flags passed to the callback
                std::string prefix = "ERROR: ";
#if defined(_WIN32)
                prefix = "\033[31m" + prefix + "\033[0m";
#endif
                std::cout << prefix << msg << "\n\n";

            }
            fflush(stdout);
        }

        void Warning(const std::string& msg) const
        {
            if (messageCallBack)
            {
                messageCallBack(MessageSeverity::Warning, msg.c_str());
            }
            else
            {
                // Select prefix depending on flags passed to the callback
                std::string prefix = "WARNING: ";
#if defined(_WIN32)
                prefix = "\033[33m" + prefix + "\033[0m";
#endif
                std::cout << prefix << msg << "\n\n";

            }
            fflush(stdout);
        }
    };
}