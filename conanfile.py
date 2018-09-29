from conans import ConanFile, CMake, tools
import shutil
import os


class GoogleBenchmarkConan(ConanFile):
    name = "benchmark"
    description = "A microbenchmark support library."
    url = "https://github.com/google/benchmark"
    homepage = "https://github.com/google/benchmark"
    author = "Google Inc."
    license = "Apache-2.0"
    settings = "arch", "build_type", "compiler", "os"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "enable_lto": [True, False],
        "enable_exceptions": [True, False],
        "enable_testing": [True, False],
        "enable_gtest_tests": [True, False]
    }
    default_options = "shared=False", "fPIC=True", "enable_lto=False", "enable_exceptions=True", "enable_testing=False", "enable_gtest_tests=False"
    exports_sources = ["*"]
    generators = "cmake"

    build_subfolder = "."

    def source(self):
        """Wrap the original CMake file to call conan_basic_setup
        """
        shutil.move("CMakeLists.txt", "CMakeListsOriginal.txt")
        shutil.move(os.path.join("conan", "CMakeLists.txt"), "CMakeLists.txt")

    def config_options(self):
        if self.settings.os == "Windows":
            if self.settings.compiler == "Visual Studio" and float(self.settings.compiler.version.value) <= 12:
                raise Exception("{} {} does not support Visual Studio <= 12".format(self.name, self.version))
            del self.options.fPIC

    def configure(self):
        if self.settings.os == "Windows" and self.options.shared:
            raise Exception("Windows shared builds are not supported right now, see issue #639")

        if self.options.enable_testing == False:
            self.options.enable_gtest_tests = False

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions['BENCHMARK_ENABLE_TESTING'] = "ON" if self.options.enable_testing else "OFF"
        cmake.definitions['BENCHMARK_ENABLE_GTEST_TESTS'] = "ON" if self.options.enable_gtest_tests and self.options.enable_testing else "OFF"
        cmake.definitions["BENCHMARK_ENABLE_LTO"] = "ON" if self.options.enable_lto else "OFF"
        cmake.definitions["BENCHMARK_ENABLE_EXCEPTIONS"] = "ON" if self.options.enable_exceptions else "OFF"

        # See https://github.com/google/benchmark/pull/638 for Windows 32 build explanation
        if self.settings.os != "Windows":
            cmake.definitions["BENCHMARK_BUILD_32_BITS"] = "ON" if "64" not in str(self.settings.arch) else "OFF"
            cmake.definitions["BENCHMARK_USE_LIBCXX"] = "ON" if (str(self.settings.compiler.libcxx) == "libc++") else "OFF"
        else:
            cmake.definitions["BENCHMARK_USE_LIBCXX"] = "OFF"

        cmake.configure(build_folder=self.build_subfolder)
        return cmake

    def build_requirements(self):
        if self.options.enable_gtest_tests:
            self.build_requires("gtest/1.8.0@bincrafters/stable")

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

        self.copy(pattern="LICENSE", dst="licenses")

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
        if self.settings.os == "Linux":
            self.cpp_info.libs.extend(["pthread", "rt"])
        elif self.settings.os == "Windows":
            self.cpp_info.libs.append("shlwapi")
        elif self.settings.os == "SunOS":
            self.cpp.info.libs.append("kstat")

    def package_id(self):
        # Development options don't change the binary output
        # from a user point of view; overwrite id information
        # so that the package_id is the same
        # if only the development options are different
        self.info.options.enable_testing = "dev-option"
        self.info.options.enable_gtest_tests = "dev-option"
