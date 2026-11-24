# j_arb.learn_cpp.object_oriented_programming

### Disclaimer
> All tredemarks, assets, and third-party libraries belong to their respective owners. This project is independent and is neither sponsored by nor affiliated with any external tools or companies, and is intended strictly for educational use.

## Table of Content
+ [TITLE](#j_arblearn_cppobject_oriented_programming)
+ [Prerequisite](#prerequisite-toc)
+ [Layout](#layout-toc)
+ [CLI](#cli-toc)
    - [1. Happy Path](#1-happy-path-toc)
        - [1.1. With test](#11-with-test-default)
        - [1.2. Without test](#12-without-test-explicit)
        - [1.3. Packaging](#13-packaging)
        - [1.4. Configure with `--fresh` and `skip redownload for fetchcontent`](#14-configure-with---fresh-and-skip-downloading-content-of-fetchcontent-toc)
        - [1.5. With Sanitizers](#15-with-sanitizers)
        - [1.6. Conan + Explicit toolchain file](#16-conan--explicit-toolchain-file-toc)
        - [1.6.1. Conan + Preset](#161-conan--preset-toc)
        - [1.6.2. Conan + Preset; LLVM Clang | MSYS64 Clang64](#162-conan--preset-llvm-clang--msys64-clang64-toc)
        - [1.6.3. Conan + Preset; MinGW | MSYS64 ucrt64](#163-conan--preset-mingw--msys64-ucrt64-toc)
    - [2. Flappy Path](#2-flappy-path)
        - [2.1. Try `git worktree` (comsumer)](#21-try-git-worktree-consumer)
        - [2.1.1. `git-worktree` Layout](#211-git-worktree-layout)
        - [2.1.2. `Create-From-Scratch` (producer)](#212-create-from-scratch-producer-toc)
+ [oop](#)
    - [encapsulation](#) - concrete class
    - [inheritance](#) - concrete class inheritance (without overriding)
    - [abstraction](#) - abstract class & interface (hidden logic and with overriding)
    - [polymorphism](#) - two or more abstract class & interfaces implementations

## Prerequisite [ToC](#table-of-content)
+ C++ std 17 (Recommended MSVC, Clang-CL, GNU Clang)
+ CMake v3.28+
+ Visual Studio 17 2022 v14.4+ or Ninja v1.10+
+ Conan 2+

### `NOTE:` On Windows, CMake enforces a legacy CMAKE_OBJECT_PATH_MAX limit of ~259 characters (MAX_PATH) for generated object file paths, which can cause build failures in deeply nested directory structures. Solutions;
+ **Option A.** Shorten `target`/`folder` names; Reduce deeply nested source directories and verbose CMake target names, which get duplicated in object paths.
+ **Option B.** Increase CMake's path limit & Enable Windows Long Paths; Override `set(CMAKE_OBJECT_PATH_MAX 1024) or more` at the top of your CMakeLists.txt. And then set LongPathEnabled to 1 in the Windows Registry (`HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\FileSystem`). 

## Layout [^ToC](#table-of-content)
```text
j_arb.learn_cpp.object_oriented_programming/
|--- .gitattributes
|--- .gitignore
|--- README.md
|--- conanfile.py
|--- CMakeLists.txt
|--- cmake/
|--- conan/
|   \--- profile/
|       |--- native/
|       |   |--- x86_64-windows-vs-msvc.ini [/]
|       |   |--- x86-windows-vs-msvc.ini    [/]
|       |   |--- [ x86_64-windows-vs-clang_cl.ini ]
|       |   |--- [ x86_64-windows-vs-clang.ini ]
|       |   |--- [ x86_64-windows-vs-gcc.ini ]
|       |   |--- x86_64-windows-ninja_mc-clang_cl.ini
|       |   |--- x86_64-windows-ninja-clang_cl.ini
|       |   |--- x86_64-windows-ninja_mc-clang.ini  [/]
|       |   |--- x86_64-windows-ninja-clang.ini     [/]
|       |   |--- x86_64-windows-makefiles-clang.ini
|       |   |--- x86_64-linux-ninja_mc-clang.ini
|       |   |--- x86_64-linux-ninja-clang.ini
|       |   |--- x86_64-linux-makefiles-clang.ini
|       |   |--- armv7-rasp-makefiles-clang.ini
|       |   |--- armv7-rasp-ninja-clang.ini
|       |   \--- armv7-rasp-ninja_mc-clang.ini
|       \--- cross/
|           |--- x86_64-windows-cross-x86_64-linux-ninja-clang.ini
|           |--- x86_64-windows-cross-x86_64-linux-ninja_mc-clang.ini
|           |--- x86_64-linux-cross-x86_64-windows-ninja-clang.ini
|           |--- x86_64-linux-cross-x86_64-windows-ninja_mc-clang.ini
|           |--- x86_64-linux-cross-armv7-rasp-makefiles-clang.ini
|           |--- x86_64-linux-cross-armv7-rasp-ninja-clang.ini
|           \--- x86_64-linux-cross-armv7-rasp-ninja_mc-clang.ini
|--- out/
|   |--- build/
|   |--- install/
|   \--- dist/
\--- src/
    |--- main/
    |   |--- cpp/
    |   |   |--- CMakeLists.txt
    |   |   |--- private/
    |   |   |   \--- j_arb/learn_cpp/object_oriented_programming/
    |   |   |       |--- main.{cc,cxx,cpp}
    |   |   |       |--- sanitize_hook.{cc,cxx,cpp}
    |   |   |       \--- **/*.{cc,cxx,cpp,h,hxx,hpp,tpp,ipp,inl}
    |   |   \--- public/
    |   |       \--- j_arb/learn_cpp/object_oriented_programming/
    |   |           |--- export.{h,hxx,hpp}
    |   |           \--- **/*.{h,hxx,hpp,tpp,ipp,inl}
    |   \--- resources/
    |       \--- j_arb/learn_cpp/object_oriented_programming/
    |           |--- .env.example 
    |           |--- .env.development 
    |           \--- .env.production
    \--- test/
        |--- cpp/
        |   |--- CMakeLists.txt
        |   \--- private/
        |       \--- j_arb/learn_cpp/object_oriented_programming/test/
        |           |--- main.{cc,cxx,cpp}
        |           |--- unit/**/*.{cc,cxx,cpp}
        |           \--- integration/**/*.{cc,cxx,cpp}
        \--- resources/
            \--- j_arb/learn_cpp/object_oriented_programming/test/
                |--- .env.example 
                |--- .env.development 
                \--- .env.production 
```

## CLI [^ToC](#table-of-content)
### 1. Happy Path [^ToC](#table-of-content)

#### 1.1. With test (Default)

```ps
#REM: Multi-Configuration
cmake -S . -B out/build -G "Visual Studio 17 2022"
#REM: or
cmake -S . -B out/build -G "Ninja Multi-Config"

#REM: Build | Compiled
cmake --build out/build --config (Debug|Release)

#REM: CTest both unit | integration
ctest --test-dir out/build -C (Debug|Release) -VV
```

#### 1.2. Without test (explicit)
```ps
cmake -S . -B out/build `
    -G "Visual Studio 17 2022"
    -DOBJECT_ORIENTED_PROGRAMMING_BUILD_TESTING=OFF
```

#### 1.3. Packaging
```ps
#REM: CPack Packaging | Distributing (ZIP|TGZ) | core library
cpack --config out/build/CPackConfig.cmake `
    -B out/dist `
    -G "ZIP;TGZ"
    -C (Debug|Release)
    "-DCPACK_COMPONENTS_ALL=core;core_runtime_deps;resources"
```

#### 1.4. Configure with `--fresh` and skip downloading content of fetchcontent [^ToC](#table-of-content)
```ps
cmake -S . -B out/build `
    --fresh `
    -DFETCHCONTENT_FULLY_DISCONNECTED=ON
```

#### 1.5. With sanitizers
```ps
#REM: MSVC (built-in ASan only)
cmake -S . -B out/build `
    -G "Visual Studio 17 2022" `
    -DOBJECT_ORIENTED_PROGRAMMING_SAN_MODE=address 

#REM: Clang64 (Both ASan & UBSan) [MSYS2 UCRT64]
cmake -S . -B out/build `
    -G "Ninja Multi-Config" `
    -DCMAKE_CXX_COMPILER=clang++ `
    "-DOBJECT_ORIENTED_PROGRAMMING_SAN_MODE=address;undefined"
```

#### 1.6. Conan + Explicit Toolchain file [ToC](#table-of-content)
```ps
conan install . `
    --build missing `
    -pr conan/profile/native/x86_64-windows-vs-msvc.ini `
    -s build_type=Debug

cmake -S . -B out/build/x86_64-windows-vs-msvc `
    -G "Visual Studio 17 2022"
    -DCMAKE_TOOLCHAIN_FILE="./out/build/x86_64-windows-vs-msvc/generators/conan_toolchain.cmake"

cmake --build out/bulid/x86_64-windows-vs-msvc `
    --config Debug

ctest --test-dir out/build/x86_64-windows-vs-msvc `
    -C Debug -V [-LE "^\.unit"]
```

#### 1.6.1. Conan + Preset [ToC](#table-of-content)
```ps
conan install . `
    --build missing `
    -pr conan/profile/native/x86_64-windows-vs-msvc.ini `
    -s build_type=Debug

cmake --preset conan-x86_64-windwos-vs-msvc

cmake --build `
    --preset conan-x86_64-windows-vs-msvc-debug

ctest --preset conan-x86_64-windows-vs-msvc-debug `
    -V [-LE "^\.unit"]
```

#### 1.6.2. Conan + Preset; LLVM Clang | MSYS64 Clang64 [ToC](#table-of-content)
> NOTE: Strict toolchain root directory. See: [`x86_64-win-llvm_ucrt-ninja_mc.ini`](./conan/profile/universal/x86_64-win-llvm_ucrt-ninja_mc.ini#L13-L25)
```ps
conan install . `
    --build missing `
    -pr conan/profile/native/x86_64-win-llvm_ucrt-ninja_mc.ini `
    -s build_type=RelWithDebInfo
```

#### 1.6.3. Conan + Preset; MinGW | MSYS64 ucrt64 [ToC](#table-of-content)
> NOTE: Strict toolchain root directory. See: [`x86_64-win-gnu_ucrt-ninja_mc.ini`](./conan/profile/universal/x86_64-win-gnu_ucrt-ninja_mc.ini#L18-L24)
```ps
conan install . `
    --build missing `
    -pr conan/profile/native/x86_64-win-gnu_ucrt-ninja_mc.ini `
    -s build_type=RelWithDebInfo
```

### 2. Flappy Path [^ToC](#table-of-content)
#### 2.1. Try `git-worktree` (consumer)
```ps
mkdir j_arb.learn_cpp.object_oriented_programming

cd j_arb.learn_cpp.object_oriented_programming

git clone --bare `
    https://github.com/jayoadonis/j_arb.learn_cpp.object_oriented_programming.git `
    .bare/

set-content .git -value "gitdir: ./.bare" `
    -nonewline `
    -encoding ascii

#REM: To avoid worktree collisions
git -C .bare/ `
    config remote.origin.fetch '+refs/heads/*:refs/remotes/origin/*'

git worktree add wt_main/ main

cd wt_main/

#REM: create new worktree
cd ../
git worktree add wt_new_branch/ -b new_branch

#REM: rename worktree and corresponding branch
git worktree move wt_new_branch wt_rename_branch
cd ../wt_rename_branch
git branch -m rename_branch

#REM: remove worktree and corresponding branch
cd ../
git worktree remove wt_rename_branch
git branch -d rename_branch
```
#### 2.1.1 `git-worktree` Layout
```sh
j_arb.learn_cpp.object_oriented_programming/
|--- .bare/             #REM: Contain index/meta-data and more
|--- .git/              #REM: (Hidden) point to `.bare/`
|--- wt_main            #REM: main branch
\--- wt_other_branch    #REM: main branch
```
#### 2.1.2. `Create-From-Scratch` (producer) [ToC](#table-of-content)
```ps
mkdir -p j_arb.learn_cpp.object_oriented_programming
cd j_arb.learn_cpp.object_oriented_programming

git init --bare -b main .bare/

set-content -path .git `
    -value "gitdir: ./.bare" `
    -nonewline `
    -encoding ASCII

git -C .bare/ `
    remote add origin <remote_url>

git -C .bare/ `
    config remote.origin.fetch "+refs/heads/*:refs/remotes/origin/*"

git worktree add wt_main/ -b main --orphan 
cd wt_main

new-item README.md -itemtype file
git add README.md
git commit -m "[init]"
git push -u origin main

cd ..
git worktree add wt_new_branch/ -b new_branch
cd wt_new_branch/
git push -u origin new_branch

cd ..
git worktree move wt_new_branch wt_rename_branch
cd wt_rename_branch
git branch -m rename_branch

cd ..
git worktree remove wt_rename_branch
git branch -d rename_branch
```