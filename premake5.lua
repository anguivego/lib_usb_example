workspace "libusb_test"
   configurations { "Debug", "Release" }
   local vcpkg_root = os.getenv("VCPKG_ROOT")

   if not vcpkg_root then
       error("VCPKG_ROOT environment variable is not set!")
   end

project "lib_usb_test"
   kind "ConsoleApp"
   language "C++"
   targetdir "build/%{cfg.buildcfg}"
   objdir "build/obj/%{cfg.buildcfg}"

   files { "listdevs.c" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

    -- 2. Dynamically resolve the installed package architecture path
    filter "platforms:x64"
        architecture "x86_64"
        
        -- Adjust triplet folders based on OS ("x64-windows", "x64-linux", "x64-osx")

        filter "system:windows"
            local triplet = "x64-windows"
            includedirs { vcpkg_root .. "/installed/" .. triplet .. "/include/libusb-1.0/" }
            libdirs     { vcpkg_root .. "/installed/" .. triplet .. "/lib" }
            links { "usb-1.0", "udev"}

        filter "configurations:Debug"
            libdirs { vcpkg_root .. "/installed/" .. triplet .. "/debug/lib" }
        filter "system:linux"
            local triplet = "x64-linux"
            includedirs { vcpkg_root .. "/installed/" .. triplet .. "/include/libusb-1.0/" }
            libdirs     { vcpkg_root .. "/installed/" .. triplet .. "/lib" }
            links { "usb-1.0", "udev"}
