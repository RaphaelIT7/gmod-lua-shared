-- Defines which version of the project generator to use, by default
-- (can be overriden by the environment variable PROJECT_GENERATOR_VERSION)
PROJECT_GENERATOR_VERSION = 3

newoption({
    trigger = "gmcommon",
    description = "Sets the path to the garrysmod_common (https://github.com/danielga/garrysmod_common) directory",
    default = "garrysmod_common"
})

local gmcommon = assert(_OPTIONS.gmcommon or os.getenv("GARRYSMOD_COMMON"),
    "you didn't provide a path to your garrysmod_common (https://github.com/danielga/garrysmod_common) directory")
include(gmcommon)
include("overrides.lua")
include("source/Bootil")

local current_dir = _SCRIPT_DIR
CreateWorkspace({name = "lua_shared", abi_compatible = false})
    -- Serverside module (gmsv prefix)
    -- Can define "source_path", where the source files are located
    -- Can define "manual_files", which allows you to manually add files to the project,
    -- instead of automatically including them from the "source_path"
    -- Can also define "abi_compatible", for project specific compatibility
    CreateProject({serverside = true, manual_files = false})
        -- Remove some or all of these includes if they're not needed
        IncludeHelpersExtended()
        --IncludeLuaShared()
        --IncludeSDKEngine()
        --IncludeSDKCommon()
        IncludeBootil()
        IncludeSDKCommon()
        IncludeSDKTier0()
        IncludeSDKTier1()
        --IncludeSDKTier2()
        IncludeSDKTier3()
        --IncludeSDKMathlib()
        --IncludeSDKRaytrace()
        --IncludeSDKBitmap()
        --IncludeSDKVTF()
        --IncludeSteamAPI()
        IncludeDetouring()
        IncludeScanning()

		files({
			[[garrysmod_common\sourcesdk-minimal\tier1\convar.cpp]],
		})

        targetsuffix("")
        libdirs(current_dir .. "/libs")
        filter("system:windows")
        	linkoptions("/NODEFAULTLIB:LIBCMT")
	        links("msvcrt.lib")

            files({"source/win32/*.cpp", "source/win32/*.hpp"})

		filter("system:windows", "platforms:x86_64")
			links({
				"bootil_static_64.lib",
				"lua51_64.lib",
	        })

		filter("system:windows", "platforms:x86")
			links({
				"bootil_static_32.lib",
				"lua51_32.lib",
	        })

		filter({"system:linux", "platforms:x86_64"})
			links("bootil_static_64")
			links("luajit_64")

		filter({"system:linux", "platforms:x86"})
			links("bootil_static_32")
			links("luajit_32")
            linkoptions(current_dir .. "/libs/libluajit_32.a")

		filter("system:linux or macosx")
			files({"source/posix/*.cpp", "source/posix/*.hpp"})
			targetextension(".so")
			links({
				"dl",
				"pthread"
			})