-- Project file for testing XPLOsion on Linux
-- (C) 2018 Nicolaus Anderson

local src_dir = "XPLOsion"
local utils_dir = "utilities"
local reader_dir = "../irrvFreader/irrvFreader"
local irrlicht_dir = "/usr/local/include/irrlicht"
local irrlicht_src_dir = "../../irrlicht-trunk-5104/source/Irrlicht"

workspace "debug"
	location "build"
	targetdir "bin"
	kind "ConsoleApp"
	configurations { "default" }

project "debug_xplo"
	buildoptions {
		"-Wall"
	}
	includedirs {
		src_dir,
		utils_dir,
		reader_dir,
		irrlicht_dir,
		irrlicht_src_dir,
	}
	files {
		"XPLOsion/*.h",
		"XPLOsion/*.cpp",
		"irrVFreader.cpp",
		"CAttributes.h",
		-- irrlicht_src_dir .. "CAttributes.cpp"
	}
	excludes {
		"XPLOsion/divClass.*"
	}
	links {
		"X11",
		"Irrlicht"
	}
	targetname "debug.out"
