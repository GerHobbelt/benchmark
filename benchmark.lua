project "benchmark"

dofile(_BUILD_DIR .. "/static_library.lua")

configuration { "*" }

uuid "A83E4955-539D-4F1C-AC26-35DB8047C142"

includedirs {
  ".",
  "include",
}

files {
  "include/benchmark/**.h",
  "src/**",
}

excludes {
  "src/benchmark_main.cc",
}

if (_PLATFORM_ANDROID) then
end

if (_PLATFORM_COCOA) then
end

if (_PLATFORM_IOS) then
end

if (_PLATFORM_LINUX) then
end

if (_PLATFORM_MACOS) then
end

if (_PLATFORM_WINDOWS) then
end

if (_PLATFORM_WINUWP) then
end
