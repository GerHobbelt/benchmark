#!/usr/bin/env python
# -*- coding: utf-8 -*-
from cpt.packager import ConanMultiPackager
import os

def set_appveyor_environment():
    if os.getenv("APPVEYOR") is not None:
        os.environ["CONAN_STABLE_BRANCH_PATTERN"] = os.getenv("CONAN_STABLE_BRANCH_PATTERN", "master")
        os.environ["CONAN_ARCHS"] = os.getenv("CONAN_ARCHS", "x86,x86_64")
        os.environ["CONAN_BUILD_TYPES"] = os.getenv("CONAN_BUILD_TYPES", "Release,Debug").replace('"', '')

if __name__ == "__main__":
    login_username = os.getenv("CONAN_LOGIN_USERNAME")
    username = os.getenv("CONAN_USERNAME")
    reference = os.getenv("CONAN_REFERENCE")
    channel = os.getenv("CONAN_CHANNEL")
    upload = os.getenv("CONAN_UPLOAD")
    stable_branch_pattern = os.getenv("CONAN_STABLE_BRANCH_PATTERN", r"v\d+\.\d+\.\d+.*")
    test_folder = os.getenv("CPT_TEST_FOLDER", os.path.join("conan", "test_package"))
    upload_only_when_stable = os.getenv("CONAN_UPLOAD_ONLY_WHEN_STABLE", True)
    set_appveyor_environment()

    builder = ConanMultiPackager(username=username,
                                 reference=reference,
                                 channel=channel,
                                 login_username=login_username,
                                 upload=upload,
                                 stable_branch_pattern=stable_branch_pattern,
                                 upload_only_when_stable=upload_only_when_stable,
                                 test_folder=test_folder)
    builder.add_common_builds(pure_c=False)
    builder.run()
