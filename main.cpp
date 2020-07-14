#include <iostream>
#include <Windows.h>

#include <optional>
#include <iostream>
#include <string>

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VK_USE_PLATFORM_WIN32_KHR
#define NDEBUG

#include <vulkan/vulkan.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE;


static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
                                                    , VkDebugUtilsMessageTypeFlagsEXT messageType
                                                    , const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData
                                                    , void *pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

int main() {
    // Initial loading of Vulkan instance
    vk::DynamicLoader dl;
    auto vkGetInstanceProcAddr =
            dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

    vk::ApplicationInfo appInfo =
            vk::ApplicationInfo("Chess++", VK_MAKE_VERSION(1, 0, 0), "Snek", VK_MAKE_VERSION(1, 0, 0)
                                , VK_API_VERSION_1_2);


    const std::vector<const char *> extensions = {VK_KHR_SURFACE_EXTENSION_NAME,
                                                  VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
                                                  VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    vk::InstanceCreateInfo info = vk::InstanceCreateInfo({}, &appInfo, 1, &validationLayers[0]
                                                         , 3
                                                         , &extensions[0]);
    vk::Instance instance = vk::createInstance(info, nullptr);
    VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);

    vk::DebugUtilsMessengerEXT debugMessenger;
    vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT({},
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral, debugCallback);
    vk::Result debuggerResult = instance.createDebugUtilsMessengerEXT(&debugCreateInfo, nullptr, &debugMessenger);
    if (debuggerResult != vk::Result::eSuccess) {
        std::cout << "Could not create debugger!" << std::endl;
    }


    HMODULE hinstance = GetModuleHandle(nullptr);

    // Taken from here: https://support.microsoft.com/en-us/help/124103/how-to-obtain-a-console-window-handle-hwnd
    HWND hwnd;         // This is what is returned to the caller.
    char pszNewWindowTitle[1024]; // Contains fabricated
    // WindowTitle.
    char pszOldWindowTitle[1024]; // Contains original
    // WindowTitle.

    // Fetch current window title.
    GetConsoleTitle(pszOldWindowTitle, 1024);

    // Format a "unique" NewWindowTitle.
    wsprintf(pszNewWindowTitle, "%d/%d", GetTickCount(), GetCurrentProcessId());

    // Change current window title.
    SetConsoleTitle(pszNewWindowTitle);
    hwnd = FindWindow(nullptr, pszNewWindowTitle);

    vk::Win32SurfaceCreateInfoKHR surfaceCreateInfo = vk::Win32SurfaceCreateInfoKHR({}, hinstance, hwnd);
    vk::SurfaceKHR surface;
    vk::Result surfaceResult = instance.createWin32SurfaceKHR(&surfaceCreateInfo, nullptr, &surface);
    if (surfaceResult != vk::Result::eSuccess) {
        std::cout << "Could not create surface! Good!" << std::endl;
        std::cout << surfaceResult << std::endl;
        return 1;
    } else {
        std::cout << "Surface creation succeeded even though it should not have?" << std::endl;
    }

    std::vector<vk::PhysicalDevice> physicalDevices =
            instance.enumeratePhysicalDevices();
    vk::SurfaceCapabilitiesKHR surfaceCapabilities;
    vk::Result result = physicalDevices[0].getSurfaceCapabilitiesKHR(surface, &surfaceCapabilities);
    if (result != vk::Result::eSuccess) {
        std::cout << "Surface capabilities will say it lost the surface:" << std::endl;
        std::cout << result << std::endl;
    } else {
        std::cout
                << "If surface capabilities passed, then we are in a weird situation, but generally the app crashes if you don't check the result"
                << std::endl;
    }


    if (instance) {
        instance.destroy(debugMessenger);
        instance.destroy(surface);
        instance.destroy();
    }


    return 0;
}

