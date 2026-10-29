# j_arb.learn_cpp.object_oriented_programming

### Disclaimer
> All tredemarks, assets, and third-party libraries belong to their respective owners. This project is independent and is neither sponsored by nor affiliated with any external tools or companies, and is intended strictly for educational use.

## Table of Content
+ [TITLE](#j_arblearn_cppobject_oriented_programming)
+ [Layout](#layout-toc)
+ [CLI](#cli-toc)
    - [1. Happy Path](#1-happy-path-toc)
        - [1.1. With test](#11-with-test)
        - [1.2. Without test](#12-without-test)
        - [1.3. Packaging](#13-packaging)
        - [1.4. Configure with `--fresh` and `skip redownload for fetchcontent`](#14-configure-with---fresh-and-skip-redownload-fetchcontent-toc)
    - [2. Flappy Path](#2-flappy-path)
        - [2.1. Try `git worktree` (comsumer)](#21-try-git-worktree-consumer)
        - [2.1.1. `git-worktree` Layout](#211-git-worktree-layout)
+ [oop](#)
    - [encapsulation](#) - private/public attributes and setter/getter
    - [inheritance](#) - concrete inheritance
    - [abstraction](#) - abstract class contracts
    - [polymorphism](#) - all inheritance & interface contracts 


## Layout [^ToC](#table-of-content)
```text
j_arb.learn_cpp.object_oriented_programming/
|--- .gitattributes
|--- .gitignore
|--- README.md
|--- CMakeLists.txt
|--- cmake/
|--- conan/
|   \--- profile/
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
    |   |   |       \--- **/*.{cc,cxx,cpp,h,hxx,hpp,tpp,inl}
    |   |   \--- public/
    |   |       \--- j_arb/learn_cpp/object_oriented_programming/
    |   |           \--- **/*.{h,hxx,hpp,tpp,inl}
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
        |           |--- unit/**/*..{cc,cxx,cpp}
        |           \--- integration/**/*.{cc,cxx,cpp}
        \--- resources/
            \--- j_arb/learn_cpp/object_oriented_programming/test/
                |--- .env.example 
                |--- .env.development 
                \--- .env.production 
```

## CLI [^ToC](#table-of-content)
### 1. Happy Path [^ToC](#table-of-content)

#### 1.1. With test

```ps
#REM: Configure with default generator and definitions
cmake -S . -B out/build

#REM: Build | Compiled
cmake --build out/build --config (Debug|Release)

#REM: CTest both unit | integration
ctest --test-dir out/build -C (Debug|Release) -VV
```

#### 1.2. Without test
```ps
#REM: Configure 
cmake -S . -B out/build `
    -DOBJECT_ORIENTED_PROGRAMMING_BUILD_TESTING=OFF

#REM: Build | Compiled
cmake --build out/build --config (Debug|Release)
```

#### 1.3. Packaging
```ps
#REM: CPack Packaging | Distributing (ZIP|TGZ) | core library
cpack --config out/build/CPackConfig.cmake `
    -B out/dist `
    -G "ZIP;TGZ"
    -C (Debug|Release)
    "-DCPACK_COMPONENTS_ALL=core;core_runtime_dpes;resources"
```

#### 1.4. Configure with `--fresh` and skip redownload fetchcontent [^ToC](#table-of-content)
```ps
cmake -S . -B out/build `
    --fresh `
    -DFETCHCONTENT_FULLY_DISCONNECTED=ON
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