from conans import ConanFile, CMake, tools


class NinGiTidy(ConanFile):

    name = "ningitidy"
    version = "0.1"
    license = "Apache 2.0"
    description = "Accelerate Clang-Tidy using Git and Ninja"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"

    def source(self):
        pass

    def requirements(self):
        self.requires("fmt/7.1.3")
        self.requires("nlohmann_json/3.9.1")
        self.requires("frozen/1.0.1")
        self.requires("abseil/20210324.1")
        # enable when they build on Debian
        #self.requires("protobuf/3.15.5")
        #self.requires("grpc/1.37.1")

    def configure(self):
        tools.check_min_cppstd(self, "20")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

    def package(self):
        pass

    def package_info(self):
        pass
