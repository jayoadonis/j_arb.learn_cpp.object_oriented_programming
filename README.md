# j_arb.learn_cpp.object_oriented_programming

### Disclaimer
> All tredemarks, assets, and third-party libraries belong to their respective owners. This project is independent and is neither sponsored by nor affiliated with any external tools or companies, and is intended strictly for educational use.

## Table of Content
+ [TITLE](#j_arblearn_cppobject_oriented_programming)
+ [Layout](#layout-toc)
+ [CLI](#cli-toc)
    - [1. Happy Path](#a-happy-path-toc)
        - [1.1. With test](#11-with-test)
        - [1.2. Without test](#12-without-test)
        - [1.3. Packaging](#13-packaging)


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