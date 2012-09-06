solution "minui3"
    configurations { "Debug", "Release" }
    language "C++"

    includedirs
    {
        "include",
        "deps/ucdata",
        "deps/zlib",
        "deps/expat/lib",
        "deps/tidy",
        "src/Stream/Zip"
    }

    defines { "_MINUI3_", "HAVE_BCOPY" }

    configuration { "Debug*" }
        defines { "_DEBUG", "DEBUG" }
        flags   { "Symbols" }
        targetdir "bin/debug"

    configuration { "Release*" }
        defines { "NDEBUG" }
        flags   { "Optimize" }
        targetdir "bin/release"


    if _ACTION == "clean" then
        os.rmdir("bin")
    end

    -- put your projects here
    project "minui_test"
        kind     "ConsoleApp"

        files
        {
          "scratchpads/minui_test.cpp"
        }

        links
        {
            "minui3"
        }

    project "minui3"
        kind     "SharedLib"


        if os.is("linux") then
            links
            {
                "z",
                "curl"
            }
        else
            links
            {
                "z",
                "iconv",
                "curl"
            }
        end


        files
        {
            "deps/ucdata/ucdata.cpp",
            "deps/ucdata/ucpgba.c",

            "deps/expat/lib/*.c",

            "deps/zlib/adler32.c",
            "deps/zlib/crc32.c",
            "deps/zlib/gzio.c",
            --"deps/zlib/gzread.c",
            "deps/zlib/infback.c",
            "deps/zlib/inflate.c",
            "deps/zlib/uncompr.c",
            "deps/zlib/compress.c",
            "deps/zlib/deflate.c",
            --"deps/zlib/gzclose.c",
            --"deps/zlib/gzlib.c",
            --"deps/zlib/gzwrite.c",
            "deps/zlib/inffast.c",
            "deps/zlib/inftrees.c",
            "deps/zlib/trees.c",
            "deps/zlib/zutil.c"

        }


        files
        {
            "src/Application/nglConsole.cpp",
            "src/Application/nglCPUInfo_x86.cpp",
            "src/Application/nglCPUInfo.cpp",
            "src/Application/nglKernel.cpp",
            "src/Application/nglLog.cpp",
            "src/Application/nglMimeSource.cpp",
            "src/Application/nglModule.cpp",
            "src/Application/nglModule.cpp",
            -- "src/Application/nuiLocale.cpp",

            "src/String/nglString.cpp",
            "src/String/nuiRegExp.cpp",
            "src/String/nuiTranslator.cpp",
            "src/String/ConvertUTF.cpp",
            "src/String/nglStringConv_iconv.cpp",
            "src/String/nglUTFStringConv.cpp",
            "src/String/nuiUnicode.cpp",
            "src/String/nuiStringTemplate.cpp",

            "src/Json/*.cpp",

            "src/Stream/*.cpp",
            "src/Stream/Zip/*.c",

            "src/Time/*.cpp",
            "src/File/*.cpp",
            "src/Math/*.cpp",
            "src/Net/nuiHTTP.cpp",
            "src/Net/nuiHTTPServer.cpp",
            "src/Net/nuiMimeMultiPart.cpp",
            "src/Net/nuiNetworkHost.cpp",
            "src/Net/nuiSocket.cpp",
            "src/Net/nuiTCPClient.cpp",
            "src/Net/nuiTCPServer.cpp",

            "src/Attributes/nuiAttribute.cpp",

            "src/Base/nuiAnimation.cpp",
            "src/Base/nuiEvent.cpp",
            "src/Base/nuiCommand.cpp",
            "src/Base/nuiCommandContainer.cpp",
            "src/Base/nuiCommandManager.cpp",
            "src/Base/nuiObject.cpp",
            --"src/Base/nuiHTML.cpp",
            "src/Base/nuiInit.cpp",
            "src/Base/nuiMessageQueue.cpp",
            "src/Base/nuiMemoryPool.cpp",
            "src/Base/nuiNotification.cpp",
            "src/Base/nuiSignalsSlots.cpp",
            "src/Base/nuiTask.cpp",
            "src/Base/nuiTimer.cpp",
            "src/Base/nuiToken.cpp",
            "src/Base/nuiTree.cpp",
            "src/Base/nuiXML.cpp",
            "src/Base/nuiRange.cpp",
            "src/Base/nuiNativeResource.cpp",

            "src/Threading/*.cpp",

            "src/Utils/nuiCSV.cpp",
            "src/Utils/nuiParser.cpp",
            --"src/Utils/nuiRSS.cpp",
            "src/Utils/nuiStopWatch.cpp",
            "src/Utils/nglError.cpp"

        }

        if os.is("linux") then
            defines { "_LINUX_" }

            files
            {
                "src/Window/Unix/nglEvent_Unix.cpp",
                "src/Application/Unix/nglKernel_Unix.cpp",
                "src/Application/Unix/nglModule_Unix.cpp",
                "src/Application/Unix/nglConsole_Unix.cpp",

                "src/Time/Unix/*.cpp",
                --"src/File/Unix/*.cpp",
                "src/File/Unix/nglPath_Unix.cpp",
                "src/Net/Unix/*.cpp",
                "src/Threading/posix/*.cpp"
            }
        else
            files
            {
                "src/Window/Unix/nglEvent_Unix.cpp",
                "src/Application/Unix/nglKernel_Unix.cpp",
                "src/Application/Unix/nglModule_Unix.cpp",
                "src/Application/Unix/nglConsole_Unix.cpp",

                "src/Time/Unix/*.cpp",
                --"src/File/Unix/*.cpp",
                "src/File/Unix/nglPath_Unix.cpp",
                "src/Net/Unix/*.cpp",
                "src/Threading/posix/*.cpp"
            }
        end


