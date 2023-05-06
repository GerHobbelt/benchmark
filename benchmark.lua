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

if (_PLATFORM_ANDROID or _PLATFORM_IOS or _PLATFORM_WINUWP) then
  return
end

defines {
  "BENCHMARK_STATIC_DEFINE",
  "HAVE_STD_REGEX",
  "HAVE_STEADY_CLOCK",
}

if (_PLATFORM_LINUX) then
  defines {
    "BENCHMARK_HAS_PTHREAD_AFFINITY",
    "HAVE_POSIX_REGEX",
    "HAVE_PTHREAD_AFFINITY",
    "HAVE_THREAD_SAFETY_ATTRIBUTES",
  }
end

if (_PLATFORM_MACOS) then
  defines {
    "HAVE_POSIX_REGEX",
    "HAVE_THREAD_SAFETY_ATTRIBUTES",
  }
end

if (_PLATFORM_WINDOWS) then
end
