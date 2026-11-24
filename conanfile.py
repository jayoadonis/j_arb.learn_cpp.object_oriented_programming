import os
import re
from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.files import copy


class ObjectOrientedProgramming(ConanFile):
    name = "object_oriented_programming"
    version = "0.1.0"
    settings = "os", "arch", "compiler", "build_type"
    package_type = "library"

    _CMAKE_PROJECT_OPTION_PREFIX = name.upper()

    _PROJECT_OUTPUT_NAME = name.lower()

    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "header_only": [True, False],
        "with_app": [True, False],
        "warnings_as_errors": [True, False],
        "san_mode": ["ANY"],
        "san_recover": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "header_only": False,
        "with_app": False,
        "warnings_as_errors": False,
        "san_mode": "none",
        "san_recover": False,
    }

    exports_sources = (
        "CMakeLists.txt",
        "cmake/*",
        "src/main/*",
        "src/test/*",
    )

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        # REM: HHO and shared are mutually exclusive.
        if self.options.header_only:
            if self.options.get_safe("shared"):
                self.output.warning(
                    "shared=True ignored: header_only=True produces an "
                    "INTERFACE (non-compiled) package."
                )
            self.options.rm_safe("shared")
            self.options.rm_safe("fPIC")
        elif self.options.shared:
            self.options.rm_safe("fPIC")

        san_raw = str(self.options.san_mode).strip()
        if san_raw and san_raw != "none":
            allowed = {"address", "undefined", "thread", "memory", "leak"}

            # REM: Normalize delimiters and split into clean, unique names
            normalized = san_raw.replace(",", ";")
            unique_sanitizers = set()

            for item in normalized.split(";"):
                name = item.strip()
                if name:
                    unique_sanitizers.add(name)

            # REM: Validate against allowed options
            invalid = unique_sanitizers - allowed
            if invalid:
                bad_list = ", ".join(sorted(invalid))
                raise ConanInvalidConfiguration(f"Invalid san_mode values: {bad_list}")

            # REM: Canonicalize the option string so Conan generates a consistent Package ID
            self.options.san_mode = ";".join(sorted(unique_sanitizers))

    def _normalized(self, value):
        return re.sub(r"[ -]", "_", str(value).strip().lower())

    def layout(self):

        generator = self._normalized(
            self.conf.get("tools.cmake.cmaketoolchain:generator", default="default")
        )
        compiler = self._normalized(self.settings.get_safe("compiler") or "unknown")
        os_name = self._normalized(self.settings.get_safe("os") or "unknown")
        arch = self._normalized(self.settings.get_safe("arch") or "unknown")

        default_folder = f"out/build/{arch}-{os_name}-{generator}-{compiler}"
        build_folder = self.conf.get(
            "tools.cmake.cmake_layout:build_folder",
            default=default_folder,
        )
        cmake_layout(self, build_folder=build_folder.lower())

    def generate(self):
        deps = CMakeDeps(self)
        tc = CMakeToolchain(self)
        # user_toolchain_path = os.path.join(
        #     self.recipe_folder,
        #     "cmake",
        #     "user_toolchain",
        #     "obj_path_max_win_toolchain.cmake",
        # )
        # tc.user_toolchains = [user_toolchain_path]

        # tc.variables["CMAKE_OBJECT_PATH_MAX"] = 512

        skip_test = self.conf.get(
            "tools.build:skip_test", default=False, check_type=bool
        )
        p = self._CMAKE_PROJECT_OPTION_PREFIX

        # REM: Map Conan options -> your existing CMake option cache variables.
        tc.variables[f"{p}_BUILD_APP"] = bool(self.options.with_app)
        tc.variables[f"{p}_BUILD_HHO"] = bool(self.options.header_only)
        tc.variables[f"{p}_BUILD_SHARED"] = bool(self.options.get_safe("shared", False))
        tc.variables[f"{p}_BUILD_WAE"] = bool(self.options.warnings_as_errors)
        tc.variables[f"{p}_BUILD_TESTING"] = not skip_test
        tc.variables[f"{p}_PACKAGE"] = True  # REM: required for cmake.install()

        tc.variables[f"{p}_SAN_MODE"] = str(self.options.san_mode)
        tc.variables[f"{p}_SAN_RECOVER"] = bool(self.options.san_recover)
        tc.generate()

        deps.generate()
        tc.generate()

    def requirements(self):
        pass

    def build_requirements(self):
        skip_test = self.conf.get(
            "tools.build:skip_test", default=False, check_type=bool
        )
        if skip_test:
            return
        self.test_requires("catch2/3.16.0")

    def build(self):
        skip_test = self.conf.get(
            "tools.build:skip_test", default=False, check_type=bool
        )

        cmake = CMake(self)
        cmake.configure()  # REM: always configure: package() needs CMakeCache

        is_pure_hho = (
            self.options.header_only and not self.options.with_app and skip_test
        )
        if is_pure_hho:
            return  # REM: nothing to compile

        cmake.build()
        if not skip_test:
            cmake.test()

    def package_id(self):
        # REM: WAE only changes compile diagnostics, not binary content.
        self.info.options.rm_safe("warnings_as_errors")
        # REM: HHO has no build-time variability - collapse to a single package id.
        if self.info.options.header_only:
            self.info.clear()

    def package(self):
        # REM: install() rules in CMakeLists dispatch per flavor AND ship the
        # REM: export targets + config files for plain-CMake consumers:
        # REM:   STATIC/SHARED -> lib + public headers + cmake/*.cmake
        # REM:                    + (bin/ on Win .dll) + share/ resources
        # REM:   HHO           -> include/ (public headers) + src/ (private .cpp
        # REM:                    + private headers) + cmake/*.cmake
        # REM:                    + share/ resources
        # REM:   with_app      -> bin/*_app
        cmake = CMake(self)
        cmake.install()

        copy(
            self,
            "LICENSE*",
            src=self.source_folder,
            dst=os.path.join(self.package_folder, "licenses"),
        )

    def package_info(self):
        # REM: `cmake_file_name` / `cmake_target_name` drive CMakeDeps on the
        # REM: consumer side (find_package + target_link_libraries). The shipped
        # REM: `*-config.cmake` files in the package are an ADDITIONAL fallback
        # REM: for plain-CMake consumers who put the package folder on
        # REM: CMAKE_PREFIX_PATH - no `cmake_find_mode` override needed.
        self.cpp_info.set_property(
            "cmake_file_name", f"j_arb.learn_cpp.{_PROJECT_OUTPUT_NAME}"
        )
        self.cpp_info.set_property(
            "cmake_target_name", f"j_arb.learn_cpp::{_PROJECT_OUTPUT_NAME}"
        )

        if self.options.header_only:
            self.cpp_info.set_property("header_only", True)
            self.cpp_info.libs = []
            # REM: Hybrid: headers AND implementation sources are shipped.
            self.cpp_info.includedirs = ["include", "src"]
            self.cpp_info.resdirs = ["share"]
            return

        # REM: STATIC / SHARED: canonical, version-agnostic basename.
        # REM: Must equal OUTPUT_NAME set under CONAN_TOOLCHAIN_INCLUDED below.
        self.cpp_info.libs = [self._PROJECT_OUTPUT_NAME]
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.resdirs = ["share"]

        if self.settings.os == "Windows" and self.options.shared:
            self.cpp_info.bindirs = ["bin"]

    def parse_sanitizers(san_raw, allowed):
        if not san_raw:
            return set()

        # REM: Split on commas or semicolons into a clean list
        raw_items = re.split(r"[,;]", san_raw)

        selected = set()
        for item in raw_items:
            clean_item = item.strip()
            if not clean_item:
                continue

            if clean_item not in allowed:
                raise ConanInvalidConfiguration(
                    f"Invalid sanitizer '{clean_item}'. Allowed: {sorted(allowed)}"
                )

            selected.add(clean_item)

        return selected
